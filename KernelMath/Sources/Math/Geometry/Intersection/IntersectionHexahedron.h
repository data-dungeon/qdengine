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
//	CLASS:	INTERSECTION_HEXAHEDRON
//	The INTERSECTION_HEXAHEDRON class implements ...
//
//	01-10-09:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_INTERSECTION_INTERSECTIONHEXAHEDRON_H__
#define __KMATH_MATH_INTERSECTION_INTERSECTIONHEXAHEDRON_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(HEXAHEDRON)
DECL_KMATH(VECTOR)

namespace QDT
{
	namespace KMATH
	{
		class	INTERSECTION_HEXAHEDRON
		{
		public:

			static bool	TestIntersectionHexahedronPoint( const HEXAHEDRON &Hexahedron, const VECTOR& Point);

		private:

			INTERSECTION_HEXAHEDRON();
		~INTERSECTION_HEXAHEDRON();
		};
	}
}

#endif //__KMATH_MATH_INTERSECTION_INTERSECTIONHEXAHEDRON_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

