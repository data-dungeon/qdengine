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
// CLASS: CAM_FREE
//
//
//	00-12-11	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (CAM_FREE)
//
//	The CAM_FREE class implements ...
//
//*****************************************************************************

#include	"Root.h" 
#include	"CamFree.h"

#include	INCL_3DENGINE(Behavior/Behaviors/Node Behaviors/CamFreeStruct)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_3DENGINE(M3D/Moteur3D)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

CAM_FREE *	CAM_FREE::_pActiveCamFree = NULL;

//-----------------------------------------------------------------------------
//	Name:		CAM_FREE constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
CAM_FREE::CAM_FREE()
:
ANIM_NODE( GetGenericCommunicatorDescriptor() )
{
	CAM_FREE_STRUCT	*pS = new CAM_FREE_STRUCT();
	pS->SetAnimNode(this);
	pS->Init();
	SetAnimStruct(pS);
	SetActiveStatus(true);
}

//-----------------------------------------------------------------------------
//	Name:		CAM_FREE constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
CAM_FREE::CAM_FREE(CLASS_DESCRIPTOR * const	pCD)
:
ANIM_NODE( pCD )
{
}


//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *			pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetActive", "Set this camera as the active camera");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bVal", "Status value", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE, pMD, EM_SetActive);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("SetKey", "Set key command");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nEnumKeyMovType", "Move type, see the enums", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pMD, "nKeyCode", "Key code value", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_KEY, pMD, EM_SetKey);
	//
	pMD = METHOD_DESCRIPTOR__Allocate("GetKey", "Get key code for a special move");
	METHOD_DESCRIPTOR__AddParameter(pMD, "nEnumKeyMovType", "Move type, see the enums", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD, "nKeyCode", "Key code value", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GET_KEY, pMD, EM_GetKey);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CAM_FREE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BEHAVIOR_CAM_FREE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CAM_FREE::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("CAM_FREE"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BEHAVIOR_CAM_FREE,
											QDT_COMMENT("637D5423-22DC-38EE-1253-137F2468D7B9"),
											sizeof(CAM_FREE), 
											COMInit,
											QDT_COMMENT("Free camera"),
											ANIM_NODE::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	CAM_FREE::GetClassGUID() const
{
	return ( _sClassGUID );
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::EM_Construct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	CAM_FREE		*pAN;
	
	pAN = (CAM_FREE*) pO;

	QDT_NEW_PLACED(pAN, CAM_FREE)();
	QDT_ASSERT(pAN);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::EM_Destruct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	CAM_FREE*	pAN;
	
	pAN = (CAM_FREE*) pO;
	
	QDT_DELETE_PLACED(pAN, CAM_FREE);
	QDT_ASSERT(pAN);
}

//-----------------------------------------------------------------------------
//	Name: CAM_FREE::BE_CAM_FREE
//	Object: 
//	01-01-24:	RMA - Created
//-----------------------------------------------------------------------------
CAM_FREE::CAM_FREE(const CAM_FREE &	BehCamFree)
{
	Copy(BehCamFree);
}

//-----------------------------------------------------------------------------
// Name: CAM_FREE::~CAM_FREE
// Object: Destructor
//00-12-11 RMA Created: 
//-----------------------------------------------------------------------------
CAM_FREE::~CAM_FREE()
{
	if	(_pActiveCamFree == this)
	{
		_pActiveCamFree = NULL;
	}
	if ( GetAnimStruct() )
	{
		delete _pStruct;
		_pStruct = NULL;
	}
}	

//-----------------------------------------------------------------------------
//	Name: CAM_FREE::operator
//	Object: 
//	01-01-24:	RMA - Created
//-----------------------------------------------------------------------------
CAM_FREE & CAM_FREE::operator=(const CAM_FREE & BehCamFree)
{
	if ( this != &BehCamFree )
	{	
		Copy ( BehCamFree ) ;
	}

	return ( *this ) ;
}


//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	01-01-24:	GHP - Created
//-----------------------------------------------------------------------------
void CAM_FREE::Copy( const CAM_FREE & BehCamFree ) 
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-01-24:	GHP - Created
//-----------------------------------------------------------------------------
bool CAM_FREE::operator==( const CAM_FREE & BehCamFree) 
{
	QDT_NOT_IMPLEMENTED();
	/*if ( *_pStruct == *(BehCamFree._pStruct) )
	{
		return (true );
	}*/
	return ( false );
		
}

//-----------------------------------------------------------------------------
// Name: CAM_FREE::IsActived
// Object: 
//01-01-03 CVI Created: 
//-----------------------------------------------------------------------------
bool	CAM_FREE::IsActived()
{
	return ( _bIsActived );
}


//-----------------------------------------------------------------------------
//	Name:		GetActive
//	Object:		
//	01-08-30:	VPI - Created
//-----------------------------------------------------------------------------
CAM_FREE *	CAM_FREE::GetActive()
{
	return	(_pActiveCamFree);
}

//-----------------------------------------------------------------------------
//	Name:		GetFreeCamAnimFromStruct
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
CAM_FREE *	CAM_FREE::GetFreeCamAnimFromStruct(CAM_FREE_STRUCT *	pStruct)
{
	CAM_FREE	*pCam;

	pCam = static_cast<CAM_FREE*> ( pStruct->GetAnimNode() );
	return ( pCam );
}


//-----------------------------------------------------------------------------
// Name: CAM_FREE_STRUCT::SetActiveStatus
// Object: 
//01-01-03 CVI Created: 
//-----------------------------------------------------------------------------
void	CAM_FREE::SetActiveStatus( bool b )
{

	_bIsActived = b;
	if	(b)
	{
		_pActiveCamFree = this;
	}
	else
	{
		if	(_pActiveCamFree == this)
		{
			_pActiveCamFree = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetEntityInst
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::SetEntityInst(const COM_INSTANCE &	EntityInst)
{
	static_cast<CAM_FREE_STRUCT *>(GetAnimStruct())->SetEntityInst(EntityInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetRoll
//	Object:		
//	03-04-02:	DAC - Created
//-----------------------------------------------------------------------------
float	CAM_FREE::GetRoll() const
{
	CAM_FREE_STRUCT *pStruct;
	
	pStruct = static_cast<CAM_FREE_STRUCT*>(GetAnimStruct());

	return	(pStruct->GetRoll());
}

//-----------------------------------------------------------------------------
//	Name:		SetRoll
//	Object:		
//	03-04-02:	DAC - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::SetRoll(const float	rRoll)
{
	CAM_FREE_STRUCT * pStruct;
	
	pStruct = static_cast<CAM_FREE_STRUCT*>(GetAnimStruct());

	pStruct->SetRoll(rRoll);
}

//-----------------------------------------------------------------------------
//	Name:		ReinitRoll
//	Object:		
//	03-04-02:	DAC - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::ReinitRoll(const float	rRoll)
{
	CAM_FREE_STRUCT * pStruct;
	
	pStruct = static_cast<CAM_FREE_STRUCT*>(GetAnimStruct());

	pStruct->ReinitRoll(rRoll);
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	03-04-02:	DAC - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::Reset()
{
	CAM_FREE_STRUCT * pStruct;
	
	pStruct = static_cast<CAM_FREE_STRUCT*>(GetAnimStruct());
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//	COMMANDS
////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//	Name:		EM_SetActive
//	Object:		
//	03-04-04:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::EM_SetActive(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	CAM_FREE			*pC;
	bool				bVal;

	pC = static_cast<CAM_FREE*> ( pO );
	
	bVal = *( (bool*) pP );
	pC->SetActiveStatus( bVal );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetKey
//	Object:		
//	03-04-04:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::EM_SetKey(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	CAM_FREE			*pC;
	CAM_FREE_STRUCT		*pS;
	

	struct LOCAL_PARAM
	{
		int _i1, _i2;
	};

	LOCAL_PARAM p;

	pC = static_cast<CAM_FREE*> ( pO );
	pS = static_cast<CAM_FREE_STRUCT*> ( pC->GetAnimStruct() );
	
	p = *( (LOCAL_PARAM*) pP );
		
	pS->SetKey( (CAM_FREE_STRUCT::KEYMOV_TYPE)p._i1, p._i2 );
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetKey
//	Object:		
//	03-04-04:	CVI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE::EM_GetKey(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	CAM_FREE			*pC;
	CAM_FREE_STRUCT		*pS;
	int					i1, i2;
	int					*pI;

	pC = static_cast<CAM_FREE*> ( pO );
	pS = static_cast<CAM_FREE_STRUCT*> ( pC->GetAnimStruct() );
	
	i1 = *( (int*) pP );
		
	i2 = pS->GetKey( (CAM_FREE_STRUCT::KEYMOV_TYPE)i1 );

	pI = (int*) pR;
	*pI = i2;

}

////////////////////////////////////////////////////////////////


//=============================================================================
// CODE ENDS HERE
//=============================================================================

