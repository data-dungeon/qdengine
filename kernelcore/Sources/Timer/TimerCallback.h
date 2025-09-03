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
//	CLASS:	TIMER_CALLBACK
//	The TIMER_CALLBACK class implements ...
//
//	02-08-13:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TIMER_TIMERCALLBACK_H__
#define __KCORE_TIMER_TIMERCALLBACK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class TIMER_CALLBACK
		{
		public:
						TIMER_CALLBACK();
			explicit	TIMER_CALLBACK(const TIMER_CALLBACK & C);
						TIMER_CALLBACK(int p, void (*pF)(IntPtr), IntPtr pa, IntPtr Ref, int NbLoop);
						~TIMER_CALLBACK();

			TIMER_CALLBACK &	operator=(const TIMER_CALLBACK & C);
			void	Copy(const TIMER_CALLBACK & C);

			bool			Check();
			TIMER_CALLBACK*	GetNext();
			void			SetNext(TIMER_CALLBACK *pTC);
			int				GetModule();
			void			Pause();
			void			Resume();
			
		private:
			enum
			{
				PAUSE = 1<<0,
			};
			int	_Periode;
			int _TimeCount;
			int	_nNbLoop;
			void (*_pFnct)(IntPtr);
			int	_Param;
			int _Module;
			TIMER_CALLBACK *_pNext;
			int _Flags;
		};
	}
}

#endif //__KCORE_TIMER_TIMERCALLBACK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
