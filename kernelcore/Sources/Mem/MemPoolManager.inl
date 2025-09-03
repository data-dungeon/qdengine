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
//	CLASS:	MEM_POOL_MANAGER
//
//	06-06-30:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#undef new

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-05-31:	VHA - Created
//-----------------------------------------------------------------------------
void* MEM_POOL_MANAGER::operator new(size_t s)
{
	return malloc(sizeof(MEM_POOL_MANAGER));
}

//-----------------------------------------------------------------------------
//	Name:		operator delete
//	Object:		
//	06-05-31:	VHA - Created
//-----------------------------------------------------------------------------
void MEM_POOL_MANAGER::operator delete(void* p)
{
	return free(p);
}

//-----------------------------------------------------------------------------
//	Name:		lock
//	Object:		
//	06-05-31:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::Lock()
{
	QDT_ASSERT(_Mutex);

	_Mutex->Lock();
}

//-----------------------------------------------------------------------------
//	Name:		unlock
//	Object:		
//	06-05-31:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::Unlock()
{
	QDT_ASSERT(_Mutex);

	_Mutex->Unlock();
}

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL_MANAGER::Allocate(UInt32	uSize)
{
	MEM_ALLOCATION_REQUEST request;

	request._uSize = uSize;
	request._uBoundary = 0;

#ifdef QDT_MEM_DEBUG
	request._uLibId = 0;
	request._uDepth = 1;
#endif

	return Allocate(&request);
}

//-----------------------------------------------------------------------------
//	Name:		AllocateCustom
//	Object:		
//	06-06-15:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL_MANAGER::AllocateCustom(UInt32	uSize)
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(uSize);
	}

	Lock();
	void* ptr = Allocate(uSize);
	Unlock();

	return ptr;
}

//-----------------------------------------------------------------------------
//	Name:		FreeCustom
//	Object:		
//	06-03-22:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::FreeCustom(const void *	pPtr)
{
	QDT_ASSERT(pPtr);

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free((void*)pPtr);
		return;
	}

	if((pPtr >=_pMemPoolArray) && (pPtr < _pEndOfMemPoolArray))
	{
		Lock();
		Free(pPtr);
		Unlock();
	}
	else
	{
		MEM::GetInstance()->Free(pPtr);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
