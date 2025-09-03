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
// CLASS: VECTOR_PACKED
//
//
//00-11-10 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: VECTOR_PACKED
//00-11-10 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES1D_VECTOR_VECTORPACKED_H__
#define __KMATH_MATH_SHAPES_SHAPES1D_VECTOR_VECTORPACKED_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(VECTOR)

namespace QDT
{
	namespace KMATH
	{

		class	VECTOR_PACKED
		{
		public:


			_INLINE_ explicit	VECTOR_PACKED();
			
			_INLINE_			VECTOR_PACKED(const VECTOR_PACKED &V);
			_INLINE_			VECTOR_PACKED(char cVx, char cVy, char cVz);
			_INLINE_ explicit	VECTOR_PACKED(const VECTOR &V);
			
			_INLINE_ void			Build(const VECTOR_PACKED &V);
			_INLINE_ void			Build(char cVx, char cVy, char cVz);
			_INLINE_ void			Build(const VECTOR &V);

			_INLINE_ void			Add(VECTOR_PACKED &V1, VECTOR_PACKED &V2);
			_INLINE_ void			Add(VECTOR_PACKED &V);
			_INLINE_ void			Sub(VECTOR_PACKED &V1, VECTOR_PACKED &V2);
			_INLINE_ void			Sub(VECTOR_PACKED &V);
			_INLINE_ void			Neg(VECTOR_PACKED &V);
			_INLINE_ void			Neg();

			_INLINE_ void			GetXYZ(int &xx, int &yy, int &zz) const ;
			

			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s)const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const VECTOR_PACKED &m)
			{
				m.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, VECTOR_PACKED &m)
			{
				m.Read(s);
				return s;
			}

		private:
			char			_cX, _cY, _cZ;

		};
	}
}

#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

#ifndef _DEBUG
#include "VectorPacked.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES1D_VECTOR_VECTORPACKED_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
