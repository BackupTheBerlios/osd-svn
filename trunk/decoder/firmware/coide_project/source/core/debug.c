/**************************************************************************//**
 * @file debug.c
 *
 * @brief Debug-Routinen für die Entwicklung.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "M051series.h"
#include "debug.h"

unsigned int current = 0;

void debug_init (bool use_timer)
{
	if (use_timer == TRUE)  {
    
	   /* Select timer3 Operation mode as period mode */
	   TIMER3->TCSR.MODE = 1;

	   /* Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	   TIMER3->TCSR.PRESCALE = 12-1;		// steps of 1 us
	   TIMER3->TCMPR = 10000;	    		// Set TCMPR [0~16777215]

	   /* Enable timer3 interrupt */
	   TIMER3->TCSR.IE = 1;                 
	   NVIC->ISER[0] = BIT11;

	   /* Enable count for Timer3 */
       TIMER3->TCSR.CEN = 1;
	}
}

void debug_idle ()
{
	debug_idlecount++;
}

void debug_fetchidle ()
{
	static uint32 u32last_idlecount;

	debug_metrics[current++] = debug_idlecount - u32last_idlecount;
    if (current > 15) current = 0;
    u32last_idlecount = debug_idlecount;
}

// interrupt for debug timer
void TMR3_IRQHandler(void)
{
    TIMER3->u32TISR = 1;				// clear request

    debug_fetchidle();
}
