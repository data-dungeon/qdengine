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
//	CLASS:	TARGET_ANIM
//
//	01-12-18:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"TargetAnim.h"
#include	INCL_3DENGINE(Behavior/TargetAnim/TargetAnimStruct)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include "TargetAnim.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TARGET_ANIM constructor
//	Object:		
//	01-12-18:	cvi - Created
//-----------------------------------------------------------------------------
TARGET_ANIM::TARGET_ANIM()
:
ANIM_NODE( GetGenericCommunicatorDescriptor() )
{
	TARGET_ANIM_STRUCT	*pS = new TARGET_ANIM_STRUCT();
	SetAnimStruct( pS );
	pS->SetAnimNode( this );
	pS->Init();
}

//-----------------------------------------------------------------------------
//	Name:		TARGET_ANIM constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
TARGET_ANIM::TARGET_ANIM(CLASS_DESCRIPTOR * const	pCD)
:
ANIM_NODE( pCD )
{
	// Do nothing
}


//-----------------------------------------------------------------------------
//	Name:		TARGET_ANIM constructor
//	Object:		
//	01-12-18:	cvi - Created
//-----------------------------------------------------------------------------
TARGET_ANIM::TARGET_ANIM(const TARGET_ANIM & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-18:	cvi - Created
//-----------------------------------------------------------------------------
TARGET_ANIM &	TARGET_ANIM::operator=(const TARGET_ANIM & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		~TARGET_ANIM destructor
//	Object:		
//	03-03-31:	ELE - Created
//-----------------------------------------------------------------------------
TARGET_ANIM::~TARGET_ANIM()
{
	if ( GetAnimStruct() )
	{
		delete _pStruct;
		_pStruct = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-12-18:	cvi - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM::Copy(const TARGET_ANIM & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
/*	METHOD_DESCRIPTOR *			pMD;
	QDT_ASSERT(pCD == &_CD);*/

	pCD->SetConstructors(EM_Construct, EM_Destruct);

/*
	pMD = METHOD_DESCRIPTOR__Allocate("Test", "Test");
	pCD->AddMethod(CMD_TEST, pMD, EM_Test);
*/
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TARGET_ANIM::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TARGET_ANIM) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TARGET_ANIM::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("TARGET_ANIM"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TARGET_ANIM,
											QDT_COMMENT("637D5423-C2FC-15DE-3273-1A7F24A8D7B9"),
											sizeof(TARGET_ANIM), 
											COMInit,
											QDT_COMMENT("Target Animation"),
											ANIM_NODE::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	TARGET_ANIM::GetClassGUID() const
{
	return ( _sClassGUID );
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	TARGET_ANIM*	pAN;
	
	pAN = (TARGET_ANIM*) pO;
	
	QDT_NEW_PLACED(pAN, TARGET_ANIM)();
	QDT_ASSERT(pAN);
	
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	TARGET_ANIM*	pAN;
	
	pAN = (TARGET_ANIM*) pO;
	
	QDT_DELETE_PLACED(pAN, TARGET_ANIM);
	QDT_ASSERT(pAN);
}


