//-----------------------------------------------------------------------------
//	Name:		GetNbrQuatKeys
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK::GetNbrQuatKeys() const
{
	return ( _nNbrQuatKeys );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTransKeys
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK::GetNbrTransKeys() const
{
	return ( _nNbrTransKeys );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrScaleKeys
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK::GetNbrScaleKeys() const
{
	return ( _nNbrScaleKeys );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrAnimTops
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK::GetNbrAnimTops() const
{
	return ( _nNbrAnimTops );
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrQuatKeys
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetNbrQuatKeys(int	n)
{
	_nNbrQuatKeys = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrTransKeys
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetNbrTransKeys(int	n)
{
	_nNbrTransKeys = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrScaleKeys
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetNbrScaleKeys(int	n)
{
	_nNbrScaleKeys = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrAnimTops
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetNbrAnimTops(int	n)
{
	_nNbrAnimTops = n;
}
