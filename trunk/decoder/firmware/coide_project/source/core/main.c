/**************************************************************************//**
 * @file main.c
 *
 * @brief Aufruf der Initialisierung und Hauptschleife (main).
 *
 * Das Programm startet mit Ausführung der Funktion main, initialisiert dann
 * den Prozessor, die Peripherie und alle anderen Ressourcen. Anschließend
 * wartet es in der Hauptschleife auf zu bearbeitenden Eingangsdaten und
 * Ereignisse.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

/* Anlegen der globalen Variablen */
#define MAIN
#include "globals.h"

#include "debug.h"

/**
 * @brief Hauptschleife
 */
int main (void)
{
	debug_init;

    while (1)
    {
    	DEBUG_IDLE
    }
}
