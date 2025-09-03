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
//	CLASS:	VECTOR_2
//
//	03-03-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SPHERICAL_HARMONICS constructor
//	Object:		
//	03-03-10:	RMA - Created
//-----------------------------------------------------------------------------
SPHERICAL_HARMONICS::SPHERICAL_HARMONICS()
{
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR_2 destructor
//	Object:		
//	03-03-10:	RMA - Created
//-----------------------------------------------------------------------------
SPHERICAL_HARMONICS::~SPHERICAL_HARMONICS()
{
}



//-----------------------------------------------------------------------------
//	Name:		GetRedComponents
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32 *	SPHERICAL_HARMONICS::GetRedComponents()
{
	return _redComponents;
}

//-----------------------------------------------------------------------------
//	Name:		GetGreenComponents
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32 *	SPHERICAL_HARMONICS::GetGreenComponents()
{
	return _greenComponents;
}

//-----------------------------------------------------------------------------
//	Name:		GetBlueComponents
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32 *	SPHERICAL_HARMONICS::GetBlueComponents()
{
	return _blueComponents;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
