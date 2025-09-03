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
// CLASS: MATRIX
//
//
//00-11-08 GHP Created: 
//*****************************************************************************

//=============================================================================
// Name: MATRIX
// 00-11-08 GHP Created: 
//=============================================================================

#ifndef __KMATH_MATH_MATRIX_MATRIX_H__
#define __KMATH_MATH_MATRIX_MATRIX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PS3_MATH
#include	INCL_KCORE(Includes_PS3/Includes_PS3)

DECL_KCORE(IO_STREAM)
DECL_KMATH(VECTOR)
DECL_KMATH(ANGLE3D)
DECL_KMATH(QUATERNION)

namespace QDT
{
	namespace KMATH
	{
		class MATRIX
		{
			friend class VECTOR;
			friend class QUATERNION;

		public:
			
			// Constructors & destructor
			MATRIX() {}		// Default constructor don't initialize to identity!!
							// Please use Identity() explicitly or initialize
							// with the IDENTITY or ZERO matrix
			MATRIX(	Float32 r00, Float32 r01, Float32 r02, Float32 r03,
					Float32 r10, Float32 r11, Float32 r12, Float32 r13,
					Float32 r20, Float32 r21, Float32 r22, Float32 r23,
					Float32 r30, Float32 r31, Float32 r32, Float32 r33);
			MATRIX(const MATRIX& Mat)										{ QDT_APPEND_PLATFORM(Copy)(Mat); }
			MATRIX(Bool bSetIdentity)										{ if (bSetIdentity) Identity(); }
			~MATRIX() {}

			// Commonly used matrices...
			static const MATRIX		IDENTITY;
			static const MATRIX		ZERO;
			static const MATRIX		ROTATION_X_180;
			static const MATRIX		ROTATION_Y_180;
			static const MATRIX		ROTATION_Z_180;
			static const MATRIX		MIRROR_X;
			static const MATRIX		MIRROR_Y;
			static const MATRIX		MIRROR_Z;

			_INLINE_ 	void			Identity();
			_INLINE_ 	void			Zero();

			// Only authorised operators
			_INLINE_	MATRIX&			operator =  (const MATRIX& Mat)				{ return (QDT_APPEND_PLATFORM(Assign)(Mat)); }
						bool			operator == (const MATRIX& Mat) const;
						Float32			operator () (UInt8 nLin, UInt8 nCol) const  { return (GetElementGen(nLin, nCol)); }
						Float32&		operator () (UInt8 nLin, UInt8 nCol)	    { return (GetElementGen(nLin, nCol)); }
			
						void			SetPerspectiveFov(Float32 rHorizontalFieldOfVision, Float32 rAspectRatio, Float32 rZNearPlane, Float32 rZFarPlane);
						void			SetPerspectiveFov_D3D(Float32 rHorizontalFieldOfVision, Float32 rAspectRatio, Float32 rZNearPlane, Float32 rZFarPlane);
						void			SetPerspectiveFov_OGL(Float32 rHorizontalFieldOfVision, Float32 rAspectRatio, Float32 rZNearPlane, Float32 rZFarPlane);
						void			SetPerspectiveFovMayaOGL(Float32 rHorizontalFieldOfVision, Float32 rViewportAspectRatio, Float32 rZNearPlane, Float32 rZFarPlane, Float32 rScale);
						void			SetPerspectiveFovMayaD3D(Float32 rHorizontalFieldOfVision, Float32 rViewportAspectRatio, Float32 rZNearPlane, Float32 rZFarPlane, Float32 rScale);
						void			SetOrthogonalProjLeftHanded(Float32 rXmin, Float32 rXmax, Float32 rYmin, Float32 rYmax, Float32 rZmin, Float32 rZmax);
						void			LookAt(const VECTOR & vEyePoint, const VECTOR & vLookAtPoint, const VECTOR & vUp);
						void			LookAtLH(const VECTOR & vEyePoint, const VECTOR & vLookAt, const VECTOR & vUp);

			_INLINE_ 	void			SetRight(const VECTOR& vAxisRight);
			_INLINE_ 	void			SetUp(const VECTOR& vAxisUp);
			_INLINE_ 	void			SetAt(const VECTOR& vAxisAt);

			_INLINE_ 	const VECTOR& 	GetRight(void) const;
			_INLINE_ 	const VECTOR& 	GetUp(void) const;
			_INLINE_ 	const VECTOR& 	GetAt(void) const;

			_INLINE_ 	void			MakeAffine();	// Put the last column to (0, 0, 0, 1)
			_INLINE_ 	void			Transpose()																									{ QDT_APPEND_PLATFORM(Transpose)(); }
						void			TransformPosition(const VECTOR& Src, VECTOR& Dst) const														{ QDT_APPEND_PLATFORM(TransformPosition)(Src, Dst); }
						void			TransformDirection(const VECTOR& Src, VECTOR& Dst) const													{ QDT_APPEND_PLATFORM(TransformDirection)(Src, Dst); }
			_INLINE_	void			TransformPosition(VECTOR & V) const																			{ QDT_APPEND_PLATFORM(TransformPosition)(V); }
			_INLINE_	void			TransformDirection(VECTOR & V) const																		{ QDT_APPEND_PLATFORM(TransformDirection)(V); }
						void			BatchTransformPosition(const VECTOR *pSrc, VECTOR *pDst, unsigned int *pIndices, int nNbrVertices) const	{ QDT_APPEND_PLATFORM(BatchTransformPosition)(pSrc, pDst, pIndices, nNbrVertices); }
						void			BatchTransformDirection(const VECTOR *pSrc, VECTOR *pDst, unsigned int *pIndices, int nNbrVertices) const	{ QDT_APPEND_PLATFORM(BatchTransformDirection)(pSrc, pDst, pIndices, nNbrVertices); }
						void			BatchTransformPosition(const VECTOR *pSrc, VECTOR *pDst, unsigned int nNbrVertices) const;
						void			BatchTransformDirection(const VECTOR *pSrc, VECTOR *pDst, unsigned int nNbrVertices) const;
						void			BatchTransformPosition(VECTOR *pDstSrc, unsigned int nNbrVertices) const;
						void			BatchTransformDirection(VECTOR *pDstSrc, unsigned int nNbrVertices) const;
						void			BatchTransformPoint(VECTOR *pDstSrc, unsigned int nNbrVertices) const;
						void			Mul(const MATRIX & M1, const MATRIX & M2)		{ QDT_APPEND_PLATFORM(Mul)(M1, M2); }
						void			Mul(const MATRIX & M)							{ QDT_APPEND_PLATFORM(Mul)(M); }
						void			Mul(Float32 r)									{ QDT_APPEND_PLATFORM(Mul)(r); }
			
						void			MulProject(const VECTOR& In, VECTOR& Out) const;

						void			MulAffine(const MATRIX & M1, const MATRIX & M2) { QDT_APPEND_PLATFORM(MulAffine)(M1, M2); }
						void			MulAffine(const MATRIX & M)						{ QDT_APPEND_PLATFORM(MulAffine)(M); }
						void			MulAffine(Float32 r)							{ QDT_APPEND_PLATFORM(MulAffine)(r); }

						void			Add(const MATRIX & M1, const MATRIX & M2)		{ QDT_APPEND_PLATFORM(Add)(M1, M2); }
						void			Add(const MATRIX & M)							{ QDT_APPEND_PLATFORM(Add)(M); }

						void			AddAffine(const MATRIX & M1, const MATRIX & M2)	{ QDT_APPEND_PLATFORM(AddAffine)(M1, M2); }
						void			AddAffine(const MATRIX & M)						{ QDT_APPEND_PLATFORM(AddAffine)(M); }
						void			Square()										{ QDT_APPEND_PLATFORM(Square)(); }
						void			SquareAffine()									{ QDT_APPEND_PLATFORM(SquareAffine)(); }
						void			Inverse(const MATRIX& MatrixSource)				{ QDT_APPEND_PLATFORM(Inverse)(MatrixSource); }
						void			InversePerspective(const MATRIX & Mat);
						void			InverseAffine(const MATRIX & Mat)				{ QDT_APPEND_PLATFORM(InverseAffine)(Mat); }
						void			InverseAffineNoScale(const MATRIX & Mat)		{ QDT_APPEND_PLATFORM(InverseAffineNoScale)(Mat); }
						void			GetTranspose(MATRIX & Transpose)				{ QDT_APPEND_PLATFORM(GetTranspose)(Transpose); }
						bool			IsIdentity() const;
						bool			IsAffine() const;
						bool			HasNoScale() const;
						bool			HasNoTranslation() const;
						void			Target(const VECTOR & TargetPos, bool bReverse = false);
						void			SetPerspectiveProjection(Float32 rFOV, Float32 rNearPlane, Float32 rFarPlane, Float32 rViewportAspect);
						void			SetRotX(Float32 rAlpha);
						void			SetRotY(Float32 rBeta);
						void			SetRotZ(Float32 rGamma);
			
			// Rotations
			_INLINE_ 	void			SetRotXYZ(const ANGLE3D & Angle);
			_INLINE_ 	void			SetRotXYZ(const VECTOR & vAngle);
						VECTOR 			GetRotXYZ() const;
						void			SetRotXYZ(Float32 rX, Float32 rY, Float32 rZ)		{ QDT_APPEND_PLATFORM(SetRotXYZ)(rX, rY, rZ); }
			_INLINE_	void			SetRotation(const VECTOR & Axe, const Float32 rAngle);
						void			SetRotation(const VECTOR & Vector1, const VECTOR & Vector2, Float32 rRoll);
				
			// Arithmetic operations	
						void			Mul3x3(const MATRIX & Mat1, const MATRIX & Mat2)	{ QDT_APPEND_PLATFORM(Mul3x3)(Mat1, Mat2); }
				
			// Translation
			_INLINE_ 	VECTOR			GetTranslation() const ;
			_INLINE_ 	void			GetPosition(VECTOR & Vector) const;
			_INLINE_ 	void			SetTranslation(const VECTOR & Vector) ;
			_INLINE_ 	void			SetPosition(const VECTOR & Vector) ;
			_INLINE_ 	void			SetTranslation(Float32 rX, Float32 rY, Float32 rZ);
			_INLINE_ 	void			ResetTranslation() ;
			_INLINE_ 	void			ResetRotation() ;

			// Quaternion
						void			GetQuaternion(QUATERNION& Quat) const				{ QDT_APPEND_PLATFORM(GetQuaternion)(Quat); }
						void			SetQuaternion(const QUATERNION & Quat)				{ QDT_APPEND_PLATFORM(SetQuaternion)(Quat); }
						void			SlowSetQuaternion(const QUATERNION & Quat) ;
				
			// Scale
			_INLINE_	void			SetScale(const VECTOR & Scale) ;
			
			// Filter
			_INLINE_ 	void			FilterRotXYZ( bool bX, bool bY, bool bZ);
			_INLINE_ 	void			FilterTrans( bool bX, bool bY, bool bZ);
			_INLINE_ 	void			FilterPitchHeadRoll(bool bPitch, bool bHead, bool bRoll);
						void			GetPitchHeadRoll(QUATERNION& QPitch, QUATERNION& QHead, QUATERNION& QRoll)const;
						void			SetPitchHeadRoll( Float32 rPitch, Float32 rHead, Float32 rRoll );
						void			SetHeadPitchRoll( Float32 rHead, Float32 rPitch, Float32 rRoll );

			//Euler operations
						void			SetEulerAnglesYXZ(Float32 rYAngle, Float32 rXAngle, Float32 rZAngle);	//head/pitch/roll
						Bool			GetEulerAnglesYXZ(Float32& rYAngle, Float32& rXAngle, Float32& rZAngle) const;

			// Unit Quaternions
			_INLINE_	bool			QPitch( QUATERNION& Qx )const ; 
			_INLINE_	bool			QHeading( QUATERNION& Qy )const ; 
						bool			QRoll( QUATERNION& Qz )const; 

						void			Apply(const VECTOR & Src, VECTOR & Res) const		{ QDT_APPEND_PLATFORM(Apply)(Src, Res); }
			_INLINE_	void			Apply(VECTOR & Point) const ;
			
			_INLINE_	const Float32*	ToPointer() const;
			_INLINE_	Float32*		ToPointer();
						VECTOR			GetBasis(const unsigned int Index) const;
						VECTOR			GetImageBasis(const unsigned int Index) const;
						VECTOR			ApplyRotation(const VECTOR &V) const;

						void			SetFromPtr(const Float32 Ptr[16]);

						void			ClearRotation();
						void			Set(const MATRIX &M);
						void			SlowSet(const MATRIX &M);

			// Streams
						void			Read(IO_STREAM &s);
						void			Write(IO_STREAM &s) const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const MATRIX &m)
			{
				m.Write(s);
				return s;
			}

			friend IO_STREAM&	operator >>(IO_STREAM &s, MATRIX &m)
			{
				m.Read(s);
				return s;
			}


		private:

						void			Copy(const MATRIX & Mat)			{ QDT_APPEND_PLATFORM(Copy)(Mat); }
						void			SlowCopy(const MATRIX & Mat);
						void			Transpose3x3(const MATRIX & Mat)	{ QDT_APPEND_PLATFORM(Transpose3x3)(Mat); }

			// Include platform specific .h files here
			#include	"MatrixGen.h"

			#if defined PLATFORM_PC
				#include	"MatrixPC.h"
			#endif

			#if defined PLATFORM_PS3
				#include	"MatrixPS3.h"
			#endif

			#if defined PLATFORM_X360
				#include	"MatrixX360.h"
			#endif
		};
	}
}

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#if (!defined _DEBUG)
	#include	"Matrix.inl"
#endif

#if (!defined _DEBUG)
	#include	"MatrixGen.inl"
#endif

#if (!defined _DEBUG) && (defined PLATFORM_PS3)
	#include	"MatrixPS3.inl"
#endif

#endif //__KMATH_MATH_MATRIX_MATRIX_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
