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
//	CLASS:	BOUNDING_BOX
//
//	02-05-29:	JLT - Created
//*****************************************************************************

/*

#include "Include.h"


#ifdef _DEBUG
	#include "BoundingBox.inl"
#else
	#include "Inline.h"
		
	#include INL_KRNCORE(Display/RenderManager)
	#include INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)
	#include INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
#endif
	
//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*		BOUNDING_BOX::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		BOUNDING_BOX constructor
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_BOX::BOUNDING_BOX()
:
BOUNDING_VOLUME	(TYPE_BOX)
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		BOUNDING_BOX constructor
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_BOX::BOUNDING_BOX(const POINT3D &	Min,
						   const POINT3D &	Max)
:
BOUNDING_VOLUME	(TYPE_BOX)
{
	Compute(Min, Max);
}


//-----------------------------------------------------------------------------
//	Name:		BOUNDING_BOX constructor
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_BOX::BOUNDING_BOX(const BOUNDING_BOX & C)
:
BOUNDING_VOLUME	(TYPE_BOX)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BOUNDING_BOX destructor
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_BOX::~BOUNDING_BOX()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_BOX& BOUNDING_BOX::operator=(const BOUNDING_BOX &C)
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
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
void BOUNDING_BOX::Copy(const BOUNDING_BOX & C)
{
	_Center = C._Center;
	_HalfDiag = C._HalfDiag;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-07-03:	JLT - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void BOUNDING_BOX::Read(IO_STREAM &	s)
{
	s >> _Center >> _HalfDiag;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-07-03:	JLT - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void BOUNDING_BOX::Write(IO_STREAM &	s) const
{
	s << _Center << _HalfDiag;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	BOUNDING_BOX::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(BOUNDING_BOX) );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-07-04:	JLT - Created
//-----------------------------------------------------------------------------
void BOUNDING_BOX::Display(const COLOR_FLOAT &Color) const
{
#ifndef _MASTER
	POINT3D Pt0, Pt1, Pt2, Pt3, Pt4, Pt5, Pt6, Pt7;
	POINT3D U, L;

	U = _Center + _HalfDiag;
	L = _Center - _HalfDiag;
	
	
	Pt0.SetXYZ( U[_X_], U[_Y_], U[_Z_] );
	Pt1.SetXYZ( L[_X_], U[_Y_], U[_Z_] );
	Pt2.SetXYZ( L[_X_], L[_Y_], U[_Z_] );
	Pt3.SetXYZ( U[_X_], L[_Y_], U[_Z_] );
	
	Pt4.SetXYZ( U[_X_], U[_Y_], L[_Z_] );
	Pt5.SetXYZ( L[_X_], U[_Y_], L[_Z_] );
	Pt6.SetXYZ( L[_X_], L[_Y_], L[_Z_] );
	Pt7.SetXYZ( U[_X_], L[_Y_], L[_Z_] );
	
	RENDER_MANAGER::Instance()->GetRenderContext()->SetAlphaBlendDstFactor(RENDER_CONTEXT::BLENDFACTOR_ZERO);
	RENDER_MANAGER::Instance()->GetRenderContext()->SetAlphaBlendSrcFactor(RENDER_CONTEXT::BLENDFACTOR_ONE);
	
	//Top
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt0, Pt1), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt1, Pt2), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt2, Pt3), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt3, Pt0), Color );
	
	//Bottom
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt4, Pt5), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt5, Pt6), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt6, Pt7), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt7, Pt4), Color );
	
	//Sides
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt0, Pt4), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt1, Pt5), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt2, Pt6), Color );
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( SEGMENT(Pt3, Pt7), Color );
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BOUNDING_BOX::Transform(const MATRIX &	Transform)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const PLANE &	Plane,
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
bool	BOUNDING_BOX::Intersect(const RAY &	Ray,
								POINT3D &	HitPoint) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const RAY &	Ray,
								float &		rDistance) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-07-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const HEXAHEDRON &	Hexahedron,
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
bool	BOUNDING_BOX::Intersect(const HEXAHEDRON &	Hexahedron,
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
bool	BOUNDING_BOX::IntersectFront(const HEXAHEDRON &	Hexahedron,
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
bool	BOUNDING_BOX::IntersectFront(const HEXAHEDRON &	Hexahedron,
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
bool	BOUNDING_BOX::Intersect(const FRUSTRUM &	Frustrum) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME &	BOUNDING_BOX::operator=(const BOUNDING_VOLUME &	BoundingVolume)
{
	QDT_ASSERT(BoundingVolume.GetType() == GetType());
	return (*this = static_cast<const BOUNDING_BOX &>(BoundingVolume));
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const CONE &	Cone) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
float	BOUNDING_BOX::GetSize() const
{
	return (_HalfDiag.Length() * 2.0f);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const SPHERE &	Sphere) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const AABB &	Aabb) const
{
	QDT_NOT_IMPLEMENTED();
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(const POINT3D &	Point) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_BOX::Intersect(VERTICAL_RAY &	VerticalRay) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetOptimalFocalAngle
//	Object:		
//	03-04-16:	ELE - Created
//-----------------------------------------------------------------------------
float	BOUNDING_BOX::GetOptimalFocalAngle(const POINT3D &	Pos,
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
//	02-05-30:	JLT - Created
//-----------------------------------------------------------------------------
const VECTOR &	BOUNDING_BOX::GetCenter() const
{
	return _Center;
}*/

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
