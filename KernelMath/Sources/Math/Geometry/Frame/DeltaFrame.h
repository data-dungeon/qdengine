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
//	CLASS:	DELTA_FRAME
//	The DELTA_FRAME class implements ...
//
//	02-06-10:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_FRAME_DELTAFRAME_H__
#define __KMATH_MATH_FRAME_DELTAFRAME_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class DELTA_FRAME
		{
		protected :
			VECTOR _v[2];
			
		public :
			/** */
			DELTA_FRAME();
			/** */
			virtual ~DELTA_FRAME();

			/** */
			VECTOR operator[] (const unsigned int i) const;
			/** */
			VECTOR& operator[] (const unsigned int i);

		};
	}
}

#ifndef _DEBUG
#include "DeltaFrame.inl"
#endif

#endif //__KMATH_MATH_FRAME_DELTAFRAME_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
