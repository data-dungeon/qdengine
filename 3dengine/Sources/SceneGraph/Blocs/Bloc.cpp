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
//	CLASS:	BLOC
//
//	02-04-15:	CVI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(OldCom/Database/DatabaseManager)

#ifdef	_DEBUG
#include "Bloc.inl"
#else
#include "Inline.h"
#include INL_3DENGINE(SceneGraph\Node)
#endif


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLOC constructor
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC::BLOC()
:
_nIndex( -1 ),
_SGNodeInst( COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION )
{
	_vNodeInst.Clear();
	_Flags = 0;
}

//-----------------------------------------------------------------------------
//	Name:		BLOC constructor
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC::BLOC(const BLOC & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BLOC destructor
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC::~BLOC()
{
	_vNodeInst.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC &	BLOC::operator=(const BLOC & C)
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
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::Copy(const BLOC & C)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		LoadEntities
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::LoadEntities(READ_TASK *	pFatherReadTask)
{
	int					i, n;
	COM_INSTANCE		Inst( COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION );
	BLOC_READ_TASK		*pBRT;
	bool				bAllLoaded;

	n = GetNbrNodes();
	if ( n <= 0 )
	{
		SetLoaded();
		return;
	}

	bAllLoaded	= true;
	pBRT		= new BLOC_READ_TASK();
	pBRT->SetBloc( this );
	pFatherReadTask->PushLastReadTask( pBRT );


	for ( i = 0; i < n; ++i )
	{
		Inst = GetNodeInst( i );
		if ( MustLoadNode( Inst ) )
		{
			AddNodeToLoad( pBRT, Inst );
		}
		else
		{
			AddNodeToSceneGraph( Inst );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unload
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC::Unload()
{


	int				i, n;
	COM_INSTANCE	Inst( COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION );
	NODE			*pN;

	n = GetNbrNodes();

	if ( IsLoaded() == false )
	{
		return ( false );
	}

	for (i = 0; i < n; ++i)
	{
		Inst = GetNodeInst(i);
		pN = static_cast<NODE*>(Inst.GetCommunicator());

		if (!pN)
		{
			QDT_FAIL();
			continue;
		}

		if ( pN->DecBlocReference() <= 0 )
		{
			pN->DetachFromFather();
			Inst.Release();
		}
	}
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		UnloadBlocking
//	Object:		
//	02-06-12:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::UnloadBlocking()
{
	/*if ( Unload() )
	{
		return;
	}*/

	int				i, n;
	COM_INSTANCE	Inst;
	NODE			*pN;

	n = GetNbrNodes();

	for ( i = 0; i < n; ++i )
	{
		Inst = GetNodeInst( i );

		if ( Inst.IsLoaded() == false )
		{
			pN = static_cast<NODE*>(Inst.GetCommunicator());
			pN->ForceLoad();
		}
	}
	QDT_ASSERT( IsLoaded() );

	for ( i = 0; i < n; ++i )
	{
		Inst = GetNodeInst( i );
		pN = static_cast<NODE*>(Inst.GetCommunicator());

		if ( !pN )
		{
			QDT_FAIL();
			continue;
		}

		if ( pN->DecBlocReference() <= 0 )
		{
			pN->DetachFromFather();
			Inst.Release();
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		AddNodeToSceneGraph
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::AddNodeToSceneGraph(const COM_INSTANCE &	Inst)
{
	if ( _SGNodeInst.HasHandle() == false )	return;

	NODE	*pN0;
	NODE	*pN1;
	int		nRef;

	pN0 = static_cast<NODE*>( _SGNodeInst.GetCommunicator() );
	if ( !pN0 )
	{
		QDT_FAIL();
		return;
	}
	
	pN1 = static_cast<NODE*>( Inst.GetCommunicator() );
	nRef = pN1->GetBlocReference();
	if ( nRef <= 0 )
	{
		pN1->ResetBlocReference();
		pN0->AttachChild( Inst );
	}
	pN1->IncBlocReference();
}

//-----------------------------------------------------------------------------
//	Name:		MustLoadNode
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC::MustLoadNode(const COM_INSTANCE & Inst)
{
	if ( Inst.HasHandle() == false )	return ( false );

	if ( Inst.IsLoaded() )
	{
		return ( false );
	}
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		AddNodeToLoad
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::AddNodeToLoad(READ_TASK *				pRT,
							const COM_INSTANCE &	Inst)
{
	BLOC_READ_ENTITY	*pBRE;

	pBRE = new BLOC_READ_ENTITY();
	pBRE->SetComHandle( Inst.GetHandle() );
	pBRE->SetBloc( this );
	pRT->PushLastReadTask( pBRE );
}


//-----------------------------------------------------------------------------
//	Name:		GetSizeToWrite
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC::GetSizeToWrite()
{
	int		nSize;

	nSize = 0;

	nSize += sizeof( int );
	nSize += sizeof( AABB );
	nSize += sizeof( int );
	nSize += GetNbrNodes() * sizeof( int );
	nSize += GetNbrNodes() * sizeof( OBJECT_TYPE );
	return ( nSize );
}

//-----------------------------------------------------------------------------
//	Name:		WriteInBuffer
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	BLOC::WriteInBuffer( MEM_STREAM *pStream )
{
	int				i, n;
	OBJECT_ID		Id;

	(*pStream) << _nIndex;
	(*pStream) << _Box;
	
	n	= GetNbrNodes();

	(*pStream) << n;

	for ( i = 0; i < n; ++i )
	{
		(*pStream) << GetNodeInst( i );
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadInBuffer
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	BLOC::ReadInBuffer( MEM_STREAM *pStream )
{
	int				i, n;
	COM_INSTANCE	Inst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

	if ( pStream->IsWrite() )
	{
		pStream->SwitchWriteToRead();

	}
	pStream->Restart();
	
	(*pStream) >> _nIndex;
	(*pStream) >> _Box;
	(*pStream) >> n;

	for ( i = 0; i < n; ++i )
	{
		(*pStream) >> Inst;
		AddNodeInst( Inst );
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CheckAllNodesLoaded
//	Object:		
//	02-06-11:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC::CheckAllNodesLoaded()
{
//	int			i, n;
//	INDEX_INT	*pIdx;
		

	return ( false );
/*	n = GetNbrNodes();

	if ( n <= 0 )
	{
		SetLoaded();
		return ( true );
	}


	for ( i = 0; i < n; ++i )
	{
		pIdx = GetNodeIdx( i );
		if ( pIdx->IsLoaded() == false )
		{
			SetLoaded( false );
			return ( false );
		}
	}
	SetLoaded();
	return ( true );*/
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
