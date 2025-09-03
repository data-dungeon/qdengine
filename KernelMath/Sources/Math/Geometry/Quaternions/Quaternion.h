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
// CLASS: QUATERNION
//
// 00-11-09 JVA Created: 
//
//*****************************************************************************


//=============================================================================
// Name: QUATERNION
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_QUATERNION_QUATERNION_H__
#define __KMATH_MATH_QUATERNION_QUATERNION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PS3_MATH
#include	INCL_KCORE(Includes_PS3/Includes_PS3)

DECL_KMATH(MATRIX)
DECL_KMATH(QUATERNION_COMPRESSED)
DECL_KCORE(IO_STREAM)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)
#include	INCL_KMATH(Math/Geometry/More/Angle3D)

namespace QDT
{
	namespace KMATH
	{
		class	QUATERNION
		{
			friend class MATRIX;
			friend class VECTOR;

		public:

			enum OBJECT_TYPE
			{
				VM_OBJECT,
				CPP_OBJECT
			};

			// Commonly used Quaternions ...
			static const QUATERNION		IDENTITY;
			static const QUATERNION		ZERO;

			// constructors
			_INLINE_ 					QUATERNION();					// THIS IS NOT TRUE !! : -->"we don't initialize on the Identity for optimization reasons"
			_INLINE_ 					QUATERNION(bool	bSetIdentity);
			_INLINE_ 					QUATERNION(OBJECT_TYPE eObjectType);
			_INLINE_					QUATERNION(const QUATERNION& qSource);
			_INLINE_					QUATERNION(const QUATERNION& Q, OBJECT_TYPE eObjectType);
			_INLINE_					QUATERNION(const Float32 rW, const Float32 rX, const Float32 rY, const Float32 rZ);
			_INLINE_	explicit		QUATERNION(const ANGLE3D& Angle3D);
			_INLINE_					QUATERNION(const VECTOR& V, const Float32 rRoll);
			_INLINE_					QUATERNION(const VECTOR& V1, const VECTOR& V2);
			_INLINE_					~QUATERNION()										{ }

			_INLINE_ 	void			Id();							//Initialize with Identity(0.0f, 0.0f, 0.0f, 1.0f)
			_INLINE_ 	void			SlowId();
			_INLINE_ 	void			Set(const QUATERNION& Q);
			_INLINE_ 	void			SlowSet(const QUATERNION& Q);

			//Initialize members
			_INLINE_	void			SetWXYZ(const Float32 rW, const Float32 rX, const Float32 rY, const Float32 rZ);
			_INLINE_	void			SlowSetWXYZ(const Float32 rW, const Float32 rX, const Float32 rY, const Float32 rZ);
						void			SetRotation(const VECTOR& V1, const VECTOR& V2)		{ QDT_APPEND_PLATFORM(SetRotation)(V1, V2); } 
						void			Set(const ANGLE3D& Angle3D);
			_INLINE_	ANGLE3D			GetAngle3D() const;
						void			GetPitchHeadRoll(Float32& rPitch, Float32& rHead, Float32& rRoll) const;
						void			Set(const VECTOR& vAngle3D);
						void			SetFromMatrix(const MATRIX& mtxSource)				{ QDT_APPEND_PLATFORM(SetFromMatrix)(mtxSource); }
						void			SetVectRoll(const VECTOR& V, const Float32 rRoll)		{ QDT_APPEND_PLATFORM(SetVectRoll)(V, rRoll); }

			_INLINE_ 	void			SetX(Float32 rValueX);
			_INLINE_ 	void			SetY(Float32 rValueY);
			_INLINE_ 	void			SetZ(Float32 rValueZ);
			_INLINE_ 	void			SetW(Float32 rValueW);

			// Get members
			_INLINE_ 	void			GetWXYZ(Float32& rW, Float32& rX, Float32& rY, Float32& rZ) const;
			_INLINE_ 	Float32			GetX() const;
			_INLINE_ 	Float32			GetY() const;
			_INLINE_ 	Float32			GetZ() const;
			_INLINE_ 	Float32			GetW() const;

			_INLINE_	const Float32&	GetXRef() const;
			_INLINE_	const Float32&	GetYRef() const;
			_INLINE_	const Float32&	GetZRef() const;
			_INLINE_	const Float32&	GetWRef() const;

			//Get the vector and the roll calculated from the quaternion
						void			GetVectRoll(VECTOR& V, Float32& rRoll) const;

			_INLINE_ 	QUATERNION&		operator =	(const QUATERNION&	Q)					{ return (QDT_APPEND_PLATFORM(Assign)(Q)); }
					 	QUATERNION&		operator =	(const QUATERNION_COMPRESSED& Q);		
			_INLINE_ 	QUATERNION&		SlowAssign	(const QUATERNION& Q);
			_INLINE_ 	bool			operator ==	(const QUATERNION& Q) const;
			_INLINE_ 	bool			operator !=	(const QUATERNION& Q) const;
			_INLINE_ 	QUATERNION		operator *	(const QUATERNION& Q) const				{ return (QDT_APPEND_PLATFORM(MulConst)(Q)); }
			_INLINE_ 	QUATERNION		SlowMulConst(const QUATERNION& Q) const;

			_INLINE_ 	QUATERNION&		operator *=	(const QUATERNION& Q)					{ return (QDT_APPEND_PLATFORM(MulAssign)(Q)); }
			_INLINE_ 	QUATERNION&		SlowMulAssign(const QUATERNION& Q) ;
			_INLINE_ 	QUATERNION&		operator +=	(const QUATERNION& Q)					{ return (QDT_APPEND_PLATFORM(AddAssign)(Q)); }
			_INLINE_ 	QUATERNION&		SlowAddAssign(const QUATERNION& Q) ;

			_INLINE_ 	QUATERNION&		operator /=	(const Float32 rVal)						{ return (QDT_APPEND_PLATFORM(DivAssign)(rVal)); }
			_INLINE_ 	QUATERNION&		SlowDivAssign(const Float32 rVal) ;
			_INLINE_ 	Float32			operator []	(const unsigned int i) const;
			_INLINE_ 	Float32&			operator []	(const unsigned int i);
			_INLINE_ 	QUATERNION		operator *	(const Float32 rVal ) const				{ return (QDT_APPEND_PLATFORM(Mul)(rVal)); }
			_INLINE_ 	QUATERNION		SlowMul(const Float32 rVal ) const;
			_INLINE_ 	QUATERNION		SlowMulNoConst(const Float32 rVal );
			
			friend QUATERNION operator * (const Float32 k, const QUATERNION& Q)
			{
				return (Q*k);
			}

						// Multiply V by rValue and put the result on Q1
						void			Mul(const QUATERNION& Q1, const QUATERNION& Q2)		{ QDT_APPEND_PLATFORM(Mul)(Q1, Q2); }
						void			SlowMul(const QUATERNION& Q1, const QUATERNION& Q2);
						void			Mul(const QUATERNION& Q)							{ QDT_APPEND_PLATFORM(Mul)(Q); }
						void			SlowMul(const QUATERNION& Q);

			// INTERPOLATION : shortest way

						//Make a linear interpolation between Q1 and Q2 with a coefficient from 0.0f to 1.0f
						void			InterLinear(const Float32 rLerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2, bool bNormalize = false)		{ QDT_APPEND_PLATFORM(InterLinear)(rLerpCoeff, Q1, Q2, bNormalize); }
						void			SlowInterLinear(const Float32 rLerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2, bool bNormalize = false);

						//Make a spherical interpolation between Q1 and Q2 with a coefficent from 0.0f to 1.0f
						void			InterSLERP(const Float32 rSlerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2)								{ QDT_APPEND_PLATFORM(InterSLERP)(rSlerpCoeff, Q1, Q2); }
						void			SlowInterSLERP(const Float32 rSlerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2);

						//Make a linear interpolation between Q and Id with a coefficient from 0.0f to 1.0f
						void			InterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize = false);
						void			SlowInterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize = false);

						//Make a spherical interpolation between Q and Id with a coefficient from 0.0f to 1.0f
						void			InterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q);
						void			SlowInterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q);

			//INTERPOLATION : angle gradient way

						//Make a linear interpolation between Q1 and Q2 with a coefficient from 0.0f to 1.0f
						void			BasicInterLinear(const Float32 rLerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2, bool bNormalize = false)	{ QDT_APPEND_PLATFORM(BasicInterLinear)(rLerpCoeff, Q1, Q2, bNormalize); }
						void			SlowBasicInterLinear(const Float32 rLerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2, bool bNormalize = false);
						//Make a spherical interpolation between Q1 and Q2 with a coefficent from 0.0f to 1.0f
						void			BasicInterSLERP(const Float32 rSlerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2)							{ QDT_APPEND_PLATFORM(BasicInterSLERP)(rSlerpCoeff, Q1, Q2); }
						void			SlowBasicInterSLERP(const Float32 rSlerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2);
						//Make a linear interpolation between Q and Id with a coefficient from 0.0f to 1.0f
						void			BasicInterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize = false);
						void			SlowBasicInterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize = false);
						//Make a spherical interpolation between Q and Id with a coefficient from 0.0f to 1.0f
						void			BasicInterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q);
						void			SlowBasicInterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q);
			
			//Normalize the quaternion
						void			Normalize()										{ QDT_APPEND_PLATFORM(Normalize)(); }
						void			SlowNormalize();

			_INLINE_ 	Float32			Norm2() const									{ return (QDT_APPEND_PLATFORM(Norm2)()); } 			//Norm2 of a quaternion
			_INLINE_ 	bool			IsNormalized() const;														//Check normalized
			_INLINE_ 	void			Inverse(const QUATERNION& Q);												//Compute the inverse of the quaternion in parameter
			_INLINE_ 	QUATERNION&		Inverse();																	//Invert the quaternion in place
			_INLINE_ 	bool			EqualWithError(const QUATERNION& Q, const Float32	rError = 0.001f ) const;	//True if quaternions are almost equal less than 1/1000 error on components

			_INLINE_ 	QUATERNION&		operator *=	(const Float32 rVal)					{ return (QDT_APPEND_PLATFORM(MulAssign)(rVal)); }
			_INLINE_ 	QUATERNION&		SlowMulAssign(const Float32 rVal);
			_INLINE_ 	QUATERNION		operator +	(const QUATERNION& Q) const			{ return (QDT_APPEND_PLATFORM(Add)(Q)); }
			_INLINE_ 	QUATERNION		SlowAdd(const QUATERNION& Q) const;
			_INLINE_ 	QUATERNION		operator -	(const QUATERNION& Q) const			{ return (QDT_APPEND_PLATFORM(Sub)(Q)); }
			_INLINE_ 	QUATERNION		SlowSub(const QUATERNION& Q) const;
						VECTOR			operator *	(const VECTOR& V) const				{ return (QDT_APPEND_PLATFORM(Mul)(V)); }
						VECTOR			SlowMul		(const VECTOR& V) const;

			_INLINE_ 	Float32			Dot(const QUATERNION& Q) const					{ return (QDT_APPEND_PLATFORM(Dot)(Q)); }
			_INLINE_ 	Float32			SlowDot(const QUATERNION& Q) const;
			_INLINE_ 	Float32			GetAngleWith(const QUATERNION& Q) const;
			_INLINE_ 	void			UnitInverse(const QUATERNION& Q);
			_INLINE_ 	void			SlowUnitInverse(const QUATERNION& Q);
			_INLINE_ 	QUATERNION&		UnitInverse();
			_INLINE_ 	QUATERNION&		SlowUnitInverse();
			_INLINE_ 	bool			IsPure() const;
			_INLINE_ 	bool			IsUnit() const;

						void			GetXYEulerAngle(Float32& Alpha, Float32& Beta) const;			//Works only if there 's no rotation on z

			_INLINE_ 	VECTOR			GetAxis() const;
			_INLINE_ 	VECTOR			SlowGetAxis() const;
			_INLINE_ 	bool			IsIdentity() const;
			_INLINE_ 	Float32			Norm() const;

			//Pitch, Head, Roll
						void			GetPitchHeadRoll(QUATERNION& Pitch, QUATERNION& Head, QUATERNION& Roll);
						void			SetPitchHeadRoll(const Float32 rX, const Float32 rY, const Float32 rZ);
			
						static QUATERNION	Exp(const QUATERNION& Q);
						static QUATERNION	Log(const QUATERNION& Q);
						static QUATERNION	Power(const QUATERNION& Q, const Float32 t);
						static QUATERNION	Slerp(const Float32 rCoeff, const QUATERNION& Q1, const QUATERNION& Q2)		{ return (QDT_APPEND_PLATFORM(Slerp)(rCoeff, Q1, Q2)); }
						static QUATERNION	Squad(const Float32 rCoeff, const QUATERNION& Q1, const QUATERNION& Q2, const QUATERNION& Q3, const QUATERNION& Q4) { return (QDT_APPEND_PLATFORM(Squad)(rCoeff, Q1, Q2, Q3, Q4)); }
						static void			BuildControlQuaternions(const QUATERNION& Q1, const QUATERNION& Q2, const QUATERNION& Q3, QUATERNION& Q4);

			// STREAM operations
						void				Read(IO_STREAM& Stream);
						void				Write(IO_STREAM& Stream) const;
						void				ReadPacked(IO_STREAM& Stream);
						void				WritePacked(IO_STREAM& Stream) const;

						friend IO_STREAM& operator <<(IO_STREAM& Stream, const QUATERNION& q)
						{
							q.Write(Stream);
							return Stream;
						}

						friend IO_STREAM& operator >>(IO_STREAM& Stream, QUATERNION& q)
						{
							q.Read(Stream);
							return Stream;
						}
			
			int GetDataSize() const { return (sizeof(Float32) << 2); }

			_INLINE_	const Float32*	ToPointer() const;
			_INLINE_	Float32*			ToPointer();

		private:

			_INLINE_ 	void			Mul(const VECTOR& V, const Float32 rValue )		{ QDT_APPEND_PLATFORM(Mul)(V, rValue); }
			_INLINE_ 	void			SlowMul(const VECTOR& V, const Float32 rValue );

			// Include platform specific .h files here
			#include	"QuaternionGen.h"

			#if defined PLATFORM_PC
				#include	"QuaternionPC.h"
			#endif

			#if defined PLATFORM_PS3
				#include	"QuaternionPS3.h"
			#endif

			#if defined PLATFORM_X360
				#include	"QuaternionX360.h"
			#endif
		};
	}
}

#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KMATH(Math/Geometry/More/Angle3D)

#if (!defined _DEBUG)
	#include	"Quaternion.inl"
#endif

#if (!defined _DEBUG)
	#include	"QuaternionGen.inl"
#endif

#if (!defined _DEBUG) && (defined PLATFORM_PS3)
	#include	"QuaternionPS3.inl"
#endif

#endif //__KMATH_MATH_QUATERNION_QUATERNION_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
