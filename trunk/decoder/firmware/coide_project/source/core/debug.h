/**************************************************************************//**
 * @file debug.h
 *
 * @brief Debug-Routinen f�r die Entwicklung.
 *
 * Die Debug-Routinen werden nur f�r die Entwicklung ben�tigt. Sie erlauben
 * insbesondere eine Performance-Messung (Profiling).
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include "types.h"

/* Z�hler-Register f�r Performance-Absch�tzung */
#ifdef MAIN
extern uint32 debug_idlecount;
extern uint32 debug_metrics[16];
#else
uint32 debug_idlecount;
uint32 debug_metrics[16];
#endif

/**
 * @brief Initialisierung der Debug-Routinen.
 *
 * Achtung, es wird ein Timer f�r das regelm��ige Abholen der Idle-Werte
 * ben�tigt, wenn diese Funktion eingeschaltet wird. Man kann die Abhol-
 * funktion auch in einen bestehenden Timer selbst einbinden.
 *
 * @param[in]  use_timer  True, wenn ein eigener Timer durch die Debug-
 *                        Routine verwendet werden soll.
 */
void debug_init (bool use_timer);

/**
 * @brief Erfassungsfunktion f�r Idle-Time.
 *
 * Die Funktion wird an den Stellen aufgerufen, an denen "nichts" gemacht wird,
 * an denen also insbesondere die Hauptschleife ihre Warterunden dreht. Die
 * Funktion erh�ht einen Z�hler, der regelm��ig (Timer-Interrupt) abgespeichert
 * und auf Null gesetzt wird.
 */
void debug_idle ();

/**
 * @brief Inline-Funktion f�r die Erfassung der Idle-Time.
 *
 * Diese Alternative ist schneller, weil sie keinen Funktionsaufruf ben�tigt,
 * sondern das Erh�hen des Idle-Counts direkt inline erledigt. Dieses Makro
 * sollte normalerweise verwendet werden, da es auch am wenigsten Auswirkungen
 * auf den restlichen Code hat.
 */
#define DEBUG_IDLE debug_idlecount++;

/**
 * @brief Zyklischer Timer-Interrupt zum Abholen der Idle-Z�hler.
 *
 * Die Funktion muss nur dann zyklisch aufgerufen werden, wenn das Debug-Modul
 * keinen eigenen Timer verwenden darf.
 */
void debug_fetchidle ();

#endif
