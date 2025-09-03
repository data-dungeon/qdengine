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
//	CLASS:	I_VERTEX_DECLARATION
//	The I_VERTEX_DECLARATION class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_I_VERTEX_DECLARATION_H__
#define __KRENDERER_RENDERER_I_VERTEX_DECLARATION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(S_VERTEX_ELEMENT)

namespace	QDT
{
	namespace	KRENDERER
	{
		class I_VERTEX_DECLARATION
		{
		public:

			virtual ~I_VERTEX_DECLARATION() {}
			virtual void GetDeclaration(S_VERTEX_ELEMENT * &pElements, UInt32& nNumElements) = 0;
		};
	}
}

#endif // __KRENDERER_RENDERER_I_VERTEX_DECLARATION_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
