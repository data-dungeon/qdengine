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
//	CLASS:	RENDER_CONTEXT
//	The RENDER_CONTEXT class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_S_VERTEX_ELEMENT_H__
#define __KRENDERER_RENDERER_S_VERTEX_ELEMENT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace	QDT
{
	namespace	KRENDERER
	{
		class S_VERTEX_ELEMENT
		{
		public:
			Int8 _nStream;
			Int8 _nOffset;
			Int8 _nType;
			Int8 _nUsage;
		};
	}
}

#endif // __KRENDERER_RENDERER_S_VERTEX_ELEMENT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
