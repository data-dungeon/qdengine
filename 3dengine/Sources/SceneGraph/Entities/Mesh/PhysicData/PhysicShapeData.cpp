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
//	CLASS:	PHYSIC_SHAPE_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)

#ifdef _DEBUG
	#include	"PhysicShapeData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_SHAPE_DATA constructor
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_DATA::PHYSIC_SHAPE_DATA(PHYSIC_SHAPE_DATA_TYPE Type)
:
_Type	(Type)
{
	_LocalMatrix.Identity();
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_SHAPE_DATA destructor
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_DATA::~PHYSIC_SHAPE_DATA()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ReadChunk
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_DATA::ReadChunk(IO_STREAM &	Stream)
{
	// Type is read earlier
	Stream >> _LocalMatrix;
}

//-----------------------------------------------------------------------------
//	Name:		WriteChunk
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_DATA::WriteChunk(IO_STREAM &	Stream)
{
	// Type is written earlier
	Stream << _LocalMatrix;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
