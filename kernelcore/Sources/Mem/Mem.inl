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
//	CLASS:	MEM
//
//	06-06-30:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(Thread/MUTEX)

//-----------------------------------------------------------------------------
//	Name:		CreateInstance
//	Object:		
//	06-06-30:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::CreateInstance()
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	
	#ifdef PLATFORM_PC
		if (_pInstance == NULL)
	#else
		QDT_ASSERT(_pInstance == NULL);
	#endif
	{
		_pInstance = new MEM();
		atexit(MEM::ExitCleanup);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetInstance
//	Object:		
//	06-06-30:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::SetInstance(MEM* pNewInstance)
{
	QDT_ASSERT(_pInstance == NULL);
	QDT_ASSERT(pNewInstance != NULL);
	QDT_ASSERT(g_MemConfig._bIsActive == mem_inherit);

	g_MemConfig = *pNewInstance->_pMemConfig;

	_pInstance = pNewInstance;

	#ifdef QDT_MEM_DEBUG
		pNewInstance->_bIsCreatingTime = true;
	#endif
		TIME::Instance()->GetSystemTime(); // force time creation
	#ifdef QDT_MEM_DEBUG
		pNewInstance->_bIsCreatingTime = false;
	#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetInstance
//	Object:		
//	06-06-30:	VHA - Created
//-----------------------------------------------------------------------------
MEM*	MEM::GetInstance()
{
	//#ifdef PLATFORM_PC
		if (_pInstance == NULL) // Tmp
			CreateInstance();
	//#else
	//	QDT_ASSERT(_pInstance);
	//#endif

	return _pInstance;
}

//-----------------------------------------------------------------------------
//	Name:		IsInstancied
//	Object:		
//	06-06-30:	VHA - Created
//-----------------------------------------------------------------------------
bool MEM::IsInstancied()
{
	return _pInstance ? true : false;
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	06-03-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::Lock()
{
#ifdef QDT_MEM_THREAD_SAFE
	GetInstance()->_Mutex->Lock();
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	06-03-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::Unlock()
{
#ifdef QDT_MEM_THREAD_SAFE
	GetInstance()->_Mutex->Unlock();
#endif
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
