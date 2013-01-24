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

// storage for MM packets
/*union {	uint32_t	u32data;
		struct { uint8_t u8addrbits, u8fubits, u8data, u8xdata; } ;
		}	u_mmpacketnew, u_mmpacketold;

uint8_t u8mmbitnr, u8address;	*/


// entry from protocol selector
uint8_t handleOXP(uint8_t u8pulselen)
{
	if (u8pulselen & E_New) {
//		u_mmpacketnew.u32data = 0;	// start of new packet
//		u8mmbitnr = 0;
//	printf("\n OXP");
	}

	return E_idle;	//E_OXP_rec;
}

// entry from scheduling
void executeOXP (void)
{
//		PROTPRINT(u_mmpacketnew.u8addrbits, u_mmpacketnew.u8data, u_mmpacketnew.u8xdata);
}



//enum E_ProtState { E_idle, E_MM_rec, E_DCC_rec, E_DCC_ans, E_OXP_rec, E_OXP_ans };
//enum E_Pulselength	{ E_25, E_50, E_100, E_200, E_400, E_900, E_New = 128 };

