/**************************************************************************//**
 * @file OXPprotocol.c
 *
 * @brief OXP Protocol Implementation.
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

// storage for OXP packets
uint32_t a32oxpnewdaten[3], a32oxprecdaten[3];

uint32_t u32oxpcrc;
uint8_t u8oxpbitstate, u8oxpchanges, u8oxprecbit, u8oxpnrones, u8oxpnrbits;
uint8_t u8oxpexec, u8oxpanswering ;

// entry from protocol selector
uint8_t handleOXP(uint8_t u8pulselen)
{
	if (u8pulselen & E_New) {
		u8oxpchanges = 2;
		u8oxpbitstate = 2;
		u8pulselen &= 0x3F;
	}
	u8oxpbitstate = ((u8oxpbitstate << 1) + (u8pulselen == E_100 ? 1 : 0)) & 7;
	
#ifdef SIMULATION
	if (u8pulselen > E_100)	// used for RDS readout clock
		printf(" %d ", u8pulselen);
#endif

	if (++u8oxpchanges == 3) {
		switch (u8oxpbitstate) {
			case 0: case 1:	// a "one" has been received
							u8oxpchanges = 1;
							u8oxprecbit = 1;
							a32oxpnewdaten[u8oxpnrbits >> 5] |= (0x80000000 >> (u8oxpnrbits & 31));
							u8oxpnrones++;
							break;
			case 5:			// a "flag" has been received
							u8oxpchanges = 0;
#ifdef SIMULATION
							printf("<FLAG>");
#endif
							if (u8oxpnrbits > 0) {
//								printf(" CRC = %x, Bits %d", (u32oxpcrc & 0xFF), u8oxpnrbits);
//								printf("\n");
								if ((u32oxpcrc & 0xFF) == 0) {
									a32oxprecdaten[0] = a32oxpnewdaten[0];
									a32oxprecdaten[1] = a32oxpnewdaten[1];
									a32oxprecdaten[2] = a32oxpnewdaten[2];
									u8oxpexec = u8oxpnrbits;	// store for background handling
								}
							}
							u8oxpnrbits = 0;					// clear for next packet
							a32oxpnewdaten[0] = 0;
							a32oxpnewdaten[1] = 0;
							a32oxpnewdaten[2] = 0;
							u32oxpcrc = 127;					// CRC start value
							u8oxpanswering = 0;
							return E_OXP_rec;
			default:		// a "zero" has been received
							u8oxpchanges = 2;
							if (u8oxpnrones >= 8) {				// was a "filler zero"
								u8oxpnrones = 0;
								return E_OXP_rec;
							}
							u8oxprecbit = 0;
							u8oxpnrones = 0;
							break;
		}
		// here we have one bit from rail signal
		if (++u8oxpnrbits > 95) return E_idle;		// too many bits received without flag
		if (u8oxpnrbits == 4) {						// check the first four bits
			if ((a32oxpnewdaten[0] & 0xF0000000) == 0x30000000) {
				// activate carrier
#ifdef SIMULATION
				printf(" ** RDS window ** ");
#endif
				u8oxpanswering = 1;
			}
		}
							
		// CRC calculation
		u32oxpcrc = (u32oxpcrc << 1) + u8oxprecbit;
		if (u32oxpcrc & 0x100) u32oxpcrc ^= 7;
	}

	if (u8oxpanswering) return E_OXP_ans;
	else return E_OXP_rec;
}

// entry from scheduling
void executeOXP (void)
{
	if (u8oxpexec == 0)	return;

#ifdef SIMULATION
	printf("\n OXP >");
	for (uint8_t ui=0; ui<u8oxpexec ; ui++) 
		printf("%d", (a32oxprecdaten[ui >> 5] >> (31-(ui & 31))) & 1);

	// quick and dirty decoding only for test!
	switch ((a32oxprecdaten[0] >> 17) & 0x3F) {
		case 0x38:	printf(" READ ");	break;
		case 0x39:	printf(" PROG ");	break;
		case 0x3A:	printf(" SUCH ");	break;
		case 0x3B:	printf(" NADR ");	break;
		case 0x3C:	printf(" PING ");	break;
		case 0x3D:	printf(" BAKE ");	break;
	}
#endif

	u8oxpexec = 0;
}



//enum E_ProtState { E_idle, E_MM_rec, E_DCC_rec, E_DCC_ans, E_OXP_rec, E_OXP_ans };
//enum E_Pulselength	{ E_25, E_50, E_100, E_200, E_400, E_900, E_New = 128 };

