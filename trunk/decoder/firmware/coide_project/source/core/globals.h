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

#ifdef MAIN
#define EXTERN
#else
#define EXTERN extern
#endif



#endif
