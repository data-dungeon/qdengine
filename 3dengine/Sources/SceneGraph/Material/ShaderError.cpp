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
//	CLASS:	SHADER_ERROR
//
//	05-11-09:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderError)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)

// Include vertex and pixel shaders
#include "ErrorVertexShader.h"
#include "ErrorSkinnedVertexShader.h"
#include "ErrorPixelShader.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SHADER_ERROR
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_ERROR::SHADER_ERROR()
:
SHADER_ABC(GetGenericCommunicatorDescriptor()),
_pSkinnedVertexShader(NULL),
_pVertexShader(NULL),
_pPixelShader(NULL)
{
	_pVertexShader = QDT_CREATE_VERTEXSHADER(ErrorVertexShader);
	_pSkinnedVertexShader = QDT_CREATE_VERTEXSHADER(ErrorSkinnedVertexShader);
	_pPixelShader = QDT_CREATE_PIXELSHADER(ErrorPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_ERROR
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_ERROR::~SHADER_ERROR()
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
UInt32 SHADER_ERROR::GetNbrPasses()
{
	return (1);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ERROR::SetPass(UInt32 uiPass, const VECTOR & Pos)
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
}

//-----------------------------------------------------------------------------
//	Name:		SetModelViewProj
//	Object:		
//	05-11-24:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ERROR::SetModelViewProj()
{
	const MATRIX & WorldViewProj = DISPLAY_MANAGER::Instance()->GetWorldViewProjMatrix();
	RENDER_CONTEXT::SetVertexShaderConstant(_uiCurrentVertexShaderRegister, WorldViewProj);
	_uiCurrentVertexShaderRegister += 4;
}

//-----------------------------------------------------------------------------
//	Name:		SetFocalPlane
//	Object:		
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_ERROR::SetFocalPlane()
{
	DISPLAY_MANAGER * pDisplay = DISPLAY_MANAGER::Instance();

	CAMERA * pCam = pDisplay->GetActiveCamera();
	Float32 fFarDepthOfField = pCam->GetFarDepthOfField();

	MATRIX mViewToWorld = pCam->GetNode()->GetGlobalMatrix();
	MATRIX mWorldToObject = pDisplay->GetInvWorldMatrix();

	MATRIX mViewToObject;
	mViewToObject.Mul(mViewToWorld, mWorldToObject);

	PLANE CameraPlane(0.0f, 0.0f, -1.0f, 0.0f);
	CameraPlane.Transform(mViewToObject);

	Float32 f1, f2, f3, f4;
	CameraPlane.GetCoeff(f1, f2, f3, f4);

	RENDER_CONTEXT::SetVertexShaderConstant( _uiCurrentVertexShaderRegister++,
											f1 / fFarDepthOfField,
											f2 / fFarDepthOfField,
											f3 / fFarDepthOfField,
											f4 / fFarDepthOfField);
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinningMatrices
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SHADER_ERROR::SetSkinningMatrices()
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
	_uiCurrentVertexShaderRegister += 3 * nNbrMatrices;
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_ERROR::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_ERROR));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_ERROR::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("SHADER_ERROR"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_ERROR,
										QDT_COMMENT("43044667-6DFA-F943-8BE3-26908EE51CD1"), 
										sizeof(SHADER_ERROR), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ERROR::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ERROR::EM_Construct(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	SHADER_ERROR	*pShader;
	pShader = static_cast<SHADER_ERROR *>(pO);

	QDT_NEW_PLACED(pShader, SHADER_ERROR)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_ERROR::EM_Destruct(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	SHADER_ERROR	*pShader;
	pShader = static_cast<SHADER_ERROR *>(pO);

	QDT_DELETE_PLACED(pShader, SHADER_ERROR);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexShader
//	Object:		
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	SHADER_ERROR::GetVertexShader() const
{
	return (_pVertexShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetSkinnedVertexShader
//	Object:		
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	SHADER_ERROR::GetSkinnedVertexShader() const
{
	return (_pSkinnedVertexShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelShader
//	Object:		
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	SHADER_ERROR::GetPixelShader() const
{
	return (_pPixelShader);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
