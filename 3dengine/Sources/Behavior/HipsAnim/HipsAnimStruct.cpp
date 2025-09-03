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
//	CLASS:	HIPS_ANIM_STRUCT
//
//	01-10-18:	cvi - Created
//*****************************************************************************

#include "Root.h"
#include "HipsAnimStruct.h"
#include INCL_3DENGINE(Behavior/AnimWrapper/IAnimWrapper)
#include INCL_3DENGINE(Behavior/AnimWrapper/ITrackWrapper)
#include INCL_3DENGINE(Behavior/AnimNode/AnimNode)
#include INCL_3DENGINE(Behavior/BehaviorManager)
#include INCL_3DENGINE(Behavior/HandleStreamAnim/StreamAnimWrapper)
#include INCL_3DENGINE(Behavior/HipsAnim/AnimPcWrapper)
#include INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include "HipsAnimStruct.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM_STRUCT constructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM_STRUCT::HIPS_ANIM_STRUCT() :
_pAnimWrapper	(NULL)
{
	///////////////////////////
	_pTrackWrapper = NULL;
	///////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM_STRUCT constructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM_STRUCT::HIPS_ANIM_STRUCT(const HIPS_ANIM_STRUCT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM_STRUCT destructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM_STRUCT::~HIPS_ANIM_STRUCT()
{
	///////////////////////////////////
	if ( _pAnimWrapper )	delete		_pAnimWrapper;
	///////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM_STRUCT &	HIPS_ANIM_STRUCT::operator=(const HIPS_ANIM_STRUCT & C)
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
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM_STRUCT::Copy(const HIPS_ANIM_STRUCT & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		ExecInstanciate
//	Object:		
//	04-02-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	HIPS_ANIM_STRUCT::ExecInstanciate(const COM_INSTANCE &	Anim)
{
#if 0
	bool	bRes;


	switch( Anim.GetType() )
	{
	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ANIM_PC:
		bRes = InstanciateAnimPC( Anim );
		break;

	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAM_ANIMATION:
		bRes = InstanciateStreamedAnim( Anim );
		break;

	default:
		QDT_FAIL();
		bRes = false;
	}
	return ( bRes );
#else
	bool		bRes;

	//////////////////////////////////////////
	bRes = CreateAndConfigureWrapper( Anim );
	QDT_ASSERT( bRes );
	if ( bRes == false )
	{
		return ( false );
	}
	//////////////////////////////////////////

	//////////////////////////////////////////
	bRes = InstanciateAnim();
	QDT_ASSERT( bRes );
	return ( bRes );
	//////////////////////////////////////////

#endif
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateAnim
//	Object:		
//	01-10-30:	cvi - Created
//-----------------------------------------------------------------------------
bool	HIPS_ANIM_STRUCT::InstanciateAnim()
{
	int						n;
	ANIM_STATUS				AnimStatus;
	COM_INSTANCE			This;

	/////////////////////////////////////
	Init();
	This = COM_INSTANCE(GetAnimNode());
	n = _pAnimWrapper->GetNbrTracks();
	/////////////////////////////////////

	/////////////////////////////////////
	_bAnimDeleted	= false;
	_bFinished		= false;
	/////////////////////////////////////
	
	/////////////////////////////////////
	if ( n < 0 )
	{
		QDT_Error(QDT_COMMENT(" HIPS_ANIM_STRUCT::InstanciateAnim(), Wrong Data Animation"));
	}
	/////////////////////////////////////

	/////////////////////////////////////	
	_TimeAttr.Init();
	_TimeAttr.SetInfinite( false );
	_TimeAttr.SetEndTime( _pAnimWrapper->GetNbrFrames() );

	if ( _TimeAttr.GetSpeed() < 0.0f )
	{
		float	rs = _TimeAttr.GetSpeed();
//		TimeAttr.ExStartEndTime();
		QDT_NOT_IMPLEMENTED();
		_TimeAttr.SetCurTime( _TimeAttr.GetStartTime() );
		_TimeAttr.SetLastTime( _TimeAttr.GetCurTime() );
		_TimeAttr.SetSpeed( rs );
	}
	/////////////////////////////////////

	/////////////////////////////////////
	InitChildren( n - 1 );
	_pTrackWrapper = _pAnimWrapper->GetTrack( 0 );
	QDT_ASSERT( _pTrackWrapper );
	/////////////////////////////////////

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ExecDelete
//	Object:		
//	03-06-05:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM_STRUCT::ExecDelete()
{
	COM_INSTANCE	CI;

	if ( _pAnimWrapper )
	{
		_pAnimWrapper->ReleaseAnim();
	}
	
	_Anim.Release();

	CI = COM_INSTANCE(GetAnimNode());
	if (_AnimStatus.CanBeDelete() )
	{
		BEHAVIOR_MANAGER::Instance()->AddFreeHipsAnim( CI );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamHandle
//	Object:		
//	03-06-20:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE *	HIPS_ANIM_STRUCT::GetStreamHandle()
{
	STREAM_ANIM_WRAPPER *pW;

	pW = static_cast<STREAM_ANIM_WRAPPER*> ( _pAnimWrapper );
	return ( pW->GetStreamHandle() );
}

//-----------------------------------------------------------------------------
//	Name:		ReadRotTrans
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM_STRUCT::ReadRotTrans(float	rTime)
{

//	QDT_ASSERT(_CurrentQV.GetQuat().IsNormalized());
	_pAnimWrapper->UpdateCommunicator();
	_pTrackWrapper->GetRotTrans( rTime, _CurrentQV );
	_pAnimWrapper->ReadRotTrans( rTime, _pChildrenOwn );
}

//-----------------------------------------------------------------------------
//	Name:		ReadRotation
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM_STRUCT::ReadRotation(float		rTime,
									   QUATERNION &	Quat)
{
	QUATERNION		BaseQuat;
	QUATERNION		TempQuat;
	/////////////////////////////
	Quat.Id();
	if ( !_pTrackWrapper )
	{
		QDT_Error(QDT_COMMENT("No trackWrapper for this track"));
		return;
	}
	/////////////////////////////

	/////////////////////////////
	QDT_ASSERT( _bAnimDeleted == false );
	_pAnimWrapper->UpdateCommunicator();
	_pTrackWrapper->GetRotation( rTime, Quat );
//	QDT_ASSERT(Quat.IsNormalized());
}

//-----------------------------------------------------------------------------
//	Name:		ReadTranslation
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM_STRUCT::ReadTranslation(float		rTime,
										  VECTOR &	Vect)
{
	////////////////////////////////////////
	Vect.SetXYZ( 0.0f, 0.0f, 0.0f );
	if ( !_pTrackWrapper )	return;
	////////////////////////////////////////

	////////////////////////////////////////
	_pAnimWrapper->UpdateCommunicator();
	_pTrackWrapper->GetTranslation( rTime, Vect );
	////////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		InitAbsoluteTransfo
//	Object:		
//	03-08-11:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM_STRUCT::InitAbsoluteTransfo()
{
	_pTrackWrapper->GetBaseRotation( _Absolute._Quat );
	_pTrackWrapper->GetBaseTranslation( _Absolute._Vect );
}

//-----------------------------------------------------------------------------
//	Name:		CreateWrapperFromAnim
//	Object:		
//	05-03-01:	CVI - Created
//-----------------------------------------------------------------------------
IANIM_WRAPPER *	HIPS_ANIM_STRUCT::CreateWrapperFromAnim(const COM_INSTANCE &	Anim)
{
	IANIM_WRAPPER	*pAnimWrapper;

	pAnimWrapper	= NULL;

	switch( Anim.GetType() )
	{
	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ANIM_PC:
		pAnimWrapper = new ANIM_PC_WRAPPER();		
		break;

	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAM_ANIMATION:
		pAnimWrapper = new STREAM_ANIM_WRAPPER();
		break;

	default:
		QDT_FAIL();
		pAnimWrapper	= NULL;
	}
	return ( pAnimWrapper );
}

//-----------------------------------------------------------------------------
//	Name:		CreateAndConfigureWrapper
//	Object:		
//	05-03-01:	CVI - Created
//-----------------------------------------------------------------------------
bool	HIPS_ANIM_STRUCT::CreateAndConfigureWrapper( const COM_INSTANCE& Anim )
{
	/////////////////////////////////////
	QDT_ASSERT( Anim.HasHandle() );
	if ( !Anim.HasHandle() )		return( false );
	/////////////////////////////////////

	/////////////////////////////////////
	if ( _pAnimWrapper )	delete _pAnimWrapper;
	SetAnim( COM_INSTANCE::GetNullComInstance() );
	_pAnimWrapper = HIPS_ANIM_STRUCT::CreateWrapperFromAnim( Anim );
	if ( _pAnimWrapper == NULL )
	{
#ifndef _MASTER
		QDT_Error("HIPS_ANIM_STRUCT:: Could not instanciate animation, animation type for %s unsupported", Anim.GetName().GetBuffer() );
#endif
		return ( false );
	}
	/////////////////////////////////////

	/////////////////////////////////////
	COM_INSTANCE	AnimInst;
	AnimInst = Anim;
	_pAnimWrapper->SetAnim( AnimInst );
	SetAnim( Anim );
	/////////////////////////////////////

	return ( true );	
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
