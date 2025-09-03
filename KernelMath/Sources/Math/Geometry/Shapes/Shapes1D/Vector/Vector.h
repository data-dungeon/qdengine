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
// CLASS: VECTOR
//
//
//00-11-10 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: VECTOR
//00-11-10 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES1D_VECTOR_VECTOR_H__
#define __KMATH_MATH_SHAPES_SHAPES1D_VECTOR_VECTOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PS3_MATH
#include	INCL_KCORE(Includes_PS3/Includes_PS3)

DECL_KCORE(IO_STREAM)
DECL_KMATH(COLOR_FLOAT)
DECL_KMATH(POINT4D)
DECL_KMATH(VECTOR)
DECL_KMATH(VECTOR_PACKED)

namespace QDT
{
	namespace KMATH
	{
		struct VECTOR_INT8
		{
			unsigned char x, y, z;
		};

		struct VECTOR_INT16
		{
			unsigned short x, y, z;
		};

		class	VECTOR
		{
			friend class MATRIX;
			friend class QUATERNION;

		public:

			enum OBJECT_TYPE
			{
				VM_OBJECT,
				CPP_OBJECT
			};

			// Commonly used vectors...
			static const VECTOR		ZERO;
			static const VECTOR		UNIT_X;
			static const VECTOR		UNIT_Y;
			static const VECTOR		UNIT_Z;
			static const VECTOR		NEG_UNIT_X;
			static const VECTOR		NEG_UNIT_Y;
			static const VECTOR		NEG_UNIT_Z;
			static const VECTOR		FLOAT_MAX;
			static const VECTOR		FLOAT_MIN;
			
			// Static functions pointers (to optimize basic method with VU0 and SSE (later..)
			typedef void (*pfs_VECTOR_VECTOR)(VECTOR& Dst, const VECTOR& Src);

			/** Default constructor. If the flag is set to 'true', the vector is set to null. Else nothing is done. */
			_INLINE_ explicit	VECTOR();
			_INLINE_ explicit	VECTOR(OBJECT_TYPE _ObjectType);
			/** Copy constructor. */
			_INLINE_ 			VECTOR(const VECTOR& V)											{ QDT_APPEND_PLATFORM(Construct)(V); }
			_INLINE_ explicit	VECTOR(const VECTOR& V, OBJECT_TYPE _ObjectType);
			/** Create a vector with its coordinates. */
			_INLINE_ explicit	VECTOR(const float rVx, const float rVy, const float rVz)		{ QDT_APPEND_PLATFORM(Construct)(rVx, rVy, rVz); }
			_INLINE_ explicit	VECTOR(const POINT4D& Pt);
			_INLINE_ explicit	VECTOR(const POINT4D& Pt, OBJECT_TYPE _ObjectType);
			/** Create a vector from a VectorPacked. */
			_INLINE_			VECTOR(const VECTOR_PACKED& V);
			/** Create a vector from another vector, then setting the right length. */
			_INLINE_ explicit	VECTOR(const VECTOR& V, const float rNorm);

			void				Interpolate(float rT, const VECTOR& V1, const VECTOR& V2)					{ QDT_APPEND_PLATFORM(Interpolate)(rT, V1, V2); }

			_INLINE_ bool		IsInRange(const VECTOR &Pos, const float R) const;


			/** Assignement operator. */ 
			_INLINE_ VECTOR&	operator = (const VECTOR& Vector)								{ return (QDT_APPEND_PLATFORM(Assign)(Vector)); }
			_INLINE_ VECTOR&	SlowAssign(const VECTOR& Vector);
			_INLINE_ VECTOR&	operator = (const POINT4D& Point);

			/** Equality operator. */
			_INLINE_ bool		operator ==(const VECTOR& Vector) const;
			/** Difference operator. */
			_INLINE_ bool		operator !=(const VECTOR& Vector) const;

			/** Copy method. */
			_INLINE_ void		Copy(const VECTOR & V)											{ QDT_APPEND_PLATFORM(Copy)(V); }
			_INLINE_ void		SlowCopy(const VECTOR & V);

			_INLINE_ bool		IsZero() const;
		 
			/** Static copy method. */
			static void			StaticCopy(VECTOR & Dst, const VECTOR & Src);
			/** Static slow copy method. */
			static void			StaticSlowCopy(VECTOR & Dst, const VECTOR & Src);

			/** Accessors. */
			_INLINE_ float				GetX() const;
			_INLINE_ float				GetY() const;
			_INLINE_ float				GetZ() const;
			_INLINE_ float				GetPad() const;
			_INLINE_ void				GetXYZ(float & rX, float & rY, float & rZ) const;
			_INLINE_ const Float32&		GetXRef() const;
			_INLINE_ const Float32&		GetYRef() const;
			_INLINE_ const Float32&		GetZRef() const;


			/** Modifiers. */
			_INLINE_ void		SetX(const float rX);
			_INLINE_ void		SetY(const float rY);
			_INLINE_ void		SetZ(const float rZ);
			_INLINE_ void		SetPad(const float rPad);
			_INLINE_ void		SetXYZ(const float x, const float y, const float z);
			_INLINE_ void		SlowSetXYZ(const float x, const float y, const float z);
			_INLINE_ void		Set(IO_STREAM &	Stream);
			
			/** Set a vector from two pojntx points (the extremities of the vector). */
			_INLINE_ void		SetFromPoints(const VECTOR& Pt1, const VECTOR& Pt2);
			_INLINE_ void		SlowSetFromPoints(const VECTOR& Pt1, const VECTOR& Pt2);
			/** Set a vector from a VectorPacked. */
			_INLINE_ void		SetFromVectorPacked(const VECTOR_PACKED& V);
			/** Set a vector from another vector, then setting the right length. */
			_INLINE_ void		SetFromVectorAndNorm(const VECTOR& V, const float rNorm);
			
			/** Return the length of a vector. */
			_INLINE_ float		Length() const													{ return (QDT_APPEND_PLATFORM(Length)()); }
			_INLINE_ float		SlowLength() const;
			/** Return the squared length of a vector. */
			_INLINE_ float		SquareLength() const											{ return (QDT_APPEND_PLATFORM(SquareLength)()); }
			_INLINE_ float		SlowSquareLength() const;

			/** True if vector are almost equal less than 1/1000 error on components*/
			_INLINE_ bool		EqualWithError(const VECTOR &	Q, const float	rError = 0.001f ) const;
			/** */
			_INLINE_ bool		IsOneD();

			void				Display(COLOR_FLOAT &Color, const float rSize) const;

			//=========================================================================
			// Below are the arithmetic method, that are safer than the operators
			// but less readable.
			//=========================================================================

			_INLINE_ void		Absolute()														{ QDT_APPEND_PLATFORM(Absolute)(); }

			_INLINE_ void		AddMul(const VECTOR &V, float r) 								{ QDT_APPEND_PLATFORM(AddMul)(V, r); }	// *this += (V * r);

			_INLINE_ void		Add(const VECTOR &p, const float s)								{ QDT_APPEND_PLATFORM(Add)(p, s); }
			_INLINE_ void		SlowAdd(const VECTOR &p, const float s);
			_INLINE_ void		Add(const VECTOR &V1, const VECTOR &V2)							{ QDT_APPEND_PLATFORM(Add)(V1, V2); }	
			_INLINE_ void		SlowAdd(const VECTOR &V1, const VECTOR &V2);
			_INLINE_ void		Add(const VECTOR &V)											{ QDT_APPEND_PLATFORM(Add)(V); }
			_INLINE_ void		SlowAdd(const VECTOR &V);
			_INLINE_ void		Add(float r)													{ QDT_APPEND_PLATFORM(Add)(r); }
			_INLINE_ void		SlowAdd(float r);
			_INLINE_ void		Sub(const VECTOR &V1, const VECTOR &V2)							{ QDT_APPEND_PLATFORM(Sub)(V1, V2); }
			_INLINE_ void		SlowSub(const VECTOR &V1, const VECTOR &V2);
			_INLINE_ void		Sub(const VECTOR &V)											{ QDT_APPEND_PLATFORM(Sub)(V); }
			_INLINE_ void		SlowSub(const VECTOR &V);
			_INLINE_ void		Sub(float r)													{ QDT_APPEND_PLATFORM(Sub)(r); }
			_INLINE_ void		SlowSub(float r);
			_INLINE_ void		Neg(const VECTOR &V)											{ QDT_APPEND_PLATFORM(Neg)(V); }
			_INLINE_ void		SlowNeg(const VECTOR &V);
			_INLINE_ void		Neg()															{ QDT_APPEND_PLATFORM(Neg)(); }
			_INLINE_ void		SlowNeg();
			_INLINE_ void		Mul(const VECTOR &V, const float rValue)						{ QDT_APPEND_PLATFORM(Mul)(V, rValue); }
			_INLINE_ void		SlowMul(const VECTOR &V, const float rValue);
			_INLINE_ void		Mul(const float rValue)											{ QDT_APPEND_PLATFORM(Mul)(rValue); }
			_INLINE_ void		SlowMul(const float rValue);
			_INLINE_ void		Mul(const VECTOR &V, const VECTOR &Scale3D);
			_INLINE_ void		SlowMul(const VECTOR &V, const VECTOR &Scale3D);
			_INLINE_ void		Mul(const VECTOR &Scale3D);
			_INLINE_ void		SlowMul(const VECTOR &Scale3D);

			//=========================================================================


			//=========================================================================
			// Below are the operators that might be slower because of unecessary
			// objects allocation, but their use makes the code more readable.
			//=========================================================================

			/** Operator +=. Same as Add(const VECTOR &V). You can write V1 += V2. */
			_INLINE_ VECTOR &	operator +=(const VECTOR &V)									{ return (QDT_APPEND_PLATFORM(OpAddAssign)(V)); }
			_INLINE_ VECTOR &	SlowAddAssign(const VECTOR &V);
			/** Operator +. You can write V = V1 + V2. */
			_INLINE_ VECTOR 	operator +(const VECTOR &V) const								{ return (QDT_APPEND_PLATFORM(OpAdd)(V)); }
			_INLINE_ VECTOR 	SlowAdd(const VECTOR &V) const;
			/** Substract a vector from another vector. Same as Sub(const VECTOR &V). You can write: V1 -= V2. */
			_INLINE_ VECTOR &	operator -=(const VECTOR &V)									{ return (QDT_APPEND_PLATFORM(OpSubAssign)(V)); }
			_INLINE_ VECTOR &	SlowSubAssign(const VECTOR &V);
			/** Binary operator -. You can write: V = V1 - V2. */
			_INLINE_ VECTOR 	operator -(const VECTOR &V) const								{ return (QDT_APPEND_PLATFORM(OpSub)(V)); }
			_INLINE_ VECTOR 	SlowSub(const VECTOR &V) const;
			/** Unitary Operator -. You can write: V1 = -V2. */
			_INLINE_ VECTOR		operator -() const												{ return (QDT_APPEND_PLATFORM(OpMinusUnary)()); }
			_INLINE_ VECTOR		SlowMinusUnaryOp() const;
			/** Unitary Operator -. You can write: V1 = +V2. */
			_INLINE_ VECTOR		operator +() const;
			/** Multiply the coordinates of a vector by a value. Same as Mul(const float rValue). You can write V *= 2.0f. */
			_INLINE_ VECTOR &	operator *=(float rValue)										{ return (QDT_APPEND_PLATFORM(OpMulAssign)(rValue)); }
			_INLINE_ VECTOR &	SlowMulAssign(float rValue);
			/** Operator *. You can write: V1 = V2 * 2.0f. */
			_INLINE_ VECTOR		operator *(float rValue) const									{ return (QDT_APPEND_PLATFORM(OpMul)(rValue)); }
			_INLINE_ VECTOR		SlowMul(float rValue) const;
			
			/** Operator *. You can write: V1 = 2.0f * V2. */
			friend VECTOR operator *(const float rValue, const VECTOR &Vector) 
			{ 
				return ( Vector * rValue );
			}
			/** Operator / */
			_INLINE_ VECTOR		operator / (const float rValue) const							{ return (QDT_APPEND_PLATFORM(OpDiv)(rValue)); }
			/** Operator /= */
			_INLINE_ VECTOR	&	operator /= (const float rValue)								{ return (QDT_APPEND_PLATFORM(OpDivAssign)(rValue)); }
			
			/** */
			_INLINE_ float operator [] (const unsigned int i) const;
			/** */
			_INLINE_ float& operator [] (const unsigned int i);
			/** */
			_INLINE_ float operator *  (const VECTOR& V) const;
			/** */	
			_INLINE_ int GetClosestAxis() const;
			/** */
			_INLINE_ VECTOR operator^ (const VECTOR &V) const;
			/** */
			_INLINE_ const float* ToPointer() const;
			/** */
			_INLINE_ float* ToPointer();
			/** */
			_INLINE_ float GetNormeInf() const;
			/** */
			_INLINE_ bool IsNull( float repsilon = 0.01f ) const;
			/** */
			_INLINE_ void Set(const VECTOR &V);


			//=========================================================================


			/** Set a vector to the cross product af two other vectors. */
			_INLINE_ void		Cross(const VECTOR &V1, const VECTOR &V2)					{ QDT_APPEND_PLATFORM(Cross)(V1, V2); }
			_INLINE_ void		SlowCross(const VECTOR &V1, const VECTOR &V2);

			/** Return the dot product of two vectors. */
			_INLINE_ float		Dot(const VECTOR &V) const									{ return (QDT_APPEND_PLATFORM(Dot)(V)); }
			_INLINE_ float		SlowDot(const VECTOR &V) const;
			_INLINE_ float		ZDot(VECTOR &V);
			/** Return the det of three vectors (corresponding to the volume generated by the three vectors). */
			_INLINE_ float		Det(VECTOR &V1, VECTOR &V2) const;
			/** Set to zero the coordinates of a vector. */
			_INLINE_ void		Zero()														{ return (QDT_APPEND_PLATFORM(Zero)()); }
			_INLINE_ void		SlowZero();
			
					VECTOR		GetOrthoVector() const;


			/** Change the length of a vector. */
			_INLINE_ float		SetLength(float rNorm);
			_INLINE_ float		SlowSetLength(float rNorm);
			/** Set the length of a vector to 1. */
			_INLINE_ float		Normalize();
			_INLINE_ float		SlowNormalize();
			/** Set a vector to another normalized vector. */
			_INLINE_ float		Normalize(const VECTOR& V);
			_INLINE_ float		SlowNormalize(const VECTOR& V);


			/** Check if the norm a vector is 1. */
			_INLINE_ bool		IsNormalized() const;
			/** Linear interpolation between 2 vector coef range from 0.0f to 1.0f */
			_INLINE_ void		InterLinear(const float rLerpCoeff, const VECTOR & V1, const VECTOR & V2);
			_INLINE_ void		SlowInterLinear(const float rLerpCoeff, const VECTOR & V1, const VECTOR & V2);
			/** Linear interpolation between vector and Id vector coef range from 0.0f to 1.0f */
			_INLINE_ void		InterLinearId(const float rLerpCoeff, VECTOR& v) ;
			_INLINE_ void		SlowInterLinearId(const float rLerpCoeff, VECTOR& v) ;

			_INLINE_ void		MinimizeWith(const VECTOR &	V);
			_INLINE_ void		MaximizeWith(const VECTOR &	V);

			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s) const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const VECTOR &m)
			{
				m.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, VECTOR &m)
			{
				m.Read(s);
				return s;
			}

		/*	friend std::istream& operator>>( std::istream& in, VECTOR& vect)
			{
				in >> vect[_X_] >> vect[_Y_] >> vect[_Z_] ;
				return ( in ) ;
			};
			friend std::ostream& operator<<( std::ostream& out, const VECTOR& vect)
			{
		#ifndef _MASTER
				out << " x: " << vect[_X_] << " y: " << vect[_Y_] << " z: " << vect[_Z_] ;
		#endif
				return ( out ) ;
			};*/
			
			int GetDataSize() const { return sizeof(float) * 3; }

		private:

			/** Copy method. */
			_INLINE_ void			Copy(const POINT4D & P);
			_INLINE_ void			SlowCopy(const POINT4D & P);

			// Include platform specific .h files here
			#include	"VectorGen.h"

			#if defined PLATFORM_PC
				#include	"VectorPC.h"
			#endif

			#if defined PLATFORM_PS3
				#include	"VectorPS3.h"
			#endif

			#if defined PLATFORM_X360
				#include	"VectorX360.h"
			#endif
		};
	}
}

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/More/Angle3D)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/VectorPacked)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point4D/Point4D)

#if (!defined _DEBUG)
	#include	"Vector.inl"
#endif

#if (!defined _DEBUG)
	#include	"VectorGen.inl"
#endif

#if (!defined _DEBUG) && (defined PLATFORM_PS3)
	#include	"VectorPS3.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES1D_VECTOR_VECTOR_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================

