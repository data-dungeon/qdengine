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
//	CLASS:	PAD_MANAGER_X360
//	The PAD_MANAGER_X360 class implements ...
//
//	06-29-06:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_X360_PADMANAGER_X360_H__
#define __KINPUT_INPUT_X360_PADMANAGER_X360_H__

#define MAX_PAD 4

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define QDT_INCLUDE_X360_PAD
#include INCL_KCORE(Includes_X360/Includes_X360)
#include INCL_KINPUT(Input/X360/PadX360)

namespace QDT
{
	namespace KINPUT
	{
		class	PAD_MANAGER_X360	
		{
			public:
				static PAD_MANAGER_X360 * GetInstance() { return &sPadManager; }
				PAD_X360 *		GetPad(UInt32 i);
				void			ReleasePad(UInt32 i);
				void			Init();
				void			Close();
				void			Update();
				UInt32			GetNbPadMax() const { return MAX_PAD; }
			private:
				static PAD_MANAGER_X360 sPadManager;

				PAD_X360		_Pads[MAX_PAD];
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
