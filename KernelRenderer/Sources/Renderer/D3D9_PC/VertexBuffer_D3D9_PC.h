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
//	CLASS:	VERTEX_BUFFER_D3D9_PC
//	The VERTEX_BUFFER_D3D9_PC class implements ...
//
//	05-08-31:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_VERTEX_BUFFER_D3D9_PC_H__
#define __KRENDERER_RENDERER_VERTEX_BUFFER_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer\IVertexBuffer)

namespace	QDT
{
	namespace	KRENDERER
	{
		class VERTEX_BUFFER_D3D9_PC : public I_VERTEX_BUFFER
		{
		public:

			VERTEX_BUFFER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pRenderContext, UInt32 vertexCount, UInt32 vertexStride, Bool bDynamic);
			~VERTEX_BUFFER_D3D9_PC();

			// Derivation from I_VERTEX_BUFFER
			void*	Lock();
			void	Unlock();
			UInt32	GetVertexCount() const;
			UInt32	GetVertexStride() const;
			Bool	IsDynamic() const;
			Bool	IsLocked() const;
			//

			IDirect3DVertexBuffer9 * GetD3DVertexBuffer() const;

		private:

			void CreateVertexBuffer();
			void ReleaseVertexBuffer();

			RENDER_CONTEXT_D3D9_PC * 			_pRenderContext;
			IDirect3DVertexBuffer9 *			_pD3DVertexBuffer;
			UInt32								_nVertexCount;
			UInt32								_nVertexStride;
			Bool								_bLocked;
			Bool								_bDynamic;
		};
	}
}

#endif // __KRENDERER_RENDERER_VERTEX_BUFFER_D3D9_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================