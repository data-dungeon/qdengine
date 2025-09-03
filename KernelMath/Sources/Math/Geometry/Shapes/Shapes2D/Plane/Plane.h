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
// CLASS: PLANE
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: PLANE
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_PLANE_PLANE_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_PLANE_PLANE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(MATRIX)
DECL_KMATH(TRIANGLE)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class PLANE
		{

		public:

			/** Default constructor */
					PLANE();
					PLANE(const PLANE &);
					PLANE(float ra, float rb, float rc, float rd);
					PLANE(const VECTOR &, const VECTOR &, const VECTOR &);
					PLANE(const VECTOR & Pt, const VECTOR & Normal);
					PLANE(const TRIANGLE &);

			_INLINE_ bool	operator ==(const PLANE& P) const;
			_INLINE_ bool	operator !=(const PLANE& P) const;
			_INLINE_ void	operator =(const PLANE& P);
			_INLINE_ void	operator =(const POINT4D & V);

			_INLINE_ void	NormalizePlane();
			_INLINE_ void	NormalizeNormal();

			_INLINE_ bool	IsPlaneNormalized() const;
			_INLINE_ bool	IsNormalNormalized() const;

			_INLINE_ float	GetSignedDistance(const VECTOR &Pos) const;
			_INLINE_ void	SetD(float r);
			_INLINE_ float	GetD(void) const;

					void	VerticalProjection(const VECTOR &Src, VECTOR &Dst) const;

					bool	AreOutside(int nNbrPoints, VECTOR * pPoints) const;
					bool	IsOutside(const VECTOR & Point) const;

			/** Project orthogonaly a point on a plane */
			void			Project(const VECTOR & Pt, VECTOR & Res);

			/** Project a point on a plane according to the vector Dir */
			bool			Project(const VECTOR & Pt, const VECTOR & Dir, VECTOR & Res) const;

			_INLINE_ void	Copy(const PLANE &);
			_INLINE_ void	SetFromTriangle(const TRIANGLE &);
					void	SetFromPoints(const VECTOR & Pt1, const VECTOR & Pt2, const VECTOR & Pt3);
			_INLINE_ void	SetFromPointAndNormal(const VECTOR &Pt, const VECTOR &	Normal);

			_INLINE_ void	Flip();
			_INLINE_ void	FlipNormal();

			_INLINE_ void	SetCoeff(float, float, float, float);
			_INLINE_ void	GetCoeff(float &, float &, float &, float &) const;	

					void			GetPoint(VECTOR &) const;
			_INLINE_ VECTOR			GetPoint(bool bNormalizedNormal) const;
			_INLINE_ void			GetNormal(VECTOR &) const; 
			_INLINE_ const VECTOR&	GetNormal() const; 
			void			GetVectors(VECTOR &, VECTOR &) const;

			_INLINE_ void			SetNormal(const VECTOR & V);

			bool 			GetSide(const VECTOR & Pt)const;
			void 			GetSide(const VECTOR & Pt, int & nSide, const float rEpsilon = 0.00001f) const;
					void	Transform(const MATRIX &);

			float			GetT(VECTOR &, VECTOR &)const;
			bool			IsOn(const VECTOR &)const;
			bool			IsOn(const VECTOR &, bool &)const;
			float			Apply(VECTOR &);
			void			ZProjection(const float xcen, const float ycen, const float xfoc, const float yfoc);

			bool			Intersect(const VECTOR &PIn,
									VECTOR		&POut,
									const VECTOR  &vMotion,
									const float	rInertia = 1.0f,
									const float	rE = 0.0f) const;

			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s) const;
			/** */
			float GetConstant() const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const PLANE &m) 
			{
				m.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, PLANE &m)
			{
				m.Read(s);
				return s;
			}

		private:

		//	float			_rD; -> Padded in _vNormal.GetPad()
			VECTOR			_vNormal;
		};

	}
}

#include INCL_KMATH(Math/Math)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point4D/Point4D)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)

#ifndef _DEBUG
#include "plane.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_PLANE_PLANE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
