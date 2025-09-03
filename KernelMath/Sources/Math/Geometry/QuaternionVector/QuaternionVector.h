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
//	CLASS:	QUATERNION_VECTOR
//	The QUATERNION_VECTOR class implements ...
//
//	02-02-13:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_QUATERNIONVECTOR_QUATERNIONVECTOR_H__
#define __KMATH_MATH_QUATERNIONVECTOR_QUATERNIONVECTOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	QUATERNION_VECTOR
		{
		public:
			///////////////////////////////////////////////////////////////////////////////////
													QUATERNION_VECTOR();
													QUATERNION_VECTOR(const QUATERNION_VECTOR & C);
													~QUATERNION_VECTOR();
			_INLINE_	QUATERNION_VECTOR&			operator=(const QUATERNION_VECTOR & C);
			_INLINE_	void						Copy(const QUATERNION_VECTOR & C);
			///////////////////////////////////////////////////////////////////////////////////
			_INLINE_					void					SetQuat( const QUATERNION& Quat );
			_INLINE_					const QUATERNION&		GetQuat()const;
			_INLINE_					void					SetVector( const VECTOR& V );
			_INLINE_					const VECTOR&			GetVector()const;
			_INLINE_					void					Id();
			///////////////////////////////////////////////////////////////////////////////////
			_INLINE_					QUATERNION_VECTOR		operator*( const QUATERNION_VECTOR& QV )const;
										void					operator*=( const QUATERNION_VECTOR& QV );
			_INLINE_					VECTOR					operator*( const VECTOR& V )const;
			_INLINE_					QUATERNION				operator*( const QUATERNION& Q )const;
			_INLINE_					QUATERNION_VECTOR		operator*( float rVal )const;
										void					InterSLERP( float rVal, const QUATERNION_VECTOR& QV1, const QUATERNION_VECTOR& QV2);
										void					InterSLERPId( float rVal, const QUATERNION_VECTOR& QV );
										void					BasicInterSLERP( float rVal, const QUATERNION_VECTOR& QV1, const QUATERNION_VECTOR& QV2);
										void					BasicInterSLERPId( float rVal, const QUATERNION_VECTOR& QV );
										QUATERNION_VECTOR		operator/( const QUATERNION_VECTOR& QV );
			_INLINE_					void					Normalize();
			///////////////////////////////////////////////////////////////////////////////////
			void					Inverse();
			///////////////////////////////////////////////////////////////////////////////////

		//private:
			///////////////////////////////////////////////////////////////////////////////////
			QUATERNION					_Quat;
			VECTOR						_Vect;
			///////////////////////////////////////////////////////////////////////////////////
		};
	}
}

#ifndef _DEBUG
#include	"QuaternionVector.inl"
#endif

#endif //__KMATH_MATH_QUATERNIONVECTOR_QUATERNIONVECTOR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
