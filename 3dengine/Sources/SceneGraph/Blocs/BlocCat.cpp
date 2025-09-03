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
//	CLASS:	BLOC_CAT
//
//	02-04-15:	CVI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_KRNCORE(Stream)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(Streaming)
#include	INC_KRNCOM(CommunicatorType)
	
#include	INC_3DENGINE(m3d)
#include	INC_3DENGINE(Behavior)
#include	INC_3DENGINE(SceneGraph\Entities\SceneGraph)

#ifdef	_DEBUG
	#include "BlocCat.inl"
#else
	#include "Inline.h"
	#include INL_3DENGINE(SceneGraph/Node)
	#include INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include INL_KRNCORE(Math/Geometry/Matrix)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLOC_CAT constructor
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_CAT::BLOC_CAT()
:
COMMUNICATOR(CreateGenericCommunicatorDescriptor()),
_nMinX( 0 ),
_nMinY( 0 ),
_nMinZ( 0 ),
_nMaxX( 0 ),
_nMaxY( 0 ),
_nMaxZ( 0 ),
_nCenterX(1),
_nCenterY(1),
_nCenterZ(1),
_nNbrBlocs(1),
_rBlocSizeX(10000.0f),
_rBlocSizeY(10000.0f),
_rBlocSizeZ(10000.0f),
_nNbrBlocsX( 0 ),
_nNbrBlocsY( 0 ),
_nNbrBlocsZ( 0 ),
_nNbrBlocsFront( 1 ),
_nNbrBlocsBack( 1 ),
_nNbrBlocsOnSide( 1 ),
_nNbrBlocsUp( 1 ),
_nNbrBlocsDown( 1 ),
_BaseNodeInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_SGNodeInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_pCurrentReadTask( NULL ),
//_pTabOffsetIdx( NULL ),
//_pPartitionIdx( NULL )
_TabOffset( COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION ),
_Partition( COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION )
{
	_vBlocMatrix.Clear();
	_sBlocToUnload.Clear();
	_vBlocOffsets.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		BLOC_CAT constructor
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_CAT::BLOC_CAT(const BLOC_CAT & C)
:
COMMUNICATOR(&_CD)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BLOC_CAT destructor
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_CAT::~BLOC_CAT()
{
	ResetBlocCat( true );
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
BLOC_CAT &	BLOC_CAT::operator=(const BLOC_CAT & C)
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
void	BLOC_CAT::Copy(const BLOC_CAT & C)
{
	QDT_FAIL();
}


//-----------------------------------------------------------------------------
//	Name:		GetWorldCoord
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::GetWorldCoord(int			nX,
								int			nY,
								int			nZ,
								VECTOR &	P) const
{
	float				rX, rY, rZ;

	rX = ( nX + 0.5f ) * GetBlocSizeX();
	rY = ( nY + 0.5f ) * GetBlocSizeY();
	rZ = ( nZ + 0.5f ) * GetBlocSizeZ();

	rX += _Box.GetXMin();
	rY += _Box.GetYMin();
	rZ += _Box.GetZMin();

	P.SetXYZ( rX, rY, rZ );
}

//-----------------------------------------------------------------------------
//	Name:		GetWorldCoordFromMatrix
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::GetWorldCoordFromMatrix(int		nX,
										  int		nY,
										  int		nZ,
										  VECTOR &	P) const
{
	int		nX1, nY1, nZ1;

	nX1 = nX + _nMinX;
	nY1 = nY + _nMinY;
	nZ1 = nZ + _nMinZ;

	GetWorldCoord( nX1, nY1, nZ1, P );
}

//-----------------------------------------------------------------------------
//	Name:		GetCoordInBlocSpace
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::GetCoordInBlocSpace(const VECTOR &	P,
									  int &				nX,
									  int &				nY,
									  int &				nZ) const
{
	float	rX, rY, rZ;

	rX = P.GetX() - _Box.GetXMin();
	rY = P.GetY() - _Box.GetYMin();
	rZ = P.GetZ() - _Box.GetZMin();

	nX = static_cast<int> ( rX / GetBlocSizeX() );
	nY = static_cast<int> ( rY / GetBlocSizeY() );
	nZ = static_cast<int> ( rZ / GetBlocSizeZ() );
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::Init(const COM_INSTANCE &	BaseNodeInst,
					   bool					bForceLoad)
{
	int			nX, nY, nZ;
	int			n;

	n =  ( _nNbrBlocsX - 1 ) / 2;
	if ( n < _nNbrBlocsOnSide )
	{
		_nNbrBlocsOnSide = n;
	}

	n =  ( _nNbrBlocsY - 1 ) / 2;
	if ( n < _nNbrBlocsUp )
	{
		_nNbrBlocsUp = n;
	}

	n =  ( _nNbrBlocsY - 1 ) / 2;
	if ( n < _nNbrBlocsDown )
	{
		_nNbrBlocsDown = n;
	}

	n =  ( _nNbrBlocsZ - 1 ) / 2;
	if ( n < _nNbrBlocsFront )
	{
		_nNbrBlocsFront = n;
	}
	
	n =  ( _nNbrBlocsZ - 1 ) / 2;
	if ( n < _nNbrBlocsBack )
	{
		_nNbrBlocsBack = n;
	}

	ResetBlocCat( bForceLoad );
	SetBaseNodeInst( BaseNodeInst );

	if ( BaseNodeInst.HasHandle() == false )	return;
	UpdateCenter( nX, nY, nZ );
	UpdateMinMaxFromCenter();
	InitMatrix( bForceLoad );
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::Update()
{
	int		nDeltaX, nDeltaY, nDeltaZ;

	if ( _vBlocMatrix.GetSize() <= 0 )
	{
		InitMatrix();
	}
	UpdateCenter( nDeltaX, nDeltaY, nDeltaZ );
	UpdateMinMaxFromCenter();
	ProcessUnloadList();
	if ( nDeltaX != 0 || nDeltaY != 0 || nDeltaZ != 0 )
	{
		TranslateMatrix( nDeltaX, nDeltaY, nDeltaZ );
		CheckBlocsToBeUnloaded();
	}
	CheckBlocsToBeLoaded();
	PostReadTask();
}

//-----------------------------------------------------------------------------
//	Name:		ResetBlocCat
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::ResetBlocCat( bool bForceLoad )
{
	ResetMatrix();
}

//-----------------------------------------------------------------------------
//	Name:		TranslateMatrix
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::TranslateMatrix(int	nDeltaX,
								  int	nDeltaY,
								  int	nDeltaZ)
{
	int	nVal;

#if _DEBUG
	int		i, n;
	BLOC    *pB;
	
	n = _vBlocMatrix.GetSize();

	for ( i = 0; i < n; ++i )
	{
		pB = _vBlocMatrix[i];
		if ( !pB )
		{
			continue;
		}
	}
#endif


/*	nDeltaX = -nDeltaX;
	nDeltaY = -nDeltaY;
	nDeltaZ = -nDeltaZ;*/

	nVal = GetMatrixIndexFromMatrixCoord( nDeltaX, nDeltaY, nDeltaZ );
	MoveMatrixBuffer( nVal );
	
}

//-----------------------------------------------------------------------------
//	Name:		InitMatrix
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::InitMatrix(bool	bForceLoad)
{
	int			i, n;
	BLOC		*pB;
	

	n = GetMatrixNbrBlocs();
	_vBlocMatrix.Reserve( n );

	for ( i = 0; i < n; ++i )
	{
		pB = NULL;
		_vBlocMatrix.PushTail( pB );
	}
}


//-----------------------------------------------------------------------------
//	Name:		ResetMatrix
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::ResetMatrix()
{

	UnloadAllBlocs( true );
	_vBlocMatrix.Clear();

	int		n;

	n = ( _nNbrBlocsFront + _nNbrBlocsBack + 1 ) * ( _nNbrBlocsOnSide * 2 + 1 ) * ( _nNbrBlocsUp + _nNbrBlocsDown + 1 );
	_vBlocMatrix.Reserve( n);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMinMax
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::UpdateMinMax(int	nDeltaX,
							   int	nDeltaY,
							   int	nDeltaZ)
{
	_nMinX += nDeltaX;
	_nMaxX += nDeltaX;

	_nMinY += nDeltaY;
	_nMaxY += nDeltaY;

	_nMinZ += nDeltaZ;
	_nMaxZ += nDeltaZ;

}

//-----------------------------------------------------------------------------
//	Name:		UpdateMinMaxFromCenter
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::UpdateMinMaxFromCenter()
{
	_nMinX = _nCenterX - GetNbrBlocsOnSide();
	_nMaxX = _nCenterX + GetNbrBlocsOnSide();

	_nMinY = _nCenterY - GetNbrBlocsUp();
	_nMaxY = _nCenterY + GetNbrBlocsDown();

	_nMinZ = _nCenterZ - GetNbrBlocsBack();
	_nMaxZ = _nCenterZ + GetNbrBlocsAtFront();
}


//-----------------------------------------------------------------------------
//	Name:		UpdateCenter
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::UpdateCenter(int &	nDeltaX,
							   int &	nDeltaY,
							   int &	nDeltaZ)
{
	NODE			*pN;
	VECTOR			P;
	int				nX, nY, nZ;
	
	if ( _BaseNodeInst.HasHandle() == false )
	{
		pN = NULL;
	}
	else
	{
		pN = static_cast<NODE*>( _BaseNodeInst.GetCommunicator() );
	}

	if ( !pN )
	{
		nDeltaX = 0;
		nDeltaY = 0;
		nDeltaZ = 0;
		return;
	}

	pN->GetGlobalPosition( P );
	GetCoordInBlocSpace( P, nX, nY, nZ );

	nDeltaX = nX - _nCenterX;
	nDeltaY = nY - _nCenterY;
	nDeltaZ = nZ - _nCenterZ;

	_nCenterX = nX;
	_nCenterY = nY;
	_nCenterZ = nZ;
}

//-----------------------------------------------------------------------------
//	Name:		CheckBlocsToBeUnloaded
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::CheckBlocsToBeUnloaded()
{
	int			i, n;
	BLOC		*pB;

	n = _vBlocMatrix.GetSize();

	for ( i = 0; i < n; ++i )
	{
		pB = _vBlocMatrix[i];
		if ( !pB )	continue;
		if ( BlocIsInMatrix( pB ) == false )
		{
			UnloadBloc( pB );
			_vBlocMatrix[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMatrixBuffer
//	Object:		
//	02-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::UpdateMatrixBuffer(int	nX,
									 int	nY,
									 int	nZ)
{

	
}


//-----------------------------------------------------------------------------
//	Name:		MoveMatrixBuffer
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::MoveMatrixBuffer(int	nOffset)
{
	int			i, n;
	BLOC		*pB;
	int			nVal;

	n = _vBlocMatrix.GetSize();
	//	Rel move
	nOffset = - nOffset;


	if ( nOffset >= 0 )
	{
		//	Unload blocs that we know they'll be out 
		if ( nOffset <= n )
		{
			nVal = nOffset;
		}
		else
		{
			nVal = n;
		}
		for ( i = n - nVal; i < n; ++i )
		{
			pB = _vBlocMatrix[i];
			UnloadBloc( pB );
			_vBlocMatrix[i] = NULL;
		}
		for ( i = n - 1; i >= 0; --i )
		{
			nVal = i - nOffset;
			if ( nVal >= 0 )
			{
				pB = _vBlocMatrix[nVal];
			}
			else
			{
				pB = NULL;
			}
			_vBlocMatrix[i] = pB;
		}
	}
	else
	{
		if ( -nOffset <= n )
		{
			nVal = nOffset;
		}
		else
		{
			nVal = -n;
		}
		for ( i = 0; i < -nVal; ++i )
		{
			pB = _vBlocMatrix[i];
			UnloadBloc( pB );
			_vBlocMatrix[i] = NULL;
		}

		for ( i = 0; i < n; ++i )
		{
			nVal = i - nOffset;
			if ( nVal < n )
			{
				pB = _vBlocMatrix[nVal];
			}
			else
			{
				pB = NULL;
			}
			_vBlocMatrix[ i ] = pB;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CheckBlocsToBeLoaded
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::CheckBlocsToBeLoaded()
{
	BLOC			*pB;
	int				i, n;
	int				nIndex;

	if ( UnloadListIsEmpty() == false )	return;

	n = _vBlocMatrix.GetSize();

	for ( i = 0; i < n ; ++i )
	{
		pB = _vBlocMatrix[i];
		if ( pB )	continue;

		nIndex = GetGlobalIndexFromMatrixIndex( i );
		if ( nIndex < 0 )	continue;
		if ( nIndex >= GetNbrBlocs() )
		{
			pB = NULL;
		}
		else
		{
			pB = LoadBloc( nIndex );
		}
		_vBlocMatrix[i] = pB;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PostReadTask
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::PostReadTask()
{
	if ( _pCurrentReadTask )
	{
		FILES_MANAGER::AddRead(FILES_MANAGER::FM_NORMAL_PRIORITY, _pCurrentReadTask );
	}
	_pCurrentReadTask = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		UnloadAllBlocs
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::UnloadAllBlocs(bool	bBlocking)
{
	int		i, n;
	BLOC	*pB;

	n = _vBlocMatrix.GetSize();
	for ( i = 0; i < n; ++i )
	{
		pB = _vBlocMatrix[i];
		if ( pB )
		{
			int				nIndex;
			//PARTITIONS		*pP;
			COMMUNICATOR_PARTITION		*pP;

			FindBlocInMatrix( pB );
//			pB->UnloadBlocking();
			nIndex = pB->GetIndex();
			QDT_ASSERT( nIndex != -1 );
			pB->SetIndex( -1 );
			delete pB;

			//pP = static_cast<PARTITIONS*> ( _pPartitionIdx->GetObject() );
			pP = static_cast<COMMUNICATOR_PARTITION*> ( _Partition.GetCommunicator() );				
			pP->UnLoad( nIndex );
		}
		_vBlocMatrix[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		LoadBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
BLOC *	BLOC_CAT::LoadBloc(int	nBlocIndex)
{
	BLOC	*pB;

	pB = NULL;
	pB = ReadBloc( nBlocIndex );
	if ( pB )
	{
		if ( !_pCurrentReadTask )
		{
			_pCurrentReadTask = new READ_TASK();
		}
		pB->SetSGNodeInst( GetSGNodeInst() );
		pB->LoadEntities( _pCurrentReadTask );
	}
	return ( pB );
}

//-----------------------------------------------------------------------------
//	Name:		ReadBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
BLOC *	BLOC_CAT::ReadBloc(int	nBlocIndex)
{
#ifndef _MASTER
	BLOC			*pB;
	//PARTITIONS		*pP;
	COMMUNICATOR_PARTITION		*pP;
	int				nSize;
	MEM_STREAM		*pStream;
	
	//pP = static_cast<PARTITIONS*> ( _pPartitionIdx->GetObject() );
	pP = static_cast<COMMUNICATOR_PARTITION*> ( _Partition.GetCommunicator() );
	if ( pP->IsLoaded( nBlocIndex ) == false )
	{
		pP->Load( nBlocIndex, FILES_MANAGER::FM_NORMAL_PRIORITY );
		return ( NULL );
	}

	nSize = -1;
	pStream = pP->GetPartition( nBlocIndex );
	pB = new BLOC();
	pB->ReadInBuffer( pStream );
	return ( pB );
#else
	// TODO Gestion via Databank
	return NULL;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		UnloadBloc
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::UnloadBloc(BLOC *	pBloc)
{
	if ( !pBloc )	return;

	if ( pBloc->Unload() )
	{
		int				nIndex;
		//PARTITIONS		*pP;
		COMMUNICATOR_PARTITION		*pP;

		//pP = static_cast<PARTITIONS*> ( _pPartitionIdx->GetObject() );
		pP = static_cast<COMMUNICATOR_PARTITION*> ( _Partition.GetCommunicator() );				
		
		nIndex = pBloc->GetIndex();

		pBloc->SetIndex( -1 );

		delete pBloc;
		pP->UnLoad( nIndex );

	}
	else
	{
		AddBlocToUnload( pBloc );
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddBlocToUnload
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::AddBlocToUnload(BLOC *	pB)
{
	_sBlocToUnload.InsertHead( pB );
}

//-----------------------------------------------------------------------------
//	Name:		ProcessUnloadList
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::ProcessUnloadList()
{
	QDT_DLIST<BLOC*>::ITERATOR			It;
	QDT_DLIST<BLOC*>::ITERATOR			ItPrev;
	QDT_DLIST<BLOC*>::ITERATOR			ItLast;
	QDT_DLIST<BLOC*>::ITERATOR			ItHead;
	BLOC								*pB;
	//PARTITIONS							*pP;
	COMMUNICATOR_PARTITION				*pP;

	if ( _sBlocToUnload.GetSize() <= 0 )	return;

	//pP = static_cast<PARTITIONS*> ( _pPartitionIdx->GetObject() );
	pP = static_cast<COMMUNICATOR_PARTITION*> ( _Partition.GetCommunicator() );

	ItHead	= _sBlocToUnload.GetHead();
	ItLast	= _sBlocToUnload.GetTail();
	It		= ItHead;

	while ( It != ItLast )
	{
		pB = *It;
		if ( !pB )
		{
			QDT_FAIL();
			It++;
			continue;
		}
		else
		{
			if ( pB->Unload() )
			{
				_sBlocToUnload.Remove( It );
			}
		}
		return;
	}
	
}

//-----------------------------------------------------------------------------
//	Name:		ProcessBlockingload
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::ProcessBlockingload()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		BlocIsInMatrix
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_CAT::BlocIsInMatrix(BLOC *	pB)
{
	int				nOffset;
	int				nX, nY, nZ;
	bool			bVal;

	if ( !pB )	return ( false );
	nOffset = pB->GetIndex();

	GetCoordFromIndex( nOffset, nX, nY, nZ );
	bVal = IsInMatrix( nX, nY, nZ );
	return (  bVal );

}

//-----------------------------------------------------------------------------
//	Name:		WriteVersion2
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
bool	BLOC_CAT::WriteVersion2(class IO_STREAM &	Stream) const
{

	WriteAttributes( Stream );

	/*OBJECT_ID		Id;

	if ( _pTabOffsetIdx )
	{
		Id = _pTabOffsetIdx->GetObjectId();
		Stream << Id.GetType();
		Stream << Id.GetID();
	}

	if ( _pPartitionIdx )
	{
		Id = _pPartitionIdx->GetObjectId();
		Stream << Id.GetType();
		Stream << Id.GetID();
	}*/

	if ( _TabOffset.HasHandle() )
	{
		Stream << _TabOffset.GetType();
		Stream << _TabOffset.GetID();
	}

	if ( _Partition.HasHandle() )
	{
		Stream << _Partition.GetType();
		Stream << _Partition.GetID();
	}

	WriteDynamicObjects( Stream );
	return ( true );

}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadVersion2
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_CAT::ReadVersion2(class IO_STREAM &	Stream)
{
	ReadAttributes( Stream );

	OBJECT_ID		Id;
	int				nId;
	OBJECT_TYPE		nType;

	Stream >> nType;
	Stream >> nId;
	//_pTabOffsetIdx = DATABASE_MANAGER::Instance()->GetById( nType, nId );
	_TabOffset = COM_SERVICE::RetrieveComHandle( nType, nId );

	Stream >> nType;
	Stream >> nId;
	//_pPartitionIdx = DATABASE_MANAGER::Instance()->GetById( nType, nId );
	_Partition = COM_SERVICE::RetrieveComHandle( nType, nId );

	ReadDynamicObjects( Stream );
	return ( true );
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		ReadAttributes
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_CAT::ReadAttributes(class IO_STREAM &	Stream)
{
	Stream >> _Box;
	Stream >> _nMinX;
	Stream >> _nMinY;
	Stream >> _nMinZ;
	Stream >> _nMaxX;
	Stream >> _nMaxY;
	Stream >> _nMaxZ;
	Stream >> _nNbrBlocs;
	Stream >> _rBlocSizeX;
	Stream >> _rBlocSizeY;
	Stream >> _rBlocSizeZ;
	Stream >> _nNbrBlocsX;
	Stream >> _nNbrBlocsY;
	Stream >> _nNbrBlocsZ;
	Stream >> _nNbrBlocsFront;
	Stream >> _nNbrBlocsBack;
	Stream >> _nNbrBlocsOnSide;
	Stream >> _nNbrBlocsUp;				
	Stream >> _nNbrBlocsDown;			

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ReadDynamicObjects
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_CAT::ReadDynamicObjects(class IO_STREAM &	Stream)
{
	int	i, n;

	n = _DynObjListInst.GetSize();
	Stream >> n;

	for ( i = 0; i < n; ++i )
	{
		Stream >> _DynObjListInst[i];
	}
	return ( true );
}

#endif

//-----------------------------------------------------------------------------
//	Name:		WriteAttributes
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
bool	BLOC_CAT::WriteAttributes(class IO_STREAM &	Stream) const
{
	Stream << _Box;
	Stream << _nMinX;
	Stream << _nMinY;
	Stream << _nMinZ;
	Stream << _nMaxX;
	Stream << _nMaxY;
	Stream << _nMaxZ;
	Stream << _nNbrBlocs;
	Stream << _rBlocSizeX;
	Stream << _rBlocSizeY;
	Stream << _rBlocSizeZ;
	Stream << _nNbrBlocsX;
	Stream << _nNbrBlocsY;
	Stream << _nNbrBlocsZ;
	Stream << _nNbrBlocsFront;
	Stream << _nNbrBlocsBack;
	Stream << _nNbrBlocsOnSide;
	Stream << _nNbrBlocsUp;				
	Stream << _nNbrBlocsDown;			
	return ( true );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteDynamicObjects
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
bool	BLOC_CAT::WriteDynamicObjects(class IO_STREAM &	Stream) const
{
	int	i, n;

	n = _DynObjListInst.GetSize();
	Stream << n;

	for ( i = 0; i < n; ++i )
	{
		Stream << _DynObjListInst[i];
	}
	return ( true );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		FindBlocInMatrix
//	Object:		
//	02-06-05:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::FindBlocInMatrix(BLOC *	pB)
{
	int			i, n;
	BLOC		*pB2;
	bool		bFound;
	int			nVal;

	bFound	= false;
	nVal	= -1;

	n = _vBlocMatrix.GetSize();
	for ( i = 0; i < n; ++i )
	{
		pB2 = _vBlocMatrix[i];
		if ( pB2 == pB )
		{
			QDT_ASSERT( bFound == false );
			bFound = true;
			nVal = i;
		}
	}
	return ( nVal );
}

//-----------------------------------------------------------------------------
//	Name:		AddDynObjects
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::AddDynObjects()
{
	int		i, n;
	NODE	*pSGNode;

	if ( _SGNodeInst.HasHandle() == false )
	{
		QDT_FAIL();
		return;
	}
	pSGNode = static_cast<NODE*>( _SGNodeInst.GetCommunicator() );

	n = _DynObjListInst.GetSize();
	if ( n <= 0 )	return;

	for ( i = 0; i < n; ++i )
	{
		pSGNode->AttachChild( _DynObjListInst[i] );
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveDynObjects
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::RemoveDynObjects()
{
	int				i, n;
	COM_INSTANCE	Inst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
	NODE			*pN;


	n = _DynObjListInst.GetSize();
	if ( n <= 0 )	return;

	for ( i = 0; i < n; ++i )
	{
		Inst = _DynObjListInst[i];
		if ( Inst.HasHandle() == false ) continue;
		
		pN = static_cast<NODE*>( Inst.GetCommunicator() );
		pN->DetachFromFather();
		Inst.Release();
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR		*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Methodes	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetGraphNode"), QDT_COMMENT("Set Graph Node to attach to."));
	pMD->AddParameter(QDT_COMMENT("GraphNode"), QDT_COMMENT("The node index (old com)"), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_GRAPH_NODE, pMD, EM_SetGraphNode);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetBaseNode"), QDT_COMMENT("Set the Node to be used for position update."));
	pMD->AddParameter(QDT_COMMENT("BaseNode"), QDT_COMMENT("The node index (old com)"), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_BASE_NODE, pMD, EM_SetBaseNode);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetMatrixWidth"), QDT_COMMENT("Set the width of the matrix."));
	pMD->AddParameter(QDT_COMMENT("Width"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Vertical Size"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_MATRIX_WIDTH, pMD, EM_SetMatrixWidth);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("BLOC_CAT", 2, CM_ReadChunk);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("BLOC_CAT", 2, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	BLOC_CAT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLOC_CAT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	BLOC_CAT::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("BLOC_CAT"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLOC_CAT,
										QDT_COMMENT("7323A0FF-4F08-0E19-2561-D21624539487"),
										sizeof(BLOC_CAT),
										ComInit,
										QDT_COMMENT("Class for bloc_cat"),
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));

}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::EM_Construct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	BLOC_CAT	*pBlocCat;
	pBlocCat = static_cast<BLOC_CAT *>(pO);

	QDT_NEW_PLACED(pBlocCat, BLOC_CAT)();
	QDT_ASSERT(pBlocCat);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::EM_Destruct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	BLOC_CAT	*pBlocCat;
	pBlocCat = static_cast<BLOC_CAT *>(pO);

	QDT_DELETE_PLACED(pBlocCat, BLOC_CAT);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetGraphNode
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::EM_SetGraphNode(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pInst;
	BLOC_CAT		*pBlocCat;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pBlocCat		= static_cast<BLOC_CAT *>(pComInstance->GetCommunicator());
	pInst			= static_cast<COM_INSTANCE *>(pP);

	if (pInst->GetType() != MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE)
	{
		QDT_Warning(QDT_COMMENT("Command aborted : the parameter is not a node"));
		return;
	}

	pBlocCat->SetSGNodeInst(*pInst);

	NODE		*pN;
	SCENE_GRAPH	*pSG;

	pN	= static_cast<NODE*>(pInst->GetCommunicator());
	pSG	= static_cast<SCENE_GRAPH*>(pN->GetEntityInst().GetCommunicator());
	pSG->AddBlocCat(COM_INSTANCE(pBlocCat));
	pBlocCat->Init(COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetBaseNode
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::EM_SetBaseNode(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE	*pInst;
	BLOC_CAT		*pBlocCat;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pBlocCat		= static_cast<BLOC_CAT *>(pComInstance->GetCommunicator());
	pInst			= static_cast<COM_INSTANCE *>(pP);

	if (pInst->GetType() != MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE)
	{
		QDT_Warning(QDT_COMMENT("Command aborted : the parameter is not a node"));
		return;
	}

	pBlocCat->SetBaseNodeInst(*pInst);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMatrixWidth
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::EM_SetMatrixWidth(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	struct	PARAMETERS
	{
		int	_nSize;
		int	_nVertSize;
	};

	PARAMETERS		*pParameters;
	BLOC_CAT		*pBlocCat;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pBlocCat		= static_cast<BLOC_CAT *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	pBlocCat->SetNbrBlocsAtFront(pParameters->_nSize);
	pBlocCat->SetNbrBlocsOnSide(pParameters->_nSize);
	pBlocCat->SetNbrBlocsBack(pParameters->_nSize);
	pBlocCat->SetNbrBlocsUp(pParameters->_nVertSize);
	pBlocCat->SetNbrBlocsDown(pParameters->_nVertSize);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	BLOC_CAT::CM_ReadChunk(IO_STREAM &		Stream,
							   COMMUNICATOR *	pCommunicator)
{
	BLOC_CAT	*pBlocCat;
	pBlocCat = static_cast<BLOC_CAT *>(pCommunicator);

	pBlocCat->ReadVersion2(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	BLOC_CAT::CM_WriteChunk(IO_STREAM &		Stream,
								COMMUNICATOR *	pCommunicator)
{
	BLOC_CAT	*pBlocCat;
	pBlocCat = static_cast<BLOC_CAT *>(pCommunicator);

	pBlocCat->WriteVersion2(Stream);
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
