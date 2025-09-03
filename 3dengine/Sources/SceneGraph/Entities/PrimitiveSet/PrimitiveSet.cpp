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
//	CLASS:	PRIMITIVE_SET
//
//	02-12-19:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/PrimitiveSet/PrimitiveSet)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComplexType/TypeVector)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/Viewport/Viewport)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE_SET constructor
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
PRIMITIVE_SET::PRIMITIVE_SET()
:
ENTITY_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_PRIMITIVE_SET)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE_SET destructor
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
PRIMITIVE_SET::~PRIMITIVE_SET()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::Register(void)
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->AddPrimitive(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::Unregister(void)
{
	SCENE_GRAPH * pSG = GetSceneGraph();
			
	if	(pSG)
	{
		pSG->RemovePrimitive(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-12-20:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::Copy(const PRIMITIVE_SET& PrimSet)
{
	_vLines		= PrimSet._vLines;
	_vTriangles	= PrimSet._vTriangles;
	//_vBoxForIntersection	= PrimSet._vBoxForIntersection;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::Clear(void)
{
	_vLines.Clear();
	_vTriangles.Clear();
}

/*
//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	03-05-26:	FDE - Created
//-----------------------------------------------------------------------------
bool PRIMITIVE_SET::RayCast(RAY_CAST_PARAMETERS& RayCastParameters, bool& bContinue)
{
	RAY				Ray				= RayCastParameters.GetRay();
	RAY				LocalRay		= Ray;
	bool			bRes			= false;
	float			rDistance		= 0.0f;
	unsigned int	nNbrTriangles	= _vBoxForIntersection.GetSize();
	unsigned int	i;

	bContinue = true;

	if (!RayCastParameters.CatchPrimitiveSet()) 
	{
		return (false);
	}

	// Compute local ray from inverse matrix of the global matrix of the mesh
	MATRIX	Inverse;
	Inverse.InverseAffine(RayCastParameters.GetGlobalMatrix());
	LocalRay = RayCastParameters.GetRay();
	LocalRay.Transform(Inverse);

	for (i = 0; i < nNbrTriangles; ++i)
	{
		if (INTERSECTION::FindIntersectionRayTriangle(LocalRay, _vBoxForIntersection[i], rDistance))
		{
			// Check if the distance is the nearest
			if (RayCastParameters.TestTriangleHit( _vBoxForIntersection[i], rDistance))
			{
				bRes = true;
			}
		}
	}
	return bRes;
}
*/

//-----------------------------------------------------------------------------
//	Name:		PushBoxForIntersection
//	Object:		
//	03-05-27:	FDE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushBoxForIntersection(const VECTOR& Min, const VECTOR& Max)
{
	VECTOR Pt0, Pt1, Pt2, Pt3, Pt4, Pt5, Pt6, Pt7;
	VECTOR U, L;

	VECTOR	HalfDiag	= (Max - Min) * 0.5f;
	VECTOR	Center		= Min + HalfDiag;

	U = Center + HalfDiag;
	L = Center - HalfDiag;

	Pt0.SetXYZ( U[_X_], U[_Y_], U[_Z_] );
	Pt1.SetXYZ( L[_X_], U[_Y_], U[_Z_] );
	Pt2.SetXYZ( L[_X_], L[_Y_], U[_Z_] );
	Pt3.SetXYZ( U[_X_], L[_Y_], U[_Z_] );

	Pt4.SetXYZ( U[_X_], U[_Y_], L[_Z_] );
	Pt5.SetXYZ( L[_X_], U[_Y_], L[_Z_] );
	Pt6.SetXYZ( L[_X_], L[_Y_], L[_Z_] );
	Pt7.SetXYZ( U[_X_], L[_Y_], L[_Z_] );

	//Top
	_vBoxForIntersection.PushTail( TRIANGLE(Pt0, Pt1, Pt2) );
	_vBoxForIntersection.PushTail( TRIANGLE(Pt0, Pt2, Pt3) );

	//Bottom
	_vBoxForIntersection.PushTail( TRIANGLE(Pt5, Pt4, Pt6) );
	_vBoxForIntersection.PushTail( TRIANGLE(Pt6, Pt4, Pt7) );

	//Sides
	_vBoxForIntersection.PushTail( TRIANGLE(Pt4, Pt0, Pt3) );
	_vBoxForIntersection.PushTail( TRIANGLE(Pt4, Pt3, Pt7) );

	_vBoxForIntersection.PushTail( TRIANGLE(Pt7, Pt6, Pt2) );
	_vBoxForIntersection.PushTail( TRIANGLE(Pt7, Pt2, Pt3) );

	_vBoxForIntersection.PushTail( TRIANGLE(Pt2, Pt5, Pt6) );
	_vBoxForIntersection.PushTail( TRIANGLE(Pt5, Pt2, Pt1) );

	_vBoxForIntersection.PushTail( TRIANGLE(Pt4, Pt5, Pt1) );
	_vBoxForIntersection.PushTail( TRIANGLE(Pt4, Pt1, Pt0) );
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMinMax
//	Object:		
//	03-05-27:	FDE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE_SET::UpdateMinMax(const VECTOR &	Pt1,
									const VECTOR &	Pt2,
									const VECTOR &	Pt3,
									VECTOR &		Min,
									VECTOR &		Max)
{
	UpdateMinPoint(Pt1, Min);
	UpdateMinPoint(Pt2, Min);
	UpdateMinPoint(Pt3, Min);

	UpdateMaxPoint(Pt1, Max);
	UpdateMaxPoint(Pt2, Max);
	UpdateMaxPoint(Pt3, Max);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMinPoint
//	Object:		
//	03-05-27:	FDE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE_SET::UpdateMinPoint(const VECTOR	&	Pt,
									  VECTOR &			Min)
{
	if (Pt.GetX() < Min.GetX())
	{
		Min.SetX(Pt.GetX());
	}
	if (Pt.GetY() < Min.GetY())
	{
		Min.SetY(Pt.GetY());
	}
	if (Pt.GetZ() < Min.GetZ())
	{
		Min.SetZ(Pt.GetZ());
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMaxPoint
//	Object:		
//	03-05-27:	FDE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE_SET::UpdateMaxPoint(const VECTOR	&	Pt,
									  VECTOR &			Max)
{
	if (Pt.GetX() > Max.GetX())
	{
		Max.SetX(Pt.GetX());
	}
	if (Pt.GetY() > Max.GetY())
	{
		Max.SetY(Pt.GetY());
	}
	if (Pt.GetZ() > Max.GetZ())
	{
		Max.SetZ(Pt.GetZ());
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushLine
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushLine(const VECTOR& vStart, const VECTOR& vEnd, const COLOR& Color, const bool bBuildBoxForRaycast)
{
	_vLines.PushTail(DEBUG_VERTEX(vStart,Color));
	_vLines.PushTail(DEBUG_VERTEX(vEnd,Color));

	if (bBuildBoxForRaycast)
	{
		VECTOR	vMin = vStart, vMax = vEnd;
		UpdateMinPoint(vStart, vMin);
		UpdateMaxPoint(vStart, vMax);

		UpdateMinPoint(vEnd, vMin);
		UpdateMaxPoint(vEnd, vMax);

		vMin.SetXYZ(vMin.GetX() - 2.0f, vMin.GetY() - 2.0f, vMin.GetZ() - 2.0f);
		vMax.SetXYZ(vMax.GetX() + 2.0f, vMax.GetY() + 2.0f, vMax.GetZ() + 2.0f);
		PushBoxForIntersection(vMin, vMax);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushTriangle
//	Object:		
//	03-05-22:	FDE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushTriangle(const VECTOR& vPt1, const VECTOR& vPt2, const VECTOR& vPt3, const COLOR& Color, const bool bBuildBoxForRaycast)
{
	_vTriangles.PushTail(DEBUG_VERTEX(vPt1, Color));
	_vTriangles.PushTail(DEBUG_VERTEX(vPt2, Color));
	_vTriangles.PushTail(DEBUG_VERTEX(vPt3, Color));
}

//-----------------------------------------------------------------------------
//	Name:		PushBox
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushBox(const VECTOR& vMin, const VECTOR& vMax, const COLOR& Color, const bool bBuildBoxForRaycast)
{
	VECTOR	vHalfDiag	= (vMax - vMin) * 0.5f;
	VECTOR	vCenter		= vMin + vHalfDiag;

	VECTOR U(vCenter + vHalfDiag);
	VECTOR L(vCenter - vHalfDiag);

	VECTOR vPt0( U[_X_], U[_Y_], U[_Z_] );
	VECTOR vPt1( L[_X_], U[_Y_], U[_Z_] );
	VECTOR vPt2( L[_X_], L[_Y_], U[_Z_] );
	VECTOR vPt3( U[_X_], L[_Y_], U[_Z_] );
	VECTOR vPt4( U[_X_], U[_Y_], L[_Z_] );
	VECTOR vPt5( L[_X_], U[_Y_], L[_Z_] );
	VECTOR vPt6( L[_X_], L[_Y_], L[_Z_] );
	VECTOR vPt7( U[_X_], L[_Y_], L[_Z_] );

	PushTriangle( vPt0, vPt1, vPt2, Color );	//Top
	PushTriangle( vPt0, vPt2, vPt3, Color );
	PushTriangle( vPt5, vPt4, vPt6, Color );	//Bottom
	PushTriangle( vPt6, vPt4, vPt7, Color );
	PushTriangle( vPt4, vPt0, vPt3, Color );	//Sides
	PushTriangle( vPt4, vPt3, vPt7, Color );
	PushTriangle( vPt7, vPt6, vPt2, Color );
	PushTriangle( vPt7, vPt2, vPt3, Color );
	PushTriangle( vPt2, vPt5, vPt6, Color );
	PushTriangle( vPt5, vPt2, vPt1, Color );
	PushTriangle( vPt4, vPt5, vPt1, Color );
	PushTriangle( vPt4, vPt1, vPt0, Color );

	//contour
	COLOR ColorContour = Color.GetInverse();
	PushLine( vPt0, vPt1, ColorContour );	//face
	PushLine( vPt1, vPt2, ColorContour );
	PushLine( vPt2, vPt3, ColorContour );
	PushLine( vPt3, vPt0, ColorContour );
	PushLine( vPt4, vPt5, ColorContour );	//behind
	PushLine( vPt5, vPt6, ColorContour );
	PushLine( vPt6, vPt7, ColorContour );
	PushLine( vPt7, vPt4, ColorContour );
	PushLine( vPt0, vPt4, ColorContour );	//sides
	PushLine( vPt1, vPt5, ColorContour );
	PushLine( vPt2, vPt6, ColorContour );
	PushLine( vPt3, vPt7, ColorContour );

	if (bBuildBoxForRaycast)
	{
		PushBoxForIntersection(vMin, vMax);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushCone
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushCone(const VECTOR& vTop, const VECTOR& vAxe, const Float32 rRadius, const Float32	rLength, const COLOR& Color, const Int32 nSubdiv, const bool bBuildBoxForRaycast)
{
	Float32	rReason;
	Int32	nIdx;
	PLANE	Plane;
	VECTOR	V1, V2;
	VECTOR	vCenter, vMin = vTop, vMax = vTop;
	COLOR	ColorContour = Color.GetInverse();

	vCenter = vTop + vAxe * rLength;
	Plane.SetFromPointAndNormal(VECTOR::ZERO, vAxe);
	Plane.GetVectors( V1, V2 );

	rReason	= 2.0f * MATH::PI() / (Float32)nSubdiv;

	// Display Disc
	for	( nIdx = 0; nIdx < nSubdiv; ++nIdx )
	{
		VECTOR	vStartPoint, vEndPoint;
		Float32	rStartAngle, rEndAngle;

		rStartAngle = nIdx			* rReason;
		rEndAngle	= (nIdx + 1)	* rReason;

		// Compute Points
		vStartPoint	= (rRadius / 2.0f) * (MATH::Cos(rStartAngle) * V1 + MATH::Sin(rStartAngle) * V2);
		vEndPoint	= (rRadius / 2.0f) * (MATH::Cos(rEndAngle) * V1 + MATH::Sin(rEndAngle) * V2);

		vStartPoint	+= vCenter;
		vEndPoint	+= vCenter;

		PushTriangle(vStartPoint, vEndPoint, vCenter, Color);
		PushTriangle(vEndPoint, vStartPoint, vTop, Color);

		PushLine(vStartPoint, vEndPoint, ColorContour);
		PushLine(vStartPoint, vTop, ColorContour);

		if (bBuildBoxForRaycast)
		{
			UpdateMinMax(vStartPoint, vEndPoint, vCenter, vMin, vMax);
			UpdateMinMax(vEndPoint, vStartPoint, vTop, vMin, vMax);
		}
	}

	if (bBuildBoxForRaycast)
	{
		PushBoxForIntersection(vMin, vMax);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushCylinder
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushCylinder(const VECTOR& vCenter, const VECTOR& vCylDir, const Float32 rCylRadius, const Float32 rCylHeight, const COLOR& Color, const Int32 nSubdiv, const bool bBuildBoxForRaycast)
{
	VECTOR			XDir(1.0f, 0.0f, 0.0f), YDir(0.0f, 1.0f, 0.0f), ZDir(0.0f, 0.0f, 1.0f);
	const Float32	dA	= MATH::PI() * 2.f / (Float32)(nSubdiv);
	Float32			A	= 0.f;

	VECTOR*	Pts = new VECTOR[2 * nSubdiv];
	VECTOR	CenterHeight(vCenter), vMin = vCenter, vMax = vCenter;
	CenterHeight.SetY(vCenter.GetY() + rCylHeight);

	VECTOR	vCylDirNormalized(vCylDir);
	vCylDirNormalized.Normalize();
	QUATERNION Q(ZDir, vCylDirNormalized);
	XDir = Q * XDir;
	YDir = Q * YDir;
	ZDir.Copy(vCylDirNormalized);

	Int32	i;
	for (i = 0; i < nSubdiv; ++i)
	{
		Float32 dX;
		Float32 dY;
		MATH::GetCosinusAndSinus(A, dX, dY);
		dX *= rCylRadius;
		dY *= rCylRadius;

		Pts[i] = vCenter;
		Pts[i] += XDir * dX;
		Pts[i] += YDir * dY;

		Pts[i + nSubdiv] = vCenter + ZDir * rCylHeight;
		Pts[i + nSubdiv] += XDir * dX;
		Pts[i + nSubdiv] += YDir * dY;

		A += dA;
	}

	COLOR ColorContour = Color.GetInverse();
	for (i=0; i<nSubdiv; ++i)
	{
		PushTriangle(Pts[i],							Pts[(i + 1) % nSubdiv],				Pts[i + nSubdiv],		Color);
		PushTriangle(Pts[((i + 1) % nSubdiv) + nSubdiv],Pts[i + nSubdiv],					Pts[(i + 1) % nSubdiv],	Color);
		PushTriangle(Pts[(i + 1) % nSubdiv],			Pts[i],								vCenter,					Color);
		PushTriangle(Pts[i + nSubdiv],					Pts[((i + 1) % nSubdiv) + nSubdiv], CenterHeight,			Color);

		PushLine(Pts[i], Pts[i+nSubdiv], ColorContour);

		if (bBuildBoxForRaycast)
		{
			UpdateMinMax(Pts[i], Pts[(i + 1) % nSubdiv], Pts[i + nSubdiv], vMin, vMax);
			UpdateMinMax(Pts[((i + 1) % nSubdiv) + nSubdiv], Pts[i + nSubdiv], Pts[(i + 1) % nSubdiv], vMin, vMax);
			UpdateMinMax(Pts[(i + 1) % nSubdiv], Pts[i], vCenter, vMin, vMax);
			UpdateMinMax(Pts[i + nSubdiv], Pts[((i + 1) % nSubdiv) + nSubdiv], CenterHeight, vMin, vMax);
		}
	}

	for (i=0; i<nSubdiv; ++i)
	{
		PushLine(Pts[i],Pts[(i+1)%nSubdiv], ColorContour);
		PushLine(Pts[i+nSubdiv],Pts[((i + 1) % nSubdiv) + nSubdiv], ColorContour);
	}

	if (bBuildBoxForRaycast)
	{
		PushBoxForIntersection(vMin, vMax);
	}

	delete [] Pts;
}


//-----------------------------------------------------------------------------
//	Name:		PushSphere
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushSphere(const VECTOR& vCenter, Float32 rRadius, const COLOR& Color, const Int32 nSubdiv, const bool bBuildBoxForRaycast)
{
	Float32				rTheta, rPhi;			// Angles used to sweep around sphere  
	Float32				rDTheta, rDPhi;			// Angle between each section and ring 
	Float32				rY, rV, rRSinTheta;		// Temporary variables  
	Float32				rCos, rSin;
	Int32				i, j, n, m;				// counters  
	Int32				nVert;					// Internal vertex and triangle count  
	VECTOR				Vtx, vMin, vMax;
	QDT_VECTOR<VECTOR>	vVtx;

	nVert = (nSubdiv + 1) * nSubdiv + 2;

	// Generate vertices at the top and bottom points. 
	Vtx.SetXYZ(0.0f, rRadius, 0.0f); 
	Vtx += vCenter;
	vVtx.PushTail(Vtx);

	// Generate vertex points for rings 
	rDTheta		= MATH::PI() / (Float32) (nSubdiv + 2); 
	rDPhi		= 2.0f * MATH::PI() / (Float32) nSubdiv; 
	n			= 1;								// vertex being generated, begins at 1 to skip top point 
	rTheta		= rDTheta; 

	for (i = 0; i <= nSubdiv; ++i) 
	{ 
		MATH::GetCosinusAndSinus(rTheta, rCos, rSin);
		rY			= (Float32)(rRadius * rCos);			// y is the same for each ring
		rV			= rTheta / MATH::PI();					// v is the same for each ring 
		rRSinTheta	= (Float32)(rRadius * rSin); 
		rPhi		= 0.0f; 

		for (j = 0; j < nSubdiv; ++j) 
		{ 
			MATH::GetCosinusAndSinus(rPhi, rCos, rSin);
			Vtx.SetXYZ(rRSinTheta * rSin, rY, rRSinTheta * rCos);
			Vtx		+= vCenter;
			rPhi	+= rDPhi; 
			vVtx.PushTail(Vtx);
		} 

		rTheta += rDTheta; 
	} 

	Vtx.SetXYZ(0.0f, -rRadius, 0.0f);
	Vtx += vCenter;
	vVtx.PushTail(Vtx);

	vMin = vVtx[0];
	vMax = vVtx[nVert - 1];

	// Generate triangles for top and bottom caps. 
	for (i = 0; i < nSubdiv; ++i) 
	{ 
		PushTriangle(vVtx[0], vVtx[i + 1], vVtx[1 + ((i + 1) % nSubdiv)], Color);
		PushTriangle(vVtx[nVert - 1], vVtx[nVert - 2 - i], vVtx[nVert - 2 - ((1 + i) % nSubdiv)], Color);

		UpdateMinMax(vVtx[0], vVtx[i + 1], vVtx[1 + ((i + 1) % nSubdiv)], vMin, vMax);
		UpdateMinMax(vVtx[nVert - 1], vVtx[nVert - 2 - i], vVtx[nVert - 2 - ((1 + i) % nSubdiv)], vMin, vMax);
	} 

	// Generate triangles for the rings  
	m = 1;					// first vertex in current ring,begins at 1 to skip top point
	n = nSubdiv;			// triangle being generated, skip the top cap

	for (i = 0; i < nSubdiv; ++i) 
	{ 
		for (j = 0; j < nSubdiv; ++j) 
		{ 
			PushTriangle(vVtx[m + j], vVtx[m + nSubdiv + j], vVtx[m + nSubdiv + ((j + 1) % nSubdiv)], Color);
			PushTriangle(vVtx[m + j], vVtx[m + nSubdiv + ((j + 1) % nSubdiv)], vVtx[m + ((j + 1) % nSubdiv)], Color);
			n += 2;

			if (bBuildBoxForRaycast)
			{
				UpdateMinMax(vVtx[m + j], vVtx[m + nSubdiv + j], vVtx[m + nSubdiv + ((j + 1) % nSubdiv)], vMin, vMax);
				UpdateMinMax(vVtx[m + j], vVtx[m + nSubdiv + ((j + 1) % nSubdiv)], vVtx[m + ((j + 1) % nSubdiv)], vMin, vMax);
			}
		} 

		m += nSubdiv; 
	}

	if (bBuildBoxForRaycast)
	{
		PushBoxForIntersection(vMin, vMax);
	}

	//contour
	COLOR ColorContour = Color.GetInverse();
	PushCircle( vCenter, VECTOR::UNIT_X, rRadius, ColorContour, nSubdiv*2 );
	PushCircle( vCenter, VECTOR::UNIT_Y, rRadius, ColorContour, nSubdiv*2 );
	PushCircle( vCenter, VECTOR::UNIT_Z, rRadius, ColorContour, nSubdiv*2 );
}

//-----------------------------------------------------------------------------
//	Name:		PushCircle
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::PushCircle(const VECTOR& vCenter, const VECTOR& Normal, Float32 rRadius, const COLOR& Color, const Int32 nNbrSubdivisions, const bool bBuildBoxForRaycast)
{
	Float32	rReason;
	Int32	nIdx;
	PLANE	Plane;
	VECTOR	V1, V2;

	Plane.SetFromPointAndNormal(VECTOR::ZERO, Normal);
	Plane.GetVectors( V1, V2 );

	rReason	= 2.0 * MATH::PI() / (Float32)nNbrSubdivisions;

	// Display Disc
	for	( nIdx = 0; nIdx < nNbrSubdivisions; ++nIdx )
	{
		VECTOR	StartPoint, EndPoint;
		Float32	rStartAngle, rEndAngle;

		rStartAngle = nIdx			* rReason;
		rEndAngle	= ( nIdx + 1 )	* rReason;

		// Compute Points
		StartPoint	= rRadius * ( MATH::Cos( rStartAngle )	* V1 + MATH::Sin( rStartAngle ) * V2 );
		EndPoint	= rRadius * ( MATH::Cos( rEndAngle )	* V1 + MATH::Sin( rEndAngle )	* V2 );

		StartPoint	+= vCenter;
		EndPoint	+= vCenter;

		// Add Segment
		PushLine( StartPoint, EndPoint, Color, bBuildBoxForRaycast);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::Display(void)
{
	const DISPLAY_MANAGER*	pDisplay	=DISPLAY_MANAGER::Instance();
	const CAMERA*			pCamera		=pDisplay->GetActiveCamera();
	MATRIX					mViewMatrix =pCamera->GetViewMatrix();
	const MATRIX&			mProjMatrix =pCamera->GetProjectionMatrix();
	
	MATRIX Matrix = mViewMatrix;

	NODE* pNode = GetNode();
	if (pNode)
	{
		Matrix = pNode->GetGlobalMatrix();
		Matrix.Mul(mViewMatrix);
	}

	VIEWPORT* pViewport= pDisplay->GetCurrentViewport();
	pViewport->EnablePostProcessing(false);					//doesn't work :p

	const UInt32 nLines =_vLines.GetSize()/2;
	if (nLines>0)
	{
		RENDER_CONTEXT::DrawLines(nLines, &(_vLines[0]), Matrix, mProjMatrix);
	}

	const UInt32 nTriangles =_vTriangles.GetSize()/3;
	if (nTriangles>0)
	{
		RENDER_CONTEXT::DrawTriangles(nTriangles, &(_vTriangles[0]), Matrix, mProjMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR*		pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("PushLine"), QDT_COMMENT("Push a line."));
	pMD->AddParameter(QDT_COMMENT("Start"), QDT_COMMENT("Start point of the line."), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("End"), QDT_COMMENT("End point of the line."), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT("Color of the line."), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_PUSH_LINE, pMD, EM_PushLine);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("Clear"), QDT_COMMENT("Clear all primitives."));
	pCD->AddMethod(CMD_CLEAR, pMD, EM_Clear);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR* PRIMITIVE_SET::GetGenericCommunicatorDescriptor(void)
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PRIMITIVE_SET) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR* PRIMITIVE_SET::CreateGenericCommunicatorDescriptor(void)
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PRIMITIVE_SET"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PRIMITIVE_SET,
										QDT_COMMENT("671641C7-76D0-F862-C1A8-6BE08B1443E6"), 
										sizeof(PRIMITIVE_SET), 
										ComInit, 
										QDT_COMMENT("Primitive set is an entity used to primitive displaying."),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::EM_Construct(void* const pO, void* const pR, void* const pP)
{
	PRIMITIVE_SET* pPS;
	pPS = static_cast<PRIMITIVE_SET *>(pO);

	QDT_NEW_PLACED(pPS, PRIMITIVE_SET)();
	QDT_ASSERT(pPS);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::EM_Destruct(void* const pO, void* const pR, void* const pP)
{
	PRIMITIVE_SET* pPS;
	pPS = static_cast<PRIMITIVE_SET *>(pO);

	QDT_DELETE_PLACED(pPS, PRIMITIVE_SET);
}

//-----------------------------------------------------------------------------
//	Name:		EM_PushLine
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::EM_PushLine(void* const pO, void* const pR, void* const pP)
{
	struct PARAMETERS
	{
		VECTOR	_Start;
		VECTOR	_End;
		COLOR	_Color;
	};

	PRIMITIVE_SET*	pPS;
	COM_INSTANCE*	pComInstance;
	PARAMETERS*		pParam;

	pComInstance = static_cast<COM_INSTANCE *>(pO);
	pPS = static_cast<PRIMITIVE_SET*>(pComInstance->GetCommunicator());
	pParam = static_cast<PARAMETERS*>(pP);

	pPS->PushLine(pParam->_Start, pParam->_End, pParam->_Color);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Clear
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void PRIMITIVE_SET::EM_Clear(void* const pO, void* const pR, void* const pP)
{
	PRIMITIVE_SET*	pPS;
	COM_INSTANCE*	pComInstance;

	pComInstance = (COM_INSTANCE *) pO;
	pPS = static_cast<PRIMITIVE_SET*>(pComInstance->GetCommunicator());

	pPS->Clear();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
