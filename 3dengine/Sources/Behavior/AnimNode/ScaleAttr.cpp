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
//	CLASS:	SCALE_ATTR
//
//	01-10-11:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"ScaleAttr.h"

#ifdef _DEBUG
	#include "ScaleAttr.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SCALE_ATTR constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
SCALE_ATTR::SCALE_ATTR()
:
_vTransScale( 1.0f, 1.0f, 1.0f )
{
}

//-----------------------------------------------------------------------------
//	Name:		SCALE_ATTR constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
SCALE_ATTR::SCALE_ATTR(const SCALE_ATTR & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		SCALE_ATTR destructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
SCALE_ATTR::~SCALE_ATTR()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
SCALE_ATTR &	SCALE_ATTR::operator=(const SCALE_ATTR & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	SCALE_ATTR::Copy(const SCALE_ATTR & C)
{
	_vTransScale = C._vTransScale;
}

//-----------------------------------------------------------------------------
//	Name:		ScaleVector
//	Object:		
//	01-10-24:	cvi - Created
//-----------------------------------------------------------------------------
void	SCALE_ATTR::ScaleVector(VECTOR &	V) const
{
	float	rX, rY, rZ;
	float	rsX, rsY, rsZ;

	///////////////////////////////////////
	_vTransScale.GetXYZ( rsX, rsY, rsZ );
	V.GetXYZ( rX, rY, rZ );
	///////////////////////////////////////

	///////////////////////////////////////
	rX *= rsX;
	rY *= rsY;
	rZ *= rsZ;
	///////////////////////////////////////
	V.SetXYZ( rX, rY, rZ );
	///////////////////////////////////////
	///////////////////////////////////////
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
