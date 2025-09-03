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
//	CLASS:	HIPS_ANIM
//
//	01-10-18:	cvi - Created
//*****************************************************************************

#include	"Root.h" 
#include	"HipsAnim.h"
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(Streaming/CommunicatorPartitionDescriptor)
#include	INCL_3DENGINE(Behavior/HipsAnim/HipsAnimStruct)
#include	INCL_3DENGINE(Behavior/Datas/AnimPC)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamAnimation)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include "HipsAnim.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM constructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM::HIPS_ANIM()
:
ANIM_NODE( GetGenericCommunicatorDescriptor() )
{
	HIPS_ANIM_STRUCT *pS = new HIPS_ANIM_STRUCT();
	pS->SetAnimNode(this);
	pS->Init();
	SetAnimStruct(pS);
}

//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
HIPS_ANIM::HIPS_ANIM(CLASS_DESCRIPTOR * const	pCD)
:
ANIM_NODE( pCD )
{
	HIPS_ANIM_STRUCT *pS = new HIPS_ANIM_STRUCT();
	SetAnimStruct( pS );
	pS->SetAnimNode( this );
	pS->Init();
}


//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM constructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM::HIPS_ANIM(const HIPS_ANIM & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		HIPS_ANIM destructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM::~HIPS_ANIM()
{
	if ( _pStruct )	delete _pStruct;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
HIPS_ANIM &	HIPS_ANIM::operator=(const HIPS_ANIM & C)
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
void	HIPS_ANIM::Copy(const HIPS_ANIM & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR			*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = METHOD_DESCRIPTOR__Allocate("Instanciate", "Instanciate an animation from an ANIM_PC data animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "AnimPc", "AnimPc data animation", ANIM_PC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SKL_INSTANCE, pMD, EM_Instanciate);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetDataAnim", "Get the ANIM_PC data animation");
	METHOD_DESCRIPTOR__SetReturn(pMD,"AnimPc", "AnimPc data animation", ANIM_PC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GET_DATA_ANIM, pMD, EM_GetDataAnim);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("InstanciateStream", "Instanciate an stream animation from an stream data animation");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Stream animation", "Stream anim data animation", STREAM_ANIMATION::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SKL_INSTANCE_STREAM, pMD, EM_InstanciateStream);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	HIPS_ANIM::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_HIPS_TRACK_ANIM) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	HIPS_ANIM::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("HIPS_ANIM"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_HIPS_TRACK_ANIM,
											QDT_COMMENT("638D5423-92DC-35FE-3243-1C7F2498E7B9"),
											sizeof(HIPS_ANIM), 
											COMInit,
											QDT_COMMENT("hiearchy track animation"),
											ANIM_NODE::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	HIPS_ANIM::GetClassGUID() const
{
	return ( _sClassGUID );
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	HIPS_ANIM*	pAN;
	
	pAN = (HIPS_ANIM*) pO;
	
	QDT_NEW_PLACED(pAN, HIPS_ANIM)();
	QDT_ASSERT(pAN);

}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	HIPS_ANIM*	pAN;
	
	pAN = (HIPS_ANIM*) pO;
	
	QDT_DELETE_PLACED(pAN, HIPS_ANIM);
	QDT_ASSERT(pAN);
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//	COMMANDS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//	Name:		EM_Instanciate
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::EM_Instanciate(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	HIPS_ANIM	*pAN = static_cast<HIPS_ANIM*> ( pO );
	HIPS_ANIM_STRUCT	*pANS = static_cast<HIPS_ANIM_STRUCT*> ( pAN->GetAnimStruct() );

	COM_INSTANCE	CI;

	CI = *( (COM_INSTANCE*) ( pP ) );

	if ( CI.HasHandle() == false )	return;
	pANS->ExecInstanciate( CI );
}


//-----------------------------------------------------------------------------
//	Name:		EM_InstanciateStream
//	Object:		
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::EM_InstanciateStream(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	HIPS_ANIM			*pAN = static_cast<HIPS_ANIM*> ( pO );
	HIPS_ANIM_STRUCT	*pANS = static_cast<HIPS_ANIM_STRUCT*> ( pAN->GetAnimStruct() );

	COM_INSTANCE	CI;

	CI = *( (COM_INSTANCE*) ( pP ) );

	if ( CI.HasHandle() == false )	return;
	pANS->ExecInstanciate( CI );

}


//-----------------------------------------------------------------------------
//	Name:		EM_GetDataAnim
//	Object:		
//	03-03-31:	CVI - Created
//-----------------------------------------------------------------------------
void	HIPS_ANIM::EM_GetDataAnim(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	HIPS_ANIM	*pAN = static_cast<HIPS_ANIM*> ( pO );
	HIPS_ANIM_STRUCT	*pANS = static_cast<HIPS_ANIM_STRUCT*> ( pAN->GetAnimStruct() );


	COM_INSTANCE	CI;
	COM_INSTANCE	*pCI;

	CI = pANS->GetAnim();

	pCI = static_cast<COM_INSTANCE*> ( pR );
	QDT_NEW_PLACED( pCI, COM_INSTANCE )();
	*pCI = CI;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
