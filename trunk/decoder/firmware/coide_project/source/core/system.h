/**************************************************************************//**
 * @file system.h
 *
 * @brief Zugriff auf die System-Ressourcen des Prozessors.
 *
 * Die Routinen enthalten den HW-spezifischen Code des Prozessors, also
 * insbesondere die Initialisierung des Prozessors (Clock, ...). Integrierte
 * Peripherie-Komponenten (I/O, PWM, SPI, ...) werden in den entsprechenden
 * Output-Modulen initialisiert und angesprochen.
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

// prototypes for core functions
void Init_CV_area (void);

// prototypes for rail signal detection
void Init_Rail_detect (void);
void handlechange(uint32_t u32duration);

// prototypes for functions
void setPWMduty (uint32_t u32channel, uint8_t u8duty);

// prototypes for feedback
void InitFeedback (void);
void SendBidiFeedback (uint32_t u32length, uint8_t * pu8buffer);

#endif
