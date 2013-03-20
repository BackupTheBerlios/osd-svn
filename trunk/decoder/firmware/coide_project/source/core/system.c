/**************************************************************************//**
 * @file system.c
 *
 * @brief Zugriff auf die System-Ressourcen des Prozessors.
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


void SystemInit (void)
{
	// set init port pin values and types for LDG34+ HW
	PORT0->DOUT = 0;
	PORT1->DOUT = 0;
	PORT2->DOUT = 0;
	PORT3->DOUT = BIT5 + BIT4 + BIT1;	// inputs and TXD remain high
	PORT3->DMASK = BIT7;				// reserved for debugging

	// open pins are set to QBM to keep them fixed
	// PORT0            7:OUT   6:QBM   5:OUT   4:OUT  3:QBM  2:QBM  1:OUT  0:OUT
	outpw(&PORT0->PMD, (1<<14)+(3<<12)+(1<<10)+(1<<8)+(3<<6)+(3<<4)+(1<<2)+ 1);
	// PORT1            7:QBM   6:QBM   5:IN    4:IN   3:OUT  2:OUT  1:QBM  0:IN
	outpw(&PORT1->PMD, (3<<14)+(3<<12)+(0<<10)+(0<<8)+(1<<6)+(1<<4)+(3<<2)+ 0);
	// PORT2            7:QBM   6:OUT   5:OUT   4:OUT  3:OUT  2:OUT  1:QBM  0:QBM
	outpw(&PORT2->PMD, (3<<14)+(1<<12)+(1<<10)+(1<<8)+(1<<6)+(1<<4)+(3<<2)+ 3);
	// PORT3            7:QBM   6:OUT   5:QBM   4:QBM  3:QBM  2:IN   1:QBM  0:QBM
	outpw(&PORT3->PMD, (3<<14)+(1<<12)+(3<<10)+(3<<8)+(3<<6)+(0<<4)+(3<<2)+ 3);

 	/* Unlock proctected registers */
	UNLOCKREG();

	/* Enable external 12M Crystal */
	SYSCLK->PWRCON.XTL12M_EN = 1;

	/* Wait until 12M clock is stable. */
	while(SYSCLK->CLKSTATUS.XTL12M_STB == 0);

	/* switch PLL to fast CPU clock */
	outpw(&SYSCLK->PLLCON, (0xC1FE + CPUFREQ));

	/* Wait for PLL stable */
	while (SYSCLK->CLKSTATUS.PLL_STB == 0);

	/* Change HCLK clock source to be PLL. */
	SYSCLK->CLKSEL0.HCLK_S = 2;

	/* Lock procteted registers */
	LOCKREG();

	/* enable required clock lines and select 12M clock */
	SYSCLK->u32APBCLK = 0xFFFFFFFF;
	SYSCLK->u32CLKSEL1 = 0;
	SYSCLK->u32CLKSEL2 = 0;

	/* initialise configuration data from DataFlah */
	Init_CV_area();

	/* Select timer0 Operation mode as period mode */
	TIMER0->TCSR.MODE = 1;

	/* Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER0->TCSR.PRESCALE = 12-1;		// steps of 1 us
	TIMER0->TCMPR = 8;					// Set TCMPR [0~16777215]

	/* Enable timer0 interrupt */
	TIMER0->TCSR.IE = 1;
	NVIC->ISER[0] = BIT8;

	/* Enable Timer0 */
	TIMER0->TCSR.CEN = 1;

	// set type, alt, mfp for all pins
	outpw(&SYS->P0_MFP, 0);
	outpw(&SYS->P1_MFP, 0x001 + (0x001<<4) + (0x001<<5));		// analog inputs
	outpw(&SYS->P2_MFP, 		(0x100<<2) + (0x100<<6));		// PWM
	outpw(&SYS->P3_MFP, 0xFF0000 + (0x001<<1) + (0x001<<2));	// TXD and INT0, Schmitt Trigger
//	outpw(&SYS->P4_MFP, BIT7 + BIT6);	// Emulator seem automatically be detected ?

	/* int HW for feedback */
	InitFeedback();

	/* select usable ADC input channels, chan 7 for temperature */
	SYS->TEMPCR = 1;					// enable temperature sensor
	ADC->u32ADCHER = 0x200 + BIT7 + BIT5 + BIT4 + BIT0;

	/* set ADC mode and start it */
	ADC->u32ADCR = 0xD;
	ADC->ADCR.ADST = 1;

	// enable rail signal detection via INT0
	Init_Rail_detect();
}
