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
//	CLASS:	JOB_ABC
//	The JOB_ABC class implements ...
//
//	06-01-13:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_JOB_JOBABC_H__
#define __KCORE_JOB_JOBABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

class	JOB_ABC
{
public:
	enum THREAD_POWER
	{
		TP_NONE = 0,
		TP_LOW,
		TP_MIDDLE,
		TP_HIGH
	};

	virtual ~JOB_ABC() {};
	virtual void	Execute(unsigned int) = 0;
	virtual int		Wait() = 0;

	void			SetThreadPower(THREAD_POWER eThreadPower) {_eThreadPower = eThreadPower;}
	THREAD_POWER	GetThreadPower() const { return _eThreadPower;}

protected:

	THREAD_POWER						_eThreadPower;

#ifdef PLATFORM_PS3
	const char*							_pSpuElf;
	void SetElfPtr(const char * pElf)	{_pSpuElf = pElf;}
#endif

};

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
