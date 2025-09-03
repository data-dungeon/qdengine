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
//	CLASS:	PAD_MANAGER_PS3
//	The PAD_MANAGER_PS3 class implements ...
//
//	05-10-05:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_PS3_PADMANAGER_PS3_H__
#define __KINPUT_INPUT_PS3_PADMANAGER_PS3_H__

#define MAX_PAD 1

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define QDT_INCLUDE_PS3_PAD
#include INCL_KCORE(Includes_PS3/Includes_PS3)
#include INCL_KCORE(tools/QdtFlags)

#include INCL_KINPUT(Input/PS3/Pad_PS3)

namespace QDT
{
	namespace KINPUT
	{
		class	PAD_MANAGER_PS3	
		{
			public:
				static PAD_MANAGER_PS3 * GetInstance() { return &sPadManager; }
				PAD_PS3 *		GetPad(UInt32 i);
				void			ReleasePad(UInt32 i);
				void			Init();
				void			Close();
				void			Update();
				UInt32		GetNbPadMax() const { return MAX_PAD; }
				bool			HasDipSwitchChanged(Int32	nNoSwitch);
			private:
				static PAD_MANAGER_PS3 sPadManager;

				PAD_PS3		_Pads[MAX_PAD];
				CellPadInfo	_PadInfo;
				QDT_CHAR_FLAGS 	_OldDIP;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
