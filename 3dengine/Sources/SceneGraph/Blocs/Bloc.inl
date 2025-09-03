//-----------------------------------------------------------------------------
//	Name:		GetIndex
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC::GetIndex() const
{
	return ( _nIndex );
}

//-----------------------------------------------------------------------------
//	Name:		GetBox
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
const AABB &	BLOC::GetBox() const
{
	return ( _Box );
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeIdx
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	BLOC::GetNodeInst(int	nNumNode) const
{
	int	n; 

	n = GetNbrNodes();

	if ( nNumNode >= n )
	{
		QDT_FAIL();
		nNumNode = n - 1;
	}
	if ( _vNodeInst.IsEmpty() )
	{
		QDT_FAIL();
	}
	return ( _vNodeInst[nNumNode] );
}

//-----------------------------------------------------------------------------
//	Name:		AddNode
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::AddNodeInst(const COM_INSTANCE &	Inst)
{
	_vNodeInst.PushOnce( Inst );
}

//-----------------------------------------------------------------------------
//	Name:		SetIndex
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::SetIndex(int	nIndex)
{
	_nIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		SetBox
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::SetBox(const AABB &	Box)
{
	_Box = Box;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrNodes
//	Object:		
//	02-04-15:	CVI - Created
//-----------------------------------------------------------------------------
int	BLOC::GetNbrNodes() const
{
	return ( _vNodeInst.GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		IsLoading
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC::IsLoading() const
{
	if ( _Flags.IsSet( BLOC_IS_LOADING ) )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsLoaded
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	BLOC::IsLoaded() const
{
	if ( _Flags.IsSet( BLOC_LOADED ) )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSGNodeInst
//	Object:		
//	02-04-18:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::SetSGNodeInst(const COM_INSTANCE &	Inst)
{
	_SGNodeInst = Inst;
}

//-----------------------------------------------------------------------------
//	Name:		SetLoading
//	Object:		
//	02-04-19:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::SetLoading(bool	b)
{
	_Flags = 0;
	_Flags.Set( BLOC_IS_LOADING, b );
}

//-----------------------------------------------------------------------------
//	Name:		SetLoaded
//	Object:		
//	02-04-19:	CVI - Created
//-----------------------------------------------------------------------------
void	BLOC::SetLoaded(bool	b)
{
	_Flags = 0;
	_Flags.Set( BLOC_LOADED, b );
}

