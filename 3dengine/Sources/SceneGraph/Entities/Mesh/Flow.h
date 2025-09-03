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
//	CLASS:	FLOWGRID
//
//	05-11-09:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_MESH_FLOW_H__
#define __M3D_SCENEGRAPH_MESH_FLOW_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define MAX_DROPS 50

DECL_3DENGINE(FLOW_DATA)
DECL_3DENGINE(ENGINE_TEXTURE_FLOW)
#include	INCL_KRENDERER(Renderer/IRenderTarget)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_VERTEX_DECLARATION)

namespace QDT
{
	namespace M3D
	{
		class FLOW
		{
		
		public:
			FLOW(ENGINE_TEXTURE_FLOW* texFlow);
			~FLOW();

			void Update();

			I_TEXTURE* GetTexture(){return _pRenderTarget->GetTexture();}

			void SetFlowData(FLOW_DATA* fd) {_pFlowData = fd;};

		private:


			void UpdateParticles();

			Float32 _fLastUpdate;
			Float32 _fLastSubstract;


			bool _bNeedsInit;

			void SpawnDrop(FLOW_DATA* _pFlowData, UInt32 area);


			struct DROP
			{
				Float32 x;
				Float32 y;
				Float32 sx;
				Float32 sy;
				Float32 life;
			};

			DROP _pDrops[MAX_DROPS];

			Int32 _iLastAlive;

			DROP* AllocDrop();
			void ReleaseDrop(Int32 index);

			Float32 _fCurMaxTrailHeight;

			I_RENDER_TARGET*  _pRenderTarget;
			FLOW_DATA*           _pFlowData;
			ENGINE_TEXTURE_FLOW* _pTexFlow;


			// static stuff
			static UInt32     _uiRefcount;
			static I_TEXTURE* _pGaussianDrop;

			struct FLOW_VERTEX
			{
				Float32 pos[3];
				Float32 uv[2];
				UInt32  color;
			};

			static I_VERTEX_SHADER*	_pFlowVS;
			static I_PIXEL_SHADER *	_pDropsPS;
			static I_PIXEL_SHADER *	_pQuadPS;

			static UInt32			_uiUsesDropsBuffer;
			static I_VERTEX_BUFFER* _pDropsBuffer[4];
			static I_VERTEX_BUFFER* _pQuadBuffer;
			static I_VERTEX_DECLARATION* _pVertexFormat;
		};
	}
}

#endif // __M3D_SCENEGRAPH_MESH_FLOW_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

