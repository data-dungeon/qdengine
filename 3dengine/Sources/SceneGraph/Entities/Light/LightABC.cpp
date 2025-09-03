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
//	CLASS:	LIGHT_ABC
//
//	01-07-03:	RMA - Created
//	01-09-03:	ONA - Updated categories includes
//	05-11-21:	RMA - Completly redone
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Light/LightABC)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(ComBasis/ComStack)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		LIGHT_ABC constructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_ABC::LIGHT_ABC(COMMUNICATOR_DESCRIPTOR *	pCD,
					 ENTITY_TYPE				eEntityType)
:
ENTITY_ABC(pCD, eEntityType),
_bCastShadow(false),
_bSwitch(true)
{
	_LightSource._eType = LIGHT_UNDEFINED;
	_LightSource._pShadowAttributes = NULL;
	_LightSource._rBias = 0.0000005f;

	_LightSource._nShadowBlurMode = SBM_NONE;
	_LightSource._rShadowBlurRadius = 0.f;
}

//-----------------------------------------------------------------------------
//	Name:		~LIGHT_ABC destructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_ABC::~LIGHT_ABC()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetSwitch
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	LIGHT_ABC::GetSwitch() const
{
	return (_bSwitch);
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::Register()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->AddLight(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::Unregister()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->RemoveLight(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-05-29:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::Display()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Switch
//	Object:		b = true  -> ON
//				b = false -> OFF
//	02-10-07:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::Switch(bool	b)
{
	_bSwitch = b;
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	METHOD_DESCRIPTOR*	pMD;
	pMD	=METHOD_DESCRIPTOR::Allocate("SetShadowBlurMode",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("nMode"),				QDT_COMMENT(""),	TYPE_INT::GetDescriptor());
	pCD->AddCommand(	CMD_LIGHTABC_SETSHADOWBLURMODE, pMD, CM_SetShadowBlurMode);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetShadowBlurRadius",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rRadius"),				QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_LIGHTABC_SETSHADOWBLURRADIUS, pMD, CM_SetShadowBlurRadius);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetShadowColor",	QDT_COMMENT("Set shadow color"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rRed"),		QDT_COMMENT("Red component"),	TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(	QDT_COMMENT("rGreen"),		QDT_COMMENT("Green component"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(	QDT_COMMENT("rBlue"),		QDT_COMMENT("Blue component"),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_LIGHTABC_SETSHADOWCOLOR, pMD, CM_SetShadowColor);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetBias",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rBias"),	QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_LIGHTABC_SETBIAS, pMD, CM_SetBias);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetCastShadow",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("bEnable"),			QDT_COMMENT(""),	TYPE_BOOL::GetDescriptor());
	pCD->AddCommand(	CMD_LIGHTABC_SETCASTSHADOW, pMD, CM_SetCastShadow);

}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	LIGHT_ABC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIGHT_ABC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	LIGHT_ABC::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("LIGHT_ABC"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIGHT_ABC,
											QDT_COMMENT("A16CA5E4-9118-55A8-2575-4D9A3E879FAD"),
											sizeof(LIGHT_ABC),
											ComInit,
											QDT_COMMENT("Abstract class for lights"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::Copy(const LIGHT_ABC &	C)
{
	ENTITY_ABC::Copy(C);

	_LightSource = C._LightSource;
	_bSwitch = C._bSwitch;
}

//-----------------------------------------------------------------------------
//	Name:		CastShadow
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::CastShadow(Bool	b)
{
	if (_bCastShadow != b)
	{
		UInt8 nSwitchShadowValue = 3;
		UInt32 nType = (UInt32) _LightSource._eType;

		QDT_ASSERT(nType != LIGHT_UNDEFINED);		

		if (DISPLAY_MANAGER::Instance()->SupportHardwareShadowMap())
		{
			nSwitchShadowValue *= 2;
			//nSwitchShadowValue += _LightSource._nShadowBlurMode;
		}

		_bCastShadow = b;

		if (nType >= LIGHT_POINT_SHADOW)
		{
			if (_bCastShadow == false)
			{
				nType -= nSwitchShadowValue;

				QDT_ASSERT(_LightSource._pShadowAttributes != NULL);
				delete _LightSource._pShadowAttributes;				
				_LightSource._pShadowAttributes = NULL;
			}
		}
		else
		{
			if (_bCastShadow == true)
			{
				nType += nSwitchShadowValue;

				QDT_ASSERT(_LightSource._pShadowAttributes == NULL);
				_LightSource._pShadowAttributes = new S_SHADOW_ATTRIBUTES;
				memset(_LightSource._pShadowAttributes, 0, sizeof(S_SHADOW_ATTRIBUTES));	
			}
		}	

		_LightSource._eType = (LIGHT_TYPE) nType;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CleanShadowRenderTargets
//	Object:		
//	06-02-13:	SBE - Created
//-----------------------------------------------------------------------------
void	LIGHT_ABC::CleanShadowRenderTargets()
{
	QDT_ASSERT(_LightSource._pShadowAttributes);
	QDT_ASSERT(_LightSource._pShadowAttributes->_pAssociatedDepthRenderTargets);

	_LightSource._pShadowAttributes->_pAssociatedDepthRenderTargets->pLight = NULL;
	_LightSource._pShadowAttributes->_pAssociatedDepthRenderTargets = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		GetLightedMeshes
//	Object:		
//	06-02-14:	SBE - Created
//-----------------------------------------------------------------------------
QDT_DLIST < MESH * > &	LIGHT_ABC::GetLightedMeshes()
{
	QDT_ASSERT(_LightSource._pShadowAttributes != NULL);

	return _LightSource._pShadowAttributes->_lLightedMeshes; 
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetShadowBlurMode
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int LIGHT_ABC::CM_SetShadowBlurMode(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_LIGHTABC_SETSHADOWBLURMODE);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	LIGHT_ABC* pThis =static_cast<LIGHT_ABC*>(CI.GetCommunicator());

	Int32 nMode;
	Stack.GetParameter(0, nMode);

	pThis->SetShadowBlurMode((SHADOW_BLUR_MODE)nMode);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetShadowBlurRadius
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int LIGHT_ABC::CM_SetShadowBlurRadius(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_LIGHTABC_SETSHADOWBLURRADIUS);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	LIGHT_ABC* pThis =static_cast<LIGHT_ABC*>(CI.GetCommunicator());

	Float32 rRadius;
	Stack.GetParameter(0, rRadius);

	pThis->SetShadowBlurRadius(rRadius);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetShadowColor
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int LIGHT_ABC::CM_SetShadowColor(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_LIGHTABC_SETSHADOWCOLOR);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	LIGHT_ABC* pThis =static_cast<LIGHT_ABC*>(CI.GetCommunicator());

	Float32	rRed, rGreen, rBlue;
	Stack.GetParameter(0, rRed);
	Stack.GetParameter(1, rGreen);
	Stack.GetParameter(2, rBlue);

	pThis->SetShadowColor(rRed, rGreen, rBlue);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetBias
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int LIGHT_ABC::CM_SetBias(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_LIGHTABC_SETBIAS);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	LIGHT_ABC* pThis =static_cast<LIGHT_ABC*>(CI.GetCommunicator());

	Float32 rBias;
	Stack.GetParameter(0, rBias);

	pThis->SetBias(rBias);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetCastShadow
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int LIGHT_ABC::CM_SetCastShadow(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_LIGHTABC_SETCASTSHADOW);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	LIGHT_ABC* pThis =static_cast<LIGHT_ABC*>(CI.GetCommunicator());

	Bool bEnable;
	Stack.GetParameter(0, bEnable);

	pThis->CastShadow(bEnable);

	return (COM_STACK::NO_RET_VALUE);
}		

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
