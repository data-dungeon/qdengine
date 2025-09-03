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
//	CLASS:	CYLINDER
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)
#include	INCL_KMATH(Math/Geometry/Distance/Distance)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
#include	"Cylinder.inl"
#endif
	
void*		CYLINDER::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		CYLINDER constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
CYLINDER::CYLINDER()
:
BOUNDING_VOLUME(TYPE_CYLINDER)
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		CYLINDER constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
CYLINDER::CYLINDER(const CYLINDER & C)
:
BOUNDING_VOLUME(TYPE_CYLINDER)
{
	Clear();
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		CYLINDER constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
CYLINDER::CYLINDER(const VECTOR &Center, 
				   const VECTOR &CylDir,
				   const float rCylRadius,
				   const float rCylHeight)
:
BOUNDING_VOLUME(TYPE_CYLINDER)
{
	Clear();
	Build(Center, CylDir, rCylRadius, rCylHeight);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
CYLINDER& CYLINDER::operator=(const CYLINDER &C)
{
	Copy(C);
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
void CYLINDER::Copy(const CYLINDER &C)
{	
	_Center = C._Center;
	_XDir = C._XDir;	
	_YDir = C._YDir;
	_ZDir = C._ZDir;
	SetRadius(C.GetRadius());
	SetHeight(C.GetHeight());
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CYLINDER::Clear()
{
	_Center = VECTOR::ZERO;
	_XDir = VECTOR::UNIT_X;
	_YDir = VECTOR::UNIT_Y;
	_ZDir = VECTOR::UNIT_Z;
	SetRadius(0.0f);
	SetHeight(0.0f);
}


//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	04-08-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	CYLINDER::Read(IO_STREAM &	s)
{
	float rRadius, rHeight;
	s >> _Center >>  _XDir >> _YDir >> _ZDir >> rRadius >> rHeight;
	SetRadius(rRadius);
	SetHeight(rHeight);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	04-08-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	CYLINDER::Write(IO_STREAM &	s) const
{
	s << _Center << _XDir << _YDir << _ZDir << GetRadius() << GetHeight();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	CYLINDER::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(CYLINDER) );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
void CYLINDER::Display(const COLOR_FLOAT &Color) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
}

//-----------------------------------------------------------------------------
// Name: CYLINDER::Transform
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void CYLINDER::Transform(const CYLINDER &Cylinder, const MATRIX &Matrix)
{
	Matrix.TransformPosition(Cylinder._Center, _Center);
	Matrix.TransformDirection(Cylinder._XDir, _XDir);
	Matrix.TransformDirection(Cylinder._YDir, _YDir);
	Matrix.TransformDirection(Cylinder._ZDir, _ZDir);
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	04-08-13:	RMA - Created
//-----------------------------------------------------------------------------
void	CYLINDER::Transform(const MATRIX &	Matrix)
{
	Matrix.TransformPosition(_Center);
	Matrix.TransformDirection(_XDir);
	Matrix.TransformDirection(_YDir);
	Matrix.TransformDirection(_ZDir);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayFilled
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void	CYLINDER::DisplayFilled(const COLOR_FLOAT &	Color,
								const int			nStep) const
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const PLANE &	Plane,
							bool &			bSide) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const RAY &	Ray,
							VECTOR &	HitPoint) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const RAY &	Ray,
							float &		rDistance) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const HEXAHEDRON &	Hexahedron) const
{
	QDT_NOT_IMPLEMENTED();
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-07-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const HEXAHEDRON &	Hexahedron,
							bool &				bIntersectFront) const
{
	QDT_NOT_IMPLEMENTED();
	bIntersectFront = false;
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const HEXAHEDRON &	Hexahedron,
							bool &				bInside,
							bool &				bIntersectFront) const
{
	QDT_NOT_IMPLEMENTED();
	bIntersectFront = false;
	bInside = false;
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::IntersectFront(const HEXAHEDRON &	Hexahedron,
								 bool &				bIntersectFront) const
{
	QDT_NOT_IMPLEMENTED();
	bIntersectFront = false;
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::IntersectFront(const HEXAHEDRON &	Hexahedron,
								 bool &				bInside,
								 bool &				bIntersectFront) const
{
	QDT_NOT_IMPLEMENTED();
	bIntersectFront = false;
	bInside = false;
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const FRUSTRUM &Frustrum) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME &	CYLINDER::operator=(const BOUNDING_VOLUME &	BoundingVolume)
{
	QDT_ASSERT(BoundingVolume.GetType() == GetType());
	return (*this = static_cast<const CYLINDER &>(BoundingVolume));
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
float	CYLINDER::GetSize() const
{
	return (GetHeight());
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const CONE & Cone) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const SPHERE &	Sphere) const
{
	QDT_NOT_IMPLEMENTED();

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const AABB &	Aabb) const
{
	// WARNING : assuming cylinder axis is (0.0, 1.0, 0.0)
	// DO NOT USE FOR OTHER CYLINDER ORIENTATION
	// FOR CLOTH INTERSECTION ONLY

	float	rMinX, rMaxX, rMinY, rMaxY, rMinZ, rMaxZ;

			rMinY = _Center.GetY();
			rMaxY = rMinY + GetHeight();

			rMinX = _Center.GetX() - GetRadius();
			rMaxX = _Center.GetX() + GetRadius();

			rMinZ = _Center.GetZ() - GetRadius();
			rMaxZ = _Center.GetZ() + GetRadius();

	AABB	aabb(rMinX, rMinY, rMinZ, rMaxX, rMaxY, rMaxZ);

	return	(aabb.Intersect(Aabb));
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(const VECTOR &	Point) const
{
	const float eps = 0.01f;
	float rHeadHeight = GetHeight() - GetRadius();

	VECTOR	V1, V2, N, VT, V;
	float	ps;

	V1.Sub(Point, _Center);
	ps = V1.Dot(_ZDir);

	if	((ps < eps) || (ps > GetHeight() + eps))
	{
		return	(false);
	}

	if	(ps < GetRadius())
	{
		VECTOR	TailCenter;
		
		V.Mul(_ZDir, -GetRadius());
		TailCenter.Add(_Center, V);
		
		V2.Sub(Point, TailCenter);
		float	d = V2.Normalize();

		if	(d > (GetRadius() + eps))
		{
			return	(false);
		}

		return	(true);
	}
	else
	{
		if	(ps < rHeadHeight)
		{
			VECTOR	HeadCenter;
		
			V.Mul(_ZDir, rHeadHeight);
			HeadCenter.Add(_Center, V);
			
			V2.Sub(Point, HeadCenter);
			float	d = V2.Normalize();

			if	(d > (GetRadius() + eps))
			{
				return	(false);
			}

			return	(true);
		}
		else
		{
			V2.Mul(_ZDir, ps);
			N.Sub(V1, V2);

			float	d = N.Normalize();
			
			if	(d > (GetRadius() + eps))
			{
				return	(false);
			}

			return	(true);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	CYLINDER::Intersect(VERTICAL_RAY &	VerticalRay) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-02:	JLT - Created
//-----------------------------------------------------------------------------
/*bool CYLINDER::Intersect(const CYLINDER &C, const BOUNDING_BOX &bb)
{
	VECTOR HD = bb.GetHalfDiag();
	VECTOR Center = bb.GetCenter();
	PLANE Plane;
	VECTOR P;
	VECTOR N;
	CYLINDER CTrf(C);

	////////////////////////////////////////////////////////
	N.SetXYZ(1,0,0);
	P[_X_] = Center[_X_] + HD[_X_];
	P[_Y_] = Center[_Y_];
	P[_Z_] = Center[_Z_];
	Plane.SetFromPointAndNormal(P, N);

	if (INTERSECTION::PlaneCylinder(Plane, CTrf))
		return false;
	
	////////////////////////////////////////////////////////
	N.SetXYZ(-1,0,0);
	P[_X_] = Center[_X_] - HD[_X_];
	P[_Y_] = Center[_Y_];
	P[_Z_] = Center[_Z_];
	Plane.SetFromPointAndNormal(P, N);

	if (INTERSECTION::PlaneCylinder(Plane, CTrf))
		return false;
	
	////////////////////////////////////////////////////////
	N.SetXYZ(0,1,0);
	P[_X_] = Center[_X_];
	P[_Y_] = Center[_Y_] + HD[_Y_];
	P[_Z_] = Center[_Z_];
	Plane.SetFromPointAndNormal(P, N);

	if (INTERSECTION::PlaneCylinder(Plane, CTrf))
		return false;
	
	////////////////////////////////////////////////////////
	N.SetXYZ(0,-1,0);
	P[_X_] = Center[_X_];
	P[_Y_] = Center[_Y_] - HD[_Y_];
	P[_Z_] = Center[_Z_];
	Plane.SetFromPointAndNormal(P, N);

	if (INTERSECTION::PlaneCylinder(Plane, CTrf))
		return false;

	////////////////////////////////////////////////////////
	N.SetXYZ(0,0,1);
	P[_X_] = Center[_X_];
	P[_Y_] = Center[_Y_];
	P[_Z_] = Center[_Z_] + HD[_Z_];
	Plane.SetFromPointAndNormal(P, N);

	if (INTERSECTION::PlaneCylinder(Plane, CTrf))
		return false;
	
	////////////////////////////////////////////////////////
	N.SetXYZ(0,0,-1);
	P[_X_] = Center[_X_];
	P[_Y_] = Center[_Y_];
	P[_Z_] = Center[_Z_] - HD[_Z_];
	Plane.SetFromPointAndNormal(P, N);

	if (INTERSECTION::PlaneCylinder(Plane, CTrf))
		return false;

	return true;
}*/

//-----------------------------------------------------------------------------
//	Name:		GetOptimalFocalAngle
//	Object:		
//	03-04-16:	ELE - Created
//-----------------------------------------------------------------------------
float	CYLINDER::GetOptimalFocalAngle(const VECTOR &	Pos,
									   float			rAngle,
									   float &			rNear,
									   float &			rIntersectNear,
									   const VECTOR &	FrontDir) const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
const VECTOR & CYLINDER::GetCenter() const
{
	return _Center;
}


//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-04-15:	JLT - Created
//-----------------------------------------------------------------------------
bool CYLINDER::Intersect(const VECTOR &PIn,
						 VECTOR &POut,
						 const VECTOR  &vMotion,
						 const float Inertia,
						 const float rE) const
{
	const float eps = 0.01f;
	float rHeadHeight = GetHeight() - GetRadius();

	VECTOR	V1, V2, N, VT, V;
	float	ps;

	V1.Sub(PIn, _Center);
	ps = V1.Dot(_ZDir);

	if	((ps < eps) || (ps > GetHeight() + eps))
	{
		return	(false);
	}

	if	(ps < rHeadHeight)
	{
		V2.Mul(_ZDir, ps);
		N.Sub(V1, V2);

		float	d = N.Normalize();
		
		if	(d > (GetRadius() + eps))
		{
			return	(false);
		}
		
		V1.Mul(N, GetRadius() - d);
		POut.Add(PIn, V1);

		return	(true);
	}
	else
	{
		VECTOR	HeadCenter;
		
		V.Mul(_ZDir, rHeadHeight);
		HeadCenter.Add(_Center, V);
		
		V2.Sub(PIn, HeadCenter);
		float	d = V2.Normalize();

		if	(d > (GetRadius() + eps))
		{
			return	(false);
		}
		
		V1.Mul(V2, GetRadius());
		POut.Add(HeadCenter, V1);

		return	(true);
	}
}

//-----------------------------------------------------------------------------
// Name: CYLINDER::GetRandomPoint
// Object: 
//00-12-07 JVA Created: 
//-----------------------------------------------------------------------------
void CYLINDER::GetRandomPoint(VECTOR &Point) const
{
	float	x, y, z;
	//	VECTOR	Vector;
	float	rRandAngle;
	float	rRandRadius;
	float	rRandHeight;
	float	rRandCos, rRandSin;
	
	rRandAngle = MATH::Rand(MATH::PI() * 2.0f);
	rRandRadius = MATH::Rand(GetRadius());
	rRandHeight = MATH::Rand(GetHeight());

	MATH::GetCosinusAndSinus(rRandAngle, rRandCos, rRandSin);
	
	x = rRandCos * rRandRadius;
	y = rRandSin * rRandRadius;
	z = -rRandHeight;
	
	Point.SetXYZ(x, y, z);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
bool CYLINDER::operator == (const CYLINDER &C) const
{
	bool Equal = true;
	
	if (this != &C)
	{
		Equal =((_Center == C._Center) &&
				(GetRadius() == C.GetRadius()) &&
				(GetHeight() == C.GetHeight()) &&
				(_XDir == C._XDir) &&
				(_YDir == C._YDir) &&
				(_ZDir == C._ZDir));
	}
	
	return Equal;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

