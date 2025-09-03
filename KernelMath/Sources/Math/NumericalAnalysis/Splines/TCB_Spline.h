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
//	CLASS:	TCB_SPLINE
//	The TCB_SPLINE class implements ...
//
//	03-01-31:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_TCB_SPLINES_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_TCB_SPLINES_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)

#include	INCL_KCORE(Template/QDT_Vector)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace KMATH
	{
		class	TCB_SPLINE
		{
		public:
			struct	HERMITE_POINT
			{
				VECTOR		Point;
				VECTOR		TangentIn;
				VECTOR		TangentOut;
				float		rTension;
				float		rContinuity;
				float		rBias;
			};

			TCB_SPLINE();
			explicit TCB_SPLINE(const TCB_SPLINE & C);
			~TCB_SPLINE();
			//	REMEMBER To make the destructor virtual if 
			//	you create a subclass, or if you have one

			TCB_SPLINE &	operator=(const TCB_SPLINE & C);
			void	Copy(const TCB_SPLINE & C);

			// Vector
			void			AddHermitePoint( const HERMITE_POINT& HermitePoint );
			void			AddHermitePoint( const VECTOR& Point );
			void			AddHermitePoint( const VECTOR& Point, const VECTOR& TangentIn, const VECTOR& TangentOut, const float rTension );
			void			AddHermitePoint( const VECTOR& Point, const VECTOR& TangentIn, const VECTOR& TangentOut, const float rTension, const float rContinuity, const float rBias );
			HERMITE_POINT	GetHermitePoint( const int nIndex ) const;
			int				GetNbrPoints() const;

			// Accessors
			void			SetPoint( const int nIndex, const VECTOR& Point );
			void			SetTangentOut( const int nIndex, const VECTOR& Tangent );
			void			SetTangentIn( const int nIndex, const VECTOR& Tangent );
			void			SetTension( const int nIndex, const float rTension );
			void			SetContinuity( const int nIndex, const float rContinuity );
			void			SetBias( const int nIndex, const float rBias );
			VECTOR			GetTangentOut( const int nIndex ) const;
			VECTOR			GetPoint( const int nIndex ) const;
			VECTOR			GetTangentIn( const int nIndex ) const;
			float			GetTension( const int nIndex ) const;

			// Spline
			void			AutoComputeTangents();

		private:
			float			GetContinuity( const int nIndex ) const;
			float			GetBias( const int nIndex ) const;
			QDT_VECTOR<HERMITE_POINT>	_vPoints;
		};
	}
}

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_TCB_SPLINES_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
