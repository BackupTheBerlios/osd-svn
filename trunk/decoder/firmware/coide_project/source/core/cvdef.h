

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
	DEFCV(48, E_CV_Protokolle,	7)		// CV 48

	};

