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
// CLASS: RAY
//
//
//00 - 11 - 09 JVA Created:
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include "Root.h"
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)

#include INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include "ray.inl"
#endif

//-----------------------------------------------------------------------------
//	Name: RAY::Read
//	Object: 
//	01-02-28:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY::Read(IO_STREAM &s)
{
	s >> _Origin >> _Direction ;
}

//=============================================================================
// CODE STARTS HERE
//=============================================================================
void	RAY::Write(IO_STREAM &s) const
{
	s << _Origin << _Direction ;
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		Transform a ray with a matrix
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
void	RAY::Transform( const MATRIX & Matrix )
{
	Matrix.TransformPosition(_Origin);
	Matrix.TransformDirection(_Direction);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
