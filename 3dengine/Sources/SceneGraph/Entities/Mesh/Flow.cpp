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
//	CLASS:	ENGINE_TEXTURE_ABC
//
//	05-11-10:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Flow)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/FlowData)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/ITexture)
#include	INCL_KRENDERER(Renderer/IRenderTarget)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFlow)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

UInt32     FLOW::_uiRefcount         = 0;
I_TEXTURE* FLOW::_pGaussianDrop      = NULL;

I_VERTEX_SHADER*		FLOW::_pFlowVS = NULL;
I_PIXEL_SHADER *		FLOW::_pDropsPS = NULL;
I_PIXEL_SHADER *		FLOW::_pQuadPS = NULL;
UInt32					FLOW::_uiUsesDropsBuffer = 0;
I_VERTEX_BUFFER*		FLOW::_pDropsBuffer[4] = {NULL, NULL, NULL, NULL};
I_VERTEX_BUFFER*		FLOW::_pQuadBuffer = NULL;
I_VERTEX_DECLARATION*	FLOW::_pVertexFormat = NULL;


#include "./Shaders/Flow_vs.h"
#include "./Shaders/FlowQuad_ps.h"
#include "./Shaders/FlowDrop_ps.h"

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_ABC Constructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
FLOW::FLOW(ENGINE_TEXTURE_FLOW* flowTex)
{
	QDT_ASSERT(flowTex);
	_pFlowData = NULL;
	_pTexFlow  = flowTex;

	if (_uiRefcount == 0)
	{
		// Creation de la Texture de drop
		_pGaussianDrop = RENDER_CONTEXT::CreateTexture(128, 128, PF_L8, 1);

		UInt32 size     = 128;
		UInt32 sizeDiv2 = 64;
		char* mem = (char*)_pGaussianDrop->Lock(0);
		for (UInt32 y = 0; y < size; y++)
		{
			Int32 dy = y - sizeDiv2;
			for (UInt32 x = 0; x < size; x++)
			{
				Int32 dx = x - sizeDiv2;
				Float32 len = MATH::Sqrt(dx * dx + dy * dy) / sizeDiv2;
				Int32 gaussian = (Int32)(255.0f * (1.0f - (len * len)));
				char val = (char)(MATH::Min(MATH::Max(0, gaussian), 255));
				*mem++ = val;
			}
		}
		size /= 2;
		sizeDiv2 /= 2;
		_pGaussianDrop->Unlock(0);
		_pGaussianDrop->SetFilterType(FT_BILINEAR);

		// Creation dees vertex buffers
		static S_VERTEX_ELEMENT elems[3] = {{0, 0,	DT_FLOAT3,	DU_POSITION},
											{0, 12, DT_FLOAT2,	DU_TEXCOORD0},
											{0, 20, DT_COLOR ,	DU_COLOR}};

		_pVertexFormat = RENDER_CONTEXT::CreateVertexDeclaration(elems, 3);

		for (UInt32 v = 0; v < 4; v++)
		{
			_pDropsBuffer[v] = RENDER_CONTEXT::CreateVertexBuffer(MAX_DROPS * 6, sizeof(FLOW_VERTEX), false);
		}

		_pQuadBuffer = RENDER_CONTEXT::CreateVertexBuffer(4, sizeof(FLOW_VERTEX), false);
		FLOW_VERTEX *verts = (FLOW_VERTEX*)_pQuadBuffer->Lock();

		static FLOW_VERTEX quadGeom[4] = {
											{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, 0x000000},
											{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, 0x000000},
											{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}, 0x000000},
											{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}, 0x000000}
										 };
		verts[0] = quadGeom[0];
		verts[1] = quadGeom[1];
		verts[2] = quadGeom[2];
		verts[3] = quadGeom[3];
		_pQuadBuffer->Unlock();

		_pFlowVS  = QDT_CREATE_VERTEXSHADER(Flow_vs);
		_pDropsPS = QDT_CREATE_PIXELSHADER(FlowDrop_ps);
		_pQuadPS  = QDT_CREATE_PIXELSHADER(FlowQuad_ps);

	}

	_bNeedsInit = true;
	_uiRefcount++;
	_iLastAlive = -1;
	_fCurMaxTrailHeight = 0.0f;

	_pRenderTarget = RENDER_CONTEXT::CreateRenderTarget(512, 512, PF_A8R8G8B8);

	DISPLAY_MANAGER::Instance()->RegisterFlow(this);
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_ABC Destructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
FLOW::~FLOW()
{
	DISPLAY_MANAGER::Instance()->UnRegisterFlow(this);

	RENDER_CONTEXT::ReleaseRenderTarget(_pRenderTarget);

	_uiRefcount--;

	if (_uiRefcount == 0)
	{
		RENDER_CONTEXT::ReleaseTexture(_pGaussianDrop);
		RENDER_CONTEXT::ReleaseVertexShader(_pFlowVS);
		RENDER_CONTEXT::ReleasePixelShader(_pDropsPS);
		RENDER_CONTEXT::ReleasePixelShader(_pQuadPS);
		RENDER_CONTEXT::ReleaseVertexDeclaration(_pVertexFormat);

		for (UInt32 v = 0; v < 4; v++)
		{
			RENDER_CONTEXT::ReleaseVertexBuffer(_pDropsBuffer[v]);
		}

		RENDER_CONTEXT::ReleaseVertexBuffer(_pQuadBuffer);
	}
}


//-----------------------------------------------------------------------------
//	Name:		AllocDrop
//	Object:		
//	06-03-27:	PDE - Created
//-----------------------------------------------------------------------------
FLOW::DROP *	FLOW::AllocDrop()
{
	if (_iLastAlive == MAX_DROPS - 1)
		return NULL;

	_iLastAlive++;
	FLOW::DROP * res = &_pDrops[_iLastAlive];
	return res;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseDrop
//	Object:		
//	06-03-27:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW::ReleaseDrop(Int32	index)
{
	if (index != _iLastAlive)
	{
		_pDrops[index] = _pDrops[_iLastAlive];
	}
	_iLastAlive--;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	06-03-28:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW::Update()
{
	Float32 time = TIME::Instance()->GetFrameTime();

	if (_bNeedsInit)
	{
		RENDER_CONTEXT::SetCurrentRenderTarget(_pRenderTarget);
		RENDER_CONTEXT::SetCurrentDepthBuffer(NULL);
		RENDER_CONTEXT::SetViewport(0, 0, _pRenderTarget->GetWidth(), _pRenderTarget->GetHeight());
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR, 0,  0,  0, 0);
		_bNeedsInit = false;
		_fLastUpdate = time;
		_fLastSubstract = time;
	}

	if ( time - _fLastUpdate > (1000.0f / _pTexFlow->GetUpdateFrequency()))
	{
		UpdateParticles();

		RENDER_CONTEXT::EnableDepthTest(false);
		RENDER_CONTEXT::EnableDepthWrite(false);
		RENDER_CONTEXT::SetCullMode(CULL_NONE);

		RENDER_CONTEXT::SetCurrentRenderTarget(_pRenderTarget);
		RENDER_CONTEXT::SetCurrentDepthBuffer(NULL);
		RENDER_CONTEXT::SetViewport(0, 0, _pRenderTarget->GetWidth(), _pRenderTarget->GetHeight());
	
		RENDER_CONTEXT::SetVertexShader(_pFlowVS);
		RENDER_CONTEXT::SetVertexDeclaration(_pVertexFormat);

		// Substracts a solid color full-screen quad
		RENDER_CONTEXT::SetPixelShader(_pQuadPS);
		RENDER_CONTEXT::SetPixelShaderConstant(0, 0.01f, 0.01f, 0.01f, 0.01f);
		RENDER_CONTEXT::SetVertexBuffer(0, _pQuadBuffer);

		RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_REVSUBTRACT, BM_ONE);
		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_FAN, 2, 0);

		// max withs the drops
		RENDER_CONTEXT::SetPixelShader(_pDropsPS);
		RENDER_CONTEXT::SetPixelShaderTexture(0, _pGaussianDrop);

		I_VERTEX_BUFFER* vBuffer = _pDropsBuffer[_uiUsesDropsBuffer];
		_uiUsesDropsBuffer = (_uiUsesDropsBuffer + 1) % 4;

		// Fills the vertex buffer with the representation of the drops
		FLOW_VERTEX* verts = (FLOW_VERTEX*)vBuffer->Lock();

		bool bOglReverse = RENDER_CONTEXT::GetRenderContextType() != RENDER_CONTEXT::RCT_DIRECT3D9;

		for (Int32 d = 0; d <= _iLastAlive; d++)
		{
			float dropX   = _pDrops[d].x * 2.0f - 1.0f;
			float dropY   = _pDrops[d].y * 2.0f - 1.0f;
			if (bOglReverse)
			{
				dropY = -dropY;
			}

			float dropRad = 0.05f;

			FLOW_VERTEX tmpVert[4];

			tmpVert[0].pos[0] = tmpVert[3].pos[0] = dropX - dropRad;
			tmpVert[1].pos[0] = tmpVert[2].pos[0] = dropX + dropRad;
			tmpVert[0].pos[1] = tmpVert[1].pos[1] = dropY - dropRad;
			tmpVert[2].pos[1] = tmpVert[3].pos[1] = dropY + dropRad;

			tmpVert[0].pos[2] = tmpVert[1].pos[2] = tmpVert[2].pos[2] = tmpVert[3].pos[2] = 0.0f;
			tmpVert[0].uv[0] = tmpVert[3].uv[0] = tmpVert[0].uv[1] = tmpVert[1].uv[1] = 0.0f;
			tmpVert[1].uv[0] = tmpVert[2].uv[0] = tmpVert[2].uv[1] = tmpVert[3].uv[1] = 1.0f;

			UInt32 col = RENDER_CONTEXT::GetPlatformIntColor(COLOR(0, 255, 255, 255));
			tmpVert[0].color = tmpVert[1].color =  tmpVert[2].color = tmpVert[3].color = col;
		

			*(verts++) = tmpVert[0];
			*(verts++) = tmpVert[1];
			*(verts++) = tmpVert[3];
			*(verts++) = tmpVert[1];
			*(verts++) = tmpVert[2];
			*(verts++) = tmpVert[3];
		}
		vBuffer->Unlock();

		RENDER_CONTEXT::SetVertexBuffer(0, vBuffer);
		RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_MAX, BM_ZERO);
		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_LIST, (_iLastAlive + 1) * 2, 0);

		_fLastUpdate = time;
		RENDER_CONTEXT::EnableDepthTest(true);
		RENDER_CONTEXT::EnableDepthWrite(true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SpawnDrop
//	Object:		
//	06-03-28:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW::SpawnDrop(FLOW_DATA* _pFlowData, UInt32	area)
{
	DROP* drp = AllocDrop();

	if (drp)
	{
		VECTOR_2 p(-666, 0);

		_pFlowData->GetSpawnPos(p, area);

		if (p.GetX() >= 0)
		{
			drp->x    = p.GetX();
			drp->y    = p.GetY();
			drp->sx   = 0.0f;
			drp->sy   = 0.0f;
			drp->life = 0.0f;
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		UpdateParticles
//	Object:		
//	06-03-30:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW::UpdateParticles()
{
//	_pDrops[0].x = MATH::Rand(1.0f);
//	_pDrops[0].y = MATH::Rand(1.0f);
//	_iLastAlive = 0;

	Float32 fElapsed = (1000.0f / _pTexFlow->GetUpdateFrequency());

	if (_pFlowData)
	{
		SpawnDrop(_pFlowData, 0);
	}

	for (Int32 d = 0; d <= _iLastAlive; d++)
	{
		_pDrops[d].y += 0.1f;
	}
}




//=============================================================================
//	CODE ENDS HERE
//=============================================================================
