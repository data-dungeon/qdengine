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
//	The JOB_OUT1_IN2 class implements ...
//
//	06-01-13:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_JOB_JOB_H__
#define __KCORE_JOB_JOB_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Includes/Includes)
#include INCL_KCORE(Job/Job_ABC)

#ifdef PLATFORM_PS3
#define MAX_NUM_SPU_TASK 10

#include "Template_Out1In2.h"

#define QDT_INCLUDE_STDIO
#define	QDT_INCLUDE_PS3_SPURS
#include INCL_KCORE(Includes_PS3/Includes_PS3)
#endif

#ifdef PLATFORM_PS3
#define JOB_ENTRY(N) (void(*)(void *, const void *, const void *, unsigned int nCount))N, _binary_ ## N ## _elf_start
#else
#define JOB_ENTRY(N) (void(*)(void *, const void *, const void *, unsigned int nCount))N
#endif

class	JOB_OUT1_IN2 : public JOB_ABC
{
public:
	typedef void (*PTR_JOB_FUNC)(void * pOut, const void * pIn1, const void * pIn2, unsigned int nCount);

#ifdef PLATFORM_PS3
	JOB_OUT1_IN2(PTR_JOB_FUNC pJN, const char* pSpuElf);
#else
	JOB_OUT1_IN2(PTR_JOB_FUNC pJN);
#endif

	~JOB_OUT1_IN2();

	void			SetOutPointer(void * pPtr);
	void			SetOutPointerSize(unsigned int nSize);
	void			SetInPointer1(void * pPtr);
	void			SetInPointer1Size(unsigned int nSize);
	void			SetInPointer2(void * pPtr);
	void			SetInPointer2Size(unsigned int nSize);

	void	SetCount(unsigned int nC);

	void	Execute(unsigned int);
	int		Wait();

	void SetFuncPtr(PTR_JOB_FUNC pJN)	{_pfJobName = pJN;}

private:

	PTR_JOB_FUNC	_pfJobName;

	void *			_pOut;
	unsigned int	_nSizeOfOut;
	void *			_pIn1;
	unsigned int	_nSizeOfIn1;
	void *			_pIn2;
	unsigned int	_nSizeOfIn2;

	unsigned int	_nCount;
	bool			_bInitialized;

	THREAD_POWER	_eThreadPower;

#ifdef PLATFORM_PS3
	CellSpursTaskset		_taskset;
	uint32_t				_atomicCounter						__attribute__((aligned(128)));
	JOB_PARAM_OUT1_IN2		_jobParam_Out1In2[MAX_NUM_SPU_TASK]	__attribute__ ((aligned(128)));
	CellSpursTaskArgument	_task_arg[MAX_NUM_SPU_TASK];

	int						_nNbrSPUAvailable;

	bool			_bSPUsUsed;

	unsigned int	_nNbrSPUThreadsUsed;
	unsigned int	_nCountRest;
	void *			_pOutRest;
	void *			_pIn1Rest;
	void *			_pIn2Rest;
#endif
};

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
