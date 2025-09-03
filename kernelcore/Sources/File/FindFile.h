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
// CLASS: FIND_FILE
//
//
//01-01-24 FKE Created: 
//*****************************************************************************

#ifndef __KCORE_FILE_FINDFILE_H__
#define __KCORE_FILE_FINDFILE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class FIND_FILE
		{
		public:
			explicit FIND_FILE(const char *pName);
			virtual ~FIND_FILE();
			bool	IsExist() const;
			const char *GetName() const;
			int GetSize() const;
			bool GetNext();
		private:
			enum
			{
				END_SEARCH = 1 << 0,
			};
			int		_nFlags;
			class	FIND_FILE_DATAS *_pPltDatas;
		};
	}
}

#endif //__KCORE_FILE_FINDFILE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
