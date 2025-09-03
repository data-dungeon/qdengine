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
//	CLASS:	AABB_TREE
//
//	03-09-01:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/AABB_Tree)
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/AABB_TreeCell)
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/AABB_TreeCellCompressed)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		AABB_TREE constructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE::AABB_TREE()
:
_nNbrPoints(0),
_pPoints(NULL),
_pCompressedRoot(NULL),
_pRoot(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AABB_TREE destructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE::~AABB_TREE()
{
	delete [] _pPoints;
	delete _pRoot;
	delete _pCompressedRoot;
	_pPoints = NULL;
	_pCompressedRoot = NULL;
	_pRoot = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		GetRoot
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE_CELL *	AABB_TREE::GetRoot() const
{
	return (_pRoot);
}

//-----------------------------------------------------------------------------
//	Name:		GetRoot
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE_CELL_COMPRESSED *	AABB_TREE::GetCompressedRoot() const
{
	return (_pCompressedRoot);
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::SetPoint(unsigned int	i,
							const VECTOR&	P)
{
	QDT_ASSERT(i < _nNbrPoints);
	_pPoints[i] = P;
}

//-----------------------------------------------------------------------------
//	Name:		CreateRoot
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::CreateRoot()
{
	_pRoot = new AABB_TREE_CELL;
}

//-----------------------------------------------------------------------------
//	Name:		CreateRoot
//	Object:		
//	04-04-16:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::CreateCompressedRoot()
{
	_pCompressedRoot = new AABB_TREE_CELL_COMPRESSED;
}

//-----------------------------------------------------------------------------
//	Name:		CreatePoints
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::CreatePoints(unsigned int	nNbrPoints)
{
	QDT_ASSERT(_pPoints == NULL);
	QDT_ASSERT(nNbrPoints > 0);

	_nNbrPoints = nNbrPoints;
	_pPoints = new VECTOR [_nNbrPoints];
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::Read0(IO_STREAM &	Stream)
{
	Stream >>  _nNbrPoints;
	QDT_ASSERT(_nNbrPoints > 0);
	
	_pPoints = new VECTOR [_nNbrPoints];

	unsigned int nPoint;
	for (nPoint = 0 ; nPoint < _nNbrPoints ; ++nPoint)
	{
		float rX, rY, rZ;
		Stream >> rX >> rY >> rZ;
		_pPoints[nPoint].SetX(rX);
		_pPoints[nPoint].SetY(rY);
		_pPoints[nPoint].SetZ(rZ);
	}

	if (_nNbrPoints > 0)
	{
		_pRoot = new AABB_TREE_CELL;
		_pRoot->Read0(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::Read1(IO_STREAM &	Stream)
{
	Stream >>  _nNbrPoints;
	QDT_ASSERT(_nNbrPoints > 0);
	
	_pPoints = new VECTOR [_nNbrPoints];

	unsigned int nPoint;
	for (nPoint = 0 ; nPoint < _nNbrPoints ; ++nPoint)
	{
		float rX, rY, rZ;
		Stream >> rX >> rY >> rZ;
		_pPoints[nPoint].SetX(rX);
		_pPoints[nPoint].SetY(rY);
		_pPoints[nPoint].SetZ(rZ);
	}

	if (_nNbrPoints > 0)
	{
		_pRoot = new AABB_TREE_CELL;
		_pRoot->Read1(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	04-04-16:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::Read2(IO_STREAM & Stream)
{
	Stream >>  _nNbrPoints;
	QDT_ASSERT(_nNbrPoints > 0);
	
	_pPoints = new VECTOR [_nNbrPoints];

	unsigned int nPoint;
	for (nPoint = 0 ; nPoint < _nNbrPoints ; ++nPoint)
	{
		float rX, rY, rZ;
		Stream >> rX >> rY >> rZ;
		_pPoints[nPoint].SetX(rX);
		_pPoints[nPoint].SetY(rY);
		_pPoints[nPoint].SetZ(rZ);
	}

	if (_nNbrPoints > 0)
	{
		int nCompression;
		Stream >> nCompression;
		if (nCompression)
		{
			_pCompressedRoot = new AABB_TREE_CELL_COMPRESSED;
			_pCompressedRoot->Read(Stream);
		}
		else
		{
			_pRoot = new AABB_TREE_CELL;
			_pRoot->Read1(Stream);
		}
	}
}

#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	AABB_TREE::Write(IO_STREAM &	Stream)
{
	Stream.DebugOutput("AABB TREE : \r\n");
	Stream.DebugOutput("Number of points : ");
	Stream << _nNbrPoints;
	Stream.DebugOutput("\r\n");

	unsigned int nPoint;
	for (nPoint = 0 ; nPoint < _nNbrPoints ; ++nPoint)
	{
		Stream.DebugOutput("Point : x = ");
		Stream << _pPoints[nPoint].GetX();
		Stream.DebugOutput(", y = ");
		Stream << _pPoints[nPoint].GetY();
		Stream.DebugOutput(", z = ");
		Stream << _pPoints[nPoint].GetZ();
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");

	if (_nNbrPoints > 0)
	{	
		int nCompression;

		if (_pCompressedRoot)
		{
			QDT_ASSERT(_pCompressedRoot);
			nCompression = true;
			Stream << nCompression;
			Stream.DebugOutput("Compressed Root");
			_pCompressedRoot->Write(Stream);
		}
		else
		{
			QDT_ASSERT(_pRoot);
			nCompression = false;
			Stream << nCompression;
			Stream.DebugOutput("Root");
			_pRoot->Write(Stream);
		}
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-09-05:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::Display()
{
	if (_pCompressedRoot)
	{
		_pCompressedRoot->Display(_pPoints);
	}
	else if (_pRoot)
	{
		_pRoot->Display(_pPoints);
	}
}

//-----------------------------------------------------------------------------
//	Name:		TestZone
//	Object:		
//	03-09-05:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE::TestZone(VERTICAL_RAY &	Ray,
							bool &			bAbove,
							bool &			bBelow) const
{
	if (_pCompressedRoot)
	{
		_pCompressedRoot->TestZone(_pPoints, Ray, bAbove, bBelow);
	}
	else if (_pRoot)
	{
		_pRoot->TestZone(_pPoints, Ray, bAbove, bBelow);
	}
}
/*
//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	03-09-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB_TREE::RayCast(RAY_CAST_PARAMETERS &	RayCastParameters)
{
	if (_pCompressedRoot)
	{
		return (_pCompressedRoot->RayCast(_pPoints, RayCastParameters, RayCastParameters.GetRay()));
	}
	else if (_pRoot)
	{
		return (_pRoot->RayCast(_pPoints, RayCastParameters, RayCastParameters.GetRay()));
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	03-09-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB_TREE::RayCast(RAY_CAST_PARAMETERS & RayCastParameters, 
						   const RAY & LocalRay)
{
	if (_pCompressedRoot)
	{
		return (_pCompressedRoot->RayCast(_pPoints, RayCastParameters, LocalRay));
	}
	else if (_pRoot)
	{
		return (_pRoot->RayCast(_pPoints, RayCastParameters, LocalRay));
	}

	return (false);
}
*/
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
