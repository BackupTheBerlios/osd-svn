/**************************************************************************//**
 * @file debug.c
 *
 * @brief Debug-Routinen für die Entwicklung.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include "debug.h"

unsigned int current = 0;

void debug_init (bool use_timer)
{
	if (use_timer == TRUE)
	{
		/// @todo Timer aufsetzen, mit Interrupt verbinden
	}
}

void debug_idle ()
{
	debug_idlecount++;
}

void debug_fetchidle ()
{
	debug_metrics[current++] = debug_idlecount;
	debug_idlecount = 0;
}
