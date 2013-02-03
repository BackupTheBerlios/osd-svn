/**************************************************************************//**
 * @file DCCprotocol.c
 *
 * @brief DCC Protocol Implementation.
 *
 * @note not HW related
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "config.h"
#include "protocol.h"
#include "globals.h"

#ifdef SIMULATION
	#include <stdio.h>
#endif

const uint8_t a8dccbitmask[8] = {
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

// storage for DCC packets
uint8_t a8dccbuffer[8];

uint8_t u8dccbitctr, u8dccbytectr, u8dccchecksum, u8dccexec;	


// entry from protocol selector (time critical)
uint8_t handleDCC(uint8_t u8pulselen)
{
	if (u8pulselen & E_New) {
		u8dccbitctr = 0;
		u8dccbytectr = 0;
		a8dccbuffer[0] = 0;
		u8dccchecksum = 0;
		u8pulselen &= 0x3F;
#ifdef SIMULATION
		printf("\n DCC ");
#endif
	}
	if ((++u8dccbitctr & 1) == 0) {
		if (u8dccbitctr > 2) {
			// check if received bit was a "1"
			if (u8pulselen == E_50) 
				a8dccbuffer[u8dccbytectr] |= a8dccbitmask[(u8dccbitctr-4) >> 1];
			// after 9 bits (8 data + cont/stop bit go to next byte
			if (u8dccbitctr >= 18) {
				u8dccchecksum ^= a8dccbuffer[u8dccbytectr];
				u8dccbitctr = 0;
				if (++u8dccbytectr >= sizeof(a8dccbuffer)) return E_idle;	// error: too long
				a8dccbuffer[u8dccbytectr] = 0;
			}
		}
		// after stop bit the packet has been stored completely
		else if (u8pulselen == E_50) {
#ifdef SIMULATION
			for (int i=0; i<=(u8dccbytectr-1); i++) 
						printf(" %2.2x", a8dccbuffer[i]);
#endif
			if (u8dccchecksum == 0)	u8dccexec = TRUE;
			return E_idle;
		}
	}
	return E_DCC_rec;
}


// entry from scheduling
void executeDCC (void)
{
	if (u8dccexec != TRUE)	return;
	u8dccexec = FALSE;

#ifdef SIMULATION
	int i, bef = 0;

	printf(" Chk OK \n");

	// check address format (broadcast / short / long)
	if (a8dccbuffer[0] == 0) {
		printf(" Broadcast");
		bef = 1;	}
	else if ((a8dccbuffer[0] >= 1) && (a8dccbuffer[0] <= 127)) {
		printf(" Short %3d", a8dccbuffer[0]);
		bef = 1;	}
	else if ((a8dccbuffer[0] >= 192) && (a8dccbuffer[0] <= 231)) {
		printf(" Long %5d", (a8dccbuffer[0] - 192) * 256 + a8dccbuffer[1]);
		bef = 2;	}
	// interpretation of command part
	for (i=bef; i<(u8dccbytectr-1); i++) {
		switch (a8dccbuffer[i] >> 5) {
		case 0: switch (a8dccbuffer[i]) {
				case 0:	printf(" RESET");		break;
				case 1: printf(" HARD RESET");	break;
				}
				break;
		case 1:	if (a8dccbuffer[i+1] & 0x80)
						printf(" ADV-Fwd %d", a8dccbuffer[i+1] & 127);
				else	printf(" ADV-Rev %d", a8dccbuffer[i+1] & 127);
				return;
		case 2:	printf(" S+D-Rev %d", a8dccbuffer[i] & 31);	break;
		case 3:	printf(" S+D-Fwd %d", a8dccbuffer[i] & 31);	break;
		case 4:	printf(" FuGrp1:%x", a8dccbuffer[i] & 31);		break;
		case 5: printf(" FuGrp2:%x", a8dccbuffer[i] & 31);		break;
		case 7: printf(" POM");
				/* dcc_cv_acc(i); */	i += 2;				break;
		default:printf(" ??");
		}	
	}
#else
	// handle the DCC packet for normal operation
#endif
}
