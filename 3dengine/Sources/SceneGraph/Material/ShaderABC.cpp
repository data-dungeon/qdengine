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
//	CLASS:	SHADER_ABC
//
//	05-11-09:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/SortedObject/SortedObject)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(ComBasis/ComStack)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SHADER_ABC Constructor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_ABC::SHADER_ABC(COMMUNICATOR_DESCRIPTOR *pCD)
:
_eBlending(SB_DECAL),
_bDoubleSided(false),
_bDoubleRendering(false),
_bEnableAlphaTest(false),
_rAlphaTestValue(1.0f),
_uiTransparencyPriority(127),
COMMUNICATOR(pCD)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_ABC Destructor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_ABC::~SHADER_ABC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_ABC::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_ABC));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_ABC::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("SHADER_ABC"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_ABC,
										QDT_COMMENT("43044444-6DFA-F943-8BE3-26908EE51CD1"), 
										sizeof(SHADER_ABC), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		SetBlending
//	Object:		This method set the blending equation according to the
//				SHADER_BLENDING attribute of the class
//				Notice that the blending equation is different for the second 
//				pass
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::PrepareBlending(UInt32 uiPass)
{
	if (uiPass == 0)
	{
		switch (_eBlending)
		{
		default:
			QDT_FAIL();

		case SB_DECAL:
			RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
			break;

		case SB_ALPHA:
			RENDER_CONTEXT::SetBlendMode(BM_SRCALPHA, BOP_ADD, BM_INVSRCALPHA);
			break;

		case SB_INVALPHA:
			RENDER_CONTEXT::SetBlendMode(BM_INVSRCALPHA, BOP_ADD, BM_SRCALPHA);
			break;

		case SB_ADDITIVE:
			RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ONE);
			break;

		case SB_SUBTRACTIVE:
			RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_REVSUBTRACT, BM_ONE);
			break;
		}
		
	}
	else
	{
		switch (_eBlending)
		{
		default:
			QDT_FAIL();

		case SB_DECAL:
			RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ONE);
			break;

		case SB_ALPHA:
			RENDER_CONTEXT::SetBlendMode(BM_SRCALPHA, BOP_ADD, BM_ONE);
			break;

		case SB_INVALPHA:
			RENDER_CONTEXT::SetBlendMode(BM_INVSRCALPHA, BOP_ADD, BM_ONE);
			break;

		case SB_ADDITIVE:
			RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ONE);
			break;

		case SB_SUBTRACTIVE:
			RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_REVSUBTRACT, BM_ONE);
			break;
		}
	}

	RENDER_CONTEXT::EnableAlphaTest(_bEnableAlphaTest);
	RENDER_CONTEXT::SetAlphaTestFunc(CF_GREATER);
	RENDER_CONTEXT::SetAlphaTestRef((UInt8)(_rAlphaTestValue * 255.0f));
}

//-----------------------------------------------------------------------------
//	Name:		Restore
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_ABC::Restore()
{
	RENDER_CONTEXT::EnableAlphaTest(false);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_ABC::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	METHOD_DESCRIPTOR*	pM;

	pM = METHOD_DESCRIPTOR::Allocate("SetBlendingMode", "", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "eBlendingMode", "", TYPE_INT::GetDescriptor());
	pCD->AddCommand(CMD_SET_BLENDING_MODE, pM, CM_SetBlendingMode);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_ABC::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_ABC::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		SetBlending
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::SetBlending(SHADER_BLENDING eBlend)
{
	_eBlending = eBlend;
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleSided
//	Object:		
//	06-03-13:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::SetDoubleSided(Bool b)
{
	_bDoubleSided = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleRendering
//	Object:		
//	06-03-13:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::SetDoubleRendering(Bool b)
{
	_bDoubleRendering = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetSupportShadowing
//	Object:		
//	06-03-28:	SBE - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::SetSupportShadowing(Bool b)
{
	_bSupportShadowing = b;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetAlphaTest
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::SetAlphaTest(Bool bAlphaTest)
{
	_bEnableAlphaTest = bAlphaTest;
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestValue
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ABC::SetAlphaTestValue(Float32 rAlphaTestValue)
{
	_rAlphaTestValue = rAlphaTestValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetObjectsToDisplay
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < SORTED_OBJECT > &	SHADER_ABC::GetObjectsToDisplay()
{
	return	(_vDisplayedObjects);
}

//-----------------------------------------------------------------------------
//	Name:		IsTransparent
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
bool	SHADER_ABC::IsTransparent() const
{
	return	(_eBlending != SB_DECAL);
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleSided
//	Object:		
//	06-03-13:	RMA - Created
//-----------------------------------------------------------------------------
Bool	SHADER_ABC::GetDoubleSided() const
{
	return (_bDoubleSided);
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleSided
//	Object:		
//	06-03-13:	RMA - Created
//-----------------------------------------------------------------------------
Bool	SHADER_ABC::GetDoubleRendering() const
{
	return (_bDoubleRendering);
}

//-----------------------------------------------------------------------------
//	Name:		NeedSeparateDepthPass
//	Object:		This is used to determine if we need an additional depth pass
//				for transparent objects with depth of field
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
Bool	SHADER_ABC::NeedSeparateDepthPass() const
{
	if (IsTransparent() || _bEnableAlphaTest)
	{
		CAMERA * pCam = DISPLAY_MANAGER::Instance()->GetActiveCamera();
		if (pCam->GetEnableDepthOfField())
		{
			return (true);
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetBlendingMode
//	Object:		
//	06-03-30:	NHO - Created
//-----------------------------------------------------------------------------
int	SHADER_ABC::CM_SetBlendingMode(COM_PARAM	P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_BLENDING_MODE);;
	COM_INSTANCE	ThisCI;

	if (Stack.GetThis(ThisCI))
	{
		SHADER_ABC* pThis = static_cast<SHADER_ABC*>(ThisCI.GetCommunicator());
		Int32 eBlendingMode;

		Stack.GetParameter(0, eBlendingMode);
		pThis->SetBlending((SHADER_BLENDING)eBlendingMode);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		GetTransparencyPriority
//	Object:		
//	06-03-30:	RMA - Created
//-----------------------------------------------------------------------------
UInt8	SHADER_ABC::GetTransparencyPriority() const
{
	return (_uiTransparencyPriority);
}

//-----------------------------------------------------------------------------
//	Name:		SetTransparencyPriority
//	Object:		
//	06-03-30:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_ABC::SetTransparencyPriority(UInt8 uiPriority)
{
	_uiTransparencyPriority = uiPriority;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
