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
//	CLASS:	VERTEX_DECLARATION_PSGL
//	The VERTEX_DECLARATION_PSGL class implements ...
//
//	05-08-31:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_VERTEX_DECLARATION_PSGL_H__
#define __KRENDERER_VERTEX_DECLARATION_PSGL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define MAX_NUM_ELEMENTS 16

#include INCL_KRENDERER(Renderer\IVertexDeclaration)
#include INCL_KRENDERER(Renderer\SVertexElement)

namespace	QDT
{
	namespace	KRENDERER
	{
		class VERTEX_DECLARATION_PSGL : public I_VERTEX_DECLARATION
		{
		public:

			VERTEX_DECLARATION_PSGL(S_VERTEX_ELEMENT* pElements, UInt32 nNumElements);
			~VERTEX_DECLARATION_PSGL();

			// Derivation from I_VERTEX_DECLARATION
			void GetDeclaration(S_VERTEX_ELEMENT* & pElements, UInt32& nNumElements);
			//

		private:

			UInt32							_nNumElements;
			S_VERTEX_ELEMENT				_aElements[MAX_NUM_ELEMENTS];
		};
	}
}

#endif // __KRENDERER_VERTEX_DECLARATION_PSGL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
