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
//	CLASS:	BI_QUAT_VECT
//
//	02-02-27:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"BiQuatVect.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BI_QUAT_VECT constructor
//	Object:		
//	02-02-27:	cvi - Created
//-----------------------------------------------------------------------------
BI_QUAT_VECT::BI_QUAT_VECT()
{

}

//-----------------------------------------------------------------------------
//	Name:		BI_QUAT_VECT constructor
//	Object:		
//	02-02-27:	cvi - Created
//-----------------------------------------------------------------------------
BI_QUAT_VECT::BI_QUAT_VECT(const BI_QUAT_VECT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BI_QUAT_VECT destructor
//	Object:		
//	02-02-27:	cvi - Created
//-----------------------------------------------------------------------------
BI_QUAT_VECT::~BI_QUAT_VECT()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-02-27:	cvi - Created
//-----------------------------------------------------------------------------
BI_QUAT_VECT &	BI_QUAT_VECT::operator=(const BI_QUAT_VECT & C)
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
//	02-02-27:	cvi - Created
//-----------------------------------------------------------------------------
void	BI_QUAT_VECT::Copy(const BI_QUAT_VECT & C)
{
	_Relative = C._Relative;
	_Absolute = C._Absolute;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
