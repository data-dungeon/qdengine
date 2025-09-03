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
// CLASS: QUATERNION
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: QUATERNION
//00-11-09 JVA Created: 
//=============================================================================

// Macros that casts the platform data structure into the generic data structure (for generic code compatibility)
// Call this macro at the beginning of every methods that manipulates _r directly.
#define QUAT_GET_GENERIC_DATA()
#define QUAT_GET_GENERIC_DATA_PARAM(Q, PARAM) QUATERNION & Q = (QUATERNION&) PARAM;

	private:
	// Put platform specific Data structure here
		Float32	_r[4];

//=============================================================================
// CODE ENDS HERE
//=============================================================================
