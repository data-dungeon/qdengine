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
//	CLASS:	COLLISION_MESH
//
//	03-08-07:	JPE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CollisionMesh)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Face)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Edge)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Sector)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Collision/CollisionManager)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include	INCL_KMATH(Math/Color/ColorFloat)


#define SATURATE_BETWEEN(Value, Min, Max)		if(Value < Min)				\
													Value = Min;			\
												else if(Value > Max)		\
													Value = Max;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

int									COLLISION_MESH::_nDebugDisplayMode	= 0;

#ifndef _MASTER
	SEGMENT		COLLISION_MESH::_SegmentListTbl[MAX_SEG];
	int			COLLISION_MESH::_nUsedSegments = 0;
	COLOR_FLOAT	COLLISION_MESH::_SegmentTblColors[MAX_SEG];
#endif


//-----------------------------------------------------------------------------
//	Name:		COLLISION_MESH constructor
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MESH::COLLISION_MESH()
:
ENTITY_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_COLLISION_MESH),
_pSectors		(0),
_StepsX			(0),
_StepsY			(0),
_pVertices		(0),
_pFVtx			(0),
_pEdges			(0),
_pEdgesNormals	(0),
_pFaces			(0),
_nbVertices		(0),
_nbEdges		(0),
_nbFaces		(0),
_nbFVtx			(0),
#ifndef _MASTER
_s1x			(0),
_s1y			(0),
_s2x			(0),
_s2y			(0),
_EdgeID1		(0),
_EdgeID2		(0),
#endif
_nVersion		(0),
_nYRepMode		(COLLISION_MESH::YRM_RAYCAST)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COLLISION_MESH destructor
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MESH::~COLLISION_MESH()
{
	COLLISION_MANAGER::Instance()->RemoveMesh( this );

	if(_pFaces)
		delete[] _pFaces;

	if(_pVertices)
		delete[] _pVertices;

	if(_pFVtx)
		delete[] _pFVtx;

	if(_pEdges)
		delete[] _pEdges;

	if(_pEdgesNormals)
		delete[] _pEdgesNormals;

	if(_pSectors)
	{
		int i;
		for(i=0; i<(_StepsX*_StepsY); i++)
		{
			if(_pSectors[i])
				delete _pSectors[i];
		}
		delete[] _pSectors;
	}

	_pEdgesNormals	= 0;
	_pFVtx			= 0;
	_pSectors		= 0;
	_pFaces			= 0;
	_pVertices		= 0;
	_pEdges			= 0;
}

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		DebugSetSectors
//	Object:		
//	04-02-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::AddLineToDebugDisplay(SEGMENT &Segment, COLOR_FLOAT &Color)
{
	if(_nUsedSegments < MAX_SEG)
	{
		_SegmentListTbl[_nUsedSegments] = Segment;
		_SegmentTblColors[_nUsedSegments] = Color;
		_nUsedSegments++;
	}
}


//-----------------------------------------------------------------------------
//	Name:		DebugSetSectors
//	Object:		
//	04-02-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::DebugSetSectors(int	s1x,
										int	s1y,
										int	s2x,
										int	s2y)
{
	_s1x = s1x;
	_s1y = s1y;
	_s2x = s2x;
	_s2y = s2y;
}

//-----------------------------------------------------------------------------
//	Name:		DebugSetEdges
//	Object:		
//	04-02-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::DebugSetEdges(int	EdgeID1,
									  int	EdgeID2)
{
	_EdgeID1 = EdgeID1;
	_EdgeID2 = EdgeID2;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ExcludeCameraSectors
//	Object:		
//	04-04-26:	FDE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::ExcludeCameraSectors(AABB *	pAABB)
{
	int StartX	= (int)( (pAABB->GetXMin() - _Origin.GetX()) / _StepSize );
	int EndX	= (int)( (pAABB->GetXMax() - _Origin.GetX()) / _StepSize );
	int StartY	= (int)( (pAABB->GetZMin() - _Origin.GetY()) / _StepSize );
	int EndY	= (int)( (pAABB->GetZMax() - _Origin.GetY()) / _StepSize );

	SATURATE_BETWEEN(StartX, 0, _StepsX-1);
	SATURATE_BETWEEN(StartY, 0, _StepsY-1);
	SATURATE_BETWEEN(EndX, 0, _StepsX-1);
	SATURATE_BETWEEN(EndY, 0, _StepsY-1);

	int		ScanX, ScanY;
	for(ScanY=StartY; ScanY<=EndY; ++ScanY)
	{
		for(ScanX=StartX; ScanX<=EndX; ++ScanX)
		{
			int Idx = ScanY*_StepsX + ScanX;
			if( _pSectors[Idx] )
			{
				_pSectors[Idx]->SetFlags( CM_SECTOR::CMSF_CAMERA_DISABLE );
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetCameraSectors
//	Object:		
//	04-04-27:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::ResetCameraSectors(void	)
{
	int		ScanX, ScanY;
	for(ScanY=0; ScanY<_StepsY; ++ScanY)
	{
		for(ScanX=0; ScanX<_StepsX; ++ScanX)
		{
			int Idx = ScanY*_StepsX + ScanX;
			if( _pSectors[Idx] )
			{
				_pSectors[Idx]->ClearFlags( CM_SECTOR::CMSF_CAMERA_DISABLE );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::Display(DISPLAY_PARAMETERS &DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
#ifndef _MASTER
	if(!_nbEdges)
		return;

	RENDER_CONTEXT *pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	QDT_ASSERT(pRC);

	pRC->SetWorldTransform0(&(DisplayParameters.GetNode()->GetGlobalMatrix()));

	if(_nUsedSegments)
	{
		int iSeg;
		for(iSeg=0; iSeg<_nUsedSegments; iSeg++)
			pRC->Render3DSegment(_SegmentListTbl[iSeg], _SegmentTblColors[iSeg]);
		_nUsedSegments = 0;
	}

	if ( DISPLAY_MANAGER::Instance()->DisplayNavigationMesh() )
	{
		int			i;
		COLOR_FLOAT Red(1.0f, 1.0f, 0.0f, 0.0f);
		COLOR_FLOAT Green(1.0f, 0.0f, 1.0f, 0.0f);
		COLOR_FLOAT Yellow(1.0f, 1.0f, 1.0f, 0.0f);
		COLOR_FLOAT Magenta(1.0f, 1.0f, 0.0f, 1.0f);
		COLOR_FLOAT White(0.8f, 0.8f, 0.8f, 0.8f);

		static float	Ang				= 0.0f;
		static int		CosOff			= true;
		static float	HardCodedAlt	= 110.0f;

		if(Ang > 6.28f)
			Ang = 0.0f;
		Ang += 0.01f;

		if(_pFVtx)
			HardCodedAlt = _pFVtx->GetY() + 50.0f;
		else
			HardCodedAlt = _fYLock;

		for(i=0; i<_nbEdges; i++)					// Affichage des Edges
		{
			VECTOR P1;
			VECTOR P2;
			SEGMENT	S;

			POINT2D	&pVect1 = _pVertices[ _pEdges[i].GetIndex(0) ];
			POINT2D	&pVect2 = _pVertices[ _pEdges[i].GetIndex(1) ];

			float Altitude = 10.0f * (MATH::Cos(Ang + i*0.02f)+1.0f);

			if(CosOff)
				Altitude = 5.0f;

			P1.SetXYZ( pVect1.GetX() , HardCodedAlt + Altitude, pVect1.GetY());
			P2.SetXYZ( pVect2.GetX() , HardCodedAlt + Altitude, pVect2.GetY());

			VECTOR		N1 = (P1 + P2) * 0.5f;
			VECTOR		N2 = N1;
			N2.SetX( N2.GetX() + _pEdgesNormals[i].GetX()*10.0f );
			N2.SetZ( N2.GetZ() + _pEdgesNormals[i].GetY()*10.0f );

			if((i == _EdgeID1) || (i == _EdgeID2))
			{
				COLOR_FLOAT RandC(1.0f, ((float)MATH::Rand())/(float)RAND_MAX, 
										((float)MATH::Rand())/(float)RAND_MAX,
										((float)MATH::Rand())/(float)RAND_MAX	);
				S.SetPoints(P1, P2);	pRC->Render3DSegment(S, RandC);
				S.SetPoints(N1, N2);	pRC->Render3DSegment(S, RandC);
			}
			else
			{
				S.SetPoints(P1, P2);	pRC->Render3DSegment(S, Red);
				S.SetPoints(N1, N2);	pRC->Render3DSegment(S, Magenta);
			}

//			if(_pEdges[i].GetVtxFlag(0) == CM_EDGE::EVF_BLOCKING)
//			{
//				N1 = P1;
//				N1.SetY( N1.GetY() + 10.0f);
//				P1.SetY( P1.GetY() - 10.0f);
//				S.SetPoints(N1, P1);	pRC->Render3DSegment(S, Yellow);
//			}
//			if(_pEdges[i].GetVtxFlag(1) == CM_EDGE::EVF_BLOCKING)
//			{
//				N2 = P2;
//				N2.SetY( N2.GetY() + 10.0f);
//				P2.SetY( P2.GetY() - 10.0f);
//				S.SetPoints(N2, P2);	pRC->Render3DSegment(S, Yellow);
//			}
		}

		int Sx, Sy;

		if( _nDebugDisplayMode )
		{
			for(Sx=0; Sx<_StepsX; Sx++)
			{
				for(Sy=0; Sy<_StepsY; Sy++)
				{
					int idx = Sy*_StepsX+Sx;

					if(_pSectors[idx])
					{
						float Altitude;

						if(_pSectors[idx]->IsFlagSet( CM_SECTOR::CMSF_CAMERA_DISABLE ))
							Altitude = HardCodedAlt + 100.0f + 10.0f * (MATH::Cos(Ang+ idx*0.05f)+1.0f);
						else
							Altitude = HardCodedAlt + 100.0f;

						COLOR_FLOAT Exists(1.0f, 0.47f, 0.07f, 0.26f);
						COLOR_FLOAT Flagged(1.0f, 0.05f, 0.93f, 0.63f);

						VECTOR		TL( _Origin.GetX()+Sx*_StepSize, Altitude, _Origin.GetY()+Sy*_StepSize );
						VECTOR		TR( _Origin.GetX()+(Sx+1)*_StepSize, Altitude, _Origin.GetY()+Sy*_StepSize );
						VECTOR		BR( _Origin.GetX()+(Sx+1)*_StepSize, Altitude, _Origin.GetY()+(Sy+1)*_StepSize );
						VECTOR		BL( _Origin.GetX()+Sx*_StepSize, Altitude, _Origin.GetY()+(Sy+1)*_StepSize );
						SEGMENT		C1( TL, TR );
						SEGMENT		C2( TR, BR );
						SEGMENT		C3( BR, BL );
						SEGMENT		C4( BL, TL );
						if(_pSectors[idx]->IsFlagSet( CM_SECTOR::CMSF_CAMERA_DISABLE ))
						{
							pRC->Render3DSegment(C1, Flagged);
							pRC->Render3DSegment(C2, Flagged);
							pRC->Render3DSegment(C3, Flagged);
							pRC->Render3DSegment(C4, Flagged);
						}
						else
						{
							pRC->Render3DSegment(C1, Exists);
							pRC->Render3DSegment(C2, Exists);
						}
					}
				}
			}
		}
	}
#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		IsPointInside
//	Object:		
//	04-02-25:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MESH::IsPointInside(VECTOR &	pt)
{
	if( pt.GetX() < _Origin.GetX() )
		return false;
	if( pt.GetX() > _Max.GetX() )
		return false;
	if( pt.GetZ() < _Origin.GetY() )
		return false;
	if( pt.GetZ() > _Max.GetY() )
		return false;
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		IsPointInside
//	Object:		
//	04-02-25:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MESH::IsPointInside(POINT2D &	pt)
{
	if( pt.GetX() < _Origin.GetX() )
		return false;
	if( pt.GetX() > _Max.GetX() )
		return false;
	if( pt.GetY() < _Origin.GetY() )
		return false;
	if( pt.GetY() > _Max.GetY() )
		return false;
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		ClearUsedFlags
//	Object:		
//	04-03-30:	FDE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::ClearUsedFlags(VECTOR &	Center,
									   float		rRadius)
{
	int StartX	= (int)( ((Center.GetX() - rRadius) - _Origin.GetX()) / _StepSize );
	int EndX	= (int)( ((Center.GetX() + rRadius) - _Origin.GetX()) / _StepSize );
	int StartY	= (int)( ((Center.GetZ() - rRadius) - _Origin.GetY()) / _StepSize );
	int EndY	= (int)( ((Center.GetZ() + rRadius) - _Origin.GetY()) / _StepSize );

	SATURATE_BETWEEN(StartX, 0, _StepsX-1);
	SATURATE_BETWEEN(StartY, 0, _StepsY-1);
	SATURATE_BETWEEN(EndX, 0, _StepsX-1);
	SATURATE_BETWEEN(EndY, 0, _StepsY-1);

	int		ScanX, ScanY;
	for(ScanY=StartY; ScanY<=EndY; ++ScanY)
	{
		for(ScanX=StartX; ScanX<=EndX; ++ScanX)
		{
			int Idx = ScanY*_StepsX + ScanX;
			if( _pSectors[Idx] )
			{
				int iSecEdge;
				CM_SECTOR &Sector = *_pSectors[Idx];

				for(iSecEdge=0; iSecEdge<Sector.GetNbElements(); ++iSecEdge)
				{
					_pEdges[ Sector.GetValue(iSecEdge) ].SetUsed( false );
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		CorrectPosition
//	Object:		
//	04-03-25:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MESH::CM_RETURN_CODES	COLLISION_MESH::CorrectPosition(VECTOR &	Source,
																VECTOR &	Dest,
																float		Dir,
																int		&	bMerged,
																float	&	rMinResizeAllowed)
{
	int StartX	= (int)( (Source.GetX() - _Origin.GetX()) / _StepSize );
	int EndX	= (int)( (Dest.GetX() - _Origin.GetX()) / _StepSize );
	int StartY	= (int)( (Source.GetZ() - _Origin.GetY()) / _StepSize );
	int EndY	= (int)( (Dest.GetZ() - _Origin.GetY()) / _StepSize );

	SATURATE_BETWEEN(StartX, 0, _StepsX-1);
	SATURATE_BETWEEN(StartY, 0, _StepsY-1);
	SATURATE_BETWEEN(EndX, 0, _StepsX-1);
	SATURATE_BETWEEN(EndY, 0, _StepsY-1);

	if(StartX > EndX)
	{
		int Temp = EndX;
		EndX = StartX;
		StartX = Temp;
	}
	if(StartY > EndY)
	{
		int Temp = EndY;
		EndY = StartY;
		StartY = Temp;
	}

	int		ScanX, ScanY;
	int		FinalEdgeID	= -1;
	float	MaxDist		= QDT_FLOAT32_MAX;
	int		bParallel	= false;
	POINT2D	SourceDest( false );
	SourceDest.SetXY(	Dest.GetX() - Source.GetX(),
						Dest.GetZ()	- Source.GetZ()	);

	for(ScanY=StartY; ScanY<=EndY; ++ScanY)
	{
		for(ScanX=StartX; ScanX<=EndX; ++ScanX)
		{
			int Idx = ScanY*_StepsX + ScanX;
			if( _pSectors[Idx] && (!_pSectors[Idx]->IsFlagSet(CM_SECTOR::CMSF_CAMERA_DISABLE)) )
			{
				POINT2D	Intersect;
				int		EdgeID = bMerged;
				switch( _pSectors[Idx]->GetIntersection(Source, Dest, _pVertices, _pEdgesNormals, _pEdges, Intersect, EdgeID) )
				{
					case CM_SECTOR::CMSRC_FULLINTERSECT:
					{
						POINT2D	SI(Source.GetX(), Source.GetZ());
						SI -= Intersect;
						float fLen = SI.GetSquareLength();
						if(fLen < MaxDist)
						{
							MaxDist = fLen;
							FinalEdgeID = EdgeID;
						}
						break;
					}

					case CM_SECTOR::CMSRC_PARALLEL:
						bParallel	= true;
						FinalEdgeID = EdgeID;
						ScanX		= EndX + 1;
						ScanY		= EndY + 1;
						break;
				}
			}
		}
	}

	if( bParallel )
	{
		CM_EDGE	&Edge		= _pEdges[FinalEdgeID];
		POINT2D	&EdgeNormal	= _pEdgesNormals[FinalEdgeID];
		POINT2D E1E0( false );

		Edge.SetUsed(true);

		E1E0.SetXY(		_pVertices[Edge.GetIndex(0)].GetX() - _pVertices[Edge.GetIndex(1)].GetX(),
						_pVertices[Edge.GetIndex(0)].GetY() - _pVertices[Edge.GetIndex(1)].GetY()		);

		if (E1E0.Dot(SourceDest) < 0)
		{
			E1E0.Inverse();
		}

		POINT2D	E1E0Perp(-E1E0.GetY(), E1E0.GetX());

		if ((E1E0Perp.Dot(EdgeNormal) * Dir) > 0)
		{
			E1E0.Inverse();
		}
		E1E0.Normalize();
		E1E0	*= (SourceDest.GetLength()-1.0f);

		Dest.SetX( E1E0.GetX() + Source.GetX() );
		Dest.SetZ( E1E0.GetY() + Source.GetZ() );	

		return	CMRC_CORRECTION_DONE_RESULT_UNSURE;
	}
	else if(FinalEdgeID != -1)
	{
		MaxDist = MATH::Sqrt(MaxDist);
		if( MaxDist > rMinResizeAllowed )
		{
			SourceDest.Normalize();
			SourceDest *= MaxDist;
			Dest.SetX( Source.GetX() + SourceDest.GetX() );
			Dest.SetZ( Source.GetZ() + SourceDest.GetY() );
			return CMRC_CORRECTION_DONE_RESULT_SURE;
		}
		else
		{
			SourceDest.Normalize();
			SourceDest *= rMinResizeAllowed;
			Dest.SetX( Source.GetX() + SourceDest.GetX() );
			Dest.SetZ( Source.GetZ() + SourceDest.GetY() );
		}

		// Projection ortho de la source sur l'edge
		CM_EDGE	&Edge = _pEdges[FinalEdgeID];
		POINT2D E1E0( false );
		POINT2D	E1Source( false );

		Edge.SetUsed(true);

		E1E0.SetXY(		_pVertices[Edge.GetIndex(0)].GetX() - _pVertices[Edge.GetIndex(1)].GetX(),
						_pVertices[Edge.GetIndex(0)].GetY() - _pVertices[Edge.GetIndex(1)].GetY()		);

		E1Source.SetXY(	Source.GetX() - _pVertices[Edge.GetIndex(1)].GetX(),
						Source.GetZ() - _pVertices[Edge.GetIndex(1)].GetY()	);

		E1E0.Normalize();
		float	d3 = E1E0.Dot(E1Source);
/*
		if (rE1E0Len < d3)
			return CMRC_NO_CORRECTION_DONE;
*/
		float	r2 = SourceDest.GetSquareLength();
		POINT2D	E2( E1E0 );
		E2 *= d3;
		E2 += _pVertices[Edge.GetIndex(1)];
		POINT2D E2Source(	Source.GetX() - E2.GetX(),
							Source.GetZ() - E2.GetY()	);
		float	d2 = E2Source.GetSquareLength();

		if( d2 >= r2 )
			return CMRC_NO_CORRECTION_DONE;

 		float	dx = MATH::Sqrt(r2 - d2);

		E1E0 *= dx;			// Solutions are (E2 - E1E0)  &  (E2 + E1E0)

		POINT2D		Tmp(E2);
		const float	ConstEpsilon = 0.0f;

//		Tmp += E1E0;
//		Tmp	-= Source;
		Tmp.SetXY(Tmp.GetX() + E1E0.GetX() - Source.GetX(), Tmp.GetY() + E1E0.GetY() - Source.GetZ());

		if( (Tmp.Cross(SourceDest) * Dir) < ConstEpsilon)
		{
			E1E0.Inverse();
		}

		E2 += E1E0;

		// Check si le point est sur le segment ... 

		switch( Edge.IsPointInside(E2, _pVertices) )
		{
			case CM_EDGE::SIDE_INSIDE:
				Dest.SetX( E2.GetX() );
				Dest.SetZ( E2.GetY() );
				return CMRC_CORRECTION_DONE_RESULT_SURE;
				break;

			case CM_EDGE::SIDE_P1:
				E2.SetXY(	_pVertices[Edge.GetIndex(0)].GetX() - Source.GetX(),
							_pVertices[Edge.GetIndex(0)].GetY() - Source.GetZ()  );
				break;

			case CM_EDGE::SIDE_P2:
				E2.SetXY(	_pVertices[Edge.GetIndex(1)].GetX() - Source.GetX(),
							_pVertices[Edge.GetIndex(1)].GetY() - Source.GetZ()  );
				break;
		}

		if( E2.IsNull() )
		{
			E2.SetXY(	_pVertices[Edge.GetIndex(0)].GetX() - _pVertices[Edge.GetIndex(1)].GetX(),
						_pVertices[Edge.GetIndex(0)].GetY() - _pVertices[Edge.GetIndex(1)].GetY()		);
			if( (E2.Cross(SourceDest) * Dir) < 0.0f)
			{
				E2.Inverse();
			}
		}

		float	rFactor	= MATH::Sqrt(r2) / E2.GetLength();

		E2 *= rFactor;
		Dest.SetX( E2.GetX() + Source.GetX() );
		Dest.SetZ( E2.GetY() + Source.GetZ() );

		return CMRC_CORRECTION_DONE_RESULT_UNSURE;
	}

	return CMRC_NO_CORRECTION_DONE;
}


//-----------------------------------------------------------------------------
//	Name:		VerticalRayCast
//	Object:		
//	04-03-25:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MESH::VerticalRayCast(VECTOR &	Source, float &fAlt)
{
	if( !IsPointInside(Source) )
		return 0;

	int SX		= (int)((Source.GetX() - _Origin.GetX()) / _StepSize);
	int SY		= (int)((Source.GetZ() - _Origin.GetY()) / _StepSize);
	int Sidx	= SY * _StepsX + SX;

	CM_SECTOR	*pSector = _pSectors[Sidx];

	if(!pSector)
		return 0;

	int iSectEntry;

	for(iSectEntry=0; iSectEntry<pSector->GetNbFaces(); iSectEntry++)
	{
		int			iFace		= pSector->GetFaceValue( iSectEntry );
		const float	fTolerance	= 1e-07f;
		CM_FACE		&Face		= _pFaces[iFace];
		VECTOR		*VList		= GetFaceVtx();
		VECTOR		&v0			= VList[ Face.GetIdx(0) ];
		VECTOR		v1			= VList[ Face.GetIdx(1) ] - v0;
		VECTOR		v2			= VList[ Face.GetIdx(2) ] - v0;
		VECTOR		Normal;

		Normal.Cross(v1, v2);
		Normal.Normalize();

		if( MATH::Abs(Normal.GetY()) <= fTolerance )
			continue;
			
		VECTOR kDiff0 = Source - v0;
		VECTOR kDiff1 = kDiff0;
		float fTime = - Normal.Dot(kDiff0) / Normal.GetY();
		kDiff1.SetY( kDiff1.GetY() + fTime );

		float fE00 = v1.SquareLength();
		float fE01 = v1.Dot(v2);
		float fE11 = v2.SquareLength();
		float fDet = MATH::Abs(fE00*fE11-fE01*fE01);
		float fR0 = v1.Dot(kDiff1);
		float fR1 = v2.Dot(kDiff1);

		float fS0 = fE11*fR0 - fE01*fR1;
		float fS1 = fE00*fR1 - fE01*fR0;

		if ( fS0 >= 0.0 && fS1 >= 0.0 && fS0 + fS1 <= fDet )
		{
			fAlt = kDiff1.GetY() + v0.GetY();
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
bool	COLLISION_MESH::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	COLLISION_MESH::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MESH) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	COLLISION_MESH::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("COLLISION_MESH"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MESH,
										QDT_COMMENT("00C72AB2-7E81-49b0-B3DD-BC610F501365"),
										sizeof(COLLISION_MESH), 
										ComInit, 
										QDT_COMMENT("Collision mesh & Collision optimized datas"),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::EM_Construct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COLLISION_MESH *pCM;
	pCM = static_cast<COLLISION_MESH *>(pO);

	QDT_NEW_PLACED(pCM, COLLISION_MESH)();
	QDT_ASSERT(pCM);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::EM_Destruct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COLLISION_MESH *pCM;
	pCM = static_cast<COLLISION_MESH *>(pO);

	QDT_DELETE_PLACED(pCM, COLLISION_MESH);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("COLMESH", 1 ,CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("COLMESH", 2 ,CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("COLMESH", 3 ,CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("COLMESH", 4 ,CM_ReadChunk4);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("COLMESH", 4 ,CM_WriteChunk4);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SetNbFaces
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MESH::SetNbSteps(int StepsX, int StepsY)
{
	if(_pSectors)
		delete[] _pSectors;

	_pSectors = new CM_SECTOR*[ StepsX*StepsY ];
	_StepsX = StepsX;
	_StepsY = StepsY;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbFaces
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MESH::SetNbFaces(int nbFaces)
{
	if(_pFaces)
		delete[] _pFaces;

	if(nbFaces)
	{
		_pFaces = new CM_FACE[nbFaces];
		_nbFaces = nbFaces;
	}
	else
	{
		_pFaces = 0;
		_nbFaces = nbFaces;
	}
}


//-----------------------------------------------------------------------------
//	Name:		SetNbVertices
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MESH::SetNbVertices(int nbVertices)
{
	if(_pVertices)
		delete[] _pVertices;

	_pVertices = new POINT2D[nbVertices];
	_nbVertices = nbVertices;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbVertices
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MESH::SetNbFaceVtx(int nbFVtx)
{
	if(_pFVtx)
		delete[] _pFVtx;

	if(nbFVtx)
	{
		_pFVtx	= new VECTOR[nbFVtx];
		_nbFVtx	= nbFVtx;
	}
	else
	{
		_pFVtx	= 0;
		_nbFVtx	= nbFVtx;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetNbEdges
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MESH::SetNbEdges(int nbEdges)
{
	if(_pEdges)
		delete[] _pEdges;

	if(_pEdgesNormals)
		delete[] _pEdgesNormals;

	_pEdges = new CM_EDGE[nbEdges];
	_nbEdges = nbEdges;

	_pEdgesNormals = new POINT2D[nbEdges];
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::CM_ReadChunk1(	IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	COLLISION_MESH	*pCM = static_cast<COLLISION_MESH *>(pCommunicator);
	int				Temp;
	float			fTemp;
	int				iter;
	int				Idx1, Idx2, Idx3;

	pCM->_nVersion = 1;

	QDT_Message(QDT_COMMENT("WARNING: Loading a deprecated version of a Collision Mesh"));

	Stream >> fTemp;	pCM->SetRadius(fTemp);
	Stream >> Temp;		pCM->SetNbEdges( Temp );
	Stream >> Temp;		pCM->SetNbFaces( Temp );
	Stream >> Temp;		pCM->SetNbVertices( Temp );

	CM_EDGE	*pEdges = pCM->GetEdges();
	for(iter=0; iter< pCM->_nbEdges; iter++)
	{
		Stream >> Idx1;	Stream >> Idx2;
		pEdges[iter].SetIndexes(Idx1, Idx2);
	}

	CM_FACE *pFaces = pCM->GetFaces();
	for(iter=0; iter < pCM->_nbFaces; iter++)
	{
		Stream >> Idx1; Stream >> Idx2; Stream >> Idx3;
		pFaces[iter].SetIdx(Idx1, Idx2, Idx3);
	}

	POINT2D *pVertices = pCM->GetVertices();
	for(iter=0; iter < pCM->_nbVertices; iter++)
	{
		VECTOR	Convert;
		Stream >> Convert;
		pVertices[iter].SetXY( Convert.GetX(), Convert.GetZ() );
	}

	POINT2D *pEdgesNormals = pCM->GetEdgesNormals();
	for(iter=0; iter < pCM->_nbEdges; iter++)
	{
		Stream >> pEdgesNormals[iter];
	}

	POINT2D	Tmp2D;
	int		StepsX, StepsY;

	Stream >> fTemp;		pCM->SetStepSize( fTemp );
	Stream >> Tmp2D;		pCM->SetOrigin( Tmp2D );
	Stream >> Tmp2D;		pCM->SetMax( Tmp2D );
	Stream >> StepsX;
	Stream >> StepsY;		pCM->SetNbSteps(StepsX, StepsY);

	for(iter=0; iter<StepsX*StepsY; iter++)
	{
		int NbElements;

		Stream >> NbElements;
		if(NbElements)
		{
			CM_SECTOR	*pSector = new CM_SECTOR;
			int			idx;
			int			nX = iter % StepsX;
			int 		nY = iter / StepsX;

			pSector->SetOrigin( pCM->_Origin.GetX() + nX*fTemp, pCM->_Origin.GetY() + nY*fTemp);
			pSector->SetMax( pCM->_Origin.GetX() + (nX+1)*fTemp, pCM->_Origin.GetY() + (nY+1)*fTemp);
			pSector->SetNbElements(NbElements);

			for( idx=0; idx<NbElements; idx++ )
			{
				int TmpVal;
				Stream >> TmpVal;
				pSector->SetValue(idx, TmpVal);
			}

			pCM->_pSectors[iter] = pSector;
		}
		else
		{
			pCM->_pSectors[iter] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::CM_ReadChunk2(	IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	COLLISION_MESH	*pCM = static_cast<COLLISION_MESH *>(pCommunicator);
	int				Temp;
	float			fTemp;
	int				iter;
	int				Idx1, Idx2, Idx3;

	pCM->_nVersion = 2;

	QDT_Message(QDT_COMMENT("WARNING: Loading a deprecated version of a Collision Mesh"));

	Stream >> fTemp;	pCM->SetRadius(fTemp);
	Stream >> Temp;		pCM->SetNbEdges( Temp );
	Stream >> Temp;		pCM->SetNbFaces( Temp );
	Stream >> Temp;		pCM->SetNbVertices( Temp );

	CM_EDGE	*pEdges = pCM->GetEdges();
	for(iter=0; iter< pCM->_nbEdges; iter++)
	{
		Stream >> Idx1;	Stream >> Idx2;
		pEdges[iter].SetIndexes(Idx1, Idx2);
		Stream >> Idx1; Stream >> Idx2;
		pEdges[iter].SetVtxFlags(Idx1, Idx2);
	}

	CM_FACE *pFaces = pCM->GetFaces();
	for(iter=0; iter < pCM->_nbFaces; iter++)
	{
		Stream >> Idx1; Stream >> Idx2; Stream >> Idx3;
		pFaces[iter].SetIdx(Idx1, Idx2, Idx3);
	}

	POINT2D *pVertices = pCM->GetVertices();
	for(iter=0; iter < pCM->_nbVertices; iter++)
	{
		VECTOR	Convert;
		Stream >> Convert;
		pVertices[iter].SetXY( Convert.GetX(), Convert.GetZ() );
	}

	POINT2D *pEdgesNormals = pCM->GetEdgesNormals();
	for(iter=0; iter < pCM->_nbEdges; iter++)
	{
		Stream >> pEdgesNormals[iter];
	}

	POINT2D	Tmp2D;
	int		StepsX, StepsY;

	Stream >> fTemp;		pCM->SetStepSize( fTemp );
	Stream >> Tmp2D;		pCM->SetOrigin( Tmp2D );
	Stream >> Tmp2D;		pCM->SetMax( Tmp2D );
	Stream >> StepsX;
	Stream >> StepsY;		pCM->SetNbSteps(StepsX, StepsY);

	for(iter=0; iter<StepsX*StepsY; iter++)
	{
		int NbElements;

		Stream >> NbElements;
		if(NbElements)
		{
			CM_SECTOR	*pSector = new CM_SECTOR; 
			int			idx;
			int			nX = iter % StepsX;
			int 		nY = iter / StepsX;

			pSector->SetOrigin( pCM->_Origin.GetX() + nX*fTemp, pCM->_Origin.GetY() + nY*fTemp);
			pSector->SetMax( pCM->_Origin.GetX() + (nX+1)*fTemp, pCM->_Origin.GetY() + (nY+1)*fTemp);
			pSector->SetNbElements(NbElements);

			for( idx=0; idx<NbElements; idx++ )
			{
				int TmpVal;
				Stream >> TmpVal;
				pSector->SetValue(idx, TmpVal);
			}

			pCM->_pSectors[iter] = pSector;
		}
		else
		{
			pCM->_pSectors[iter] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::CM_ReadChunk3(	IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	COLLISION_MESH	*pCM = static_cast<COLLISION_MESH *>(pCommunicator);
	int				Temp;
	float			fTemp;
	int				iter;
	int				Idx1, Idx2, Idx3;

	pCM->_nVersion = 3;

	QDT_Message(QDT_COMMENT("WARNING: Loading a deprecated version of a Collision Mesh"));

	Stream >> fTemp;	pCM->SetRadius(fTemp);
	Stream >> Temp;		pCM->SetNbEdges( Temp );
	Stream >> Temp;		pCM->SetNbFaces( Temp );
	Stream >> Temp;		pCM->SetNbVertices( Temp );
	Stream >> Temp;		pCM->SetNbFaceVtx( Temp );

	CM_EDGE	*pEdges = pCM->GetEdges();
	for(iter=0; iter< pCM->_nbEdges; iter++)
	{
		Stream >> Idx1;	Stream >> Idx2;
		pEdges[iter].SetIndexes(Idx1, Idx2);
		Stream >> Idx1; Stream >> Idx2;
		pEdges[iter].SetVtxFlags(Idx1, Idx2);
	}

	CM_FACE *pFaces = pCM->GetFaces();
	for(iter=0; iter < pCM->_nbFaces; iter++)
	{
		Stream >> Idx1; Stream >> Idx2; Stream >> Idx3;
		pFaces[iter].SetIdx(Idx1, Idx2, Idx3);
	}

	POINT2D *pVertices = pCM->GetVertices();
	for(iter=0; iter < pCM->_nbVertices; iter++)
	{
		VECTOR	Convert;
		Stream >> Convert;
		pVertices[iter].SetXY( Convert.GetX(), Convert.GetZ() );
	}

	VECTOR *pFVtx = pCM->GetFaceVtx();
	for(iter=0; iter < pCM->_nbFVtx; iter++)
	{
		Stream >> pFVtx[iter];
	}

	POINT2D *pEdgesNormals = pCM->GetEdgesNormals();
	for(iter=0; iter < pCM->_nbEdges; iter++)
	{
		Stream >> pEdgesNormals[iter];
	}

	POINT2D	Tmp2D;
	int		StepsX, StepsY;

	Stream >> fTemp;		pCM->SetStepSize( fTemp );
	Stream >> Tmp2D;		pCM->SetOrigin( Tmp2D );
	Stream >> Tmp2D;		pCM->SetMax( Tmp2D );
	Stream >> StepsX;
	Stream >> StepsY;		pCM->SetNbSteps(StepsX, StepsY);

	for(iter=0; iter<StepsX*StepsY; iter++)
	{
		int NbElements;

		Stream >> NbElements;
		if(NbElements)
		{
			CM_SECTOR	*pSector = new CM_SECTOR; 
			int			idx;
			int			nX = iter % StepsX;
			int 		nY = iter / StepsX;

			pSector->SetOrigin( pCM->_Origin.GetX() + nX*fTemp, pCM->_Origin.GetY() + nY*fTemp);
			pSector->SetMax( pCM->_Origin.GetX() + (nX+1)*fTemp, pCM->_Origin.GetY() + (nY+1)*fTemp);
			pSector->SetNbElements(NbElements);

			for( idx=0; idx<NbElements; idx++ )
			{
				int TmpVal;
				Stream >> TmpVal;
				pSector->SetValue(idx, TmpVal);
			}

			pCM->_pSectors[iter] = pSector;
		}
		else
		{
			pCM->_pSectors[iter] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MESH::CM_ReadChunk4(	IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	COLLISION_MESH	*pCM = static_cast<COLLISION_MESH *>(pCommunicator);
	int				Temp;
	float			fTemp;
	int				iter;
	int				Idx1, Idx2, Idx3;

	pCM->_nVersion = 4;

	int	RepMode;
	Stream >> RepMode;	pCM->SetRepulsionMode( (Y_REPLUSION_MODE)RepMode );

	Stream >> fTemp;	pCM->SetRadius(fTemp);
	Stream >> Temp;		pCM->SetNbEdges( Temp );
	Stream >> Temp;		pCM->SetNbFaces( Temp );
	Stream >> Temp;		pCM->SetNbVertices( Temp );
	Stream >> Temp;		pCM->SetNbFaceVtx( Temp );

	CM_EDGE	*pEdges = pCM->GetEdges();
	for(iter=0; iter< pCM->_nbEdges; iter++)
	{
		Stream >> Idx1;	Stream >> Idx2;
		pEdges[iter].SetIndexes(Idx1, Idx2);
		Stream >> Idx1; Stream >> Idx2;
		pEdges[iter].SetVtxFlags(Idx1, Idx2);
	}

	POINT2D *pVertices = pCM->GetVertices();
	for(iter=0; iter < pCM->_nbVertices; iter++)
	{
		Stream >> pVertices[iter];
	}

	if( pCM->GetRepulsionMode() == YRM_RAYCAST)
	{
		CM_FACE *pFaces = pCM->GetFaces();
		for(iter=0; iter < pCM->_nbFaces; iter++)
		{
			Stream >> Idx1; Stream >> Idx2; Stream >> Idx3;
			pFaces[iter].SetIdx(Idx1, Idx2, Idx3);
		}

		VECTOR *pFVtx = pCM->GetFaceVtx();
		for(iter=0; iter < pCM->_nbFVtx; iter++)
		{
			Stream >> pFVtx[iter];
		}
	}
	else if( pCM->GetRepulsionMode() == YRM_YLOCKED)
	{
		Stream >> fTemp;
		pCM->SetYLock(fTemp);
	}

	POINT2D *pEdgesNormals = pCM->GetEdgesNormals();
	for(iter=0; iter < pCM->_nbEdges; iter++)
	{
		Stream >> pEdgesNormals[iter];
	}

	POINT2D	Tmp2D;
	int		StepsX, StepsY;

	Stream >> fTemp;		pCM->SetStepSize( fTemp );
	Stream >> Tmp2D;		pCM->SetOrigin( Tmp2D );
	Stream >> Tmp2D;		pCM->SetMax( Tmp2D );
	Stream >> StepsX;
	Stream >> StepsY;		pCM->SetNbSteps(StepsX, StepsY);

	for(iter=0; iter<StepsX*StepsY; iter++)
	{
		int	Dummy;
		int NbElements;

		Stream >> Dummy;
		if(Dummy)
		{
			CM_SECTOR	*pSector = new CM_SECTOR; 
			int			idx;
			int			nX = iter % StepsX;
			int 		nY = iter / StepsX;

			pSector->SetOrigin( pCM->_Origin.GetX() + nX*fTemp, pCM->_Origin.GetY() + nY*fTemp);
			pSector->SetMax( pCM->_Origin.GetX() + (nX+1)*fTemp, pCM->_Origin.GetY() + (nY+1)*fTemp);

			Stream >> NbElements;
			pSector->SetNbElements(NbElements);
			for( idx=0; idx<NbElements; idx++ )
			{
				int TmpVal;
				Stream >> TmpVal;
				pSector->SetValue(idx, TmpVal);
			}

			Stream >> NbElements;
			pSector->SetNbFaces(NbElements);
			for( idx=0; idx<NbElements; idx++ )
			{
				int TmpVal;
				Stream >> TmpVal;
				pSector->SetFaceValue(idx, TmpVal);
			}

			pCM->_pSectors[iter] = pSector;
		}
		else
		{
			pCM->_pSectors[iter] = 0;
		}
	}
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_Write
//	Object:		
//	03-08-07:	JPE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	COLLISION_MESH::CM_WriteChunk4(	IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	int				Idx1, Idx2, Idx3;
	int				iter;
	COLLISION_MESH	*pCM;
	pCM = static_cast<COLLISION_MESH *>(pCommunicator);

	Stream << pCM->GetRepulsionMode();

	Stream << pCM->_fRadius;
	Stream << pCM->_nbEdges;
	Stream << pCM->_nbFaces;
	Stream << pCM->_nbVertices;
	Stream << pCM->_nbFVtx;

	CM_EDGE	*pEdges = pCM->GetEdges();
	for(iter=0; iter< pCM->_nbEdges; iter++)
	{
		Idx1 = pEdges[iter].GetIndex(0);
		Idx2 = pEdges[iter].GetIndex(1);
		Stream << Idx1;
		Stream << Idx2;
		Idx1 = pEdges[iter].GetVtxFlag(0);
		Idx2 = pEdges[iter].GetVtxFlag(1);
		Stream << Idx1;
		Stream << Idx2;
	}

	POINT2D *pVertices = pCM->GetVertices();
	for(iter=0; iter < pCM->_nbVertices; iter++)
	{
		Stream << pVertices[iter];
	}

	if( pCM->GetRepulsionMode() == YRM_RAYCAST)
	{
		CM_FACE *pFaces = pCM->GetFaces();
		for(iter=0; iter < pCM->_nbFaces; iter++)
		{
			Idx1 = pFaces[iter].GetIdx(0);
			Idx2 = pFaces[iter].GetIdx(1);
			Idx3 = pFaces[iter].GetIdx(2);
			Stream << Idx1;
			Stream << Idx2;
			Stream << Idx3;
		}

		VECTOR *pFVtx = pCM->GetFaceVtx();
		for(iter=0; iter < pCM->_nbFVtx; iter++)
		{
			Stream << pFVtx[iter];
		}
	}
	else if( pCM->GetRepulsionMode() == YRM_YLOCKED)
	{
		Stream << pCM->GetYLock();
	}

	POINT2D *pEdgesNormals = pCM->GetEdgesNormals();
	for(iter=0; iter < pCM->_nbEdges; iter++)
	{
		Stream << pEdgesNormals[iter];
	}

	Stream << pCM->_StepSize;
	Stream << pCM->_Origin;
	Stream << pCM->_Max;
	Stream << pCM->_StepsX;
	Stream << pCM->_StepsY;

	for(iter=0; iter<pCM->_StepsX*pCM->_StepsY; iter++)
	{
		if(pCM->_pSectors[iter])
		{
			int idx;
			Stream << 0xffffffff;

			Stream << pCM->_pSectors[iter]->GetNbElements();
			for( idx=0; idx<pCM->_pSectors[iter]->GetNbElements(); idx++ )
			{
				Stream << pCM->_pSectors[iter]->GetValue(idx);
			}

			Stream << pCM->_pSectors[iter]->GetNbFaces();
			for( idx=0; idx<pCM->_pSectors[iter]->GetNbFaces(); idx++ )
			{
				Stream << pCM->_pSectors[iter]->GetFaceValue(idx);
			}
		}
		else
		{
			Stream << (int)0x00;
		}
	}
}
#endif
