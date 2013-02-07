/**************************************************************************//**
 * @file globals.h
 *
 * @brief Definition globaler Variablen und Konstanten.
 *
 * Globale Variablen werden nur an einer Stelle instanziiert und zwar in der
 * Datei main.c. Diese weist sich durch das Symbol MAIN aus.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"
#include "cvdef.h"

#ifdef MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

// Data which should be flash-backuped
typedef struct {	uint32_t	u32functions;
					uint8_t 	u8speed, u8direction; } S_COMMAND;

EXTERN uint8_t a8CVvalues[512];
EXTERN S_COMMAND sCommand;


// other global data


#endif
