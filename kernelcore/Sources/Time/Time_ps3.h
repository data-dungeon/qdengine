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
// CLASS: TIME_PS3
//
//
// 05-11-15 VMA Merge with imer Lib
//*****************************************************************************

//=============================================================================
// Name: TIME_PS3
//05-11-15 VMA Created: 
//=============================================================================
#ifndef __KCORE_TIME_TIME_PS3_H__
#define __KCORE_TIME_TIME_PS3_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif
#include INCL_KCORE(Time/Time_ABC)

namespace QDT
{
	namespace KCORE
	{
		class TIME_PS3 : public TIME
		{
		public:
										TIME_PS3();
										~TIME_PS3();

			virtual Float64				GetSystemTime() const; // in ms
			virtual void				UpdateSystemTime();
			virtual UInt64				GetUniversalTime() const;

		private:
			float						_fTimeBase;
		};
	}
}

#endif //__KCORE_TIME_TIME_PS3_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
