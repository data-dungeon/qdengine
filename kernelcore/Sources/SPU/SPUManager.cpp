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
//
//	06-05-18:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"root.h"

#ifdef PLATFORM_PS3

#include	INCL_KCORE(SPU/SPUMANAGER)

CellSpurs SPU_MANAGER::_spurs __attribute__((aligned( CELL_SPURS_ALIGN )));

extern "C" {
int spurs_printf_service_initialize (CellSpurs*, int prio);
}

Bool	SPU_MANAGER::_bIsInitialized = false;

UInt32	SPU_MANAGER::_nMaxPhysicalSPU;
UInt32	SPU_MANAGER::_nMaxRawSpu;
UInt32	SPU_MANAGER::_nNbrSPUAvailable;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SPU_MANAGER constructor
//	Object:		
//	06-05-18:	VHA - Created
//-----------------------------------------------------------------------------
void SPU_MANAGER::Init(	int	nMaxPhysicalSPU,
						int	nMaxRawSpu)
{
	QDT_ASSERT(_bIsInitialized == false);

	// Initialize SPUs
	Int32	nResult = sys_spu_initialize(nMaxPhysicalSPU, nMaxRawSpu);
	QDT_ENSURE(nResult == CELL_OK);

	// Initialize SPURS
	nResult = cellSpursInitialize(&_spurs, nMaxPhysicalSPU-nMaxRawSpu, 250, 1, 1);
	QDT_ENSURE(nResult == CELL_OK);

/*	nResult = spurs_printf_service_initialize (&_spurs, 10);
	QDT_ENSURE(nResult == CELL_OK);*/

	_nNbrSPUAvailable = _nMaxPhysicalSPU = nMaxPhysicalSPU;
	_nMaxRawSpu = nMaxRawSpu;

	_bIsInitialized = true;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

#endif
