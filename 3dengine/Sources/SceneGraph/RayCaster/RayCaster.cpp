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
//	CLASS:	RAY_CASTER
//
//	01-07-26:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/RayCaster/RayCaster)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(ComplexType/TypeVector)

#ifdef _DEBUG
	#include "RayCaster.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RAY_CASTER constructor
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CASTER::RAY_CASTER()
:
_pRayCastParameters(new	RAY_CAST_PARAMETERS)
{
	//	Do nothing...
}

//-----------------------------------------------------------------------------
//	Name:		~RAY_CASTER destructor
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CASTER::~RAY_CASTER()
{
	delete	_pRayCastParameters;
}

//-----------------------------------------------------------------------------
//	Name:		Cast
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::CastRayFromScreen(int	nScreenX,
									  int	nScreenY,
									  bool	bCulling)
{
	_pRayCastParameters->Reset();

	if (bCulling)
	{
		_pRayCastParameters->SetFlags(RAY_CAST_PARAMETERS::CULLING, RAY_CAST_PARAMETERS::CULLING);
	}

	DISPLAY_MANAGER::Instance()->RayCast(nScreenX, nScreenY, *_pRayCastParameters);
}

//-----------------------------------------------------------------------------
//	Name:		CastRayFromViewport
//	Object:		
//	01-08-02:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::CastRayFromViewport(int						nScreenX,
										int						nScreenY,
										const COM_INSTANCE &	Viewport,
										bool					bCulling)
{
	_pRayCastParameters->Reset();

	if (bCulling)
	{
		_pRayCastParameters->SetFlags(RAY_CAST_PARAMETERS::CULLING, RAY_CAST_PARAMETERS::CULLING);
	}

	DISPLAY_MANAGER::Instance()->RayCastFromViewport(nScreenX, nScreenY, *_pRayCastParameters, Viewport);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-02:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	RAY_CASTER *	pObject;
	pObject = static_cast<RAY_CASTER *>(pO);
	QDT_NEW_PLACED(pObject, RAY_CASTER)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-02:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	RAY_CASTER *	pObject;
	pObject = static_cast<RAY_CASTER*> (pO);
	pObject->RAY_CASTER::~RAY_CASTER();
}

//-----------------------------------------------------------------------------
//	Name:		EM_CastRayFromScreen
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_CastRayFromScreen(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	struct	PARAMETERS
	{
		int		_nScreenX;
		int		_nScreenY;
		bool	_bCulling;
	};

	RAY_CASTER *	pObject;
	QDT_ASSERT(pR == NULL);
	PARAMETERS *	pParameters;

	pObject = (RAY_CASTER *)pO;
	pParameters = (PARAMETERS *)pP;

	pObject->CastRayFromScreen(pParameters->_nScreenX, pParameters->_nScreenY, pParameters->_bCulling);
}

//-----------------------------------------------------------------------------
//	Name:		EM_HasHitSomething
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_HasHitSomething(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	RAY_CASTER *	pObject;
	bool *			pReturn;
	QDT_ASSERT(pP == NULL);

	pObject = (RAY_CASTER *)pO;
	pReturn = (bool *)pR;

	*pReturn = pObject->HasHitSomething();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetImpactPoint
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_GetImpactPoint(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	RAY_CASTER *	pObject;
	VECTOR *		pReturn;
	QDT_ASSERT(pP == NULL);

	pObject = (RAY_CASTER *)pO;
	pReturn = (VECTOR *)pR;

	if	(pObject->HasHitSomething())
	{
		QDT_NEW_PLACED(pReturn, VECTOR)(pObject->GetImpactPoint());
	}
	else
	{
		QDT_NEW_PLACED(pReturn, VECTOR)(0.0f, 0.0f, 0.0f);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetImpactNode
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_GetImpactNode(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	RAY_CASTER *	pObject;
	COM_INSTANCE *	pReturn;
	QDT_ASSERT(pP == NULL);

	pObject = (RAY_CASTER *)pO;
	pReturn = (COM_INSTANCE *)pR;

	QDT_NEW_PLACED(pReturn, COM_INSTANCE)(pObject->GetImpactNodeInst());
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetImpactDistance
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_GetImpactDistance(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	RAY_CASTER *	pObject;
	float *			pReturn;
	QDT_ASSERT(pP == NULL);

	pObject = (RAY_CASTER *)pO;
	pReturn = (float *)pR;

	*pReturn = pObject->GetImpactDistance();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetImpactNormal
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_GetImpactNormal(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	RAY_CASTER *	pObject;
	VECTOR *		pReturn;
	QDT_ASSERT(pP == NULL);

	pObject = (RAY_CASTER *)pO;
	pReturn = (VECTOR *)pR;

	QDT_NEW_PLACED(pReturn, VECTOR)(pObject->GetImpactNormal());
}

//-----------------------------------------------------------------------------
//	Name:		EM_CastRayFromViewport
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::EM_CastRayFromViewport(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	struct	PARAMETERS
	{
		PARAMETERS()
		:
		_Viewport(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
		{
			// Do nothing
		}

		int				_nScreenX;
		int				_nScreenY;
		COM_INSTANCE	_Viewport;
		bool			_bCulling;
	};

	RAY_CASTER *	pObject		= static_cast<RAY_CASTER *> (pO);
	PARAMETERS *	pParameters = static_cast<PARAMETERS *> (pP);

	pObject->CastRayFromViewport(pParameters->_nScreenX, pParameters->_nScreenY, pParameters->_Viewport, pParameters->_bCulling);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CASTER::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pM = METHOD_DESCRIPTOR__Allocate("CastRayFromScreen", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__AddParameter(pM, "nScreenX", "Ask 3D Engine to document this parameter", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "nScreenY", "Ask 3D Engine to document this parameter", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "bCulling", "Ask 3D Engine to document this parameter", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_RC_CAST_RAY_FROM_SCREEN, pM, EM_CastRayFromScreen);

	pM = METHOD_DESCRIPTOR__Allocate("HasHitSomething", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__SetReturn(pM, "bHit", "Ask 3D Engine to document this parameter", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_RC_HAS_HIT_SOMETHING, pM, EM_HasHitSomething);

	pM = METHOD_DESCRIPTOR__Allocate("GetImpactPoint", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__SetReturn(pM, "Point", "Ask 3D Engine to document this parameter", TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_RC_GET_IMPACT_POINT, pM, EM_GetImpactPoint);

	pM = METHOD_DESCRIPTOR__Allocate("GetImpactNode", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__SetReturn(pM, "Node", "Ask 3D Engine to document this parameter", NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_RC_GET_IMPACT_NODE, pM, EM_GetImpactNode);

	pM = METHOD_DESCRIPTOR__Allocate("GetImpactDistance", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__SetReturn(pM, "rDistance", "Ask 3D Engine to document this parameter", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_RC_GET_IMPACT_DISTANCE, pM, EM_GetImpactDistance);

	pM = METHOD_DESCRIPTOR__Allocate("GetImpactNormal", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__SetReturn(pM, "Normal", "Ask 3D Engine to document this parameter", TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_RC_GET_IMPACT_NORMAL, pM, EM_GetImpactNormal);

	pM = METHOD_DESCRIPTOR__Allocate("CastRayFromViewport", "Ask 3D Engine to document this command");
	METHOD_DESCRIPTOR__AddParameter(pM, "nScreenX", "Ask 3D Engine to document this parameter", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "nScreenY", "Ask 3D Engine to document this parameter", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Viewport", "Ask 3D Engine to document this parameter", VIEWPORT::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "bCulling", "Ask 3D Engine to document this parameter", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_RC_CAST_RAY_FROM_VIEWPORT, pM, EM_CastRayFromViewport);
}

//-----------------------------------------------------------------------------
//	Name:		GetClassDescriptor
//	Object:		
//	02-07-08:	ONA - Created
//-----------------------------------------------------------------------------
CLASS_DESCRIPTOR *	RAY_CASTER::GetClassDescriptor()
{
	return	(CLASS_DESCRIPTOR::FindClassDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RAY_CASTER));
}

//-----------------------------------------------------------------------------
//	Name:		CreateClassDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
CLASS_DESCRIPTOR *	RAY_CASTER::CreateClassDescriptor()
{
	return ( new CLASS_DESCRIPTOR(	QDT_COMMENT("m3d_ray_caster"),
									MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RAY_CASTER,
									QDT_COMMENT("DE54636C-FBC1-2BEE-5FE2-66939ADC94EC"),
									sizeof(RAY_CASTER), 
									Init,
									QDT_COMMENT("Ask 3D Engine to document this class.")));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
