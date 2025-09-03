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
//	CLASS:	AABB
//
//	02-04-02:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)

#include 	INCL_KMATH(Math/Math)
#include 	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include 	INCL_KMATH(Math/Geometry/Distance/Distance)
#include 	INCL_KMATH(Math/Geometry/VerticalRay/VerticalRay)
#include 	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
#include	"AABB.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*		AABB::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		AABB constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
AABB::AABB()
:
BOUNDING_VOLUME(TYPE_AABB)
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		AABB constructor
//	Object:		
//	03-04-07:	SBE - Created
//-----------------------------------------------------------------------------
AABB::AABB(const int		nNbrPts,
		   const VECTOR *	pPoints)
:
BOUNDING_VOLUME(TYPE_AABB)
{
	Clear();
	Compute(nNbrPts, pPoints);
}

//-----------------------------------------------------------------------------
//	Name:		AABB constructor
//	Object:		
//	03-11-04:	ELE - Created
//-----------------------------------------------------------------------------
AABB::AABB(float	rMinX,
		   float	rMinY,
		   float	rMinZ,
		   float	rMaxX,
		   float	rMaxY,
		   float	rMaxZ)
:
BOUNDING_VOLUME(TYPE_AABB)
{
	_Min.SetXYZ(rMinX, rMinY, rMinZ);
	_Max.SetXYZ(rMaxX, rMaxY, rMaxZ);
	_Size.Sub(_Max, _Min);
	_Center.Add(_Min, _Max);
	_Center.Mul(0.5f);
}

//-----------------------------------------------------------------------------
//	Name:		AABB constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
AABB::AABB(const AABB &C)
:
BOUNDING_VOLUME(TYPE_AABB),
_Max(C._Max),
_Min(C._Min),
_Center(C._Center),
_Size(C._Size)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AABB destructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
AABB::~AABB()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
AABB& AABB::operator=(const AABB &C)
{
	_Min = C._Min;
	_Max = C._Max;
	_Center = C._Center;
	_Size = C._Size;
	
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-07-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	AABB::Read(IO_STREAM & Stream)
{
	float rMinX, rMinY, rMinZ;
	float rMaxX, rMaxY, rMaxZ;

	Stream >> rMinX >> rMaxX >> rMinY >> rMaxY >> rMinZ >> rMaxZ;

	_Min.SetXYZ(rMinX, rMinY, rMinZ);
	_Max.SetXYZ(rMaxX, rMaxY, rMaxZ);
	_Size.Sub(_Max, _Min);
	_Center.Add(_Min,_Max);
	_Center.Mul(0.5f);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-07-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	AABB::Write(IO_STREAM &	Stream) const
{
	Stream.DebugOutput("xMin : ");
	Stream << _Min.GetX();
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("xMax : ");
	Stream << _Max.GetX();
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("yMin : ");
	Stream << _Min.GetY();
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("yMax : ");
	Stream << _Max.GetY();
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("zMin : ");
	Stream << _Min.GetZ();
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("zMax : ");
	Stream << _Max.GetZ();
	Stream.DebugOutput("\t");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	AABB::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(AABB) );
}
#endif

//-----------------------------------------------------------------------------
//	Name: AABB::Display
//	Object: 
//	01-06-12:	GGO - Created
//-----------------------------------------------------------------------------
void	AABB::Display(const COLOR_FLOAT &Color) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-03-11:	ELE - Created
//-----------------------------------------------------------------------------
float	AABB::GetSize() const
{
	float	rH[3];

	rH[0] = _Size.GetX();
	rH[1] = _Size.GetY();
	rH[2] = _Size.GetZ();

	return	(MATH::Max(3, rH));
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	02-05-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::IsInside(const AABB &	Aabb) const
{
	if (GetXMin() < Aabb.GetXMin()) return (false);
	if (GetYMin() < Aabb.GetYMin()) return (false);
	if (GetZMin() < Aabb.GetZMin()) return (false);

	if (GetXMax() > Aabb.GetXMax()) return (false);
	if (GetYMax() > Aabb.GetYMax()) return (false);
	if (GetZMax() > Aabb.GetZMax()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	03-06-17:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::IsInside(const AABB &	Aabb,
					   float		rEpsilon) const
{
	if (GetXMin() < Aabb.GetXMin() + rEpsilon) return (false);
	if (GetYMin() < Aabb.GetYMin() + rEpsilon) return (false);
	if (GetZMin() < Aabb.GetZMin() + rEpsilon) return (false);

	if (GetXMax() > Aabb.GetXMax() - rEpsilon) return (false);
	if (GetYMax() > Aabb.GetYMax() - rEpsilon) return (false);
	if (GetZMax() > Aabb.GetZMax() - rEpsilon) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	06-07-06:	ELE - Created
//-----------------------------------------------------------------------------
bool	AABB::IsInside(const OBB &	Obb,
					   float		rEpsilon) const
{
	VECTOR	ObbPoints[8];

	Obb.GetPoints(ObbPoints);

	for	(UInt32 i = 0 ; i < 8 ; ++i)
	{
		if	(Intersect(ObbPoints[i], rEpsilon) == false)
		{
			return	(false);
		}
	}

	return	(true);
}

static PLANE	Planes[6];
static VECTOR	Binormals[6];
static VECTOR	Tangents[6];

//-----------------------------------------------------------------------------
//	Name:		ExtractPlanes
//	Object:		
//	04-08-24:	ELE - Created
//-----------------------------------------------------------------------------
void	AABB::ExtractPlanes() const
{
	VECTOR	N;

	N = VECTOR::NEG_UNIT_X;
	Binormals[0] = VECTOR::UNIT_Y;
	Tangents[0] = VECTOR::UNIT_Z;
	Planes[0].SetFromPointAndNormal(_Min, N);

	N = VECTOR::NEG_UNIT_Y;
	Binormals[1] = VECTOR::UNIT_X;
	Tangents[1] = VECTOR::UNIT_Z;
	Planes[1].SetFromPointAndNormal(_Min, N);

	N = VECTOR::NEG_UNIT_Z;
	Binormals[2] = VECTOR::UNIT_X;
	Tangents[2] = VECTOR::UNIT_Y;
	Planes[2].SetFromPointAndNormal(_Min, N);

	N = VECTOR::UNIT_X;
	Binormals[3] = VECTOR::UNIT_Y;
	Tangents[3] = VECTOR::UNIT_Z;
	Planes[3].SetFromPointAndNormal(_Max, N);

	N = VECTOR::UNIT_Y;
	Binormals[4] = VECTOR::UNIT_X;
	Tangents[4] = VECTOR::UNIT_Z;
	Planes[4].SetFromPointAndNormal(_Max, N);

	N = VECTOR::UNIT_Z;
	Binormals[5] = VECTOR::UNIT_X;
	Tangents[5] = VECTOR::UNIT_Y;
	Planes[5].SetFromPointAndNormal(_Max, N);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-11-05:	ELE - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const VECTOR &PIn,
						VECTOR		&POut,
						const VECTOR  &vMotion,
						const float	rInertia,
						const float	rE) const
{
	if	(Intersect(PIn) == false)
	{
		POut = PIn;
		return	(false);
	}

	static float	eps = 0.1f;

	int		i, d = 0;
	VECTOR	N;
	VECTOR	OldPos = PIn - vMotion;
	VECTOR	HitPoint(OldPos);

	for	(i = 0 ; i < 6 ; ++i)
	{
		if	(Planes[i].GetSide(PIn) != Planes[i].GetSide(OldPos))
		{
			VECTOR	V(vMotion), V2, B, T, VProj;
			float	r = V.Normalize();

			float	t = -(Planes[i].GetNormal().Dot(OldPos) + Planes[i].GetD()) / (V.Dot(Planes[i].GetNormal()));

			if	(t > d)
			{
				d = (Int32)t;

				V2.Mul(V, t - eps);
				HitPoint.Add(OldPos, V2);
				
				float rB = V.Dot(Binormals[i]);
				float rT = V.Dot(Tangents[i]);

				B.Mul(Binormals[i], rB);
				T.Mul(Tangents[i], rT);
				VProj.Add(B, T);
	
				VProj.Mul(r + 0.1f);
				HitPoint.Add(VProj);
			}
		}
	}

	POut = HitPoint;

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-07-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const AABB &	Aabb) const
{
	if (GetXMin() >= Aabb.GetXMax()) return (false);
	if (GetYMin() >= Aabb.GetYMax()) return (false);
	if (GetZMin() >= Aabb.GetZMax()) return (false);

	if (GetXMax() <= Aabb.GetXMin()) return (false);
	if (GetYMax() <= Aabb.GetYMin()) return (false);
	if (GetZMax() <= Aabb.GetZMin()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const VECTOR &	Point) const
{
	if (Point.GetX() >= GetXMax()) return (false);
	if (Point.GetY() >= GetYMax()) return (false);
	if (Point.GetZ() >= GetZMax()) return (false);

	if (Point.GetX() <= GetXMin()) return (false);
	if (Point.GetY() <= GetYMin()) return (false);
	if (Point.GetZ() <= GetZMin()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-06-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const VECTOR &	Point,
						float			rEpsilon) const
{
	if (Point.GetX() >= GetXMax() + rEpsilon) return (false);
	if (Point.GetY() >= GetYMax() + rEpsilon) return (false);
	if (Point.GetZ() >= GetZMax() + rEpsilon) return (false);

	if (Point.GetX() <= GetXMin() - rEpsilon) return (false);
	if (Point.GetY() <= GetYMin() - rEpsilon) return (false);
	if (Point.GetZ() <= GetZMin() - rEpsilon) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name: AABB::Compute
//	Object: Construct the AABB of a set of points.
//	01-02-20:	GGO - Created
//-----------------------------------------------------------------------------
void	AABB::Compute( int				nNbrPoints,
					   const VECTOR *	pPoints )
{
	int i;
	for (i = 0 ; i < nNbrPoints; i++)
	{
		Grow(pPoints[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	06-05-24:	ELE - Created
//-----------------------------------------------------------------------------
void	AABB::Compute(UInt32		nNbrPoints,
					  const void *	pPoints,
					  UInt32		nStride)
{
	UInt32	i;
	UInt8 * p = (UInt8 *)pPoints;

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		Float32 *	Point = (Float32 *)p;

		GrowX(Point[0]);
		GrowY(Point[1]);
		GrowZ(Point[2]);
		_Center = (_Min + _Max) * 0.5f;
		_Size.Sub(_Max, _Min);

		p += nStride;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	04-02-09:	ELE - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const PLANE &	Plane) const
{
	if	(Plane.GetNormal().IsNull())
	{
		return (false);
	}

	QDT_ASSERT( Plane.GetNormal().IsNormalized() == true );

	int		i;
	bool	bSide;
	VECTOR	Points[8];

	GetPoints(Points);

	bSide = Plane.GetSide(Points[0]);
	for	(i = 1 ;  i < 8 ; ++i)
	{
		if	(bSide != Plane.GetSide(Points[i]))
		{
			return	(true);
		}
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectWithPlane
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const PLANE &	Plane,
						bool &			bSide) const
{
	if	(Plane.GetNormal().IsNull())
	{
		bSide = false;
		return (false);
	}

	QDT_ASSERT(Plane.GetNormal().IsNormalized() == true);
	
	// We project the aabb and the plane on the plane normal.
	float rProjPlane = Plane.GetNormal().Dot(_Center) + Plane.GetD();
	float rEff		 = GetEffectiveRadius(Plane);

	// Intersection.
	if ( MATH::Abs(rProjPlane) <= rEff)
	{
		return (true);
	}

	// No intersection, so we look if the obb in the half space of the plane normal.
	bSide = (rProjPlane >= 0.0f);
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	04-04-15:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const HEXAHEDRON & Hexahedron) const
{
	if (IsOutsideOf(Hexahedron.GetPlaneNear())	 ||
		IsOutsideOf(Hexahedron.GetPlaneLeft())	 ||
		IsOutsideOf(Hexahedron.GetPlaneRight())	 ||
		IsOutsideOf(Hexahedron.GetPlaneTop())	 ||
		IsOutsideOf(Hexahedron.GetPlaneBottom()) ||
		IsOutsideOf(Hexahedron.GetPlaneFar()))
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-07-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const HEXAHEDRON &	Hexahedron,
						bool &				bIntersectFront) const
{
	bool	bSide;

	// Test if the AABB is outside a plane of the hexahedron.
	if (IsOutsideOf(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		return	(false);
	}
	else
	{
		bIntersectFront = Intersect(Hexahedron.GetPlaneNear(), bSide);
	}

	if (IsOutsideOf(Hexahedron.GetPlaneLeft())	||
		IsOutsideOf(Hexahedron.GetPlaneRight())	||
		IsOutsideOf(Hexahedron.GetPlaneTop())	||
		IsOutsideOf(Hexahedron.GetPlaneBottom())||
		IsOutsideOf(Hexahedron.GetPlaneFar()))
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const HEXAHEDRON &	Hexahedron,
						bool &				bInside,
						bool &				bIntersectFront) const
{
	bool	bSide;

	bInside = true;

	if	(!Intersect(Hexahedron.GetPlaneNear(), bSide))
	{
		bIntersectFront = false;

		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bIntersectFront = true;
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneLeft(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneRight(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneBottom(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneTop(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneFar(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	AABB::IntersectFront(const HEXAHEDRON &	Hexahedron,
							 bool &				bIntersectFront) const
{
	bool	bSide;

	// Test if the normals of the planes of the hexahedron are separating axis.
	if (IsOutsideOf(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		return	(false);
	}
	else
	{
		int	nSide;

		Hexahedron.GetPlaneNear().GetSide(_Center, nSide, 1.0f);
		if	(nSide <= 0)
		{
			bIntersectFront = Intersect(Hexahedron.GetPlaneNear(), bSide);
		}
		else
		{
			bIntersectFront = false;
			return	(false);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneLeft())	||
		IsOutsideOf(Hexahedron.GetPlaneRight())	||
		IsOutsideOf(Hexahedron.GetPlaneTop())	||
		IsOutsideOf(Hexahedron.GetPlaneBottom())||
		IsOutsideOf(Hexahedron.GetPlaneFar()))
	{
		return (false);
	}
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	AABB::IntersectFront(const HEXAHEDRON &	Hexahedron,
							 bool &				bInside,
							 bool &				bIntersectFront) const
{
	bool	bSide;

	bInside = true;

	int	nSide;

	Hexahedron.GetPlaneNear().GetSide(_Center, nSide, 1.0f);
	if	(nSide <= 0)
	{
		if	(!Intersect(Hexahedron.GetPlaneNear(), bSide))
		{
			bIntersectFront = false;

			if	(bSide)
			{
				bInside = false;
				return	(false);
			}
		}
		else
		{
			bIntersectFront = true;
			bInside = false;
		}
	}
	else
	{
		bIntersectFront = false;
		return	(false);
	}

	if	(!Intersect(Hexahedron.GetPlaneLeft(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneRight(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneBottom(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneTop(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneFar(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		Source & explanations : www.codecorner.com/RayAABB.cpp
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
// ! Integer representation of a floating-point value.
typedef unsigned int udword;
#define IR(x)	((udword&)x)

bool	AABB::Intersect(const RAY &	Ray,
						VECTOR &	HitPoint) const
{
	VECTOR MaxT(-1.0f, -1.0f, -1.0f);
	const VECTOR &Origin = Ray.GetOrigin();
	const VECTOR &Dir = Ray.GetDirection();
	unsigned int i;
	bool bInside = true;

	const float rEpsilon = 0.01f;

	// Find candidate planes.
	for (i = 0 ; i < 3 ; i++)
	{
		if (Origin[i] < _Min[i])
		{
			HitPoint[i]	= _Min[i];
			bInside		= false;

			// Calculate T distances to candidate planes
			float rValue = Dir[i];
			if(IR(rValue))
			{
				MaxT[i] = (_Min[i] - Origin[i]) / Dir[i];
			}
		}
		else if (Origin[i] > _Max[i])
		{
			HitPoint[i]	= _Max[i];
			bInside		= false;

			// Calculate T distances to candidate planes
			float rValue = Dir[i];
			if(IR(rValue))
			{
				MaxT[i] = (_Max[i] - Origin[i]) / Dir[i];
			}
		}
	}

	// Ray origin inside bounding box
	if(bInside)
	{
		HitPoint = Origin;
		return (true);
	}

	// Get largest of the maxT's for final choice of intersection
	unsigned int nWhichPlane = 0;
	if(MaxT[1] > MaxT[nWhichPlane])	nWhichPlane = 1;
	if(MaxT[2] > MaxT[nWhichPlane])	nWhichPlane = 2;

	// Check final candidate actually inside box
	if(IR(MaxT[nWhichPlane]) & 0x80000000) 
	{
		return (false);
	}

	for(i = 0 ; i < 3 ; i++)
	{
		if (i != nWhichPlane)  
		{
			HitPoint[i] = Origin[i] + MaxT[nWhichPlane] * Dir[i];
			if(HitPoint[i] < _Min[i] - rEpsilon || HitPoint[i] > _Max[i] + rEpsilon)	
			{
				return (false); 
			}
		}
	}

	return (true);	// ray hits box
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const RAY &	Ray,
						float &		rSquareDistance) const
{
	VECTOR HitPoint;

	bool bRes = Intersect(Ray, HitPoint);

	if (bRes == true)
	{
		rSquareDistance = DISTANCE::DistanceSquarePointPoint(Ray.GetOrigin(), HitPoint);
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const FRUSTRUM &Frustrum) const
{
	float rRatio = Frustrum.GetFar() / Frustrum.GetNear();
	float rProjNear, rProjFar;

	// We test if the normal of the AABB (ie. the canonic axis) are separating axis.

	// Projection along the X-axis.
	rProjNear = Frustrum.GetOrigin().GetX() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetX() - 
				Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetX()) -
				Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetX());

	if (rProjNear >= GetXMax())
	{
		rProjFar =	Frustrum.GetOrigin().GetX() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetX() -
					Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetX()) * rRatio -
					Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetX())  * rRatio;

		if (rProjFar >= GetXMax())
		{
			return (false);
		}
	}

	rProjNear =	Frustrum.GetOrigin().GetX() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetX() + 
				Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetX()) +
				Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetX());

	if (rProjNear <= GetXMin())
	{
		rProjFar =	Frustrum.GetOrigin().GetX() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetX() +
					Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetX() ) * rRatio +
					Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetX() )  * rRatio;

		if (rProjFar <= GetXMin())
		{
			return (false);
		}
	}

	// Projection along the Y-axis.
	rProjNear = Frustrum.GetOrigin().GetY() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetY() - 
				Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetY()) -
				Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetY());

	if (rProjNear >= GetYMax())
	{
		rProjFar =	Frustrum.GetOrigin().GetY() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetY() -
					Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetY()) * rRatio -
					Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetY())  * rRatio;

		if (rProjFar >= GetYMax())
		{
			return (false);
		}
	}

	rProjNear = Frustrum.GetOrigin().GetY() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetY() + 
				Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetY()) +
				Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetY());

	if (rProjNear <= GetYMin())
	{
		rProjFar =	Frustrum.GetOrigin().GetY() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetY() +
					Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetY()) * rRatio +
					Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetY())  * rRatio;

		if (rProjFar <= GetYMin())
		{
			return ( false );
		}
	}

	// Projection along the Z-axis.
	rProjNear = Frustrum.GetOrigin().GetZ() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetZ() - 
				Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetZ()) -
				Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetZ());

	if (rProjNear >= GetZMax())
	{
		rProjFar =	Frustrum.GetOrigin().GetZ() +
					Frustrum.GetFar() * Frustrum.GetFrontDirection().GetZ() -
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetZ()) * rRatio -
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetZ())  * rRatio;

		if (rProjFar >= GetZMax())
		{
			return ( false );
		}
	}

	rProjNear = Frustrum.GetOrigin().GetZ() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetZ() + 
				Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetZ()) +
				Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetZ());

	if (rProjNear <= GetZMin())
	{
		rProjFar =	Frustrum.GetOrigin().GetZ() +
					Frustrum.GetFar() * Frustrum.GetFrontDirection().GetZ() +
					Frustrum.GetLeft() * MATH::Abs(Frustrum.GetLeftDirection().GetZ()) * rRatio +
					Frustrum.GetTop()  * MATH::Abs(Frustrum.GetTopDirection().GetZ())  * rRatio;

		if (rProjFar <= GetZMin())
		{
			return (false);
		}
	}

	// We test if the normal of the frustrum are separating axis.
	float rProjAABB;
	float rDotProduct;

	float rAABB_DistanceX = 0.5f * ( GetXMax() - GetXMin() );
	float rAABB_DistanceY = 0.5f * ( GetYMax() - GetYMin() );
	float rAABB_DistanceZ = 0.5f * ( GetZMax() - GetZMin() );

	VECTOR CenterAABB( 0.5f * ( GetXMin() + GetXMax() ),
			 		    0.5f * ( GetYMin() + GetYMax() ),
					    0.5f * ( GetZMin() + GetZMax() ) );

	VECTOR NewFrustrumOrigin = Frustrum.GetOrigin() - CenterAABB;


	// Near and far
	rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetFrontDirection().GetX() ) +
				rAABB_DistanceY * MATH::Abs( Frustrum.GetFrontDirection().GetY() ) +
				rAABB_DistanceZ * MATH::Abs( Frustrum.GetFrontDirection().GetZ() );
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetFrontDirection() );

	if ( rDotProduct <= 0.0f )
	{
		if ( Frustrum.GetFar() + rProjAABB + rDotProduct <= 0.0f ) // Remember here rDotProduct is negative.
		{
			return ( false );
		}
	}
	else
	{
		if ( rDotProduct + Frustrum.GetNear() >= rProjAABB )
		{
			return ( false );
		}
	}

	// Left
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalLeft() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalLeft().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalLeft().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalLeft().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

	// Right
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalRight() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalRight().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalRight().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalRight().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

	// Top
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalTop() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalTop().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalTop().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalTop().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

	// Bottom
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalBottom() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalBottom().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalBottom().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalBottom().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

//	if ( nSpeed == 1 )
//	{
//		return ( true );
//	}

	// Finally we test if the cross products are separating axis.
	int   i;
	float rProjLeftTop;

	VECTOR NewFrustrumFar  = NewFrustrumOrigin + Frustrum.GetFar()  * Frustrum.GetFrontDirection();
	VECTOR NewFrustrumNear = NewFrustrumOrigin + Frustrum.GetNear() * Frustrum.GetFrontDirection();

	
	// Near/far edges.
	VECTOR vFrustrumEdge;
	for ( i = 0; i < 6; i++ )
	{
		switch( i )
		{
		case 0:
			vFrustrumEdge = Frustrum.GetLeftDirection();
			break;
		case 1:
			vFrustrumEdge = Frustrum.GetTopDirection();
			break;
		case 2:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() +
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() +
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		case 3:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() +
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() -
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		case 4:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() -
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() +
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		case 5:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() -
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() -
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		default:
			QDT_FAIL();
		}


		// Cross vector: 0.0f
		//				 vFrustrumEdge.GetZ()
		//				-vFrustrumEdge.GetY()

		rProjAABB    = rAABB_DistanceY * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   rAABB_DistanceZ * MATH::Abs( vFrustrumEdge.GetY() );

		rProjNear    = NewFrustrumNear.GetY() * vFrustrumEdge.GetZ() -
					   NewFrustrumNear.GetZ() * vFrustrumEdge.GetY();

		rProjFar     = NewFrustrumFar.GetY() * vFrustrumEdge.GetZ() -
					   NewFrustrumFar.GetZ() * vFrustrumEdge.GetY();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{	
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( Frustrum.GetLeftDirection().GetY() * vFrustrumEdge.GetZ() -
						     		  Frustrum.GetLeftDirection().GetZ() * vFrustrumEdge.GetY() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( Frustrum.GetTopDirection().GetY()  * vFrustrumEdge.GetZ() -
					     			  Frustrum.GetTopDirection().GetZ()  * vFrustrumEdge.GetY() );

			if ( ( rProjAABB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
			     ( rProjAABB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}


		// Cross vector: vFrustrumEdge.GetZ()
		//				 0.0f
		//				-vFrustrumEdge.GetX()

		rProjAABB    = rAABB_DistanceX * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   rAABB_DistanceZ * MATH::Abs( vFrustrumEdge.GetX() );

		rProjNear    = NewFrustrumNear.GetX() * vFrustrumEdge.GetZ() -
					   NewFrustrumNear.GetZ() * vFrustrumEdge.GetX();

		rProjFar     = NewFrustrumFar.GetX() * vFrustrumEdge.GetZ() -
					   NewFrustrumFar.GetZ() * vFrustrumEdge.GetX();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{			
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( Frustrum.GetLeftDirection().GetX() * vFrustrumEdge.GetZ() -
					     			  Frustrum.GetLeftDirection().GetZ() * vFrustrumEdge.GetX() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( Frustrum.GetTopDirection().GetX()  * vFrustrumEdge.GetZ() -
					     			  Frustrum.GetTopDirection().GetZ()  * vFrustrumEdge.GetX() );

			if ( ( rProjAABB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjAABB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}


		// Cross vector: vFrustrumEdge.GetY()
		//				-vFrustrumEdge.GetX()
		//				 0.0f

		rProjAABB    = rAABB_DistanceX * MATH::Abs( vFrustrumEdge.GetY() ) +
					   rAABB_DistanceY * MATH::Abs( vFrustrumEdge.GetX() );

		rProjNear    = NewFrustrumNear.GetX() * vFrustrumEdge.GetY() -
					   NewFrustrumNear.GetY() * vFrustrumEdge.GetX();

		rProjFar     = NewFrustrumFar.GetX() * vFrustrumEdge.GetY() -
					   NewFrustrumFar.GetY() * vFrustrumEdge.GetX();

		if ( rProjNear * rProjFar > 0.0f )   // 'rProjNear' and 'rProjFar' have the same sign.
		{
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( Frustrum.GetLeftDirection().GetX() * vFrustrumEdge.GetY() -
					     			  Frustrum.GetLeftDirection().GetY() * vFrustrumEdge.GetX() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( Frustrum.GetTopDirection().GetX()  * vFrustrumEdge.GetY() -
					     			  Frustrum.GetTopDirection().GetY()  * vFrustrumEdge.GetX() );

			if ( ( rProjAABB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjAABB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::Clear()
{
	_Min	= VECTOR::FLOAT_MAX;
	_Max	= VECTOR::FLOAT_MIN;
	_Center = VECTOR::ZERO;
	_Size	= VECTOR::ZERO;
}

//-----------------------------------------------------------------------------
//	Name:		Extend
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::Extend(float	rValue)
{
	_Max.Add(rValue);
	_Min.Sub(rValue);
	_Size.Sub(_Max, _Min);
}

//-----------------------------------------------------------------------------
//	Name:		Extend
//	Object:		
//	06-07-06:	ELE - Created
//-----------------------------------------------------------------------------
void	AABB::Extend(const VECTOR &	v)
{
	_Max.Add(v);
	_Min.Sub(v);
	_Size.Sub(_Max, _Min);
}

void  AABB::Extend(const AABB & Aabb)
{
	_Max.Add(Aabb.GetMax());
	_Min.Add(Aabb.GetMin());
	_Center = (_Min + _Max) * 0.5f;
	_Size.Sub(_Max, _Min);
}


//-----------------------------------------------------------------------------
//	Name:		Translate
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::Translate(const VECTOR &	vTrans)
{
	_Min += vTrans;
	_Max += vTrans;
	_Center += vTrans;
}

//-----------------------------------------------------------------------------
//	Name:		Translate
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::Translate(float rdx,
						float rdy,
						float rdz)
{
	Translate(VECTOR(rdx, rdy, rdz));
}

//-----------------------------------------------------------------------------
//	Name:		GrowX
//	Object:		
//	01-08-23:	JLT - Created
//-----------------------------------------------------------------------------
void AABB::GrowX(float	rx)
{	
	if (GetXMin() > rx)
	{
		_Min.SetX(rx);
		_Size.SetX(_Max.GetX() - _Min.GetX());
	}
	if (rx > GetXMax())
	{
		_Max.SetX(rx);
		_Size.SetX(_Max.GetX() - _Min.GetX());
	}
}

//-----------------------------------------------------------------------------
//	Name:		GrowY
//	Object:		
//	01-08-23:	JLT - Created
//-----------------------------------------------------------------------------
void AABB::GrowY(float	ry)
{	
	if (GetYMin() > ry)
	{
		_Min.SetY(ry);
		_Size.SetY(_Max.GetY() - _Min.GetY());
	}
	if (ry > GetYMax())
	{
		_Max.SetY(ry);
		_Size.SetY(_Max.GetY() - _Min.GetY());
	}
}

//-----------------------------------------------------------------------------
//	Name:		GrowZ
//	Object:		
//	01-08-23:	JLT - Created
//-----------------------------------------------------------------------------
void AABB::GrowZ(float	rz)
{	
	if (GetZMin() > rz)
	{
		_Min.SetZ(rz);
		_Size.SetZ(_Max.GetZ() - _Min.GetZ());
	}
	if (rz > GetZMax())
	{
		_Max.SetZ(rz);
		_Size.SetZ(_Max.GetZ() - _Min.GetZ());
	}
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	01-11-26:	JLT - Created
//-----------------------------------------------------------------------------
void AABB::Grow(const AABB &bbox)
{
	Grow(bbox.GetMin());
	Grow(bbox.GetMax());
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	04-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::Grow(const OBB &	bbox)
{
	VECTOR V1(bbox.GetCenter());
	VECTOR V2(bbox.GetCenter());
	VECTOR V3(bbox.GetCenter());
	VECTOR V4(bbox.GetCenter());
	VECTOR V5(bbox.GetCenter());
	VECTOR V6(bbox.GetCenter());

	V1.AddMul(bbox.GetDirection1(), bbox.GetDistance1());
	V2.AddMul(bbox.GetDirection2(), bbox.GetDistance2());
	V3.AddMul(bbox.GetDirection3(), bbox.GetDistance3());
	V4.AddMul(bbox.GetDirection1(), -bbox.GetDistance1());
	V5.AddMul(bbox.GetDirection2(), -bbox.GetDistance2());
	V6.AddMul(bbox.GetDirection3(), -bbox.GetDistance3());

	Grow(V1);
	Grow(V2);
	Grow(V3);
	Grow(V4);
	Grow(V5);
	Grow(V6);
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	01-08-23:	JLT - Created
//-----------------------------------------------------------------------------
void AABB::Grow(const VECTOR &P)
{
	GrowX(P.GetX());
	GrowY(P.GetY());
	GrowZ(P.GetZ());
	_Center = (_Min + _Max) * 0.5f;
	_Size.Sub(_Max, _Min);
}

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	AABB::GetCenter() const
{
	return (_Center);
}

//-----------------------------------------------------------------------------
//	Name:		IsInfinite
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
bool AABB::IsInfinite() const
{
	return (GetXMin() == QDT_FLOAT32_MAX || GetXMax() == QDT_FLOAT32_MAX ||
			GetYMin() == QDT_FLOAT32_MAX || GetYMax() == QDT_FLOAT32_MAX ||
			GetZMin() == QDT_FLOAT32_MAX || GetZMax() == QDT_FLOAT32_MAX);
}

//-----------------------------------------------------------------------------
//	Name:		GetPoints
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::GetPoints(VECTOR	(& Points)[8]) const
{
	Points[0].SetXYZ(GetXMin(), GetYMin(), GetZMin());
	Points[1].SetXYZ(GetXMin(), GetYMin(), GetZMax());
	Points[2].SetXYZ(GetXMin(), GetYMax(), GetZMin());
	Points[3].SetXYZ(GetXMin(), GetYMax(), GetZMax());
	Points[4].SetXYZ(GetXMax(), GetYMin(), GetZMin());
	Points[5].SetXYZ(GetXMax(), GetYMin(), GetZMax());
	Points[6].SetXYZ(GetXMax(), GetYMax(), GetZMin());
	Points[7].SetXYZ(GetXMax(), GetYMax(), GetZMax());
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	03-02-20:	ELE - Created
//-----------------------------------------------------------------------------
float	AABB::GetArea() const
{
	if	(GetSizeX() < GetSizeY())
	{
		if	(GetSizeZ() < GetSizeX())
		{
			return	(GetSizeX() * GetSizeY());
		}
		else
		{
			return	(GetSizeY() * GetSizeZ());
		}
	}
	else
	{
		if	(GetSizeZ() < GetSizeY())
		{
			return	(GetSizeX() * GetSizeY());
		}
		else
		{
			return	(GetSizeX() * GetSizeZ());
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	AABB::Transform(const MATRIX &	Transform)
{
	VECTOR	TmpMin, TmpMax;

	Transform.TransformPosition(_Min, TmpMin);
	Transform.TransformPosition(_Max, TmpMax);

	if	(TmpMax.GetX() > TmpMin.GetX())
	{
		_Max.SetX(TmpMax.GetX());
		_Min.SetX(TmpMin.GetX());
	}
	else
	{
		_Max.SetX(TmpMin.GetX());
		_Min.SetX(TmpMax.GetX());
	}

	if	(TmpMax.GetY() > TmpMin.GetY())
	{
		_Max.SetY(TmpMax.GetY());
		_Min.SetY(TmpMin.GetY());
	}
	else
	{
		_Max.SetY(TmpMin.GetY());
		_Min.SetY(TmpMax.GetY());
	}

	if	(TmpMax.GetZ() > TmpMin.GetZ())
	{
		_Max.SetZ(TmpMax.GetZ());
		_Min.SetZ(TmpMin.GetZ());
	}
	else
	{
		_Max.SetZ(TmpMin.GetZ());
		_Min.SetZ(TmpMax.GetZ());
	}

	_Center.Add(_Min, _Max);
	_Center.Mul(0.5f);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME &	AABB::operator=(const BOUNDING_VOLUME &	BoundingVolume)
{
	QDT_ASSERT(BoundingVolume.GetType() == GetType());
	return (*this = static_cast<const AABB &>(BoundingVolume));
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const CONE &Cone) const
{
	SPHERE S(GetCenter(), MATH::Max((MATH::Max(GetSizeX(), GetSizeY())), GetSizeZ()) * 0.5f);
	return (S.Intersect(Cone));
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		Found on http://www.gamasutra.com/features/19991018/Gomez_4.htm
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(const SPHERE &	Sphere) const
{
	float s, d = 0;
	const VECTOR & C = Sphere.GetCenter();
	float r = Sphere.GetRadius();

	//find the square of the distance
	//from the sphere to the box
	for( long i=0 ; i<3 ; i++ ) 
	{ 	
		if( C[i] < _Min[i] )
		{
			
			s = C[i] - _Min[i];
			d += s*s; 
			
		}
		
		else if( C[i] > _Max[i] )
		{ 
			
			s = C[i] - _Max[i];
			d += s*s; 
		}	
	}

	return d <= r*r;
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::Intersect(VERTICAL_RAY &	VerticalRay) const
{
	if (VerticalRay.GetOrigin().GetX() > GetXMax()) return (false);
	if (VerticalRay.GetOrigin().GetZ() > GetZMax()) return (false);
	if (VerticalRay.GetOrigin().GetX() < GetXMin()) return (false);
	if (VerticalRay.GetOrigin().GetZ() < GetZMin()) return (false);
	
	if (VerticalRay.GetOrigin().GetY() + VerticalRay.GetNegDistance() > GetYMax())	return (false);
	if (VerticalRay.GetOrigin().GetY() + VerticalRay.GetPosDistance() < GetYMin())	return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		SetXYZ
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
void	AABB::SetXYZ(const float	rXMin,
					 const float	rXMax,
					 const float	rYMin,
					 const float	rYMax,
					 const float	rZMin,
					 const float	rZMax)
{
	QDT_ASSERT( rXMin <= rXMax );
	QDT_ASSERT( rYMin <= rYMax );
	QDT_ASSERT( rZMin <= rZMax );

	_Min.SetXYZ( rXMin, rYMin, rZMin );
	_Max.SetXYZ( rXMax, rYMax, rZMax );
	_Size.Sub(_Max, _Min);
	_Center.Add(_Min, _Max);
	_Center.Mul(0.5f);
}

//-----------------------------------------------------------------------------
//	Name:		GetOptimalFocalAngle
//	Object:		
//	03-04-16:	ELE - Created
//-----------------------------------------------------------------------------
float	AABB::GetOptimalFocalAngle(const VECTOR &	Pos,
								   float			rAngle,
								   float &			rNear,
								   float &			rIntersectNear,
								   const VECTOR &	FrontDir) const
{
	int		i, nMinPoint;
	float	rDistance, rMinDistance, rDot, rOpeningAngle, rDiffAngle, rCos, rMinCos = 1.0f;
	VECTOR	Points[8];
	VECTOR	V[8]; 
	VECTOR	Vc;

	Vc.SetFromPoints(Pos, GetCenter());
	rIntersectNear = Vc.Normalize();

	GetPoints(Points);

	nMinPoint = 0;
	V[nMinPoint].SetFromPoints(Pos, Points[nMinPoint]);
	rMinDistance = V[nMinPoint].Normalize();

	for	(i = 1 ; i < 8 ; ++i)
	{
		V[i].SetFromPoints(Pos, Points[i]);
		rDistance = V[i].Normalize();

		if	(rDistance < rMinDistance)
		{
			rMinDistance = rDistance;
			nMinPoint = i;
		}
	}

	if	(Vc.Dot(V[nMinPoint]) < 0.0f)
	{
		return	(0.0f);
	}

	rNear = rMinDistance * FrontDir.Dot(V[nMinPoint]);
	rDot = FrontDir.Dot(Vc);

	for	(i = 0 ; i < 8 ; ++i)
	{
		if	(i != nMinPoint)
		{
			rCos = V[i].Dot(FrontDir);

			if	(rCos < rMinCos)
			{
				rMinCos = rCos;
			}
		}
	}

	rOpeningAngle = MATH::ACos(rMinCos);
	rDiffAngle = MATH::ACos(rDot);

	if	(rAngle == 0.0f)
	{
		return	((rOpeningAngle + rDiffAngle) * 2.0f);
	}
	else
	{
		if	(rAngle + rOpeningAngle > rDiffAngle)
		{
			return	((rOpeningAngle + rDiffAngle) * 2.0f);
		}
		else
		{
			return	(0.0f);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFromSphere
//	Object:		
//	03-05-30:	SBE - Created
//-----------------------------------------------------------------------------
void	AABB::SetFromSphere(const SPHERE & Sphere)
{
	_Min = Sphere.GetCenter();
	_Min.Sub(Sphere.GetRadius());

	_Max = Sphere.GetCenter();
	_Max.Add(Sphere.GetRadius());

	_Size.Sub(_Max, _Min);
	_Center.Add(_Min, _Max);
	_Center.Mul(0.5f);
}

//-----------------------------------------------------------------------------
//	Name:		GetRadius
//	Object:		
//	06-03-28:	PDE - Created
//-----------------------------------------------------------------------------
float	AABB::GetRadius() const
{
	return _Size.Length() * 0.5f;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
