/**************************************************************************//**
 * @file MMprotocol.c
 *
 * @brief MM Protocol Implementation.
 *
 * @note not HW related
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "config.h"
#include "globals.h"
#include "protocol.h"

#ifdef SIMULATION
	#include <stdio.h>
#endif

// Tables for MM address field decoding
const uint8_t a8mm_adrtab[256] = {
	 80,  2, 81,  1,  6,  8, 82,  7,145,177, 83,161,  3,  5, 84,  4,
	 18, 20, 85, 19, 24, 26, 86, 25,146,178, 87,162, 21, 23, 88, 22,
	193,201, 89,197,217,225, 90,221,147,179, 91,163,205,213, 92,209,
	  9, 11, 93, 10, 15, 17, 94, 16,148,180, 95,164, 12, 14, 96, 13,

	 54, 56, 97, 55, 60, 62, 98, 61,149,181, 99,165, 57, 59,100, 58,
	 72, 74,101, 73, 78,  0,102, 79,150,182,103,166, 75, 77,104, 76,
	194,202,105,198,218,226,106,222,151,183,107,167,206,214,108,210,
	 63, 65,109, 64, 69, 71,110, 70,152,184,111,168, 66, 68,112, 67,

	229,231,113,230,235,237,114,236,153,185,115,169,232,234,116,233,
	247,249,117,255,248,253,118,254,154,186,119,170,250,252,120,251,
	195,203,121,199,219,227,122,223,155,187,123,171,207,215,124,211,
	238,240,125,239,244,246,126,245,156,188,127,172,241,243,128,242, 
	
	 27, 29,129, 28, 33, 35,130, 34,157,189,131,173, 30, 32,132, 31,
	 45, 47,133, 46, 51, 53,134, 52,158,190,135,174, 48, 50,136, 49,
	196,204,137,200,220,228,138,224,159,191,139,175,208,216,140,212,
	 36, 38,141, 37, 42, 44,142, 43,160,192,143,176, 39, 41,144, 40	};

const uint32_t a32mmbitmask[18] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,	// address bits
	0x100, 0x200,									// function bits
	0x10000, 0x1000000, 0x20000, 0x2000000,			// data bits
	0x40000, 0x4000000, 0x80000, 0x8000000 };


// storage for MM packets
// Bits are stored here for later evaluation
// Bit 7 of u8fubits is misused for speed indication (1 = turnout etc)
union {	uint32_t	u32data;
		struct { uint8_t u8addrbits, u8fubits, u8data, u8xdata; } ;
		}	u_mmpacketnew, u_mmpacketold;

uint8_t u8mmbitnr, u8address, u8mmexec;


// entry from protocol selector (time critical)
uint8_t handleMM(uint8_t u8pulselen)
{
	if (u8pulselen & E_New) {
		u_mmpacketnew.u32data = 0;	// start of new packet
		u8mmbitnr = 0;
		u8pulselen &= 0x3F;
	}

	if ((u8mmbitnr & 1) == 0) {
		// Code for one bit received
		if (u8pulselen !=  E_25) u_mmpacketnew.u32data |= a32mmbitmask[u8mmbitnr >> 1];
	}
		// check if double speed MM
	if (u8pulselen == E_100) u_mmpacketnew.u8fubits |= 0x80;

	if (++u8mmbitnr >= 35) {
		// packet completely received, if OK trigger scheduling
		if (u_mmpacketold.u32data == u_mmpacketnew.u32data) u8mmexec = TRUE;
		else u_mmpacketold.u32data = u_mmpacketnew.u32data;
		return E_idle;
	}
	return E_MM_rec;
}

// entry from scheduling
void executeMM (void)
{
	uint8_t	mmaddr;

	if (u8mmexec != TRUE)	return;
	u8mmexec = FALSE;

	mmaddr = a8mm_adrtab[u_mmpacketold.u8addrbits];

#ifdef SIMULATION
	if (u_mmpacketold.u8data == u_mmpacketold.u8xdata) {
		printf("\n MM1 A=%3d, F=%2x, D=%2d ", mmaddr, u_mmpacketold.u8fubits, u_mmpacketold.u8data);	}
	else {
		printf("\n MM2 A=%3d, F=%2x, D=%2d, X=%2d ", mmaddr, u_mmpacketold.u8fubits, u_mmpacketold.u8data, u_mmpacketold.u8xdata);
		switch (u_mmpacketold.u8xdata) {
			case  2:  case 10:	printf("FWD   ");	break;
			case  3:			printf("F1 off");	break;
			case  4:			printf("F2 off");	break;
			case  5:  case 13:	printf("REV   ");	break;
			case  6:			printf("F3 off");	break;
			case  7:			printf("F4 off");	break;
			case 11:			printf("F1 on ");	break;
			case 12:			printf("F2 on ");	break;
			case 14:			printf("F3 on ");	break;
			case 15:			printf("F4 on ");	break;
			}	
	}
#else
	// valid data available for evaluation
	// check if programming => do programming
	// check frequency and address
	// handle first and following assigned MM addresses
	if (mmaddr == a8CVvalues[E_CV_Short_Addr])
		sCommand.u8speed = u_mmpacketold.u8data;
#endif
}
