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
//	CLASS:	MEM_POOL_HEADER
//	The MEM_POOL_HEADER class implements ...
//
//	02-01-03:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_MEMORY_MEMPOOLHEADER_H__
#define __KCORE_MEMORY_MEMPOOLHEADER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

// #define TRACK_MEM_POOL_HEADERS

DECL_KCORE(MEM_POOL);
DECL_KCORE(IO_STREAM);

#ifdef TRACK_MEM_POOL_HEADERS
#include INCL_KCORE(Template/QDT_Vector)
#endif

namespace QDT
{
	namespace KCORE
	{
		class	MEM_POOL_HEADER
		{
		public:
			enum
			{
				ALLOCATION_SIZE = 1024,
			};

		#ifdef TRACK_MEM_POOL_HEADERS
			MEM_POOL_HEADER(const int    nSize, 
							const char * pcFile,
							int			 nLine,
							const int    nFirstCapacity, 
							const int    nNextCapacity);
		#else
			MEM_POOL_HEADER(const int nSize, 
							const int nFirstCapacity, 
							const int nNextCapacity);
		#endif
			~MEM_POOL_HEADER();

			void *					Allocate();
			void					Deallocate(const void * pBuffer);
			bool					VerifyAddress(const void * pBuffer);

			int						GetSize() const;
			int						GetNextCapacity() const;

			int						GetFreeMem() const;
			int						GetNbrFree() const;
			int						GetNbrTotal() const;

			void					Print(IO_STREAM * pStream);

			static int				GetMemoryUsed();
			static int				GetMemoryFree();

		#ifdef TRACK_MEM_POOL_HEADERS
			static void				PrintMemoryTracking();
			void					PrintIndividualMemoryTracking();
		#endif

			MEM_POOL_HEADER *		Clone() const;

		private:

			void					AddMemPool();

			int						_nSize;
			int						_nFirstCapacity;
			int						_nNextCapacity;

			struct SPOOL
			{
				MEM_POOL *		_pPool;
				SPOOL *			_pNext;
			};

			SPOOL *					_pFirstPool;

			unsigned int			_nNbrFreeItems;
			unsigned int			_nNbrAllocatedFreeItems;
			void **					_pFreeList;
			unsigned int			_nNbrUsedItems;

			static bool				_bNoVerify;
			static int				_nMemoryFree;
			static int				_nFreeListMemory;

		#ifdef TRACK_MEM_POOL_HEADERS
			int									_nLine;
			char								_tcFile[256];
			int									_nMaxNumberAllocated;
			static QDT_VECTOR<MEM_POOL_HEADER*>	_MemPoolList;
		#endif
		};

		#ifdef TRACK_MEM_POOL_HEADERS
			#define MEM_POOL_HEADER(a, b, c) MEM_POOL_HEADER(a, __FILE__, __LINE__, b, c)
		#endif
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
