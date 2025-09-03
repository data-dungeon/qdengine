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
//	CLASS:	BLOC_READ_TASK
//
//	02-04-17:	CVI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#ifdef	_DEBUG
#else
#include "Inline.h"
#endif


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLOC_READ_TASK constructor
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_TASK::BLOC_READ_TASK()
:
_pBloc ( NULL )
{

}

//-----------------------------------------------------------------------------
//	Name:		BLOC_READ_TASK constructor
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_TASK::BLOC_READ_TASK(const BLOC_READ_TASK & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BLOC_READ_TASK destructor
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_TASK::~BLOC_READ_TASK()
{
	_pBloc = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_TASK &	BLOC_READ_TASK::operator=(const BLOC_READ_TASK & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_READ_TASK::Copy(const BLOC_READ_TASK & C)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		PreLoad
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_READ_TASK::PreLoad(READ_TASK *	pRT)
{
	_pBloc->SetLoading();
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ChildsPostLoad
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_READ_TASK::ChildsPostLoad(READ_TASK *	pRT)
{
	_pBloc->SetLoaded();
}

//-----------------------------------------------------------------------------
//	Name:		GetBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
BLOC *	BLOC_READ_TASK::GetBloc()
{
	return ( _pBloc );
}

//-----------------------------------------------------------------------------
//	Name:		SetBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_READ_TASK::SetBloc(BLOC *	pB)
{
	_pBloc = pB;
}




//=============================================================================
//	CODE ENDS HERE
//=============================================================================
