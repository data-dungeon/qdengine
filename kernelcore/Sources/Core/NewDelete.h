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
//	CLASS:	NEW_DELETE
//	The NEW_DELETE class implements ...
//
//	05-11-16:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_CORE_NEWDELETE_H__
#define __KCORE_CORE_NEWDELETE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef QDT_MEM_INTERNAL

// must be declared out of any namespace

#if (defined PLATFORM_PC) || (defined PLATFORM_X360) 

	inline	void *	operator new		(unsigned int, void * ptr); // new with placement
			void *	operator new		(unsigned int);
			void *	operator new[]		(unsigned int);
#ifndef __PLACEMENT_NEW_INLINE // This used by visual to implement the placed new/delete. This way, we should be always using our own implementation
#define __PLACEMENT_NEW_INLINE
	inline void *	operator new(size_t, void *_P){return (_P); }
	inline	void	operator delete		(void*,void*){};
#endif
			void	operator delete		(void*);
			void	operator delete[]	(void*);

#ifdef QDT_MEM_DEBUG
	#define	QDT_NEW_PLACED(a, b)		MEM::DebugNextPtrMutexLock(); MEM::SetNextReturnPtr(a); new b
#else
	//#define	QDT_NEW_PLACED(a, b)		new (a) b
	#define	QDT_NEW_PLACED(a, b)		(a)->b::b
#endif

	#define	QDT_DELETE_PLACED(a, b)		(a)->b::~b()

	inline  void *	operator new(size_t, void *_P, void *_P2){return _P2;}

#elif defined (PLATFORM_PS3) && !(defined (SPU))

	inline	void *	operator new		(unsigned int, void * ptr) { return (ptr); } // new with placement
			void *	operator new		(unsigned int);
			void *	operator new[]		(unsigned int);
			void	operator delete		(void*);
			void	operator delete[]	(void*);

#ifdef QDT_MEM_DEBUG
	#define	QDT_NEW_PLACED(a, b)		MEM::DebugNextPtrMutexLock(); MEM::SetNextReturnPtr(a); new b
	//#define	QDT_NEW_PLACED(a, b)		new (a) b
#else
	#define	QDT_NEW_PLACED(a, b)		new (a) b
#endif

	#define	QDT_DELETE_PLACED(a, b)		(a)->b::~b()

#elif defined (PLATFORM_PS3) && defined (SPU)
	inline	void *	operator new		(unsigned long int, void * ptr) { return (ptr); } // new with placement
			void *	operator new		(unsigned long int);
			void *	operator new[]		(unsigned long int);
			void	operator delete		(void*);
			void	operator delete[]	(void*);

#else

	#error CAN T IDENTIFY TARGET PLATFORM, PLEASE CHECK

#endif

#endif //QDT_MEM_INTERNAL

#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
