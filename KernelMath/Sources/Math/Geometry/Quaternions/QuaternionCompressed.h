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
//	CLASS:	QUATERNION_COMPRESSED
//	The QUATERNION_COMPRESSED class implements ...
//
//	04-01-22:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_QUATERNION_QUATERNIONCOMPRESSED_H__
#define __KMATH_MATH_QUATERNION_QUATERNIONCOMPRESSED_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(QUATERNION)

namespace QDT
{
	namespace KMATH
	{
		class	QUATERNION_COMPRESSED
		{
		public:

			// Constructor & destructor
								QUATERNION_COMPRESSED();
							~QUATERNION_COMPRESSED();

			bool				operator == (const QUATERNION_COMPRESSED & Q) const;
			bool				operator != (const QUATERNION_COMPRESSED & Q) const;

			void				Compress(const QUATERNION & Q);
			_INLINE_ void		Uncompress(QUATERNION & Q) const;

		private:

			unsigned short		_nX;
			unsigned short		_nY;
			unsigned short		_nZ;
			unsigned short		_nW;
		};

	}
}

#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)

#ifndef _DEBUG
#include "QuaternionCompressed.inl"
#endif

#endif //__KMATH_MATH_QUATERNION_QUATERNIONCOMPRESSED_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
