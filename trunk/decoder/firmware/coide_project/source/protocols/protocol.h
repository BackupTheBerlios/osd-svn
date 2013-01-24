/**************************************************************************//**
 * @file protocol.h
 *
 * @brief Definitionen für die Protokollmodule.
 *
 * @note not HW related
*
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define uint8_t unsigned char
#define uint32_t unsigned int

// enumerations
enum E_ProtState { E_idle, E_MM_rec, E_DCC_rec, E_DCC_ans, E_OXP_rec, E_OXP_ans };
enum E_Pulselength	{ E_25, E_50, E_100, E_200, E_400, E_900, E_New = 128 };

// prototypes for functions
uint8_t handleMM(uint8_t u8pulselen);
uint8_t handleDCC(uint8_t u8pulselen);
uint8_t handleOXP(uint8_t u8pulselen);
void executeMM (void);
void executeDCC (void);
void executeOXP (void);

void handlechange(uint32_t u32duration);


#endif
