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
// CLASS: TRACK_DATA
//
//
//00-11-29 GHP Created: 
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (TRACK_DATA)
//
//	The TRACK_DATA class implements the inlined functions of the class
//
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetTrackIndexes
//	Object:		
//	01-12-07:	RMA - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetTrackIndexes(TRACK_INDEXES *	pTrackIndexes)
{
	_pTrackIndexes = pTrackIndexes;
}

//-----------------------------------------------------------------------------
//	Name:		SetReverse
//	Object:		
//	01-04-20:	GHP - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::SetReverse(bool bReverse ) 
{
	_bReverse = bReverse ; 
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-19:	GHP - Created
//-----------------------------------------------------------------------------
bool  TRACK_DATA::CheckLastTrans(const float rValue, VECTOR & V)
{	
	QDT_ASSERT(_pTransKeys);

	if (_pTransKeys[_nNbrTransKeys - 1].GetPad() <= rValue)
	{
		V = _pTransKeys[_nNbrTransKeys - 1];
		return (true);
	}

	return (false) ;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-19:	GHP - Created
//-----------------------------------------------------------------------------
bool  TRACK_DATA::CheckFirstTrans(const float rValue, VECTOR& V)
{
	QDT_ASSERT(_pTransKeys);
	
	if (_pTransKeys[0].GetPad() >= rValue)
	{
		V = _pTransKeys[0];
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-19:	GHP - Created
//-----------------------------------------------------------------------------
bool  TRACK_DATA::CheckLastScale(const float	rValue, 
								 VECTOR &		S) 
{
	QDT_ASSERT(_pScaleKeys);

	if (_prScaleTimes[_nNbrScaleKeys - 1] < rValue)
	{
		S = _pScaleKeys[_nNbrScaleKeys - 1];
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		CheckFirstScale	
//	Object:		
//	01-04-19:	GHP - Created
//-----------------------------------------------------------------------------
bool  TRACK_DATA::CheckFirstScale(const float	rValue, 
								  VECTOR &		S) 
{	
	if (_prScaleTimes[0] > rValue) 
	{
		S = _pScaleKeys[0];
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		HasRotation
//	Object:		
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
bool	TRACK_DATA::HasRotation() const 
{
	return (_pRotKeys != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		HasTranslation
//	Object:		
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
bool	TRACK_DATA::HasTranslation() const 
{
	return (_pTransKeys != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		HasScaling
//	Object:		
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
bool	TRACK_DATA::HasScaling() const 
{
	return (_pScaleKeys != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		HasAnimTops
//	Object:		
//	02-04-02:	CVI - Created
//-----------------------------------------------------------------------------
bool	TRACK_DATA::HasAnimTops() const
{
	return (_pAnimTops != NULL) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrAnimTops
//	Object:		
//	02-04-02:	CVI - Created
//-----------------------------------------------------------------------------
UInt32	TRACK_DATA::GetNbrAnimTops() const
{
	return (_nNbrAnimTops);
}

//-----------------------------------------------------------------------------
//	Name:		CheckLast
//	Object:		
//	01-06-15:	GHP - Created
//-----------------------------------------------------------------------------
bool	TRACK_DATA::CheckLastAnimTop(float				rFrame,
									 QDT_DLIST<int> &	vAnimTopsValues)
{	
	// if the time of the last key is smaller than the current time code 
	// get the value from that key, 
	// which means if current frame is not resetted the last value will last forever :)
	if (_pAnimTops[_nNbrAnimTops - 1].GetTime() < rFrame) 
	{
		int nCurVal = _pAnimTops[_nNbrAnimTops - 1].GetValue(); 
		
		if (nCurVal != 0 && _nLastVal != nCurVal)
		{
			_nLastVal = nCurVal;
			vAnimTopsValues.Push(nCurVal, _bReverse);
			return (true); 
		}
	}
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseTranslation
//	Object:		
//	01-11-07:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::GetBaseTranslation(VECTOR &	vTrans) const
{
	vTrans = _vAbsTrans;
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseRotation
//	Object:		
//	01-11-07:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::GetBaseRotation(QUATERNION &	Quat) const
{
	Quat = _AbsQuat;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRotKeys
//	Object:		
//	02-03-22:	CVI - Created
//-----------------------------------------------------------------------------
UInt32	TRACK_DATA::GetNbrRotKeys() const
{
	return (_nNbrRotKeys);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTransKeys
//	Object:		
//	02-03-22:	CVI - Created
//-----------------------------------------------------------------------------
UInt32	TRACK_DATA::GetNbrTransKeys() const
{
	return (_nNbrTransKeys);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrScaleKeys
//	Object:		
//	02-03-22:	CVI - Created
//-----------------------------------------------------------------------------
UInt32	TRACK_DATA::GetNbrScaleKeys() const
{
	return (_nNbrScaleKeys);
}

//-----------------------------------------------------------------------------
//	Name:		GetTransKey
//	Object:		
//	02-04-02:	CVI - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::GetTransKey(UInt32 n, 
							 VECTOR & V) const
{
	QDT_ASSERT(n < _nNbrTransKeys);
	V = _pTransKeys[n];
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimTop
//	Object:		
//	02-04-02:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::GetAnimTop(UInt32	n,
							   ANIM_TOP & AT) const
{
	QDT_ASSERT(n < _nNbrAnimTops);
	AT = _pAnimTops[n];
}

//-----------------------------------------------------------------------------
//	Name:		GetRotTrans
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::GetRotTrans(const float	rValue,
								QUATERNION_VECTOR & QV)
{
	GetRot(rValue, QV._Quat);
	GetTrans( rValue, QV._Vect);
}

//-----------------------------------------------------------------------------
//	Name:		GetQuatTime
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
float	TRACK_DATA::GetQuatTime(UInt32	n) const
{
	QDT_ASSERT(n < _nNbrRotKeys);
	return (_pnRotTimes[n]);
}

//-----------------------------------------------------------------------------
//	Name:		GetTransTime
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
float	TRACK_DATA::GetTransTime(UInt32	n) const
{
	QDT_ASSERT(n < _nNbrTransKeys);
	return(_pTransKeys[n].GetPad());
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

