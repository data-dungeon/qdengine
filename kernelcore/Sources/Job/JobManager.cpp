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
//
//	06-01-13:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Job/JobManager)

#define		QDT_INCLUDE_PS3_SPURS
#define		QDT_INCLUDE_PS3_SPU_INITIALIZE
#include	INCL_KCORE(Includes_PS3/Includes_PS3)

#include	INCL_KCORE(SPU/SPUManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

QDT_VECTOR<JOB_ABC *>	JOB_MANAGER::_vJobs;

//-----------------------------------------------------------------------------
//	Name:		CreateJob
//	Object:		
//	06-01-13:	SBE - Created
//-----------------------------------------------------------------------------
void		JOB_MANAGER::AddJob(JOB_ABC * pJob)
{
	_vJobs.PushTail((JOB_ABC *)pJob);
}

//-----------------------------------------------------------------------------
//	Name:		ExecuteJob
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_MANAGER::ExecuteJob(JOB_ABC	* pJob)
{
	int nNbrSPUAvailable = 0;

#ifdef PLATFORM_PS3
	switch (pJob->GetThreadPower())
	{
		default:
		case JOB_ABC::TP_HIGH:
		{
			nNbrSPUAvailable = SPU_MANAGER::GetNumSPUAvailable();
			break;
		}
		case JOB_ABC::TP_MIDDLE:
		{
			nNbrSPUAvailable = SPU_MANAGER::GetNumSPUAvailable() / 2;
			break;
		}
		case JOB_ABC::TP_LOW:
		{
			nNbrSPUAvailable = 1;
			break;
		}
		case JOB_ABC::TP_NONE:
		{
			nNbrSPUAvailable = 0;
			break;
		}
	}

	SPU_MANAGER::DecNumSPUAvailable(nNbrSPUAvailable);
#endif

	pJob->Execute(nNbrSPUAvailable);
}

//-----------------------------------------------------------------------------
//	Name:		ExecuteJob
//	Object:		
//	06-05-29:	VHA - Created
//-----------------------------------------------------------------------------
void	JOB_MANAGER::ExecuteJob(JOB_ABC	*pJob,
								int		numSpu)
{
	//SPU_MANAGER::DecNumSPUAvailable(numSpu);
	pJob->Execute(numSpu);
}


//-----------------------------------------------------------------------------
//	Name:		RemoveJob
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_MANAGER::RemoveJob(JOB_ABC	* pJob)
{
	_vJobs.Remove((JOB_ABC *)pJob);
}

//-----------------------------------------------------------------------------
//	Name:		DeleteAllJobs
//	Object:		
//	06-01-18:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_MANAGER::DeleteAllJobs()
{
	for (unsigned int iJob=0; iJob < _vJobs.GetSize(); ++iJob)
	{
		JOB_ABC * pJob = _vJobs[iJob];
		RemoveJob(pJob);
		delete pJob;
	}
}

//-----------------------------------------------------------------------------
//	Name:		WaitAllJobs
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_MANAGER::WaitAllJobs()
{
	for (unsigned int iJob=0; iJob < _vJobs.GetSize(); ++iJob)
	{
		WaitJob(_vJobs[iJob]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		WaitJob
//	Object:		
//	06-01-16:	SBE - Created
//-----------------------------------------------------------------------------
void	JOB_MANAGER::WaitJob(JOB_ABC * pJob)
{
	UInt32 numSPUFreed = pJob->Wait();

#ifdef PLATFORM_PS3
	SPU_MANAGER::IncNumSPUAvailable(numSPUFreed);
#endif
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
