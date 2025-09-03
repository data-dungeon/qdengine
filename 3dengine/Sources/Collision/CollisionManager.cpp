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
//	CLASS:	COLLISION_MANAGER
//
//	03-09-01:	JPE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Collision/CollisionManager)
#include	INCL_3DENGINE(Collision/CollisionEntity)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection2D)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis\GlobalVariable)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(ComplexType/TypeVector)
 
//#define VISUAL_DEBUG_MODE

#ifdef VISUAL_DEBUG_MODE
#include "KernelCore/Sources/Debug/DebugWindow.h"
#endif

#if (defined(PLATFORM_PC) || defined(PLATFORM_XBOX) || defined(PLATFORM_PS2))
#	define QDT_SIGN(exp, bitpos) (((*(int*)&exp)&0x80000000)>>(31-bitpos))
#else
#	define QDT_SIGN(exp, bitpos) ((1<<bitpos)&(exp>0))
#endif

static const float	dBias			= 20.0f;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MANAGER::COLLISION_MANAGER()
:
_pFirstEntity(0),
_pFirstMesh(0),
_IsActive(0)
{
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MANAGER::~COLLISION_MANAGER()
{
}

//-----------------------------------------------------------------------------
//	Name:		ExcludeCameraSectors
//	Object:		
//	04-04-26:	FDE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::ExcludeCameraSectors(AABB *	pAABB)
{
	COLLISION_MESH_CONTAINER	*pScanner		= _pFirstMesh;

	while(pScanner)
	{
		pScanner->GetMesh()->ExcludeCameraSectors(pAABB);
		pScanner = pScanner->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetCameraSectors
//	Object:		
//	04-04-27:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::ResetCameraSectors(void)
{
	COLLISION_MESH_CONTAINER	*pScanner		= _pFirstMesh;

	while(pScanner)
	{
		pScanner->GetMesh()->ResetCameraSectors();
		pScanner = pScanner->GetNext();
	}
}



//-----------------------------------------------------------------------------
//	Name:		CPStaticLoop
//	Object:		
//	04-03-24:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::CPStaticLoop(VECTOR &	Source,
									VECTOR &	CW,
									VECTOR &	CCW,
									int		&	bMerged,
									float		rMinResizeAllowed )
{
	// Find "Target" Mesh
	float						TargetAlt		= 1e33f;
	COLLISION_MESH_CONTAINER	*pTargetMesh	= 0;
	COLLISION_MESH_CONTAINER	*pScanner		= _pFirstMesh;
	VECTOR						SourceCW(Source);
	float						Radius;

	SourceCW -= CW;
	Radius	= SourceCW.Length();

	while(pScanner)
	{
		float	fAlt;
		if(pScanner->GetMesh()->VerticalRayCast(Source, fAlt))
		{
			fAlt = Source.GetY() - fAlt;
			if( (fAlt >= 0) && (fAlt < TargetAlt) )
			{
				TargetAlt	= fAlt;
				pTargetMesh = pScanner;
			}
		}
		pScanner = pScanner->GetNext();
	}

	if(!pTargetMesh)
		return 0;

	COLLISION_MESH	*pMesh		= pTargetMesh->GetMesh();
	int				bLoop		= true;
	int				bCorrected	= false;
	//int				LastEdgeID	= -1;

	pMesh->ClearUsedFlags(Source, Radius);

	do
	{
		switch( pMesh->CorrectPosition(Source, CW, 1.0f, bMerged, rMinResizeAllowed) )
		{
			case COLLISION_MESH::CMRC_CORRECTION_DONE_RESULT_SURE:
				bMerged		= false;
				bLoop		= false;
				bCorrected	= true;
				break;

			case COLLISION_MESH::CMRC_CORRECTION_DONE_RESULT_UNSURE:
				bMerged		= false;
				bLoop		= true;
				bCorrected	= true;
				break;

			case COLLISION_MESH::CMRC_NO_CORRECTION_DONE:
				if(bMerged)
					return 0;
				bLoop	= false;
				break;
		}		

	} while(bLoop);

	pMesh->ClearUsedFlags(Source, Radius);
	do
	{
		switch( pMesh->CorrectPosition(Source, CCW, -1.0f, bMerged, rMinResizeAllowed) )
		{
			case COLLISION_MESH::CMRC_CORRECTION_DONE_RESULT_SURE:
				bLoop		= false;
				bCorrected	= true;
				break;

			case COLLISION_MESH::CMRC_CORRECTION_DONE_RESULT_UNSURE:
				bLoop		= true;
				bCorrected	= true;
				break;

			case COLLISION_MESH::CMRC_NO_CORRECTION_DONE:
				bLoop	= false;
				break;
		}		

	} while(bLoop);

	if(bCorrected)
		return 1;

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		CorrectOnEdge
//	Object:		
//	04-04-19:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::CorrectOnEdge (	POINT2D &	vSource2,
											POINT2D &	vNorm12,
											float &		Source2SqLen,
											float &		SourceDestLen,
											float &		SourceDestSqLen,
											VECTOR &	Source,
											VECTOR &	Dest)
{
	if( SourceDestSqLen > Source2SqLen )
	{
		float	Source2Len	= MATH::Sqrt(Source2SqLen);
		float	fFactor		= SourceDestLen / Source2Len;
		Dest.SetX( Source.GetX() + (vSource2.GetX()*fFactor) );
		Dest.SetZ( Source.GetZ() + (vSource2.GetY()*fFactor) );
	}
	else
	{
		float	dPSource	= vNorm12.Cross(vSource2);
		float	PRSqLen		= SourceDestSqLen - (dPSource*dPSource);
		float	PRLen		= MATH::Sqrt(PRSqLen);
		POINT2D	P( Source.GetX()-(vNorm12.GetY()*dPSource), Source.GetZ()+(vNorm12.GetX()*dPSource) );

		Dest.SetX( P.GetX()+(vNorm12.GetX()*PRLen) );
		Dest.SetZ( P.GetY()+(vNorm12.GetY()*PRLen) );
	}

}

//-----------------------------------------------------------------------------
//	Name:		TestIntersection
//	Object:		
//	04-05-25:	FDE - Created
//-----------------------------------------------------------------------------
bool	COLLISION_MANAGER::TestIntersection(const POINT2D &	Source,
											POINT2D &		Segment,
											const float &	rDist1,
											const float &	rDist3,
											const POINT2D &	vDir1,
											const POINT2D &	vDir3,
											const POINT2D & Center)
{
	static float	rBias	= 1.0f;
    float			fAWdU[2], fADdU[2], fRhs;

    POINT2D kSDir(0.5f * Segment.GetX(), 0.5f * Segment.GetY());
    POINT2D kSCen	= Source + kSDir;
    POINT2D kDiff(kSCen.GetX() - Center.GetX(), kSCen.GetY() - Center.GetY());

    fAWdU[0]		= MATH::Abs(kSDir.Dot(vDir1));
    fADdU[0]		= MATH::Abs(kDiff.Dot(vDir1));
    fRhs			= rDist1 + fAWdU[0];
    if ( fADdU[0] > fRhs )
        return false;

    fAWdU[1]		= MATH::Abs(kSDir.Dot(vDir3));
    fADdU[1]		= MATH::Abs(kDiff.Dot(vDir3));
    fRhs			= rDist3 + fAWdU[1];
    if (fADdU[1] > fRhs)
        return false;

    POINT2D kPerp(Segment.GetY(), -Segment.GetX());
    float fLhs		= MATH::Abs(kPerp.Dot(kDiff));
    float fPart0	= MATH::Abs(kPerp.Dot(vDir1));
    float fPart1	= MATH::Abs(kPerp.Dot(vDir3));
    fRhs			= rDist1 * fPart0 + rDist3 * fPart1;
    return (fLhs - rBias <= fRhs); 
}

//-----------------------------------------------------------------------------
//	Name:		ProcessInside
//	Object:		
//	04-06-11:	FDE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::ProcessInside(OBB &				Obb,
									 VECTOR &			Source,
									 VECTOR &			Dest,
									 float				fSens,
									 BOUNDING_VOLUME *	pBV)
{
	static	float	rBias	= 1.0f;
	OBB				CorrectedObb(Obb);
	VECTOR			Center	= Obb.GetCenter();
	POINT2D			CenterSource(Source.GetX() - Center.GetX(), Source.GetZ() - Center.GetZ());
	POINT2D			Dir1(Obb.GetDirection1().GetX(), Obb.GetDirection1().GetZ());
	POINT2D			Dir2(Obb.GetDirection3().GetX(), Obb.GetDirection3().GetZ());

	float	rDot1 = MATH::Abs(CenterSource.Dot(Dir1));
	float	rDot2 = MATH::Abs(CenterSource.Dot(Dir2));

	if ((rDot1 > Obb.GetDistance1() + dBias) || (rDot2 > Obb.GetDistance3() + dBias))
	{
		return 0;
	}

	if (rDot1 /*/ (Obb.GetDistance1() + dBias)*/ < rDot2 /*/ (Obb.GetDistance3() + dBias)*/)
	{
		CorrectedObb.SetDistance3(rDot2 - rBias - dBias);
	}
	else 
	{
		CorrectedObb.SetDistance1(rDot1 - rBias - dBias);
	}

	return CPDynamicObject(0, Source, Dest, fSens, &CorrectedObb, 1e33, false);
}


//-----------------------------------------------------------------------------
//	Name:		CPDynamicObject
//	Object:		
//	04-04-13:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::CPDynamicObject(COLLISION_ENTITY *	pObj,
									   VECTOR &				Source,
									   VECTOR &				Dest,
									   float 				fSens,
									   BOUNDING_VOLUME		*pBV,
									   float				rMinResizeAllowed,
									   bool					bTryToResize)
{
	//---------------------------------------
	//-- WARNING: Highly optimized code :P --
	//---------------------------------------

	if ((pObj != NULL) && (pObj->IsProcessingCollisionForCamSystem() == false))
	{
		return 0;
	}

	if( !pBV )
	{
		pBV = pObj->GetTransformedBoundingVolume();
	}

	if( pBV )
	{
		// Normalement la Source ne peut pas etre a l'interieur d'un objet quel qu'il soit.

		switch( pBV->GetType() )
		{
			case BOUNDING_VOLUME::TYPE_OBB:
			{
				OBB					*pObb			= (OBB*)pBV;
				static const float	CrossEpsilon	= 0.001f;
				static const float	rConstDist		= 1.0f;
				float				Dx				= pObb->GetDistance1() + dBias;
				float				Dz				= pObb->GetDistance3() + dBias;
				POINT2D				ObbCenter( pObb->GetCenter().GetX(), pObb->GetCenter().GetZ() );
				POINT2D				SourceDest( Dest.GetX()-Source.GetX(), Dest.GetZ()-Source.GetZ() );
				POINT2D				Source2D(Source.GetX(), Source.GetZ());
				POINT2D				Dest2D(Dest.GetX(), Dest.GetZ());

				if (TestIntersection(Source2D, SourceDest, Dx, Dz, POINT2D(pObb->GetDirection1().GetX(), pObb->GetDirection1().GetZ()), POINT2D(pObb->GetDirection3().GetX(), pObb->GetDirection3().GetZ()), ObbCenter) == false)
					return 0;

				POINT2D		ObbVx( pObb->GetDirection1().GetX()*Dx, pObb->GetDirection1().GetZ()*Dx );
				POINT2D		ObbVz( pObb->GetDirection3().GetX()*Dz, pObb->GetDirection3().GetZ()*Dz );
				POINT2D		pt1( ObbCenter.GetX()-ObbVx.GetX()-ObbVz.GetX(), ObbCenter.GetY()-ObbVx.GetY()-ObbVz.GetY() );
				POINT2D		pt2( ObbCenter.GetX()+ObbVx.GetX()-ObbVz.GetX(), ObbCenter.GetY()+ObbVx.GetY()-ObbVz.GetY() );
				POINT2D		pt3( ObbCenter.GetX()+ObbVx.GetX()+ObbVz.GetX(), ObbCenter.GetY()+ObbVx.GetY()+ObbVz.GetY() );
				POINT2D		pt4( ObbCenter.GetX()-ObbVx.GetX()+ObbVz.GetX(), ObbCenter.GetY()-ObbVx.GetY()+ObbVz.GetY() );
				POINT2D		Source1( pt1.GetX()-Source.GetX(),  pt1.GetY()-Source.GetZ() ); // --
				POINT2D		Source2( pt2.GetX()-Source.GetX(),  pt2.GetY()-Source.GetZ() ); // +-
				POINT2D		Source3( pt3.GetX()-Source.GetX(),  pt3.GetY()-Source.GetZ() ); // ++
				POINT2D		Source4( pt4.GetX()-Source.GetX(),  pt4.GetY()-Source.GetZ() ); // -+

				float		SourceDestSqLen = SourceDest.GetSquareLength();
				float		SourceDestLen	= MATH::Sqrt(SourceDestSqLen);
				POINT2D		SourceDestNorm( SourceDest.GetX()/SourceDestLen, SourceDest.GetY()/SourceDestLen );

				float		S1Cross = SourceDestNorm.Cross( Source1 );
				float		S2Cross = SourceDestNorm.Cross( Source2 );
				float		S3Cross = SourceDestNorm.Cross( Source3 );
				float		S4Cross = SourceDestNorm.Cross( Source4 );

				int			Flag =	QDT_SIGN( S1Cross, 0 ) | QDT_SIGN( S2Cross, 1 ) | QDT_SIGN( S3Cross, 2 ) | QDT_SIGN( S4Cross, 3 ) ;

				// Try to reduce vector
				if(bTryToResize)
				{
					POINT2D	BoxIntersect;
					switch(Flag)
					{
						case 0:			// ++++		Out 
						case 5:			// +-+-		FAIL
						case 10:		// -+-+		FAIL
						case 15:		// ----		Out 
							return 0;
							break;

						case 1:			// ---+		14
						case 3:			// --++		14
						case 7:			// -+++		14
							INTERSECTION2D::FindIntersection( Source2D, Dest2D, pt1, pt4, BoxIntersect);
							break;

						case 2:			// --+-		21
						case 6:			// -++-		21
						case 14:		// +++-		21
							INTERSECTION2D::FindIntersection( Source2D, Dest2D, pt2, pt1, BoxIntersect);
							break;

						case 4:			// -+--		32
						case 12:		// ++--		32
						case 13:		// ++-+		32
							INTERSECTION2D::FindIntersection( Source2D, Dest2D, pt3, pt2, BoxIntersect);
							break;

						case 8:			// +---		43
						case 9:			// +--+		43
						case 11:		// +-++		43
							INTERSECTION2D::FindIntersection( Source2D, Dest2D, pt4, pt3, BoxIntersect);
							break;
					}

					POINT2D		SourceBoxIntersect( BoxIntersect.GetX()-Source2D.GetX(),BoxIntersect.GetY()-Source2D.GetY() );

					if( SourceDestNorm.Dot(SourceBoxIntersect) > 0 )
					{
						if(SourceBoxIntersect.GetLength() > rMinResizeAllowed)
						{
							Dest.SetX( BoxIntersect.GetX() );
							Dest.SetZ( BoxIntersect.GetY() );
							return 1;
						}
						else
						{
							SourceDestNorm *= rMinResizeAllowed;
							Dest.SetX( Source.GetX() + SourceDestNorm.GetX() );
							Dest.SetZ( Source.GetZ() + SourceDestNorm.GetY() );
							return CPDynamicObject(pObj, Source, Dest, fSens, pBV, rMinResizeAllowed, false);
						}
					}
				}

				POINT2D		v1Dest	( Dest.GetX()-pt1.GetX(),	Dest.GetZ()-pt1.GetY()	);
				POINT2D		v2Dest	( Dest.GetX()-pt2.GetX(),	Dest.GetZ()-pt2.GetY()	);
				POINT2D		v3Dest	( Dest.GetX()-pt3.GetX(),	Dest.GetZ()-pt3.GetY()	);
				POINT2D		v4Dest	( Dest.GetX()-pt4.GetX(),	Dest.GetZ()-pt4.GetY()	);

				float		Dest1SqLen = v1Dest.GetSquareLength();
				float		Dest3SqLen = v3Dest.GetSquareLength();
				float		Dest4SqLen = v4Dest.GetSquareLength();

				float		Source1SqLen = Source1.GetSquareLength();
				float		Source2SqLen = Source2.GetSquareLength();
				float		Source3SqLen = Source3.GetSquareLength();
				float		Source4SqLen = Source4.GetSquareLength();

				switch(Flag)
				{						// 4321		-+
					case 15:			// ----		Out 
					case 0:				// ++++		Out 
						return 0;
						break;

					case 10:		// -+-+		FAIL
					case 5:			// +-+-		FAIL
						QDT_Message(QDT_COMMENT("OUCH !!! Impossible case (yes yes), please call JPE"));
						return 0;
						break;

					case 1:					// ---+		14
					{
						if( Dest4SqLen < rConstDist )
							return 0;
						POINT2D	v41		( pt1.GetX()-pt4.GetX(),	pt1.GetY()-pt4.GetY()	);
						v41.Normalize();
						//v4Dest	/= MATH::Sqrt(Dest4SqLen);
						//if( v41.Cross( v4Dest ) < CrossEpsilon )
						//	return 0;
						if( v41.Cross( Source4 ) < 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							if (MATH::Abs(S1Cross) < 0.01f)
								return 0;
							CorrectOnEdge(Source1, v41, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						else
						{
							S3Cross /= MATH::Sqrt(Source3SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = 2 - S3Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = 2 - S4Cross;
							if(S3Cross > S4Cross)
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								POINT2D	v43( pt3.GetX()-pt4.GetX(),	pt3.GetY()-pt4.GetY()	);
								v43.Normalize();
								CorrectOnEdge(Source3, v43, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								v41.Inverse();
								CorrectOnEdge(Source4, v41, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 3:					// --++		14
					{
						if( Dest4SqLen < rConstDist )
							return 0;
						POINT2D	v41		( pt1.GetX()-pt4.GetX(),	pt1.GetY()-pt4.GetY()	);
						v41.Normalize();
						//v4Dest	/= MATH::Sqrt(Dest4SqLen);
						//if( v41.Cross( v4Dest ) < CrossEpsilon )
						//	return 0;
						if( v41.Cross( Source4 ) < 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S2Cross /= MATH::Sqrt(Source2SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = -2 - S1Cross;
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = -2 - S2Cross;
							if(S1Cross < S2Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source1, v41, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								POINT2D	v12( pt2.GetX()-pt1.GetX(),	pt2.GetY()-pt1.GetY() );
								v12.Normalize();
								CorrectOnEdge(Source2, v12, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							S3Cross /= MATH::Sqrt(Source3SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = 2 - S3Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = 2 - S4Cross;
							if(S3Cross > S4Cross)
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								POINT2D	v43( pt3.GetX()-pt4.GetX(),	pt3.GetY()-pt4.GetY()	);
								v43.Normalize();
								CorrectOnEdge(Source3, v43, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								v41.Inverse();
								CorrectOnEdge(Source4, v41, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 7:				// -+++		14
					{
						if( Dest4SqLen < rConstDist )
							return 0;
						POINT2D	v41		( pt1.GetX()-pt4.GetX(),	pt1.GetY()-pt4.GetY()	);
						v41.Normalize();
						//v4Dest	/= MATH::Sqrt(Dest4SqLen);
						//if( v41.Cross( v4Dest ) < CrossEpsilon )
						//	return 0;
						if( v41.Cross( Source4 ) < 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S2Cross /= MATH::Sqrt(Source2SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = -2 - S1Cross;
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = -2 - S2Cross;
							if(S1Cross < S2Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source1, v41, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								POINT2D	v12( pt2.GetX()-pt1.GetX(),	pt2.GetY()-pt1.GetY() );
								v12.Normalize();
								CorrectOnEdge(Source2, v12, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
							v41.Inverse();
							CorrectOnEdge(Source4, v41, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						return 1;
						break;
					}


					case 2:			// --+-		21
					{
						if( Dest1SqLen < rConstDist )
							return 0;
						POINT2D	v12		( pt2.GetX()-pt1.GetX(),	pt2.GetY()-pt1.GetY()	);
						v12.Normalize();
						//v1Dest	/= MATH::Sqrt(Dest1SqLen);
						//if( v12.Cross( v1Dest ) < CrossEpsilon )
						//	return 0;
						if( v12.Cross( Source1 ) < 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
							CorrectOnEdge(Source2, v12, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						else
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = 2 - S1Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = 2 - S4Cross;
							if(S1Cross > S4Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								v12.Inverse();
								CorrectOnEdge(Source1, v12, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								POINT2D	v14( pt4.GetX()-pt1.GetX(),	pt4.GetY()-pt1.GetY()	);
								v14.Normalize();
								CorrectOnEdge(Source4, v14, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 6:				// -++-		21
					{
						if( Dest1SqLen < rConstDist )
							return 0;
						POINT2D	v12		( pt2.GetX()-pt1.GetX(),	pt2.GetY()-pt1.GetY()	);
						v12.Normalize();
						//v1Dest	/= MATH::Sqrt(Dest1SqLen);
						//if( v12.Cross( v1Dest ) < CrossEpsilon )
						//	return 0;
						if( v12.Cross( Source1 ) < 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S2Cross /= MATH::Sqrt(Source2SqLen);
							S3Cross /= MATH::Sqrt(Source3SqLen);
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = -2 - S2Cross;
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = -2 - S3Cross;
							if(S2Cross < S3Cross)
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source2, v12, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								POINT2D	v23( pt3.GetX()-pt2.GetX(),	pt3.GetY()-pt2.GetY()	);
								v23.Normalize();
								CorrectOnEdge(Source3, v23, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = 2 - S1Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = 2 - S4Cross;
							if(S1Cross > S4Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								v12.Inverse();
								CorrectOnEdge(Source1, v12, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								POINT2D	v14( pt4.GetX()-pt1.GetX(),	pt4.GetY()-pt1.GetY()	);
								v14.Normalize();
								CorrectOnEdge(Source4, v14, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 14:				// +++-		21
					{
						if( Dest1SqLen < rConstDist )
							return 0;
						POINT2D	v12		( pt2.GetX()-pt1.GetX(),	pt2.GetY()-pt1.GetY()	);
						v12.Normalize();
						//v1Dest	/= MATH::Sqrt(Dest1SqLen);
						//if( v12.Cross( v1Dest ) < CrossEpsilon )
						//	return 0;
						if( v12.Cross( Source1 ) < 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S2Cross /= MATH::Sqrt(Source2SqLen);
							S3Cross /= MATH::Sqrt(Source3SqLen);
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = -2 - S2Cross;
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = -2 - S3Cross;
							if(S2Cross < S3Cross)
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source2, v12, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								POINT2D	v23( pt3.GetX()-pt2.GetX(),	pt3.GetY()-pt2.GetY()	);
								v23.Normalize();
								CorrectOnEdge(Source3, v23, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
							v12.Inverse();
							CorrectOnEdge(Source1, v12, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						return 1;
						break;
					}
					
					case 4:			// -+--		32
					{
						if( Dest3SqLen < rConstDist )
							return 0;
						POINT2D	v32		( pt2.GetX()-pt3.GetX(),	pt2.GetY()-pt3.GetY()	);
						v32.Normalize();
						//v3Dest	/= MATH::Sqrt(Dest3SqLen);
						//if( v32.Cross( v3Dest ) > -CrossEpsilon )
						//	return 0;
						if( v32.Cross( Source3 ) > 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
							v32.Inverse();
							CorrectOnEdge(Source3, v32, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						else
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S2Cross /= MATH::Sqrt(Source2SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = 2 - S1Cross;
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = 2 - S2Cross;
							if(S1Cross > S2Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								POINT2D	v21( pt1.GetX()-pt2.GetX(),	pt1.GetY()-pt2.GetY()	);
								v21.Normalize();
								CorrectOnEdge(Source1, v21, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source2, v32, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 12:				// ++--		32
					{
						if( Dest3SqLen < rConstDist )
							return 0;
						POINT2D	v32		( pt2.GetX()-pt3.GetX(),	pt2.GetY()-pt3.GetY()	);
						v32.Normalize();
						//v3Dest	/= MATH::Sqrt(Dest3SqLen);
						//if( v32.Cross( v3Dest ) > -CrossEpsilon )
						//	return 0;
						if( v32.Cross( Source3 ) > 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S3Cross /= MATH::Sqrt(Source3SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = -2 - S3Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = -2 - S4Cross;
							if(S3Cross < S4Cross)
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								v32.Inverse();
								CorrectOnEdge(Source3, v32, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								POINT2D	v34( pt4.GetX()-pt3.GetX(),	pt4.GetY()-pt3.GetY()	);
								v34.Normalize();
								CorrectOnEdge(Source4, v34, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S2Cross /= MATH::Sqrt(Source2SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = 2 - S1Cross;
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = 2 - S2Cross;
							if(S1Cross > S2Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								POINT2D	v21( pt1.GetX()-pt2.GetX(),	pt1.GetY()-pt2.GetY()	);
								v21.Normalize();
								CorrectOnEdge(Source1, v21, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source2, v32, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 13:				// ++-+		32
					{
						if( Dest3SqLen < rConstDist )
							return 0;
						POINT2D	v32		( pt2.GetX()-pt3.GetX(),	pt2.GetY()-pt3.GetY()	);
						v32.Normalize();
						//v3Dest	/= MATH::Sqrt(Dest3SqLen);
						//if( v32.Cross( v3Dest ) > -CrossEpsilon )
						//	return 0;
						if( v32.Cross( Source3 ) > 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S3Cross /= MATH::Sqrt(Source3SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = -2 - S3Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = -2 - S4Cross;
							if(S3Cross < S4Cross)
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								v32.Inverse();
								CorrectOnEdge(Source3, v32, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								POINT2D	v34( pt4.GetX()-pt3.GetX(),	pt4.GetY()-pt3.GetY()	);
								v34.Normalize();
								CorrectOnEdge(Source4, v34, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
							CorrectOnEdge(Source2, v32, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						return 1;
						break;
					}

					case 8:				// +---		43
					{
						if( Dest4SqLen < rConstDist )
							return 0;
						POINT2D	v43		( pt3.GetX()-pt4.GetX(),	pt3.GetY()-pt4.GetY()	);
						v43.Normalize();
						//v4Dest	/= MATH::Sqrt(Dest4SqLen);
						//if( v43.Cross( v4Dest ) > -CrossEpsilon )
						//	return 0;
						if( v43.Cross( Source4 ) > 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
							v43.Inverse();
							CorrectOnEdge(Source4, v43, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						else
						{
							S2Cross /= MATH::Sqrt(Source2SqLen);
							S3Cross /= MATH::Sqrt(Source3SqLen);
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = 2 - S2Cross;
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = 2 - S3Cross;
							if(S2Cross > S3Cross)
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								POINT2D	v32( pt2.GetX()-pt3.GetX(),	pt2.GetY()-pt3.GetY()	);
								v32.Normalize();
								CorrectOnEdge(Source2, v32, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source3, v43, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 9:				// +--+		43
					{
						if( Dest4SqLen < rConstDist )
							return 0;
						POINT2D	v43		( pt3.GetX()-pt4.GetX(),	pt3.GetY()-pt4.GetY()	);
						v43.Normalize();
						//v4Dest	/= MATH::Sqrt(Dest4SqLen);
						//if( v43.Cross( v4Dest ) > -CrossEpsilon )
						//	return 0;
						if( v43.Cross( Source4 ) > 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = -2 - S1Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = -2 - S4Cross;
							if(S1Cross < S4Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								POINT2D	v41( pt1.GetX()-pt4.GetX(),	pt1.GetY()-pt4.GetY()	);
								v41.Normalize();
								CorrectOnEdge(Source1, v41, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								v43.Inverse();
								CorrectOnEdge(Source4, v43, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							S2Cross /= MATH::Sqrt(Source2SqLen);
							S3Cross /= MATH::Sqrt(Source3SqLen);
							if(SourceDestNorm.Dot(Source2) < 0)		S2Cross = 2 - S2Cross;
							if(SourceDestNorm.Dot(Source3) < 0)		S3Cross = 2 - S3Cross;
							if(S2Cross > S3Cross)
							{	//S2
								if (MATH::Abs(S2Cross) < 0.01f)
									return 0;
								POINT2D	v32( pt2.GetX()-pt3.GetX(),	pt2.GetY()-pt3.GetY()	);
								v32.Normalize();
								CorrectOnEdge(Source2, v32, Source2SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S3
								if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
								CorrectOnEdge(Source3, v43, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						return 1;
						break;
					}

					case 11:				// +-++		43
					{
						if( Dest4SqLen < rConstDist )
							return 0;
						POINT2D	v43		( pt3.GetX()-pt4.GetX(),	pt3.GetY()-pt4.GetY()	);
						v43.Normalize();
						//v4Dest	/= MATH::Sqrt(Dest4SqLen);
						//if( v43.Cross( v4Dest ) > -CrossEpsilon )
						//	return 0;
						if( v43.Cross( Source4 ) > 0)
							return ProcessInside(*pObb, Source, Dest, fSens,	pBV);

						if(fSens > 0)
						{
							S1Cross /= MATH::Sqrt(Source1SqLen);
							S4Cross /= MATH::Sqrt(Source4SqLen);
							if(SourceDestNorm.Dot(Source1) < 0)		S1Cross = -2 - S1Cross;
							if(SourceDestNorm.Dot(Source4) < 0)		S4Cross = -2 - S4Cross;
							if(S1Cross < S4Cross)
							{	//S1
								if (MATH::Abs(S1Cross) < 0.01f)
									return 0;
								POINT2D	v41( pt1.GetX()-pt4.GetX(),	pt1.GetY()-pt4.GetY()	);
								v41.Normalize();
								CorrectOnEdge(Source1, v41, Source1SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
							else
							{	//S4
								if (MATH::Abs(S4Cross) < 0.01f)
									return 0;
								v43.Inverse();
								CorrectOnEdge(Source4, v43, Source4SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
							}
						}
						else
						{
							if (MATH::Abs(S3Cross) < 0.01f)
									return 0;
							CorrectOnEdge(Source3, v43, Source3SqLen, SourceDestLen, SourceDestSqLen, Source, Dest);
						}
						return 1;
						break;
					}
				}

				break;
			}

			case BOUNDING_VOLUME::TYPE_CYLINDER:
			{
				const float	fExtrude	= 20.0f;
				const float	fRadiusEpsilon	= 0.5f;
				CYLINDER	*pCyl		= ((CYLINDER*)pBV);
				VECTOR		A( pCyl->GetCenter() );
				POINT2D		BA( A.GetX() - Source.GetX(),			A.GetZ() - Source.GetZ()	);
				float		BASqLen		= BA.GetSquareLength();
				float		BALen		= MATH::Sqrt(BASqLen);
				float		rRadius		= pCyl->GetRadius() + fExtrude;

				if( BALen < rRadius)
					break;
				
				POINT2D		BDest(	Dest.GetX() - Source.GetX(),	Dest.GetZ() - Source.GetZ()	);
				float		BDestLen	= BDest.GetLength();
				POINT2D		BPNorm(	BDest.GetX()/BDestLen, BDest.GetY()/BDestLen );
				float		BPLen	= BPNorm.Dot(BA);
				POINT2D		P(false);

				if( BPLen < 0 )
					return 0;

				POINT2D		PA;
				float		PASqLen;
				float		rSq = rRadius*rRadius;
				float		PArcLen;

				if( BPLen > BDestLen)
				{
					POINT2D BP(BPNorm * BPLen);
					P.SetXY( BP.GetX() + Source.GetX(),	BP.GetY() + Source.GetZ() );
					PA.SetXY( A.GetX()-P.GetX(), A.GetZ()-P.GetY() );
					PArcLen = MATH::Sqrt(rSq - PA.GetSquareLength());

					P.SetXY( Dest.GetX(), Dest.GetZ() );
					PA.SetXY( A.GetX()-P.GetX(), A.GetZ()-P.GetY() );
					PASqLen = PA.GetSquareLength();
				}
				else
				{
					POINT2D BP(BPNorm * BPLen);
					P.SetXY( BP.GetX() + Source.GetX(),	BP.GetY() + Source.GetZ() );
					PA.SetXY( A.GetX()-P.GetX(), A.GetZ()-P.GetY() );
					PASqLen = PA.GetSquareLength();
					PArcLen = MATH::Sqrt(rSq - PASqLen);
				}

				if( PASqLen >= (rSq - fRadiusEpsilon) )
					return 0;

				// Need to repulse
				float	d2Sq		= BASqLen - rSq;
				float	BDestSqLen	= BDestLen * BDestLen;

				float	BArcLen = BPLen-PArcLen;
				if( BArcLen >= rMinResizeAllowed )
				{
					BPNorm *= BArcLen;
					Dest.SetX( Source.GetX() + BPNorm.GetX() );
					Dest.SetZ( Source.GetZ() + BPNorm.GetY() );
					return 1;
				}
				else
					BDestLen = rMinResizeAllowed;

				if( d2Sq > BDestSqLen )
					d2Sq = BDestSqLen;

				float	a = (d2Sq - rSq +BASqLen) / (2 * BALen);
				float	h = MATH::Sqrt( d2Sq - (a*a) );

				POINT2D	D( BA.GetX()/BALen, BA.GetY()/BALen );
				D *= a;
				D.SetX( D.GetX() + Source.GetX() );
				D.SetY( D.GetY() + Source.GetZ() );

				float	hd		= h / BALen;
				float	xArg	= hd * ( A.GetZ() - Source.GetZ() );
				float	yArg	= hd * ( A.GetX() - Source.GetX() );
				POINT2D	Temp;

				if(fSens > 0.0f)
				{
					Temp.SetX( (D.GetX() + xArg - Source.GetX()));
					Temp.SetY( (D.GetY() - yArg - Source.GetZ()));
				}
				else
				{
					Temp.SetX( (D.GetX() - xArg - Source.GetX()));
					Temp.SetY( (D.GetY() + yArg - Source.GetZ()));
				}

				Temp.Normalize();
				Temp *= BDestLen;

				Dest.SetX( Source.GetX() + Temp.GetX() );
				Dest.SetZ( Source.GetZ() + Temp.GetY() );

				return 1;
				break;
			}

			default:
				return 0;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		CPDynamicLoop
//	Object:		
//	04-03-24:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::CPDynamicLoop(VECTOR &	Source,
									 VECTOR &	CW,
									 VECTOR &	CCW,
									 int &		bMerged,
									 OBB *		pObb,
									 int		nbObb,
									 int		Flag,
									 float		rMinResizeAllowed)
{
	int nLoops	= 10;		// 10 loops avant de lacher l'affaire
	int	bLoop;
	int	ret = false;

	do 
	{
		bLoop	= false;

		if( Flag & CMCPF_DYNAMIC )
		{
			COLLISION_ENTITY	*pScan = _pFirstEntity;

			while( pScan )
			{
				if( pScan->IsActive() )
				{
					if( pScan->GetFlags() & COLLISION_ENTITY::CET_DYNAMIC )
					{
						if( CPDynamicObject(pScan, Source, CW, 1.0f, 0, rMinResizeAllowed) )
						{
							bLoop	= true;
							bMerged	= false;
							ret		= true;
						}
					}

					if( (!bMerged) && (pScan->GetFlags() & COLLISION_ENTITY::CET_DYNAMIC) )
					{
						if( CPDynamicObject(pScan, Source, CCW, -1.0f, 0, rMinResizeAllowed) )
						{
							bLoop	= true;
							ret		= true;
						}
					}
				}
				pScan = pScan->GetNext();
			}
		}

		if( pObb && (Flag&CMCPF_USER_OBB) )
		{
			int i;
			for(i=0; i<nbObb; i++)
			{
				if( CPDynamicObject(0, Source, CW, 1.0f, &pObb[i], rMinResizeAllowed) )
				{
					bLoop	= true;
					bMerged	= false;
					ret		= true;
				}
				if( !bMerged )
				{
					if( CPDynamicObject(0, Source, CCW, -1.0f, &pObb[i], rMinResizeAllowed) )
					{
						bLoop	= true;
						ret		= true;
					}
				}
			}
		}

		nLoops--;
		if(!nLoops)
			break;

	} while(bLoop);

	return ret;
}



//-----------------------------------------------------------------------------
//	Name:		CorrectPosition
//	Object:		
//	04-03-23:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::CorrectPosition(	VECTOR &	TargetP,
										VECTOR &	Position,
										VECTOR	&	CW,
										VECTOR	&	CCW,
										OBB *		pObb,
										int			nbObb,
										float		rMinResizeAllowed,
										int			nFlag)
{
	int		nContinue;
	int		Guardian	= 10;		// Eh oui, j'en met meme ici des Guard :P
	int		bMerged		= true;
	VECTOR	Target(TargetP);
	POINT2D	PositionTarget( Target.GetX()-Position.GetX(), Target.GetZ()-Position.GetZ() );
	float	fPTLen = PositionTarget.GetSquareLength();

	if(fPTLen < 0.1f)
		return false;

	fPTLen = MATH::Sqrt(fPTLen);
	PositionTarget.SetXY( PositionTarget.GetX()/fPTLen, PositionTarget.GetY()/fPTLen );
	PositionTarget.Normalize();

	if( rMinResizeAllowed > fPTLen )
		rMinResizeAllowed = fPTLen;

#ifdef QDT_DEBUG
	static bool bForceBypass = false;
	if(bForceBypass)
		return false;
#endif

	Target.SetX( Target.GetX()+PositionTarget.GetX() );
	Target.SetZ( Target.GetZ()+PositionTarget.GetY() );

	CW	= Position;
	CCW	= Position;

	do 
	{
		nContinue = 0;

		if( nFlag & CMCPF_STATIC )
			if( CPStaticLoop(Target, CW, CCW, bMerged, rMinResizeAllowed) )
				nContinue++;

		if( CPDynamicLoop(Target, CW, CCW, bMerged, pObb, nbObb, nFlag, rMinResizeAllowed) )
			nContinue++;

		Guardian--;
		if(!Guardian)
		{
			QDT_Warning(QDT_COMMENT("!!!WATCHDOG!!! Can't find an appropriate vector in less than 10 tries, aborting process (ouaf ouaf)"));
			break;
		}
	} while(nContinue >= 2);

	if(!bMerged)
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		IsCollisionEnable
//	Object:		
//	04-03-30:	VPI - Created
//-----------------------------------------------------------------------------
bool	COLLISION_MANAGER::IsCollisionEnable()
{
	return (_IsActive != 0);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EnableProcessing(int OnOff)
{
	//_IsActive = OnOff;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::InsertNode(const COM_INSTANCE &NodeCI)
{
	COLLISION_ENTITY	*pNewEntity = new COLLISION_ENTITY(NodeCI);
	pNewEntity->SetFlags(	COLLISION_ENTITY::CET_STATIC			+
							COLLISION_ENTITY::CET_DYNAMIC			+ 
							COLLISION_ENTITY::CET_DYNAMIC_REPULSION	+
							COLLISION_ENTITY::CET_STATIC_REPULSION	);
	
	if( InsertNode(pNewEntity) )
	{
		delete pNewEntity;
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		InsertNode
//	Object:		
//	04-02-12:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::InsertNode(	const COM_INSTANCE &	NodeCI,
									const COM_INSTANCE &	NodeCIBounding,
									int						bIgnoreValidityTest)
{
	COLLISION_ENTITY	*pNewEntity = new COLLISION_ENTITY(NodeCI);
	pNewEntity->SetFlags(	COLLISION_ENTITY::CET_STATIC			+
							COLLISION_ENTITY::CET_DYNAMIC			+ 
							COLLISION_ENTITY::CET_DYNAMIC_REPULSION	+
							COLLISION_ENTITY::CET_STATIC_REPULSION	);
	
	if( InsertNode(pNewEntity, (COM_INSTANCE*)&NodeCIBounding, bIgnoreValidityTest) )
	{
		delete pNewEntity;
		return -1;
	}
	return 0;
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::InsertNode(	COLLISION_ENTITY	*pEntity, 
									COM_INSTANCE		*pNodeCIBounding,
									int					bIgnoreValidityTest)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;
	NODE				*pNode		= pEntity->GetNode();

	while (Iterator) 
	{
		if( Iterator->GetNode() == pNode )
			return -1;
		Iterator	= Iterator->GetNext();
	}

	pEntity->SetNext( _pFirstEntity );
	_pFirstEntity = pEntity;

	if(!bIgnoreValidityTest)
	{
		IsPositionValid(pNode, true);
	}

	if(pNodeCIBounding)
	{
		pEntity->SetDynamicVolumeNode( *pNodeCIBounding );
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-04-21 - RMA Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::ReplaceNode(COLLISION_ENTITY *pEntity, 
									COLLISION_ENTITY *pNewEntity)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;
	COLLISION_ENTITY	*pPrev		= 0;

	while (Iterator) 
	{
		if( Iterator == pEntity )
		{
			if(pPrev)
				pPrev->SetNext(pNewEntity);
			else
				_pFirstEntity = pNewEntity;

			pNewEntity->SetNext(Iterator->GetNext());
			return;
		}

		pPrev		= Iterator;
		Iterator	= Iterator->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::RemoveNode(const COM_INSTANCE &ComNodeCI)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;
	COLLISION_ENTITY	*pPrev		= 0;

	QDT_ASSERT(ComNodeCI.HasHandle());

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == ComNodeCI )
		{
			if(pPrev)
				pPrev->SetNext(Iterator->GetNext());
			else
				_pFirstEntity = Iterator->GetNext();

			delete Iterator;
			return;
		}
		pPrev		= Iterator;
		Iterator	= Iterator->GetNext();
	}

	//QDT_Warning(QDT_COMMENT("Tried to remove Node %s from Collision manager but it was not registered"), ComNodeCI.GetName().GetBuffer());
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::RemoveNode(COLLISION_ENTITY *pEntity, int bFromDestructor, int bDestructEntity)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;
	COLLISION_ENTITY	*pPrev		= 0;

	while (Iterator) 
	{
		if( Iterator == pEntity )
		{
			if(pPrev)
				pPrev->SetNext(Iterator->GetNext());
			else
				_pFirstEntity = Iterator->GetNext();

			if(bFromDestructor)
			{
				QDT_Warning(QDT_COMMENT("COLLISION_MANAGER::RemoveNode:  Removing entity via it's destructor !"));
			}

			if(bDestructEntity)
			{
				delete Iterator;
			}
			break;
		}
		pPrev		= Iterator;
		Iterator	= Iterator->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::InsertMesh(const COM_INSTANCE &ComMeshCI)
{
	COLLISION_MESH_CONTAINER	*pNewEntity = new COLLISION_MESH_CONTAINER(ComMeshCI);

	if( InsertMesh(pNewEntity) )
	{
		delete pNewEntity;
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::InsertMesh(COLLISION_MESH_CONTAINER	*pColMeshContainer)
{
	COLLISION_MESH_CONTAINER	*Iterator	= _pFirstMesh;

	while (Iterator) 
	{
		if( Iterator->GetMeshNodeCI() == pColMeshContainer->GetMeshNodeCI())
			return -1;
		Iterator	= Iterator->GetNext();
	}

	pColMeshContainer->SetNext(_pFirstMesh);
	_pFirstMesh = pColMeshContainer;
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::RemoveMesh(const COM_INSTANCE &pComMesh)
{
	COLLISION_MESH_CONTAINER	*Iterator	= _pFirstMesh;
	COLLISION_MESH_CONTAINER	*pPrev		= 0;

	while (Iterator) 
	{
		if( Iterator->GetMeshNodeCI() == pComMesh )
		{
			if(pPrev)
				pPrev->SetNext(Iterator->GetNext());
			else
				_pFirstMesh = Iterator->GetNext();

			delete Iterator;
			break;
		}
		pPrev		= Iterator;
		Iterator	= Iterator->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::RemoveMesh(COLLISION_MESH_CONTAINER	*pColMeshContainer)
{
	COLLISION_MESH_CONTAINER	*Iterator	= _pFirstMesh;
	COLLISION_MESH_CONTAINER	*pPrev		= 0;

	while (Iterator) 
	{
		if( Iterator == pColMeshContainer )
		{
			if(pPrev)
				pPrev->SetNext(Iterator->GetNext());
			else
				_pFirstMesh = Iterator->GetNext();

			delete Iterator;
			break;
		}
		pPrev		= Iterator;
		Iterator	= Iterator->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveMesh
//	Object:		
//	04-02-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::RemoveMesh(COLLISION_MESH *pColMesh)
{
	COLLISION_MESH_CONTAINER	*Iterator	= _pFirstMesh;
	COLLISION_MESH_CONTAINER	*pPrev		= 0;

	while (Iterator) 
	{
		if( Iterator->IsLoaded() )
		{
			if( Iterator->GetMesh() == pColMesh )
			{
				if(pPrev)
					pPrev->SetNext(Iterator->GetNext());
				else
					_pFirstMesh = Iterator->GetNext();
				delete Iterator;
				break;
			}
		}
		pPrev		= Iterator;
		Iterator	= Iterator->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::IsPositionValid(NODE *pNode, int bTryToReplace)
{
	VECTOR	NodePos;
	pNode->GetGlobalPosition(NodePos);

	if( IsPositionValid(NodePos, bTryToReplace) )
	{
		return true;
	}
	else
	{
		if(bTryToReplace)
		{
			pNode->SetGlobalPosition( NodePos );
			ObjectHasBeenTeleported(pNode);
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
//	Name:		IsPositionValid
//	Object:		
//	04-05-11:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::IsPositionValid(VECTOR &	Point,
									   int			bTryToReplace)
{
	COLLISION_MESH_CONTAINER	*pContainer = _pFirstMesh;
	POINT2D						Closest;
	float						LastScore = 0.0f;
	int							bFound;
	int							bIsClosestValid = false;

	while(pContainer)
	{
		POINT2D	LocalClosest;
		float	fAlt;

		if(pContainer->GetMesh()->VerticalRayCast(Point, fAlt))
		{
			fAlt = Point.GetY() - fAlt;
			if( (fAlt >= -10.0f) && (fAlt < 100.0f) )
			{
		if( COLLISION_ENTITY::IsPositionValid(pContainer->GetMesh(), LocalClosest, bFound, Point) )
			return true;

		if(bFound && bTryToReplace)
		{
			float Score =	((Point.GetX()-LocalClosest.GetX())*(Point.GetX()-LocalClosest.GetX())) +
							((Point.GetZ()-LocalClosest.GetY())*(Point.GetZ()-LocalClosest.GetY()));

			if(bIsClosestValid)
			{
				if(Score < LastScore)
				{
					LastScore		= Score;
					Closest			= LocalClosest;
				}
			}
			else
			{
				bIsClosestValid	= true;
				LastScore		= Score;
				Closest			= LocalClosest;
			}
		}
			}
		}

		pContainer = pContainer->GetNext();
	}

	if(bIsClosestValid && bTryToReplace)
	{
		Point.SetX( Closest.GetX() );
		Point.SetZ( Closest.GetY() );
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_ENTITY *COLLISION_MANAGER::GetEntity(const NODE *pNode)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNode() == pNode )
			return Iterator;
		Iterator	= Iterator->GetNext();
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::ObjectHasBeenTeleported(NODE *pNode)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNode() == pNode )
		{
			Iterator->UpdateForTeleport();
			break;
		}
		Iterator	= Iterator->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::ObjectHasBeenTeleported(COM_INSTANCE TeleportedNode)
{
	NODE				*pNode		= (NODE*)( TeleportedNode.GetCommunicator() );
	ObjectHasBeenTeleported(pNode);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetActivity(const COM_INSTANCE &NodeCI, int bOnOff)
{
	SetActivity(static_cast<NODE*>(NodeCI.GetCommunicator()), bOnOff);
}

//-----------------------------------------------------------------------------
//	Name:		GetActivity
//	Object:		
//	04-03-10:	JPE - Created
//-----------------------------------------------------------------------------
bool	COLLISION_MANAGER::GetActivity(const COM_INSTANCE &	NodeCI)
{
	return GetActivity(static_cast<NODE*>(NodeCI.GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		GetActivity
//	Object:		
//	04-03-10:	JPE - Created
//-----------------------------------------------------------------------------
bool	COLLISION_MANAGER::GetActivity(NODE *	pNode)
{
	COLLISION_ENTITY	*pEntity = GetEntity(pNode);

	if(pEntity)
	{
		if(pEntity->IsActive())
			return true;
		else
			return false;
	}
	return false;
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetActivity(NODE *pNode, int bOnOff)
{
	COLLISION_ENTITY	*pEntity = GetEntity(pNode);

	if(pEntity)
	{
		if(bOnOff)
		{
			pEntity->ForceProcessOccured();
			pEntity->PostCollide();
			pEntity->SetActivity(true);
			IsPositionValid(pNode, true);
		}
		else
		{
			pEntity->SetActivity(false);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableNodeOnGround
//	Object:		
//	04-04-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::EnableNodeOnGround(NODE *	pNode,
											  int		bOnOff)
{
	COLLISION_ENTITY	*pEntity = GetEntity(pNode);

	if(pEntity)
	{
		pEntity->SetNodeAsGroundPoint( bOnOff );
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableNodeOnGround
//	Object:		
//	04-04-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::EnableNodeOnGround(const COM_INSTANCE &	NodeCI,
											  int					bOnOff)
{
	EnableNodeOnGround(static_cast<NODE*>(NodeCI.GetCommunicator()), bOnOff);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetGravityProcessing(NODE *pNode, int bOnOff)
{
	COLLISION_ENTITY	*pEntity = GetEntity(pNode);

	if(pEntity)
	{
		pEntity->SetGravityProcessing(bOnOff);
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetGravityProcessing(const COM_INSTANCE &NodeCI, int bOnOff)
{
	SetGravityProcessing(static_cast<NODE*>(NodeCI.GetCommunicator()), bOnOff);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::Loop(void)
{
	if(!_IsActive)
	{
		COLLISION_ENTITY	*pEntity = _pFirstEntity;

		while (pEntity)
		{
			if(pEntity->IsLoaded())
			{
				pEntity->PrepareProcessing();
				pEntity->ForceProcessOccured();
				pEntity->PostCollide();
			}
			pEntity = pEntity->GetNext();
		}
		return;
	}

	COLLISION_ENTITY	*pEntity = _pFirstEntity;
	while (pEntity)
	{
		COLLISION_ENTITY	*pNext = pEntity->GetNext();
		if( pEntity->IsActive() )
		{
			if( !pEntity->IsLoaded() )
			{
				QDT_Warning("Node not loaded detected in collision system. Removing from list. This may cause a LEAK !!!!");
				RemoveNode(pEntity, false, false);
				pEntity = pNext;
				continue;
			}
		}
		pEntity->GetNode()->SetStaticCollide(false);
		pEntity->ResetSliding();
		pEntity = pNext;
	}

	// 1st pass : Ensure everything is in valid space
	StaticLoop();

	// 2nd pass : Dynamic objects loop  /!\ Can retrigger some static computations
	DynamicLoop();

	// Process the pending wall notifications
	pEntity	= _pFirstEntity;
	while (pEntity)
	{
		pEntity->PostCollide();
		if( pEntity->IsStaticTouched() )
			pEntity->GenerateStaticColEvent();
		else
			pEntity->ResetStaticColEvent();
		pEntity = pEntity->GetNext();
	}

	// Process the pending dynamic notifications
	int i;
	for(i=0; i<_nbEvents; i++)
	{
		_FirstCollider[i]->GenerateDynamicColEvent( _CollideWith[i] );
		_CollideWith[i]->GenerateDynamicColEvent( _FirstCollider[i] );
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::ReleaseLists(void)
{
	// Entity list
	COLLISION_ENTITY *pEntityScan = _pFirstEntity;
	_pFirstEntity = 0;
	while(pEntityScan)
	{
		COLLISION_ENTITY *pScheduledToDie = pEntityScan->GetNext();
		delete pEntityScan;
		pEntityScan = pScheduledToDie;
	}

	// Meshes list
	COLLISION_MESH_CONTAINER	*pMeshScan = _pFirstMesh;
	_pFirstMesh = 0;
	while(pMeshScan)
	{
		COLLISION_MESH_CONTAINER	*pNextMesh = pMeshScan->GetNext();
		delete pMeshScan;
		pMeshScan = pNextMesh;
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::DynamicLoop(void)
{
	COLLISION_ENTITY	*pEntity	= _pFirstEntity;
	int					nMaxLoops	= 10;
	int					EntityID;
	int					bLoop;

	_nbEvents = 0;

	do 
	{
		bLoop		= false;
		pEntity		= _pFirstEntity;
		EntityID	= 0;

		while (pEntity)
		{
			if(pEntity->IsActive())
			{
				if( pEntity->GetFlags() & COLLISION_ENTITY::CET_DYNAMIC )
				{
					pEntity->PrepareProcessing();
					int CollidedID = ProcessDynamicEntity( pEntity, EntityID );
					if(CollidedID != -1)
					{ 
						bLoop = true;
					}
				}
			}

			EntityID++;
			pEntity = pEntity->GetNext();
		}
		nMaxLoops--;
	} while(bLoop && nMaxLoops);

	pEntity	= _pFirstEntity;
	while (pEntity)
	{
		if( pEntity->GetFlags() & COLLISION_ENTITY::CET_DYNAMIC )
			pEntity->PostCollide();
		pEntity = pEntity->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::StaticLoop(void)
{
	// Iterate on each registered Node :
	COLLISION_ENTITY	*pEntity = _pFirstEntity;

	while (pEntity)
	{
		if( pEntity->IsActive() )
		{
			pEntity->ResetStaticTouchedState();
			if( pEntity->GetFlags() & COLLISION_ENTITY::CET_STATIC )
			{
				ProcessEntity( pEntity );
			}
		}
		pEntity = pEntity->GetNext();
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-12-02:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::ProcessDynamicEntity(COLLISION_ENTITY *pEntity, int EntityID)
{
	COLLISION_ENTITY	*pIterEntity	= pEntity->GetNext();
	int					ret				= -1;
	NODE				*pEntityNode	= (NODE *)pEntity->GetNodeCI().GetCommunicator();
	COM_INSTANCE		SceneGraphCI	= pEntityNode->GetSceneGraphNodeInst();

	if( !pEntity->IsAttachedToSceneGraph() )
		return ret;

	while (pIterEntity)
	{
		if(pIterEntity->IsActive())
		{
			NODE			*pIterEntityNode	= (NODE *)pIterEntity->GetNodeCI().GetCommunicator();

			if( pIterEntity->IsAttachedToSceneGraph() )
			{
				if( pIterEntity->GetFlags() & COLLISION_ENTITY::CET_DYNAMIC )
				{
					int			nMaxResolutionSteps = 5;
					bool		bLoop = false;
					POINT2D		SafePosition;
					
					pEntity->GetLastPos(SafePosition);
					pIterEntity->PrepareProcessing();

					do 
					{
						int DynamicCollideLevel = pEntity->DynamicCollide(pIterEntity);

						if( DynamicCollideLevel == 0 )
						{
							bLoop = false;
							break;
						}

						if( DynamicCollideLevel < 0 )
						{
							return -1;
						}
						if( !bLoop )
						{
							int		iEvent;
							for(iEvent=0; iEvent<_nbEvents; iEvent++)
							{
								if( ((_FirstCollider[iEvent] == pEntity) && (_CollideWith[iEvent] == pIterEntity)) ||
									((_FirstCollider[iEvent] == pIterEntity) && (_CollideWith[iEvent] == pEntity)) )
								{
									iEvent = -1;
									break;
								}
							}
							if(iEvent != -1)
							{
								_FirstCollider[_nbEvents] = pEntity;
								_CollideWith[_nbEvents]	= pIterEntity;
								_nbEvents++;
								if(_nbEvents>NB_MAX_SIMULTANEOUS_DYNAMIC_COLS)
								{
									QDT_Warning("Too many simultaneous dynamic collisions !!");
									_nbEvents = 0;
								}
							}
						}
						if( DynamicCollideLevel > 1 )
						{
							int WeightScore = pEntity->GetWeight() - pIterEntity->GetWeight();

							bLoop = true;

							if( WeightScore < 0)
							{
								if( !ProcessEntity(pEntity) )
									bLoop = false;
							}
							else if( WeightScore == 0)
							{
								if( (!ProcessEntity(pEntity)) && (!ProcessEntity(pIterEntity)) )
									bLoop = false;
							}
							else //if( WeightScore > 0)
							{
								if( !ProcessEntity(pIterEntity) )
									bLoop = false;
							}
						}
						nMaxResolutionSteps--;

					} while( bLoop&&nMaxResolutionSteps );

					if(bLoop)
					{
						//pEntity->ForcePosition(SafePosition);
					}
				}
			}
		}
		pIterEntity = pIterEntity->GetNext();
	}

	return ret;
}

//-----------------------------------------------------------------------------
//	Name:		ShallIProcess
//	Object:		
//	04-02-23:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_MANAGER::ShallIProcess(COLLISION_MESH_CONTAINER *	pContainer)
{
	if( !pContainer->GetMeshNodeCI().IsLoaded() )
	{
		RemoveMesh(pContainer);
		return false;
	}

	NODE	*pCMNode = (NODE *)pContainer->GetMeshNodeCI().GetCommunicator();
	NODE	*pParentNode = pCMNode->GetParentNode();

	if( pParentNode )
	{
		pParentNode = pParentNode->GetParentNode();

		if( pParentNode )
		{
			if( pParentNode->GetEntityType() == ENTITY_SCENE_GRAPH )
			{
				return true;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::GetGroundAltitude(VECTOR &SourcePos, float &fAltitude)
{
	COLLISION_MESH_CONTAINER	*pContainer			= _pFirstMesh;
	float						MaxAltitude			= -1e33f;
	int							bValid				= false;
	
	while(pContainer)
	{
		COLLISION_MESH_CONTAINER *pNext = pContainer->GetNext();
		if( ShallIProcess(pContainer) )
		{			
			float			LocalAlt;
			COLLISION_MESH	*pTarget	= pContainer->GetMesh();

			if( COLLISION_ENTITY::GetGroundAltitude( pTarget, SourcePos, LocalAlt, pContainer->GetMeshNodeCI() ) )
			{
				float Differential = SourcePos.GetY() - LocalAlt;
				if( Differential > 0 )
				{
					if(LocalAlt > MaxAltitude)
					{
						MaxAltitude = LocalAlt;
						bValid		= true;
					}
				}
			}
		}
		pContainer = pNext;
	}

	if(bValid)
	{
		fAltitude = MaxAltitude;
		return true;
	}
	
	fAltitude = SourcePos.GetY();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeYLockOnGround
//	Object:		
//	04-12-21:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::GetNodeYLockOnGround(COLLISION_ENTITY *pEntity, float &fAltitude)
{
	int							iGPoint;
	int							nbPoints;
	COLLISION_MESH				*pLastMesh		= pEntity->GetLastPosColMesh();
	COLLISION_MESH_CONTAINER	*pTheOne		= 0;
	int							bGlobalValid	= false;
	VECTOR						*GPoints = pEntity->GetGroundPoints(nbPoints);
	VECTOR						NodePos;

	fAltitude = -1e33f;

	pEntity->FindSGCI();
	pEntity->GetNode()->GetGlobalPosition(NodePos);

	// Initialiseur du for() ... 
	if( pEntity->IsNodeActivatedAsGroundPoint() )
		iGPoint = -1;
	else
		iGPoint = 0;

	for(; iGPoint<nbPoints; ++iGPoint)
	{
		COLLISION_MESH_CONTAINER	*pContainer			= _pFirstMesh;
		COLLISION_MESH_CONTAINER	*pDesiredColMesh	= 0;
		int							CurrentPri			= -2;		// can't be lower
		float						MaxAltitude			= -1e33f;
		int							bValid				= false;
		
		while(pContainer)
		{
			COLLISION_MESH_CONTAINER *pNext = pContainer->GetNext();
			if( ShallIProcess(pContainer) )
			{			
				float			LocalAlt;
				COLLISION_MESH	*pTarget	= pContainer->GetMesh();
				int				Priority	= pEntity->VerticalProcess( pTarget, LocalAlt, iGPoint, pContainer->GetMeshNodeCI() );

				if( Priority )
				{
					if(!bValid)
					{
						bValid		= true;
						MaxAltitude	= LocalAlt;
						CurrentPri	= Priority;
						pDesiredColMesh	= pContainer;
					}
					else if( (LocalAlt > MaxAltitude) && (Priority >= CurrentPri) )
					{
						CurrentPri	= Priority;
						MaxAltitude = LocalAlt;
						pDesiredColMesh	= pContainer;
					}
				}
			}
			pContainer = pNext;
		}

		if(bValid)
		{
			if( pDesiredColMesh->GetMesh() == pLastMesh)
			{
				bGlobalValid	= true;
				pTheOne			= pDesiredColMesh;
				if( MaxAltitude > fAltitude )
				{
					fAltitude	= MaxAltitude;
				}
			}
			else if( MaxAltitude > fAltitude )
			{
				bGlobalValid	= true;
				fAltitude	= MaxAltitude;
				if( !pTheOne )
				{
					pTheOne = pDesiredColMesh;
				}
				else if( (pTheOne->GetMesh()) != pLastMesh )
				{
					pTheOne = pDesiredColMesh;
				}
			}
		}
		else if(pDesiredColMesh)
		{
			if( (!pTheOne) || (pDesiredColMesh->GetMesh() == pLastMesh) )
			{
				pTheOne = pDesiredColMesh;
			}
		}
	}

	pEntity->ReleaseZoneCI();

	if(bGlobalValid)
	{
		if(pEntity->_YSpeed > 0.01f)
		{
			fAltitude+=pEntity->_YSpeed;
		}
		else
			fAltitude+=pEntity->_YAccel;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeYLockOnGround
//	Object:		
//	04-12-21:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::GetNodeYLockOnGround(const COM_INSTANCE &NodeCI, float &fAltitude)
{
	COLLISION_ENTITY *pEntity = GetEntity( static_cast<NODE*>(NodeCI.GetCommunicator()) );
	if(pEntity)
		return GetNodeYLockOnGround(pEntity, fAltitude);
	return 0;
}

//#define ENFORCE_DEBUG
//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::ProcessEntity(COLLISION_ENTITY *pEntity)
{
	COLLISION_MESH	*pLastMesh		= pEntity->GetLastPosColMesh();
	int				nReturn			= 0;

	// TODO: Test si la gravité est activée

	// XZ Repulsion, based on the last "walked" mesh
	if( pLastMesh )
	{
		int nbLoops			= 4;
		int	bProcessMain	= true;
		int	bProcessSub		= true;

#ifdef ENFORCE_DEBUG
		int	ForceReloop;
		do {
		ForceReloop = 10;
#endif

		while( nbLoops && (bProcessSub||bProcessMain) )
		{
			if(bProcessMain)
			{
				if( pEntity->CollideAgainst( pLastMesh ) )
				{
					nReturn		= 1;
					bProcessSub = true;
				}
				bProcessMain = false;
			}

			if(bProcessSub)
			{
				if( pEntity->ProcessSubEntity( pLastMesh ) )
				{
					nReturn			= 1;
					bProcessMain	= true;
				}
				bProcessSub = false;
			}

			nbLoops--;
		}
		if(!nbLoops)
		{
			//QDT_Warning("/!\\ Reaching interaction limits between entity & subentity");
			pEntity->UpdateForPanicMode();
#ifdef ENFORCE_DEBUG
			ForceReloop--;
			nbLoops			= 4;
			bProcessMain	= true;
			bProcessSub		= true;
#endif
		}
#ifdef ENFORCE_DEBUG
		else
			ForceReloop		= 0;

		} while(ForceReloop);
#endif

		pEntity->FindSGCI();
		pEntity->GravityApplication();
	}
	else
	{
		pEntity->FindSGCI();
	}


	// Normalement c'est a faire quoiqu'il arrive, meme si la gravité est inactive, just because c'est
	// le truc qui me permet de savoir ou appliquer la repu XZ
	int							iGPoint;
	int							nbPoints;
	COLLISION_MESH_CONTAINER	*pTheOne		= 0;
	float						GlobalMaxAlt	= -1e33f;
	float						LowestAltFound	= 1e33f;
	int							bGlobalValid	= false;
	VECTOR						*GPoints = pEntity->GetGroundPoints(nbPoints);
	VECTOR						NodePos;

	pEntity->GetNode()->GetGlobalPosition(NodePos);

	// Initialiseur du for() ... 
	if( pEntity->IsNodeActivatedAsGroundPoint() )
		iGPoint = -1;
	else
		iGPoint = 0;

	for(; iGPoint<nbPoints; ++iGPoint)
	{
		COLLISION_MESH_CONTAINER	*pContainer			= _pFirstMesh;
		COLLISION_MESH_CONTAINER	*pDesiredColMesh	= 0;
		int							CurrentPri			= -2;		// can't be lower
		float						MaxAltitude			= -1e33f;
		int							bValid				= false;
		
		while(pContainer)
		{
			COLLISION_MESH_CONTAINER *pNext = pContainer->GetNext();
			if( ShallIProcess(pContainer) )
			{			
				float			LocalAlt;
				COLLISION_MESH	*pTarget	= pContainer->GetMesh();
				int				Priority	= pEntity->VerticalProcess( pTarget, LocalAlt, iGPoint, pContainer->GetMeshNodeCI() );

				if( LocalAlt < LowestAltFound )
					LowestAltFound = LocalAlt;

				if( Priority )
				{
					if( Priority < 0 )
					{
						if( !bValid )
						{
							if( (LocalAlt > MaxAltitude) && (Priority >= CurrentPri) )
							{
								CurrentPri	= Priority;
								MaxAltitude = LocalAlt;
								pDesiredColMesh	= pContainer;
							}
						}
					}
					else if(!bValid)
					{
						bValid		= true;
						MaxAltitude	= LocalAlt;
						CurrentPri	= Priority;
						pDesiredColMesh	= pContainer;
					}
					else if( (LocalAlt > MaxAltitude) && (Priority >= CurrentPri) )
					{
						CurrentPri	= Priority;
						MaxAltitude = LocalAlt;
						pDesiredColMesh	= pContainer;
					}
				}
			}
			pContainer = pNext;
		}

		if(bValid)
		{
			if( iGPoint != -1 )
			{
				VECTOR	PtContact(GPoints[iGPoint]);
				PtContact.SetY(MaxAltitude + (GPoints[iGPoint].GetY() - NodePos.GetY()));

				pEntity->SetContactPoint(iGPoint, PtContact );
			}

			if( pDesiredColMesh->GetMesh() == pLastMesh)
			{
				bGlobalValid	= true;
				pTheOne			= pDesiredColMesh;
				if( MaxAltitude > GlobalMaxAlt )
				{
					GlobalMaxAlt	= MaxAltitude;
				}
			}
			else if( MaxAltitude > GlobalMaxAlt )
			{
				bGlobalValid	= true;
				GlobalMaxAlt	= MaxAltitude;
				if( !pTheOne )
				{
					pTheOne = pDesiredColMesh;
				}
				else if( (pTheOne->GetMesh()) != pLastMesh )
				{
					pTheOne = pDesiredColMesh;
				}
			}
		}
		else if(pDesiredColMesh)
		{
			if( (!pTheOne) || (pDesiredColMesh->GetMesh() == pLastMesh) )
			{
				pTheOne = pDesiredColMesh;
			}
		}
	}

	if(pTheOne)
	{
		pEntity->SetLastColMesh( pTheOne->GetMeshCI() );
	}
	pEntity->PostVerticalProcessing(bGlobalValid, GlobalMaxAlt, LowestAltFound);
	pEntity->ReleaseZoneCI();

	return nReturn;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetEntityWeight(const COM_INSTANCE &NodeCI, int nWeight)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == NodeCI )
		{
			Iterator->SetWeight( (COLLISION_ENTITY::ENTITY_WEIGHT)nWeight );
			return;
		}
		Iterator	= Iterator->GetNext();
	}
	//QDT_Error(QDT_COMMENT("There is no such node declared in the COLLISION_MANAGER - Impossible to set Dynamic Weight"));
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::GetEntityWeight(const COM_INSTANCE &NodeCI)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == NodeCI )
		{
			return Iterator->GetWeight();
		}
		Iterator	= Iterator->GetNext();
	}
	//QDT_Error(QDT_COMMENT("There is no such node declared in the COLLISION_MANAGER - Impossible to get Dynamic Weight"));
	return -1;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetEntityCollisionMode(const COM_INSTANCE &NodeCI, int nColMode)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == NodeCI )
		{
			Iterator->SetFlags(nColMode);
			return;
		}
		Iterator	= Iterator->GetNext();
	}

	//QDT_Error(QDT_COMMENT("There is no such node declared in the COLLISION_MANAGER - Impossible to apply Collision Mode"));
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_MANAGER::GetEntityCollisionMode(const COM_INSTANCE &NodeCI)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == NodeCI )
		{
			return Iterator->GetFlags();
			break;
		}
		Iterator	= Iterator->GetNext();
	}
	//QDT_Error(QDT_COMMENT("There is no such node declared in the COLLISION_MANAGER - Impossible to get Collision Mode"));
	return -1;
}


//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MANAGER *COLLISION_MANAGER::Instance()
{
	return	((COLLISION_MANAGER*)GetSingletonDescriptor()->Instance());
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::Release()
{
	GetSingletonDescriptor()->Release();
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	04-01-06:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::SetNotifier(const NODE *pNode, const COLLISION_NOTIFIER *pNotifier)
{
	COLLISION_ENTITY	*pTarget = GetEntity( pNode );

	if(pTarget)
	{
		pTarget->SetNotifier( pNotifier );
	}
	else 
	{
		QDT_Message(QDT_COMMENT("Force registering of a node !"));
		QDT_Message(QDT_COMMENT("Can't add a notifier to node, this node isn't active in the COLLISION_MANAGER"));
	}
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	04-01-06:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::UnsetNotifier(const NODE *pNode)
{
	COLLISION_ENTITY	*pTarget = GetEntity( pNode );
	if(pTarget)
	{
		pTarget->SetNotifier( 0 );
	}
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_SetWeight(void * const pO, void * const pR, void * const pP)
{
	struct PARAMS
	{
		COM_INSTANCE 		_NodeComInstance;
		int					_nWeight;
	};

	PARAMS	*pParams	= static_cast<PARAMS*>(pP);

	COLLISION_MANAGER::Instance()->SetEntityWeight(pParams->_NodeComInstance, pParams->_nWeight);
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_UnRegisterAllDynamicNodes(	void * const pO, 
														void * const pR, 
														void * const pP		)
{
	COLLISION_MANAGER::Instance()->UnRegisterAllDynamicNodes();
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::UnRegisterAllDynamicNodes(void)
{
	// Entity list
	COLLISION_ENTITY *pEntityScan = _pFirstEntity;
	_pFirstEntity = 0;
	while(pEntityScan)
	{
		COLLISION_ENTITY *pScheduledToDie = pEntityScan->GetNext();
		if(pEntityScan->GetFlags() & COLLISION_ENTITY::CET_DYNAMIC)
		{
			if( !(pEntityScan->GetFlags() & COLLISION_ENTITY::CET_STATIC) )
			{
				if(pEntityScan == _pFirstEntity)
					_pFirstEntity = pEntityScan->GetNext();
				delete pEntityScan;
			}
		}
		pEntityScan = pScheduledToDie;
	}
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_UnRegisterAllNodes(	void * const pO, 
												void * const pR, 
												void * const pP		)
{
	COLLISION_MANAGER::Instance()->UnRegisterAllNodes();
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::UnRegisterAllNodes(void)
{
	// Entity list
	COLLISION_ENTITY *pEntityScan = _pFirstEntity;
	_pFirstEntity = 0;
	while(pEntityScan)
	{
		COLLISION_ENTITY *pScheduledToDie = pEntityScan->GetNext();
		delete pEntityScan;
		pEntityScan = pScheduledToDie;
	}
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_GetWeight(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE	*NodeComInstance	= (COM_INSTANCE*)pP;
	int				*pInt				= (int*)pR;

	*pInt = COLLISION_MANAGER::Instance()->GetEntityWeight( *(NodeComInstance) );
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_SetCollisionMode(void * const pO, void * const pR, void * const pP)
{
	struct PARAMS
	{
		COM_INSTANCE 		_NodeComInstance;
		int					_nMode;
	};

	PARAMS	*pParams	= static_cast<PARAMS*>(pP);

	COLLISION_MANAGER::Instance()->SetEntityCollisionMode(pParams->_NodeComInstance, pParams->_nMode);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-12-24:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_GetCollisionMode(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE	*NodeComInstance	= (COM_INSTANCE*)pP;
	int				*pInt				= (int*)pR;

	*pInt = COLLISION_MANAGER::Instance()->GetEntityCollisionMode( *(NodeComInstance) );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNodeAsGroundPoint
//	Object:		
//	04-04-06:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::EM_SetNodeAsGroundPoint(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	struct PARAMS
	{
		COM_INSTANCE 		_NodeComInstance;
		bool				_bOnOff;
	};

	PARAMS	*pParams	= static_cast<PARAMS*>(pP);

	COLLISION_MANAGER::Instance()->EnableNodeOnGround(pParams->_NodeComInstance, pParams->_bOnOff );
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_IsPositionValid(void * const pO, void * const pR, void * const pP)
{
	QDT_Error(QDT_COMMENT("Not implemented !!!"));
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_IsProcessing(void * const pO, void * const pR, void * const pP)
{
	bool *pBool;

	pBool = (bool*) pR;
	*pBool = COLLISION_MANAGER::Instance()->_IsActive;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_EnableProcessing(void * const pO, void * const pR, void * const pP)
{
	bool	OnOff = *(static_cast<bool*>(pP));

	COLLISION_MANAGER::Instance()->EnableProcessing(OnOff);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_RegisterNode(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE 		*pComInst		= static_cast<COM_INSTANCE*>(pP);	// Parameter

	COLLISION_MANAGER::Instance()->InsertNode(*pComInst);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_UnRegisterNode(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE 		*pComInst		= static_cast<COM_INSTANCE*>(pP);	// Parameter

	COLLISION_MANAGER::Instance()->RemoveNode(*pComInst);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_RegisterMesh(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE 		*pComInst		= static_cast<COM_INSTANCE*>(pP);	// Parameter

	COLLISION_MANAGER::Instance()->InsertMesh(*pComInst);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_UnRegisterMesh(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE 		*pComInst		= static_cast<COM_INSTANCE*>(pP);	// Parameter

	COLLISION_MANAGER::Instance()->RemoveMesh(*pComInst);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::COM_Init(CLASS_DESCRIPTOR * const pCD)
{
	METHOD_DESCRIPTOR *	pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetLoopMethod(EM_Loop);

	// Methods
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RegisterNode"), QDT_COMMENT("Registers a node in the collision system"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_REGISTER_NODE, pMD, EM_RegisterNode);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UnRegisterNode"), QDT_COMMENT("UnRegisters a node previously added in the collision system"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_UNREGISTER_NODE, pMD, EM_UnRegisterNode);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RegisterMesh"), QDT_COMMENT("Registers a collision mesh to collide with"));
	pMD->AddParameter(QDT_COMMENT("CollisionMesh"), QDT_COMMENT(""), COLLISION_MESH::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_REGISTER_MESH, pMD, EM_RegisterMesh);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UnRegisterMesh"), QDT_COMMENT("Exclude a mesh from the collision system"));
	pMD->AddParameter(QDT_COMMENT("CollisionMesh"), QDT_COMMENT(""), COLLISION_MESH::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_UNREGISTER_MESH, pMD, EM_UnRegisterMesh);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableProcessing"), QDT_COMMENT("Global control over the collision engine (on/off)"));
	pMD->AddParameter(QDT_COMMENT("OnOff"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_PROCESSING, pMD, EM_EnableProcessing);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsProcessing"), QDT_COMMENT("Returns true if the collision system is activated"));
	METHOD_DESCRIPTOR__SetReturn(pMD,"State", "State of collision manager", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_PROCESSING, pMD, EM_IsProcessing);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetWeight"), QDT_COMMENT("Sets the dynamic repulsion weight for a given node (Use DCW_xxx enums)"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	pMD->AddParameter(QDT_COMMENT("Weight"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_WEIGHT, pMD, EM_SetWeight);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetWeight"), QDT_COMMENT("Returns the dynamic repulsion weight for a given node"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD,"Weight", "Dynamic weight of a given node", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_WEIGHT, pMD, EM_GetWeight);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetCollisionMode"), QDT_COMMENT("Sets the collision mode for a given node (e.g. Dynamic/Static) Can be a combination of any NCM_xxx enums"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	pMD->AddParameter(QDT_COMMENT("CollisionMode"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_COLLISION_MODE, pMD, EM_SetCollisionMode);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetCollisionMode"), QDT_COMMENT("Returns the collision mode for a given node (e.g. Dynamic/Static) Can be a combination of any NCM_xxx enums"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD,"Weight", "Dynamic weight of a given node", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_COLLISION_MODE, pMD, EM_GetCollisionMode);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UnregisterAllNodes"), QDT_COMMENT("Removes ALL the Nodes active in the collisions manager. /!\\ WARNING: it WILL also remove auto declared node via the motion kit !"));
	pCD->AddMethod(CMD_UNREGISTER_ALL_NODES, pMD, EM_UnRegisterAllNodes);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UnregisterAllDynamicOnlyNodes"), QDT_COMMENT("Removes ALL the DYNAMIC ONLY Nodes (i.e. DYNAMIC flag and NOT static flag) active in the collisions manager."));
	pCD->AddMethod(CMD_UNREGISTER_DYNAMIC_ONLY_NODES, pMD, EM_UnRegisterAllDynamicNodes);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetNodeAsGroundPoint"), QDT_COMMENT("Activates the main node like if it was a ground point (it won't generate any events tough)"));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT(""), NODE::GetGenericCommunicatorDescriptor());
	pMD->AddParameter(QDT_COMMENT("OnOff"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_NODE_AS_GROUND_POINT, pMD, EM_SetNodeAsGroundPoint);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("CorrectPosition"), QDT_COMMENT("Checks the validity of a ray through collision engine, and returns two potentials corrected vectors outside any collision trying to preserve vector original length."));
	pMD->AddParameter(QDT_COMMENT("TargetP"), QDT_COMMENT("Position to be corrected"), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Position"), QDT_COMMENT("Fixed position (rotation center)"), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameterPointer(QDT_COMMENT("CW"), QDT_COMMENT("Pointer to vector that will hold the first corrected vector (if applicable) (ClockWise)"), TYPE_VECTOR::GetDescriptor(), 1);
	pMD->AddParameterPointer(QDT_COMMENT("CCW"), QDT_COMMENT("Pointer to vector that will hold the second corrected vector (if applicable) (CounterClockWise)"), TYPE_VECTOR::GetDescriptor(), 1);
	pMD->AddParameter(QDT_COMMENT("Flags"), QDT_COMMENT("Use flags to choose the elements that will be used to perform collisions tests (static/dynamic) (use CPF_xx enum)"), TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pMD,"Correction", "Returns true if a correction has been done", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_CORRECT_POSITION, pMD, EM_CorrectPosition);

#ifndef _MASTER
	pCD->AddEnum("CPF_STATIC",	CMCPF_STATIC,	"Use this to perform CorrectPosition on static entries (collision mesh)");
	pCD->AddEnum("CPF_DYNAMIC",	CMCPF_DYNAMIC,	"Use this to perform CorrectPosition on dynamic entries (characters/script objects/...)");

	pCD->AddEnum("DCW_LIGHT",	COLLISION_ENTITY::EW_LIGHT,	"Light can push only lights entities (50/50 repulsion for same weight objects)");
	pCD->AddEnum("DCW_MEDIUM",	COLLISION_ENTITY::EW_MEDIUM,"Medium can push light & medium entities (50/50 repulsion for same weight objects)");
	pCD->AddEnum("DCW_HEAVY",	COLLISION_ENTITY::EW_HEAVY,	"Heavy can push every entities (50/50 repulsion for same weight objects)");

	pCD->AddEnum("NCM_NONE",				COLLISION_ENTITY::CET_NONE,				"No collision computation (you should exculde the entity frome the COLLISION_MANAGER rather than using this enum)");
	pCD->AddEnum("NCM_DYNAMIC",				COLLISION_ENTITY::CET_DYNAMIC,			"Dynamic computations will be performed (Don't forget the NCM_DYNAMIC_REPULSION if you want the node to be repulsed !)");
	pCD->AddEnum("NCM_STATIC",				COLLISION_ENTITY::CET_STATIC,			"Static computations will be performed (mainly walls) (Don't forget the NCM_STATIC_REPULSION if you want the node to be repulsed !)");
	pCD->AddEnum("NCM_DYNAMIC_REPULSION",	COLLISION_ENTITY::CET_DYNAMIC_REPULSION,"This node will be repulsed against dynamic entities (no effect without NCM_DYNAMIC)");
	pCD->AddEnum("NCM_STATIC_REPULSION",	COLLISION_ENTITY::CET_STATIC_REPULSION,	"This node will be repulsed against static entities (no effect without NCM_STATIC)");
#endif

	//	Now register the class as a global variable
	GLOBAL_VARIABLE::AddVariable(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MANAGER, "COLLISION_MANAGER", pCD, GetSingletonDescriptor()->GetSingletonPointer());
}

//-----------------------------------------------------------------------------
//	Name:		EM_CorrectPosition
//	Object:		
//	04-09-02:	JPE - Created
//-----------------------------------------------------------------------------
#ifdef QDT_DEBUG
static int VisualAI = false;
#endif
void	COLLISION_MANAGER::EM_CorrectPosition(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	struct PARAMS
	{
		VECTOR	_TargetP;
		VECTOR	_Position;
		VECTOR	*_pCW;
		VECTOR	*_pCCW;
		int		_nFlags;
	};

	PARAMS	*pParams	= static_cast<PARAMS*>(pP);
	bool	*pBool		= (bool*)pR;

	VECTOR	AlignedTargetP, AlignedPosition, AlignedCW, AlignedCCW;

	AlignedTargetP.SlowCopy(pParams->_TargetP);
	AlignedPosition.SlowCopy(pParams->_Position);

#ifdef QDT_DEBUG
//	RENDER_CONTEXT *pRC = RENDER_MANAGER::Instance()->GetRenderContext();
//	pRC->SetAlphaBlendDstFactor(RENDER_CONTEXT::BLENDFACTOR_ZERO);
//	pRC->SetAlphaBlendSrcFactor(RENDER_CONTEXT::BLENDFACTOR_ONE);
//	pRC->SetWorldTransform0( 0 );
#endif

	if( COLLISION_MANAGER::Instance()->CorrectPosition(	AlignedPosition, 
														AlignedTargetP,
														AlignedCW,
														AlignedCCW,
														0, 0, 1e33,
														pParams->_nFlags)   )
	{
		*pBool = true;
		pParams->_pCW->SlowCopy(AlignedCW);
		pParams->_pCCW->SlowCopy(AlignedCCW);

#ifdef QDT_DEBUG
		if(VisualAI)
		{
			/*COLOR_FLOAT Red(1.0f, 1.0f, 0.0f, 0.0f);
			COLOR_FLOAT Vert(1.0f, 0.0f, 1.0f, 0.0f);
			COLLISION_MESH::AddLineToDebugDisplay( SEGMENT(pParams->_TargetP, pParams->_Position), Red );
			COLLISION_MESH::AddLineToDebugDisplay( SEGMENT(*pParams->_pCW, pParams->_Position), Vert );
			COLLISION_MESH::AddLineToDebugDisplay( SEGMENT(*pParams->_pCCW, pParams->_Position), Vert );*/
		}
#endif
	}
	else
	{
#ifdef QDT_DEBUG
		if(VisualAI)
		{
			/*COLOR_FLOAT Red(1.0f, 1.0f, 0.0f, 0.0f);
			COLOR_FLOAT Vert(1.0f, 0.0f, 1.0f, 0.0f);
			COLLISION_MESH::AddLineToDebugDisplay( SEGMENT(pParams->_TargetP, pParams->_Position), Red );*/
		}
#endif
		*pBool = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_Construct(	void * const pO, 
										void * const pR, 
										void * const pP		)
{
	COLLISION_MANAGER *	pObject;
	pObject = (COLLISION_MANAGER *) pO;
	QDT_NEW_PLACED(pObject, COLLISION_MANAGER)();

	MODULE_ABC::AddClassDescriptorLoop(MOTEUR_3D_MODULE::COLLISION_MANAGER_LOOP, GetSingletonDescriptor(), NULL);
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_Destruct(	void * const pO, 
										void * const pR, 
										void * const pP		)
{
	COLLISION_MANAGER *	pObject;
	pObject = (COLLISION_MANAGER *) pO;
	
	MODULE_ABC::RemoveClassDescriptorLoop(pO);

	QDT_DELETE_PLACED(pObject, COLLISION_MANAGER);
}


//-----------------------------------------------------------------------------
//	Name:		EM_Loop
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_MANAGER::EM_Loop(	void * const	pO,
									void * const	pR,
									void * const	pP		)
{
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);

	COLLISION_MANAGER::Instance()->Loop();
}	


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *COLLISION_MANAGER::GetSingletonDescriptor()
{
	return (SINGLETON_DESCRIPTOR::FindSingDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MANAGER));
}

//-----------------------------------------------------------------------------
//	Name:		CreateSingletonDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	COLLISION_MANAGER::CreateSingletonDescriptor()
{
	return (new SINGLETON_DESCRIPTOR(	QDT_COMMENT("COLLISION_MANAGER"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MANAGER,
										QDT_COMMENT("664BF313-FA71-457c-B62F-355AC7B4A983"),
										sizeof(COLLISION_MANAGER), 
										COM_Init,
										QDT_COMMENT("The collision Manager Singleton")));
}

//-----------------------------------------------------------------------------
//	Name:		SetEntityDebugInfo
//	Object:		
//	04-03-23:	DAC - Created
//-----------------------------------------------------------------------------
void	COLLISION_MANAGER::SetEntityDebugInfo(const COM_INSTANCE &	NodeCI,
											  const QDT_STRING &	sDebugInfo)
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == NodeCI )
		{
			Iterator->SetDebugInfo(sDebugInfo);
			return;
		}
		Iterator	= Iterator->GetNext();
	}
	QDT_Error(QDT_COMMENT("There is no such node declared in the COLLISION_MANAGER - Impossible to set DebugInfo"));
}

//-----------------------------------------------------------------------------
//	Name:		GetEntityDebugInfo
//	Object:		
//	04-03-23:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	COLLISION_MANAGER::GetEntityDebugInfo(const COM_INSTANCE &	NodeCI) const
{
	COLLISION_ENTITY	*Iterator	= _pFirstEntity;

	while (Iterator) 
	{
		if( Iterator->GetNodeCI() == NodeCI )
		{
			return Iterator->GetDebugInfo();
		}
		Iterator	= Iterator->GetNext();
	}
	QDT_Error(QDT_COMMENT("There is no such node declared in the COLLISION_MANAGER - Impossible to get debug info"));
	return (QDT_STRING::CONST_EMPTY_STRING);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================

