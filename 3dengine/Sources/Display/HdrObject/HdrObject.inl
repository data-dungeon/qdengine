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
//	CLASS:	HDR_OBJECT
//
//	04-05-19:	FPR - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetNode
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	HDR_OBJECT::SetNode(NODE *	pNode)
{
	_pNode = pNode;
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometryIndex
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	HDR_OBJECT::SetGeometryIndex(unsigned int	n)
{
	_nGeometryIndex = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinClusterIndex
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	HDR_OBJECT::SetSkinClusterIndex(unsigned int	n)
{
	_nSkinClusterIndex = n;
}	
	
//-----------------------------------------------------------------------------
//	Name:		GetSkinClusterIndex
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
unsigned int	HDR_OBJECT::GetSkinClusterIndex() const
{
	return (_nSkinClusterIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
NODE *	HDR_OBJECT::GetNode()
{
	return (_pNode);
}

//-----------------------------------------------------------------------------
//	Name:		SetNormalMeshDisplay
//	Object:		
//	04-06-22:	FPR - Created
//-----------------------------------------------------------------------------
void	HDR_OBJECT::SetNormalMeshDisplay(bool	b)
{
	_bNormalMeshDisplay = b;
}

//-----------------------------------------------------------------------------
//	Name:		IsNormalMeshDisplay
//	Object:		
//	04-06-22:	FPR - Created
//-----------------------------------------------------------------------------
bool	HDR_OBJECT::IsNormalMeshDisplay()
{
	return (_bNormalMeshDisplay);
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
