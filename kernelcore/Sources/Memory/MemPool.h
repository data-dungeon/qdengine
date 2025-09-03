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
//	CLASS:	MEM_POOL
//	The MEM_POOL class implements ...
//
//	02-01-03:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_MEMORY_MEMPOOL_H__
#define __KCORE_MEMORY_MEMPOOL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(MEM_POOL_HEADER);

namespace QDT
{
	namespace KCORE
	{

		class	MEM_POOL
		{
		public:
			MEM_POOL(const int nSize, const int nCapacity);
			explicit MEM_POOL(const MEM_POOL & C);
			~MEM_POOL();

			MEM_POOL &	operator=(const MEM_POOL & C);
			void		Copy(const MEM_POOL & C);

			int			GetSize() const;
			int			GetCapacity() const;
			void *		GetBuffer() const;
			void *		GetBuffer(const int nIndex) const;

			bool		IsInside(void * pBuffer) const;

			static int	GetMemoryUsed();

		private:

		friend class MEM_POOL_HEADER;

			int			_nSize;
			int			_nCapacity;
			void *		_pBuffer;

		static int		_nMemoryUsed;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
