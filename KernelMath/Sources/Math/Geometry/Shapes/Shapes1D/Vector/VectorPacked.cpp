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
// CLASS: VECTOR_PACKED
//
//
//00 - 11 - 10 JVA Created:
//*****************************************************************************

#include	"Root.h"
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/VectorPacked)

#include INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include "VectorPacked.inl"
#endif

void VECTOR_PACKED::Read(IO_STREAM &s)
{
	s >> _cX >> _cY >> _cZ;
}

void VECTOR_PACKED::Write(IO_STREAM &s) const
{
	s << _cX << _cY << _cZ;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
