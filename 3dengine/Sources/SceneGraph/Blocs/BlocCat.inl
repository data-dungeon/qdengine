//-----------------------------------------------------------------------------
//	Name:		GetBox
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
const AABB &	BLOC_CAT::GetBox() const
{
	return ( _Box );
}

//-----------------------------------------------------------------------------
//	Name:		GetMinX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMinX() const
{
	return ( _nMinX );
}

//-----------------------------------------------------------------------------
//	Name:		GetMinY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMinY() const
{
	return ( _nMinY );
}

//-----------------------------------------------------------------------------
//	Name:		GetMinZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMinZ() const
{
	return ( _nMinZ );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMaxX() const
{
	return ( _nMaxX );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMaxY() const
{
	return ( _nMaxY );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMaxZ() const
{
	return ( _nMaxZ );
}

//-----------------------------------------------------------------------------
//	Name:		GetCenterX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetCenterX() const
{
	return ( _nCenterX );
}

//-----------------------------------------------------------------------------
//	Name:		GetCenterY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetCenterY() const
{
	return ( _nCenterY );
}

//-----------------------------------------------------------------------------
//	Name:		GetCenterZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetCenterZ() const
{
	return ( _nCenterZ );
}

//-----------------------------------------------------------------------------
//	Name:		GetBlocSizeX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
float	BLOC_CAT::GetBlocSizeX() const
{
	return ( _rBlocSizeX );
}

//-----------------------------------------------------------------------------
//	Name:		GetBlocSizeY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
float	BLOC_CAT::GetBlocSizeY() const
{
	return ( _rBlocSizeY );
}

//-----------------------------------------------------------------------------
//	Name:		GetBlocSizeZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
float	BLOC_CAT::GetBlocSizeZ() const
{
	return ( _rBlocSizeZ );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsAtFront
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsAtFront() const
{
	return ( _nNbrBlocsFront );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsBack
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsBack() const
{
	return ( _nNbrBlocsBack );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsOnSide
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsOnSide() const
{
	return ( _nNbrBlocsOnSide );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsUp
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsUp() const
{
	return ( _nNbrBlocsUp );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsDown
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsDown() const
{
	return ( _nNbrBlocsDown );
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseNodeInst
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	BLOC_CAT::GetBaseNodeInst() const
{
	return ( _BaseNodeInst );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocs
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocs() const
{
	return ( _nNbrBlocs );
}

//-----------------------------------------------------------------------------
//	Name:		GetMatrixNbrBlocs
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMatrixNbrBlocs() const
{
	int		nA, nB, nC;

	nA = GetNbrBlocsAtFront() + GetNbrBlocsBack() + 1;
	nB = 2 * GetNbrBlocsOnSide() + 1;
	nC = GetNbrBlocsUp() + GetNbrBlocsDown() + 1;

	return ( nA * nB * nC );
}

//-----------------------------------------------------------------------------
//	Name:		GetMatrixNbrBlocsX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMatrixNbrBlocsX() const
{
	return ( GetNbrBlocsOnSide() * 2 + 1 );
}

//-----------------------------------------------------------------------------
//	Name:		GetMatrixNbrBlocsY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMatrixNbrBlocsY() const
{
	return ( GetNbrBlocsUp() + GetNbrBlocsDown() + 1 );
}

//-----------------------------------------------------------------------------
//	Name:		GetMatrixNbrBlocsZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMatrixNbrBlocsZ() const
{
	return ( GetNbrBlocsAtFront() + GetNbrBlocsBack() + 1);
}


//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsX() const
{
	return ( _nNbrBlocsX );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsY() const
{
	return ( _nNbrBlocsY );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlocsZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetNbrBlocsZ() const
{
	return ( _nNbrBlocsZ );
}


//-----------------------------------------------------------------------------
//	Name:		GetBlocOffset
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetBlocOffset(int	nNumBloc)
{
	if ( nNumBloc >= GetNbrBlocs() )	return ( -1 );

	OFFSET_PARTITION *	pOP;

	pOP = static_cast<OFFSET_PARTITION*> ( _TabOffset.GetCommunicator() );
//	return ( _vBlocOffsets[nNumBloc] );
	return ( pOP->GetPosition( nNumBloc ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetSGNodeInst
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	BLOC_CAT::GetSGNodeInst() const
{
	return ( _SGNodeInst );
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentReadTask
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
READ_TASK*	BLOC_CAT::GetCurrentReadTask()
{
	return ( _pCurrentReadTask );
}

//-----------------------------------------------------------------------------
//	Name:		GetIndexFromCoord
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetIndexFromCoord(int	nX,
								int	nY,
								int	nZ) const
{
	return ( nX + GetNbrBlocsX() * nY + GetNbrBlocsX() * GetNbrBlocsY() * nZ );
}

//-----------------------------------------------------------------------------
//	Name:		GetCoordFromIndex
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::GetCoordFromIndex(int		nIndex,
									int &	nX,
									int &	nY,
									int &	nZ) const
{
	int		m, n;

	//
	m = GetNbrBlocsX();
	n = m * GetNbrBlocsY();
	//
	nZ		=	nIndex / n;
	nIndex -=	nZ * n;
	//
	nY		=	nIndex / m;
	nIndex -=	nY * m;
	//
	nX		= nIndex;
	//
	QDT_ASSERT( nX < _nNbrBlocsX );
	QDT_ASSERT( nY < _nNbrBlocsY );
	QDT_ASSERT( nZ < _nNbrBlocsZ );
}

//-----------------------------------------------------------------------------
//	Name:		IsInMatrix
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_CAT::IsInMatrix(int	nX,
							 int	nY,
							 int	nZ)
{
	if ( nX > GetMaxX() )	return ( false );
	if ( nX < GetMinX() )	return ( false );

	if ( nY > GetMaxY() )	return ( false );
	if ( nY < GetMinY() )	return ( false );

	if ( nZ > GetMaxZ() )	return ( false );
	if ( nZ < GetMinZ() )	return ( false );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalIndexFromMatrixIndex
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetGlobalIndexFromMatrixIndex(int	nIndex)
{
	int		m, n;
	int		nX;
	int		nY;
	int		nZ;

	//
	m = GetMatrixNbrBlocsX();
	n = m * GetMatrixNbrBlocsY();
	//
	nZ		=	nIndex / n;
	nIndex -=	nZ * n;
	//
	nY		=	nIndex / m;
	nIndex -=	nY * m;
	//
	nX		= nIndex;

	////////////////////
	nX		+= GetMinX();
	nY		+= GetMinY();
	nZ		+= GetMinZ();

	if ( nX < 0 || nY < 0 || nZ < 0 )
	{
		return ( -1 );
	}
	if ( nX >= _nNbrBlocsX || nY >= _nNbrBlocsY || nZ >= _nNbrBlocsZ )
	{
		return ( -1 );
	}
	nIndex = GetIndexFromCoord( nX, nY, nZ );
	return ( nIndex );
}

//-----------------------------------------------------------------------------
//	Name:		UnloadListIsEmpty
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC_CAT::UnloadListIsEmpty() const
{
	if ( _sBlocToUnload.GetSize() >= 0 )	return ( true );
	return ( true );
}	

//-----------------------------------------------------------------------------
//	Name:		SetBox
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetBox(const AABB &	Box)
{
	_Box = Box;
}

//-----------------------------------------------------------------------------
//	Name:		SetMinX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetMinX(int	nVal)
{
	_nMinX = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetMinY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetMinY(int	nVal)
{
	_nMinY = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetMinZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetMinZ(int	nVal) 
{
	_nMinZ = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetMaxX(int	nVal) 
{
	_nMaxX = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetMaxY(int	nVal) 
{
	_nMaxY= nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetMaxZ(int	nVal) 
{
	_nMaxZ = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetCenterX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetCenterX(int	nVal) 
{
	_nCenterX = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetCenterY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetCenterY(int	nVal) 
{
	_nCenterY = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetCenterZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetCenterZ(int	nVal) 
{
	_nCenterZ = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlocSizeX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetBlocSizeX(float	rVal) 
{
	_rBlocSizeX = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlocSizeY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetBlocSizeY(float	rVal) 
{
	_rBlocSizeY = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlocSizeZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetBlocSizeZ(float	rVal) 
{
	_rBlocSizeZ = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsAtFront
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsAtFront(int	nVal) 
{
	_nNbrBlocsFront = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsBack
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsBack(int	nVal) 
{
	_nNbrBlocsBack = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsOnSide
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsOnSide(int	nVal) 
{
	_nNbrBlocsOnSide = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsUp
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsUp(int	nVal) 
{
	_nNbrBlocsUp = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsDown
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsDown(int	nVal) 
{
	_nNbrBlocsDown = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetBaseNodeInst
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetBaseNodeInst(const COM_INSTANCE &	Inst)
{
	_BaseNodeInst = Inst;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocs
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocs(int	nVal) 
{
	_nNbrBlocs = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsX
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsX(int	nVal) 
{
	_nNbrBlocsX = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsY
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsY(int	nVal) 
{
	_nNbrBlocsY = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlocsZ
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetNbrBlocsZ(int	nVal) 
{
	_nNbrBlocsZ = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlocOffset
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetBlocOffset(int	nNumBloc, int nOffset )
{
	if ( nNumBloc >= GetNbrBlocs() )
	{
		QDT_FAIL();
		return;
	}

	_vBlocOffsets[nNumBloc] = nOffset;
}

//-----------------------------------------------------------------------------
//	Name:		SetSGNodeInst
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetSGNodeInst(const COM_INSTANCE &	Inst)
{
	_SGNodeInst = Inst;
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentReadTask
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetCurrentReadTask(READ_TASK *	pRT)
{
	_pCurrentReadTask = pRT;
}

//-----------------------------------------------------------------------------
//	Name:		GetMatrixIndexFromMatrixCoord
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC_CAT::GetMatrixIndexFromMatrixCoord(int	nX,
											int	nY,
											int	nZ)
{
	int		m, n;
	int		nRes;

	m = GetMatrixNbrBlocsX();
	n = m * GetMatrixNbrBlocsY();

	nRes = nX + nY * m + nZ * n;
	return ( nRes );
}


//-----------------------------------------------------------------------------
//	Name:		GetTabOffsetIdx
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
/*INDEX_INT *	BLOC_CAT::GetTabOffsetIdx()
{
	return ( _pTabOffsetIdx );
}

//-----------------------------------------------------------------------------
//	Name:		GetPartitionIdx
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
INDEX_INT *	BLOC_CAT::GetPartitionIdx()
{
	return( _pPartitionIdx );
}*/

//-----------------------------------------------------------------------------
//	Name:		GetTabOffset
//	Object:		
//	03-01-30:	AMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	BLOC_CAT::GetTabOffset()
{
	return (_TabOffset);
}

//-----------------------------------------------------------------------------
//	Name:		GetPartition
//	Object:		
//	03-01-30:	AMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	BLOC_CAT::GetPartition()
{
	return (_Partition);
}

//-----------------------------------------------------------------------------
//	Name:		SetTabOffsetIdx
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
/*void	BLOC_CAT::SetTabOffsetIdx(INDEX_INT *	pIdx)
{
	_pTabOffsetIdx = pIdx;
}

//-----------------------------------------------------------------------------
//	Name:		SetPartitionIdx
//	Object:		
//	02-05-23:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetPartitionIdx(INDEX_INT *	pIdx)
{
	_pPartitionIdx = pIdx;
}*/

//-----------------------------------------------------------------------------
//	Name:		SetTabOffset
//	Object:		
//	03-01-30:	AMA - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetTabOffset(COM_INSTANCE	CI)
{
	_TabOffset = CI;
}

//-----------------------------------------------------------------------------
//	Name:		SetPartition
//	Object:		
//	03-01-30:	AMA - Created
//-----------------------------------------------------------------------------
void	BLOC_CAT::SetPartition(COM_INSTANCE	CI)
{
	_Partition = CI;
}

