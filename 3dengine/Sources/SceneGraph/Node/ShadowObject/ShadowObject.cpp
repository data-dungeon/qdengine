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
//	CLASS:	SHADOW_OBJECT
//
//	03-10-23:	FPR - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Node/ShadowObject/ShadowObject)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SHADOW_OBJECT constructor
//	Object:		
//	03-10-23:	FPR - Created
//-----------------------------------------------------------------------------
SHADOW_OBJECT::SHADOW_OBJECT()
{
	_vGeomIndex.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SHADOW_OBJECT destructor
//	Object:		
//	03-10-23:	FPR - Created
//-----------------------------------------------------------------------------
SHADOW_OBJECT::~SHADOW_OBJECT()
{
	// Do Nothing
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
