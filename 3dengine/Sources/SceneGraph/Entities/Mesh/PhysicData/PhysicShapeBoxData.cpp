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
//	CLASS:	PHYSIC_SHAPE_BOX_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeBoxData)

#ifdef _DEBUG
	#include	"PhysicShapeBoxData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_SHAPE_BOX_DATA constructor
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_BOX_DATA::PHYSIC_SHAPE_BOX_DATA()
:
PHYSIC_SHAPE_DATA	(PHYSIC_SHAPE_DATA::PSDT_BOX),
_Dimensions			(0.0f, 0.0f, 0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_SHAPE_BOX_DATA destructor
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_BOX_DATA::~PHYSIC_SHAPE_BOX_DATA()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ReadChunk
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_BOX_DATA::ReadChunk(IO_STREAM &	Stream)
{
	PHYSIC_SHAPE_DATA::ReadChunk(Stream);

	Stream >> _Dimensions;
}

//-----------------------------------------------------------------------------
//	Name:		WriteChunk
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_BOX_DATA::WriteChunk(IO_STREAM &	Stream)
{
	PHYSIC_SHAPE_DATA::WriteChunk(Stream);

	Stream << _Dimensions;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
