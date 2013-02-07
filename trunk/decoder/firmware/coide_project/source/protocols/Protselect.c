/**************************************************************************//**
 * @file feedback.c
 *
 * @brief Protocol Selector.
 *
 * @note HW-abhängiges Modul für: Prozessor M0516ZBN
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "config.h"
#include "protocol.h"

#ifdef SIMULATION
	#include <stdio.h>
#endif


uint32_t u32olddur, u32countpreamp;
uint8_t  u8protstate;

void handlechange(uint32_t u32duration)
{
	if (u32duration < 75)	{
		if (u32duration < 40)	{		// 0 to 40us
			switch (u8protstate) {
				case E_idle:	// short pulse after long idle assumed to be MM
								if (u32olddur > 250)
									u8protstate = handleMM(E_25 + E_New);
								break;
				case E_MM_rec:	// continue to receive MM short
								u8protstate = handleMM(E_25);
								break;
				case E_OXP_ans:	// answering clock for OXP
								u8protstate = handleOXP(E_25);
								break;
				default:		// unexpected
								u8protstate = E_idle;
#ifdef SIMULATION
								printf("<E_25>");
#endif
			}
		}
		else {							// 40 to 75us
			switch (u8protstate) {
				case E_idle:	// count changes to detect preamble
								u32countpreamp++;
								break;
				case E_DCC_rec: // continue to receive DCC short
								u8protstate = handleDCC(E_50);
								break;
				case E_OXP_rec:
				case E_OXP_ans:	// continue to receive OXP short
								u8protstate = handleOXP(E_50);
								break;
				default:		// unexpected
								u8protstate = E_idle;
#ifdef SIMULATION
								printf("<E_50>");
#endif
			}
		}
	}
	else {
		switch (u8protstate) {			// longer than 75us
			case E_idle:	// if preamble detected assume DCC
							if (u32countpreamp >= 20) {
								if (u32countpreamp >= 40) 
										u8protstate = handleDCC(E_100 + E_LongPrea + E_New);
								else	u8protstate = handleDCC(E_100 + E_New);
							}
							else {
								if (u32duration < 130) {
									// MM with double speed has been started
									if (u32olddur > 250) u8protstate = handleMM(E_100 + E_New);
									// OXP packet has been started
									else u8protstate = handleOXP(E_100 + E_New);
								}
								else if ((u32duration < 250) && (u32olddur > 250))
									// MM with normal speed has been started
									u8protstate = handleMM(E_200 + E_New);
							}
							break;
			case E_MM_rec:	// continue to receive MM long
							if (u32duration < 130) u8protstate = handleMM(E_100);
							else u8protstate = handleMM(E_200);
							break;
			case E_DCC_rec: // continue to receive DCC short
							u8protstate = handleDCC(E_100);
							break;
			case E_OXP_rec:	// continue to receive OXP long
							u8protstate = handleOXP(E_100);
							break;
			case E_OXP_ans:	// feedback interval for OXP
							if (u32duration < 200) u8protstate = handleOXP(E_100);
							else if (u32duration < 600) u8protstate = handleOXP(E_400);
							else u8protstate = handleOXP(E_900);
							break;
			default:		// unexpected
							u8protstate = E_idle;
#ifdef SIMULATION
							printf("<LONG>");
#endif
		}
		u32countpreamp = 0;
	}
	u32olddur = u32duration;
#ifdef SIMULATION
	// in simu mode we have no time scheduler, so let it handle here
	executeMM();
	executeDCC();
	executeOXP();
#endif
}
