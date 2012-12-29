/**************************************************************************//**
 * @file config.h
 *
 * @brief Konstanten zur Compile-Zeit-Konfiguration der FW.
 *
 * Die Firmware kann in manchen Aspekten konfiguriert werden, ohne dass dazu
 * eine echte Code-Änderung notwendig ist. Die Konfiguration führt über die
 * Veränderung einiger Konstanten (defines), die in dieser Datei zusammen-
 * gefasst sind.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

// frequency in MHz for processor clock
#define CPUFREQ	50

// mask for function outputs used for servo, pulse cycle set to 20ms
#define SERVOOUTPUTS BIT7

#endif
