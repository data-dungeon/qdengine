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
//	CLASS:	AABB_TREE_CELL
//
//	03-09-01:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/AABB_TreeCell)
#include	INCL_KMATH(Math/Geometry/VerticalRay/VerticalRay)


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		AABB_TREE_CELL constructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE_CELL::AABB_TREE_CELL()
:
_pLeftChild(NULL),
_pRightChild(NULL),
_nNbrIndices(0),
_pIndices(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AABB_TREE_CELL destructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE_CELL::~AABB_TREE_CELL()
{
	delete _pLeftChild;
	delete _pRightChild;
	delete [] _pIndices;

	_pLeftChild = NULL;
	_pRightChild = NULL;
	_pIndices = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SetIndex
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::SetIndex(unsigned int	i,
								 unsigned int	nIndex)
{
	QDT_ASSERT(i < _nNbrIndices);
	_pIndices[i] = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		CreateChilds
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::CreateChilds()
{
	QDT_ASSERT(_pLeftChild == NULL);
	QDT_ASSERT(_pRightChild == NULL);

	_pLeftChild = new AABB_TREE_CELL;
	_pRightChild = new AABB_TREE_CELL;
}

//-----------------------------------------------------------------------------
//	Name:		CreateIndex
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::CreateIndices(unsigned int	nNbrIndices)
{
	QDT_ASSERT(_pIndices == NULL);
	QDT_ASSERT(nNbrIndices > 0);
	
	_nNbrIndices = nNbrIndices;
	_pIndices = new unsigned int [_nNbrIndices];
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-09-05:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::Display(VECTOR * pPoints)
{
	QDT_NOT_IMPLEMENTED();
/*
	static COLOR_FLOAT Green(1.0f, 0.0f, 1.0f, 0.0f); 
//	_BoundingBox.Display(Green);

	if (_pIndices)
	{
		static QDT_VECTOR <SEGMENT> vSegments;
		vSegments.Clear();

		unsigned int i;
		for (i = 0 ; i < _nNbrIndices / 3 ; ++i)
		{
			unsigned int n1 = _pIndices[i * 3];
			unsigned int n2 = _pIndices[i * 3 + 1];
			unsigned int n3 = _pIndices[i * 3 + 2];

			SEGMENT s1(pPoints[n1], pPoints[n2]);
			SEGMENT s2(pPoints[n2], pPoints[n3]);
			SEGMENT s3(pPoints[n3], pPoints[n1]);

			vSegments.PushTail(s1);
			vSegments.PushTail(s2);
			vSegments.PushTail(s3);
		}

		RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegments(vSegments, Green);
	}

	if (_pLeftChild)
	{
		_pLeftChild->Display(pPoints);
		_pRightChild->Display(pPoints); 
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		TestZone
//	Object:		
//	03-09-05:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::TestZone(VECTOR *		pPoints,
								 VERTICAL_RAY &	Ray,
								 bool &			bAbove,
								 bool &			bBelow) const
{
	QDT_ASSERT(pPoints);
		
	if (_BoundingBox.Intersect(Ray))
	{
		unsigned int nTriangle;
		unsigned int nIndex = 0;
		unsigned int nNbrTriangles = _nNbrIndices / 3;
		for (nTriangle = 0 ; nTriangle < nNbrTriangles ; ++nTriangle)
		{
			unsigned int n0 = _pIndices[nIndex++];
			unsigned int n1 = _pIndices[nIndex++];
			unsigned int n2 = _pIndices[nIndex++];
			Ray.TestIntersection(pPoints[n0], pPoints[n1], pPoints[n2], bAbove, bBelow);
		}

		if (_pLeftChild)	// && _pRightChild
		{
			_pLeftChild->TestZone(pPoints, Ray, bAbove, bBelow);
			_pRightChild->TestZone(pPoints, Ray, bAbove, bBelow);
		}
	}
}
/*
//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	03-09-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB_TREE_CELL::RayCast(VECTOR * pPoints, 
								RAY_CAST_PARAMETERS & RayCastParameters,
								const RAY & Ray)
{
	float rDistance, rSquareDistance;

	if (!_BoundingBox.Intersect(Ray, rSquareDistance))
	{
		return (false);
	}

	if (rSquareDistance > RayCastParameters.GetImpactDistance() * RayCastParameters.GetImpactDistance())
	{
		return (false);
	}
	
	unsigned int nIndex = 0;
	bool bRes = false;
	
	while (nIndex < _nNbrIndices)
	{
		unsigned int n0 = _pIndices[nIndex++];
		unsigned int n1 = _pIndices[nIndex++];
		unsigned int n2 = _pIndices[nIndex++];

		TRIANGLE Triangle(pPoints[n0], pPoints[n1], pPoints[n2]);
		
		if (INTERSECTION::FindIntersectionRayTriangleWithCulling(Ray, Triangle, rDistance))
		{
			// Check if the distance is the nearest
			if (RayCastParameters.TestTriangleHit(Triangle, rDistance))
			{
				bRes = true;
			}
		}		
	}
	
	if (bRes) return (true);
	
	if (_pLeftChild)
	{
		if (_pLeftChild->RayCast(pPoints, RayCastParameters, Ray))
		{
			bRes = true;
		}

		if (_pRightChild->RayCast(pPoints, RayCastParameters, Ray))
		{
			bRes = true;
		}
	}
	
	return (bRes);
}
*/
#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::Read0(IO_STREAM &	Stream)
{
	Stream >> _BoundingBox;

	Stream >> _nNbrIndices;

	unsigned short nShort;

	if (_nNbrIndices > 0)
	{
		_pIndices = new unsigned int [_nNbrIndices];

		unsigned int nIndex;
		for (nIndex = 0 ; nIndex < _nNbrIndices ; ++nIndex)
		{
			Stream >> nShort;
			_pIndices[nIndex] = nShort;
		}
	}

	bool bHasChilds;
	Stream >> bHasChilds;

	if (bHasChilds)
	{
		CreateChilds();
		_pLeftChild->Read0(Stream);
		_pRightChild->Read0(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB_TREE_CELL::Read1(IO_STREAM &	Stream)
{
	Stream >> _BoundingBox;

	Stream >> _nNbrIndices;

	if (_nNbrIndices > 0)
	{
		_pIndices = new unsigned int [_nNbrIndices];

		unsigned int nIndex;
		for (nIndex = 0 ; nIndex < _nNbrIndices ; ++nIndex)
		{
			Stream >> _pIndices[nIndex];
		}
	}

	bool bHasChilds;
	Stream >> bHasChilds;

	if (bHasChilds)
	{
		CreateChilds();
		_pLeftChild->Read1(Stream);
		_pRightChild->Read1(Stream);
	}
}	
	
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	AABB_TREE_CELL::Write(IO_STREAM &	Stream)
{
	Stream.DebugOutput("AABB TREE : ");
	Stream << _BoundingBox;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of indices : ");
	Stream << _nNbrIndices;
	Stream.DebugOutput("\r\n");

	unsigned int nIndex;
	for (nIndex = 0 ; nIndex < _nNbrIndices ; ++nIndex)
	{	
		Stream.DebugOutput("Index : = ");
		Stream << _pIndices[nIndex];
		Stream.DebugOutput("\r\n");
	}

	if (_pLeftChild && _pRightChild)
	{
		Stream.DebugOutput("Have child ? : ");
		Stream << true;
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Left child");
		Stream.DebugOutput("\r\n");
		_pLeftChild->Write(Stream);		

		Stream.DebugOutput("Right child");
		Stream.DebugOutput("\r\n");
		_pRightChild->Write(Stream);
	}
	else
	{
		Stream << false;
	}
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
