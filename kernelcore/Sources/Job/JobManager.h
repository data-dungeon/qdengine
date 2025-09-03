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
//	CLASS:	JOB_MANAGER
//	The JOB_MANAGER class implements ...
//
//	06-01-13:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_JOB_JOBMANAGER_H__
#define __KCORE_JOB_JOBMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PS3_SPURS

#include	INCL_KCORE(Job/Job_ABC)
#include	INCL_KCORE(Template/QDT_Vector)
#include	INCL_KCORE(Includes_PS3/Includes_PS3)

#include INCL_KCORE(Template/Singleton)

class	JOB_MANAGER : public SINGLETON<JOB_MANAGER>
{
public:

	static void	AddJob(JOB_ABC *);

	static void ExecuteJob(JOB_ABC *);
	static void ExecuteJob(JOB_ABC *, int numSpu);
	static void RemoveJob(JOB_ABC *);
	static void WaitJob(JOB_ABC *);

	static void DeleteAllJobs();
	static void WaitAllJobs();

private:
	friend class JOB_ABC;

	static QDT_VECTOR<JOB_ABC *>	_vJobs;
};

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
