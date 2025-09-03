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
//	CLASS:	SHADER_SHADOW
//
//	06-03-08:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderShadow)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)

// Include vertex and pixel shaders
#include "ShaderShadowVS_Spot.h"
#include "ShaderShadowVS_HardwareSpot.h"
#include "ShaderShadowSkinnedVS_HardwareSpot.h"
#include "ShaderShadowSkinnedVS_Spot.h"
#include "ShaderShadowPS_Spot.h"
#include "ShaderShadowPS_HardwareSpot.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SHADER_SHADOW
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_SHADOW::SHADER_SHADOW()
:
SHADER_ABC(GetGenericCommunicatorDescriptor()),
_ShadowType(ST_UNKNOWN),
_pSkinnedVertexShader(NULL),
_pVertexShader(NULL),
_pPixelShader(NULL)
{
	_pSkinnedVertexShader_Spot			= QDT_CREATE_VERTEXSHADER(ShaderShadowSkinnedVS_Spot);
	_pSkinnedVertexShader_HardwareSpot	= QDT_CREATE_VERTEXSHADER(ShaderShadowSkinnedVS_HardwareSpot);
	_pVertexShader_Spot					= QDT_CREATE_VERTEXSHADER(ShaderShadowVS_Spot);
	_pVertexShader_HardwareSpot			= QDT_CREATE_VERTEXSHADER(ShaderShadowVS_HardwareSpot);
	_pPixelShader_Spot					= QDT_CREATE_PIXELSHADER(ShaderShadowPS_Spot);
	_pPixelShader_HardwareSpot			= QDT_CREATE_PIXELSHADER(ShaderShadowPS_HardwareSpot);

	_pSkinnedVertexShader = NULL;
	_pVertexShader = NULL;
	_pPixelShader = NULL;

	// RMA: Bizarre mais je dois mettre le double sided à true pour le moment
	// afin d'éviter de graves problemes sur les objets avec une simple couche
	_bDoubleSided = true;
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_SHADOW
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_SHADOW::~SHADER_SHADOW()
{
	RENDER_CONTEXT::ReleaseVertexShader(_pSkinnedVertexShader_Spot);
	RENDER_CONTEXT::ReleaseVertexShader(_pSkinnedVertexShader_HardwareSpot);
	RENDER_CONTEXT::ReleaseVertexShader(_pVertexShader_Spot);
	RENDER_CONTEXT::ReleaseVertexShader(_pVertexShader_HardwareSpot);
	RENDER_CONTEXT::ReleasePixelShader(_pPixelShader_Spot);
	RENDER_CONTEXT::ReleasePixelShader(_pPixelShader_HardwareSpot);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SHADER_SHADOW::GetNbrPasses()
{
	return (1);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_SHADOW::SetPass(UInt32 uiPass, const VECTOR & Pos)
{
	QDT_ASSERT(_pVertexShader);
	QDT_ASSERT(_pSkinnedVertexShader);
	QDT_ASSERT(_pPixelShader);

	if (_ShadowType == ST_HARDWARE_SPOT)
	{	
		// Hardware shadow map doesn't use a render target, only a z-buffer
		// Disabling color write allow us to have double speed z-stencil only rendering with high-end GPU!
		RENDER_CONTEXT::EnableColorWrite(false, false, false, false);
	}

	_uiCurrentVertexShaderRegister = 0;

	PrepareBlending(uiPass);

	RENDER_CONTEXT::SetPixelShader(_pPixelShader);

	if	(DISPLAY_MANAGER::Instance()->IsSkinningEnabled())
	{
		RENDER_CONTEXT::SetVertexShader(_pSkinnedVertexShader);

		if (uiPass == 0)
		{
			SetModelViewProj();
			SetSkinningMatrices();
		}
	}
	else
	{
		RENDER_CONTEXT::SetVertexShader(_pVertexShader);

		if (uiPass == 0)
		{
			SetModelViewProj();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetModelViewProj
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_SHADOW::SetModelViewProj()
{
	const MATRIX & WorldViewProj = DISPLAY_MANAGER::Instance()->GetWorldViewProjMatrix();
	RENDER_CONTEXT::SetVertexShaderConstant(_uiCurrentVertexShaderRegister, WorldViewProj);
	_uiCurrentVertexShaderRegister += 4;
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinningMatrices
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_SHADOW::SetSkinningMatrices()
{
	DISPLAY_MANAGER *	pDisplay = DISPLAY_MANAGER::Instance();

	UInt32	i, j, Index = 0, nNbrMatrices = pDisplay->GetNbrSkinningMatrices();
	Float32	rValues[12 * QD_MAX_SKINNING_MATRIX];

	for	(i = 0 ; i < nNbrMatrices ; ++i)
	{
		const MATRIX & SkinningMatrix = pDisplay->GetSkinningMatrix(i);

		for	(j = 0 ; j < 3 ; ++j)
		{
			rValues[Index++] = SkinningMatrix(0, j);
			rValues[Index++] = SkinningMatrix(1, j);
			rValues[Index++] = SkinningMatrix(2, j);
			rValues[Index++] = SkinningMatrix(3, j);
		}
	}

	RENDER_CONTEXT::SetVertexShaderArrayConstant(_uiCurrentVertexShaderRegister, 12 * nNbrMatrices, &rValues[0]);
	_uiCurrentVertexShaderRegister += 60;
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_SHADOW::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_SHADOW));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_SHADOW::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("SHADER_SHADOW"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_SHADOW,
										QDT_COMMENT("43044667-6DFA-F666-8BE3-26908EE51CD1"), 
										sizeof(SHADER_SHADOW), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_SHADOW::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_SHADOW::EM_Construct(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	SHADER_SHADOW	*pShader;
	pShader = static_cast<SHADER_SHADOW *>(pO);

	QDT_NEW_PLACED(pShader, SHADER_SHADOW)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_SHADOW::EM_Destruct(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	SHADER_SHADOW	*pShader;
	pShader = static_cast<SHADER_SHADOW *>(pO);

	QDT_DELETE_PLACED(pShader, SHADER_SHADOW);
}

//-----------------------------------------------------------------------------
//	Name:		SetShadowType
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_SHADOW::SetShadowType(SHADOW_TYPE Type)
{
	_ShadowType = Type;

	switch (Type)
	{
	case ST_UNKNOWN:
		_pSkinnedVertexShader = NULL;
		_pVertexShader = NULL;
		_pPixelShader = NULL;
		QDT_FAIL();
		break;

	case ST_SPOT:
		_pSkinnedVertexShader = _pSkinnedVertexShader_Spot;
		_pVertexShader = _pVertexShader_Spot;
		_pPixelShader = _pPixelShader_Spot;
		break;

	case ST_HARDWARE_SPOT:
		_pSkinnedVertexShader = _pSkinnedVertexShader_HardwareSpot;
		_pVertexShader = _pVertexShader_HardwareSpot;
		_pPixelShader = _pPixelShader_HardwareSpot;
		break;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
