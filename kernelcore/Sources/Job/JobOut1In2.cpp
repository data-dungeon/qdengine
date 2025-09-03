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
//	CLASS:	JOB_OUT1_IN2
//
//	06-01-13:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Job/JobOut1In2)
#include	INCL_KCORE(Job/JobManager)

#include	INCL_KCORE(Core/Message)
#include	INCL_KCORE(Tools/QDTString)

#include	INCL_KCORE(SPU/SPUManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#define QDT_JOBMessage //QDT_Message
//#define QDT_JOBMessage 

#ifdef PLATFORM_PS3
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/spu_initialize.h>
	#include <sys/spu_thread.h>
	#include <sys/spu_thread_group.h>
	#include <sys/spu_utility.h>
	#include <sys/event.h>
	#include <sys/paths.h>
	#include <cell/atomic.h>
	
	#define MAX_NUM_SPU_THREADS    6
#endif

//-----------------------------------------------------------------------------
//	Name:		JOB_OUT1_IN2 constructor
//	Object:		
//	06-01-13:	SBE - Created
//-----------------------------------------------------------------------------
JOB_OUT1_IN2::JOB_OUT1_IN2(	PTR_JOB_FUNC pJN
#ifdef PLATFORM_PS3
							,const char* pSpuElf
#endif
							)
		 :
_bInitialized(false)
{
	SetFuncPtr(pJN);
#ifdef PLATFORM_PS3
	SetElfPtr(pSpuElf);
#endif

	JOB_MANAGER::AddJob(this);
}

//-----------------------------------------------------------------------------
//	Name:		~JOB_OUT1_IN2 destructor
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
JOB_OUT1_IN2::~JOB_OUT1_IN2()
{
	JOB_MANAGER::RemoveJob(this);
}

//-----------------------------------------------------------------------------
//	Name:		SetOutPointer
//	Object:		
//	06-01-13:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetOutPointer(void *	pPtr)
{
	_pOut = pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		SetOutPointerSize
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetOutPointerSize(unsigned int	nSize)
{
	_nSizeOfOut = nSize;
}

//-----------------------------------------------------------------------------
//	Name:		SetInPointer1
//	Object:		
//	06-01-13:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetInPointer1(void *	pPtr)
{
	_pIn1 = pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		SetInPointer1Size
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetInPointer1Size(unsigned int	nSize)
{
	_nSizeOfIn1 = nSize;
}

//-----------------------------------------------------------------------------
//	Name:		SetInPointer2
//	Object:		
//	06-01-13:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetInPointer2(void *	pPtr)
{
	_pIn2 = pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		SetInPointer1Size
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetInPointer2Size(unsigned int	nSize)
{
	_nSizeOfIn2 = nSize;
}

//-----------------------------------------------------------------------------
//	Name:		SetCount
//	Object:		
//	06-01-13:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::SetCount(unsigned int	nC)
{
	_nCount = nC;
}

//-----------------------------------------------------------------------------
//	Name:		Execute
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_OUT1_IN2::Execute(unsigned int n)
{
#ifndef PLATFORM_PS3
	(*_pfJobName)(_pOut, _pIn1, _pIn2, _nCount);
#else
	_nNbrSPUAvailable = n;

	if(n==0)
	{
		(*_pfJobName)(_pOut, _pIn1, _pIn2, _nCount);
		return;
	}
	uint8_t prios[8] = {1, 1, 1, 1, 1, 1, 1, 1};

	Int32	nResult = cellSpursCreateTaskset (SPU_MANAGER::GetSpurs(), &_taskset, (uint64_t)&_atomicCounter, prios, n);
	QDT_ENSURE(nResult == CELL_OK);

	int nCount = _nCount;
	_nCountRest = nCount % _nNbrSPUAvailable;
	nCount -= _nCountRest;

	QDT_ASSERT(_nNbrSPUAvailable < MAX_NUM_SPU_TASK);

	unsigned char *	pOut = (unsigned char *) _pOut;
	unsigned char *	pIn1 = (unsigned char *) _pIn1;
	unsigned char *	pIn2 = (unsigned char *) _pIn2;

	// init atomic
	cellAtomicStore32(&_atomicCounter, _nNbrSPUAvailable);

	int i;
	for (i=0; i < _nNbrSPUAvailable; i++) 
	{
		CellSpursTaskId	tid;

		QDT_ASSERT((((uint64_t) pOut) % 16) == 0);
		QDT_ASSERT((((uint64_t) pIn1) % 16) == 0);
		QDT_ASSERT((((uint64_t) pIn2) % 16) == 0);

		_jobParam_Out1In2[i].pOut = (uint64_t) pOut;
		_jobParam_Out1In2[i].pIn1 = (uint64_t) pIn1;
		_jobParam_Out1In2[i].pIn2 = (uint64_t) pIn2;
		_jobParam_Out1In2[i].nCount = (nCount / _nNbrSPUAvailable);

		pOut += _jobParam_Out1In2[i].nCount * _nSizeOfOut;
		pIn1 += _jobParam_Out1In2[i].nCount * _nSizeOfIn1;
		pIn2 += _jobParam_Out1In2[i].nCount * _nSizeOfIn2;

		_task_arg[i].u64[0] = (uint64_t)&_jobParam_Out1In2[i];
		_task_arg[i].u64[1] = i;

		nResult = cellSpursCreateTask(&	_taskset, &tid,
										_pSpuElf,
										NULL,
										CELL_SPURS_TASK_CONTEXT_SIZE_ALL,
										&gCellSpursTaskLsAll,
										&_task_arg[i]);

		QDT_ENSURE(nResult == CELL_OK);
	}

	_pOutRest = pOut;
	_pIn1Rest = pIn1;
	_pIn2Rest = pIn2;

	_bSPUsUsed = true;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Wait
//	Object:		
//	06-01-17:	SBE - Created
//-----------------------------------------------------------------------------
int		JOB_OUT1_IN2::Wait()
{
#ifdef PLATFORM_PS3
	if(_nNbrSPUAvailable)
	{
		if (_nCountRest > 0)
		{
			(*_pfJobName)(_pOutRest, _pIn1Rest, _pIn2Rest, _nCountRest);
		}

		int ret = cellSpursJoinTaskset(&_taskset);
		QDT_ASSERT(ret == CELL_OK);

		int nNbrSPUAvailable = _nNbrSPUAvailable;	
		_nNbrSPUAvailable = 0;

		return nNbrSPUAvailable;
	}
#endif
	return 0;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
