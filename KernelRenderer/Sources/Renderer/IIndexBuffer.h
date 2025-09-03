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
//	CLASS:	I_INDEX_BUFFER
//	The I_INDEX_BUFFER class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_I_INDEX_BUFFER_H__
#define __KRENDERER_RENDERER_I_INDEX_BUFFER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace	QDT
{
	namespace	KRENDERER
	{
		class I_INDEX_BUFFER
		{
		public:

			virtual				   ~I_INDEX_BUFFER() {}

			virtual	void *			Lock() = 0;
			virtual	void			Unlock() = 0;
			virtual UInt32			GetNbrIndices() const = 0;
		};
	}
}

#endif // __KRENDERER_RENDERER_I_INDEX_BUFFER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
