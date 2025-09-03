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
// CLASS: DISPLAY_MANAGER
//
//
//00-11-13 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: DISPLAY_MANAGER
//00-11-13 JVA Created: 
//=============================================================================

#ifndef __DISPLAY_DISPLAY_MANAGER_H__
#define __DISPLAY_DISPLAY_MANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(RAY_CAST_PARAMETERS)
DECL_3DENGINE(VIEWPORT)
DECL_3DENGINE(FLOW)
DECL_3DENGINE(VIEWPORT_LIST)
DECL_3DENGINE(CAMERA)
DECL_3DENGINE(NODE)
DECL_3DENGINE(MESH_DATA)
DECL_KCOM(SINGLETON_DESCRIPTOR)
DECL_KMATH(MATRIX)
DECL_KCORE(FILE_ABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementText)

#include	INCL_KCOM(CommunicatorType/ComInstance)
#include	INCL_KCOM(CommunicatorType/Communicator)
#include	INCL_KCORE(Template/QDT_DList)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/LightABC)
#include	INCL_3DENGINE(Display/Statistics/Statistics)
#include	INCL_3DENGINE(Display/Statistics/StatsEntityGraph)
#include	INCL_3DENGINE(Display/Statistics/StatsEntityText)
#include	INCL_3DENGINE(Display/DisplayParameters/DisplayParameters)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/Color/ColorFloat)
#include	INCL_KRENDERER(Renderer/SVertexElement)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IVertexBuffer)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Flow)


#define SORT_OBJECT_BY_MATERIALS
#define QD_MAX_LIGHT_COUNT 64
#define QD_MAX_SKINNING_MATRIX	80
#define MAX_SM_RENDER_TARGETS_AVAILABLE	4
#define QD_MAX_QUAD_IN_VB 1024
#define QD_MAX_LINE_IN_VB 64

namespace QDT
{
	namespace M3D
	{
		/** For shadow */
		class SHADOW_RENDERTARGETS // render targets reserved for shadow
		{
		public:
			I_RENDER_TARGET *	pRenderTarget;
			I_RENDER_TARGET *	pDepthBuffer;
			LIGHT_ABC *			pLight;
		};

		class DISPLAY_MANAGER
		{
		public :
			enum
			{
				CMD_GET_DISPLAY_TRIGGER_VOLUMES = CMD_USER_START,
				CMD_SET_DISPLAY_TRIGGER_VOLUMES,
				CMD_GET_DISPLAY_PHYSICS,
				CMD_SET_DISPLAY_PHYSICS,
				CMD_GET_HIDE_SKIN_CLUSTERS,
				CMD_SET_HIDE_SKIN_CLUSTERS,
				CMD_GET_DISPLAY_PRIMITIVES,
				CMD_SET_DISPLAY_PRIMITIVES,
				CMD_GET_DISPLAY_MEMSTATS,
				CMD_SET_DISPLAY_MEMSTATS,
				CMD_DISPLAY_MANAGER_LAST,
			};

			struct S_2D_VERTEX
			{
				Float32 x;
				Float32 y;
				Float32 z;
				Float32 w;
				Float32 u;
				Float32 v;
				UInt32	color;
			};

			struct S_LINE_2D_VERTEX
			{
				Float32 x;
				Float32 y;
				Float32 z;
				Float32 w;
				UInt32	color;
			};

			// TODO: retirer en master
			struct S_DEBUG_MESSAGE
			{
				QDT_STRING	_Message;
				Float32		_fLeft;
				Float32		_fTop;
			};

			/** For singleton */
			static DISPLAY_MANAGER *				Instance();
			static void								Release();
			static bool								IsInstancied();

#if defined (PLATFORM_PC)
			void									SwapFullscreenMode();
			void									ChangeWindowSize(UInt32 w, UInt32 h);
			void									ChangeWindowPos(UInt32 w, UInt32 h);
#endif	

			// Very important for maya!
			void									ReinitRenderTargets();

			void									FillCubeMap(SCENE_GRAPH * pSceneGraph, const VECTOR & Position, I_RENDER_TARGET * pRenderTarget, I_RENDER_TARGET *  pDepthBuffer);

			/** Init */
			void									Init();
			void									Init2D();
			void									Close();
			void									Close2D();
			
			/** Display */
			void									Display();
			void									Display2D();
			static void								SetDisplay2D(const bool bFlag);

			// Generic shaders
			const COM_INSTANCE &					GetLineShader();
			const COM_INSTANCE &					GetDefaultShader();
			const COM_INSTANCE &					GetErrorShader();
			const COM_INSTANCE &					GetShadowShader();
			const COM_INSTANCE &					GetReplacementShader();
			void									SetReplacementShader(const COM_INSTANCE & ci);

			// Enable depth pass
			void									EnableShaderReplacement(Bool b);
			Bool									IsShaderReplacementEnabled() const;

			// Matrices
			void									SetWorldMatrix(const MATRIX & mat);
			void									SetViewMatrix(const MATRIX & mat);
			void									SetProjMatrix(const MATRIX & mat);
			const MATRIX &							GetWorldMatrix() const;
			const MATRIX &							GetInvWorldMatrix();
			const MATRIX &							GetViewMatrix() const;
			const MATRIX &							GetProjMatrix() const;
			const MATRIX &							GetWorldViewProjMatrix();
			const MATRIX &							GetWorldViewMatrix();

			// Lighting
			void									PushLight(const S_LIGHT_SOURCE & LightSource);
			const S_LIGHT_SOURCE &					GetLight(UInt32 i);
			UInt32									GetNbrLights() const;
			void									ClearLights();
			void									SetAmbientColor(const COLOR_FLOAT & cAmbient);
			const COLOR_FLOAT &						GetAmbientColor() const;

			// Skinning
			void									EnableSkinning(Bool bSkinning);
			Bool									IsSkinningEnabled() const;
			void									SetNbrSkinningMatrices(UInt32 nNbrSkinningMatrices);
			UInt32									GetNbrSkinningMatrices();
			void									SetSkinningMatrix(UInt32 uiIndex, const MATRIX & mat);
			const MATRIX &							GetSkinningMatrix(UInt32 uiIndex) const;

			// Display various informations
			_INLINE_	bool						GetDisplayTriggerVolumes() const;
			_INLINE_	void						SetDisplayTriggerVolumes(bool b);
			_INLINE_	bool						GetDisplayPhysics() const;
			_INLINE_	void						SetDisplayPhysics(bool b);
			_INLINE_	bool						GetHideSkinClusters() const;
			_INLINE_	void						SetHideSkinClusters(bool b);
			_INLINE_	bool						GetDisplaySkeletons() const;
			_INLINE_	void						SetDisplaySkeletons(bool b);
			_INLINE_	bool						GetDisplayPrimitives() const;
			_INLINE_	void						SetDisplayPrimitives(bool b);

			/** Accessors */
		#ifndef _MASTER_FINAL_BUILD
			_INLINE_ STATISTICS &					GetStats();
		#endif
			_INLINE_ int							GetWindowSizeX() const;
			_INLINE_ int							GetWindowSizeY() const;

			/** Fullscreen */
			void									ChangeWindowSizeRequested(int nSizeX, int nSizeY);
			void									FullScreenRequested(bool b);

			/** Raycast */
			void									RayCast(int nScreenX, int nScreenY, RAY_CAST_PARAMETERS & RayCastParameters);
			void									RayCastFromViewport(int nScreenX, int nScreenY, RAY_CAST_PARAMETERS & RayCastParameters, const COM_INSTANCE &ViewportInst);
			
			bool									IsDynamicShadowsEnabled() const;
			bool									IsFogEnabled() const;

			_INLINE_ Bool							SupportHardwareShadowMap() const;
			SHADOW_RENDERTARGETS *					GetHardWareShadowRenderTargets();
			SHADOW_RENDERTARGETS *					GetBasicShadowRenderTargets();

			/** Viewport */
					VIEWPORT*						GetCurrentViewport() const;
			_INLINE_ void							SetCurrentViewportInst(const COM_INSTANCE &Viewport)	{ _CurrentViewport = Viewport; }
			_INLINE_ const COM_INSTANCE &			GetCurrentViewportInst() const							{ return (_CurrentViewport); }
					VIEWPORT_LIST*					GetViewportList() const;
			_INLINE_ const COM_INSTANCE&			GetViewportListInst() const								{ return (_ViewportList); }
			_INLINE_ void							SetViewportListInst(const COM_INSTANCE &ViewportList)	{ _ViewportList = ViewportList; }

			void PushQuad(POINT2D* points, POINT2D* uvs, COLOR*	colors);
			void PushQuadArray(UInt32 uiSize, POINT2D** points, POINT2D** uvs, COLOR**	colors);
			void FlushQuads();

			void PushLine(POINT2D* points, COLOR*	colors);
			void FlushLines();

			/** Ihm elements */
					void							AddIhmElement(const COM_INSTANCE &Element, bool bUnderInterface = true);
					bool							RemoveIhmElement(const COM_INSTANCE &Element);
					void							ClearIhmElements();

					void							AddVideoIhmElement(const COM_INSTANCE &Element);
					bool							RemoveVideoIhmElement(const COM_INSTANCE &Element);
					void							ClearVideoIhmElements();

					void							AddSoftBody(const COM_INSTANCE & SoftBodyInst);
					void							RemoveSoftBody(const COM_INSTANCE & SoftBodyInst);
					void							ResetSoftBodies();
					void							AddBlendShape(const COM_INSTANCE & BlendShapeInst);
					void							RemoveBlendShape(const COM_INSTANCE & BlendShapeInst);
					void							ResetBlendShapes();

					void							CheckIHMGeometries();
					void							ClearIHMGeometries();

			/** Active camera in rendering */
			_INLINE_ CAMERA	*						GetActiveCamera() const;
			_INLINE_ void							SetActiveCamera(CAMERA *pCamera);

			/** For fog */
			_INLINE_ void							NeedTestNearFogPlane(bool b);
			_INLINE_ bool							IsTestNearFogPlaneNeeded()const;
			_INLINE_ const PLANE &					GetNearFogPlane() const;

			_INLINE_ NODE *							GetCameraNode();
			_INLINE_ DISPLAY_PARAMETERS &			GetDisplayParameters();

			_INLINE_ void							SetDisplayParameters(const DISPLAY_PARAMETERS & DisplayParameters);

			/** For Profiler */
					void							DisplayProfiler();					
					void							DisplayProfile( const QDT_VECTOR<int> & IdList, Float32  fNameX, Float32 & fNameY);
					void							DumpProfile(FILE_ABC* pFile, unsigned int & nCurrOffset, const QDT_VECTOR<int> & IdList, int nNextx);

			/** Mem Stats */
					void							UpdateMemStats();
					Bool							GetDisplayMemStats(void);
					void							SetDisplayMemStats(Bool bDisplay);

					void							UpdateGraphicStats();
					Bool							GetDisplayGraphicStats(void);
					void							SetDisplayGraphicStats(Bool bDisplay);
					
					void							Update2D();
					 
			_INLINE_ void							ManageVideo(bool b);

			static SINGLETON_DESCRIPTOR *			GetSingletonDescriptor();
			static SINGLETON_DESCRIPTOR *			CreateSingletonDescriptor();

		#ifdef PLATFORM_PC
			void									SetCursorProperties(const COM_INSTANCE & ciTexture, unsigned int nXHotSpot, unsigned int nYHotSpot);
		#endif

			_INLINE_ void							EnableWideScreenMode(bool bActive);
			_INLINE_ bool							IsWideScreenModeEnabled()const;


			MESH*									GetCurrentMesh()			  {return _pCurrentMesh;};
			void									SetCurrentMesh(MESH* m)		  {_pCurrentMesh = m;};

			COM_INSTANCE							GetCurrentMeshData()		   {return _pCurrentMeshData;};
			void									SetCurrentMeshData(COM_INSTANCE m) {_pCurrentMeshData = m;};

			void									RegisterFlow(FLOW* flow);
			void									UnRegisterFlow(FLOW* flow);
			void									UpdateAllFlows();

			// TODO: retirer en master
			void									AddDebugText(QDT_STRING text, Float32 fLeft, Float32 fTop);
		private :

													DISPLAY_MANAGER();
													~DISPLAY_MANAGER();



													// TODO: Retirer tout ca en master.
			COM_INSTANCE							_pDebugFontCI;
			COM_INSTANCE							_pDebugIHM_ElementTextCI;
			IHM_ELEMENT_TEXT*						_pDebugIHM_ElementText;
			QDT_VECTOR <S_DEBUG_MESSAGE>			_pDebugMessageList;

			/** Render modes */
			bool									_bChangeWindowSizeRequested;
			int										_nSizeX, _nSizeY;
			bool									_bFullScreenRequested;
			bool									_bWindowedRequested;
			unsigned int							_nFullSizeX, _nFullSizeY;

	        // Flags
	        enum DEBUG_RENDER_STATE
	        {		
		        DRS_DISPLAY_TRIGGER_VOLUMES	= 1 << 0,
		        DRS_DISPLAY_PHYSICS			= 1 << 1,
				DRS_DISPLAY_SKELETONS		= 1 << 2,
		        DRS_HIDE_SKIN_CLUSTERS		= 1 << 3,
				DRS_DISPLAY_PRIMITIVES		= 1 << 4,
	        };

	        QDT_FLAGS								_DebugRenderFlags;

			//bool									_bFullScreen;

			/** Current viewport */
			COM_INSTANCE							_CurrentViewport;
			COM_INSTANCE							_ViewportList;

			/** List of pointers to IHM elements */
			QDT_DLIST <COM_INSTANCE>				_UnderIhmElements;
			QDT_DLIST <COM_INSTANCE>				_OverIhmElements;
			QDT_DLIST <COM_INSTANCE>				_VideoIhmElements;

			QDT_DLIST<FLOW*>						_RegisteredFlows;

			/** Contains informations of statistics */
			STATISTICS								_Statistics;

			QDT_DLIST <COM_INSTANCE>				_dlSoftBodies;
			QDT_DLIST <COM_INSTANCE>				_dlBlendShapes;

			/** For shadow */
			SHADOW_RENDERTARGETS 					_aHardWareShadowMapsAvailable[MAX_SM_RENDER_TARGETS_AVAILABLE];
			SHADOW_RENDERTARGETS 					_aBasicShadowMapsAvailable[MAX_SM_RENDER_TARGETS_AVAILABLE];
			SHADOW_RENDERTARGETS 					_aCubeHardWareShadowMapsAvailable[MAX_SM_RENDER_TARGETS_AVAILABLE];
			SHADOW_RENDERTARGETS 					_aCubeBasicShadowMapsAvailable[MAX_SM_RENDER_TARGETS_AVAILABLE];

			/** For fog */
			bool									_bNeedTestNearFogPlane;
			PLANE									_NearFogPlane;

			DISPLAY_PARAMETERS						_DisplayParameters;

			CAMERA *								_pActiveCamera;

			bool									_bFrontBufferEffectsEnabled;
			bool									_bIsDisplaying;
			bool									_bManageVideo;

		#if (!defined(_MASTER_FINAL_BUILD) && (defined(PLATFORM_PC)||defined(PLATFORM_PS2)))
			bool									_bVideoCapture;
			unsigned int							_nVideoFileCounter;
		#endif

		#ifdef PLATFORM_PC
			// Cursor
			COM_INSTANCE							_CursorTexture;
			unsigned int							_nHotSpotX;
			unsigned int							_nHotSpotY;
		#endif

			// mem stats
			Bool									_bDisplayMemStats;
			Bool									_bDisplayGraphicStats;

			STATS_ENTITY_TEXT*						_pMemStatsEntityTable;
			STATS_ENTITY_GRAPH*						_pMemStatsEntityGraphTable;

			static bool								_bDisplay2D;

			// quad
			UInt32									_Vector2D_CurrentVB;
			UInt32									_Vector2D_PositionInVB[2];
			I_VERTEX_BUFFER*						_Vector2D_VB[2];
			I_VERTEX_DECLARATION*					_Vector2D_Decl;

			// line
			UInt32									_Line_Vector2D_CurrentVB;
			UInt32									_Line_Vector2D_PositionInVB[2];
			I_VERTEX_BUFFER*						_Line_Vector2D_VB[2];
			I_VERTEX_DECLARATION*					_Line_Vector2D_Decl;


			COM_INSTANCE							_ciLineShader;
			COM_INSTANCE							_ciDefaultShader;			// Default shader for unsupported card!	
			COM_INSTANCE							_ciErrorShader;				// Error shader which will replace NULL shader
			COM_INSTANCE							_ciShadowShader;			// shadow shader used to render shadow map
			
			Bool									_bEnableShaderReplacement;  // This mechanism is used to make a full pass with a 
			COM_INSTANCE							_ciShaderReplacement;		// fixed shader, as for rendering a shadow map for example

			bool									_bNeedUpdateWorldViewProj;
			bool									_bNeedUpdateWorldView;
			bool									_bNeedUpdateInvWorld;
			MATRIX									_mWorld;
			MATRIX									_mInvWorld;
			MATRIX									_mView;
			MATRIX									_mProj;
			MATRIX									_mWorldViewProj;
			MATRIX									_mWorldView;

			S_LIGHT_SOURCE							_aLights[QD_MAX_LIGHT_COUNT];
			UInt32									_uiNbrLights;
			COLOR_FLOAT								_cAmbient;

			// Skinning
			Bool									_bSkinning;
			UInt32									_nNbrSkinningMatrices;
			MATRIX									_SkinningMatrices[QD_MAX_SKINNING_MATRIX];

			// Shadow
			Bool									_bDynamicShadowsEnabled;


			COM_INSTANCE							_pCurrentMeshData;
			MESH*									_pCurrentMesh;

			//Grphic options
			static bool								_bNeedToRestartEngine;
			//static bool								_bApplyChanges;

			static void		COM_Init(CLASS_DESCRIPTOR * const pCD);

			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			static int		CM_GetDisplayTriggerVolumes(COM_PARAM P);
			static int		CM_SetDisplayTriggerVolumes(COM_PARAM P);
			static int		CM_GetDisplayPhysics(COM_PARAM P);
			static int		CM_SetDisplayPhysics(COM_PARAM P);
	        static int		CM_GetHideSkinClusters(COM_PARAM P);
	        static int		CM_SetHideSkinClusters(COM_PARAM P);
			static int		CM_GetDisplayPrimitives(COM_PARAM P);
			static int		CM_SetDisplayPrimitives(COM_PARAM P);
			static int		CM_GetDisplayMemStats(COM_PARAM P);
			static int		CM_SetDisplayMemStats(COM_PARAM P);
		};
	}
}

#include	INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KRENDERER(Renderer/RenderContext)

#ifndef _DEBUG
#include "DisplayManager.inl"
#endif

#endif // __DISPLAY_DISPLAY_MANAGER_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
