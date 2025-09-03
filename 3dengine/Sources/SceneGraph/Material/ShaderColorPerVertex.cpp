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
//	CLASS:	SHADER_COLOR_PER_VERTEX
//
//	05-11-09:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderColorPerVertex)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)

// Include vertex and pixel shaders
#include "ColorPerVertexVertexShader.h"
#include "ColorPerVertexSkinnedVertexShader.h"
#include "ColorPerVertexPixelShader.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

I_VERTEX_SHADER *	SHADER_COLOR_PER_VERTEX::_pSkinnedVertexShader = NULL;
I_VERTEX_SHADER *	SHADER_COLOR_PER_VERTEX::_pVertexShader = NULL;
I_PIXEL_SHADER *	SHADER_COLOR_PER_VERTEX::_pPixelShader = NULL;

//-----------------------------------------------------------------------------
//	Name:		SHADER_COLOR_PER_VERTEX
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_COLOR_PER_VERTEX::SHADER_COLOR_PER_VERTEX()
:
SHADER_ABC(GetGenericCommunicatorDescriptor())
{
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_COLOR_PER_VERTEX
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_COLOR_PER_VERTEX::~SHADER_COLOR_PER_VERTEX()
{
}

//-----------------------------------------------------------------------------
//	Name:		Initialize
//	Object:		
//	06-03-14:	VHA - Created
//-----------------------------------------------------------------------------
void	SHADER_COLOR_PER_VERTEX::Initialize()
{
	_pVertexShader = QDT_CREATE_VERTEXSHADER(ColorPerVertexVertexShader);
	_pSkinnedVertexShader = QDT_CREATE_VERTEXSHADER(ColorPerVertexSkinnedVertexShader);
	_pPixelShader = QDT_CREATE_PIXELSHADER(ColorPerVertexPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	06-03-14:	VHA - Created
//-----------------------------------------------------------------------------
void	SHADER_COLOR_PER_VERTEX::Release()
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
UInt32 SHADER_COLOR_PER_VERTEX::GetNbrPasses()
{
	return (1);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_COLOR_PER_VERTEX::SetPass(UInt32 uiPass, const VECTOR & Pos)
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

	if(_Texture.HasHandle())
	{
		ENGINE_TEXTURE_ABC * pTex = (ENGINE_TEXTURE_ABC*)_Texture.GetCommunicator();
		QDT_ASSERT(pTex);
		RENDER_CONTEXT::SetPixelShaderTexture(0, pTex->GetTexture());
	}
	else
	{
		RENDER_CONTEXT::SetPixelShaderTexture(0, NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetModelViewProj
//	Object:		
//	05-11-24:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_COLOR_PER_VERTEX::SetModelViewProj()
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
void	SHADER_COLOR_PER_VERTEX::SetSkinningMatrices()
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
COMMUNICATOR_DESCRIPTOR * SHADER_COLOR_PER_VERTEX::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_COLOR_PER_VERTEX));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_COLOR_PER_VERTEX::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("SHADER_COLOR_PER_VERTEX"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_COLOR_PER_VERTEX,
										QDT_COMMENT("43044666-6DFA-F943-8BE3-26908EE51CD1"), 
										sizeof(SHADER_COLOR_PER_VERTEX), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		SetTexture
//	Object:		
//	06-03-14:	VHA - Created
//-----------------------------------------------------------------------------
void	SHADER_COLOR_PER_VERTEX::SetTexture(const COM_INSTANCE &	ciTexture)
{
	_Texture = ciTexture;
}


//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_COLOR_PER_VERTEX::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCOLO", 0, CM_ReadChunk0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("SHADCOLO", 0, CM_WriteChunk0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_COLOR_PER_VERTEX::EM_Construct(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	SHADER_COLOR_PER_VERTEX	*pShader;
	pShader = static_cast<SHADER_COLOR_PER_VERTEX *>(pO);

	QDT_NEW_PLACED(pShader, SHADER_COLOR_PER_VERTEX)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_COLOR_PER_VERTEX::EM_Destruct(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	SHADER_COLOR_PER_VERTEX	*pShader;
	pShader = static_cast<SHADER_COLOR_PER_VERTEX *>(pO);

	QDT_DELETE_PLACED(pShader, SHADER_COLOR_PER_VERTEX);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk0
//	Object:		
//	06-03-15:	VHA - Created
//-----------------------------------------------------------------------------
void	SHADER_COLOR_PER_VERTEX::CM_ReadChunk0(	IO_STREAM &	Stream,
												COMMUNICATOR *	pCommunicator)
{
	SHADER_COLOR_PER_VERTEX	*pShader;
	pShader = static_cast<SHADER_COLOR_PER_VERTEX *>(pCommunicator);
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_Texture;
}
	
//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk0
//	Object:		
//	06-03-15:	VHA - Created
//-----------------------------------------------------------------------------
void SHADER_COLOR_PER_VERTEX::CM_WriteChunk0(	IO_STREAM & Stream, 
												COMMUNICATOR * pCommunicator)
{
	SHADER_COLOR_PER_VERTEX	*pShader;
	pShader = static_cast<SHADER_COLOR_PER_VERTEX *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("SHADER_COLOR_PER_VERTEX :");
	Stream.DebugOutput(pShader->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Blending : ");
	Stream << pShader->_eBlending;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    AlphaTest : ");
	Stream << pShader->_bEnableAlphaTest;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    AlphaTestValue : ");
	Stream << pShader->_rAlphaTestValue;
	Stream.DebugOutput("\r\n");

	Stream << pShader->_Texture;
	Stream.DebugOutput("\r\n");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
