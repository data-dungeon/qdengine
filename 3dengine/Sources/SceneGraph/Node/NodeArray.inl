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
//	CLASS:	NODE_ARRAY
//
//	06-01-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetNbrChildren
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	NODE_ARRAY::GetNbrChildren() const
{
	return	(_uiNbrChildren);
}

//-----------------------------------------------------------------------------
//	Name:		GetChild
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_CHILD *	NODE_ARRAY::GetChild(UInt32	uiIndex)
{
	return	(&_pChild[uiIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrChildren
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::SetNbrChildren(UInt32	uiNbrChildren)
{
	_uiNbrChildren = uiNbrChildren;
}

//-----------------------------------------------------------------------------
//	Name:		SetChild
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::SetChild(NODE_CHILD *	pChild)
{
	_pChild = pChild;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
