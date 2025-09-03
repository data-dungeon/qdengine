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
// CLASS: ANIM_PC
//
//
//00-11-24 GHP Created: 
//*****************************************************************************

#include	"Root.h"
#include	"AnimPC.h"

#include INCL_KCOM(ComBasis/MethodDescriptor)
#include INCL_KCOM(BaseType/TypeBool)
#include INCL_KCOM(BaseType/TypeInt)
#include INCL_KCOM(BaseType/TypeFloat)
#include INCL_KCOM(ComplexType/TypeVector)
#include INCL_3DENGINE(Behavior/Datas/TrackData)
#include INCL_3DENGINE(M3D/moteur3DModule)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ANIM_PC constructor
// Object: Default constructor
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
ANIM_PC::ANIM_PC()
:
COMMUNICATOR(GetGenericCommunicatorDescriptor()),
_ObjectName("Unknown"),
_pTracks(NULL),
_nNbrFrames(0),
_nNbrTracks(0),
_rFPS(30.0f),
_bPacked(false)
{
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_PC constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_PC::ANIM_PC(CLASS_DESCRIPTOR * const	pCD)
:
COMMUNICATOR( pCD ),
_pTracks			(NULL),
_nNbrFrames			(0),
_nNbrTracks			(0),
_bPacked			(false),
_rFPS				(30.f)
{
	_ObjectName = QDT_STRING("Unknown");
}

//-----------------------------------------------------------------------------
// Name: ANIM_PC destructor
// Object: Destructor
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
ANIM_PC::~ANIM_PC()
{
	if ( GetHandle() == NULL  || GetHandle()->IsLoadedBinary() )
	{
		return;
	}

	delete [] _pTracks;
}

//-----------------------------------------------------------------------------
// Name: ANIM_PC::GetNbFrames
// Object: 
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
unsigned int ANIM_PC::GetNbFrames() const
{
	return ( _nNbrFrames ) ;
}

//-----------------------------------------------------------------------------
// Name: ANIM_PC::GetNbTracks
// Object: 
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
unsigned int ANIM_PC::GetNbTracks() const
{
	return ( _nNbrTracks ) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetFPS
//	Object:		
//	04-03-17:	AMA - Created
//-----------------------------------------------------------------------------
float	ANIM_PC::GetFPS() const
{
	return (_rFPS);
}

//-----------------------------------------------------------------------------
//	Name:		AllocTracks
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::AllocTracks()
{
	_pTracks = new TRACK_DATA[_nNbrTracks];
}

//-----------------------------------------------------------------------------
//		Name:		ANIM_PC::GetTrack
//		Object:		Returns the track pointer corresponding to the index
//		00-12-04	GHP Created: 
//-----------------------------------------------------------------------------
const TRACK_DATA* ANIM_PC::GetTrack(unsigned int index) const 
{
	QDT_ASSERT( _pTracks );
	return ( _pTracks + index ) ; 
}

//-----------------------------------------------------------------------------
//	Name:		GetTrack
//	Object:		
//	01-07-26:	GHP - Created
//-----------------------------------------------------------------------------
TRACK_DATA*	ANIM_PC::GetTrack(unsigned int index)  
{
	QDT_ASSERT( _pTracks );
	return ( _pTracks + index ) ; 
}

//-----------------------------------------------------------------------------
// Name: ANIM_PC::SetNbFrames
// Object: 
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
void ANIM_PC::SetNbFrames(unsigned int  Nb)
{
	_nNbrFrames = Nb;
}

//-----------------------------------------------------------------------------
// Name: ANIM_PC::SetNbTracks
// Object: 
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
void ANIM_PC::SetNbTracks(unsigned int  Nb)
{
	_nNbrTracks = Nb;
}

//-----------------------------------------------------------------------------
//	Name:		SetCompressed
//	Object:		
//	03-09-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::SetCompressed(bool	b)
{
	_bPacked = b;
}

//-----------------------------------------------------------------------------
//	Name:		IsCompressed
//	Object:		
//	03-09-18:	CVI - Created
//-----------------------------------------------------------------------------
bool	ANIM_PC::IsCompressed() const
{
	return ( _bPacked );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsolute
//	Object:		
//	03-04-25:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR	ANIM_PC::GetAbsolute( float rFrame ) 
{
	TRACK_DATA*			pTrack;
	QUATERNION_VECTOR	QV, QVTemp;
	VECTOR				V;
	QUATERNION			Q;
	QUATERNION_VECTOR	QVRel;


	pTrack = GetTrack( 0 );
	QDT_ASSERT( pTrack );
	if ( !pTrack )
	{
		return ( QV );
	}
	
	/////////////////////////////////
	pTrack->GetBaseTranslation( V );
	pTrack->GetBaseRotation( Q );
	QV.SetVector( V );
	QV.SetQuat( Q );
	/////////////////////////////////
	if ( rFrame < 0.0f )
	{
		return ( QV );
	}
	/////////////////////////////////

	/////////////////////////////////
	pTrack->GetRotTrans(rFrame, QVRel);
	QVTemp = QV;
	QV = QVTemp * QVRel;
	/////////////////////////////////

	return ( QV );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsolutePosition
//	Object:		
//	03-04-25:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR	ANIM_PC::GetAbsolutePosition() const
{
	const TRACK_DATA *	pTrack;
	VECTOR				V;

	pTrack = GetTrack( 0 );
	QDT_ASSERT( pTrack );
	if ( pTrack )
	{
		pTrack->GetBaseTranslation( V );
	}
	return ( V );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsoluteRotation
//	Object:		
//	03-04-25:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION	ANIM_PC::GetAbsoluteRotation() const
{
	const TRACK_DATA *	pTrack;
	QUATERNION			Q;


	pTrack = GetTrack( 0 );
	QDT_ASSERT( pTrack );
	if ( pTrack )
	{
		pTrack->GetBaseRotation( Q );
	}
	return ( Q );
}

//-----------------------------------------------------------------------------
//	Name:		ReadAnimationParam
//	Object:		
//	01-02-26:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
bool ANIM_PC::ReadAnimationParam( IO_STREAM & Stream ) 
{	
	Stream >>_nNbrFrames;
	Stream >> _nNbrTracks;
	Stream >> _bPacked;
	Stream >> _rFPS;

	return ( true );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-12:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
bool ANIM_PC::WriteAnimationParam(IO_STREAM & Stream ) const 
{	
	Stream << _nNbrFrames;
	Stream << _nNbrTracks;
	Stream << _bPacked;
	Stream << _rFPS;

	return (true);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadTrackData
//	Object:		
//	01-02-26:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
bool ANIM_PC::ReadTrackData(IO_STREAM & Stream ) 
{
	unsigned int i;

	AllocTracks();

	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].Init();
		_pTracks[i].ReadV4( Stream );
	}

	return ( true ) ;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteTrackData	
//	Object:		
//	01-04-12:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
bool ANIM_PC::WriteTrackData(IO_STREAM & Stream ) const 
{
	unsigned int		i;

	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].Write( Stream );
	}

	return ( true );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetAnimHeading
//	Object:		
//	03-05-06:	CVI - Created
//-----------------------------------------------------------------------------
float	ANIM_PC::GetAnimHeading(bool	bAbs,
								float	rFrame)
{
	float			rNumFrame;
	QUATERNION		QP,QH, QR;
	QUATERNION		Q;
	TRACK_DATA		*pT;
	MATRIX			M;
	VECTOR			V;
	float			rA;

	///////////////////////////
	if ( rFrame < 0 )
	{
		rNumFrame = _nNbrFrames;
	}
	else
	{
		rNumFrame = rFrame;
	}
	///////////////////////////

	///////////////////////////
	pT = GetTrack( 0 );
	///////////////////////////

	///////////////////////////
	if ( bAbs )
	{
		QUATERNION	Qb;
		QUATERNION	Qt;

		//	Composition with absolute base
		pT->GetBaseRotation( Qb );
		pT->GetRot( rNumFrame, Qt);
		//
		Q = Qb * Qt;
	}
	else
	{
		//
		pT->GetRot(rNumFrame, Q);
	}
	///////////////////////////

	///////////////////////////
	M.Identity();
	M.SetQuaternion( Q );
	M.GetPitchHeadRoll( QP, QH, QR );
	///////////////////////////

	///////////////////////////
	QH.GetVectRoll( V, rA );
	return ( rA );
	///////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		GetAnimPitch
//	Object:		
//	03-05-06:	CVI - Created
//-----------------------------------------------------------------------------
float	ANIM_PC::GetAnimPitch(bool	bAbs,
							  float	rFrame)
{
	float			rNumFrame;
	QUATERNION		QP,QH, QR;
	QUATERNION		Q;
	TRACK_DATA		*pT;
	MATRIX			M;
	VECTOR			V;
	float			rA;

	///////////////////////////
	if ( rFrame < 0 )
	{
		rNumFrame = _nNbrFrames;
	}
	else
	{
		rNumFrame = rFrame;
	}
	///////////////////////////

	///////////////////////////
	pT = GetTrack( 0 );
	///////////////////////////

	///////////////////////////
	if ( bAbs )
	{
		QUATERNION	Qb;
		QUATERNION	Qt;

		//	Composition with absolute base
		pT->GetBaseRotation( Qb );
		pT->GetRot(rNumFrame, Qt);
		//
		Q = Qb * Qt;
	}
	else
	{
		//
		pT->GetRot( rNumFrame, Q);
	}
	///////////////////////////

	///////////////////////////
	M.Identity();
	M.SetQuaternion( Q );
	M.GetPitchHeadRoll( QP, QH, QR );
	///////////////////////////

	///////////////////////////
	QP.GetVectRoll( V, rA );
	return ( rA );
	///////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimRoll
//	Object:		
//	03-05-06:	CVI - Created
//-----------------------------------------------------------------------------
float	ANIM_PC::GetAnimRoll(bool	bAbs,
							 float	rFrame)
{
	float			rNumFrame;
	QUATERNION		QP,QH, QR;
	QUATERNION		Q;
	TRACK_DATA		*pT;
	MATRIX			M;
	VECTOR			V;
	float			rA;

	///////////////////////////
	if ( rFrame < 0 )
	{
		rNumFrame = _nNbrFrames;
	}
	else
	{
		rNumFrame = rFrame;
	}
	///////////////////////////

	///////////////////////////
	pT = GetTrack( 0 );
	///////////////////////////

	///////////////////////////
	if ( bAbs )
	{
		QUATERNION	Qb;
		QUATERNION	Qt;

		//	Composition with absolute base
		pT->GetBaseRotation( Qb );
		pT->GetRot(rNumFrame, Qt);
		//
		Q = Qb * Qt;
	}
	else
	{
		//
		pT->GetRot( rNumFrame, Q);
	}
	///////////////////////////

	///////////////////////////
	M.Identity();
	M.SetQuaternion( Q );
	M.GetPitchHeadRoll( QP, QH, QR );
	///////////////////////////

	///////////////////////////
	QR.GetVectRoll( V, rA );
	return ( rA );
	///////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimTrans
//	Object:		
//	03-05-06:	CVI - Created
//-----------------------------------------------------------------------------
VECTOR	ANIM_PC::GetAnimTrans(bool	bAbs,
							  float	rFrame)
{
	float			rNumFrame;
	TRACK_DATA		*pT;
	VECTOR			V;

	///////////////////////////
	if ( rFrame < 0 )
	{
		rNumFrame = _nNbrFrames;
	}
	else
	{
		rNumFrame = rFrame;
	}
	///////////////////////////

	///////////////////////////
	pT = GetTrack( 0 );
	///////////////////////////

	///////////////////////////
	if ( bAbs )
	{
		QUATERNION_VECTOR	QV;
		QUATERNION	Qb;
		VECTOR		Vb;
		VECTOR		Vt;

		//	Composition with absolute base
		pT->GetBaseRotation( Qb );
		pT->GetBaseTranslation( Vb );
		//
		QV.SetQuat( Qb );
		QV.SetVector( Vb );
		//
		pT->GetTrans( rNumFrame, Vt);
		//
		V = QV * Vt;
	}
	else
	{
		//
		pT->GetTrans( rNumFrame, V);
	}
	///////////////////////////

	///////////////////////////
	return ( V );
	///////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-03-25:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR			*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetNbrFrames", "Retreive number of frames");
	METHOD_DESCRIPTOR__SetReturn(pMD, "nFrames", "Nbr Frames", TYPE_INT::GetDescriptor());
	pCD->AddMethod(COMMAND_GET_NB_FRAMES, pMD, EM_GetNbrFrames);
	
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetAnimTrans", "Retreive animation translation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nVal", "Num frame ( all anim if -1 )", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Use absolute value", TYPE_BOOL::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD, "V", "Translation vector", TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(COMMAND_GET_TRANS, pMD, EM_GetTrans);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetAnimHeading", "Retreive heading of the animation at the specified frame");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nVal", "Number of frame", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Use absolute value", TYPE_BOOL::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD, "rVal", "Heading angle", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(COMMAND_GET_HEADING, pMD, EM_GetHead);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetAnimPitch", "Retreive pitch of the animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nVal", "Number of frame", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Use absolute value", TYPE_BOOL::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD, "rVal", "Pitch angle", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(COMMAND_GET_PITCH, pMD, EM_GetPitch);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetAnimRoll", "Retreive roll of the animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nVal", "Number of frame", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Use absolute value", TYPE_BOOL::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD, "rVal", "Roll angle", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(COMMAND_GET_ROLL, pMD, EM_GetRoll);
	//

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "ANIMPARM", 1, CM_ReadAnimationParam);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "ANIMPARM", 2, CM_ReadAnimationParam2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "TRACKDAT", 1, CM_ReadTrackData);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk( "ANIMPARM", 1, CM_WriteAnimationParam);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk( "ANIMPARM", 2, CM_WriteAnimationParam2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk( "TRACKDAT", 1, CM_WriteTrackData);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	CVI - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ANIM_PC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ANIM_PC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ANIM_PC::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("ANIM_DATA"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ANIM_PC, 
											QDT_COMMENT("627D7423-83DC-35DE-4753-1B7F2558D7B9"), 
											sizeof(ANIM_PC), 
											COMInit, 
											QDT_COMMENT("Base data node animation Class")));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-03-25:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	ANIM_PC::GetClassGUID() const
{
	return ( _sClassGUID );
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-03-25:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_Construct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	ANIM_PC		*pA = static_cast<ANIM_PC*> ( pO );
	QDT_NEW_PLACED( pA, ANIM_PC ) ();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_Destruct(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	ANIM_PC		*pA = static_cast<ANIM_PC*> ( pO );
	
	QDT_DELETE_PLACED( pA, ANIM_PC );
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-10-23:	AMA - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_Construct_In_Memory(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	QDT_FAIL();	
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadAnimationParam2
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	ANIM_PC::CM_ReadAnimationParam2(IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	ANIM_PC	*pA = static_cast<ANIM_PC*>( pCommunicator );

	pA->ReadAnimationParam( Stream );
	pA->ReadObjectName( Stream );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteAnimationParam2
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	ANIM_PC::CM_WriteAnimationParam2(IO_STREAM &	Stream,
										 COMMUNICATOR *	pCommunicator)
{
	ANIM_PC	*pA = static_cast<ANIM_PC*>( pCommunicator );

	pA->WriteAnimationParam( Stream );
	pA->WriteObjectName( Stream );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_ReadAnimationParam
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	ANIM_PC::CM_ReadAnimationParam(IO_STREAM &		Stream,
									   COMMUNICATOR *	pCommunicator)
{
	ANIM_PC	*pA = static_cast<ANIM_PC*>( pCommunicator );
	pA->ReadAnimationParam( Stream );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_ReadTrackData
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	ANIM_PC::CM_ReadTrackData(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	ANIM_PC	*pA = static_cast<ANIM_PC*>( pCommunicator );
	pA->ReadTrackData( Stream );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteAnimationParam
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	ANIM_PC::CM_WriteAnimationParam(IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	ANIM_PC	*pA = static_cast<ANIM_PC*>( pCommunicator );
	pA->WriteAnimationParam( Stream );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteTrackData
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	ANIM_PC::CM_WriteTrackData(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	ANIM_PC	*pA = static_cast<ANIM_PC*>( pCommunicator );
	pA->WriteTrackData( Stream );
}
#endif

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//	COMMANDS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//	Name:		EM_GetNbrFrames
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_GetNbrFrames(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE	*pCI = static_cast<COM_INSTANCE*> ( pO );
	ANIM_PC	*pA = static_cast<ANIM_PC*> ( pCI->GetCommunicator() );

	int		*pI;
	pI = (int*) pR;
	*pI = pA->GetNbFrames();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTrans
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_GetTrans(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	
	struct LOCAL_PARAM
	{
		int		_nFrame;
		bool	_bAbs;
	};
	
	COM_INSTANCE	*pCI = static_cast<COM_INSTANCE*> ( pO );
	ANIM_PC	*pA = static_cast<ANIM_PC*> ( pCI->GetCommunicator() );


	VECTOR			V;
	VECTOR			*pV;
	LOCAL_PARAM		*pPP;

	pPP = (LOCAL_PARAM*) pP;
	V = pA->GetAnimTrans( pPP->_bAbs, pPP->_nFrame );
	
	pV = (VECTOR*) pR;
	QDT_NEW_PLACED( pV, VECTOR ) ();
	*pV = V;
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetHead
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_GetHead(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE	*pCI = static_cast<COM_INSTANCE*> ( pO );
	ANIM_PC	*pA = static_cast<ANIM_PC*> ( pCI->GetCommunicator() );


	int		nFrame;
	float	rA;
	float	*pF;

	nFrame = *((int*) pP );
	rA = pA->GetAnimHeading( false, nFrame );

	pF = (float*) pR;
	rA = rA * 180.0f / MATH::PI();
	*pF = rA;
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetPitch
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_GetPitch(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE	*pCI = static_cast<COM_INSTANCE*> ( pO );
	ANIM_PC	*pA = static_cast<ANIM_PC*> ( pCI->GetCommunicator() );


	int		nFrame;
	float	rA;
	float	*pF;

	nFrame = *((int*) pP );
	rA = pA->GetAnimPitch( false, nFrame );

	pF = (float*) pR;
	rA = rA * 180.0f / MATH::PI();
	*pF = rA;
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetRoll
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::EM_GetRoll(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE	*pCI = static_cast<COM_INSTANCE*> ( pO );
	ANIM_PC	*pA = static_cast<ANIM_PC*> ( pCI->GetCommunicator() );


	int		nFrame;
	float	rA;
	float	*pF;

	nFrame = *((int*) pP );
	rA = pA->GetAnimRoll( false, nFrame );

	pF = (float*) pR;
	rA = rA * 180.0f / MATH::PI();
	*pF = rA;
}

//-----------------------------------------------------------------------------
//	Name:		GetObjectName
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	ANIM_PC::GetObjectName() const
{
	return ( _ObjectName );
}

//-----------------------------------------------------------------------------
//	Name:		SetObjectName
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC::SetObjectName(const QDT_STRING &	ObjName)
{
	_ObjectName = ObjName;
}

//-----------------------------------------------------------------------------
//	Name:		WriteObjectName
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
bool	ANIM_PC::WriteObjectName(IO_STREAM &	Stream) const
{
	Stream << _ObjectName;
	return ( true );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadObjectName
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
bool	ANIM_PC::ReadObjectName(IO_STREAM &	Stream)
{
	Stream >> _ObjectName;
	return ( true );
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
