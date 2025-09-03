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
//	CLASS:	SHADER_DEFAULT
//
//	05-11-09:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderDefault)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)

// Include vertex and pixel shaders
#include "DefaultVertexShader.h"
#include "DefaultSkinnedVertexShader.h"
#include "DefaultPixelShader.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SHADER_DEFAULT
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_DEFAULT::SHADER_DEFAULT()
:
SHADER_ABC(GetGenericCommunicatorDescriptor()),
_pSkinnedVertexShader(NULL),
_pVertexShader(NULL),
_pPixelShader(NULL)
{
	_pVertexShader = QDT_CREATE_VERTEXSHADER(DefaultVertexShader);
	_pSkinnedVertexShader = QDT_CREATE_VERTEXSHADER(DefaultSkinnedVertexShader);
	_pPixelShader = QDT_CREATE_PIXELSHADER(DefaultPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_DEFAULT
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_DEFAULT::~SHADER_DEFAULT()
{
	RENDER_CONTEXT::ReleaseVertexShader(_pSkinnedVertexShader);
	RENDER_CONTEXT::ReleaseVertexShader(_pVertexShader);
	RENDER_CONTEXT::ReleasePixelShader(_pPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SHADER_DEFAULT::GetNbrPasses()
{
	return (1);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_DEFAULT::SetPass(UInt32 uiPass, const VECTOR & Pos)
{
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

	SetLight();
}

//-----------------------------------------------------------------------------
//	Name:		SetModelViewProj
//	Object:		
//	05-11-24:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_DEFAULT::SetModelViewProj()
{
	const MATRIX & WorldViewProj = DISPLAY_MANAGER::Instance()->GetWorldViewProjMatrix();
	RENDER_CONTEXT::SetVertexShaderConstant(_uiCurrentVertexShaderRegister, WorldViewProj);
	_uiCurrentVertexShaderRegister += 4;
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinningMatrices
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SHADER_DEFAULT::SetSkinningMatrices()
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
//	Name:		SetLight
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SHADER_DEFAULT::SetLight()
{
	const MATRIX & invWorldMatrix = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();
	const MATRIX & cameraMatrix = DISPLAY_MANAGER::Instance()->GetCameraNode()->GetGlobalMatrix();

	VECTOR vEyeDir(VECTOR::UNIT_Z);
	cameraMatrix.TransformDirection(vEyeDir);
	invWorldMatrix.TransformDirection(vEyeDir);

	vEyeDir.Normalize();
	
	RENDER_CONTEXT::SetVertexShaderConstant(_uiCurrentVertexShaderRegister, vEyeDir);
	_uiCurrentVertexShaderRegister++;
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_DEFAULT::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_DEFAULT));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_DEFAULT::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("SHADER_DEFAULT"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_DEFAULT,
										QDT_COMMENT("43044667-6DFA-F943-8BE3-26908EE51CD1"), 
										sizeof(SHADER_DEFAULT), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_DEFAULT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_DEFAULT::EM_Construct(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	SHADER_DEFAULT	*pShader;
	pShader = static_cast<SHADER_DEFAULT *>(pO);

	QDT_NEW_PLACED(pShader, SHADER_DEFAULT)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_DEFAULT::EM_Destruct(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	SHADER_DEFAULT	*pShader;
	pShader = static_cast<SHADER_DEFAULT *>(pO);

	QDT_DELETE_PLACED(pShader, SHADER_DEFAULT);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
