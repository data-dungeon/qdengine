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
//	CLASS:	MOTEUR_3D
//
//	02-07-10:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
MOTEUR_3D *	MOTEUR_3D::Instance()
{
	return	((MOTEUR_3D *)GetSingletonDescriptor()->Instance());
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::Release()
{
	GetSingletonDescriptor()->Release();
}

//-----------------------------------------------------------------------------
//	Name:		Exist
//	Object:		
//	03-04-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	MOTEUR_3D::IsInstancied()
{
	return (GetSingletonDescriptor()->IsInstancied());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
