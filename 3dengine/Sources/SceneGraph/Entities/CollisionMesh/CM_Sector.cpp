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
//	CLASS:	CM_SECTOR
//
//	03-08-27:	JPE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Sector)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Edge)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

//-----------------------------------------------------------------------------
//	Name:		Constructor
//	Object:		
//	03-08-27:	JPE - Created
//-----------------------------------------------------------------------------
CM_SECTOR::CM_SECTOR()
:
_pIndexes(0),
_nEntries(0),
_pFaceIndexes(0),
_nFacesEntries(0),
_Flags(CMSF_NONE)
{ 
}

//-----------------------------------------------------------------------------
//	Name:		Destructor
//	Object:		
//	03-08-27:	JPE - Created
//-----------------------------------------------------------------------------
CM_SECTOR::~CM_SECTOR()
{
	Free();
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	03-08-27:	JPE - Created
//-----------------------------------------------------------------------------
void CM_SECTOR::Free()
{
	if(_pIndexes)
	{
		delete[] _pIndexes;
		_pIndexes	= 0;
		_nEntries	= 0;
	}
	if(_pFaceIndexes)
	{
		delete[] _pFaceIndexes;
		_pFaceIndexes	= 0;
		_nFacesEntries	= 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetIntersection
//	Object:		
//	05-01-18:	JPE - Created
//-----------------------------------------------------------------------------
void CM_SECTOR::ClearEdgeUsingFlags(CM_EDGE *	pEdges)
{
	int		iSectIdx;

	for(iSectIdx=0; iSectIdx<_nEntries; ++iSectIdx)
	{
		CM_EDGE	&Edge	= pEdges[ _pIndexes[iSectIdx] ];
		Edge.SetUsed(false);
	}
}


//-----------------------------------------------------------------------------
//	Name:		GetIntersection
//	Object:		
//	04-03-26:	JPE - Created
//-----------------------------------------------------------------------------
CM_SECTOR::CM_SECTOR_RC		CM_SECTOR::GetIntersection(	VECTOR &	v1,
														VECTOR &	v2,
														POINT2D *	pVertices,
														POINT2D *	pNormals,
														CM_EDGE *	pEdges,
														POINT2D &	Intersect,
														int &		EdgeID)
{
	int		iSectIdx;
	int		iEdge		= -1;
	int		bParallel	= false;
	float	SquareDist	= 1e33f;
	POINT2D	v1v2(false);
	POINT2D	OutIsect;

	v1v2.SetXY( v2.GetX() - v1.GetX(),
				v2.GetZ() - v1.GetZ()	);

	v1v2.Normalize();

	for(iSectIdx=0; iSectIdx<_nEntries; ++iSectIdx)
	{
		CM_EDGE	&Edge	= pEdges[ _pIndexes[iSectIdx] ];
		POINT2D	&Normal	= pNormals[ _pIndexes[iSectIdx] ];

		if (Edge.IsUsed())
		{
			continue;
		}
		if( Normal.Dot(v1v2) > 0.01f )
		{
			continue;
		}

		switch( Edge.GetIntersection(v1, v2, pVertices, OutIsect) )
		{
			case CM_EDGE::EIRC_FULLINTERSECT:
			{
				POINT2D	I1(	OutIsect.GetX() - v1.GetX(),
							OutIsect.GetY() - v1.GetZ() );
				float	SqDist = I1.GetSquareLength();
				if(SqDist < SquareDist)
				{
					bParallel	= false;
					Intersect	= OutIsect;
					SquareDist	= SqDist;
					iEdge		= _pIndexes[iSectIdx];
				}
				break;
			}

			case CM_EDGE::EIRC_PARALLEL:
			{
				POINT2D	I1(	OutIsect.GetX() - v1.GetX(),
							OutIsect.GetY() - v1.GetZ() );
				float	SqDist = I1.GetSquareLength();
				if(SqDist < SquareDist)
				{
					bParallel	= true;
					Intersect	= OutIsect;
					SquareDist	= SqDist;
					iEdge		= _pIndexes[iSectIdx];
				}
				break;
			}
				/*Edge.SetUsed( true );
				bParallel	= true;
				EdgeID		= _pIndexes[iSectIdx];
				return CMSRC_PARALLEL;
				break;*/
		}
	}

	if( bParallel )
	{
		EdgeID = iEdge;
		return CMSRC_PARALLEL;
	}

	if( iEdge != -1)
	{
		EdgeID = iEdge;
		return CMSRC_FULLINTERSECT;
	}

	return CMSRC_NO_INTERSECTION;
}


//-----------------------------------------------------------------------------
//	Name:		Destructor
//	Object:		
//	03-08-27:	JPE - Created
//-----------------------------------------------------------------------------
void CM_SECTOR::SetNbElements(int nElements)
{
	if(_pIndexes)
	{
		delete[] _pIndexes;
		_pIndexes	= 0;
		_nEntries	= 0;
	}
	
	_nEntries = nElements;
	_pIndexes = new int[_nEntries];
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-08-27:	JPE - Created
//-----------------------------------------------------------------------------
void CM_SECTOR::SetNbFaces(int nbFaces)
{
	if(_pFaceIndexes)
	{
		delete[] _pFaceIndexes;
		_pFaceIndexes	= 0;
		_nFacesEntries	= 0;
	}
	
	_nFacesEntries	= nbFaces;
	_pFaceIndexes	= new int[_nFacesEntries];
}

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
