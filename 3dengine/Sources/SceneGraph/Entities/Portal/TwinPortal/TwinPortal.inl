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
// CLASS: TWIN_PORTAL
//
//
//	01-03-29:	ELE - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		GetTwinPortalNodeIndex
//	Object:		
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
COM_INSTANCE TWIN_PORTAL::GetTwinPortalNodeInst() const
{
	return (_TwinPortalNodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		SetTwinPortalNodeIndex
//	Object:		
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void TWIN_PORTAL::SetTwinPortalNodeInst(const COM_INSTANCE & PortalNodeInst)
{
	_TwinPortalNodeInst = PortalNodeInst;
}