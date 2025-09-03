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
//	CLASS:	ANIM_NODE
//
//	01-10-11:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"AnimNode.h"
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(ComplexType/TypeVector)
#include	INCL_KCOM(ComplexType/TypeQuaternion)
#include	INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include "AnimNode.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

int	nNbrAnimAllocated = 0;
int	nNbrAnimDeAllocated = 0;

//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE::ANIM_NODE()
:
COMMUNICATOR(GetGenericCommunicatorDescriptor()),
_pStruct( NULL )
{
nNbrAnimAllocated++;
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_NODE::ANIM_NODE(CLASS_DESCRIPTOR * const	pCD)
:
COMMUNICATOR( pCD ),
_pStruct( NULL )
{
nNbrAnimAllocated++;
}


//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE::ANIM_NODE(const ANIM_NODE & C)
:
COMMUNICATOR(GetGenericCommunicatorDescriptor())
{
	nNbrAnimAllocated++;
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_NODE destructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE::~ANIM_NODE()
{
	nNbrAnimDeAllocated--;
	_pStruct = NULL;
	MODULE_ABC::RemoveClassDescriptorLoop(this);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE &	ANIM_NODE::operator=(const ANIM_NODE & C)
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
void	ANIM_NODE::Copy(const ANIM_NODE & C)
{

}


//-----------------------------------------------------------------------------
//	Name:		GetAnimFromInstance
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
static ANIM_NODE *	GetAnimFromInstance(const COM_INSTANCE &	CI) 
{
	ANIM_NODE	*pAN = NULL;

	if ( CI.HasHandle() )
	{
		pAN = static_cast<ANIM_NODE*> ( CI.GetCommunicator() );
	}
	
	return ( pAN );
}

//-----------------------------------------------------------------------------
//	Name:		GetStructFromInstance
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT *	ANIM_NODE::GetStructFromInstance( const COM_INSTANCE &	CI)

{
	ANIM_NODE			*pAN;
	ANIM_NODE_STRUCT	*pS;

	pAN = GetAnimFromInstance( CI );
	if ( !pAN )	return ( NULL );
	pS = pAN->_pStruct;
	return ( pS );
}



//=============================================================================
//	COMMUNICATOR
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR			*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetAnimWeight", "Set the weight of the animation to be mixed with another one");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rWeight", "Weight values ( from 0 to 1 )", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_ANIM_WEIGHT, pMD, EM_SetAnimWeight);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetRotationWeight", "Set the rotation weight of an animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rWeight", "Weight value ( from 0 to 1 )", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_ROTATION_WEIGHT, pMD, EM_SetRotationWeight);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetTranslationWeight", "Set the translation weight");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rWeight", "Weight value ( from 0 to 1 )", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_TRANSLATION_WEIGHT, pMD, EM_SetTranslationWeight);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetStartTime", "Set start frame number of the animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rWeight", "Start frame", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_START_TIME, pMD, EM_SetStartTime);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetEndTime", "Set the end frame of an animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rWeight", "End frame", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_END_TIME, pMD, EM_SetEndTime);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetSpeed", "Set the animation speed");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rWeight", "Speed value", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_SPEED, pMD, EM_SetSpeed);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("AddSequence", "Add an animation at the end of the sequence");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Anim", "Animation to add", ANIM_NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(ADD_SEQUENCE, pMD, EM_AddSequence);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("InsertSequence", "Insert an animation in the sequence");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Anim", "Animation to add", ANIM_NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(INSERT_SEQUENCE, pMD, EM_InsertSequence);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetLoop", "Set loop status of an animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", " status value", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_LOOP, pMD, EM_SetLoop);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetNbrLoops", "Set Number of loops of an animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nVal", "Number of loops", TYPE_INT::GetDescriptor());
	pCD->AddMethod(SET_NBR_LOOPS, pMD, EM_SetNbrLoops);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetPingPong", "Set PingPong status");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "PingPong status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_PING_PONG, pMD, EM_SetPingPong);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetAbsolute", "Set absolute status");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Absolute status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_ABSOLUTE, pMD, EM_SetAbsolute);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetTeleport", "Set teleport status");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Teleport status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_TELEPORT, pMD, EM_SetTeleport);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetBlending", "Set blending status");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Blending status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_BLENDING, pMD, EM_SetBlending);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetPreemptive", "Set preemptive status");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Peemptive status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_PREEMPTIVE, pMD, EM_SetPreemptive);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("RemoveSequence", "Remove an animation from the sequence");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Anim", "Anim to remove", ANIM_NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(REMOVE_SEQUENCE, pMD, EM_RemoveSequence);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("PopAnim", "Pop a child animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Anim", "Anim to pop", ANIM_NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(POP_ANIM, pMD, EM_PopAnim);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("ScaleTranslation", "Scale the translation of an animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rX", "Scale on X", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "rY", "Scale on Y", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "rZ", "Scale on Z", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SCALE_TRANSLATION, pMD, EM_ScaleTranslation);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("IsPlaying", "Check wether an animation is running or not");
	METHOD_DESCRIPTOR__SetReturn(pMD,"bVal", "Running status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(IS_PLAYING, pMD, EM_IsPlaying);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("Pause", "Pause an animation");
	pCD->AddMethod(PAUSE, pMD, EM_Pause);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("Resume", "Resume a previously paused animation");
	pCD->AddMethod(RESUME, pMD, EM_Resume);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("Stop", "Stop an animation");
	pCD->AddMethod(STOP, pMD, EM_Stop);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetAnimChild", "Get a child animation following its position order");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nPos", "Index of position", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD,"Anim", "Return wanted animation", ANIM_NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(GET_ANIM_CHILD, pMD, EM_GetAnimChild);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetChildIndex", "Get the position order of a child anim passed in argument");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Anim", "Child anim to reteive the position order", ANIM_NODE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD,"nPos", "Position order", TYPE_INT::GetDescriptor());
	pCD->AddMethod(GET_CHILD_INDEX, pMD, EM_GetChildIndex);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetOffsetStart", "Set the number of frame before the end of the previous animation to start this animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rVal", "Number of frame", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_OFFSET_START, pMD, EM_SetOffsetStart);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetManualTimer", "Set manual timer status");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Manual timer status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(SET_MANUAL_TIMER, pMD, EM_SetManualTimer);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("IncrementTimer", "Increment timer in manual timer case");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rVal", "Number of frames", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(INCREMENT_TIMER, pMD, EM_IncrementTimer);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("IsManualTimer", "Get manual timer status");
	METHOD_DESCRIPTOR__SetReturn(pMD,"bVal", "Return manaula timer status", ANIM_NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(IS_MANUAL_TIMER, pMD, EM_IsManualTimer);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetCurTime", "Set current frame of an animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rVal", "Current animation frame", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_CUR_TIME, pMD, EM_SetCurTime);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("EnableDeletation", "Enable\\Disable automatic deletetion of the animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Automatic deletation status", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(ENABLE_DELETATION, pMD, EM_EnableDeletation);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("ActiveCountDown", "Active a timer count down to stop the current animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rVal", "Countdown number of frames", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(ACTIVE_COUNT_DOWN, pMD, EM_ActiveCountDown);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetNbrBlendingFrames", "Set the number of blending frames");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rVal", "Number of frames", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(SET_NBR_BLENDING_FRAMES, pMD, EM_SetNbrBlendingFrames);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ANIM_NODE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ANIM_NODE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ANIM_NODE::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("ANIM_NODE"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ANIM_NODE,
											QDT_COMMENT("637D5423-82DC-31DE-3753-1B7F2558D7B9"),
											sizeof(ANIM_NODE), 
											COMInit,
											QDT_COMMENT("Base Animation Class")));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	ANIM_NODE::GetClassGUID() const
{
	return	(_sClassGUID);
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	ANIM_NODE*	pAN;
	
	pAN = (ANIM_NODE*) pO;
	
	QDT_NEW_PLACED(pAN, ANIM_NODE)();
	QDT_ASSERT(pAN);

}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	ANIM_NODE*	pObject;
	pObject = static_cast<ANIM_NODE*>(pO);
	pObject->ANIM_NODE::~ANIM_NODE();

}



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///	COMMANDS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//	Name:		EM_SetAnimWeight
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetAnimWeight(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	ANIM_NODE			*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32			rWeight;

	rWeight = *((Float32*)pP);
	pANS->SetAnimWeight( rWeight );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetRotationWeight
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetRotationWeight(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32			rWeight;

	rWeight = *((Float32*)pP);
	pANS->SetRotationWeight( rWeight );

}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTranslationWeight
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetTranslationWeight(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32			rWeight;

	rWeight = *((Float32*)pP);
	pANS->SetTranslationWeight( rWeight );

}

//-----------------------------------------------------------------------------
//	Name:		EM_SetStartTime
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetStartTime(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;
	
	rVal = *((Float32*)pP);
	pANS->SetStartTime( rVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetEndTime
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetEndTime(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;
	
	rVal = *((Float32*)pP);
	pANS->SetEndTime( rVal );

}

//-----------------------------------------------------------------------------
//	Name:		SetSpeed
//	Object:		
//	03-04-25:	DAC - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::SetSpeed(Float32	rSpeed)
{
	ANIM_NODE_STRUCT	*pANS = GetAnimStruct();

	pANS->SetSpeed( rSpeed);
}



//-----------------------------------------------------------------------------
//	Name:		EM_SetSpeed
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetSpeed(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	Float32		rVal;
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );

	rVal = *((Float32*)pP);
	pAN->SetSpeed(rVal);
}


//-----------------------------------------------------------------------------
//	Name:		EM_AddSequence
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_AddSequence(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	COM_INSTANCE	Inst;

	Inst = *(static_cast<COM_INSTANCE*> ( pP ));
	if ( !Inst.HasHandle() )	return;
	pANS->AddSequence( Inst );

}


//-----------------------------------------------------------------------------
//	Name:		EM_InsertSequence
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_InsertSequence(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	COM_INSTANCE	Inst;

	Inst = *(static_cast<COM_INSTANCE*> ( pP ));
	if ( !Inst.HasHandle() )	return;
	pANS->InsertSequence( Inst );
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetLoop
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetLoop(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetLoop( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetNbrLoops
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetNbrLoops(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	int		nVal;

	nVal = *((int*)pP);
	pANS->SetNbrLoops( nVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetPingPong
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetPingPong(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetPingPong( bVal );


}


//-----------------------------------------------------------------------------
//	Name:		EM_SetAbsolute
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetAbsolute(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetAbsolute( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetTeleport
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetTeleport(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetTeleport( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetBlending
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetBlending(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetBlending( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetPreemptive
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetPreemptive(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetPreemptive( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_RemoveSequence
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_RemoveSequence(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	COM_INSTANCE	Inst;

	Inst = *(static_cast<COM_INSTANCE*> ( pP ));
	if ( !Inst.HasHandle() )	return;
	pANS->RemoveSequence( Inst );


}


//-----------------------------------------------------------------------------
//	Name:		EM_PopAnim
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_PopAnim(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();
	COM_INSTANCE	Inst;

	Inst = *(static_cast<COM_INSTANCE*> ( pP ));
	if ( !Inst.HasHandle() )	return;
	pANS->PopAnim( Inst );

}


//-----------------------------------------------------------------------------
//	Name:		EM_ScaleTranslation
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_ScaleTranslation(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	struct LOCAL_PARAM
	{
		Float32	_rX,_rY,_rZ;
	};

	LOCAL_PARAM		*pParam;

	pParam = (LOCAL_PARAM*)( pP );
	pANS->ScaleTranslation( pParam->_rX, pParam->_rY, pParam->_rZ );

}


//-----------------------------------------------------------------------------
//	Name:		EM_IsPlaying
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_IsPlaying(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool bIsPlaying;
	bool *pB;

	bIsPlaying = true;
	if ( pANS->IsFinished() )
	{
		bIsPlaying = false;
	}
	
	pB = (bool*) pR;
	*pB = bIsPlaying;

}


//-----------------------------------------------------------------------------
//	Name:		EM_Pause
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_Pause(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	pANS->PauseAnim();

}


//-----------------------------------------------------------------------------
//	Name:		EM_Resume
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_Resume(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();
	
	pANS->Resume();
}


//-----------------------------------------------------------------------------
//	Name:		EM_Stop
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_Stop(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	pANS->Stop();
}


//-----------------------------------------------------------------------------
//	Name:		EM_GetAnimChild
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_GetAnimChild(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	int				nPos;
	COM_INSTANCE	AnimInst;
	COM_INSTANCE	*pRR;

				
	nPos		= *((int*)(pP));
	AnimInst	= pANS->GetAnimLink()->GetChild( nPos );
	
	pRR		= (COM_INSTANCE*) ( pR );
	*pRR	= AnimInst;
}


//-----------------------------------------------------------------------------
//	Name:		EM_GetChildIndex
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_GetChildIndex(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	COM_INSTANCE		ChildInst;
	int					nPos;
	ANIM_NODE_STRUCT	*pChildStruct;
	int					*pI;

	//	Get the anim
	ChildInst = *((COM_INSTANCE*)(pP));
	//	and its struct
	pChildStruct = ANIM_NODE::GetStructFromInstance( ChildInst );
	// check it is still existing
	nPos = -1;
	if ( pChildStruct )
	{
		//	Get its pos by searching with its child index
		nPos = pANS->GetAnimLink()->GetChildPos( pChildStruct->GetChildIndex() );
	}
	//	Prepare return value
	pI = (int*) (pR );
	*pI = nPos;
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetOffsetStart
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetOffsetStart(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;

	rVal = *((Float32*)pP);
	pANS->SetOffsetStart( rVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_SetManualTimer
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetManualTimer(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->SetManualTimer( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_IncrementTimer
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_IncrementTimer(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;

	rVal = *((Float32*)pP);
	pANS->SetManualIncrement( rVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_IsManualTimer
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_IsManualTimer(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool		bVal;
	bool*		pRR;

	bVal	= pANS->IsManualTimer();
	pRR		= (bool*)( pR );
	*pRR	= bVal;
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetCurTime
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetCurTime(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;

	rVal = *((Float32*)pP);
	pANS->SetCurTime( rVal );
}


//-----------------------------------------------------------------------------
//	Name:		EM_EnableDeletation
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_EnableDeletation(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	bool	bVal;

	bVal = *((bool*)pP);
	pANS->EnableDeletation( bVal );

}


//-----------------------------------------------------------------------------
//	Name:		EM_ActiveCountDown
//	Object:		
//	03-03-27:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_ActiveCountDown(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;

	rVal = *((Float32*)pP);
	pANS->ActiveCountDown( rVal );

}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNbrBlendingFrames
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::EM_SetNbrBlendingFrames(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	ANIM_NODE	*pAN = static_cast<ANIM_NODE*> ( pO );
	ANIM_NODE_STRUCT	*pANS = pAN->GetAnimStruct();

	Float32	rVal;

	rVal = *((Float32*)pP);
	pANS->SetNbrBlendingFrames( rVal );

}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentFrame
//	Object:		
//	03-04-01:	DAC - Created
//-----------------------------------------------------------------------------
Float32	ANIM_NODE::GetCurrentFrame() const
{
	return	(GetAnimStruct()->GetCurrentFrame());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
