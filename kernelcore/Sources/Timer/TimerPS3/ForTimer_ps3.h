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
		};
	}
}




//=============================================================================
// CODE ENDS HERE
//=============================================================================
