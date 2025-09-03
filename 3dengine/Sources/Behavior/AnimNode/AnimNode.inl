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
//	CLASS:	ANIM_NODE
//
//	01-10-11:	cvi - Created
//*****************************************************************************


//=============================================================================
//	CODE STARTS HERE
//=============================================================================



//-----------------------------------------------------------------------------
//	Name:		GetAnimStruct
//	Object:		
//	03-03-18:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT *	ANIM_NODE::GetAnimStruct() const
{
	return ( _pStruct );
}


//-----------------------------------------------------------------------------
//	Name:		SetAnimStruct
//	Object:		
//	03-03-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE::SetAnimStruct(ANIM_NODE_STRUCT *	pS)
{
	_pStruct = pS;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
