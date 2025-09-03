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
//	CLASS:	OBR
//
//	01-10-08:	GGO - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	OBR::GetCenter() const
{
	return ( _Center );
}

//-----------------------------------------------------------------------------
//	Name:		GetDirection1
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	OBR::GetDirection1() const
{
	return ( _vDirection1 );
}

//-----------------------------------------------------------------------------
//	Name:		GetDirection2
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	OBR::GetDirection2() const
{
	return ( _vDirection2 );
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance1
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
const float	OBR::GetDistance1() const
{
	return ( _rDistance1 );
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance2
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
const float	OBR::GetDistance2() const
{
	return ( _rDistance2 );
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
