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
//	CLASS:	BLOC_READ_ENTITY
//
//	02-04-17:	CVI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
	
#ifdef _DEBUG
#else
#include INL_3DENGINE(SceneGraph\Node)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLOC_READ_ENTITY constructor
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_ENTITY::BLOC_READ_ENTITY()
:
_pBloc( NULL )
{
	
}

//-----------------------------------------------------------------------------
//	Name:		BLOC_READ_ENTITY constructor
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_ENTITY::BLOC_READ_ENTITY(const BLOC_READ_ENTITY & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BLOC_READ_ENTITY destructor
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_ENTITY::~BLOC_READ_ENTITY()
{
	_pBloc = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-17:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_READ_ENTITY &	BLOC_READ_ENTITY::operator=(const BLOC_READ_ENTITY & C)
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
void	BLOC_READ_ENTITY::Copy(const BLOC_READ_ENTITY & C)
{
	QDT_FAIL();
}


//-----------------------------------------------------------------------------
//	Name:		PreLoad
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_READ_ENTITY::PreLoad(READ_TASK *	pRT)
{
	COM_HANDLE	*pHandle;

	pHandle = GetComHandle();
	if ( pHandle->IsLoaded() )
	{
		NODE	*pN;

		pN = static_cast<NODE*>( pHandle->GetCommunicator() );

		pN->IncBlocReference();
		return ( false );
	}
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		ChildsPostLoad
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_READ_ENTITY::ChildsPostLoad(READ_TASK *	pRT)
{
	QDT_ASSERT( _pBloc );
	if ( _pBloc )
	{
		COM_INSTANCE	Inst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

		Inst.SetHandle(GetComHandle());
		_pBloc->AddNodeToSceneGraph( Inst );
	}
}


//-----------------------------------------------------------------------------
//	Name:		GetBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
BLOC *	BLOC_READ_ENTITY::GetBloc()
{
	return ( _pBloc );
}

//-----------------------------------------------------------------------------
//	Name:		SetBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_READ_ENTITY::SetBloc(BLOC *	pB)
{
	_pBloc = pB;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
