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
//	CLASS:	FACE
//	The FACE class implements ...
//
//	02-12-13:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_FACE_FACE_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_FACE_FACE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class FACE
		{
		protected :
			int		_Index[3];
			VECTOR	_Normal;

		public:

			FACE();
			FACE(const FACE & C);
			FACE(const int i0, const int i1, const int i2);
			~FACE();

			FACE& operator=(const FACE &C);
			_INLINE_ int operator [] (const unsigned int i) const;
			_INLINE_ int& operator [] (const unsigned int i);

			void Copy(const FACE &C);

			_INLINE_ const VECTOR &	GetNormal() const;
			_INLINE_ void			ComputeNormal(const VECTOR & V1, const VECTOR & V2);

			_INLINE_ int			GetIndex(int i) const;

			/** Stream */
			friend IO_STREAM &		operator << (IO_STREAM & Stream, const FACE & Face)
									{
										Face.Write(Stream);
										return Stream;
									}
			friend IO_STREAM &		operator >> (IO_STREAM & Stream, FACE & Face)
									{
										Face.Read(Stream);
										return Stream;
									}

			void					Read(IO_STREAM & Stream);
			void					Write(IO_STREAM & Stream) const;
		};

		#ifndef _DEBUG 
		#include "Face.inl"
		#endif
	}
}

#endif //__KMATH_MATH_SHAPES_SHAPES2D_FACE_FACE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
