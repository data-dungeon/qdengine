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
//	CLASS:	PARTICLE_SYSTEM
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetForceFields
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_VECTOR <COM_INSTANCE> &	PARTICLE_SYSTEM::GetForceFields() const
{
	return	(_vForceFieldsNodes);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
