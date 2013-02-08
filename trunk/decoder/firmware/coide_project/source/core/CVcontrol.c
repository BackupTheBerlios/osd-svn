/**************************************************************************//**
 * @file CVcontrol.c
 *
 * @brief Handling of non-volatile storage for parameters.
 *
 * @note not HW related
 *
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

#include <string.h>
#include "globals.h"

// here we define the array for the factory default values and fill it out
#define CVCREATEDEFAULT
#include "cvdef.h"


// up to now "Init_CV_area" copies only the factory default into RAM
// later it has to fill the RAM area from the actual data in DataFlash
// but before implementing, the strategy of flash handling has to be clarified

void Init_CV_area (void)
{
    if ( sizeof(a8CV_factory) <= sizeof(a8CVvalues))
	   memcpy(a8CVvalues, a8CV_factory, sizeof(a8CV_factory));
}
