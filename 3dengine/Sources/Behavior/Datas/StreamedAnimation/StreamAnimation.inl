//-----------------------------------------------------------------------------
//	Name:		GetStreamAnimFromInstance
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_ANIMATION *	STREAM_ANIMATION::GetStreamAnimFromInstance(const COM_INSTANCE &	CI)
{
	STREAM_ANIMATION	*pSA = NULL;

	if ( CI.HasHandle() )
	{
		pSA = static_cast<STREAM_ANIMATION*> ( CI.GetCommunicator() );
	}
	return ( pSA );
}


//-----------------------------------------------------------------------------
//	Name:		GetNbrFrames
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_ANIMATION::GetNbrFrames() const
{
	return ( _nNbrFrames );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTracks
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_ANIMATION::GetNbrTracks() const
{
	return ( _nNbrTracks );
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseFrameRate
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
float	STREAM_ANIMATION::GetBaseFrameRate() const
{
	return ( _rFPS );
}

//-----------------------------------------------------------------------------
//	Name:		GetTimeFromNumFrame
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
float	STREAM_ANIMATION::GetTimeFromNumFrame(float	rFrame) const
{
	return ( rFrame / _rFPS );
}

//-----------------------------------------------------------------------------
//	Name:		GetFrameFromTime
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
float	STREAM_ANIMATION::GetFrameFromTime(float	rTime) const
{
	return ( rTime * _rFPS );
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrFrames
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::SetNbrFrames(int	n)
{
	_nNbrFrames = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrTracks
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::SetNbrTracks(int	n)
{
	_nNbrTracks = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetBaseFrameRate
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::SetBaseFrameRate(float	rVal)
{
	_rFPS = rVal;
}

