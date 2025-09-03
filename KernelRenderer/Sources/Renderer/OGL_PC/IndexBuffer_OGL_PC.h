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
//	CLASS:	INDEX_BUFFER_OGL_PC
//	The INDEX_BUFFER_OGL_PC class implements ...
//
//	05-09-15:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_INDEX_BUFFER_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_INDEX_BUFFER_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer\IIndexBuffer)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#include INCL_KCORE(Includes_PC\Includes_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	INDEX_BUFFER_OGL_PC : public I_INDEX_BUFFER
		{
		public:
			INDEX_BUFFER_OGL_PC(UInt32 nNbrIndices);
			~INDEX_BUFFER_OGL_PC();

			// Derivation from I_VERTEX_BUFFER
			void *			Lock();
			void			Unlock();
			UInt32			GetNbrIndices() const;
			Bool			IsDynamic() const;
			Bool			IsLocked() const;
			//

			GLuint			GetOGLIndexBuffer() const;

		private:
			GLuint		_nOGLIndexBuffer;
			UInt32		_nNbrIndices;
			Bool		_bLocked;
			Bool		_bDynamic;
		};
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_INDEX_BUFFER_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
