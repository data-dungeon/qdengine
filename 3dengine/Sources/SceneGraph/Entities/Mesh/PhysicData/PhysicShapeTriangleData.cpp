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
//	CLASS:	PHYSIC_SHAPE_TRIANGLE_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeTriangleData)

#ifdef _DEBUG
	#include	"PhysicShapeTriangleData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_SHAPE_TRIANGLE_DATA constructor
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_TRIANGLE_DATA::PHYSIC_SHAPE_TRIANGLE_DATA()
:
PHYSIC_SHAPE_DATA	(PHYSIC_SHAPE_DATA::PSDT_TRIANGLE),
_GeometricDataSize	(0),
_pGeometricData		(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_SHAPE_TRIANGLE_DATA destructor
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_TRIANGLE_DATA::~PHYSIC_SHAPE_TRIANGLE_DATA()
{
	delete [] (Char*) _pGeometricData;
}

//-----------------------------------------------------------------------------
//	Name:		ReadChunk
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_TRIANGLE_DATA::ReadChunk(IO_STREAM &	Stream)
{
	PHYSIC_SHAPE_DATA::ReadChunk(Stream);

	UInt32	nType;
	Stream >> nType;
	_Type = static_cast<PHYSIC_SHAPE_TRIANGLE_TYPE>(nType);

	Stream >> _GeometricDataSize;

	_pGeometricData = static_cast<void *>(new UInt8[_GeometricDataSize]);
	Stream.GetBufferFast(_pGeometricData, _GeometricDataSize);
}

//-----------------------------------------------------------------------------
//	Name:		WriteChunk
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_TRIANGLE_DATA::WriteChunk(IO_STREAM &	Stream)
{
	PHYSIC_SHAPE_DATA::WriteChunk(Stream);

	Stream << _Type;
	Stream << _GeometricDataSize;
	Stream.PutBufferFast(_pGeometricData, _GeometricDataSize);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
