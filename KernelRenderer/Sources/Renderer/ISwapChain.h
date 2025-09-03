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
//	CLASS:	I_SWAP_CHAIN
//	The I_SWAP_CHAIN class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_I_SWAP_CHAIN_H__
#define __KRENDERER_RENDERER_I_SWAP_CHAIN_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_RENDER_TARGET)

namespace	QDT
{
	namespace	KRENDERER
	{
		class I_SWAP_CHAIN
		{
		public:

			virtual ~I_SWAP_CHAIN() {}
			virtual UInt32 GetWindowWidth() const = 0;
			virtual UInt32 GetWindowHeight() const = 0;
			virtual UInt32 GetWindowPosX() const = 0;
			virtual UInt32 GetWindowPosY() const = 0;
			virtual Bool IsDisplayWindowActive() const = 0;
		};
	}
}

#endif // __KRENDERER_RENDERER_I_SWAP_CHAIN_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
