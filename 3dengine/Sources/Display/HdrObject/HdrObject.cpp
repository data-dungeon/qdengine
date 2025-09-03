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

#include	"Root.h"
#include	INCL_3DENGINE(Display/HdrObject/HdrObject)
#include	INCL_3DENGINE(SceneGraph/Node/Node)

#ifdef _DEBUG
	#include	"HdrObject.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		HDR_OBJECT constructor
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
HDR_OBJECT::HDR_OBJECT()
{
	//Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		HDR_OBJECT constructor
//	Object:		
//	04-05-24:	FPR - Created
//-----------------------------------------------------------------------------
HDR_OBJECT::HDR_OBJECT(NODE *		pNode,
					   unsigned int	nGeometryIndex,
					   bool bNormalDisplay)
:
_pNode				(pNode),
_nGeometryIndex		(nGeometryIndex),
_nSkinClusterIndex	(0),
_bNormalMeshDisplay	(bNormalDisplay)
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		HDR_OBJECT constructor
//	Object:		
//	04-05-24:	FPR - Created
//-----------------------------------------------------------------------------
HDR_OBJECT::HDR_OBJECT(NODE *		pNode,
					   unsigned int	nGeometryIndex,
					   unsigned int	nSkinClusterIndex,
					   bool bNormalDisplay)
:
_pNode				(pNode),
_nGeometryIndex		(nGeometryIndex),
_nSkinClusterIndex	(nSkinClusterIndex),
_bNormalMeshDisplay	(bNormalDisplay)
{
	//Do nothing
}



//-----------------------------------------------------------------------------
//	Name:		HDR_OBJECT destructor
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
HDR_OBJECT::~HDR_OBJECT()
{
	//Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	HDR_OBJECT::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	_pNode->DisplayHdrObject(DisplayParameters, _nGeometryIndex, _nSkinClusterIndex);
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
