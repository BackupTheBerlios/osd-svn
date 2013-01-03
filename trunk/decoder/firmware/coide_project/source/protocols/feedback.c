/**************************************************************************//**
 * @file feedback.c
 *
 * @brief Rückmeldung übers Gleis.
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


// initializes HW resources for feedback

void InitFeedback (void)
{
	/* get 250kb/s from 12MHz */
	UART0->u32BAUD = 1;

	/* data format 8N1 */
	UART0->u32LCR = 3;

#if ENABLE_RDS_FEEDBACK > 0
	/* P2.5 is PWM5 in PWMB group */
	PWMB->PPR.CP01 = 2;		// prescaler divides by 3
	PWMB->CSR.CSR1 = 4;		// input devided by 1
	PWMB->PCR.CH1MOD = 1;	// auto-repeat
	PWMB->CNR1 = 75;		// 12MHz / 3 / (75+1) = 52,6kHz
	PWMB->CMR1 = 37;		// 38 / 76 = 50% duty
	SYS->u32P2_MFP |= (0x100<<5);	// connect pin to PWM
	PWMB->POE.PWM1 = 1;		// connect to output

	PWMB->PCR.CH1EN = 1;	// Enable PWM (***** aber nur zum Test *****)
#endif
}

// send Bidi information, only a low number of bytes fitting into transmit FIFO
// parameters: number of bytes, pointer to a buffer containing the bytes to be sent

void SendBidiFeedback (uint32_t u32length, uint8_t * pu8buffer)
{
	uint32_t u32cntr;

	for (u32cntr = 0; u32cntr < u32length; u32cntr++)
		UART0->DATA =  *pu8buffer++;
}
