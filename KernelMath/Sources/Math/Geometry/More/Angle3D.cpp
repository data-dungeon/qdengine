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
// CLASS: ANGLE3D
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/More/Angle3D)
#include	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
#include "angle3d.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

void	ANGLE3D::Read(IO_STREAM &s)
{
	s >> _rAlpha >> _rBeta >> _rGamma;
}
void	ANGLE3D::Write(IO_STREAM &s) const
{
	s << _rAlpha << _rBeta << _rGamma;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================



