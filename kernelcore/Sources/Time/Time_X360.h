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
// CLASS: TIME
//
//
// 00-11-22 FKE Created
// 02-02-27 RMA Redone
//*****************************************************************************

//=============================================================================
// Name: TIME
//00-11-22 FKE Created: 
//=============================================================================

// #define USE_RTDSC

#ifndef __KCORE_TIME_TIME_X360_H__
#define __KCORE_TIME_TIME_X360_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Time/Time_ABC)

namespace QDT
{
	namespace KCORE
	{
		class TIME_X360 : public TIME
		{
		public:

			/** Accessors */
			virtual Float64				GetSystemTime() const;	// return system time in ms

			
			/** Call it once per frame */
			virtual void				UpdateSystemTime();
			virtual UInt64				GetUniversalTime() const;

										TIME_X360();
										~TIME_X360();
			
		private:

			TIME_X360 &					operator = (const TIME_X360 & Time);

			Int64						_nTimeStartOffset;
			Int64						_nProcSpeed;
		};
	}
}

#endif //__KCORE_TIME_TIME_PC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
