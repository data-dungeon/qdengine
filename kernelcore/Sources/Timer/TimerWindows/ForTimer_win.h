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
// CLASS: FOR_TIMER
//
//
//	01-01-16:	RPA - Created
//*****************************************************************************


//=============================================================================
//  Name: FOR_TIMER
//	01-01-16:	RPA - Created
//=============================================================================

#ifndef __KCORE_TIMER_TIMERWINDOWS_FORTIMER_WIN_H__
#define __KCORE_TIMER_TIMERWINDOWS_FORTIMER_WIN_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(TIMER_CALLBACK)

namespace	QDT
{
	namespace KCORE
	{
		class FOR_TIMER
		{
		public:
			explicit FOR_TIMER();
					~FOR_TIMER();
			int Init();
			void Close();
			void Loop();
			void Check();
			void AddCallback(int p, void (*pF)(IntPtr), IntPtr nParam, IntPtr nRef, int nNbLoop);
			void DelCallback(IntPtr ref);
			void PauseTimer(IntPtr ref);
			void ResumeTimer(IntPtr ref);
			void PauseTimers();
			void ResumeTimers();
		private:
			int	_nFlags;
			enum
			{
				INIT = 1 << 0,
			};
			unsigned int	_nTimerID;
			TIMER_CALLBACK *_pTimerCallback;
		};
	}
}

#endif //__KCORE_TIMER_TIMERWINDOWS_FORTIMER_WIN_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
