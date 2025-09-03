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

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(File/FindFile)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

#define		QDT_INCLUDE_X360_XTL
#include	INCL_KCORE(Includes_X360/Includes_X360)

namespace QDT
{
	namespace KCORE
	{
		class FIND_FILE_DATAS
		{
			public:
				explicit FIND_FILE_DATAS() {};
				WIN32_FIND_DATA _FindFileData;                       
				HANDLE	_Handle;
		};
	}
}

//-----------------------------------------------------------------------------
// Name: FIND_FILE constructor
// Object: 
//01-05-29 FKE Created: 
//-----------------------------------------------------------------------------
FIND_FILE::FIND_FILE(const char *pName)
:
_nFlags(0)
{
	_pPltDatas = new FIND_FILE_DATAS;
	_pPltDatas->_Handle = FindFirstFile(pName, &_pPltDatas->_FindFileData);
	if (_pPltDatas->_Handle == INVALID_HANDLE_VALUE)
	{
		_nFlags |= END_SEARCH;
	}
}

//-----------------------------------------------------------------------------
// Name: FIND_FILE destructor
// Object: 
//01-05-29 FKE Created: 
//-----------------------------------------------------------------------------
FIND_FILE::~FIND_FILE()
{
	FindClose(_pPltDatas->_Handle);
	delete _pPltDatas;
}

//-----------------------------------------------------------------------------
// Name: FIND_FILE::GetName
// Object: 
//01-05-29 FKE Created: 
//-----------------------------------------------------------------------------
const char *FIND_FILE::GetName() const
{
	return (&_pPltDatas->_FindFileData.cFileName[0]);
}

//-----------------------------------------------------------------------------
// Name: FIND_FILE::GetSize
// Object: 
//01-05-29 FKE Created: 
//-----------------------------------------------------------------------------
int FIND_FILE::GetSize() const
{
	return (_pPltDatas->_FindFileData.nFileSizeLow);
}

//-----------------------------------------------------------------------------
// Name: FIND_FILE::GetNext
// Object: 
//01-05-29 FKE Created: 
//-----------------------------------------------------------------------------
bool FIND_FILE::GetNext()
{
	if (_nFlags & END_SEARCH)
	{
		return false;
	}
	if (FindNextFile(_pPltDatas->_Handle, &_pPltDatas->_FindFileData) == FALSE)
	{
		_nFlags |= END_SEARCH;
	}
	return !(_nFlags & END_SEARCH);
}

//-----------------------------------------------------------------------------
// Name: FIND_FILE::IsExist
// Object: 
//01-05-29 FKE Created: 
//-----------------------------------------------------------------------------
bool FIND_FILE::IsExist() const
{
	return !(_nFlags & END_SEARCH);
}
//=============================================================================
// CODE ENDS HERE
//=============================================================================
