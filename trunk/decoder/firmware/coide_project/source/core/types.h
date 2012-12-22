/**************************************************************************//**
 * @file types.h
 *
 * @brief Definition grundlegender Datentypen.
 *
 * Zur besseren Lesbarkeit und eindeutigen Interpretation sind grundlegende
 * Datentypen hier neu definiert. Nur diese Datentypen sollen im Code verwendet
 * werden.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/* Integer-Typen definierter Länge */

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;

/* Bool */

typedef unsigned char bool;

#define FALSE 0
#define TRUE  1

/* Konstante NULL wird definiert, falls nicht schon anderweitig definiert. */

#ifndef NULL
#define NULL (void*)0
#endif

#endif
