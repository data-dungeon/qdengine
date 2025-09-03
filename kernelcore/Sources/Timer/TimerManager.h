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
// CLASS: TIMER_MANAGER
//
//
//00-11-22 FKE Created: 
//*****************************************************************************


//=============================================================================
// Name: TIMER_MANAGER
//00-11-22 FKE Created: 
//=============================================================================

#ifndef __KCORE_TIMER_TIMERMANAGER_H__
#define __KCORE_TIMER_TIMERMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class FOR_TIMER;

		class TIMER_MANAGER
		{
		public:
			explicit TIMER_MANAGER();
			virtual ~TIMER_MANAGER();
			int Init();
			void Close();
			void Loop();
			void Check();
			void AddCallback(int p, void (*pF)(IntPtr), IntPtr Param, IntPtr Ref, int nNbLoop = -1);
			void DelCallback(IntPtr ref);
			void PauseTimer(IntPtr ref);
			void ResumeTimer(IntPtr ref);
			void PauseTimers();
			void ResumeTimers();
			
			static TIMER_MANAGER * Instance();
			static void Suicide();
			static bool IsInstancied();
			
		private:
			FOR_TIMER*	_pForTimer;

		public:	
			static TIMER_MANAGER *_pInstance;
			static int	_bDestroyedOnce;
		};
	}
}

#endif //__KCORE_TIMER_TIMERMANAGER_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
