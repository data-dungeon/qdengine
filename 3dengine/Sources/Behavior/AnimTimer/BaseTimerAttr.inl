//-----------------------------------------------------------------------------
//	Name:		GetCurTime
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
float	BASE_TIMER_ATTR::GetCurTime() const
{
	return ( _rCurrentTime );
}

//-----------------------------------------------------------------------------
//	Name:		GetTimeLong
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
float	BASE_TIMER_ATTR::GetTimeLong() const
{
	return ( _rTimeLong );
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentTime
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
void	BASE_TIMER_ATTR::SetCurrentTime(float	rVal)
{
	_rCurrentTime = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetTimeLong
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
void	BASE_TIMER_ATTR::SetTimeLong(float	rVal)
{
	_rTimeLong = rVal;
}

//-----------------------------------------------------------------------------
//	Name:		IsFinished
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
bool	BASE_TIMER_ATTR::IsFinished() const
{
	return ( _bFinished );
}


//-----------------------------------------------------------------------------
//	Name:		Mirror
//	Object:		
//	02-03-12:	CVI - Created
//-----------------------------------------------------------------------------
void	BASE_TIMER_ATTR::Mirror()
{
	float	rTemp;

	///////////////////////////////////
	rTemp = _rTimeLong - _rCurrentTime;
	_rCurrentTime = rTemp;
	_bFinished = false;
	if ( _rCurrentTime >= _rTimeLong )
	{
		_bFinished = true;
	}
	///////////////////////////////////
}
