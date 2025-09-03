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
//	CLASS:	MEM_OPERATOR
//
//	06-05-17:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"

#include	INCL_KCORE(Mem/Mem)
#include	INCL_KCORE(Thread/MUTEX)
#include	INCL_KCORE(Mem/MemBigManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifdef QDT_MEM_ENABLED

// NEW(size_t)

#undef new

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new(	size_t s)
{
#ifdef QDT_MEM_DEBUG
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}
#endif

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(s);
	}

	void* pNewPtr;

	pNewPtr = MEM::GetInstance()->Allocate(s);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = false;
#endif

	return pNewPtr;
}

// NEW(size_t,allign)

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-05-17:	VHA - Created
//-----------------------------------------------------------------------------
void *	operator new(size_t	s,	UInt32	allign)
{
#ifdef QDT_MEM_DEBUG
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}
#endif

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(s);
	}

	void* pNewPtr;


	pNewPtr = MEM::GetInstance()->AllocateAlign(allign,s);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = false;
#endif

	return pNewPtr;
}

// NEW[](size_t)

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new[](	size_t cb )
{
#ifdef QDT_MEM_DEBUG
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}
#endif

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(cb);
	}

	void* pNewPtr = MEM::GetInstance()->Allocate(cb);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = true;
#endif

	return pNewPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//	Name:		operator delete
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void operator delete(	void * p )
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free(p);
		return;
	}

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)p - sizeof(MEM_BIG_CHUNK_ELEMENT));
	QDT_ASSERT(pElement->_bIsArray == false);
#endif

	MEM::GetInstance()->Free(p);
}

//-----------------------------------------------------------------------------
//	Name:		operator delete
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void operator delete[](	void * p )
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free(p);
		return;
	}

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)p - sizeof(MEM_BIG_CHUNK_ELEMENT));
	QDT_ASSERT(pElement->_bIsArray == true);
#endif

	MEM::GetInstance()->Free(p);
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
