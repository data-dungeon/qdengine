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
// CLASS: SKELETON
//
//
//	01-02-20:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetHair
//	Object:		
//	05-12-05:	ELE - Created
//-----------------------------------------------------------------------------
HAIR *	SKELETON::GetHair()
{
	return	(_pHair);
}

//-----------------------------------------------------------------------------
//	Name:		InitRagdollData
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::InitRagdollData(const COM_INSTANCE &	RagdollDataCI)
{
	_RagdollDataCI = RagdollDataCI;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
