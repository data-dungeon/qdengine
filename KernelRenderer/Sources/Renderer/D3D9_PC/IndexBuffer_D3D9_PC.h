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
//	CLASS:	INDEX_BUFFER_D3D9_PC
//	The INDEX_BUFFER_D3D9_PC class implements ...
//
//	05-09-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_INDEX_BUFFER_D3D9_PC_H__
#define __KRENDERER_RENDERER_INDEX_BUFFER_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/IIndexBuffer)

namespace	QDT
{
	namespace	KRENDERER
	{
		class INDEX_BUFFER_D3D9_PC : public I_INDEX_BUFFER
		{
		public:

			INDEX_BUFFER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pContext, UInt32 nNbrIndices);
			~INDEX_BUFFER_D3D9_PC();

			// Derivation from I_INDEX_BUFFER
			void*			Lock();
			void			Unlock();
			UInt32			GetNbrIndices() const;
			Bool			IsDynamic() const;
			Bool			IsLocked() const;
			//

			IDirect3DIndexBuffer9 * GetD3DIndexBuffer() const;

		private:

			RENDER_CONTEXT_D3D9_PC *	_pRenderContext;
			IDirect3DIndexBuffer9 *		_pD3DIndexBuffer;
			UInt32						_nNbrIndices;
			Bool						_bLocked;
			Bool						_bDynamic;
		};
	}
}

#endif // __KRENDERER_RENDERER_INDEX_BUFFER_D3D9_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================