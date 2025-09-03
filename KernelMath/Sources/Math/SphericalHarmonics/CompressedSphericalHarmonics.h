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
//	CLASS:	SPHERICAL_HARMONICS
//	The VECTOR_2 class implements ...
//
//	03-03-10:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_COMPRESSED_SPHERICAL_HARMONICS_H__
#define __KMATH_MATH_COMPRESSED_SPHERICAL_HARMONICS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(SPHERICAL_HARMONICS)

#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

namespace	QDT
{
	namespace KMATH
	{
		class	COMPRESSED_SPHERICAL_HARMONICS
		{
		public:
			COMPRESSED_SPHERICAL_HARMONICS() {};
			~COMPRESSED_SPHERICAL_HARMONICS() {};

			void Compress(SPHERICAL_HARMONICS& sh, Float32 ClampVal);
			void DeCompress(SPHERICAL_HARMONICS& sh, Float32 ClampVal);

			// Streams
			void				Read(IO_STREAM &s);
			void				Write(IO_STREAM &s) const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const COMPRESSED_SPHERICAL_HARMONICS &m)
			{
				m.Write(s);
				return s;
			}

			friend IO_STREAM&	operator >>(IO_STREAM &s, COMPRESSED_SPHERICAL_HARMONICS &m)
			{
				m.Read(s);
				return s;
			}

		private:

			Int16 _redComponents[9];
			Int16 _greenComponents[9];
			Int16 _blueComponents[9];
		};
	}
}

#endif //__KMATH_MATH_COMPRESSED_SPHERICAL_HARMONICS_H__
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
