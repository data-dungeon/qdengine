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
// CLASS: CONE
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: CONE
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES3D_CONE_CONE_H__
#define __KMATH_MATH_SHAPES_SHAPES3D_CONE_CONE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(COLOR_FLOAT)
DECL_KMATH(MATRIX)
DECL_KMATH(HEXAHEDRON)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace KMATH
	{
		class	CONE
		{

		public:

			/** Constructors & destructor */
			_INLINE_ explicit			CONE();
			_INLINE_					CONE(const CONE & Cone);
			_INLINE_					CONE(const VECTOR & Top, const VECTOR & Axe, const float rAngle, const float rRadius = -1.0f);
			_INLINE_				   ~CONE();

			/** Operators */
			const CONE &				operator = (const CONE & Cone);

			/** Transform */
			void						Transform(const MATRIX &Matrix);

			/** Accessors & Modifiors*/
			_INLINE_ const VECTOR &		GetTop() const;
			_INLINE_ const VECTOR &		GetAxe() const;
			_INLINE_ const float		GetRadius() const;
			_INLINE_ const float		GetAngle() const;
			_INLINE_ const float		GetSinAngle() const;
			_INLINE_ const float		GetCosAngle() const;
			_INLINE_ void				SetTop(const VECTOR & Top);
			_INLINE_ void				SetAxe(const VECTOR & Axe);
			_INLINE_ void				SetRadius(const float rRadius);
			_INLINE_ void				SetAngle(const float rAngle);

			/** Display */
			void						Display(COLOR_FLOAT &Color, float rLength = 0.0f,int nStep = 24);
			void						DisplayFilled(const COLOR_FLOAT &Color, float rLength = 0.0f, int nStep = 24);

			bool						Intersect(const HEXAHEDRON &Hexahedron) const;

			/** Streams */
			void						Read(IO_STREAM &s);
			void						Write(IO_STREAM &s) const;

			friend IO_STREAM&			operator <<(IO_STREAM &s, const CONE &a)
			{
				a.Write(s);
				return s;
			}
			friend IO_STREAM&			operator >>(IO_STREAM &s, CONE &a)
			{
				a.Read(s);
				return s;
			}

		private:

			VECTOR			_Top;
			VECTOR			_Axe;
			float			_rRadius;
			float			_rAngle;
			float			_rCosAngle;		// For speed during intersection tests
			float			_rSinAngle;		//
		};
	}
}

#ifndef _DEBUG
#include "cone.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES3D_CONE_CONE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
