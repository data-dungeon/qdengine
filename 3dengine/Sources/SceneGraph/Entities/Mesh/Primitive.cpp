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
//	CLASS:	PRIMITIVE
//
//	05-10-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Primitive)

#ifdef _DEBUG
	#include	"Primitive.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE constructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
PRIMITIVE::PRIMITIVE()
:
_Type					(PT_TRIANGLE_LIST),
_bPerPolygoneSorting	(false),
_nVertexBufferIdx		(0),
_nFirstVertex			(0),
_nNbrVertices			(0),
_nIndexBufferIdx		(0),
_nFirstIndex			(0),
_nNbrIndices			(0),
_nNbrJoints				(0),
_pJointIndices			(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE destructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
PRIMITIVE::~PRIMITIVE()
{
	if(_pJointIndices)
	{
		delete	(_pJointIndices);
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
PRIMITIVE &	PRIMITIVE::operator=(const PRIMITIVE & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::Copy(const PRIMITIVE & C)
{
	_Type = C._Type;
	_bPerPolygoneSorting = C._bPerPolygoneSorting;
	_nVertexBufferIdx = C._nVertexBufferIdx;
	_nFirstVertex = C._nFirstVertex;
	_nNbrVertices = C._nNbrVertices;
	_nIndexBufferIdx = C._nIndexBufferIdx;
	_nFirstIndex = C._nFirstIndex;
	_nNbrIndices = C._nNbrIndices;
	_nNbrJoints = C._nNbrJoints;
	_pJointIndices = C._pJointIndices;
	_ciShader = C._ciShader;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
