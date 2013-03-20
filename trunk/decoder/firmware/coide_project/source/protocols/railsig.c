/**************************************************************************//**
 * @file railsig.c
 *
 * @brief Gleissignalerkennung.
 *
 * @note HW-abhängiges Modul für: Prozessor M0516ZBN
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "M051series.h"
#include "DrvGPIO.h"
#include "config.h"
#include "system.h"


// initializes HW resources for rail signal detection

void Init_Rail_detect(void)
{
	/* Select timer1 Operation mode as period mode */
	TIMER1->TCSR.MODE = 1;

	/* Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER1->TCSR.PRESCALE = 12-1;		// steps of 1 us
	TIMER1->TCMPR = 0x10000;			// Set TCMPR [0~16777215]

	/* Enable timer1 interrupt */
//	TIMER1->TCSR.IE = 1;                 // do we really need this interrupt?
//	NVIC->ISER[0] = BIT9;

	/* Enable read and count for Timer1 */
	TIMER1->TCSR.TDR_EN = 1;
	TIMER1->TCSR.CEN = 1;

   	// INT0 edge triggered rising and falling
	PORT3->IMD = 0;
	PORT3->IEN = BIT2 + BIT18;
	NVIC->ISER[0] = BIT2;

	// Debouncing enabled
	PORT_DBNCECON -> u32DBNCECON = 0x25;	// frequency to be checked, for test set to [HCLK/32]
	PORT3 -> DBEN = BIT2;
}

// interrupt for evaluation timer
void TMR1_IRQHandler(void)
{
    TIMER1->u32TISR = 1;				// clear request

    // what do we want to do here?
}


uint32_t u32rslast;

// called for every change at P3.2, which is fed by the rail signal
// evaluates time since last change and reports it to protocol selector

void EINT0_IRQHandler(void)
{
    // For P3.2, clear the INT flag
    PORT3->ISRC = BIT2;

    // read new counter value
    uint32_t u32rsact =  TIMER1->TDR;
//  TIMER0->TCSR.CRST = 1;
//  TIMER0->TCSR.CEN = 1;

    // report the difference
    handlechange((u32rsact - u32rslast) & 0xFFFF);
    u32rslast = u32rsact;
}

