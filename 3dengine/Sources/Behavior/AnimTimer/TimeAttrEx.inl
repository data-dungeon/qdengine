
//-----------------------------------------------------------------------------
//	Name:		GetCurTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetCurTime() const
{
	return ( _rCurrentTime );
}

//-----------------------------------------------------------------------------
//	Name:		GetStartTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetStartTime() const
{
	return ( _rStartTime );
}

//-----------------------------------------------------------------------------
//	Name:		GetEndTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetEndTime() const
{
	return ( _rEndTime );
}

//-----------------------------------------------------------------------------
//	Name:		GetSpeed
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetSpeed() const
{
	return ( _rSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		HasLooped
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
bool	TIME_ATTR_EX::HasLooped() const
{
	return ( _bHasLoop );
}

//-----------------------------------------------------------------------------
//	Name:		IsFinished
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
bool	TIME_ATTR_EX::IsFinished() const
{
	return ( _bIsFinished );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrLoopsDone
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
int	TIME_ATTR_EX::GetNbrLoopsDone() const
{
	return ( _nNbrLoopsDone );
}

//-----------------------------------------------------------------------------
//	Name:		GetFrameIncrement
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetFrameIncrement() const
{
	return ( _rFrameInc );
}

//-----------------------------------------------------------------------------
//	Name:		IsManualIncrement
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
bool	TIME_ATTR_EX::IsManualIncrement() const
{
	return ( _bIsManualTimer );
}

//-----------------------------------------------------------------------------
//	Name:		SetCurTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetCurTime(float	rVal)
{
	_rCurrentTime = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetStartTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetStartTime(float	rVal)
{
	_rStartTime = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetEndTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetEndTime(float	rVal)
{
	_rEndTime = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrRepeat
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetNbrRepeat(int	nVal)
{
	_nNbrLoops = nVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetSpeed
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetSpeed(float	rVal)
{
	_rSpeed = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetPaused
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetPaused(bool	bVal)
{
	_bIsPaused = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetLastTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetLastTime(float	rVal)
{
	_rLastTime = rVal;
}


//-----------------------------------------------------------------------------
//	Name:		SetOffsetStart
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetOffsetStart(float	rVal)
{
	_rOffsetStart = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrameIncrement
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetFrameIncrement(float	rVal)
{
	_rFrameInc = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetManualIncrement
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetManualIncrement(bool	b)
{
	_bIsManualTimer = b;
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendingTimeLong
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetBlendingTimeLong() const
{
	return ( _BlendingTimer.GetTimeLong() );
}

//-----------------------------------------------------------------------------
//	Name:		GetTimeToGo
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetTimeToGo() const
{
	return ( _rTimeToGo );
}


//-----------------------------------------------------------------------------
//	Name:		ActiveBlending
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::ActiveBlending()
{
	_bBlendingForceActived = true;
}

//-----------------------------------------------------------------------------
//	Name:		SetInfinite
//	Object:		
//	03-06-12:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetInfinite(bool	bVal)
{
	_bIsInfinite = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetLoop
//	Object:		
//	03-06-12:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetLoop(bool	bVal)
{
	_bIsLoop = bVal;
}


//-----------------------------------------------------------------------------
//	Name:		GetCurrentBlendingFrame
//	Object:		
//	03-06-17:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetCurrentBlendingFrame() const
{
	return ( _BlendingTimer.GetCurTime() );
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendingActived
//	Object:		
//	03-06-17:	CVI - Created
//-----------------------------------------------------------------------------
bool	TIME_ATTR_EX::IsBlendingActived() const
{
	return ( _bBlendingForceActived );
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrBlendingFrames
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::SetNbrBlendingFrames(float	rVal)
{
	_BlendingTimer.SetTimeLong( rVal );
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentDeltaTime
//	Object:		
//	04-01-26:	CVI - Created
//-----------------------------------------------------------------------------
float	TIME_ATTR_EX::GetCurrentDeltaTime() const
{
	return ( _rCurrentDeltaTime );
}
