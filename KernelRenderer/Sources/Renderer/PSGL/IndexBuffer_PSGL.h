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
//	CLASS:	INDEX_BUFFER_PSGL
//	The INDEX_BUFFER_PSGL class implements ...
//
//	05-09-15:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_INDEX_BUFFER_PSGL_H__
#define __KRENDERER_INDEX_BUFFER_PSGL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer\IIndexBuffer)
#define QDT_INCLUDE_PS3_PSGL
#include INCL_KCORE(Includes_PS3\Includes_PS3)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	INDEX_BUFFER_PSGL : public I_INDEX_BUFFER
		{
		public:
			INDEX_BUFFER_PSGL(UInt32 nNbrIndices);
			~INDEX_BUFFER_PSGL();

			// Derivation from I_VERTEX_BUFFER
			void *			Lock();
			void			Unlock();
			UInt32			GetNbrIndices() const;
			Bool			IsDynamic() const;
			Bool			IsLocked() const;
			//

			GLuint			GetPSGLIndexBuffer() const;

		private:
			GLuint		_nPSGLIndexBuffer;
			UInt32		_nNbrIndices;
			Bool		_bLocked;
			Bool		_bDynamic;
		};
	}
}

#endif // __KRENDERER_INDEX_BUFFER_PSGL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
