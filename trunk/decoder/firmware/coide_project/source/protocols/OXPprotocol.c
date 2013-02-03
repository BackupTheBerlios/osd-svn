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
#include "protocol.h"
#include "globals.h"

#ifdef SIMULATION
	#include <stdio.h>
#endif

// storage for OXP packets
/*union {	uint32_t	u32data;
		struct { uint8_t u8addrbits, u8fubits, u8data, u8xdata; } ;
		}	u_mmpacketnew, u_mmpacketold;
	*/

uint32_t u32oxpcrc;
uint8_t u8oxpbitstate, u8oxpchanges, u8oxpdataavail, u8oxprecbit, u8oxpnrones, u8oxpnrbits;
uint8_t u8oxpexec;

// entry from protocol selector
uint8_t handleOXP(uint8_t u8pulselen)
{
	if (u8pulselen & E_New) {
		u8oxpchanges = 2;
		u8oxpbitstate = 2;
		u8pulselen &= 0x3F;
#ifdef SIMULATION
		printf("\n OXP ");
#endif
	}
	u8oxpbitstate = ((u8oxpbitstate << 1) + (u8pulselen == E_100 ? 1 : 0)) & 7;
	
#ifdef SIMULATION
	if (u8pulselen > E_100)printf(" %d ", u8pulselen);
#endif

	if (++u8oxpchanges == 3) {
		switch (u8oxpbitstate) {
			case 0: case 1:	// a "one" has been received
							u8oxpchanges = 1;
							u8oxprecbit = 1;
							u8oxpnrones++;
							break;
			case 5:			// a "flag" has been received
							u8oxpchanges = 0;
#ifdef SIMULATION
							printf("<FLAG>");
							if (u8oxpdataavail) {
								printf(" CRC = %x, Bits %d", (u32oxpcrc & 0xFF), u8oxpnrbits);
								printf("\n");
							}
#endif
							u8oxpdataavail = FALSE;
							u8oxpnrbits = 0;
							u32oxpcrc = 127;					// CRC start value
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
#ifdef SIMULATION
		printf("%d", u8oxprecbit);
#endif
		u8oxpdataavail = TRUE;
		u8oxpnrbits++;
		// CRC calculation
		u32oxpcrc = (u32oxpcrc << 1) + u8oxprecbit;
		if (u32oxpcrc & 0x100) u32oxpcrc ^= 7;

	}

	return E_OXP_rec;
}

// entry from scheduling
void executeOXP (void)
{
	if (u8oxpexec != TRUE)	return;
	u8oxpexec = FALSE;
}



//enum E_ProtState { E_idle, E_MM_rec, E_DCC_rec, E_DCC_ans, E_OXP_rec, E_OXP_ans };
//enum E_Pulselength	{ E_25, E_50, E_100, E_200, E_400, E_900, E_New = 128 };

