//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: VECTOR
//
//
//00 - 11 - 10 JVA Created:
//*****************************************************************************

#include 	"Root.h"
#include	INCL_KMATH(Math/Math)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
	#include	"Vector.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================
const VECTOR		VECTOR::ZERO(0.0f, 0.0f, 0.0f);
const VECTOR		VECTOR::UNIT_X(1.0f, 0.0f, 0.0f);	
const VECTOR		VECTOR::UNIT_Y(0.0f, 1.0f, 0.0f);
const VECTOR		VECTOR::UNIT_Z(0.0f, 0.0f, 1.0f);
const VECTOR		VECTOR::NEG_UNIT_X(-1.0f,  0.0f,  0.0f);
const VECTOR		VECTOR::NEG_UNIT_Y( 0.0f, -1.0f,  0.0f);
const VECTOR		VECTOR::NEG_UNIT_Z( 0.0f,  0.0f, -1.0f);
const VECTOR		VECTOR::FLOAT_MAX(QDT_FLOAT32_MAX, QDT_FLOAT32_MAX, QDT_FLOAT32_MAX);
const VECTOR		VECTOR::FLOAT_MIN(-QDT_FLOAT32_MAX, -QDT_FLOAT32_MAX, -QDT_FLOAT32_MAX);

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-12-10:	cvi - Created
//-----------------------------------------------------------------------------
void	VECTOR::Read(IO_STREAM &s)
{
	VECTOR_GET_GENERIC_DATA();

	s >> _r[_X_] >> _r[_Y_] >> _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-12-10:	cvi - Created
//-----------------------------------------------------------------------------
void	VECTOR::Write(IO_STREAM &s) const
{
	VECTOR_GET_GENERIC_DATA();

	s << _r[_X_];
	s.DebugOutput("\t");

	s << _r[_Y_];
	s.DebugOutput("\t");

	s << _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		GetOrthoVector
//	Object:		
//	01-12-10:	cvi - Created
//-----------------------------------------------------------------------------
VECTOR	VECTOR::GetOrthoVector() const
{
	VECTOR_GET_GENERIC_DATA();

	if ( IsNull() == true )
	{
		return ( VECTOR( 1.0f, 0.0f, 0.0f) );
	}

	if ( ( MATH::Abs( _r[_X_] ) <= MATH::Abs( _r[_Y_] ) ) &&
		 ( MATH::Abs( _r[_X_] ) <= MATH::Abs( _r[_Z_] ) ) )
	{
		return ( VECTOR( 0.0f, _r[_Z_], -_r[_Y_] ) );
	}
	else
	if ( ( MATH::Abs( _r[_Y_] ) <= MATH::Abs( _r[_X_] ) ) &&
		 ( MATH::Abs( _r[_Y_] ) <= MATH::Abs( _r[_Z_] ) ) )
	{
		return ( VECTOR( -_r[_Z_], 0.0f, _r[_X_] ) );
	}
	else
	{
		QDT_ASSERT( MATH::Abs( _r[_Z_] ) <= MATH::Abs( _r[_X_] ) );
		QDT_ASSERT( MATH::Abs( _r[_Z_] ) <= MATH::Abs( _r[_Y_] ) );

		return ( VECTOR( _r[_Y_], -_r[_X_], 0.0f ) );
		
	}

} 

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-01-24:	SBE - Created
//-----------------------------------------------------------------------------
void	VECTOR::StaticCopy(VECTOR &		Dst,
							const VECTOR &	Src)
{
	StaticSlowCopy(Dst, Src);
}

//-----------------------------------------------------------------------------
//	Name:		SlowCopy
//	Object:		
//	03-01-24:	SBE - Created
//-----------------------------------------------------------------------------
void	VECTOR::StaticSlowCopy(VECTOR &		Dst,
								const VECTOR &	Src)
{
	Dst[_X_] = Src[_X_];
	Dst[_Y_] = Src[_Y_];
	Dst[_Z_] = Src[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
void	VECTOR::Display(COLOR_FLOAT &	Color,
						const float		rSize) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
	QDT_FAIL();
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
