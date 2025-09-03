//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: DISPLAY_MANAGER
//
//
//	00-11-13	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( DISPLAY_MANAGER )
//
//	The DISPLAY_MANAGER class implements ...
//
//*****************************************************************************

#if (!defined(_MASTER) && defined(PLATFORM_PC))
#include <direct.h>
#endif

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/Viewport/ViewportList)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Interface2D3D/DisplayFont/DisplayFont)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Scenegraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/RayCaster/RayCastParameters)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderDefault)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderColorPerVertex)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderError)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderShadow)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(Services/LoadingScreenService)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include	INCL_KCORE(Profiler/Profiler)
#include	INCL_KCORE(File/FileABC)
#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/RenderStats)
#include	INCL_KRENDERER(Renderer/IRenderTarget)
#include	INCL_KRENDERER(Renderer/ITexture)
#include	INCL_KRENDERER(PostProcessing/Photographic_Pipeline)

#ifdef _DEBUG
	#include	"DisplayManager.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//	For 2D Display
bool	DISPLAY_MANAGER::_bDisplay2D = true;
bool	DISPLAY_MANAGER::_bNeedToRestartEngine = false;
//bool	DISPLAY_MANAGER::_bApplyChanges = false;

#if defined(PLATFORM_PS2) && !defined(_MASTER_FINAL_BUILD) && defined(_MASTER)
	static bool bDisplayFPS = false;
#endif

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-07-19:	ONA - Created
//-----------------------------------------------------------------------------
DISPLAY_MANAGER *	DISPLAY_MANAGER::Instance()
{
	return	((DISPLAY_MANAGER *)GetSingletonDescriptor()->Instance());
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	02-07-19:	ONA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Release()
{
	GetSingletonDescriptor()->Release();
}

//-----------------------------------------------------------------------------
//	Name:		IsInstancied
//	Object:		
//	04-10-29:	RMA - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::IsInstancied()
{
	return (GetSingletonDescriptor()->IsInstancied());
}

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_MANAGER constructor
//	Object:		Default constructor
//	01-02-15:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_MANAGER::DISPLAY_MANAGER()
:
_pActiveCamera					(NULL),
_bChangeWindowSizeRequested		(false),
_nSizeX							(0),
_nSizeY							(0),
_bFullScreenRequested			(false),
_bWindowedRequested				(false),
_nFullSizeX						(0),
_nFullSizeY						(0),
//_bFullScreen					(false),
#if (!defined(_MASTER_FINAL_BUILD) && (defined(PLATFORM_PC)||defined(PLATFORM_PS2)))
_bVideoCapture					(false),
_nVideoFileCounter				(0),
#endif
_bNeedTestNearFogPlane			(false),
_NearFogPlane					(0.0f, 0.0f, 0.0f, 0.0f),
_bFrontBufferEffectsEnabled		(true),
_bIsDisplaying					(true),
_bManageVideo					(true),
_uiNbrLights					(0),
_bNeedUpdateWorldViewProj		(false),	
_bNeedUpdateInvWorld			(false),
_DebugRenderFlags				(0),
_mWorld							(MATRIX::IDENTITY),
_mInvWorld						(MATRIX::IDENTITY),
_mView							(MATRIX::IDENTITY),
_mProj							(MATRIX::IDENTITY),
_mWorldViewProj					(MATRIX::IDENTITY),
_mWorldView						(MATRIX::IDENTITY),
_bEnableShaderReplacement		(false),
_Vector2D_CurrentVB				(0),
_bDisplayMemStats				(false),
_bDisplayGraphicStats			(false)
{
	Init();
}

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_MANAGER destructor0
//	Object:		Destructor
//	01-02-15:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_MANAGER::~DISPLAY_MANAGER()
{
	Close();
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-11-07:	DAC - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Close()
{
	UInt32 i = 0;

	Close2D();

	PHOTOGRAPHIC_PIPELINE::Release();
	
	if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == true)
	{
		if (RENDER_CONTEXT::SupportHardwareShadowMap())
		{
			// Delete HardWare Shadow Map render targets
			//for (i = 0; i < MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
			{
				RENDER_CONTEXT::ReleaseRenderTarget(_aHardWareShadowMapsAvailable[i].pRenderTarget);
				RENDER_CONTEXT::ReleaseRenderTarget(_aHardWareShadowMapsAvailable[i].pDepthBuffer);
				_aHardWareShadowMapsAvailable[i].pRenderTarget = NULL;
				_aHardWareShadowMapsAvailable[i].pDepthBuffer = NULL;
				_aHardWareShadowMapsAvailable[i].pLight = NULL;
			}
		}
		else
		{
			// Create Basic Shadow Map render targets (should disappeared, will not be used later)
			//for (i = 0; i<MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
			{
				RENDER_CONTEXT::ReleaseRenderTarget(_aBasicShadowMapsAvailable[i].pRenderTarget);
				RENDER_CONTEXT::ReleaseRenderTarget(_aBasicShadowMapsAvailable[i].pDepthBuffer);
				_aBasicShadowMapsAvailable[i].pRenderTarget = NULL;
				_aBasicShadowMapsAvailable[i].pDepthBuffer = NULL;
				_aBasicShadowMapsAvailable[i].pLight = NULL;
			}
		}

		// Create HardWare Shadow Map cube render targets (not used for the moment)
		/*for (i = 0; i < MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
		{
			RENDER_CONTEXT::ReleaseRenderTarget(_aCubeHardWareShadowMapsAvailable[i].pRenderTarget);
			RENDER_CONTEXT::ReleaseRenderTarget(_aCubeHardWareShadowMapsAvailable[i].pDepthBuffer);
			_aCubeHardWareShadowMapsAvailable[i].pRenderTarget = NULL;
			_aCubeHardWareShadowMapsAvailable[i].pDepthBuffer = NULL;
			_aCubeHardWareShadowMapsAvailable[i].pLight = NULL;
		}

		// Create Basic Shadow Map render targets (should disappeared, will not be used later)
		for (i = 0; i < MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
		{
			RENDER_CONTEXT::ReleaseRenderTarget(_aCubeBasicShadowMapsAvailable[i].pRenderTarget);
			RENDER_CONTEXT::ReleaseRenderTarget(_aCubeBasicShadowMapsAvailable[i].pDepthBuffer);
			_aCubeBasicShadowMapsAvailable[i].pRenderTarget = NULL;
			_aCubeBasicShadowMapsAvailable[i].pDepthBuffer = NULL;
			_aCubeBasicShadowMapsAvailable[i].pLight = NULL;
		}*/
	}

	_ciDefaultShader.Release();
	SHADER_COLOR_PER_VERTEX::Release();

	#ifdef PLATFORM_PC
	if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == true && RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == true)
	#endif
	{	
		_ciErrorShader.Release();
		_ciShadowShader.Release();	
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		Only init which must be reset after an API close
//	01-08-29:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Init()
{
	_nSizeX = RENDER_CONTEXT::GetScreenWidth();
	_nSizeY = RENDER_CONTEXT::GetScreenHeight();

	_bDynamicShadowsEnabled = false;
	if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == true)
	{
		QDT_Message("Shadows available");
		_bDynamicShadowsEnabled = true;

		UInt32 i = 0, nMaxSize = 2048;
		// !!! ATTENTION !!! si modif, Penser a modifier dans DISPLAY_MANAGER::Close()

		if (RENDER_CONTEXT::SupportHardwareShadowMap())
		{
			// Create HardWare Shadow Map render targets
			//for (i = 0; i<MAX_SM_RENDER_TARGETS_AVAILABLE; i++) on en cree une pr le moment (demo)
			{
				QDT_ASSERT((nMaxSize >> i) > 0);
			
				#ifndef PLATFORM_PS3
					_aHardWareShadowMapsAvailable[i].pRenderTarget = RENDER_CONTEXT::CreateRenderTarget(nMaxSize >> i, nMaxSize >> i, PF_A8R8G8B8);
				#else
					_aHardWareShadowMapsAvailable[i].pRenderTarget = NULL;
				#endif
				_aHardWareShadowMapsAvailable[i].pDepthBuffer = RENDER_CONTEXT::CreateRenderTarget(nMaxSize >> i, nMaxSize >> i, PF_D24S8);
				_aHardWareShadowMapsAvailable[i].pLight = NULL;

				_aHardWareShadowMapsAvailable[i].pDepthBuffer->GetTexture()->SetFilterType(FT_BILINEAR);
			}
		}
		else
		{
			// Create Basic Shadow Map render targets (should disappeared, will not be used later)
			for (i = 0; i<MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
			{
				QDT_ASSERT((nMaxSize >> i) > 0);
			
				_aBasicShadowMapsAvailable[i].pRenderTarget = RENDER_CONTEXT::CreateRenderTarget(nMaxSize >> i, nMaxSize >> i, PF_R32F);
				_aBasicShadowMapsAvailable[i].pDepthBuffer = RENDER_CONTEXT::CreateRenderTarget(nMaxSize >> i, nMaxSize >> i, PF_D24S8_DEPTHSTENCIL);
				_aBasicShadowMapsAvailable[i].pLight = NULL;
			}
		}

		// todo cube render target (crash a la creation !?)
		nMaxSize = 512;

		// Create HardWare Shadow Map cube render targets (not used for the moment)
		/*for (i = 0; i<MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
		{
			QDT_ASSERT((nMaxSize >> i) > 0);
		
			_aCubeHardWareShadowMapsAvailable[i].pRenderTarget = NULL;
			_aCubeHardWareShadowMapsAvailable[i].pDepthBuffer = RENDER_CONTEXT::CreateCubeRenderTarget(nMaxSize >> i, PF_D24S8);
			_aCubeHardWareShadowMapsAvailable[i].pLight = NULL;
		}*/

		// Create Basic Shadow Map render targets (should disappeared, will not be used later)
		/*for (i = 0; i<MAX_SM_RENDER_TARGETS_AVAILABLE; i++)
		{
			QDT_ASSERT((nMaxSize >> i) > 0);
		
			_aCubeBasicShadowMapsAvailable[i].pRenderTarget = RENDER_CONTEXT::CreateCubeRenderTarget(nMaxSize >> i, PF_R32F);
			_aCubeBasicShadowMapsAvailable[i].pDepthBuffer = RENDER_CONTEXT::CreateCubeRenderTarget(nMaxSize >> i, PF_D24S8_DEPTHSTENCIL);
			_aCubeBasicShadowMapsAvailable[i].pLight = NULL;
		}*/
	}

	PHOTOGRAPHIC_PIPELINE::Initialize(_nSizeX, _nSizeY, 6, 8);

	_ciDefaultShader		= SHADER_DEFAULT::GetGenericCommunicatorDescriptor()->Create();
	SHADER_COLOR_PER_VERTEX::Initialize();

	#ifdef PLATFORM_PC
	if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == true && RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == true)
	#endif
	{	
		_ciErrorShader = SHADER_ERROR::GetGenericCommunicatorDescriptor()->Create();
		_ciShadowShader	= SHADER_SHADOW::GetGenericCommunicatorDescriptor()->Create();	
	}

	Init2D();

	// init line shader
	{
		_ciLineShader.SetHandle( SHADER_COLOR_PER_VERTEX::GetGenericCommunicatorDescriptor()->Create() );
		SHADER_COLOR_PER_VERTEX* pShader = static_cast<SHADER_COLOR_PER_VERTEX *>( _ciLineShader.GetCommunicator() );
		pShader->SetBlending( SB_ALPHA );
	}

	// int debug font shader
	{
		_pDebugFontCI.Instanciate(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_FONT);
		DISPLAY_FONT* pFont = (DISPLAY_FONT *)(_pDebugFontCI.GetCommunicator());

		pFont->BuildFromStaticDebugFont();

		_pDebugIHM_ElementTextCI.Instanciate(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT);
		_pDebugIHM_ElementText = static_cast<IHM_ELEMENT_TEXT *>(_pDebugIHM_ElementTextCI.GetCommunicator());
		_pDebugIHM_ElementText->SetFontCI(_pDebugFontCI);
		_pDebugIHM_ElementText->SetRelativePosLeft(0.5f);		
		_pDebugIHM_ElementText->SetRelativePosRight(100.0f);	
		_pDebugIHM_ElementText->SetRelativePosTop(0.0f);		
		_pDebugIHM_ElementText->SetRelativePosBottom(100.0f);
		_pDebugIHM_ElementText->SetText(QDT_STRING(""));
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init2d
//	Object:		
//	06-03-15:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Init2D()
{
	S_VERTEX_ELEMENT g_decl[3] =	{
										{0, 0,	DT_FLOAT4,	DU_POSITION},
										{0, 16, DT_FLOAT2,	DU_TEXCOORD0},
										{0, 24, DT_COLOR,	DU_COLOR},
									};

	S_VERTEX_ELEMENT g_line_decl[3] =	{
											{0, 0,	DT_FLOAT4,	DU_POSITION},
											{0, 16, DT_COLOR,	DU_COLOR},
										};

	_Vector2D_Decl = RENDER_CONTEXT::CreateVertexDeclaration(g_decl, 3);
	_Line_Vector2D_Decl = RENDER_CONTEXT::CreateVertexDeclaration(g_line_decl, 2);

	int i;
	for(i=0;i<2;i++)
	{
		_Vector2D_PositionInVB[i] = 0;
		_Vector2D_VB[i] = RENDER_CONTEXT::CreateVertexBuffer(6*QD_MAX_QUAD_IN_VB, sizeof(S_2D_VERTEX));
	}

	for(i=0;i<2;i++)
	{
		_Line_Vector2D_PositionInVB[i] = 0;
		_Line_Vector2D_VB[i] = RENDER_CONTEXT::CreateVertexBuffer(2*QD_MAX_LINE_IN_VB, sizeof(S_LINE_2D_VERTEX));
	}

#if defined(QDT_MEM_ENABLED) && defined(QDT_MEM_DEBUG) && defined(QDT_MEM_USE_TIME_ID)
	_pMemStatsEntityTable = new STATS_ENTITY_TEXT[1];
	for(i=0;i<MEM_LIBID_MAX;i++)
	{
		_pMemStatsEntityTable->PushTimeLog(MEM::GetInstance()->GetTimeLogForLib(i));
	}
	_pMemStatsEntityTable->SetDisplayRange(2.f,100.f,2.f,100.f);
	_pMemStatsEntityTable->SetName("Mem stats");
	_pMemStatsEntityTable->SetMinimumValue(1.f);
	_pMemStatsEntityTable->SetRangeColor(0.f, 0.4f*1024*1024, COLOR::GREEN);
	_pMemStatsEntityTable->SetRangeColor(0.4f*1024*1024, 0.8f*1024*1024, COLOR::YELLOW);
	_pMemStatsEntityTable->SetRangeColor(0.8f*1024*1024, 1024*1024*1024, COLOR::RED);
	_pMemStatsEntityTable->SetUseSize(true);
	_pMemStatsEntityTable->SetUseColoredText(true);
	_pMemStatsEntityTable->SetShowTotal(true);

	_pMemStatsEntityGraphTable = new STATS_ENTITY_GRAPH[2];
	int j;
	for(j=0;j<2;j++)
	{
		for(i=0;i<MEM_LIBID_MAX;i++)
		{
			_pMemStatsEntityGraphTable[j].PushTimeLog(MEM::GetInstance()->GetTimeLogForLib(i));
		} 
	}
	_pMemStatsEntityGraphTable[0].SetDisplayRange(50.f,90.f,10.f,80.f);
	_pMemStatsEntityGraphTable[0].SetName("Big Mem stats");
	_pMemStatsEntityGraphTable[0].SetMinimumValue(100*1024);
	_pMemStatsEntityGraphTable[0].SetUseSize(true);
	_pMemStatsEntityGraphTable[0].SetRangeColor(0.f, 0.4f*1024*1024, COLOR::GREEN);
	_pMemStatsEntityGraphTable[0].SetRangeColor(0.4f*1024*1024, 0.8f*1024*1024, COLOR::YELLOW);
	_pMemStatsEntityGraphTable[0].SetRangeColor(0.8f*1024*1024, 1024*1024*1024, COLOR::RED);

	_pMemStatsEntityGraphTable[1].SetDisplayRange(10.f,45.f,30.f,80.f);
	_pMemStatsEntityGraphTable[1].SetName("Small Mem stats");
	_pMemStatsEntityGraphTable[1].SetMaximumValue(1);
	_pMemStatsEntityGraphTable[1].SetMaximumValue(100*1024);
	_pMemStatsEntityGraphTable[1].SetUseSize(true);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Close2D
//	Object:		
//	06-03-16:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Close2D()
{
	int i;
	for(i=0;i<2;i++)
	{
		RENDER_CONTEXT::ReleaseVertexBuffer(_Vector2D_VB[i]);
		RENDER_CONTEXT::ReleaseVertexBuffer(_Line_Vector2D_VB[i]);
	}
	RENDER_CONTEXT::ReleaseVertexDeclaration(_Vector2D_Decl);
	RENDER_CONTEXT::ReleaseVertexDeclaration(_Line_Vector2D_Decl);
}

static void* g_vMem = NULL;

//-----------------------------------------------------------------------------
//	Name:		PushQuad
//	Object:		
//	06-03-15:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::PushQuad(	POINT2D* points,
									POINT2D* uvs,
									COLOR*	colors)
{
	RENDER_CONTEXT::SetCullMode(CULL_NONE);

	QDT_ASSERT(_Vector2D_PositionInVB[_Vector2D_CurrentVB] + 6 <= QD_MAX_QUAD_IN_VB * 6);

	if (g_vMem == NULL)
	{
		g_vMem = _Vector2D_VB[_Vector2D_CurrentVB]->Lock();
	}

	S_2D_VERTEX* pVector = ((S_2D_VERTEX*)g_vMem)+_Vector2D_PositionInVB[_Vector2D_CurrentVB];

	pVector[0].x = points[0].GetX();
	pVector[0].y = points[0].GetY();
	pVector[0].z = 1.f;
	pVector[0].w = 1.f;
	pVector[0].u = uvs[0].GetX();
	pVector[0].v = uvs[0].GetY();
	pVector[0].color = RENDER_CONTEXT::GetPlatformIntColor(colors[0]);

	pVector[1].x = points[0].GetX();
	pVector[1].y = points[0].GetY();
	pVector[1].z = 1.f;
	pVector[1].w = 1.f;
	pVector[1].u = uvs[0].GetX();
	pVector[1].v = uvs[0].GetY();
	pVector[1].color = RENDER_CONTEXT::GetPlatformIntColor(colors[0]);

	pVector[2].x = points[2].GetX();
	pVector[2].y = points[2].GetY();
	pVector[2].z = 1.f;
	pVector[2].w = 1.f;
	pVector[2].u = uvs[2].GetX();
	pVector[2].v = uvs[2].GetY();
	pVector[2].color = RENDER_CONTEXT::GetPlatformIntColor(colors[2]);

	pVector[3].x = points[1].GetX();
	pVector[3].y = points[1].GetY();
	pVector[3].z = 1.f;
	pVector[3].w = 1.f;
	pVector[3].u = uvs[1].GetX();
	pVector[3].v = uvs[1].GetY();
	pVector[3].color = RENDER_CONTEXT::GetPlatformIntColor(colors[1]);

	pVector[4].x = points[3].GetX();
	pVector[4].y = points[3].GetY();
	pVector[4].z = 1.f;
	pVector[4].w = 1.f;
	pVector[4].u = uvs[3].GetX();
	pVector[4].v = uvs[3].GetY();
	pVector[4].color = RENDER_CONTEXT::GetPlatformIntColor(colors[3]);

	pVector[5].x = points[3].GetX();
	pVector[5].y = points[3].GetY();
	pVector[5].z = 1.f;
	pVector[5].w = 1.f;
	pVector[5].u = uvs[3].GetX();
	pVector[5].v = uvs[3].GetY();
	pVector[5].color = RENDER_CONTEXT::GetPlatformIntColor(colors[3]);

	_Vector2D_PositionInVB[_Vector2D_CurrentVB]+=6;

	if(_Vector2D_PositionInVB[_Vector2D_CurrentVB] >= QD_MAX_QUAD_IN_VB * 6)
	{	
		_Vector2D_VB[_Vector2D_CurrentVB]->Unlock();
		g_vMem = NULL;

		RENDER_CONTEXT::SetVertexBuffer(0, _Vector2D_VB[_Vector2D_CurrentVB]);
		RENDER_CONTEXT::SetVertexDeclaration(_Vector2D_Decl);
		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_STRIP, (6*QD_MAX_QUAD_IN_VB)-2, 0);

		if(_Vector2D_CurrentVB == 0)
			_Vector2D_CurrentVB = 1;
		else
			_Vector2D_CurrentVB = 0;

		_Vector2D_PositionInVB[_Vector2D_CurrentVB] = 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FlushQuads
//	Object:		
//	06-03-16:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::FlushQuads()
{
	if (g_vMem)
	{
		_Vector2D_VB[_Vector2D_CurrentVB]->Unlock();
		g_vMem = NULL;

		RENDER_CONTEXT::SetVertexBuffer(0, _Vector2D_VB[_Vector2D_CurrentVB]);
		RENDER_CONTEXT::SetVertexDeclaration(_Vector2D_Decl);
		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_STRIP, _Vector2D_PositionInVB[_Vector2D_CurrentVB]-2, 0);

		_Vector2D_CurrentVB = 0;

		_Vector2D_PositionInVB[_Vector2D_CurrentVB] = 0;
	}
}

void* g_vLineMem = NULL;

//-----------------------------------------------------------------------------
//	Name:		PushLine
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::PushLine(	POINT2D* points,
									COLOR*	colors)
{
	RENDER_CONTEXT::SetCullMode(CULL_NONE);

	QDT_ASSERT(_Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB] + 1 <= QD_MAX_LINE_IN_VB);

	if (g_vLineMem == NULL)
	{
		g_vLineMem = _Line_Vector2D_VB[_Line_Vector2D_CurrentVB]->Lock();
	}

	S_LINE_2D_VERTEX* pVector = ((S_LINE_2D_VERTEX*)g_vLineMem)+_Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB];

	pVector[0].x = points[0].GetX();
	pVector[0].y = points[0].GetY();
	pVector[0].z = 1.f;
	pVector[0].w = 1.f;
	pVector[0].color = RENDER_CONTEXT::GetPlatformIntColor(colors[0]);

	_Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB]++;

	if(_Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB] >= QD_MAX_LINE_IN_VB)
	{	
		_Line_Vector2D_VB[_Line_Vector2D_CurrentVB]->Unlock();
		g_vLineMem = NULL;

		RENDER_CONTEXT::SetVertexBuffer(0, _Line_Vector2D_VB[_Line_Vector2D_CurrentVB]);
		RENDER_CONTEXT::SetVertexDeclaration(_Line_Vector2D_Decl);
		RENDER_CONTEXT::DrawPrimitive(PT_LINE_STRIP, QD_MAX_LINE_IN_VB-1, 0);

		if(_Line_Vector2D_CurrentVB == 0)
			_Line_Vector2D_CurrentVB = 1;
		else
			_Line_Vector2D_CurrentVB = 0;

		_Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB] = 0;

		PushLine(points,colors);
	}
}

//-----------------------------------------------------------------------------
//	Name:		FlushLines
//	Object:		
//	06-03-28:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::FlushLines()
{
	if (g_vLineMem)
	{
		_Line_Vector2D_VB[_Line_Vector2D_CurrentVB]->Unlock();
		g_vLineMem = NULL;

		RENDER_CONTEXT::SetVertexBuffer(0, _Line_Vector2D_VB[_Line_Vector2D_CurrentVB]);
		RENDER_CONTEXT::SetVertexDeclaration(_Line_Vector2D_Decl);
		RENDER_CONTEXT::DrawPrimitive(PT_LINE_STRIP, _Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB]-1, 0);

		_Line_Vector2D_CurrentVB = 0;

		_Line_Vector2D_PositionInVB[_Line_Vector2D_CurrentVB] = 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SwapFullscreenMode
//	Object:		
//	06-02-15:	PDE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void	DISPLAY_MANAGER::SwapFullscreenMode()
{
	RENDER_CONTEXT::SetFullScreen(!RENDER_CONTEXT::IsFullScreen());
	UInt32 w = RENDER_CONTEXT::GetScreenWidth();
	UInt32 h = RENDER_CONTEXT::GetScreenHeight();
	PHOTOGRAPHIC_PIPELINE::ChangeResolution(w, h);
	_nSizeX = w;
	_nSizeY = h;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowSize
//	Object:		
//	06-02-15:	PDE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void	DISPLAY_MANAGER::ChangeWindowSize(UInt32	w,
										  UInt32	h)
{
	if (w != _nSizeX || h != _nSizeY)
	{
		RENDER_CONTEXT::ChangeWindowSize(w, h);
		PHOTOGRAPHIC_PIPELINE::ChangeResolution(w, h);
		_nSizeX = w;
		_nSizeY = h;
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowPos
//	Object:		
//	06-02-15:	PDE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void	DISPLAY_MANAGER::ChangeWindowPos(UInt32	w,
										 UInt32	h)
{
	RENDER_CONTEXT::ChangeWindowPos(w, h);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		FillCubeMap
//	Object:		
//	06-03-06:	PDE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::FillCubeMap(SCENE_GRAPH * pSceneGraph,
									 const VECTOR &	Position,
									 I_RENDER_TARGET *	pRenderTarget,
									 I_RENDER_TARGET *  pDepthBuffer)
{
	static Bool bInit = false;
	static CAMERA * pCamera = NULL;
	static VIEWPORT * pViewport = NULL;
	static COM_INSTANCE ciCamera;
	static COM_INSTANCE ciViewport;

	if (bInit == false)
	{
		ciCamera = CAMERA::GetGenericCommunicatorDescriptor()->Create();
		ciViewport = VIEWPORT::GetGenericCommunicatorDescriptor()->Create();

		pCamera = static_cast<CAMERA *>(ciCamera.GetCommunicator());
		pViewport = static_cast<VIEWPORT * >(ciViewport.GetCommunicator());

		pCamera->SetHorizontalFieldOfView(MATH::PI() / 2.0f);
		pCamera->SetAspectRatio(1.0f);
		pCamera->SetScale(1.0f);
		pCamera->SetNearClippingDistance(1.0f);
		pCamera->SetFarClippingDistance(1000000.0f);

		pViewport->EnablePostProcessing(false);
		pViewport->EnableWireframe(false);
		pViewport->SetLeft(0.0f);
		pViewport->SetTop(0.0f);
		pViewport->SetWidth(100.0f);
		pViewport->SetHeight(100.0f);
		pViewport->SetActiveCameraInst(ciCamera);

		bInit = true;
	}

	pSceneGraph->AddEntity(ciCamera);

	pCamera->GetNode()->SetGlobalPosition(Position);

	RENDER_CONTEXT::SetCurrentRenderTarget(pRenderTarget, 0, CF_POSITIVE_X);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pDepthBuffer);

	pCamera->GetNode()->SetGlobalRotXYZ(VECTOR(0.0f, -MATH::PI() / 2.0f, 0.0f));
	pViewport->Display();

	RENDER_CONTEXT::SetCurrentRenderTarget(pRenderTarget, 0, CF_NEGATIVE_X);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pDepthBuffer);
	pCamera->GetNode()->SetGlobalRotXYZ(VECTOR(0.0f, MATH::PI() / 2.0f, 0.0f));
	pViewport->Display();

	RENDER_CONTEXT::SetCurrentRenderTarget(pRenderTarget, 0, CF_POSITIVE_Y);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pDepthBuffer);
	pCamera->GetNode()->SetGlobalRotXYZ(VECTOR(MATH::PI() / 2.0f, 0.0f, 0.0f));
	pViewport->Display();

	RENDER_CONTEXT::SetCurrentRenderTarget(pRenderTarget, 0, CF_NEGATIVE_Y);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pDepthBuffer);
	pCamera->GetNode()->SetGlobalRotXYZ(VECTOR(-MATH::PI() / 2.0f, 0.0f, 0.0f));
	pViewport->Display();

	RENDER_CONTEXT::SetCurrentRenderTarget(pRenderTarget, 0, CF_POSITIVE_Z);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pDepthBuffer);
	pCamera->GetNode()->SetGlobalRotXYZ(VECTOR(0.0f, MATH::PI(), 0.0f));
	pViewport->Display();

	RENDER_CONTEXT::SetCurrentRenderTarget(pRenderTarget, 0, CF_NEGATIVE_Z);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pDepthBuffer);
	pCamera->GetNode()->SetGlobalRotXYZ(VECTOR(0.0f, 0.0f, 0.0f));
	pViewport->Display();

	pSceneGraph->RemoveEntity(ciCamera);

	RENDER_CONTEXT::SetCurrentRenderTarget(RENDER_CONTEXT::GetFrontBuffer());
	RENDER_CONTEXT::SetCurrentDepthBuffer(RENDER_CONTEXT::GetDepthBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_MANAGER::Display
//	Object:		Display all the Viewports
//	01-02-12:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::Display()
{
	if(!_bIsDisplaying)
	{
		return;
	}

	Float32 rBefore = TIME::Instance()->GetSystemTime();

	DISPLAY_MANAGER* pDisplayManager = DISPLAY_MANAGER::Instance();

	RENDER_CONTEXT::SetCurrentRenderTarget(RENDER_CONTEXT::GetFrontBuffer());
	RENDER_CONTEXT::SetCurrentDepthBuffer(RENDER_CONTEXT::GetDepthBuffer());
	RENDER_CONTEXT::EnableDepthTest(true);
	RENDER_CONTEXT::EnableDepthWrite(true);


	ResetSoftBodies();
	UpdateAllFlows();

	if (RENDER_CONTEXT::GetRenderContextType() != RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA)
	{
		RENDER_CONTEXT::SetViewport(0, 0, RENDER_CONTEXT::GetScreenWidth(), RENDER_CONTEXT::GetScreenHeight());
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER | CF_CLEAR_STENCIL, 0, 0, 0, 0, 1.0f);
	}

	VIEWPORT_LIST *pViewportList = GetViewportList();
	if (pViewportList)
	{
		if	(LOADING_SCREEN_SERVICE::Instance()->IsRunning() == false)
		{
			pViewportList->Display();
		}
		else
		{
			BEHAVIOR_MANAGER::Instance()->UpdateAnimatedNodesChildren();
		}
	}


	if (MEM::IsInstancied() )
	{
		if(MEM::GetInstance()->isMemStatsAvailable())
		{
			MEM::GetInstance()->Update();
		}

		if(_bDisplayMemStats)
		{
			if(MEM::GetInstance()->isMemStatsAvailable())
			{
				UpdateMemStats();
			}
			else
			{
				AddDebugText("{C:255:000:000}Unable to display mem stats,{P}it wasn't compiled in the binary !", 5, 5);
			}
		}
	}

	UpdateGraphicStats();
/*	Float32 rAfter = TIME::Instance()->GetSystemTime();

	static UInt32 NBF = 0;
	NBF++;
	if (NBF > 100)
	{
	QDT_STRING S;
	static UInt32 F = 0;
	F += (UInt32)(1000.f / (rAfter - rBefore));
	S.Format("FPS : %f\n", (Float32)(F/(NBF-100)));
	AddDebugText(S.GetBuffer(), 5, 5);
	}*/

	if(_bDisplay2D)
	{
		Display2D();
	}

	//DisplayProfiler();


	_CurrentViewport = COM_INSTANCE::GetNullComInstance();

	
}

//-----------------------------------------------------------------------------
//	Name:		Display2DElts
//	Object:		
//	01-10-25:	RMA - Created
//	05-04-27:	RMA - Modified : For optimization purpose, filtering is set
//				to bilinear for 2D
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Display2D()
{
	INTERFACE_MANAGER::Instance()->Update();
	INTERFACE_MANAGER::Instance()->Display();

/*
#ifndef PLATFORM_PS2
	if (GetFilterType() > TFT_BILINEAR)
	{
		// Disable temporarly trilinear or anisotropic layer 0
		RENDER_MANAGER::Instance()->GetRenderContext()->SetTextureFilterType(0, TFT_BILINEAR);
	}
#endif
*/
	IHM_ELEMENT_ABC	*pElement = NULL;
	COM_INSTANCE CI;

/*	VIDEO_MANAGER_ABC	*pVideoManager = RENDER_MANAGER::Instance()->GetVideoManager();
	if	(pVideoManager && pVideoManager->IsPlaying())
	{
		// We display all the video interface
		if (!_VideoIhmElements.IsEmpty())
		{
			QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _VideoIhmElements.GetHead();
			QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd = _VideoIhmElements.GetTail();

			while (It != ItEnd)
			{
				CI = *It;
				pElement = static_cast<IHM_ELEMENT_ABC *>(CI.GetCommunicator());

				pElement->DisplayElement(NULL);
		
				++It;
			}
		}
	}
	else*/
	{
		// We display all the under interface ihm elements
		if (!_UnderIhmElements.IsEmpty())
		{
			QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _UnderIhmElements.GetHead();
			QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd = _UnderIhmElements.GetTail();

			while (It != ItEnd)
			{
				CI = *It;
				pElement = static_cast<IHM_ELEMENT_ABC *>(CI.GetCommunicator());

				pElement->DisplayElement(NULL);
		
				++It;
			}
		}

		INTERFACE_MANAGER::Instance()->Display();

		// We display all the over interface ihm elements
		if (!_OverIhmElements.IsEmpty())
		{
			QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _OverIhmElements.GetHead();
			QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd = _OverIhmElements.GetTail();

			while (It != ItEnd)
			{
				CI = *It;
				pElement = static_cast<IHM_ELEMENT_ABC *>(CI.GetCommunicator());

				pElement->DisplayElement(NULL);
		
				++It;
			}
		}
	}
/*
#ifndef PLATFORM_PS2
	// Restore filtering
	RENDER_MANAGER::Instance()->GetRenderContext()->SetTextureFilterType(0, GetFilterType());
#endif*/

	// display des infos de debug:
	if (!_pDebugMessageList.IsEmpty())
	{
		int i;
		int numEntries = _pDebugMessageList.GetSize();

		for(i=0;i<numEntries;i++)
		{
			S_DEBUG_MESSAGE* pCurrentMessage = &_pDebugMessageList[i];

			_pDebugIHM_ElementText->SetText(pCurrentMessage->_Message);
			_pDebugIHM_ElementText->SetRelativePosLeft(pCurrentMessage->_fLeft);
			_pDebugIHM_ElementText->SetRelativePosTop(pCurrentMessage->_fTop);
			_pDebugIHM_ElementText->DisplayElement(NULL);
		}

		_pDebugMessageList.Clear();
	}

}

//-----------------------------------------------------------------------------
//	Name:		AddDebugText
//	Object:		
//	06-03-16:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::AddDebugText(QDT_STRING	text,
									  Float32		fLeft,
									  Float32		fTop)
{
	S_DEBUG_MESSAGE pNewMessage;

	pNewMessage._Message = text;
	pNewMessage._fLeft = fLeft;
	pNewMessage._fTop = fTop;

	_pDebugMessageList.PushTail(pNewMessage);
}

//-----------------------------------------------------------------------------
//	Name:		Update2D
//	Object:		
//	01-11-07:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::Update2D()
{
	IHM_ELEMENT_ABC	*pElement = NULL;

	if (!_UnderIhmElements.IsEmpty())
	{
		QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _UnderIhmElements.GetHead();
		QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd = _UnderIhmElements.GetTail();

		while (It != ItEnd)
		{
			pElement = static_cast<IHM_ELEMENT_ABC *>((*It).GetCommunicator());

			pElement->SetVerticesToReset(true);
	
			++It;
		}
	}

	if (!_OverIhmElements.IsEmpty())
	{
		QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _OverIhmElements.GetHead();
		QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd = _OverIhmElements.GetTail();

		while (It != ItEnd)
		{
			pElement = static_cast<IHM_ELEMENT_ABC *>((*It).GetCommunicator());

			pElement->SetVerticesToReset(true);
	
			++It;
		}
	}

	if (!_VideoIhmElements.IsEmpty())
	{
		QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _VideoIhmElements.GetHead();
		QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd = _VideoIhmElements.GetTail();

		while (It != ItEnd)
		{
			pElement = static_cast<IHM_ELEMENT_ABC *>((*It).GetCommunicator());

			pElement->SetVerticesToReset(true);
	
			++It;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	01-08-02:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::RayCastFromViewport(int					nScreenX,
											 int					nScreenY,
											 RAY_CAST_PARAMETERS &	RayCastParameters,
											 const COM_INSTANCE	&	ViewportInst)
{
#ifdef PLATFORM_PC
	VIEWPORT*		pViewport = NULL;
	RAY				Ray;
	int				nViewportX;
	int				nViewportY;
	CAMERA *		pCamera;
	SCENE_GRAPH *	pSG;
	COM_INSTANCE	SGNodeInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
	int				nVPosX, nVPosY;

	pViewport = static_cast<VIEWPORT *>((ViewportInst).GetCommunicator());
	QDT_ASSERT(pViewport);

	//	Windowed mode
	if ( !RENDER_CONTEXT::IsFullScreen() )
	{
		//	Get window pos
		nVPosX = RENDER_CONTEXT::GetWindowPosX();
		nVPosY = RENDER_CONTEXT::GetWindowPosY();
		//	Set coordinates from window relative to screen relative
		nScreenX = nScreenX + nVPosX;
		nScreenY = nScreenY + nVPosY;
	}
	else
	{
		//	Not supported for now
		//	Implement methods in viewport to manage
		//	screen coordinates in both cases
	}

	if (pViewport->GetRelativeCoordinates(nScreenX, nScreenY, nViewportX, nViewportY))
	{
		// Point inside viewport
		Ray = pViewport->GetRay(nViewportX, nViewportY);
		RayCastParameters.SetRay(Ray);

		pCamera = static_cast<CAMERA *>(pViewport->GetActiveCameraInst().GetCommunicator());
		if (pCamera == NULL)
		{
			return;
		}

		pSG = pCamera->GetSceneGraph();
		if (pSG == NULL)
		{
			return;
		}

		pSG->RayCastAllEntities(RayCastParameters);		
	}
#else
	QDT_NOT_IMPLEMENTED();
#endif
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		For picking 3D
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::RayCast(int					nScreenX,
								 int					nScreenY,
								 RAY_CAST_PARAMETERS &	RayCastParameters)
{
	GetViewportList()->RayCast(nScreenX, nScreenY, RayCastParameters);
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplay2D
//	Object:		
//	02-04-24:	JLT - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetDisplay2D(const bool bFlag)
{
	_bDisplay2D = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		AddIhmElement
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::AddIhmElement(const COM_INSTANCE &	Element,
									   bool					bUnderInterface)
{
	float					rZDepth;
	bool					bUnder;
	QDT_DLIST<COM_INSTANCE>	*pList;
	IHM_ELEMENT_ABC			*pElement;

	pElement = static_cast<IHM_ELEMENT_ABC *>(Element.GetCommunicator());
	rZDepth = pElement->GetZDepth();
	bUnder =  bUnderInterface && pElement->IsUnder2DInterface();
	
	DISPLAY_MANAGER::Instance()->RemoveIhmElement(Element);

	if	(bUnder)
	{
		pList = &_UnderIhmElements;
	}
	else
	{
		pList = &_OverIhmElements;
	}

	QDT_DLIST<COM_INSTANCE>::ITERATOR It = pList->GetHead();;
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd = pList->GetTail();

	while (It != ItEnd)
	{
		pElement = static_cast<IHM_ELEMENT_ABC *>((*It).GetCommunicator());

		if (rZDepth > pElement->GetZDepth())
		{
			pList->Insert(It, Element);
			return;
		}
		++It ;
	}

	pList->PushTail(Element);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIhmElement
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::RemoveIhmElement(const COM_INSTANCE &Element)
{
	bool bFound = false;

	QDT_DLIST<COM_INSTANCE>::ITERATOR It		= _UnderIhmElements.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd		= _UnderIhmElements.GetTail();

	while (It != ItEnd)
	{
		if (*It == Element)
		{
			_UnderIhmElements.Remove(It);
			bFound = true;
			break;
		}
		++It ;
	}

	if	(!bFound)
	{
		It		= _OverIhmElements.GetHead();
		ItEnd	= _OverIhmElements.GetTail();

		while (It != ItEnd)
		{
			if (*It == Element)
			{
				_OverIhmElements.Remove(It);
				bFound = true;
				break;
			}
			++It ;
		}
	}

	return	(bFound);
}

//-----------------------------------------------------------------------------
//	Name:		ClearIhmElements
//	Object:		
//	03-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ClearIhmElements()
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR	ItCur, ItTmp, ItEnd;

	ItCur = _UnderIhmElements.GetHead();
	ItEnd = _UnderIhmElements.GetTail();
	while ( ItCur != ItEnd )
	{
		if ( static_cast<IHM_ELEMENT_ABC *>( (*ItCur).GetCommunicator() )->GetLoadingScreenIhm() == false )
		{
			ItTmp = ItCur;
			++ItCur;
			_UnderIhmElements.Remove( ItTmp );
		}
		else
		{
			++ItCur;
		}
	}

	ItCur = _OverIhmElements.GetHead();
	ItEnd = _OverIhmElements.GetTail();
	while ( ItCur != ItEnd )
	{
		if ( static_cast<IHM_ELEMENT_ABC *>( (*ItCur).GetCommunicator() )->GetLoadingScreenIhm() == false )
		{
			ItTmp = ItCur;
			++ItCur;
			_OverIhmElements.Remove( ItTmp );
		}
		else
		{
			++ItCur;
		}
	}

#if defined(PLATFORM_PS2) && !defined(_MASTER_FINAL_BUILD) && defined(_MASTER)
	bDisplayFPS = false;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AddVideoIhmElement
//	Object:		
//	05-03-11:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::AddVideoIhmElement(const COM_INSTANCE &	Element)
{
	float			rZDepth;
	IHM_ELEMENT_ABC	*pElement;

	pElement = static_cast<IHM_ELEMENT_ABC *>(Element.GetCommunicator());
	rZDepth = pElement->GetZDepth();
	
	DISPLAY_MANAGER::Instance()->RemoveVideoIhmElement(Element);

	QDT_DLIST<COM_INSTANCE>::ITERATOR It = _VideoIhmElements.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd = _VideoIhmElements.GetTail();

	while (It != ItEnd)
	{
		pElement = static_cast<IHM_ELEMENT_ABC *>((*It).GetCommunicator());

		if (rZDepth > pElement->GetZDepth())
		{
			_VideoIhmElements.Insert(It, Element);
			return;
		}
		++It ;
	}

	_VideoIhmElements.PushTail(Element);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveVideoIhmElement
//	Object:		
//	05-03-11:	ELE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::RemoveVideoIhmElement(const COM_INSTANCE &	Element)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR It	= _VideoIhmElements.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd	= _VideoIhmElements.GetTail();

	while (It != ItEnd)
	{
		if (*It == Element)
		{
			_VideoIhmElements.Remove(It);
			return	(true);
		}
		++It ;
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		ClearVideoIhmElements
//	Object:		
//	05-03-11:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ClearVideoIhmElements()
{
	_VideoIhmElements.Clear();
}

static float	grStarty = 40;
static bool		gbDisplayText = false;
static bool		gbDumpProfile = false;

//-----------------------------------------------------------------------------
//	Name:		DisplayProfiler
//	Object:		
//	03-04-09:	VMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::DisplayProfiler( )
{
	Float32	fScreenWidth =	(Float32) RENDER_CONTEXT::GetScreenWidth();
	Float32 fScreenHeight =	(Float32) RENDER_CONTEXT::GetScreenHeight();

	Float32 fNameY = (40.f / fScreenHeight);
	Float32 fNameX = (60.f / fScreenWidth);

	/*if (gbDisplayText)*/	DisplayProfile( PROFILER::Instance()->GetRootProfile(), fNameX, fNameY );
/*
#ifndef _MASTER

#ifdef PLATFORM_PS2 // bidouille pour assombrir l'image et ainsi lire les profiles et aussi scroller le display du profiler

	bool bPadOpened = ps2IOpenPad(0, 0);
	if (bPadOpened)
	{
		if (ps2IIsButtonDown(0, 0, PS2_PADBUTTON_LUP))
			grStarty += 7;
		else if (ps2IIsButtonDown(0, 0, PS2_PADBUTTON_LDOWN))
			grStarty -= 7;
		if (ps2IIsButtonPressed(0, 0, PS2_PADBUTTON_LRIGHT))
			gbDisplayText ^= 1;
		if (ps2IIsButtonPressed(0, 0, PS2_PADBUTTON_LLEFT))
			gbDumpProfile = true;
	}

	if (grStarty < 512 && gbDisplayText)
	{
		RENDER_CONTEXT *pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
		pRenderContext->SetTexture(0, NULL);
		pRenderContext->EnableAlphaBlend(true);
		pRenderContext->SetAlphaBlendSrcFactor(RENDER_CONTEXT::BLENDFACTOR_SRCALPHA);
		pRenderContext->SetAlphaBlendDstFactor(RENDER_CONTEXT::BLENDFACTOR_INVSRCALPHA);
		pRenderContext->Render2DQuad(POINT2D(0,0), POINT2D(512,512), COLOR(196,0,0,0));

		float rStarty = grStarty;
		DisplayProfile( PROFILER::GetRootProfile(), rStarty, 10 );
	}
#else
	float rStarty = 40;
	if (gbDisplayText)	DisplayProfile( PROFILER::GetRootProfile(), rStarty, 20 );
#endif	

	if (gbDumpProfile)	
	{
		#ifdef PLATFORM_PS2
			QDT_STRING sFileName("Profile_PS2.txt");
		#else
			QDT_STRING sFileName("Profile_PC.txt");
		#endif

		QDT_STRING Buffer;
		FILES_MANAGER_ABC::FM_ERROR error;
		FILE_ABC* pFile;
		
		pFile = FILES_MANAGER_ABC::Instance()->Open(sFileName, FILE_ABC::FM_CREATE_READ_WRITE, error, true);		

		unsigned int nCurrOffset = pFile->GetSize();

		Buffer.Format("Begin of Profile...");		
		pFile->Write((void*)Buffer.GetBuffer(), nCurrOffset, Buffer.GetSize()); nCurrOffset += Buffer.GetSize();
		pFile->Write((void*)"\0\n", nCurrOffset, 2); nCurrOffset += 2;
		
		DumpProfile(pFile, nCurrOffset, PROFILER::GetRootProfile(), 0);

		Buffer.Format("End of Profile...");
		pFile->Write((void*)Buffer.GetBuffer(), nCurrOffset, Buffer.GetSize()); nCurrOffset += Buffer.GetSize();
		pFile->Write((void*)"\0\n", nCurrOffset, 2); nCurrOffset += 2;
		pFile->Write((void*)"\0\n", nCurrOffset, 2); nCurrOffset += 2;

		FILES_MANAGER_ABC::Instance()->Close(pFile);
		gbDumpProfile = false;

		RENDER_MANAGER::Instance()->GetRenderContext()->DumpLowLevelStats();		
	}

#endif*/
}


//-----------------------------------------------------------------------------
//	Name:		DumpProfile
//	Object:		
//	04-06-03:	SBE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	DISPLAY_MANAGER::DumpProfile(FILE_ABC* pFile, unsigned int & nCurrOffset, const QDT_VECTOR < int > &	IdList, int nNextx)
{
	QDT_STRING Buffer, sTmp;
	int nNbProfiles = IdList.GetSize();

	for (int i=0; i< nNbProfiles; i++)
	{
		int   nProfileId = IdList[i];

		const Profile& profile = PROFILER::Instance()->GetProfile( nProfileId );

		float rTune = profile._rFinalTune;
		float rTime = profile._rFinalTime;

		// Clip ProfileName to 24 Characters
		QDT_STRING sName = profile._sName;

		#define MAX_NAME_LEN 100

		if (profile._sName.GetSize() >= MAX_NAME_LEN )
		{
			sName.Resize( MAX_NAME_LEN );
		}

		Buffer = "";
		for (int i=0; i<=nNextx; ++i)
		{
			Buffer += " ";
		}
	
		// Display text
		if ( profile._SubProfile.GetSize() )
		{
			if ( profile._Flag & FL_OPEN )
			{
				sTmp.Format("*%s",(const char*) sName.GetBuffer() );
				Buffer += sTmp;

				pFile->Write((void*)Buffer.GetBuffer(), nCurrOffset, Buffer.GetSize()); nCurrOffset += Buffer.GetSize();
											   
				//Buffer.Format("%6.2fms (%5.2f%%) (%d calls)", rTime, rTune, profile._nFinalNbCalls);
				Buffer.Format("__%6.2fms (%d)", rTime, profile._nFinalNbCalls);


				pFile->Write((void*)Buffer.GetBuffer(), nCurrOffset, Buffer.GetSize()); nCurrOffset += Buffer.GetSize();
				pFile->Write((void*)"\0\n", nCurrOffset, 2); nCurrOffset += 2;

				// Display SubProfile
				DumpProfile(pFile, nCurrOffset, profile._SubProfile, nNextx+1);

				continue;
			}
			else
			{
				rTune = 0; rTime = 0;
				PROFILER::Instance()->GetFolderTime( nProfileId, rTune, rTime);
				sTmp.Format("+%s",(const char*)sName.GetBuffer());
				Buffer += sTmp;
			}
		} 
		else
		{
			sTmp.Format("| %s",(const char*)sName.GetBuffer());
			Buffer += sTmp;
		}

		pFile->Write((void*)Buffer.GetBuffer(), nCurrOffset, Buffer.GetSize()); nCurrOffset += Buffer.GetSize();
		
		//Buffer.Format("%6.2fms (%5.2f%%) (%d calls)", rTime, rTune, profile._nFinalNbCalls);
		Buffer.Format("__%6.2fms (%d)", rTime, profile._nFinalNbCalls);

		pFile->Write((void*)Buffer.GetBuffer(), nCurrOffset, Buffer.GetSize()); nCurrOffset += Buffer.GetSize();
		pFile->Write((void*)"\0\n", nCurrOffset, 2); nCurrOffset += 2;
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		DisplayProfile
//	Object:		
//	03-04-09:	VMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::DisplayProfile( const QDT_VECTOR<int> & IdList, Float32 fNameX, Float32 & fNameY)
{
#ifdef _PROFILER_
	Float32	fScreenWidth =	(Float32) RENDER_CONTEXT::GetScreenWidth();
	Float32	fScreenHeight =	(Float32) RENDER_CONTEXT::GetScreenHeight();

	Float32	fInfoX = 0.6f;
	Float32	fInfoY = fNameY;

	UInt32 nNbProfiles = IdList.GetSize();

	for (UInt32 nIdxProfile = 0; nIdxProfile < nNbProfiles; nIdxProfile++)
	{
		Int32   nProfileId = IdList[nIdxProfile];

		const Profile& profile = PROFILER::Instance()->GetProfile( nProfileId );

		Float32 rTune = profile._rFinalTune;
		Float32 rTime = profile._rFinalTime;

		// Clip ProfileName to 24 Characters
		QDT_STRING sName = profile._sName;

		#define MAX_NAME_LEN 100

		if (profile._sName.GetSize() >= MAX_NAME_LEN )
		{
			sName.Resize( MAX_NAME_LEN );
		}
	
		// Display text
		if ( profile._SubProfile.GetSize() )
		{
			if ( profile._Flag & FL_OPEN )
			{
				RENDER_CONTEXT::PrintDebugMessage(fNameX, fNameY, "*%s", sName.GetBuffer() );

				RENDER_CONTEXT::PrintDebugMessage(fInfoX, fInfoY, "__%6.2fms (%d)", rTime, profile._nFinalNbCalls );
			
				// Next
				fNameY += (15.f / fScreenHeight);
				Float32 fSubNameX = fNameX + (20.f / fScreenWidth);

				// Display SubProfile
				DisplayProfile( profile._SubProfile, fSubNameX, fNameY);

				fInfoY = fNameY;

				continue;
			}
			else
			{
				rTune = 0; rTime = 0;
				PROFILER::Instance()->GetFolderTime( nProfileId, rTune, rTime);
				
				RENDER_CONTEXT::PrintDebugMessage(fNameX, fNameY, "+%s", sName.GetBuffer() );

				RENDER_CONTEXT::PrintDebugMessage(fInfoX, fInfoY, "__%6.2fms (%d)", rTime, profile._nFinalNbCalls );
			}
		} 
		else
		{
			RENDER_CONTEXT::PrintDebugMessage(fNameX, fNameY, "| %s", sName.GetBuffer() );

			RENDER_CONTEXT::PrintDebugMessage(fInfoX, fInfoY, "__%6.2fms (%d)", rTime, profile._nFinalNbCalls );
		}

		// Next
		fNameY += (15.f / fScreenHeight);
		fInfoY = fNameY;
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMemStats
//	Object:		
//	06-03-22:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::UpdateMemStats(void)
{
#if defined(QDT_MEM_ENABLED) && defined(QDT_MEM_DEBUG) && defined(QDT_MEM_USE_TIME_ID)
/*	UInt32 i;
	UInt32 total = 0;
	QDT_STRING finalString = "{S:50}";
	for(i=0;i<MEM_LIBID_MAX;i++)
	{
		TIME_LOG* pTimeLog = _pMemStatsEntityTable[i].GetTimeLog();

		TIME_LOG::S_TIME_LOG_ELEMENT* pLastElement = pTimeLog->GetLastElement();

		UInt32 libUsage = pLastElement->_fValue;

		//if(libUsage)
		{
			total += libUsage;

			finalString+=_pMemStatsEntityTable[i].GetFormatedString();
			finalString+="{P}";

			_pMemStatsEntityTable[i].Display();
		}
	}

	finalString+="{S:70}{C:255:219:093}Total: ";
	finalString+=OS::GetHumanReadableSize(total);

	AddDebugText(finalString,2,2);*/

	_pMemStatsEntityTable->Display();
	_pMemStatsEntityGraphTable[0].Display();
	_pMemStatsEntityGraphTable[1].Display();

#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayMemStats
//	Object:		
//	06-03-22:	VHA - Created
//-----------------------------------------------------------------------------
Bool	DISPLAY_MANAGER::GetDisplayMemStats(void	)
{
	return _bDisplayMemStats;
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayMemStats
//	Object:		
//	06-03-22:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetDisplayMemStats(Bool	bDisplay)
{
	_bDisplayMemStats = bDisplay;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateGraphicStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::UpdateGraphicStats()
{
	if (_bDisplayGraphicStats)
	{
		RENDER_STATS & RenderStats = RENDER_CONTEXT::GetRenderStats();

		// API calls
		QDT_STRING sFinalString = "{S:50}API Calls{P}";

		sFinalString += "Number of Polygones : ";
		sFinalString += (Int32)RenderStats.GetNbrOfPolygones();
		sFinalString += "{P}";

		sFinalString += "Number of DrawPrimitive() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfDrawPrimitive();
		sFinalString += "{P}";
		
		sFinalString += "Number of VertexBufferLock() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfVertexBufferLock();
		sFinalString += "{P}";

		sFinalString += "Number of IndexBufferLock() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfIndexBufferLock();
		sFinalString += "{P}";

		sFinalString += "Number of TextureLock() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfTextureLock();
		sFinalString += "{P}";

		sFinalString += "Number of SetRenderTarget() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetRenderTarget();
		sFinalString += "{P}";

		sFinalString += "Number of SetDepthBuffer() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetDepthBuffer();
		sFinalString += "{P}";

		sFinalString += "Number of SetTexture() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetTexture();
		sFinalString += "{P}";

		sFinalString += "Number of SetVertexShader() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetVertexShader();
		sFinalString += "{P}";

		sFinalString += "Number of SetPixelShader() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetPixelShader();
		sFinalString += "{P}";

		sFinalString += "Number of SetVertexShaderConstant() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetVertexShaderConstant();
		sFinalString += "{P}";

		sFinalString += "Number of SetPixelShaderConstant() : ";
		sFinalString += (Int32)RenderStats.GetNbrOfSetPixelShaderConstant();
		sFinalString += "{P}";

		AddDebugText(sFinalString, 2, 2);

		// API Calls time
		sFinalString = "{S:50}API Calls Time{P}";
		
		sFinalString += "Time in DrawPrimitive() : ";
		sFinalString += static_cast<Float32>(RenderStats.GetDrawPrimitiveTime());
		sFinalString += "ms {P}";

		sFinalString += "Time in SetVertexShaderConstant() : ";
		sFinalString += static_cast<Float32>(RenderStats.GetSetVertexShaderConstantTime());
		sFinalString += "ms {P}";

		sFinalString += "Time in SetPixelShaderConstant() : ";
		sFinalString += static_cast<Float32>(RenderStats.GetSetPixelShaderConstantTime());
		sFinalString += "ms {P}";
		
		sFinalString += "Time in SetRenderTarget() : ";
		sFinalString += static_cast<Float32>(RenderStats.GetSetRenderTargetTime());
		sFinalString += "ms {P}";

		sFinalString += "Time in Present() : ";
		sFinalString += static_cast<Float32>(RenderStats.GetPresentTime());
		sFinalString += "ms {P}";

		AddDebugText(sFinalString, 32, 2);

		// Ressources
		sFinalString = "{S:50}Ressources{P}";

		sFinalString += "Number of Vertex Buffers : ";
		sFinalString += (Int32)RenderStats.GetNbrOfVertexBuffers();
		sFinalString += "{P}";

		sFinalString += "Number of Index Buffers : ";
		sFinalString += (Int32)RenderStats.GetNbrOfIndexBuffers();
		sFinalString += "{P}";

		sFinalString += "Number of Render Targets : ";
		sFinalString += (Int32)RenderStats.GetNbrOfRenderTargets();
		sFinalString += "{P}";

		sFinalString += "Number of Cube Render Targets : ";
		sFinalString += (Int32)RenderStats.GetNbrOfCubeRenderTargets();
		sFinalString += "{P}";

		sFinalString += "Number of Textures : ";
		sFinalString += (Int32)RenderStats.GetNbrOfTextures();
		sFinalString += "{P}";

		sFinalString += "Number of Cube Textures : ";
		sFinalString += (Int32)RenderStats.GetNbrOfCubeTextures();
		sFinalString += "{P}";

		sFinalString += "Number of Volume Textures : ";
		sFinalString += (Int32)RenderStats.GetNbrOfVolumeTextures();
		sFinalString += "{P}";

		sFinalString += "Number of Vertex Shaders : ";
		sFinalString += (Int32)RenderStats.GetNbrOfVertexShaders();
		sFinalString += "{P}";

		sFinalString += "Number of Pixel Shaders : ";
		sFinalString += (Int32)RenderStats.GetNbrOfPixelShaders();
		sFinalString += "{P}";

		AddDebugText(sFinalString, 65, 2);

		RENDER_CONTEXT::ClearRenderStats();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayGraphicStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
Bool	DISPLAY_MANAGER::GetDisplayGraphicStats()
{
	return (_bDisplayGraphicStats);
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayGraphicStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetDisplayGraphicStats(Bool	bDisplay)
{
	_bDisplayGraphicStats = bDisplay;
}

//-----------------------------------------------------------------------------
//	Name:		EnableDynamicShadows
//	Object:		
//	02-08-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::IsDynamicShadowsEnabled() const
{
	return	(_bDynamicShadowsEnabled);
}

//-----------------------------------------------------------------------------
//	Name:		IsFogEnable
//	Object:		
//	02-08-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::IsFogEnabled() const
{
	return (true); 
}

//-----------------------------------------------------------------------------
//	Name:		GetSingletonDescriptor
//	Object:		
//	02-07-19:	ONA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	DISPLAY_MANAGER::GetSingletonDescriptor()
{
	return	(SINGLETON_DESCRIPTOR::FindSingDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_MANAGER));
}

//-----------------------------------------------------------------------------
//	Name:		CreateSingletonDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	DISPLAY_MANAGER::CreateSingletonDescriptor()
{
	return ( QDT_NEW SINGLETON_DESCRIPTOR(	"DISPLAY_MANAGER",
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_MANAGER,
										QDT_COMMENT("850C4F44-9839-3686-A01A-85EB9CD46F04"),
										sizeof(DISPLAY_MANAGER), 
										COM_Init,
										QDT_COMMENT("The Display Manager singleton"),
										NULL,
										ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL ));
}

//-----------------------------------------------------------------------------
//	Name:		AddSoftBody
//	Object:		
//	04-03-12:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::AddSoftBody(const COM_INSTANCE &	SoftBodyInst)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR	It = _dlSoftBodies.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR	ItEnd = _dlSoftBodies.GetTail();

	while	(It != ItEnd)
	{
		if	((*It) == SoftBodyInst)
		{
			return;
		}

		++It;
	}

	_dlSoftBodies.PushTail(SoftBodyInst);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSoftBody
//	Object:		
//	04-03-12:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::RemoveSoftBody(const COM_INSTANCE &	SoftBodyInst)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR	It = _dlSoftBodies.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR	ItEnd = _dlSoftBodies.GetTail();

	while	(It != ItEnd)
	{
		if	((*It) == SoftBodyInst)
		{
			_dlSoftBodies.Remove(It);
			return;
		}

		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetSoftBodies
//	Object:		
//	04-03-12:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ResetSoftBodies()
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR	It = _dlSoftBodies.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR	ItEnd = _dlSoftBodies.GetTail();

	while	(It != ItEnd)
	{
		MESH	*pMesh = static_cast<MESH *>((*It).GetCommunicator());

//		pMesh->SetPause(true);
//		pMesh->CheckAnim();

		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddBlendShape
//	Object:		
//	04-10-28:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::AddBlendShape(const COM_INSTANCE &	BlendShapeInst)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR	It = _dlBlendShapes.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR	ItEnd = _dlBlendShapes.GetTail();

	while	(It != ItEnd)
	{
		if	((*It) == BlendShapeInst)
		{
			return;
		}

		++It;
	}

	_dlBlendShapes.PushTail(BlendShapeInst);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveBlendShape
//	Object:		
//	04-10-28:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::RemoveBlendShape(const COM_INSTANCE &	BlendShapeInst)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR	It = _dlBlendShapes.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR	ItEnd = _dlBlendShapes.GetTail();

	while	(It != ItEnd)
	{
		if	((*It) == BlendShapeInst)
		{
			_dlBlendShapes.Remove(It);
			return;
		}

		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetBlendShapes
//	Object:		
//	04-10-28:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ResetBlendShapes()
{
	QDT_NOT_IMPLEMENTED();
/*
	QDT_DLIST <COM_INSTANCE>::ITERATOR	It = _dlBlendShapes.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR	ItNext, ItEnd = _dlBlendShapes.GetTail();

	while	(It != ItEnd)
	{
		if	((*It).HasHandle() && (*It).IsLoaded())
		{
			BLEND_SHAPE	*pBlendShape = static_cast<BLEND_SHAPE *>((*It).GetCommunicator());

			pBlendShape->SetValid(false);

			++It;
		}
		else
		{
			ItNext = It + 1;
			_dlBlendShapes.Remove(It);
			It = ItNext;
		}
	}*/
}
/*
//-----------------------------------------------------------------------------
//	Name:		PushIHMGeometry
//	Object:		
//	04-05-27:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::PushIHMGeometry(const GEOMETRY_ABC *	pGeometry)
{
	QDT_PAIR<const GEOMETRY_ABC *, int>	Pair;

	Pair.SetFirst(pGeometry);
	Pair.SetSecond(0);

	_dlIHMGeometries.PushTail(Pair);
}
*/
//-----------------------------------------------------------------------------
//	Name:		CheckIHMGeometries
//	Object:		
//	04-05-27:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::CheckIHMGeometries()
{
	QDT_NOT_IMPLEMENTED();
/*
	if	(_dlIHMGeometries.GetSize() == 0) return;

	QDT_DLIST < QDT_PAIR<const GEOMETRY_ABC *, int> >::ITERATOR	ItNext, It = _dlIHMGeometries.GetHead();
	QDT_DLIST < QDT_PAIR<const GEOMETRY_ABC *, int> >::ITERATOR	ItEnd = _dlIHMGeometries.GetTail();

	int									nValue;
	QDT_PAIR<const GEOMETRY_ABC *, int>	*pPair;

	while	(It != ItEnd)
	{
		ItNext = It + 1;

		pPair = &(*It);

		nValue = pPair->GetSecond();
		if	(nValue > 0)
		{
			delete	(pPair->GetFirst());
			_dlIHMGeometries.Remove(It);
		}
		else
		{
			pPair->SetSecond(nValue + 1);
		}

		It = ItNext;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		ClearIHMGeometries
//	Object:		
//	04-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ClearIHMGeometries()
{
/*
	QDT_DLIST < QDT_PAIR<const GEOMETRY_ABC *, int> >::ITERATOR	It; 
	QDT_DLIST < QDT_PAIR<const GEOMETRY_ABC *, int> >::ITERATOR	ItEnd; 

	It		= _dlIHMGeometries.GetHead(); 
	ItEnd	= _dlIHMGeometries.GetTail();

	while (It != ItEnd)
	{
		delete (*It).GetFirst();
		++It;
	}

	_dlIHMGeometries.Clear();*/
}

//-----------------------------------------------------------------------------
//	Name:		COM_Init
//	Object:		
//	02-07-19:	ONA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::COM_Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR*	pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// GetDisplayTriggerVolumes
	pMD = METHOD_DESCRIPTOR::Allocate( "GetDisplayTriggerVolumes", QDT_COMMENT("Display trigger or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->SetReturn( QDT_COMMENT("bIsDisplayed"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_GET_DISPLAY_TRIGGER_VOLUMES, pMD, CM_GetDisplayTriggerVolumes, NULL );

	// SetDisplayTriggerVolumes
	pMD = METHOD_DESCRIPTOR::Allocate( "SetDisplayTriggerVolumes", QDT_COMMENT("Display trigger or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter( QDT_COMMENT("bDisplay"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_SET_DISPLAY_TRIGGER_VOLUMES, pMD, CM_SetDisplayTriggerVolumes, NULL );

	// GetDisplayPhysics
	pMD = METHOD_DESCRIPTOR::Allocate( "GetDisplayPhysics", QDT_COMMENT("Physics displayed or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->SetReturn( QDT_COMMENT("bIsDisplayed"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_GET_DISPLAY_PHYSICS, pMD, CM_GetDisplayPhysics, NULL );

	// SetDisplayPhysics
	pMD = METHOD_DESCRIPTOR::Allocate( "SetDisplayPhysics", QDT_COMMENT("Display physics or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter( QDT_COMMENT("bDisplay"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_SET_DISPLAY_PHYSICS, pMD, CM_SetDisplayPhysics, NULL );

	// GetHideSkinClusters
	pMD = METHOD_DESCRIPTOR::Allocate( "GetHideSkinClusters", QDT_COMMENT("Skin clusters hidden or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->SetReturn( QDT_COMMENT("bIsHidden"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_GET_HIDE_SKIN_CLUSTERS, pMD, CM_GetHideSkinClusters, NULL );

	// SetHideSkinClusters
	pMD = METHOD_DESCRIPTOR::Allocate( "SetHideSkinClusters", QDT_COMMENT("Hide Skin clusters or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter( QDT_COMMENT("bHide"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_SET_HIDE_SKIN_CLUSTERS, pMD, CM_SetHideSkinClusters, NULL );
	
	// GetDisplayPrimitives
	pMD = METHOD_DESCRIPTOR::Allocate( "GetDisplayPrimitives", QDT_COMMENT("Primitives displayed or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->SetReturn( QDT_COMMENT("bIsDisplayed"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_GET_DISPLAY_PRIMITIVES, pMD, CM_GetDisplayPrimitives, NULL );

	// SetDisplayPrimitives
	pMD = METHOD_DESCRIPTOR::Allocate( "SetDisplayPrimitives", QDT_COMMENT("Display Primitives or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter( QDT_COMMENT("bDisplay"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_SET_DISPLAY_PRIMITIVES, pMD, CM_SetDisplayPrimitives, NULL );

	// GetDisplayPrimitives
	pMD = METHOD_DESCRIPTOR::Allocate( "GetDisplayMemStats", QDT_COMMENT("MemStats displayed or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->SetReturn( QDT_COMMENT("bIsDisplayed"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_GET_DISPLAY_MEMSTATS, pMD, CM_GetDisplayMemStats, NULL );

	// SetDisplayPrimitives
	pMD = METHOD_DESCRIPTOR::Allocate( "SetDisplayMemStats", QDT_COMMENT("Display MemStats or not"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter( QDT_COMMENT("bDisplay"), QDT_COMMENT("Value"),  TYPE_BOOL::GetDescriptor() );
	pCD->AddCommand( CMD_SET_DISPLAY_MEMSTATS, pMD, CM_SetDisplayMemStats, NULL );

	//	Now register the class as a global variable
	GLOBAL_VARIABLE::AddVariable(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_MANAGER, "DISPLAY_MANAGER", pCD, GetSingletonDescriptor()->GetSingletonPointer());
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-07-19:	ONA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::EM_Construct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	DISPLAY_MANAGER *	pObject;
	pObject = static_cast<DISPLAY_MANAGER *>(pO);
	QDT_NEW_PLACED(pObject, DISPLAY_MANAGER)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-07-19:	ONA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::EM_Destruct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	DISPLAY_MANAGER *	pObject;
	pObject = static_cast<DISPLAY_MANAGER*> (pO);
	pObject->DISPLAY_MANAGER::~DISPLAY_MANAGER();
}

//-----------------------------------------------------------------------------
//	Name:		CM_GetDisplayTriggerVolumes
//	Object:		
//	05-12-09:	VPI - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_GetDisplayTriggerVolumes(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_GET_DISPLAY_TRIGGER_VOLUMES);

	bool bValue = DISPLAY_MANAGER::Instance()->GetDisplayTriggerVolumes();
	Stack.Push(bValue);

	return (COM_STACK::ONE_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetDisplayTriggerVolumes
//	Object:		
//	05-12-09:	VPI - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_SetDisplayTriggerVolumes(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_SET_DISPLAY_TRIGGER_VOLUMES);

	bool bValue;
	if (Stack.GetParameter(0, bValue))
	{
		DISPLAY_MANAGER::Instance()->SetDisplayTriggerVolumes(bValue);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_GetDisplayPhysics
//	Object:		
//	05-12-15:	ELE - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_GetDisplayPhysics(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_GET_DISPLAY_PHYSICS);

	bool bValue = DISPLAY_MANAGER::Instance()->GetDisplayPhysics();
	Stack.Push(bValue);

	return (COM_STACK::ONE_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetDisplayPhysics
//	Object:		
//	05-12-15:	ELE - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_SetDisplayPhysics(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_SET_DISPLAY_PHYSICS);

	bool bValue;
	if (Stack.GetParameter(0, bValue))
	{
		DISPLAY_MANAGER::Instance()->SetDisplayPhysics(bValue);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_GetHideSkinClusters
//	Object:		
//	05-12-19:	ELE - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_GetHideSkinClusters(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_GET_HIDE_SKIN_CLUSTERS);

	bool bValue = DISPLAY_MANAGER::Instance()->GetHideSkinClusters();
	Stack.Push(bValue);

	return (COM_STACK::ONE_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetHideSkinClusters
//	Object:		
//	05-12-19:	ELE - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_SetHideSkinClusters(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_SET_HIDE_SKIN_CLUSTERS);

	bool bValue;
	if (Stack.GetParameter(0, bValue))
	{
		DISPLAY_MANAGER::Instance()->SetHideSkinClusters(bValue);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_GetDisplayPrimitives
//	Object:		
//	06-03-15:	BFA - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_GetDisplayPrimitives(COM_PARAM P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_GET_DISPLAY_PRIMITIVES);

	bool bValue = DISPLAY_MANAGER::Instance()->GetDisplayPrimitives();
	Stack.Push(bValue);

	return (COM_STACK::ONE_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetDisplayPrimitives
//	Object:		
//	06-03-15:	BFA - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_SetDisplayPrimitives(COM_PARAM P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_SET_DISPLAY_PRIMITIVES);

	bool bValue;
	if (Stack.GetParameter(0, bValue))
	{
		DISPLAY_MANAGER::Instance()->SetDisplayPrimitives(bValue);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_GetDisplayMemStats
//	Object:		
//	06-03-22:	VHA - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_GetDisplayMemStats(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_GET_DISPLAY_MEMSTATS);

	bool bValue = DISPLAY_MANAGER::Instance()->GetDisplayMemStats();
	Stack.Push(bValue);

	return (COM_STACK::ONE_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetDisplayMemStats
//	Object:		
//	06-03-22:	VHA - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::CM_SetDisplayMemStats(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_SET_DISPLAY_MEMSTATS);

	bool bValue;
	if (Stack.GetParameter(0, bValue))
	{
		DISPLAY_MANAGER::Instance()->SetDisplayMemStats(bValue);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentViewport
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT *	DISPLAY_MANAGER::GetCurrentViewport() const								
{ 
	return (static_cast<VIEWPORT*>(_CurrentViewport.GetCommunicator())); 
}

//-----------------------------------------------------------------------------
//	Name:		GetViewportList
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT_LIST *	DISPLAY_MANAGER::GetViewportList() const									
{ 
	if (_ViewportList.HasHandle())
	{
		return (static_cast<VIEWPORT_LIST*>(_ViewportList.GetCommunicator())); 
	}

	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowSizeRequested
//	Object:		
//	02-04-05:	DAC - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ChangeWindowSizeRequested(int	nSizeX,
												   int	nSizeY)
{
	_nSizeX = nSizeX;
	_nSizeY = nSizeY;
	_bNeedToRestartEngine = true;
}

//-----------------------------------------------------------------------------
//	Name:		FullScreenRequested
//	Object:		
//	02-04-05:	DAC - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::FullScreenRequested(bool b)
{
	if	(b)
	{
		_bWindowedRequested = false;
		_bFullScreenRequested = true;
	}
	else
	{
		_bWindowedRequested = true;
		_bFullScreenRequested = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushLight
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::PushLight(const S_LIGHT_SOURCE & LightSource)
{
	_aLights[_uiNbrLights++] = LightSource;
}

//-----------------------------------------------------------------------------
//	Name:		GetLight
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
const S_LIGHT_SOURCE & DISPLAY_MANAGER::GetLight(UInt32 i)
{
	QDT_ASSERT(i < _uiNbrLights);
	return (_aLights[i]);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetNbrLights
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 DISPLAY_MANAGER::GetNbrLights() const
{
	return (_uiNbrLights);
}

//-----------------------------------------------------------------------------
//	Name:		ClearLights
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::ClearLights()
{
	_uiNbrLights = 0;
}

//-----------------------------------------------------------------------------
//	Name:		SetAmbientColor
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetAmbientColor(const COLOR_FLOAT & cAmbient)
{
	_cAmbient = cAmbient;
}

//-----------------------------------------------------------------------------
//	Name:		GetAmbientColor
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
const COLOR_FLOAT & DISPLAY_MANAGER::GetAmbientColor() const
{
	return (_cAmbient);
}

//-----------------------------------------------------------------------------
//	Name:		EnableSkinning
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::EnableSkinning(Bool	bSkinning)
{
	_bSkinning = bSkinning;
}

//-----------------------------------------------------------------------------
//	Name:		IsSkinningEnabled
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
Bool	DISPLAY_MANAGER::IsSkinningEnabled() const
{
	return	(_bSkinning);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrSkinningMatrices
//	Object:		
//	06-01-20:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetNbrSkinningMatrices(UInt32	nNbrSkinningMatrices)
{
	_nNbrSkinningMatrices = nNbrSkinningMatrices;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrSkinningMatrices
//	Object:		
//	06-01-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	DISPLAY_MANAGER::GetNbrSkinningMatrices()
{
	return	(_nNbrSkinningMatrices);
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinningMatrix
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetSkinningMatrix(UInt32			uiIndex,
										   const MATRIX &	mat)
{
	_SkinningMatrices[uiIndex] = mat;
}

//-----------------------------------------------------------------------------
//	Name:		GetSkinningMatrix
//	Object:		
//	05-11-24:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	DISPLAY_MANAGER::GetSkinningMatrix(UInt32	uiIndex) const
{
	return	(_SkinningMatrices[uiIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		SetWorldMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetWorldMatrix(const MATRIX & mat)
{
	_mWorld = mat;
	_bNeedUpdateWorldViewProj = true;
	_bNeedUpdateInvWorld = true;	
}

//-----------------------------------------------------------------------------
//	Name:		SetViewMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetViewMatrix(const MATRIX & mat)
{
	_mView = mat;
	_bNeedUpdateWorldViewProj = true;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetProjMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetProjMatrix(const MATRIX & mat)
{
	_mProj = mat;
	_bNeedUpdateWorldViewProj = true;
}

//-----------------------------------------------------------------------------
//	Name:		GetWorldMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX & DISPLAY_MANAGER::GetWorldMatrix() const
{
	return (_mWorld);
}

//-----------------------------------------------------------------------------
//	Name:		GetInvWorldMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX & DISPLAY_MANAGER::GetInvWorldMatrix()
{
	if (_bNeedUpdateInvWorld)
	{
		_mInvWorld.InverseAffine(_mWorld);
		_bNeedUpdateInvWorld = false;
	}

	return (_mInvWorld);
}

//-----------------------------------------------------------------------------
//	Name:		GetViewMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX & DISPLAY_MANAGER::GetViewMatrix() const
{
	return (_mView);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetWorldMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX & DISPLAY_MANAGER::GetProjMatrix() const
{
	return (_mProj);
}

//-----------------------------------------------------------------------------
//	Name:		GetWorldViewProjMatrix
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX & DISPLAY_MANAGER::GetWorldViewProjMatrix()
{
	if (_bNeedUpdateWorldViewProj)
	{
		MATRIX Temp;
		Temp.Mul(_mWorld, _mView);
		_mWorldViewProj.Mul(Temp, _mProj);
		_bNeedUpdateWorldViewProj = false;
	}

	return (_mWorldViewProj);
}

//-----------------------------------------------------------------------------
//	Name:		GetWorldViewMatrix
//	Object:		
//	06-03-03:	SBE - Created
//-----------------------------------------------------------------------------
const MATRIX & DISPLAY_MANAGER::GetWorldViewMatrix()
{
	if (_bNeedUpdateWorldView)
	{
		_mWorldView.Mul(_mWorld, _mView);
		_bNeedUpdateWorldView = false;
	}

	return (_mWorldView);
}

//-----------------------------------------------------------------------------
//	Name:		GetLineShader
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	DISPLAY_MANAGER::GetLineShader()
{
	return (_ciLineShader);
}


//-----------------------------------------------------------------------------
//	Name:		GetDefaultShader
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE & DISPLAY_MANAGER::GetDefaultShader()
{
	return (_ciDefaultShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetErrorShader
//	Object:		
//	06-02-09:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	DISPLAY_MANAGER::GetErrorShader()
{
	return (_ciErrorShader);
}


//-----------------------------------------------------------------------------
//	Name:		GetHardWareShadowRenderTargets
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
SHADOW_RENDERTARGETS *	DISPLAY_MANAGER::GetHardWareShadowRenderTargets()
{
	for (UInt32 i = 0; i < MAX_SM_RENDER_TARGETS_AVAILABLE; ++i)
	{
		if (_aHardWareShadowMapsAvailable[i].pLight == NULL)
		{
			return &_aHardWareShadowMapsAvailable[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		GetBasicShadowRenderTargets
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
SHADOW_RENDERTARGETS *	DISPLAY_MANAGER::GetBasicShadowRenderTargets()
{
	for (UInt32 i = 0; i < MAX_SM_RENDER_TARGETS_AVAILABLE; ++i)
	{
		if (_aBasicShadowMapsAvailable[i].pLight == NULL)
		{
			return &_aBasicShadowMapsAvailable[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		EnableShadowReplacement
//	Object:		
//	06-02-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::EnableShaderReplacement(Bool	b)
{
	_bEnableShaderReplacement = b;
}

//-----------------------------------------------------------------------------
//	Name:		IsDepthPassEnabled
//	Object:		
//	06-02-21:	RMA - Created
//-----------------------------------------------------------------------------
Bool	DISPLAY_MANAGER::IsShaderReplacementEnabled() const
{
	return (_bEnableShaderReplacement);
}

//-----------------------------------------------------------------------------
//	Name:		GetReplacementShader
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE & DISPLAY_MANAGER::GetReplacementShader()
{
	return (_ciShaderReplacement);
}

//-----------------------------------------------------------------------------
//	Name:		GetReplacementShader
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetReplacementShader(const COM_INSTANCE & ci)
{
	_ciShaderReplacement = ci;
}

//-----------------------------------------------------------------------------
//	Name:		GetReplacementShader
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE & DISPLAY_MANAGER::GetShadowShader()
{
	return (_ciShadowShader);
}

//-----------------------------------------------------------------------------
//	Name:		RegisterFlow
//	Object:		
//	06-03-30:	PDE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::RegisterFlow(FLOW *	flow)
{
	_RegisteredFlows.PushTail(flow);
}

//-----------------------------------------------------------------------------
//	Name:		UnRegisterFlow
//	Object:		
//	06-03-30:	PDE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::UnRegisterFlow(FLOW *	flow)
{
	_RegisteredFlows.Remove(flow);
}

//-----------------------------------------------------------------------------
//	Name:		UnRegisterFlow
//	Object:		
//	06-03-30:	PDE - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::UpdateAllFlows()
{
	for (QDT_DLIST<FLOW*>::ITERATOR it = _RegisteredFlows.GetHead(); it != _RegisteredFlows.GetTail(); it++)
	{
		(*it)->Update();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
