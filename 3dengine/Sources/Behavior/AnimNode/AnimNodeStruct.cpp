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
//	CLASS:	ANIM_NODE_STRUCT
//
//	01-10-11:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"AnimNodeStruct.h"
#include	INCL_3DENGINE(Behavior/AnimNode/AnimNode)
#include	INCL_3DENGINE(Behavior/Controllers/Container/Container)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)

#ifdef _DEBUG
	#include "AnimNodeStruct.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE_STRUCT constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT::ANIM_NODE_STRUCT()
:
_pChildrenOwn	( NULL )
{
	_bFirstUpdate = true;
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE_STRUCT constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT::ANIM_NODE_STRUCT(const ANIM_NODE_STRUCT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE_STRUCT destructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT::~ANIM_NODE_STRUCT()
{
	if ( _pChildrenOwn )
	{
		delete []_pChildrenOwn;
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT &	ANIM_NODE_STRUCT::operator=(const ANIM_NODE_STRUCT & C)
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
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::Copy(const ANIM_NODE_STRUCT & C)
{
	//_WeightAttr			= C._WeightAttr;
	_TimeAttr			= C._TimeAttr;
	_ScaleAttr			= C._ScaleAttr;
	_AnimStatus			= C._AnimStatus;
	_AnimLink			= C._AnimLink;
	_bAnimDeleted		= C._bAnimDeleted;
}


//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::Init()
{
	// PATCH JJA MEMORY LEAK
	if	( _pChildrenOwn )
	{
		delete[] _pChildrenOwn;
	}

	_AnimStatus.Reset();
	_AnimLink.Reset();
	_bAnimDeleted			= false;

	_bFinished				= false;
	_bIsActive				= false;
	
	_bBlendingWithPrevSeq	= false;
	_bIsBlending			= false;
	_nNbrChildren			= 0;
	_pChildrenOwn			= NULL;
	_pChildrenOutput		= NULL;
	_bBlendAtAnimEnd		= false;
	_nPartStartIndex		= -1;
	_nPartNbrTracks			= -1;
	_nPartEndIndex			= -1;
	_bIsPartAnim			= false;
	_PartBlendTimer.Reset();
	_PartBlendTimer.SetTimeLong( 10.0f );
	_PartBlending			= NO_ACTIVATED;
}

//-----------------------------------------------------------------------------
//	Name:		InitBeforeStart
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InitBeforeStart(ANIM_NODE*	pPrev)
{
	if ( _AnimStatus.IsBlended() )
	{
		_bBlendingWithPrevSeq = true;
	}
	_TimeAttr.SetCurTime( _TimeAttr.GetStartTime() );
	_bFirstUpdate = true;
}

//-----------------------------------------------------------------------------
//	Name:		IsFinished
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsFinished()
{
	return ( _bFinished );
//	return ( _TimeAttr.IsFinished() );
}

//-----------------------------------------------------------------------------
//	Name:		SetFinished
//	Object:		
//	01-10-12:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetFinished(bool	bVal)
{
	_bFinished = bVal;
//	_TimeAttr.SetFinished( bVal );
}


//-----------------------------------------------------------------------------
//	Name:		StoreRotTrans
//	Object:		
//	01-10-12:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::StoreRotTrans( float rTime )
{
	///////////////////////////////////////////
	ReadRotTrans( rTime );
	///////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		AddSequence
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::AddSequence(const COM_INSTANCE&	Seq )
{
	ANIM_NODE_STRUCT			*pAN;
	ANIM_NODE_STRUCT			*pNextAN;
	bool						bPremptive;
	
	/////////////////////////////////
	if ( !Seq.HasHandle() )	return;
	/////////////////////////////////

	/////////////////////////////////
	pAN			= ANIM_NODE::GetStructFromInstance( Seq );
	bPremptive	= pAN->GetAnimStatus().IsPreemptive();
	QDT_ASSERT( pAN != this );

	/////////////////////////////////
	pNextAN = ANIM_NODE::GetStructFromInstance( _AnimLink.GetNextSeq() );
	/////////////////////////////////

	/////////////////////////////////
	//	Not preemptive
	if ( !bPremptive )
	{
		if ( pNextAN )
		{
			pNextAN->AddSequence( Seq );
		}
		else
		{
			if( _AnimLink.GetContainer() )
			{
				pAN->GetAnimLink()->SetContainer( _AnimLink.GetContainer() );
			}
			QDT_ASSERT( pAN->GetAnimLink()->GetContainer() );
			_AnimLink.AddSequence( Seq, this->GetAnimNode() );
		}
		return;
	}
	/////////////////////////////////

	/////////////////////////////////
	//	Preemptive anim
	if ( pAN->HasBlending() == false )
	{
		//	No blending
		if( _AnimLink.GetContainer() )
		{
			pAN->GetAnimLink()->SetContainer( _AnimLink.GetContainer() );
		}
		QDT_ASSERT( pAN->GetAnimLink()->GetContainer() );
		//	Set finished all seq + this
		ForceTerminate( true );
		_AnimLink.AddSequence( Seq, this->GetAnimNode() );
		pAN->InitBeforeStart( NULL );
		pAN->InitBeforeUpdateStart( NULL);
		pAN->Activate( true );
		return;
	}

	//	Preemptive with blending
	if ( pNextAN )
	{
		if ( pNextAN->IsActive() )
		{
			//	Next anim
			pNextAN->AddSequence( Seq );
			return;
		}
		else
		{
			//	Set finish all next anim but not this
			ForceTerminateAllSeq();
		}
	}
	pAN->_bBlendingWithPrevSeq = true;
	_bIsBlending = true;
	if( _AnimLink.GetContainer() )
	{
		pAN->GetAnimLink()->SetContainer( _AnimLink.GetContainer() );
	}
	QDT_ASSERT( pAN->GetAnimLink()->GetContainer() );
	_AnimLink.AddSequence( Seq, this->GetAnimNode() );
	pAN->InitBeforeStart( NULL );
	pAN->InitBeforeUpdateStart( NULL);
	pAN->Activate( true );
	/////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		ForceTerminate
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ForceTerminate( bool bDeleteSeq )
{
	/////////////////////////////
	if ( bDeleteSeq )
	{
		//	Exec termination of 
		//	following sequences with recursive method
		ForceTerminateAllSeq();
	}
	/////////////////////////////

	/////////////////////////////
	ExecTerminate( bDeleteSeq );
	/////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		ExecTerminate
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ExecTerminate( bool bDeleteSeq )
{
	COM_INSTANCE			CI;

	///////////////////////////////////////
	//	Delete delegated animations
	///////////////////////////////////////
	SetFinished( true );
	///////////////////////////////////////	

}

//-----------------------------------------------------------------------------
//	Name:		ForceTerminateAllSeq
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ForceTerminateAllSeq()
{
	ANIM_NODE_STRUCT		*pAN;
	COM_INSTANCE			CI;

	/////////////////////////////////////
//	ExecTerminate( true );
	/////////////////////////////////////
	CI = _AnimLink.GetNextSeq();
	if ( !CI.HasHandle() )	return;
	/////////////////////////////////////

	/////////////////////////////////////
	pAN = ANIM_NODE::GetStructFromInstance( CI );
	pAN->ForceTerminateAllSeq();
	/////////////////////////////////////
	//	Delete seq
	pAN->SendToDeleteList();
	if ( CI.HasHandle() )
	{
		CI.Release();
	}
	_AnimLink.SetNextSeq( CI );
	//
	/////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		ReadRotation
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ReadRotation(float			rTime,
								QUATERNION &	Quat)
{
	Quat.Id();
}

//-----------------------------------------------------------------------------
//	Name:		ReadTranslation
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ReadTranslation(float	rTime,
								   VECTOR &	Vect)
{
	Vect.SetXYZ( 0.0f, 0.0f, 0.0f );
}

//-----------------------------------------------------------------------------
//	Name:		ReadRotTrans
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ReadRotTrans(float	rTime)
{
	ReadRotation( rTime, _CurrentQV._Quat );
	ReadTranslation( rTime, _CurrentQV._Vect );
}

//-----------------------------------------------------------------------------
//	Name:		InitAbsoluteTransfo
//	Object:		
//	01-10-22:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InitAbsoluteTransfo()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		InitBeforeUpdateStart
//	Object:		
//	01-10-23:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InitBeforeUpdateStart(ANIM_NODE *	pPrev)
{
	if ( IsTeleport() )
	{
		InitFirstFrame();
	}
	UpdateAbsolute();
}

//-----------------------------------------------------------------------------
//	Name:		InitFirstFrame
//	Object:		
//	02-10-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InitFirstFrame()
{
	QUATERNION_VECTOR	QV;
	QUATERNION			Quat;
	VECTOR				V;
	float				rStartFrame;
	float				rCurFrame;
	float				rSpeed;
	float				rCheckStart;

	//
	//	A PASS SHOULD BE MADE ON THIS METHOD !!!!!
	//
	//	

	//	Get range
	rStartFrame = _TimeAttr.GetStartTime();
	rCurFrame	= _TimeAttr.GetCurTime();
	rSpeed		= _TimeAttr.GetSpeed();

	rCheckStart = ( rCurFrame - rStartFrame ) * rSpeed;
	if ( rCheckStart > 0.0f )
	{
		//rStartFrame = rCurFrame;
	}
	if ( rStartFrame > 0.0f && ( _AnimStatus.IsAbsolute() == false ) )
	{
		//	Read keys for start time
		ReadRotation( rStartFrame, Quat );
		ReadTranslation( rStartFrame, V );
		//	Buld QUATERNION_VECTOR
		QV.SetQuat( Quat );
		QV.SetVector( V );
		//	Set Start Frame as Last
		//	TODO: set previous frame instead of start frame !!
		_LastQV = QV;
	}
	else
	{
		_LastQV.Id();
	}
}


//-----------------------------------------------------------------------------
//	Name:		SetAnimWeight
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetAnimWeight(float	rWeight)
{
	VECTOR			V;

	///////////////////////////////////////////
	V.SetXYZ( rWeight, rWeight, rWeight );
	//_WeightAttr.SetRotWeight( V );
	//_WeightAttr.SetTransWeight( V );
	///////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		SetRotationWeight
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetRotationWeight(float	rWeight)
{
	VECTOR			V;

	V.SetXYZ( rWeight, rWeight, rWeight );
	//_WeightAttr.SetRotWeight( V );
}

//-----------------------------------------------------------------------------
//	Name:		SetTranslationWeight
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetTranslationWeight(float	rWeight)
{
	VECTOR			V;

	V.SetXYZ( rWeight, rWeight, rWeight );
	//_WeightAttr.SetTransWeight( V );
}

//-----------------------------------------------------------------------------
//	Name:		SetStartTime
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetStartTime(float	rStart)
{
	_TimeAttr.SetStartTime( rStart );
}

//-----------------------------------------------------------------------------
//	Name:		SetSpeed
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetSpeed(float	rSpeed)
{
	_TimeAttr.SetSpeed( rSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		InsertSequence
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InsertSequence(const COM_INSTANCE&	CI)
{
	_AnimLink.InsertSequence( CI, this->GetAnimNode() );
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrLoops
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetNbrLoops(int	n)
{
	_TimeAttr.SetNbrRepeat( n );
}

//-----------------------------------------------------------------------------
//	Name:		SetPingPong
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
/// [MBi - 2006.01.27] - Why Ping Pong ?
void	ANIM_NODE_STRUCT::SetPingPong(bool	bVal)
{
	_AnimStatus.SetLoop( bVal );
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsolute
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetAbsolute(bool	bVal)
{
	_AnimStatus.SetAbsolute( bVal );
}

//-----------------------------------------------------------------------------
//	Name:		SetTeleport
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetTeleport(bool	bVal)
{
	_AnimStatus.SetTeleport( bVal );
}

//-----------------------------------------------------------------------------
//	Name:		SetBlending
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetBlending(bool	bVal)
{
	_AnimStatus.SetBlended( bVal );
//	_AnimStatus.SetBlended( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetPreemptive
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetPreemptive(bool	bVal)
{
	_AnimStatus.SetPreemptive( bVal );
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSequence
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::RemoveSequence(const COM_INSTANCE&	CI)
{
	_AnimLink.RemoveSequence( CI );
}

//-----------------------------------------------------------------------------
//	Name:		PopAnim
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::PopAnim(const COM_INSTANCE&	CI)
{
	_AnimLink.RemoveChild( CI );
}

//-----------------------------------------------------------------------------
//	Name:		ScaleTranslation
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ScaleTranslation(float	rSX,
									float	rSY,
									float	rSZ)
{
	VECTOR		V;

	V.SetXYZ( rSX, rSY, rSZ );
	_ScaleAttr.SetTransScale( V );
}


//-----------------------------------------------------------------------------
//	Name:		SendToDeleteList
//	Object:		
//	01-11-14:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SendToDeleteList()
{
	///////////////////////////////////////////////////////////////////////
	if ( _bAnimDeleted )	return;
	COM_INSTANCE	CI(_pAnimNode);
	ExecDelete();
	_AnimLink.Reset();
	_bAnimDeleted = true;
	///////////////////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		ExecDelete
//	Object:		
//	03-06-05:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ExecDelete()
{ }

//-----------------------------------------------------------------------------
//	Name:		Pause
//	Object:		
//	01-11-20:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::PauseAnim()
{
	_TimeAttr.SetPaused( true );
}

//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	01-11-20:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::Resume()
{
	_TimeAttr.SetPaused( false );
}

//-----------------------------------------------------------------------------
//	Name:		Stop
//	Object:		
//	01-11-20:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::Stop()
{
	SetFinished( true );
	if ( IsActive() )
	{
		if ( _bBlendAtAnimEnd )
		{
			_bIsBlending = true;		
			if ( _AnimLink.GetContainer() && !_bIsPartAnim)
			{
				_AnimLink.GetContainer()->SaveBlendingStart( this );
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RecStop
//	Object:		
//	03-06-18:	VPI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::RecStop()
{
	COM_INSTANCE		CI;
	ANIM_NODE_STRUCT	*pANS;

	/////////////////////////////////////////////////////
	CI = _AnimLink.GetNextSeq();
	if ( CI.HasHandle() )
	{
		pANS = ANIM_NODE::GetStructFromInstance( CI );
		pANS->RecStop();
	}
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	Stop();
	/////////////////////////////////////////////////////
}



//-----------------------------------------------------------------------------
//	Name:		IncludeLoopsInTeleport
//	Object:		
//	02-02-12:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::IncludeLoopsInTeleport( QUATERNION_VECTOR& QV )
{
	/////////////////////////////////////////////////////
	if ( _AnimStatus.IsTeleport() == false )	return;
	if ( _TimeAttr.HasLooped() == false )		return;
	/////////////////////////////////////////////////////
	int						nNbrLoops;
	bool					bIsPingPong;
	TIME_ATTR_EX			Time;
	int						i;
	bool					bAbsolute;
	QUATERNION				Quat;
	VECTOR					Vect;
	QUATERNION_VECTOR		QVStart;
	QUATERNION_VECTOR		QVEnd;
	QUATERNION_VECTOR		QVRes;
	QUATERNION_VECTOR		QVSE;
	QUATERNION_VECTOR		QVES;
	QUATERNION_VECTOR		AccQV;
	bool					b;

	AccQV.Id();

	///////////////////////////////////////////////
	nNbrLoops		= _TimeAttr.GetNbrLoopsDone();
	bIsPingPong		= _AnimStatus.IsPingPong();
	bAbsolute		= _AnimStatus.IsAbsolute();
	///////////////////////////////////////////////
	Time.SetStartTime( _TimeAttr.GetStartTime() );
	Time.SetEndTime( _TimeAttr.GetEndTime() );
	///////////////////////////////////////////////

	///////////////////////////////////////////////
	//	Simul exchange of start and end time due to pingpong 
	if ( bIsPingPong )
	{
		for ( i = 0; i < nNbrLoops; ++i )
		{
			QDT_NOT_IMPLEMENTED();
			//Time.ExStartEndTime();
		}
	}
	///////////////////////////////////////////////

	///////////////////////////////////////////////		
	//	Init start
	ReadRotation( _TimeAttr.GetStartTime(), Quat );		
	ReadTranslation( _TimeAttr.GetStartTime(), Vect );
	_ScaleAttr.ScaleVector( Vect );
	QVStart.SetQuat( Quat );
	QVStart.SetVector( Vect );
	///////////////////////////////////////////////

	///////////////////////////////////////////////		
	//	Init end
	ReadRotation( _TimeAttr.GetEndTime(), Quat );		
	ReadTranslation( _TimeAttr.GetEndTime(), Vect );
	_ScaleAttr.ScaleVector( Vect );
	QVEnd.SetQuat( Quat );
	QVEnd.SetVector( Vect );
	///////////////////////////////////////////////

	///////////////////////////////////////////////
	QVSE = QVEnd / QVStart;
	QVES = QVStart / QVEnd;
	///////////////////////////////////////////////

	b = true;

	///////////////////////////////////////////////
	for ( i = 0; i < nNbrLoops ; ++i )
	{
		///////////////////////////////////////////
		if ( b )
		{
			//	Initial way
			AccQV *= QVSE;
		}
		else
		{
			//	Inverse way
			AccQV *= QVES;
		}
		///////////////////////////////////////////
		if ( bIsPingPong )
		{
			QDT_NOT_IMPLEMENTED();
//			Time.ExStartEndTime();
			//	Inverse way
			b = !b;
		}
		///////////////////////////////////////////
	}
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	if ( IsFinished() )
	{
		QV = AccQV;
		return;
	}
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	QUATERNION_VECTOR	QSaved;

	QSaved = _CurrentQV;
	QVStart.Inverse();
	QVStart *= QSaved;
	//	See if this is correct !!!
	QV = AccQV * QVStart;
	/////////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		IsPlaying
//	Object:		
//	02-02-22:	RMA - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsPlayingAnimRec( const COM_INSTANCE&	CI)const 
{
	return (_AnimLink.IsPlayingAnim(CI));
}

//-----------------------------------------------------------------------------
//	Name:		OnSequenceAdded
//	Object:		
//	02-03-08:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::OnSequenceAdded(ANIM_NODE*	pAN)
{
	ANIM_NODE_STRUCT	*pANS;

	///////////////////////////////////////////////////////
	pANS = pAN->GetAnimStruct();
	if ( pANS->HasBlending() )
	{
		SetOffsetStart( pANS->GetNbrBlendingFrames() );
	}
	else
	{
		SetOffsetStart( 0.0f );
	}
	if ( pANS->GetAnimStatus().IsPreemptive() )
	{
		ActiveCountDown( pANS->GetNbrBlendingFrames() );
		QDT_ASSERT( IsActive() );
	}

	///////////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		OnRemovedAsNextSequence
//	Object:		
//	02-03-08:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::OnRemovedAsNextSequence()
{
	COM_INSTANCE		CI;

	///////////////////////////////////
	CI = GetAnimLink()->GetPrevSeq();
	if ( !CI.HasHandle() )	return;
	///////////////////////////////////
	CI.Release();
	_AnimLink.SetPrevSeq( CI );
	///////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		SetOffsetStart
//	Object:		
//	02-02-01:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetOffsetStart(float	rVal)
{
	COM_INSTANCE		CI;		

	/////////////////////////////////
	_TimeAttr.SetOffsetStart( rVal );
	/////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		EnableDeletation
//	Object:		
//	02-10-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::EnableDeletation(bool	b)
{
	_AnimStatus.EnableDeletation( b );
}


//-----------------------------------------------------------------------------
//	Name:		ActiveCountDown
//	Object:		
//	02-11-15:	CVI - Created
//-----------------------------------------------------------------------------
// [MBi - 2006.01.27] - Why on earth is this method called ActiveCountDown ?!???!!!
void	ANIM_NODE_STRUCT::ActiveCountDown(float	rVal)
{
	_TimeAttr.ActiveBlending();
}


//-----------------------------------------------------------------------------
//	Name:		SetLoop
//	Object:		
//	01-10-22:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetLoop(bool	bVal)
{
	_AnimStatus.SetLoop( bVal );
	_TimeAttr.SetLoop( bVal );
}

//-----------------------------------------------------------------------------
//	Name:		UpdateLastQV
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::UpdateLastQV()
{
	QUATERNION_VECTOR	QV;

	if ( IsTeleport() )
	{
		QV = _CurrentQV;
		IncludeLoopsInTeleport( QV );
		_Teleport	= QV / _LastQV;
		_LastQV		= _CurrentQV;
		_CurrentQV.Id();
	}
	else
	{
		_Teleport.Id();
		_LastQV.Id();
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateCollideStatus
//	Object:		
//	04-12-07:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::UpdateCollideStatus(STR_ANIM *	pStr)
{
	if ( (_TimeAttr.HasLooped()) || (IsFirstUpdate()) )
	{
		if ( (IsTeleport()) == false )
		{
			pStr->AddNotifyCollideForTeleport( true );
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		UpdateAbsolute
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::UpdateAbsolute()
{
	if ( _AnimStatus.IsAbsolute() )
	{
		InitAbsoluteTransfo();
	}
}


//-----------------------------------------------------------------------------
//	Name:		SetNbrBlendingFrames
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetNbrBlendingFrames(float	rVal)
{
	_TimeAttr.SetNbrBlendingFrames( rVal );
}

//-----------------------------------------------------------------------------
//	Name:		IsBlending
//	Object:		
//	03-07-10:	JJA - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsBlending() const
{
	return ( _bBlendingWithPrevSeq );
}


//-----------------------------------------------------------------------------
//	Name:		InitChildren
//	Object:		
//	03-07-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InitChildren(int	nNbrChildren)
{
	_nNbrChildren = nNbrChildren;
	if  ( nNbrChildren > 0 )
	{
		_pChildrenOwn		= new QUATERNION_VECTOR[2*nNbrChildren];
		_pChildrenOutput	= _pChildrenOwn + nNbrChildren;
	}
}


//-----------------------------------------------------------------------------
//	Name:		Process
//	Object:		
//	03-07-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::Process( float& rDelay, STR_ANIM *pStr )
{
	ANIM_NODE_STRUCT	*pANS;
//	rDelay = 0.02f;

	QDT_ASSERT( _AnimLink.GetContainer() );
	//////////////////////////
	//	Process timer and read
	//	rotation and translation
	ProcessTimer( rDelay );
	AnimComputation();
	UpdateCollideStatus( pStr );
	//////////////////////////
	
	//////////////////////////
	//	Send result with
	//	possible blending
	if ( _bBlendingWithPrevSeq )
	{
		ProcessBlending( pStr );
	}
	else
	{
		ProcessNoBlending( pStr );
	}
	if ( _AnimStatus.IsAbsolute() && _AnimStatus.IsTeleport() )
	{
		QUATERNION_VECTOR	QV( _Absolute );
		_Absolute = QV * _Teleport;
		_Absolute.Normalize();
	}
	//////////////////////////


	//////////////////////////
	//	Check if anim finished
	//	and update some flags
	if ( _TimeAttr.IsFinished() )
	{
		_bFinished		= true;
		_bIsBlending	= false;
	}
	//////////////////////////

	//////////////////////////
	SetFirstUpdateSatus( false );
	//////////////////////////

	//////////////////////////
	//	Check if there is a
	//	anim following current
	if ( _AnimLink.GetNextSeq().HasHandle() == false )
	{
		_AnimLink.GetContainer()->SetActiveAnim( COM_INSTANCE(GetAnimNode()) );
		if ( _bBlendAtAnimEnd )
		{
			//	Default status
			_bIsBlending = false;
			if ( IsFinished() )
			{
				_bIsBlending = true;		
				if ( !_bIsPartAnim )
				{
					_AnimLink.GetContainer()->SaveBlendingStart( this );
				}
			}
		}
		return;
	}
	//////////////////////////

	//////////////////////////
	//	Check blending status
	pANS = ANIM_NODE::GetStructFromInstance( _AnimLink.GetNextSeq() );
	if ( _bBlendAtAnimEnd )
	{
		//	Default status
		_bIsBlending = false;
		if ( IsFinished() )
		{
			if ( pANS->_AnimStatus.IsBlended() )
			{
				//	Start blending
				_bIsBlending = true;	
				if ( !_bIsPartAnim )
				{
					_AnimLink.GetContainer()->SaveBlendingStart( this );
				}
			}
		}
	}
	//////////////////////////

	// Get Next anim and
	//	activate it if needed
	if ( _bIsBlending || IsFinished() )
	{
		ActiveNextSeq( true );
	}
	//////////////////////////

	//////////////////////////
	//	Update container
	//	active anim reference
	if ( pANS->IsActive() )
	{
		pANS->_AnimLink.SetContainer( _AnimLink.GetContainer() );
		pANS->Process( rDelay, pStr );
	}
	else
	{
		_AnimLink.GetContainer()->SetActiveAnim( COM_INSTANCE(GetAnimNode()) );
	}
	//////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		ProcessTimer
//	Object:		
//	03-07-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ProcessTimer( float& rDelay )
{
	float			rSaveDelay;
	bool			bUseToBlend;

	////////////////////////////////////
	bUseToBlend = _bBlendingWithPrevSeq;
	rSaveDelay	= rDelay;
	////////////////////////////////////

	////////////////////////////////////
	_TimeAttr.UpdateTime( rSaveDelay, _bBlendingWithPrevSeq );
	////////////////////////////////////

	////////////////////////////////////
	//	Part Anim
	if ( _bIsPartAnim )
	{
		UpdatePartAnimTimer( rDelay );
	}
	////////////////////////////////////

	////////////////////////////////////
	//	Blending activation from offset start
	_bIsBlending |= _TimeAttr.IsBlendingActived();
	if ( !bUseToBlend )
	{
		return;
	}
	////////////////////////////////////

	////////////////////////////////////
	//	Check blending end with previous sequence
	if ( _TimeAttr.BlendingIsFinished() )
	{
		ANIM_NODE_STRUCT *pANS = ANIM_NODE::GetStructFromInstance( _AnimLink.GetPrevSeq() );
		if ( pANS )
		{
			pANS->SetFinished ( true );
		}
		//	Stop potential special blending
		if ( _AnimLink.GetContainer() )
		{
			_AnimLink.GetContainer()->SaveBlendingStop();
		}
	}
	////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdatePartAnimTimer
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::UpdatePartAnimTimer(float	rDelay)
{
	if ( _TimeAttr.GetTimeToGo() <= 10.0f )
	{
		ActivePartBlending( false );
	}
	switch ( _PartBlending )
	{
	case BLENDING_TO:
		_PartBlendTimer.UpdateTime( rDelay );
		if ( _PartBlendTimer.IsFinished() )
		{
			_PartBlending = ACTIVATED;
		}
		break;

	case BLENDING_FROM:
		_PartBlendTimer.UpdateTime( rDelay );
		if ( _PartBlendTimer.IsFinished() )
		{
			_PartBlending = NO_ACTIVATED;
			_AnimLink.GetContainer()->ActivatePartAnim( false );
		}
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AnimComputation
//	Object:		
//	03-07-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::AnimComputation()
{
	StoreRotTrans( _TimeAttr.GetCurTime() );
	UpdateLastQV();
}

//-----------------------------------------------------------------------------
//	Name:		ProcessBlending
//	Object:		
//	03-07-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ProcessBlending( STR_ANIM *pSTR )
{
	int					i;
	QUATERNION_VECTOR	*pQV0;
	QUATERNION_VECTOR	*pQV1;
//	ANIM_NODE_STRUCT	*pANS;
	QUATERNION_VECTOR	*pPrevOutput;
	float				rCoeff;
	QUATERNION_VECTOR	QV1, QV2;
	STR_ANIM_BASE		StrBase;


	///////////////////////////////////////////////////
	rCoeff = _TimeAttr.GetCurrentBlendingFrame() / _TimeAttr.GetBlendingTimeLong();
	///////////////////////////////////////////////////	
	
	///////////////////////////////////////////////////
	pQV0		= _pChildrenOwn;
	pQV1		= _pChildrenOutput;
	pPrevOutput = pSTR->_pChildrenOutput;
	if ( !pPrevOutput )
	{
		ProcessNoBlending( pSTR );
		return;
	}
	///////////////////////////////////////////////////
	
	///////////////////////////////////////////////////
	for ( i = 0; i < _nNbrChildren; ++i )
	{
		pQV1->BasicInterSLERP( rCoeff, *pPrevOutput, *pQV0 );
		pQV1->Normalize();
		pQV1++;
		pQV0++;
		pPrevOutput++;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	StrBase._bAbsolute = _AnimStatus.IsAbsolute();
	StrBase._bTeleport = _AnimStatus.IsTeleport();
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	if ( _AnimStatus.IsAbsolute() )
	{
		StrBase._AbsRel._Absolute = _Absolute;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	if ( _AnimStatus.IsTeleport() )
	{
		StrBase._AbsRel._Relative.Id();
		StrBase._Teleport	= _Teleport;
	}
	else
	{
		StrBase._AbsRel._Relative = _CurrentQV;
		StrBase._Teleport.Id();
	}
	///////////////////////////////////////////////////
	
	///////////////////////////////////////////////////
	pSTR->_Own.Blend( StrBase, rCoeff );
	pSTR->_Own._AbsRel._Absolute.Normalize();
	pSTR->_Own._AbsRel._Relative.Normalize();
	pSTR->_Own._Teleport.Normalize();
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	pSTR->_Own._bTeleport |= _AnimStatus.IsTeleport();
	pSTR->_Own._bAbsolute |= _AnimStatus.IsAbsolute();
	///////////////////////////////////////////////////


	///////////////////////////////////////////////////
	pSTR->_pChildrenOutput = _pChildrenOutput;
	///////////////////////////////////////////////////
	
}

//-----------------------------------------------------------------------------
//	Name:		ProcessNoBlending
//	Object:		
//	03-07-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ProcessNoBlending( STR_ANIM *pSTR )
{
	int					i;
	QUATERNION_VECTOR	*pQV0;
	QUATERNION_VECTOR	*pQV1;

	///////////////////////////////////////////////////
	pQV0 = _pChildrenOwn;
	pQV1 = _pChildrenOutput;
	///////////////////////////////////////////////////
	
	///////////////////////////////////////////////////
	for ( i = 0; i < _nNbrChildren; ++i )
	{
		*pQV1 = *pQV0;
		pQV1++;
		pQV0++;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	pSTR->_Own._bTeleport = _AnimStatus.IsTeleport();
	pSTR->_Own._bAbsolute = _AnimStatus.IsAbsolute();
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	if ( _AnimStatus.IsAbsolute() )
	{
		pSTR->_Own._AbsRel._Absolute = _Absolute;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	if ( _AnimStatus.IsTeleport() )
	{
		pSTR->_Own._AbsRel._Relative.Id();
		pSTR->_Own._Teleport	= _Teleport;
	}
	else
	{
		pSTR->_Own._AbsRel._Relative = _CurrentQV;
		pSTR->_Own._Teleport.Id();
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	pSTR->_Own._AbsRel._Absolute.Normalize();
	pSTR->_Own._AbsRel._Relative.Normalize();
	pSTR->_Own._Teleport.Normalize();
	pSTR->_pChildrenOutput = _pChildrenOutput;
	///////////////////////////////////////////////////


}

//-----------------------------------------------------------------------------
//	Name:		ActiveNextSeq
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ActiveNextSeq(bool	b)
{
	COM_INSTANCE			CI;
	ANIM_NODE_STRUCT		*pANS;

	CI = _AnimLink.GetNextSeq();
	if ( CI.HasHandle() )
	{
		pANS = ANIM_NODE::GetStructFromInstance( CI );
		if ( pANS->IsActive() == false )
		{
			pANS->_AnimLink.SetContainer( _AnimLink.GetContainer() );
			pANS->Activate( b );
			if ( _bIsPartAnim )
			{
				OnNextPartAnimActivated( pANS );
			}
			pANS->InitBeforeStart( NULL );
			pANS->InitBeforeUpdateStart( NULL );
			if ( pANS->_AnimStatus.IsBlended() )
			{
				pANS->_bBlendingWithPrevSeq = true;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnNextPartAnimActivated
//	Object:		
//	03-12-04:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::OnNextPartAnimActivated( ANIM_NODE_STRUCT *pNext )
{
	pNext->SetPartBlendingStatus( _PartBlending );
	pNext->SetPartBlendCoeff( GetPartBlendCoeff() );
}


//-----------------------------------------------------------------------------
//	Name:		ApplyPartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ApplyPartAnim( STR_ANIM *pStrPart, STR_ANIM *pStrDst )
{
	QUATERNION_VECTOR	*pQV;
	float				rCoeff = 0.0f;
	int					i, n;
	bool				bHasBlending;


	//	Check if blending and if yes compute its coeff
	bHasBlending = false;
	switch( _PartBlending )
	{
	case	NO_ACTIVATED :
		bHasBlending = false;
		return;
		break;

	case	BLENDING_TO:
		bHasBlending = true;
		rCoeff = _PartBlendTimer.GetCurTime() / _PartBlendTimer.GetTimeLong();
		break;

	case	ACTIVATED:
		bHasBlending = false;
		break;

	case	BLENDING_FROM:
		bHasBlending = true;
		rCoeff = 1.0f - ( _PartBlendTimer.GetCurTime() / _PartBlendTimer.GetTimeLong() );
		break;
	default:
		QDT_FAIL();
		return;
		break;
	}


	if ( _nPartStartIndex == 0 )
	{
		ApplyPartFirstTrack( pStrPart, pStrDst, rCoeff, bHasBlending );
	}

	if ( pStrDst->_pChildrenOutput == NULL )
	{
		pStrDst->_pChildrenOutput = pStrPart->_pChildrenOutput;
		return;
	}
	else
	{
		pQV = pStrDst->_pChildrenOutput;
	}

	QDT_ASSERT( _nPartStartIndex >= 0 );
//	QDT_ASSERT( _nPartNbrTracks > 0 );
	if ( !bHasBlending )
	{
		for ( i = _nPartStartIndex; i < _nPartEndIndex + 1; ++i )
		{
			pQV[i] = _pChildrenOutput[i];
		}
		return;
	}

	QUATERNION_VECTOR	QV;
	n = _nPartEndIndex + 1;
	if ( n > pStrDst->_nNbrChildren )
	{
		QDT_FAIL();
		n = pStrDst->_nNbrChildren;
	}

	QDT_ASSERT( _nPartStartIndex >= 0 );
//	QDT_ASSERT( _nPartEndIndex + 1 <= pStrPart->_nNbrChildren );
	QDT_ASSERT( _nPartEndIndex <= pStrDst->_nNbrChildren );
	QDT_ASSERT( pQV );
	for ( i = _nPartStartIndex; i < _nPartEndIndex + 1; ++i )
	{
		QV		= pQV[i];
		pQV[i].BasicInterSLERP( rCoeff, QV,	_pChildrenOutput[i] );
#ifdef _DEBUG
		QDT_ASSERT( (pQV[i]._Quat.IsNormalized() ));
#endif

	}

}

//-----------------------------------------------------------------------------
//	Name:		ApplyPartFirstTrack
//	Object:		
//	04-02-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ApplyPartFirstTrack(STR_ANIM *	pStrPart,
											  STR_ANIM *	pStrSrc,
											  float			rCoeff,
											  bool			bHasBlending)
{
	if ( bHasBlending )
	{
		pStrSrc->_Own.Blend( pStrPart->_Own, rCoeff );
		pStrSrc->_Own._AbsRel._Absolute.Normalize();
		pStrSrc->_Own._AbsRel._Relative.Normalize();
		pStrSrc->_Own._Teleport.Normalize();
		///////////////////////////////////////////////////

		///////////////////////////////////////////////////
		pStrSrc->_Own._bTeleport |= _AnimStatus.IsTeleport();
		pStrSrc->_Own._bAbsolute |= _AnimStatus.IsAbsolute();
	}
	else
	{
		pStrSrc->_Own = pStrPart->_Own;
		pStrSrc->ResetCollideNotification();
	}
	pStrSrc->AddNotifyCollideForTeleport( pStrPart->MustNotifyCollideForTeleport() );		
}


//-----------------------------------------------------------------------------
//	Name:		ActivePartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ActivePartAnim(bool	bVal,
										 bool	bBlend)
{
	if ( bVal )
	{
		//	Activation
		_AnimLink.GetContainer()->ActivatePartAnim( true );
		if ( bBlend )
		{
			ActivePartBlending( true );
		}
		else
		{
			_PartBlending = ACTIVATED;
		}
	}
	else
	{
		if ( bBlend )
		{
			ActivePartBlending( false );
		}
		else
		{
			_PartBlending = NO_ACTIVATED;
		}
		if ( _PartBlending == NO_ACTIVATED )
		{
			_AnimLink.GetContainer()->ActivatePartAnim( false );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ActivePartBlending
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::ActivePartBlending(bool	bToGo)
{
	switch( _PartBlending )
	{
	case	NO_ACTIVATED :
		_PartBlendTimer.Reset();
		if ( bToGo )
		{
			_PartBlending = BLENDING_TO;
		}
		break;

	case	BLENDING_TO:
		if ( bToGo == false )
		{
			_PartBlending = BLENDING_FROM;
			_PartBlendTimer.Mirror();
		}
		break;

	case	ACTIVATED:
		_PartBlendTimer.Reset();
		if ( bToGo == false )
		{
			_PartBlending = BLENDING_FROM;
		}
		break;

	case	BLENDING_FROM:
		if ( bToGo == true )
		{
			_PartBlending = BLENDING_TO;
			_PartBlendTimer.Mirror();
		}
		break;
	default:
		QDT_FAIL();
		break;
	}
}


//-----------------------------------------------------------------------------
//	Name:		InitPartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::InitPartAnim(const COM_INSTANCE &	SklInst)
{
	SKELETON *		pSkeleton;
	NODE_ARRAY *	pN;

	pSkeleton = static_cast<SKELETON *>(SklInst.GetCommunicator());
	pN = static_cast<NODE_ARRAY *>(pSkeleton->GetNode());
	QDT_ASSERT( pN );
	if ( !pN )	return;

	QDT_ASSERT( _nNbrChildren > 0 );
	if ( _nPartStartIndex < 0 && _nPartEndIndex < 0 )
	{
		_nPartStartIndex = 0;
		_nPartEndIndex = _nNbrChildren - 1;
		_nPartNbrTracks = _nNbrChildren;
		return;
	}

	if ( _nPartNbrTracks <= 0 )
	{
		if ( _nPartEndIndex < 0 )
		{
			_nPartEndIndex = _nNbrChildren - 1;
		}
		_nPartNbrTracks = _nPartEndIndex - _nPartStartIndex + 1;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPartBlendingStatus
//	Object:		
//	03-12-04:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetPartBlendingStatus(int	nStatus)
{
	_PartBlending = static_cast<BLENDING_STATE> ( nStatus );
}

//-----------------------------------------------------------------------------
//	Name:		GetPartBlendingStatus
//	Object:		
//	03-12-04:	CVI - Created
//-----------------------------------------------------------------------------
int	ANIM_NODE_STRUCT::GetPartBlendingStatus() const
{
	return ( _PartBlending );
}

//-----------------------------------------------------------------------------
//	Name:		GetPartBlendCoeff
//	Object:		
//	03-12-04:	CVI - Created
//-----------------------------------------------------------------------------
float	ANIM_NODE_STRUCT::GetPartBlendCoeff() const
{
	return ( _PartBlendTimer.GetCurTime() / _PartBlendTimer.GetTimeLong() );
}

//-----------------------------------------------------------------------------
//	Name:		SetPartBlendCoeff
//	Object:		
//	03-12-04:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetPartBlendCoeff(float	rVal)
{
	_PartBlendTimer.SetCurrentTime( rVal * _PartBlendTimer.GetTimeLong() );
}

//-----------------------------------------------------------------------------
//	Name:		Activate
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::Activate(bool	b)
{
	if ( b && !_bIsActive )
	{
		_AnimLink.GetContainer()->ResetTeleport();
		QDT_ASSERT( _bFinished == false );
	}

	_bIsActive = b;
}

//-----------------------------------------------------------------------------
//	Name:		GetHolderNode
//	Object:		
//	01-10-22:	cvi - Created
//-----------------------------------------------------------------------------
NODE *	ANIM_NODE_STRUCT::GetHolderNode() const
{
	CONTAINER	*pC;

	///////////////////////
	pC = _AnimLink.GetContainer();
	if ( !pC )	return ( NULL );
	return( pC->GetHolderNode() );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

