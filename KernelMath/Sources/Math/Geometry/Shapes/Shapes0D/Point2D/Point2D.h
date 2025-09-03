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
// CLASS: POINT_2D
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES0D_POINT2D_POINT2D_H__
#define __KMATH_MATH_SHAPES_SHAPES0D_POINT2D_POINT2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(POINT2D_PACKED)
DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KMATH
	{
		struct POINT2D_INT16
		{
			unsigned short	x, y;
		};

		class	POINT2D
		{
		public:

			/**	Constructors & destructor */
			_INLINE_ explicit	POINT2D(const bool bFlagInit = true);
			_INLINE_			POINT2D(const POINT2D & P);
			_INLINE_			POINT2D(const Float32 rX, const Float32 rY);
			_INLINE_ explicit	POINT2D(const POINT2D_PACKED & P);
			_INLINE_		   ~POINT2D();

			/** Operators */
			_INLINE_ POINT2D&	operator =  (const POINT2D& Point);
			_INLINE_ bool		operator == (const POINT2D & Point) const;
			_INLINE_ bool		operator != (const POINT2D & Point) const;
			_INLINE_ POINT2D	operator -  (const POINT2D & Point) const;
			_INLINE_ POINT2D	operator +  (const POINT2D & Point) const;
			_INLINE_ POINT2D&	operator += (const POINT2D & Point); 
			_INLINE_ POINT2D&	operator -= (const POINT2D & Point); 
			_INLINE_ Float32		operator [] (unsigned int index) const;
			_INLINE_ Float32&		operator [] (unsigned int index);
			_INLINE_ POINT2D	operator *  (const POINT2D & Point) const;
			_INLINE_ POINT2D	operator *  (Float32 k) const;
			_INLINE_ POINT2D&	operator *= (Float32 k);
			
			_INLINE_ POINT2D	operator /  (Float32 k) const;
			_INLINE_ POINT2D&	operator /= (Float32 k);

			/** Accessors & Modifiors */
			_INLINE_ void			Set(const POINT2D & P);
			_INLINE_ void			Set(const POINT2D_PACKED & P);
			_INLINE_ void			SetXY(const Float32 rX, const Float32 rY);
			_INLINE_ void			GetXY(Float32 & rX, Float32 & rY) const;
			_INLINE_ Float32			GetX() const;
			_INLINE_ Float32			GetY() const;
			_INLINE_ void			SetX(Float32 x);
			_INLINE_ void			SetY(Float32 y);
			_INLINE_ void			Set(IO_STREAM &	Stream);
			_INLINE_ const Float32&	GetXRef() const;
			_INLINE_ const Float32&	GetYRef() const;

			/** Maths */
			_INLINE_ Float32		Cross(const POINT2D & Pt) const;
			_INLINE_ Float32		Dot(const POINT2D & Pt) const;
			_INLINE_ void		Inverse(void);
			_INLINE_ void		Normalize(void);
			_INLINE_ bool		IsNull(void);
			_INLINE_ Float32		GetLength(void);
			_INLINE_ Float32		GetSquareLength(void);

			/** Add a POINT2D scaled by rS. */
			_INLINE_ void		AddScaled(const POINT2D & P, Float32 rS);
			_INLINE_ void		Add(const POINT2D & P);
			_INLINE_ void		Sub(const POINT2D & P);
			/** Make a linear interpolation between two points. */
			_INLINE_ void		Interpolate(const Float32 rT, const POINT2D & P1, const POINT2D & P2);
			/** Project a POINT2D between -1.0f and 1.0f in screen coordinate. */
			_INLINE_ void		Projection(const Float32 rCenterX, const Float32 rCenterY, const Float32 rWidth, const Float32 rHeight);
			
			_INLINE_ bool		IsInRange(const POINT2D &P, const Float32 rRadius) const;
			_INLINE_ void		MinimizeWith(const POINT2D & P);
			_INLINE_ void		MaximizeWith(const POINT2D & P);
			_INLINE_ bool		IsZero() const;


			/** Streams */
			void				Read(IO_STREAM & Stream);
			void				Write(IO_STREAM & Stream)const;

			friend IO_STREAM&	operator << (IO_STREAM & Stream, const POINT2D & Pt)
								{
									Pt.Write(Stream);
									return Stream;
								}
			friend IO_STREAM&	operator >> (IO_STREAM & Stream, POINT2D & Pt)
								{
									Pt.Read(Stream);
									return Stream;
								}

		private:

			Float32				_rX, _rY;
		};
	}
}

#include INCL_KCORE(Stream/iostream)
#include INCL_KMATH(Math/Math)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2DPacked)

#ifndef _DEBUG
#include "Point2D.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES0D_POINT2D_POINT2D_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
