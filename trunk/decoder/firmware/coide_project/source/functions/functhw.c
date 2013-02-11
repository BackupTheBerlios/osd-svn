/**************************************************************************//**
 * @file functhw.c
 *
 * @brief Steuerung der Ports des Prozessors.
 *
 * @note HW-abhängiges Modul für: Prozessor M0516ZBN
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "M051series.h"
#include "config.h"
#include "system.h"
#include "DrvGPIO.h"

// definitions
typedef union {
	uint32_t	w;
	uint8_t		b[4];
}	u_wb4_typ;

// trigger for slow timed actions
volatile char trigger10ms;

// info storage for function outputs
uint32_t u32functionframe[255];
uint32_t u32functionstatic, u32functionmask;

// periodically called, handles all outputs used as GPIO
// simulates PWM by fast switching
// triggers slower actions

void TMR0_IRQHandler(void)
{
    static uint32_t u32fbufptr, u32fbufcntr;
    u_wb4_typ functionact;

			outpw(&P37_DOUT, 1);			// time measurement P3.7 ***********************
    TIMER0->u32TISR = 1;			    // clear request

    // set all port outputs
    functionact.w = (u32functionstatic | u32functionframe[u32fbufptr]) & ~u32functionmask;
	PORT0->DOUT = functionact.b[0];
	PORT1->DOUT = functionact.b[1];
	PORT2->DOUT = functionact.b[2];
	PORT3->DOUT = functionact.b[3];

	if (++u32fbufptr > 254) {			// counts for SW-PWM
		u32fbufptr = 0;
		if (++u32fbufcntr >= 10) {		// counts for servo frame, 10 * 2ms
			u32fbufcntr = 0;
			u32functionmask = 0;
			trigger10ms = 1;			// trigger timed tasks
		}
		else {
			u32functionmask = SERVOOUTPUTS;
			if (u32fbufcntr == 5) trigger10ms = 1;
		}
	}
			outpw(&P37_DOUT, 0);			// time measurement P3.7 ***********************
}

// set duty cycle of PWM
// channel as bitmask, duty in the range of 0 to 255

void setPWMduty (uint32_t u32channel, uint8_t u8duty)
{
	uint32_t u32i;
	for (u32i = 0; u32i < 255; u32i++) {
		if (u32i < u8duty) 	u32functionframe[u32i] |=  u32channel;
		else				u32functionframe[u32i] &= ~u32channel;
	}
}
