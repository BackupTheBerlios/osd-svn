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

// trigger for slow timed actions
volatile char trigger10ms;

// info storage for function outputs
uint32_t functionframe[255];
uint32_t functionstatic, functionmask;

// periodically called, handles all outputs used as GPIO
// simulates PWM by fast switching
// triggers slower actions

void TMR0_IRQHandler(void)
{
    static uint32_t fbufptr, fbufcntr;
    uint32_t functionact;

			outpw(PORT_BIT_DOUT+0x7C, 1);					// time measurement P3.7 ***********************
    TIMER0->TISR.TIF = 1;		// clear request

    // set all port outputs
    functionact = (functionstatic | functionframe[fbufptr]) & ~functionmask;
	PORT0->DOUT = functionact & 0xFF;
	PORT1->DOUT = (functionact >> 8) & 0xFF;
	PORT2->DOUT = (functionact >> 16) & 0xFF;
	PORT3->DOUT = (functionact >> 24) & 0xFF;

	if (++fbufptr > 254) {			// counts for SW-PWM
		fbufptr = 0;
		if (++fbufcntr >= 10) {		// counts for servo frame, 10 * 2ms
			fbufcntr = 0;
			functionmask = 0;
			trigger10ms = 1;		// trigger timed tasks
		}
		else {
			functionmask = SERVOOUTPUTS;
			if (fbufcntr == 5) trigger10ms = 1;
		}
	}
			outpw(PORT_BIT_DOUT+0x7C, 0);					// time measurement P3.7 ***********************
}

// set duty cycle of PWM
// channel as bitmask, duty in the range of 0 to 255

void setPWMduty (uint32_t channel, uint8_t duty)
{
	uint32_t i;
	for (i=0; i<255; i++) {
		if (i < duty) 	functionframe[i] |= channel;
		else			functionframe[i] &= ~channel;
	}
}
