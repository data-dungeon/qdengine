
//-----------------------------------------------------------------------------
//	Name:		GetAbsRot
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const QUATERNION &	STREAM_TRACK_HEADER::GetAbsRot() const
{
	return ( _AbsoluteRot );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsTrans
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const VECTOR &	STREAM_TRACK_HEADER::GetAbsTrans() const
{
	return ( _AbsoluteTrans );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsScale
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const VECTOR &	STREAM_TRACK_HEADER::GetAbsScale() const
{
	return ( _AbsoluteScale );
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsRot
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_HEADER::SetAbsRot(const QUATERNION &	Q)
{
	_AbsoluteRot = Q;
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsTrans
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_HEADER::SetAbsTrans(const VECTOR &	V)
{
	_AbsoluteTrans = V;
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsScale
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_HEADER::SetAbsScale(const VECTOR &	S)
{
	_AbsoluteScale = S;
}

