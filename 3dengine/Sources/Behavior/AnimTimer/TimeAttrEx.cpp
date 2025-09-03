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
//	CLASS:	TIME_ATTR_EX
//
//	03-06-11:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"TimeAttrEx.h"
#include	INCL_KMATH(Math/Math)

#ifdef _DEBUG
	#include "TimeAttrEx.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TIME_ATTR_EX constructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
TIME_ATTR_EX::TIME_ATTR_EX()
:
_rStartTime( 0.0f ),
_rEndTime( 30.0f),
_rSpeed( 1.0f ),
_rCurrentTime( 0.0f ),
_rLastTime( 0.0f ),
_rTimeToGo( 30.0f ),
_rOffsetStart( 0.0f),
_nNbrLoops( 1),
_nNbrLoopsDone(0),
_bHasLoop(false),
_bIsManualTimer(false),
_bIsInfinite(true),
_bIsLoop	( false ),
_bIsFinished(false),
_bIsPaused(false)
{
	_bBlendingForceActived = false;
	_rFrameInc			= 0.0f;
}




//-----------------------------------------------------------------------------
//	Name:		TIME_ATTR_EX destructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
TIME_ATTR_EX::~TIME_ATTR_EX()
{

}


//-----------------------------------------------------------------------------
//	Name:		TIME_ATTR_EX constructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
TIME_ATTR_EX::TIME_ATTR_EX(const TIME_ATTR_EX &	Timer)
{
	Copy( Timer );
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::Copy(const TIME_ATTR_EX &	C)
{
	_rStartTime				= C._rStartTime;
	_rEndTime				= C._rEndTime;
	_rSpeed					= C._rSpeed;
	_rCurrentTime			= C._rCurrentTime;
	_rLastTime				= C._rLastTime;
	_rTimeToGo				= C._rTimeToGo;
	_rOffsetStart			= C._rOffsetStart;
	_nNbrLoops				= C._nNbrLoops;
	_nNbrLoopsDone			= C._nNbrLoopsDone;
	_bHasLoop				= C._bHasLoop;
	_bIsManualTimer			= C._bIsManualTimer;
	_bIsInfinite			= C._bIsInfinite;
	_bIsLoop				= C._bIsLoop;
	_bIsFinished			= C._bIsFinished;
	_bIsPaused				= C._bIsPaused;

	_BlendingTimer			= C._BlendingTimer;

	_bBlendingForceActived	= C._bBlendingForceActived;
	_rFrameInc				= C._rFrameInc;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
const TIME_ATTR_EX &	TIME_ATTR_EX::operator=(const TIME_ATTR_EX &	C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}


//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::Init()
{
	_rStartTime				= 0.0f;
	_rEndTime				= 30.0f;
	_rSpeed					= 1.0f;
	_rCurrentTime			= 0.0f;
	_rLastTime				= 0.0f;
	_rTimeToGo				= 30.0f;
	_rOffsetStart			= 0.0f;
	_nNbrLoops				= 1;
	_nNbrLoopsDone			= 0;
	_bHasLoop				= false;
	_bIsManualTimer			= false;
	_bIsInfinite			= true;
	_bIsLoop				= false;
	_bIsFinished			= false;
	_bIsPaused				= false;

	_bBlendingForceActived	= false;
	_rFrameInc				= 0.0f;
	_BlendingTimer.Reset();

}

//-----------------------------------------------------------------------------
//	Name:		UpdateTime
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
bool	TIME_ATTR_EX::UpdateTime(float &		rDelay,
								 bool& bBlendPrevSeq )
{
	float	rInc;

	////////////////////////////////
	_rCurrentDeltaTime = rDelay;
	if ( _bIsPaused || MATH::Abs(_rSpeed ) <= MATH::EPSILON() )
	{
		rDelay = 0.0f;
		_rCurrentDeltaTime = 0.0f;
		return ( _bIsFinished );
	}
	////////////////////////////////


	////////////////////////////////
	if ( _bIsInfinite )
	{
		UpdateInfinite( rDelay );
		return ( _bIsFinished );
	}
	////////////////////////////////

	////////////////////////////////
	if ( IsManualIncrement() )
	{
		rInc	= GetFrameIncrement();
		if ( rInc <= 0.0f )
		{
			rDelay = (_rCurrentTime - _rLastTime ) / _rSpeed;
			_rCurrentTime = _rLastTime;
		}
		else
		{
			rDelay *= rInc;
		}
	}
	////////////////////////////////

	////////////////////////////////
	_bHasLoop		= false;
	_nNbrLoopsDone	= 0;
	////////////////////////////////

	////////////////////////////////
	BasicUpdate( rDelay, bBlendPrevSeq );
	////////////////////////////////

	////////////////////////////////
	if ( IsManualIncrement() )
	{
		if ( MATH::Abs( rInc ) > MATH::EPSILON() )
		{
			rDelay /= rInc;
		}
		SetFrameIncrement( 0.0f );
	}
	////////////////////////////////

	////////////////////////////////
	return ( _bIsFinished );
	////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdateInfinite
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::UpdateInfinite(float &	rDelay)
{
	float	rDelta;

	_rLastTime		= _rCurrentTime;
	rDelta			= rDelay * _rSpeed;
	_rCurrentTime += rDelta;
	_rTimeToGo		= QDT_FLOAT32_MAX;
	
	_nNbrLoopsDone	= 0;
	_bHasLoop		= false;

}

//-----------------------------------------------------------------------------
//	Name:		BasicUpdate
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::BasicUpdate(float &		rDelay,
								  bool& bBlend )
{
	float	rUpdateTime;
	/////////////////////////////////////
	_rLastTime = _rCurrentTime;
	/////////////////////////////////////

	/////////////////////////////////////
	if ( bBlend )
	{
		if ( _BlendingTimer.IsFinished() )
		{
			bBlend = false;
		}
		else
		{
			rUpdateTime = rDelay;
			_BlendingTimer.UpdateTime( rUpdateTime );
			if ( _BlendingTimer.IsFinished() )
			{
				bBlend = false;
			}
		}
	}
	UpdateOwn( rDelay );
	/////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdateOwn
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::UpdateOwn(float &	rDelay)
{
	/////////////////////////////////////////////////////////////////
	if ( _rTimeToGo < _rOffsetStart )
	{
		_bBlendingForceActived = true;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	UpdateTimer( rDelay );
	/////////////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTimer
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::UpdateTimer(float &	rDelay)
{
	/////////////////////////////////////////////////////////////////	
	if ( MATH::Abs(rDelay) < MATH::EPSILON() )
	{
		return;
	}
	if ( _rSpeed * rDelay >= 0.0f )
	{
		UpdateTimerPos( rDelay );
	}
	else
	{
		UpdateTimerNeg( rDelay );
	}
	/////////////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTimerPos
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::UpdateTimerPos(float &	rDelay)
{
	float			rDelta;

	/////////////////////////////////////////////////////////////////
	//	Check case init with speed < 0
	if ( _rSpeed < 0.0f && rDelay >= 0.0f )
	{
		_rCurrentTime = _rStartTime;
		if ( _bIsLoop )
		{
			_rTimeToGo = QDT_FLOAT32_MAX;
		}
		else
		{
			_rTimeToGo = MATH::Abs(_rEndTime - _rStartTime ) *  (_nNbrLoops - 1 ) + ( _rCurrentTime -_rEndTime );
		}
		return;
	}
	/////////////////////////////////////////////////////////////////

	if ( _rCurrentTime < _rStartTime )	_rCurrentTime = _rStartTime;
	rDelta			= rDelay * _rSpeed;
	_rCurrentTime	+= rDelta;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( _rEndTime < _rCurrentTime )
	{
		
		if ( !_bIsLoop )
		{
			_nNbrLoops--;
		}
		else if ( _nNbrLoops < 1 )
		{
			_nNbrLoops = 1;
		}


		if ( !_bIsLoop && _nNbrLoops < 1 )
		{
			_rCurrentTime	= _rEndTime;
			
			rDelay			= (_rCurrentTime - _rEndTime) / _rSpeed;
			_rTimeToGo		= 0.0f;
			_bIsFinished	= true;
		}
		else
		{
			_rCurrentTime	= _rCurrentTime - _rEndTime + _rStartTime;
			_bHasLoop		= true;
			_nNbrLoopsDone++;
			if ( _rCurrentTime > _rEndTime )
			{
				_rCurrentTime = _rEndTime;
			}
			if ( _bIsLoop )
			{
				_rTimeToGo = QDT_FLOAT32_MAX;
			}
			else
			{
				_rTimeToGo = (_rEndTime - _rStartTime ) * (_nNbrLoops - 1 ) + ( _rEndTime - _rCurrentTime );
			}

			if ( _rTimeToGo < 0.0f )	_rTimeToGo = 0.0f;
			rDelay = 0.0f;
		}
		return;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	rDelay = 0.0f;
	if ( _bIsLoop )
	{
		_rTimeToGo = QDT_FLOAT32_MAX;
	}
	else
	{
		_rTimeToGo = (_rEndTime - _rStartTime ) * (_nNbrLoops - 1 ) + ( _rEndTime - _rCurrentTime );
	}
	/////////////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTimerNeg
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_ATTR_EX::UpdateTimerNeg(float &	rDelay)
{
	float			rDelta;

	/////////////////////////////////////////////////////////////////
	if ( _rCurrentTime > _rStartTime )	_rCurrentTime = _rStartTime;
	rDelta			= rDelay * _rSpeed;
	_rCurrentTime	+= rDelta;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
//	if ( _rCurrentTime < _rStartTime )
	if ( _rCurrentTime < _rEndTime )
	{
		
		if ( !_bIsLoop )
		{
			_nNbrLoops--;
		}
		else if ( _nNbrLoops < 1 )
		{
			_nNbrLoops = 1;
		}


		if ( !_bIsLoop && _nNbrLoops < 1 )
		{
			_rCurrentTime	= _rEndTime;
			rDelay			= ( _rCurrentTime - _rEndTime ) / _rSpeed;
			_rTimeToGo		= 0.0f;
			_bIsFinished	= true;
		}
		else
		{
			_rCurrentTime	= _rCurrentTime - _rEndTime + _rStartTime;
			_bHasLoop		= true;
			_nNbrLoopsDone++;
			if ( _rCurrentTime < _rEndTime )
			{
				_rCurrentTime = _rEndTime;
			}

			if ( _bIsLoop )
			{
				_rTimeToGo = QDT_FLOAT32_MAX;
			}
			else
			{
				_rTimeToGo = MATH::Abs(_rEndTime - _rStartTime ) *  (_nNbrLoops - 1 ) + ( _rCurrentTime -_rEndTime );
			}
			if ( _rTimeToGo < 0.0f )	_rTimeToGo = 0.0f;
			rDelay = 0.0f;
		}
		return;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	rDelay = 0.0f;
	if ( _bIsLoop )
	{
		_rTimeToGo = QDT_FLOAT32_MAX;
	}
	else
	{
		_rTimeToGo = MATH::Abs(_rEndTime - _rStartTime ) *  (_nNbrLoops - 1 ) + ( _rCurrentTime - _rEndTime );
	}
	/////////////////////////////////////////////////////////////////

}




//=============================================================================
//	CODE ENDS HERE
//=============================================================================
