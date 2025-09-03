//*****************************************************************************
//
// Copyright (C) 2000-2005 Quantic Dream SA
//
// These coded instructions, statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are 
// protected by French and EEC copyright laws. They may not be 
// disclosed to third parties or copied or duplicated, in whole or in part, 
// without prior written consent of Quantic Dream SA
//
// Unpublished-rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
//	CLASS:	SPU_MANAGER
//	The SPU_MANAGER class implements ...
//
//	06-05-18:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifdef PLATFORM_PS3

#define		QDT_INCLUDE_PS3_SPURS
#define		QDT_INCLUDE_PS3_SPU_INITIALIZE
#include	INCL_KCORE(Includes_PS3/Includes_PS3)

class	SPU_MANAGER
{
public:
		static void Init( int nMaxPhysicalSPU, int nMaxRawSpu);

		static UInt32 GetNumSPUAvailable(void) { return _nNbrSPUAvailable; }
		static UInt32 IncNumSPUAvailable(UInt32 amount) { _nNbrSPUAvailable+=amount; QDT_ASSERT(_nNbrSPUAvailable <= _nMaxPhysicalSPU);}
		static UInt32 DecNumSPUAvailable(UInt32 amount) { _nNbrSPUAvailable-=amount; QDT_ASSERT(_nNbrSPUAvailable >= 0);}

		static CellSpurs* GetSpurs(void) { QDT_ASSERT(_bIsInitialized); return &_spurs; }
		
		static Bool IsInitialized(void) { return _bIsInitialized; }
private:
		static Bool			_bIsInitialized;
		static UInt32		_nMaxPhysicalSPU;
		static UInt32		_nMaxRawSpu;

		static UInt32		_nNbrSPUAvailable;

		static CellSpurs	_spurs;
};

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
