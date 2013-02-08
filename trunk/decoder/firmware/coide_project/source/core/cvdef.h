/**************************************************************************//**
 * @file cvdef.h
 *
 * @brief Definitionen für die CV-Registersruktur.
 *
 * @note not HW related
*
 * @copyright Das Copyright liegt bei den Autoren. Das Programm unterliegt den
 *            Bedingungen der GNU General Public License 3 (GPL3).
 *            Details zu Copyright und Lizenz siehe README.
 *****************************************************************************/

// for creation of default values this file could be included a second time

#ifdef CVCREATEDEFAULT
	#undef CVDEF_H
#endif

#ifndef CVDEF_H
#define CVDEF_H


// macro definitions for allocating enums and default array for CVs

#ifdef CVCREATEDEFAULT
	const unsigned char a8CV_factory[] = {
	#undef DEFCV
	#undef RESCV
	#define DEFCV(index, symbol, defval) defval,
	#define RESCV 255,
#else
	enum E_CV_Registers { 
	#define DEFCV(index, symbol, defval) symbol=(index-1),
	#define RESCV
#endif


    // used CVs, enum symbols, and factory default
	// dient lediglich als Vorschlag, muss überarbeitet werden !

	DEFCV(1, E_CV_Short_Addr, 3)		// CV 1
	RESCV
	DEFCV(3, E_CV_Acceleration, 4)		// CV 3
	DEFCV(4, E_CV_Deceleration, 4)		// CV 4
	RESCV
	RESCV
	DEFCV(7, E_CV_ManuVersion, 1)		// CV 7
	DEFCV(8, E_CV_Manufacturer, 13)		// CV 8
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	DEFCV(17, E_CV_LongAdr_HI,	0)		// CV 17
	DEFCV(18, E_CV_LongAdr_LO,	0)		// CV 18
	DEFCV(19, E_CV_ConsistAddr,	0)		// CV 19
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	DEFCV(28, E_CV_BIDI_Config,	7)		// CV 28
	DEFCV(29, E_CV_ConfigData1,	14)		// CV 29
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	RESCV
	DEFCV(47, E_CV_Motoaddr_2,	14)		// CV 47
	RESCV
	DEFCV(49, E_CV_ConfigData2,	1)		// CV 49
	DEFCV(50, E_CV_Protokolle,	31)		// CV 50

	};

#endif
