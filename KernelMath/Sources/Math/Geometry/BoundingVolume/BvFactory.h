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
//	CLASS:	BV_FACTORY
//	The BV_FACTORY class implements ...
//
//	02-07-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_BOUNDINGVOLUME_BVFACTORY_H__
#define __KMATH_MATH_BOUNDINGVOLUME_BVFACTORY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingVolume)

namespace QDT
{
	namespace KMATH
	{				
		class BV_FACTORY
		{
		public:

			// Singleton
			static BV_FACTORY* Instance();

			// Creation
			BOUNDING_VOLUME* Create(const BOUNDING_VOLUME::TYPE type);
			BOUNDING_VOLUME* Create(const BOUNDING_VOLUME *pBV);
			
			// Stream
			BOUNDING_VOLUME* Read(IO_STREAM &Stream);
			void			 Write(IO_STREAM &Stream, BOUNDING_VOLUME *pBV);
		};
	}
}

#endif //__KMATH_MATH_BOUNDINGVOLUME_BVFACTORY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
