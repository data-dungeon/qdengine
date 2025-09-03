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
//	CLASS:	POINT_4D
//
//	01-09-26:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point4D/Point4D)
#include 	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
	#include "point4d.inl"
#endif

//-----------------------------------------------------------------------------
// Name: POINT4D::Read
// Object: 
//01-03-05 FKE Created: 
//-----------------------------------------------------------------------------
void	POINT4D::Read(IO_STREAM &s)
{
	s >> _r[_X_] >> _r[_Y_] >> _r[_Z_] >> _r[_W_];
}

//-----------------------------------------------------------------------------
// Name: POINT4D::Write
// Object: 
//01-03-05 FKE Created: 
//-----------------------------------------------------------------------------
void	POINT4D::Write(IO_STREAM &s) const
{
	s << _r[_X_] << _r[_Y_] << _r[_Z_] << _r[_W_];
}
