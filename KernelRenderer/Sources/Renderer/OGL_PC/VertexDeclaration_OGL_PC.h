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
//	CLASS:	VERTEX_DECLARATION_OGL_PC
//	The VERTEX_DECLARATION_OGL_PC class implements ...
//
//	05-08-31:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_VERTEX_DECLARATION_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_VERTEX_DECLARATION_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer\IVertexDeclaration)
#include INCL_KRENDERER(Renderer\SVertexElement)

#define MAX_NUM_ELEMENTS 16

namespace	QDT
{
	namespace	KRENDERER
	{
		class VERTEX_DECLARATION_OGL_PC : public I_VERTEX_DECLARATION
		{
		public:

			VERTEX_DECLARATION_OGL_PC(S_VERTEX_ELEMENT* pElements, UInt32 nNumElements);
			~VERTEX_DECLARATION_OGL_PC();

			// Derivation from I_VERTEX_DECLARATION
			void GetDeclaration(S_VERTEX_ELEMENT* & pElements, UInt32& nNumElements);
			//

		private:

			UInt32				_nNumElements;
			S_VERTEX_ELEMENT	_aElements[MAX_NUM_ELEMENTS];
		};
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_VERTEX_DECLARATION_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
