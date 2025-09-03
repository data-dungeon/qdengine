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

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_3DENGINE(Collision/CollisionEntity)
#include	INCL_3DENGINE(Collision/CollisionManager)
#include	INCL_3DENGINE(Collision/CollisionNotifier)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Edge)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Sector)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Face)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)


#define PARALLEL_PRECISION 0.1f

#define USE_UGLY_BOUNDING_SPHERE_PATCH


#ifdef _DEBUG
#	define IFDEF_DEBUG(exp) exp
#else
#	define IFDEF_DEBUG(exp)
#endif

#define SATURATE_BETWEEN(Value, Min, Max)		if(Value < Min)				\
													Value = Min;			\
												else if(Value > Max)		\
													Value = Max;

//#define VISUAL_DEBUG_MODE

#ifdef VISUAL_DEBUG_MODE
#include "KernelCore/Sources/Debug/DebugWindow.h"
#endif

//-------------------------------------------------------------------------------------------
//#define DEBUG_CUSTOM_WINDOW
#ifdef DEBUG_CUSTOM_WINDOW
static HWND hDebug	= 0;
static HWND	hDbg1	= 0;
static HWND	hDbg2	= 0;
static LRESULT CALLBACK DbgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_DESTROY:		break;
		default:				return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
#endif

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_ENTITY::COLLISION_ENTITY()
:
_pNode(0),
_YSpeed(0),
_nWeight(EW_MEDIUM),
_nbStaticOccurences(0),
_nProcessOccured(false),
_pNotifier(0),
_IsActive(true),
_bGravityEnabled(true),
_bNodeProcessedAsGroundPoint(false),
_pSubEntity(0),
_nInternalType(0),
_YAccel(-0.98f),
_bEnableProcessCam(true)
{
	_LastMtx.Identity();

	int i;

	for(i=0; i<MAX_GROUND_POINTS; i++)
	{
		_GPointsState[i] = 0;
	}

	for(i=0; i<MAX_GROUND_POINTS; i++)
	{
		_LastContacts[i].SetX(0.0f);
		_LastContacts[i].SetY(0.0f);
		_LastContacts[i].SetZ(0.0f);
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_ENTITY::COLLISION_ENTITY(const COM_INSTANCE &NodeCI)
:
_YSpeed(0),
_nWeight(EW_MEDIUM),
_nbStaticOccurences(0),
_nProcessOccured(false),
_pNotifier(0),
_IsActive(true),
_bGravityEnabled(true),
_bNodeProcessedAsGroundPoint(false),
_pSubEntity(0),
_nInternalType(0),
_YAccel(-0.98f),
_bEnableProcessCam(true)
{
	_LastMtx.Identity();

	int i;
	for(i=0; i<MAX_GROUND_POINTS; i++)
	{
		_GPointsState[i] = 0;
	}

	for(i=0; i<MAX_GROUND_POINTS; i++)
	{
		_LastContacts[i].SetX(0.0f);
		_LastContacts[i].SetY(0.0f);
		_LastContacts[i].SetZ(0.0f);
	}

	SetNode(NodeCI); 
}


//-----------------------------------------------------------------------------
//	Name:		COLLISION_ENTITY destructor
//	Object:		
//	04-02-06:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_ENTITY::~COLLISION_ENTITY()
{
	COLLISION_MANAGER::Instance()->RemoveNode(this, true, false);
}

//-----------------------------------------------------------------------------
//	Name:		IsAttachedToSceneGraph
//	Object:		
//	04-04-22:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_ENTITY::IsAttachedToSceneGraph(void)
{
	const COM_INSTANCE	&SGNodeInst = _pNode->GetSceneGraphNodeInst();

	if( SGNodeInst.HasHandle() )
		return true;

	return false;


/*	NODE	*pParentNode = _pNode->GetParentNode();

	while(pParentNode)
	{
		if( pParentNode->GetEntityType() == ENTITY_ZONE )
			break;
		pParentNode = _pNode->GetParentNode();
	} 

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

	return false;*/
}

//-----------------------------------------------------------------------------
//	Name:		ForcePosition
//	Object:		
//	05-02-02:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_ENTITY::ForcePosition(POINT2D &	Pos)
{
	VECTOR	NewPos;

	_pNode->GetGlobalPosition(NewPos);
	NewPos.SetX( Pos.GetX() );
	NewPos.SetZ( Pos.GetY() );

	SetNewPosition(NewPos);
}

//-----------------------------------------------------------------------------
//	Name:		GetGroundAltitude
//	Object:		
//	04-06-10:	FDE - Created
//-----------------------------------------------------------------------------
float	COLLISION_ENTITY::GetGroundAltitude(void)
{
	int		nPtCount;
	VECTOR	GlobalPos;

	if (_NodeInstance.HasHandle() == false)
		return 0.0f;

	if ( !_pNode )
		_pNode = static_cast<NODE*>(_NodeInstance.GetCommunicator());

	_pNode->GetGroundPoints(nPtCount);

	_pNode->GetGlobalPosition(GlobalPos);
	if(nPtCount == 0)
	{
		return GlobalPos.GetY();
	}

	float	rDeltaX, rDeltaZ;
	float	fRet = 0.0f;

	rDeltaX	= _LastContacts[1].GetX() - _LastContacts[0].GetX();
	rDeltaZ	= _LastContacts[1].GetZ() - _LastContacts[0].GetZ();

	if (MATH::Abs(rDeltaX) > MATH::Abs(rDeltaZ))
	{
		if (MATH::EqualsEpsilon(MATH::Abs(rDeltaX), 0.0f, 1.0f) == true)
		{
			float rDiff = _LastContacts[0].GetY() - GlobalPos.GetY();

			if (MATH::Abs(rDiff) < 1.0f)
				fRet = _LastContacts[0].GetY();
			else
				fRet = GlobalPos.GetY();
		}
		else
		{
			float	rPente	= (_LastContacts[1].GetY() - _LastContacts[0].GetY()) / rDeltaX;
			float	rB		= _LastContacts[1].GetY() - rPente * _LastContacts[1].GetX();
			fRet			= rPente * GlobalPos.GetX() +  rB; 
		}
	}
	else
	{
		if (MATH::EqualsEpsilon(MATH::Abs(rDeltaZ), 0.0f, 1.0f) == true)
		{
			float rDiff = _LastContacts[0].GetY() - GlobalPos.GetY();

			if (MATH::Abs(rDiff) < 1.0f)
				fRet = _LastContacts[0].GetY();
			else
				fRet = GlobalPos.GetY();
		}
		else
		{
			float	rPente	= (_LastContacts[1].GetY() - _LastContacts[0].GetY()) / rDeltaZ;
			float	rB		= _LastContacts[1].GetY() - rPente * _LastContacts[1].GetZ();
			fRet			= rPente * GlobalPos.GetZ() +  rB; 
		}
	}

	#ifdef VISUAL_DEBUG_MODE
		static DEBUG_WINDOW DbgWnd(2, false);
//		DbgWnd.SetText(1, "GlobalPos X = %.2f", GlobalPos.GetX());
		DbgWnd.SetText(0, "GlobalPos Z = %.2f", GlobalPos.GetZ());
		DbgWnd.SetText(1, "Y = %.2f", fRet);
	#endif

	return fRet; 
}


//-----------------------------------------------------------------------------
//	Name:		GetNbGroundPoints
//	Object:		
//	04-03-22:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_ENTITY::GetNbGroundPoints()
{
	int nReturn; 

	_pNode->GetGroundPoints(nReturn); 
	if(nReturn == 0)
		nReturn = 1;

	return nReturn; 
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::UpdateForTeleport(void)
{
	ForceProcessOccured();
	PostCollide();
	_LastPosColMeshCI.Release();
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-01-28:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::PrepareProcessing()
{
	VECTOR NodePos;
	_pNode = static_cast<NODE*>(_NodeInstance.GetCommunicator());

	if( _DynamicMeshNode.HasHandle() )
	{
		NODE *pNode = static_cast<NODE*>(_DynamicMeshNode.GetCommunicator());
		pNode->GetGlobalPosition(NodePos);
	}
	else
		_pNode->GetGlobalPosition(NodePos);

	_CurPos.SetXY( NodePos.GetX(), NodePos.GetZ() );
	_MoveVector		= _CurPos - _LastPos;
	_SquareMoveLen	= _MoveVector.GetSquareLength();
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingVolume
//	Object:		
//	04-02-12:	JPE - Created
//-----------------------------------------------------------------------------
const BOUNDING_VOLUME *COLLISION_ENTITY::GetBoundingVolume(void)
{
	if( _DynamicMeshNode.HasHandle() )
	{
		NODE *pNode = static_cast<NODE*>(_DynamicMeshNode.GetCommunicator());
		if( _pNode->GetEntity() )
			return pNode->GetEntity()->GetCollisionVolume();
	}
	else
	{
		if( _pNode->GetEntity() )
			return _pNode->GetEntity()->GetCollisionVolume();
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingVolumeGlobalMatrix
//	Object:		
//	04-02-12:	JPE - Created
//-----------------------------------------------------------------------------
MATRIX &COLLISION_ENTITY::GetBoundingVolumeGlobalMatrix(void)
{
	if( _DynamicMeshNode.HasHandle() )
	{
		NODE *pNode = static_cast<NODE*>(_DynamicMeshNode.GetCommunicator());
		return pNode->GetGlobalMatrix();
	}
	else
	{
		return _pNode->GetGlobalMatrix();;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDynamicVolumeMaximalStep
//	Object:		
//	04-04-05:	JPE - Created
//-----------------------------------------------------------------------------
float	COLLISION_ENTITY::GetDynamicVolumeMaximalStep(int &Priority)
{
	const BOUNDING_VOLUME	*pThisBV = GetBoundingVolume();

	if(!pThisBV)
	{
		switch( _pNode->GetEntity()->GetEntityType() )
		{
			case ENTITY_SKELETON:
			{
				SKELETON	*pSkl1 = (SKELETON*)_pNode->GetEntity();
				pThisBV = pSkl1->GetGlobalBoundingCylinder( _pNode );
				break;
			}

			case ENTITY_MESH:
			{
				MESH	*pMesh = (MESH*)_pNode->GetEntity();
//				pThisBV = pMesh->GetBoundingBox();
				break;
			}
		}
	}

	if(pThisBV)
	{
		if( pThisBV->GetType() == BOUNDING_VOLUME::TYPE_OBB )
		{
			OBB *pOBB = (OBB*)pThisBV;
			Priority = 0;
			if( pOBB->GetDistance1() > pOBB->GetDistance3() )
				return pOBB->GetDistance3() / 1.1f;
			return pOBB->GetDistance1() / 1.1f;
		}
		else if( pThisBV->GetType() == BOUNDING_VOLUME::TYPE_CYLINDER )
		{
			CYLINDER *pCyl = (CYLINDER*)pThisBV;
			Priority = 1;
			return pCyl->GetRadius() / 1.1f;
		}
	}

	Priority = -1;

	QDT_Message(QDT_COMMENT("Wrong Collision volume !!! Aborting"));
	LowerFlags(COLLISION_ENTITY::CET_DYNAMIC|COLLISION_ENTITY::CET_DYNAMIC_REPULSION);

	return 0.0f;
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-12-02:	JPE - Created
//-----------------------------------------------------------------------------
#define MIN_MOVE 1.0f
int COLLISION_ENTITY::DynamicCollide(COLLISION_ENTITY *pAgainst)
{
	if( !( pAgainst->IsActive() || IsActive()) )
		return 0;

	if( (_SquareMoveLen < 0.5f) && (pAgainst->_SquareMoveLen < 0.5f) && (_nProcessOccured==false) )
		return 0;

	if (_pNode->GetEntityInst().HasHandle() == false)
	{
		return (0);
	}

	if (pAgainst->_pNode->GetEntityInst().HasHandle() == false)
	{
		return (0);
	}

	int		ThisPri;
	int		AgstPri;
	float	MaximalStep		= GetDynamicVolumeMaximalStep(ThisPri);
	float	AgainstStep		= pAgainst->GetDynamicVolumeMaximalStep(AgstPri);
	float	MoveLen			= MATH::Sqrt(_SquareMoveLen);
	float	AgainstMoveLen	= MATH::Sqrt(pAgainst->_SquareMoveLen);

	if( AgainstMoveLen > 500.0f )
	{
		pAgainst->_nProcessOccured = true;
		return 0;
	}

	if( MoveLen > 500.0f )
	{
		//QDT_Warning(QDT_COMMENT("Entity %s is moving more than 5m, please check !"), _sDebugInfo.GetBuffer());
		_nProcessOccured = true;
		return 0;
	}

	if( !(_nFlags & CET_STATIC) )
	{
		_nProcessOccured = true;		// If we are only dynamic then we have processed this entity
	}									// In case of static, let the static col handle the processing infos.

	if( !(pAgainst->_nFlags & CET_STATIC) )
	{
		pAgainst->_nProcessOccured = true;		// If we are only dynamic then we have processed this entity
	}									// In case of static, let the static col handle the processing infos.

	if(ThisPri < AgstPri)
		MaximalStep = AgainstStep;
	else if (ThisPri == AgstPri)
	{
		if( AgainstStep < MaximalStep )
			MaximalStep = AgainstStep;
	}

	if( MaximalStep < 10.0f )			// Foutage de gueule.
		return 0;

	if( ((AgainstMoveLen > MaximalStep)||(MoveLen > MaximalStep)) )
	{
		int		nRet = 0;
		int		StepRet;
		int		nSteps		= (int) (MoveLen / MaximalStep);
		int		nAgstSteps	= (int) (AgainstMoveLen / MaximalStep);

		if(nSteps < nAgstSteps)
			nSteps = nAgstSteps;
		nSteps++;

		POINT2D	vStep( _MoveVector );
		POINT2D	vAgstStep( pAgainst->_MoveVector );
		VECTOR	SampledPos;
		VECTOR	AgstSampledPos;
		VECTOR	SecureThisPos;
		VECTOR	SecureAgstPos;

		_pNode->GetGlobalPosition(SampledPos);
		SecureThisPos = SampledPos;
		SampledPos.SetX( _LastPos.GetX() );
		SampledPos.SetZ( _LastPos.GetY() );

		pAgainst->_pNode->GetGlobalPosition(AgstSampledPos);
		SecureAgstPos = AgstSampledPos;
		AgstSampledPos.SetX( pAgainst->_LastPos.GetX() );
		AgstSampledPos.SetZ( pAgainst->_LastPos.GetY() );
		
		if(MoveLen > MIN_MOVE)
		{
			vStep.Normalize();
			vStep *= (MoveLen/(float)nSteps);
		}
		else
			vStep.SetXY(0.0f, 0.0f);

		if(AgainstMoveLen > MIN_MOVE)
		{
			vAgstStep.Normalize();
			vAgstStep *= (AgainstMoveLen/(float)nSteps);
		}
		else
			vAgstStep.SetXY(0.0f, 0.0f);

		while (nSteps)
		{
			if(MoveLen > MIN_MOVE)
			{
				SampledPos.SetX( SampledPos.GetX() + vStep.GetX() );
				SampledPos.SetZ( SampledPos.GetZ() + vStep.GetY() );
				SetNewPosition( SampledPos );
				PrepareProcessing();
			}

			if(AgainstMoveLen > MIN_MOVE)
			{
				AgstSampledPos.SetX( AgstSampledPos.GetX() + vAgstStep.GetX() );
				AgstSampledPos.SetZ( AgstSampledPos.GetZ() + vAgstStep.GetY() );
				pAgainst->SetNewPosition( AgstSampledPos );
				pAgainst->PrepareProcessing();
			}

			StepRet = DynamicStepProcess( pAgainst );
			if(StepRet < 0.0f)
				return StepRet;

			if( GetLastPosColMesh() )
				CollideAgainst( GetLastPosColMesh() );

			if( pAgainst->GetLastPosColMesh() )
				pAgainst->CollideAgainst( pAgainst->GetLastPosColMesh() );

			if(StepRet > nRet)
				nRet = StepRet;

			_pNode->GetGlobalPosition(SampledPos);
			pAgainst->_pNode->GetGlobalPosition(AgstSampledPos);

			nSteps--;
		}

		if( !nRet )
		{
			if(MoveLen > MIN_MOVE)
				SetNewPosition(SecureThisPos);
			
			if(AgainstMoveLen > MIN_MOVE)
				pAgainst->SetNewPosition(SecureAgstPos);
		}

		return nRet;
	}

	return DynamicStepProcess(pAgainst);
}

//-----------------------------------------------------------------------------
//	Name:		GetTransformedBoundingVolume
//	Object:		
//	04-04-13:	JPE - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME	*COLLISION_ENTITY::GetTransformedBoundingVolume(void)
{
	const BOUNDING_VOLUME	*pThisBV = GetBoundingVolume();

	if(!pThisBV)
	{
		switch( _pNode->GetEntity()->GetEntityType() )
		{
			case ENTITY_SKELETON:
			{
				SKELETON	*pSkl1 = (SKELETON*)_pNode->GetEntity();
				CYLINDER	*TransformedCyl = pSkl1->GetGlobalBoundingCylinder( _pNode );
				if(TransformedCyl)
				{
					_Cyl = *TransformedCyl;
					return &_Cyl;
				}
				break;
			}

			case ENTITY_MESH:
			{
				MESH		*pMesh = (MESH*)_pNode->GetEntity();
//				pThisBV = pMesh->GetBoundingBox();
				break;
			}
		}

		return 0;
	}
	if(!pThisBV)
		return 0;

	if(pThisBV->GetType() == BOUNDING_VOLUME::TYPE_OBB)
	{
		_Obb = *((OBB*)pThisBV);
		_Obb.Transform( GetBoundingVolumeGlobalMatrix() );
		return &_Obb;
	}
	else if(pThisBV->GetType() == BOUNDING_VOLUME::TYPE_CYLINDER)
	{
		_Cyl = *((CYLINDER*)pThisBV);
		_Cyl.Transform( GetBoundingVolumeGlobalMatrix() );
		return &_Cyl;
	}

	return 0;
}


//-----------------------------------------------------------------------------
//	Name:		DynamicStepProcess
//	Object:		
//	04-04-05:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_ENTITY::DynamicStepProcess(COLLISION_ENTITY *	pAgainst)
{
	const BOUNDING_VOLUME	*pThisBV = GetBoundingVolume();
	const BOUNDING_VOLUME	*pAgstBV = pAgainst->GetBoundingVolume();
	POINT2D					Repulsion;
	COLLISION_ENTITY		*pPositiveEntity = 0;
	COLLISION_ENTITY		*pNegativeEntity = 0;
	int						bRepulse = false;
	CYLINDER				LocalThisCyl;
	CYLINDER				LocalAgstCyl;
	int						bDontTransformThis = false;
	int						bDontTransformAgst = false;

	if(!pThisBV)
	{
		switch( _pNode->GetEntity()->GetEntityType() )
		{
			case ENTITY_SKELETON:
			{
				SKELETON	*pSkl1 = (SKELETON*)_pNode->GetEntity();
				CYLINDER	*TransformedCyl		= pSkl1->GetGlobalBoundingCylinder( _pNode );
				if(TransformedCyl)
				{
					LocalThisCyl = *TransformedCyl;
					pThisBV = &LocalThisCyl;
					bDontTransformThis = true;
				}
				break;
			}

			case ENTITY_MESH:
			{
				MESH		*pMesh = (MESH*)_pNode->GetEntity();
//				pThisBV = pMesh->GetBoundingBox();
				break;
			}
		}
	}
		
	if( !pAgstBV )
	{
		switch( pAgainst->GetNode()->GetEntity()->GetEntityType())
		{
			case ENTITY_SKELETON:
			{
				SKELETON	*pSkl2 = (SKELETON*)pAgainst->GetNode()->GetEntity();
				CYLINDER	*TransformedCyl2	= pSkl2->GetGlobalBoundingCylinder( pAgainst->GetNode() );
				if(TransformedCyl2)
				{
					LocalAgstCyl = *TransformedCyl2;
					pAgstBV = &LocalAgstCyl;
					bDontTransformAgst = true;
				}
				break;
			}

			case ENTITY_MESH:
			{
				MESH		*pMesh = (MESH*)pAgainst->GetNode()->GetEntity();
//				pAgstBV = pMesh->GetBoundingBox();
				break;
			}
		}
	}

	if( (!pAgstBV) || (!pThisBV) )
	{
		QDT_Warning(QDT_COMMENT("Can't perform collisions, no collision volume found !"));
		QDT_Warning(QDT_COMMENT("Removing node from the dynamic system !"));

		if(!pAgstBV)
		{
			pAgainst->LowerFlags(COLLISION_ENTITY::CET_DYNAMIC|COLLISION_ENTITY::CET_DYNAMIC_REPULSION);
		}

		if(!pThisBV)
		{
			LowerFlags(COLLISION_ENTITY::CET_DYNAMIC|COLLISION_ENTITY::CET_DYNAMIC_REPULSION);
			return -1;
		}

		return 0;
	}
	else
	{
		if( pThisBV->GetType() == BOUNDING_VOLUME::TYPE_OBB )
		{
			if(pAgstBV->GetType() == BOUNDING_VOLUME::TYPE_OBB)
			{
				_nProcessOccured = true;
				//QDT_Message("OBB OBB UNSUPORTED !!!!!!!!!!");
				return 0;
			}
			else if(pAgstBV->GetType() == BOUNDING_VOLUME::TYPE_CYLINDER)
			{
				CYLINDER	TransformedCyl( *((CYLINDER*)pAgstBV) );
				OBB			TransformedOBB( *((OBB*)pThisBV) ) ;
				if(!bDontTransformAgst)
					TransformedCyl.Transform( pAgainst->GetBoundingVolumeGlobalMatrix() );
				TransformedOBB.Transform( GetBoundingVolumeGlobalMatrix() );
				if( GetRepulsionVector(TransformedCyl, TransformedOBB, Repulsion) )
				{
					pPositiveEntity	= pAgainst;
					pNegativeEntity	= this;
					bRepulse		= true;
				}
			}
			else
			{
				QDT_Message(QDT_COMMENT("Wrong Collision volume !!! Aborting"));
				return 0;
			}
		}
		else if( pThisBV->GetType() == BOUNDING_VOLUME::TYPE_CYLINDER )
		{
			if(pAgstBV->GetType() == BOUNDING_VOLUME::TYPE_OBB)
			{
				CYLINDER	TransformedCyl( *((CYLINDER*)pThisBV) );
				OBB			TransformedOBB( *((OBB*)pAgstBV) ) ;
				if(!bDontTransformThis)
					TransformedCyl.Transform( GetBoundingVolumeGlobalMatrix() );
				TransformedOBB.Transform( pAgainst->GetBoundingVolumeGlobalMatrix() );
				if( GetRepulsionVector(TransformedCyl, TransformedOBB, Repulsion) )
				{
					pPositiveEntity	= this;
					pNegativeEntity	= pAgainst;
					bRepulse		= true;
				}
			}
			else if(pAgstBV->GetType() == BOUNDING_VOLUME::TYPE_CYLINDER)
			{
				CYLINDER	TransformedCyl( *((CYLINDER*)pThisBV) );
				CYLINDER	TransformedCyl2( *((CYLINDER*)pAgstBV) ) ;
				if(!bDontTransformThis)
					TransformedCyl.Transform( GetBoundingVolumeGlobalMatrix() );
				if(!bDontTransformAgst)
					TransformedCyl2.Transform( pAgainst->GetBoundingVolumeGlobalMatrix() );
				if( GetRepulsionVector(TransformedCyl, TransformedCyl2, Repulsion) )
				{
					pPositiveEntity	= this;
					pNegativeEntity	= pAgainst;
					bRepulse		= true;
				}
			}
			else
			{
				QDT_Message(QDT_COMMENT("Wrong Collision volume !!! Aborting"));
				return 0;
			}
		}
		else
		{
			QDT_Message(QDT_COMMENT("Wrong Collision volume !!! Aborting"));
			return 0;
		}
	}

	if( bRepulse )
	{
		if( _nFlags & CET_DYNAMIC_REPULSION )
		{
			// Repulse pPositiveEntity according to Repulsion vector
			// & repulse pNegativeEntity according to -Repulsion vector
			int	PositiveWeight;
			int NegativeWeight;

			if( pPositiveEntity->IsSlidingFreezed() )
				PositiveWeight = EW_ULTRA_HEAVY;
			else
				PositiveWeight = pPositiveEntity->GetWeight();

			if( pNegativeEntity->IsSlidingFreezed() )
			{
				if(PositiveWeight == EW_ULTRA_HEAVY)
				{
					VECTOR		Pos;
					pNegativeEntity->GetNode()->GetGlobalPosition(Pos);
					Pos.SetX( pNegativeEntity->_LastPos.GetX() );
					Pos.SetZ( pNegativeEntity->_LastPos.GetY() );
					pNegativeEntity->SetNewPosition( Pos );

					pPositiveEntity->GetNode()->GetGlobalPosition(Pos);
					Pos.SetX( pPositiveEntity->_LastPos.GetX() );
					Pos.SetZ( pPositiveEntity->_LastPos.GetY() );
					pPositiveEntity->SetNewPosition( Pos );

					//QDT_Error( "Dynamic collision : PANIC MODE / 2 EW_ULTRA_HEAVY colliding !" );
					return 0;
				}
				NegativeWeight = EW_ULTRA_HEAVY;
			}
			else
				NegativeWeight = pNegativeEntity->GetWeight();

			int WeightScore = PositiveWeight - NegativeWeight;

			if( WeightScore < 0)			// Positive is lighter than Negative  p=100%
			{
				if( ( ((int)pPositiveEntity->GetWeight()) - ((int)pNegativeEntity->GetWeight()) ) > 0 )
				{
					// Something made a lighter entity heavier than the one who is normaly heavier.
					VECTOR		Pos;
					pNegativeEntity->GetNode()->GetGlobalPosition(Pos);
					Pos.SetX( pNegativeEntity->_LastPos.GetX() );
					Pos.SetZ( pNegativeEntity->_LastPos.GetY() );
					pNegativeEntity->SetNewPosition( Pos );
					return 0;
				}
				else
				{
					VECTOR		PositivePos;

					pPositiveEntity->GetNode()->GetGlobalPosition(PositivePos);
					PositivePos.SetXYZ(	PositivePos.GetX() + Repulsion.GetX(), 
										PositivePos.GetY(), 
										PositivePos.GetZ() + Repulsion.GetY() );
					SetNewPosition(PositivePos, pPositiveEntity->GetNode());
					pPositiveEntity->_nProcessOccured = true;

					if(NegativeWeight == EW_ULTRA_HEAVY)
						pNegativeEntity->_bSlideFreezed = true;

					return 2;
				}
			}
			else if( WeightScore == 0)		// Positive have the same weight than Negative  p=50/n=50
			{
				VECTOR		PositivePos;
				VECTOR		NegativePos;

				pPositiveEntity->GetNode()->GetGlobalPosition(PositivePos);
				pNegativeEntity->GetNode()->GetGlobalPosition(NegativePos);

				PositivePos.SetXYZ(	PositivePos.GetX() + (Repulsion.GetX()/2.0f), 
									PositivePos.GetY(), 
									PositivePos.GetZ() + (Repulsion.GetY()/2.0f) );

				NegativePos.SetXYZ(	NegativePos.GetX() - (Repulsion.GetX()/2.0f), 
									NegativePos.GetY(), 
									NegativePos.GetZ() - (Repulsion.GetY()/2.0f) );

				SetNewPosition(PositivePos, pPositiveEntity->GetNode());
				SetNewPosition(NegativePos, pNegativeEntity->GetNode());

				pPositiveEntity->_nProcessOccured = true;
				pNegativeEntity->_nProcessOccured = true;

				return 2;
			}
			else //if( WeightScore > 0)		// Positive is heavier than Negative  n=100%
			{
				if( (((int)pPositiveEntity->GetWeight()) - ((int)pNegativeEntity->GetWeight())) < 0 )
				{
					// Something made a lighter entity heavier than the one who is normaly heavier.
					VECTOR		Pos;
					pPositiveEntity->GetNode()->GetGlobalPosition(Pos);
					Pos.SetX( pPositiveEntity->_LastPos.GetX() );
					Pos.SetZ( pPositiveEntity->_LastPos.GetY() );
					pPositiveEntity->SetNewPosition( Pos );
					return 0;
				}
				else
				{
					VECTOR		NegativePos;

					pNegativeEntity->GetNode()->GetGlobalPosition(NegativePos);

					NegativePos.SetXYZ(	NegativePos.GetX() - Repulsion.GetX(), 
										NegativePos.GetY(), 
										NegativePos.GetZ() - Repulsion.GetY() );
					SetNewPosition(NegativePos, pNegativeEntity->GetNode());

					pNegativeEntity->_nProcessOccured = true;

					if(PositiveWeight == EW_ULTRA_HEAVY)
						pPositiveEntity->_bSlideFreezed = true;

					return 2;
				}
			}
		}

		// Notify dynamic collision
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-12-03:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetRepulsionVector(CYLINDER &pCyl, CYLINDER &pCyl2, POINT2D &Repulsion)
{
	POINT2D		Cyl1( pCyl.GetCenter().GetX(), pCyl.GetCenter().GetZ() );
	POINT2D		Cyl2( pCyl2.GetCenter().GetX(), pCyl2.GetCenter().GetZ() );
	POINT2D		Cyl2ToCyl	= Cyl1 - Cyl2;
	float		MinDist		= pCyl.GetRadius() + pCyl2.GetRadius();
	float		ActualDist	= Cyl2ToCyl.GetLength();
	
	if( (ActualDist+ 0.1f) < MinDist)
	{
		float	Scale = (MinDist - ActualDist)+0.11f;
		if( ActualDist > 0.001f)
		{
			Cyl2ToCyl.Normalize();
		}
		else
		{
			Cyl2ToCyl.SetXY(1.0f, 0.0f);
		}
		Repulsion.SetXY( Cyl2ToCyl.GetX()*Scale, Cyl2ToCyl.GetY()*Scale );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-12-03:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetRepulsionVector(CYLINDER &pCyl, OBB &pObb, POINT2D &Repulsion)
{
	VECTOR		obbCenter	= pObb.GetCenter();
	POINT2D		CylCenter(pCyl.GetCenter().GetX(), pCyl.GetCenter().GetZ());
	POINT2D		obbV1( pObb.GetDirection1().GetX(), pObb.GetDirection1().GetZ() );
	POINT2D		obbV2( pObb.GetDirection3().GetX(), pObb.GetDirection3().GetZ() );

	obbV1 *= pObb.GetDistance1();
	obbV2 *= pObb.GetDistance3();

	POINT2D		BasePos( pCyl.GetCenter().GetX(), pCyl.GetCenter().GetZ() );
	POINT2D		obbPP( obbCenter.GetX()+obbV1.GetX()+obbV2.GetX(), obbCenter.GetZ()+obbV1.GetY()+obbV2.GetY() );
	POINT2D		obbPM( obbCenter.GetX()+obbV1.GetX()-obbV2.GetX(), obbCenter.GetZ()+obbV1.GetY()-obbV2.GetY() );
	POINT2D		obbMM( obbCenter.GetX()-obbV1.GetX()-obbV2.GetX(), obbCenter.GetZ()-obbV1.GetY()-obbV2.GetY() );
	POINT2D		obbMP( obbCenter.GetX()-obbV1.GetX()+obbV2.GetX(), obbCenter.GetZ()-obbV1.GetY()+obbV2.GetY() );

	// PP - PM / 1
	// PM - MM / -2
	// MM - MP / -1
	// MP - PP / 2
	POINT2D	PPPM;
	POINT2D	PMMM;
	POINT2D MMMP;
	POINT2D MPPP;

	switch(GetClosestPointTo(obbPP, obbPM, BasePos, obbV1, PPPM))
	{
		case SIDE_INSIDE:			// 1
		{				
			MMMP.Set( PPPM );

			MMMP -= obbV1;
			MMMP -= obbV1;

			PPPM -= CylCenter;
			MMMP -= CylCenter;

			float fDistPPPM = PPPM.GetLength();
			float fDistMMMP = MMMP.GetLength();

			if(fDistPPPM > fDistMMMP)
			{
				if( fDistMMMP < pCyl.GetRadius())		// -1
				{
					Repulsion.SetXY(-pObb.GetDirection1().GetX(), -pObb.GetDirection1().GetZ());
					Repulsion *= (pCyl.GetRadius()-fDistMMMP);
					return true;
				}
			}
			else
			{
				if( fDistPPPM < pCyl.GetRadius())		// 1
				{
					Repulsion.SetXY(pObb.GetDirection1().GetX(), pObb.GetDirection1().GetZ());
					Repulsion *= (pCyl.GetRadius()-fDistPPPM);
					return true;
				}
			}

			break;;
		}

		case SIDE_PARALLEL:
			break;
	}

	switch(GetClosestPointTo(obbPM, obbMM, BasePos, obbV2, PMMM))
	{
		case SIDE_P1:
		case SIDE_P2:
		{
			// Corner cases : we take the first point, calculate it's oposite, then find the closest
			// we take the first point arbitray, since in both cases, the edges would have a common point
			// and this common point is the point we are searching for.
			MPPP.Set( PMMM );

			MPPP += obbV2;
			MPPP += obbV2;

			PMMM -= CylCenter;
			MPPP -= CylCenter;

			float fDistPMMM = PMMM.GetLength();
			float fDistMPPP = MPPP.GetLength();

			if(fDistPMMM > fDistMPPP)
			{
				if( fDistMPPP < pCyl.GetRadius())		// 2
				{
					MPPP.Normalize();
					Repulsion.SetXY(-MPPP.GetX(), -MPPP.GetY());
					Repulsion *= (pCyl.GetRadius()-fDistMPPP);
					return true;
				}
			}
			else
			{
				if( fDistPMMM < pCyl.GetRadius())		// -2
				{
					PMMM.Normalize();
					Repulsion.SetXY(-PMMM.GetX(), -PMMM.GetY());
					Repulsion *= (pCyl.GetRadius()-fDistPMMM);
					return true;
				}
			}

			break;
		}

		case SIDE_INSIDE:			// 2
		{
			MPPP.Set( PMMM );

			MPPP += obbV2;
			MPPP += obbV2;

			PMMM -= CylCenter;
			MPPP -= CylCenter;

			float fDistPMMM = PMMM.GetLength();
			float fDistMPPP = MPPP.GetLength();

			if(fDistPMMM > fDistMPPP)
			{
				if( fDistMPPP < pCyl.GetRadius())		// 2
				{
					Repulsion.SetXY(pObb.GetDirection3().GetX(), pObb.GetDirection3().GetZ());
					Repulsion *= (pCyl.GetRadius()-fDistMPPP);
					return true;
				}
			}
			else
			{
				if( fDistPMMM < pCyl.GetRadius())		// -2
				{
					Repulsion.SetXY(-pObb.GetDirection3().GetX(), -pObb.GetDirection3().GetZ());
					Repulsion *= (pCyl.GetRadius()-fDistPMMM);
					return true;
				}
			}

			break;
		}

		case SIDE_PARALLEL:
			QDT_Error(QDT_COMMENT("Intersection problem, either the box is null, or the matrix is destroyed"));
			break;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-12-03:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetClosestPointTo(POINT2D &v1, POINT2D &v2, POINT2D &BasePos, POINT2D &Normal, POINT2D &Closest)
{
	if( GetIntersectionPoint(v1, v2, BasePos, Normal, Closest) )
	{
		switch( IsPointOnSegment(v1, v2, Closest) )
		{
			case SIDE_P1:
				Closest = v1;
				return SIDE_P1;
				break;

			case SIDE_P2:
				Closest = v2;
				return SIDE_P2;
				break;

			case SIDE_INSIDE:
				return SIDE_INSIDE;
				break;
		}
	}

	return SIDE_PARALLEL;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetClosestPointTo(	COLLISION_MESH	*pColMesh, 
											const VECTOR	&NodePos, 
											CM_SECTOR		*pSector, 
											POINT2D			&Intersect, 
											int				&iOutEdge,
											float			&Score		)
{
	if(!pSector)
		return false;

	int		iEdge, iSectorEdge;
	CM_EDGE	*pEdges		= pColMesh->GetEdges();
	POINT2D	*pVert		= pColMesh->GetVertices();
	POINT2D	*pNormals	= pColMesh->GetEdgesNormals();
	POINT2D BasePos( NodePos.GetX(), NodePos.GetZ() );
	POINT2D	Isect;
	int		bRet = false;

	for(iSectorEdge=0; iSectorEdge<pSector->GetNbElements(); iSectorEdge++ )
	{
		iEdge = pSector->GetValue(iSectorEdge);

		if ( !GetIntersectionPoint(	pVert[ pEdges[iEdge].GetIndex(0) ],
									pVert[ pEdges[iEdge].GetIndex(1) ],
									BasePos, pNormals[iEdge], Isect		)	)
			continue;

		switch(IsPointOnSegment(pVert[ pEdges[iEdge].GetIndex(0)], pVert[ pEdges[iEdge].GetIndex(1)], Isect))
		{
			case SIDE_INSIDE:
				break;

			case SIDE_P1:
			{
				POINT2D	e12 = pVert[ pEdges[iEdge].GetIndex(1)] - pVert[ pEdges[iEdge].GetIndex(0)];
				e12.Normalize();
				e12 *= 0.5f;
				Isect.SetXY(	pVert[ pEdges[iEdge].GetIndex(0)].GetX() + e12.GetX(), 
								pVert[ pEdges[iEdge].GetIndex(0)].GetY() + e12.GetY()	);
				break;
			}

			case SIDE_P2:
			{
				POINT2D	e21 = pVert[ pEdges[iEdge].GetIndex(0)] - pVert[ pEdges[iEdge].GetIndex(1)];
				e21.Normalize();
				e21 *= 0.5f;
				Isect.SetXY(	pVert[ pEdges[iEdge].GetIndex(1)].GetX() + e21.GetX(), 
								pVert[ pEdges[iEdge].GetIndex(1)].GetY() + e21.GetY()	);
				break;
			}
		}

		float TmpScore =	((NodePos.GetX()-Isect.GetX())*(NodePos.GetX()-Isect.GetX())) +
							((NodePos.GetZ()-Isect.GetY())*(NodePos.GetZ()-Isect.GetY())) ;

		if(TmpScore < Score)
		{
			Score		= TmpScore;
			Intersect	= Isect;
			iOutEdge	= iEdge;
		}

		bRet = true;
	}

	return bRet;
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetClosestPointTo(	COLLISION_MESH	*pColMesh, 
											const VECTOR	&NodePos, 
											int				iSX, 
											int				iSY,
											int				iScan, 
											POINT2D			&Point, 
											int				&iOutEdge	)
{
	float	LastScore	= 1e20f;
	int		bFound		= false;
	int		StartX, lsx = false;
	int		StartY, lsy = false;
	int		EndX,	lex = false;
	int		EndY,	ley = false;
	int		iTst;
	int		iStart, iEnd;
	
	StartX	= iSX - iScan;
	EndX	= iSX + iScan;
	StartY	= iSY - iScan;
	EndY	= iSY + iScan;

	if(StartX < 0)
	{
		lsx		= true;
		StartX	= 0;
	}
	if(StartY < 0)
	{
		lsy		= true;
		StartY	= 0;
	}
	if(EndX >= pColMesh->GetNbStepsX())
	{
		lex		= true;
		EndX	= pColMesh->GetNbStepsX()-1;
	}
	if(EndY >= pColMesh->GetNbStepsY())
	{
		ley		= true;
		EndY	= pColMesh->GetNbStepsY()-1;
	}

	// Ligne du haut
	if( !lsy )
	{
		iStart	= StartY * pColMesh->GetNbStepsX() + StartX;
		iEnd	= StartY * pColMesh->GetNbStepsX() + EndX;
		for( iTst=iStart; iTst<=iEnd; iTst++)
		{
			if(GetClosestPointTo(pColMesh, NodePos, pColMesh->GetSectors()[iTst], Point, iOutEdge, LastScore))
				bFound = true;
		}
	}

	if(iScan > 0)
	{
		// Ligne du bas
		if( !ley )
		{
			iStart	= EndY * pColMesh->GetNbStepsX() + StartX;
			iEnd	= EndY * pColMesh->GetNbStepsX() + EndX;
			for( iTst=iStart; iTst<=iEnd; iTst++)
			{
				if(GetClosestPointTo(pColMesh, NodePos, pColMesh->GetSectors()[iTst], Point, iOutEdge, LastScore))
					bFound = true;
			}
		}

		// Intermediaires - Gauche
		if( !lsx )
		{
			iStart	= StartY * pColMesh->GetNbStepsX() + StartX;
			iEnd	= EndY * pColMesh->GetNbStepsX() + StartX;
			for( iTst=iStart; iTst<=iEnd; iTst+=pColMesh->GetNbStepsX())
			{
				if(GetClosestPointTo(pColMesh, NodePos, pColMesh->GetSectors()[iTst], Point, iOutEdge, LastScore))
					bFound = true;
			}
		}

		// Intermediaires - Droite
		if( !lex )
		{
			iStart	= StartY * pColMesh->GetNbStepsX() + EndX;
			iEnd	= EndY * pColMesh->GetNbStepsX() + EndX;
			for( iTst=iStart; iTst<=iEnd; iTst+=pColMesh->GetNbStepsX())
			{
				if(GetClosestPointTo(pColMesh, NodePos, pColMesh->GetSectors()[iTst], Point, iOutEdge, LastScore))
					bFound = true;
			}
		}
	}

	return bFound;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::IsPositionValid(COLLISION_MESH *pColMesh, POINT2D &outCloser, int &bFound)
{
	VECTOR	NodePos;
	_pNode->GetGlobalPosition(NodePos);
	return IsPositionValid(pColMesh, outCloser, bFound, NodePos);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-05-11:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::IsPositionValid(COLLISION_MESH *pColMesh, POINT2D &outCloser, int &bFound, VECTOR &NodePos)
{
	POINT2D &CMOrigin			= pColMesh->GetOrigin();
	POINT2D &CMMax				= pColMesh->GetMax();

	bFound = false;

	if(	(NodePos.GetX() < CMOrigin.GetX()) || (NodePos.GetX() > CMMax.GetX()) ||
		(NodePos.GetZ() < CMOrigin.GetY()) || (NodePos.GetZ() > CMMax.GetY()) )
		return false;

	int		StartX	= (int)((NodePos.GetX() - pColMesh->GetOrigin().GetX()) / pColMesh->GetStepSize());
	int		StartY	= (int)((NodePos.GetZ() - pColMesh->GetOrigin().GetY()) / pColMesh->GetStepSize());
	int		iScan	= 0;
	int		iStop	= 2;

#ifndef _MASTER
	pColMesh->DebugSetSectors(StartX, StartY, StartX, StartY);
#endif

	int	iScanMax;
	if(pColMesh->GetNbStepsX() > pColMesh->GetNbStepsY())
		iScanMax = pColMesh->GetNbStepsX() / 2 + 1;
	else
		iScanMax = pColMesh->GetNbStepsY() / 2 + 1;

	int		EdgeID = 0;
	POINT2D	FoundPoint;
	float	ScoreDist = 0.0f;
	do 
	{
		int		TmpID;
		POINT2D	TmpPoint;
		if(GetClosestPointTo(pColMesh, NodePos, StartX, StartY, iScan, TmpPoint, TmpID))
		{
			float Score =	((NodePos.GetX()-TmpPoint.GetX())*(NodePos.GetX()-TmpPoint.GetX())) +
							((NodePos.GetZ()-TmpPoint.GetY())*(NodePos.GetZ()-TmpPoint.GetY()));
			if(iStop == 2)
			{
				FoundPoint	= TmpPoint;
				ScoreDist	= Score;
				EdgeID		= TmpID;
			}
			else
			{
				if(Score < ScoreDist)
				{
					FoundPoint	= TmpPoint;
					ScoreDist	= Score;
					EdgeID		= TmpID;
				}
			}
			iStop--;
		}
		else
		{
			if(iStop == 1)
				iStop = 0;
		}

		iScan++;
	} while( iStop && (iScan <= iScanMax) );

	if(iStop == 2)
		return false;

	bFound = true;

	POINT2D	vClosest( NodePos.GetX()-FoundPoint.GetX(), NodePos.GetZ()-FoundPoint.GetY() );

	vClosest.Normalize();

#ifndef _MASTER
	pColMesh->DebugSetEdges(EdgeID, EdgeID);
#endif

	outCloser = FoundPoint /*+ pColMesh->GetEdgesNormals()[EdgeID]*/;

	if( vClosest.Dot( pColMesh->GetEdgesNormals()[EdgeID] ) < 0.0f)
		return false;

	return true;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::SetNode(const COM_INSTANCE &NodeCI)
{
	_NodeInstance = NodeCI;		// Copy the Node to increase it's reference counter;

	// Acts like a teleport
	VECTOR	NodePos;
	_pNode = static_cast<NODE*>(_NodeInstance.GetCommunicator());
	_pNode->GetGlobalPosition(NodePos);
	_LastPos.SetXY(NodePos.GetX(), NodePos.GetZ());
	_LastMtx = _pNode->GetGlobalMatrix();
}

//-----------------------------------------------------------------------------
//	Name:		UpdateForPanicMode
//	Object:		
//	04-05-17:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_ENTITY::UpdateForPanicMode(void)
{
	_pNode->SetGlobalMatrix( _LastMtx );
	_nProcessOccured = false;
	if(_pSubEntity)
		_pSubEntity->_nProcessOccured = _nProcessOccured;
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetIntersectionPoint(	POINT2D	&v1, 
											POINT2D	&v2, 
											POINT2D	&BasePos, 
											POINT2D	&Normal, 
											POINT2D	&Intersect)
{
	int		bParallel	= false;
	float	XDiff12		= v1.GetX() - v2.GetX();

	if(MATH::Abs(XDiff12) < PARALLEL_PRECISION)
	{
		if(MATH::Abs(Normal.GetX()) < PARALLEL_PRECISION)
		{
			bParallel = true;
		}
		else
		{
			float c = Normal.GetY() / Normal.GetX();
			float d = BasePos.GetY() - c * BasePos.GetX();

			Intersect.SetX( v1.GetX() );
			Intersect.SetY( c * Intersect.GetX() + d );
		}
	}
	else if(MATH::Abs(Normal.GetX()) < PARALLEL_PRECISION)
	{
		float a = (v1.GetY() - v2.GetY()) / XDiff12;
		float b = v2.GetY() - a * v2.GetX();

		Intersect.SetX( BasePos.GetX() );
		Intersect.SetY( a * Intersect.GetX() + b );
	}
	else
	{
		float a = (v1.GetY() - v2.GetY()) / XDiff12;
		float b = v2.GetY() - a * v2.GetX();
		float c = Normal.GetY() / Normal.GetX();
		float d = BasePos.GetY() - c * BasePos.GetX();

		if( MATH::Abs(a-c) < PARALLEL_PRECISION/10.0f)
		{
			bParallel = true;
		}
		else
		{
			Intersect.SetX( (d-b) / (a-c) );
			Intersect.SetY( a * Intersect.GetX() + b );
		}
	}

	if(!bParallel)
		return true;

	// Regarding the way this method is called, we should never face a parallel case.
	// So there is no immediate need to use a different form of calculation here.
	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_ENTITY::SEG_SIDE COLLISION_ENTITY::IsPointOnSegment(	POINT2D	&v1, 
																POINT2D	&v2, 
																POINT2D	&Point	)
{
	//		Epsilon :
	//      |--|
	//		x--x-----------------------------x--x
	const	float	Epsilon = 0.01f;
			float	DiffX = v1.GetX() - v2.GetX();
			float	DiffY = v1.GetY() - v2.GetY();

	if( MATH::Abs(DiffX) > MATH::Abs(DiffY) )
	{
		if(DiffX > 0)
		{
			if( Point.GetX() >= (v1.GetX()-Epsilon) )	return SIDE_P1;
			if( Point.GetX() <= (v2.GetX()+Epsilon) )	return SIDE_P2;
		}
		else
		{
			if( Point.GetX() <= (v1.GetX()+Epsilon) )	return SIDE_P1;
			if( Point.GetX() >= (v2.GetX()-Epsilon) )	return SIDE_P2;
		}
	}
	else
	{
		if(DiffY > 0)
		{
			if( Point.GetY() >= (v1.GetY()-Epsilon) )	return SIDE_P1;
			if( Point.GetY() <= (v2.GetY()+Epsilon) )	return SIDE_P2;
		}
		else
		{
			if( Point.GetY() <= (v1.GetY()+Epsilon) )	return SIDE_P1;
			if( Point.GetY() >= (v2.GetY()-Epsilon) )	return SIDE_P2;
		}
	}

	return SIDE_INSIDE;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::SetNewPosition( const VECTOR& NodePos, NODE *pNode )
{
	if(_nInternalType == 1)
	{
		VECTOR	RootNodePos;
		VECTOR	SubNodePos;

		if(pNode)
		{
			QDT_Warning(QDT_COMMENT("Unexpected case, please contact JPE (SetNewPosition called for a sub entity with a valid node as parameter)"));
		}

		pNode = _pNext->GetNode();
		pNode->GetGlobalPosition(RootNodePos);
		_pNode->GetGlobalPosition(SubNodePos);

		//QDT_Message("x:%f  z:%f", SubNodePos.GetX(), SubNodePos.GetZ() );

		RootNodePos += (NodePos - SubNodePos);
		pNode->SetGlobalPosition( RootNodePos );
	}
	else 
	{
		if(!pNode)
		{
			pNode = _pNode;
		}
		pNode->SetGlobalPosition( NodePos );
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::ProcessSaturation(CM_EDGE &Edge, POINT2D &Normal, POINT2D &vSat, int VtxID)
{
	VECTOR NodePos;
	_pNode->GetGlobalPosition(NodePos);

	if(	(Edge.GetVtxFlag(VtxID) == CM_EDGE::EVF_BLOCKING)	||
		(Normal.Dot(_MoveVector) < -0.995f)					|| 
		(_nLoops > 0 )										)
	{
		NodePos.SetX( vSat.GetX() );
		NodePos.SetZ( vSat.GetY() );
		SetNewPosition( NodePos );
		_bSlideFreezed = true;
		return false;
	}

	POINT2D Intersection;

	GetIntersectionPoint(_LastPos, _CurPos, vSat, Normal, Intersection);

	POINT2D Correction(vSat.GetX() - Intersection.GetX(), vSat.GetY() - Intersection.GetY());

	if((Correction.GetX()*Correction.GetX()+Correction.GetY()*Correction.GetY()) < 0.0001f)
	{
		_LastPos.SetXY(vSat.GetX(), vSat.GetY());
		Edge.SetUsed(true);
		return true;
	}

	NodePos.SetX( NodePos.GetX() + vSat.GetX() - Intersection.GetX() );
	NodePos.SetZ( NodePos.GetZ() + vSat.GetY() - Intersection.GetY() );
	SetNewPosition( NodePos );
	_LastPos.SetXY(vSat.GetX(), vSat.GetY());
	Edge.SetUsed(true);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::ProcessRepulsion(COLLISION_MESH *pColMesh, REPULSOR &Repulsion)
{
	if(Repulsion.GetEdge() == -1)
		return false;

	CM_EDGE	&Edge		= pColMesh->GetEdges()[Repulsion.GetEdge()];
	POINT2D	*Vertices	= pColMesh->GetVertices();
	POINT2D	&Normal		= pColMesh->GetEdgesNormals()[Repulsion.GetEdge()];
	POINT2D	&v1			= Vertices[ Edge.GetIndex(0) ];
	POINT2D	&v2			= Vertices[ Edge.GetIndex(1) ];
	POINT2D	Intersection;

	_bStaticTouched = true;
	
	GetIntersectionPoint( v1, v2, _CurPos, Normal, Intersection );

	switch( IsPointOnSegment(v1, v2, Intersection) )
	{           
		case SIDE_INSIDE:
		{
			VECTOR NodePos;
			_pNode->GetGlobalPosition(NodePos);
			NodePos.SetX( Intersection.GetX() );
			NodePos.SetZ( Intersection.GetY() );
			SetNewPosition( NodePos );
			return false;
			break;
		}

		case SIDE_P1:
		{
			GetIntersectionPoint( v1, v2, _LastPos, Normal, Intersection );
			if(IsPointOnSegment(v1, v2, Intersection) == SIDE_P1)
			{
				VECTOR NodePos;
				_pNode->GetGlobalPosition(NodePos);
				NodePos.SetX( v1.GetX() );
				NodePos.SetZ( v1.GetY() );
				SetNewPosition( NodePos );
				_bSlideFreezed = true;
				return false;
			}
			else
				return ProcessSaturation(Edge, Normal, v1, 0);			

			break;
		}

		case SIDE_P2:
		{
			GetIntersectionPoint( v1, v2, _LastPos, Normal, Intersection );
			if(IsPointOnSegment(v1, v2, Intersection) == SIDE_P2)
			{
				VECTOR NodePos;
				_pNode->GetGlobalPosition(NodePos);
				NodePos.SetX( v2.GetX() );
				NodePos.SetZ( v2.GetY() );
				SetNewPosition( NodePos );
				_bSlideFreezed = true;
				return false;
			}
			else
				return ProcessSaturation(Edge, Normal, v2, 1);
			break;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-01-12:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::GenerateStaticColEvent(void)
{
	if(_pNotifier)
	{
		_pNotifier->OnWallEvent( _NodeInstance, _nbStaticOccurences );
		_nbStaticOccurences++;
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-01-12:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::GenerateDynamicColEvent(COLLISION_ENTITY *pAgainst)
{
	if(_pNotifier)
	{
		_pNotifier->OnDynamicEvent(  _NodeInstance, pAgainst->GetNodeCI() );
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-01-12:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::SetNotifier(const COLLISION_NOTIFIER *pNotifier)
{
	_pNotifier = (COLLISION_NOTIFIER*)pNotifier;
}

//-----------------------------------------------------------------------------
//	Name:		GetLastPosColMesh
//	Object:		
//	04-02-09:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_MESH *COLLISION_ENTITY::GetLastPosColMesh(void)
{
	if( _LastPosColMeshCI.HasHandle() )
	{
		if (_LastPosColMeshCI.IsLoaded()) 
		{
			return static_cast<COLLISION_MESH*>(_LastPosColMeshCI.GetCommunicator());
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		ProcessSubEntity
//	Object:		
//	04-05-12:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_ENTITY::ProcessSubEntity(COLLISION_MESH *	pColMesh)
{
	if(_pSubEntity)
	{
#ifdef DEBUG_CUSTOM_WINDOW
		if(!hDebug)
		{
			WNDCLASSEX	wcex;
			HWND		hVoid;

			wcex.cbSize = sizeof(WNDCLASSEX); 

			wcex.style			= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= (WNDPROC)DbgWndProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= 0;
			wcex.hIcon			= 0;
			wcex.hCursor		= 0;
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
			wcex.lpszMenuName	= 0;
			wcex.lpszClassName	= "GniClass";
			wcex.hIconSm		= 0;

			RegisterClassEx(&wcex);
			hDebug = CreateWindow("GniClass", "Debug window", WS_OVERLAPPED, CW_USEDEFAULT, 0, 500, 140, NULL, NULL, 0, NULL);
			hDbg1 = CreateWindowEx( NULL, "STATIC", "Pwap", WS_CHILD, 10, 20, 98, 18, hDebug, NULL, 0, NULL);
			hDbg2 = CreateWindowEx( NULL, "STATIC", "Pwap2", WS_CHILD, 10, 40, 98, 18, hDebug, NULL, 0, NULL);

			ShowWindow(hDbg1, SW_SHOW);
			ShowWindow(hDbg2, SW_SHOW);
			ShowWindow(hDebug, SW_SHOW);
			BringWindowToTop(hDebug);
			UpdateWindow(hDebug);
		}
		if(hDebug)
		{
			char	Tmp[256];
			sprintf(Tmp, "x:%f", _pSubEntity->_LastPos.GetX()); SetWindowText(hDbg1, Tmp);
			sprintf(Tmp, "y:%f", _pSubEntity->_LastPos.GetY()); SetWindowText(hDbg2, Tmp);
		}
#endif
		return _pSubEntity->CollideAgainst(pColMesh);
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		SetSecondaryNode
//	Object:		
//	04-05-13:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_ENTITY::SetSecondaryNode(COM_INSTANCE &	SecondaryNode)
{
	if(_pSubEntity)
	{
		delete _pSubEntity;
	}
	_pSubEntity = new COLLISION_ENTITY;
	_pSubEntity->SetNode( SecondaryNode );
	_pSubEntity->SetNext( this );
	_pSubEntity->_nInternalType = 1;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseSecondaryNode
//	Object:		
//	04-05-13:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_ENTITY::ReleaseSecondaryNode(void)
{
	if(_pSubEntity)
	{
		delete _pSubEntity;
		_pSubEntity = 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::CollideAgainst(COLLISION_MESH *pColMesh)
{
	int			bContinue		= false;
	int			bRepulsed		= false;
	POINT2D		LastPosSecure	= _LastPos;

	_nLoops			= 0;
	_bSlideFreezed	= false;

	PrepareProcessing();

	int	nFirstSX, nFirstEX;
	int	nFirstSY, nFirstEY;

	do
	{
		if(_SquareMoveLen < (0.05f) )
		{
			_nProcessOccured = false;
			break;
		}

		_nProcessOccured = true;

		_MoveVector.Normalize();

		POINT2D Bias	= _MoveVector * 4.0f;
		_RealLastPos	= _LastPos;
		_LastPos		-= Bias;

		UpdateBoundingBox( pColMesh->GetRadius() );

		if( !IsInBoundingBox(pColMesh) )
		{
			break;
		}

#ifdef _DEBUG
		POINT2D MVector	= _CurPos - _LastPos;
		_SquareMoveLen	= MVector.GetSquareLength();
#endif

		int StartX	= (int)((_bbMin.GetX() - pColMesh->GetOrigin().GetX()) / pColMesh->GetStepSize());
		int EndX	= (int)((_bbMax.GetX() - pColMesh->GetOrigin().GetX()) / pColMesh->GetStepSize());
		int StartY	= (int)((_bbMin.GetY() - pColMesh->GetOrigin().GetY()) / pColMesh->GetStepSize());
		int EndY	= (int)((_bbMax.GetY() - pColMesh->GetOrigin().GetY()) / pColMesh->GetStepSize());

		SATURATE_BETWEEN(StartX, 0, pColMesh->GetNbStepsX()-1);
		SATURATE_BETWEEN(StartY, 0, pColMesh->GetNbStepsY()-1);
		SATURATE_BETWEEN(EndX, 0, pColMesh->GetNbStepsX()-1);
		SATURATE_BETWEEN(EndY, 0, pColMesh->GetNbStepsY()-1);

		CM_SECTOR		**pSectors	= pColMesh->GetSectors();

		if(_nLoops)			// Patch to clear use flags on "new sectors" during loop process
		{					// Assuming only one sector can appear here (which *should* be A LOT enough)
			if(nFirstSX!=StartX)
			{
				int nY;
				for(nY=StartY; nY<=EndY; nY++)
				{
					int Idx = nY * pColMesh->GetNbStepsX() + StartX;
					if( pSectors[Idx] )
						pSectors[Idx]->ClearEdgeUsingFlags( pColMesh->GetEdges() );
				}
			}
			if(nFirstSY!=StartY)
			{
				int nX;
				for(nX=StartX; nX<=EndX; nX++)
				{
					int Idx = StartY * pColMesh->GetNbStepsX() + nX;
					if( pSectors[Idx] )
						pSectors[Idx]->ClearEdgeUsingFlags( pColMesh->GetEdges() );
				}
			}
			if(nFirstEX!=EndX)
			{
				int nY;
				for(nY=StartY; nY<=EndY; nY++)
				{
					int Idx = nY * pColMesh->GetNbStepsX() + EndX;
					if( pSectors[Idx] )
						pSectors[Idx]->ClearEdgeUsingFlags( pColMesh->GetEdges() );
				}
			}
			if(nFirstEY!=EndY)
			{
				int nX;
				for(nX=StartX; nX<=EndX; nX++)
				{
					int Idx = EndY * pColMesh->GetNbStepsX() + nX;
					if( pSectors[Idx] )
						pSectors[Idx]->ClearEdgeUsingFlags( pColMesh->GetEdges() );
				}
			}
		}

		nFirstSX = StartX; nFirstEX = EndX;
		nFirstSY = StartY; nFirstEY = EndY;

		int				ScanX, ScanY;
		FAST_INTERSECT	fastInter(&_CurPos, &_LastPos);
		REPULSOR		Repulsion(&_LastPos);

		for(ScanY=StartY; ScanY<=EndY; ScanY++)
		{
			int StartIdx = ScanY * pColMesh->GetNbStepsX() + StartX;
			for(ScanX=StartX; ScanX<=EndX; ScanX++)
			{
				if( pSectors[StartIdx] )
				{
					CollideAgainst( pSectors[StartIdx], pColMesh, fastInter, Repulsion );
				}
				StartIdx++;
			}
		}

		if( Repulsion.GetEdge() != -1 )
			bRepulsed = true;

		if( ProcessRepulsion(pColMesh, Repulsion) )
		{
			bRepulsed			= true;
			bContinue			= true;
			PrepareProcessing();
		}	
		else
			bContinue = false;

		_nLoops++;

	} while( bContinue );

	_LastPos = LastPosSecure;

	return bRepulsed;
}

//-----------------------------------------------------------------------------
//	Name:		GravityApplication
//	Object:		
//	04-02-23:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_ENTITY::GravityApplication()
{
	// Gravity application :
	if(!_bGravityEnabled)			// If no gravity, then no need to perform Y computations
		return 0;

	VECTOR NodePos;
	_pNode->GetGlobalPosition(NodePos);

	float rY = NodePos.GetY();

	_YSpeed += _YAccel;		// Constant acceleration
	NodePos.SetY( NodePos.GetY() + _YSpeed );
	SetNewPosition(NodePos);

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		PostVerticalProcessing
//	Object:		
//	04-02-24:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_ENTITY::PostVerticalProcessing(int	bDoRepositioning,
												 float	YPos, 
												 float	LowestAltFound )
{
	if(_bGravityEnabled)
	{
		int		nbGoundPoints;
		int		iGPoint;
		VECTOR	NodePos;

		_pNode->GetGroundPoints( nbGoundPoints );	// pure points without node correction
		_pNode->GetGlobalPosition(NodePos);

		for(iGPoint=0; iGPoint<nbGoundPoints; iGPoint++)
		{

			switch(_GPointsState[iGPoint])
			{
				case 0:
					break;

				case 1:				// Generate top then switch to "contact" value
					if(_pNotifier)
					{
						_pNotifier->OnGroundEvent( _NodeInstance, iGPoint, _LastContacts[iGPoint].GetY() );
					}
					_GPointsState[iGPoint] = 2;
					break;

				case 2:				// Contact not retrigged, switch to inactive (0)
					_GPointsState[iGPoint] = 0;
					break;

				default:				// Contact retriggered, switch back to normal state
					_GPointsState[iGPoint] = 2;
					break;
			}
		}

		if(bDoRepositioning)
		{
			// Gravity speed reset
			_YSpeed = 0.0f;
			NodePos.SetY( YPos );
			SetNewPosition(NodePos);
		}
		if( NodePos.GetY() < (LowestAltFound-50.0f) )
		{
			_YSpeed = 0.0f;
			NodePos.SetY( LowestAltFound );
			SetNewPosition(NodePos);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetGroundPoints
//	Object:		
//	04-03-22:	JPE - Created
//-----------------------------------------------------------------------------
VECTOR *	COLLISION_ENTITY::GetGroundPoints(int &	nbPoints)
{
	VECTOR	*pRet =	_pNode->GetGroundPoints( nbPoints );
	if(!nbPoints)
	{
		_pNode->GetGlobalPosition(_NodePos);
		nbPoints = 1;
		return &_NodePos;
	}
	return pRet;
}


//-----------------------------------------------------------------------------
//	Name:		'
//	Object:		
//	04-02-23:	JPE - Created
//-----------------------------------------------------------------------------
int	COLLISION_ENTITY::VerticalProcess(COLLISION_MESH *		pColMesh,
									  float &				FoundAltitude,
									  int					iGPoint,
									  const COM_INSTANCE &	ColMeshNodeCI)
{
	// We should now process the vertical repulsion.
	int		nbGoundPoints;
	int		Priority;
	float	Alt;
	VECTOR* GPoints		= GetGroundPoints( nbGoundPoints );
	VECTOR	NodePos;
	_pNode->GetGlobalPosition(NodePos);

	if( iGPoint == -1 )
		Priority = GetGroundAltitude( pColMesh, NodePos, Alt, ColMeshNodeCI );
	else
		Priority = GetGroundAltitude( pColMesh, GPoints[iGPoint], Alt, ColMeshNodeCI );


	if( _bNodeProcessedAsGroundPoint && (iGPoint>=0) )
	{
		float	rUnCorrected;

		if(iGPoint >= 0)
		{
			rUnCorrected = GPoints[iGPoint].GetY() - NodePos.GetY();
			
			if( (rUnCorrected <= 5.0f) && (rUnCorrected > -170.0f) )
			{
				if(_GPointsState[iGPoint] != 1)
				{
					_GPointsState[iGPoint]++;
				}
			}
		}
	}

	if( Priority )
	{
		float	DifferentialY;
		float	rUnCorrected;

		if(iGPoint == -1)
			DifferentialY = NodePos.GetY() - Alt;
		else
			DifferentialY = GPoints[iGPoint].GetY() - Alt;

		if(!_bNodeProcessedAsGroundPoint)
		{
			rUnCorrected = DifferentialY-_YSpeed;

			static const float fThreshold = 0.5f;
			
			if( (rUnCorrected <= fThreshold) && (rUnCorrected > -170.0f) )
			{
				if(_GPointsState[iGPoint] != 1)
				{
					_GPointsState[iGPoint]++;
				}
			}
		}

		if( ( DifferentialY > 0.0f ) )
		{
			FoundAltitude = Alt;
			return Priority-3;		// -2 for YLock  -1 for raycast
		}
		if( ( DifferentialY < -170.0f ) )
		{
			FoundAltitude = Alt;
			return 0;					// Everything under 1m is excluded.
		}

		// We sets the position via the node, so we must be relative to it.
		if( iGPoint != -1)
			Alt = Alt - (GPoints[iGPoint].GetY() - NodePos.GetY() );

		FoundAltitude	= Alt;
		return Priority;		// no check of priority here, it can't change within a given mesh
	}

	FoundAltitude = 1e33;
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::GetGroundAltitude(	COLLISION_MESH		*pColMesh, 
											VECTOR				&TestPt, 
											float				&fOutAlt, 
											const COM_INSTANCE	&ColMeshNodeCI	)
{
	if( !pColMesh->IsPointInside(TestPt) )
		return 0;

	switch(pColMesh->GetRepulsionMode()) 
	{
		case COLLISION_MESH::YRM_RAYCAST:
		{
			int SX	= (int)((TestPt.GetX() - pColMesh->GetOrigin().GetX()) / pColMesh->GetStepSize());
			int SY	= (int)((TestPt.GetZ() - pColMesh->GetOrigin().GetY()) / pColMesh->GetStepSize());

			if( (SX < 0) || (SX >= pColMesh->GetNbStepsX()) )
				return 0;
			if( (SY < 0) || (SY >= pColMesh->GetNbStepsY()) )
				return 0;

			CM_SECTOR	*pSector = pColMesh->GetSectors()[SY*pColMesh->GetNbStepsX() + SX];
			int			iFace;

			if(pSector)
			{
				for(iFace=0; iFace<pSector->GetNbFaces(); iFace++)
				{
					if( VerticalRayCast(	pColMesh, 
											pColMesh->GetFaces()[pSector->GetFaceValue(iFace)], 
											TestPt, fOutAlt) )
					{
						return 2;
					}
				}
			}

			break;
		}
/*
		case COLLISION_MESH::YRM_YLOCKED:
		{
			if( ColMeshNodeCI.HasHandle() )
			{
				NODE				*ColMeshNode	= static_cast <NODE*>(ColMeshNodeCI.GetCommunicator());
				const COM_INSTANCE	&ColMeshZoneCI	= ColMeshNode->GetZoneNodeInst();

				if(ColMeshZoneCI == _NodeSG)
				{
					fOutAlt = pColMesh->GetYLock();
					return 1;
				}
			}
			break;
		}
		*/
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::VerticalRayCast(COLLISION_MESH *pColMesh, CM_FACE &Face, VECTOR &Ray, float &fAlt)
{
	const float	fTolerance	= 1e-07f;
	VECTOR		*VList		= pColMesh->GetFaceVtx();
	VECTOR		&v0			= VList[ Face.GetIdx(0) ];
	VECTOR		v1			= VList[ Face.GetIdx(1) ] - v0;
	VECTOR		v2			= VList[ Face.GetIdx(2) ] - v0;
	VECTOR		Normal;

	Normal.Cross(v1, v2);
	Normal.Normalize();

	if( MATH::Abs(Normal.GetY()) <= fTolerance )
		return false;
		
    VECTOR kDiff0 = Ray - v0;
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

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		CollideAgainst  <-> Sector
//	Object:		The Core :o)
//				Moving from _LastPos to current pos of the Node
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::CollideAgainst(	CM_SECTOR		*pSector, 
										COLLISION_MESH	*pColMesh, 
										FAST_INTERSECT	&fastInter, 
										REPULSOR		&Repulsion			)
{
	int			iEdge;
	CM_EDGE		*pEdges		= pColMesh->GetEdges();
	POINT2D		*pVertices	= pColMesh->GetVertices();
	POINT2D		*pNormals	= pColMesh->GetEdgesNormals();

	for(iEdge=0; iEdge<pSector->GetNbElements(); iEdge++)
	{
		POINT2D	Intersect;
		int		EdgeIdx = pSector->GetValue(iEdge);

		if(_nLoops)
		{
			if( pEdges[EdgeIdx].IsUsed() )
				continue;
		}
		else
			pEdges[EdgeIdx].SetUsed(false);

		if(_MoveVector.Dot(pNormals[EdgeIdx]) >= 0)
			continue;

		FAST_INTERSECT::INTERSECT_TYPE isect = fastInter.GetIntersection(	
													pVertices[ pEdges[EdgeIdx].GetIndex(0) ],
													pVertices[ pEdges[EdgeIdx].GetIndex(1) ],
													Intersect,
													pNormals[EdgeIdx]	);
		switch(isect)
		{
			case FAST_INTERSECT::IT_FULL:
				Repulsion.AddIntersection(Intersect, EdgeIdx, 0);
				break;

			case FAST_INTERSECT::IT_ALMOST:
				Repulsion.AddIntersection(Intersect, EdgeIdx, 1);
				break;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
#define USE_VECTOR_ALGEBRA
COLLISION_ENTITY::FAST_INTERSECT::INTERSECT_TYPE COLLISION_ENTITY::FAST_INTERSECT::GetIntersection(	POINT2D &v3, 
																									POINT2D &v4, 
																									POINT2D &Intersect,
																									POINT2D &Normal		)
{
	int			bParallel = false;
	float		x,y;
	float		XDiff34 = v3.GetX() - v4.GetX();
	const float	Epsilon = 0.5f;
	
//------------------------------------------------------------------------------------------------------
#ifdef USE_VECTOR_ALGEBRA
	POINT2D	rkD0(_v2->GetX()-_v1->GetX(), _v2->GetY()-_v1->GetY());
	POINT2D	rkD1(v4.GetX()-v3.GetX(), v4.GetY()-v3.GetY());
    float	fDet = rkD1.Cross(rkD0);
	POINT2D	rkDiff(v3.GetX() - _v1->GetX(), v3.GetY() - _v1->GetY());
    float	rfD0SqrLen = rkD0.GetSquareLength();

    if ( fDet*fDet > MATH::EPSILON()*rfD0SqrLen*rkD1.GetSquareLength() )
    {
        float fInvDet = 1.0f/fDet;
        float afT0 = rkD1.Cross(rkDiff)*fInvDet;

		x = _v1->GetX() + afT0 * rkD0.GetX();
		y = _v1->GetY() + afT0 * rkD0.GetY();
        //float afT1 = rkD0.Cross(rkDiff)*fInvDet;
    }
    else
    {
		bParallel = true;
    }
#else
//------------------------------------------------------------------------------------------------------
	if( _IsVertical )
	{
		if(MATH::Abs(XDiff34) < PARALLEL_PRECISION)			// Parallel cases
		{
			bParallel = true;
		}
		else
		{
			float c = (v3.GetY() - v4.GetY()) / XDiff34;
			float d = v4.GetY() - c * v4.GetX();

			x = _v1->GetX();
			y = c * x + d;
		}
	}
	else if(MATH::Abs(XDiff34) < PARALLEL_PRECISION)
	{
		if(MATH::Abs(v3.GetY() - v4.GetY()) < PARALLEL_PRECISION) // Ultra small edge. Still need to process, at least for blocking flags.
		{
			POINT2D v12( _v2->GetX()-_v1->GetX(), _v2->GetY()-_v1->GetY() );
			POINT2D v13( v3.GetX()-_v1->GetX(), v3.GetY()-_v1->GetY() );

			v12.Normalize();
			if( MATH::Abs(v12.Cross(v13)) > 0.1f )
				return IT_NO_INTERSECT;

			x = v3.GetX();
			y = v3.GetY();
		}
		else
		{
			x = v3.GetX();
			y = _a * x + _b;
		}
	}
	else
	{
		float c = (v3.GetY() - v4.GetY()) / XDiff34;
		float d = v4.GetY() - c * v4.GetX();

		if( MATH::Abs(_a-c) < PARALLEL_PRECISION/10.0f)			// Parallel
		{
			bParallel = true;
		}
		else
		{
			x = (d-_b) / (_a-c);
			y = _a * x + _b;
		}
	}
#endif
//------------------------------------------------------------------------------------------------------
	if( bParallel )
	{
		POINT2D	vp1;
		POINT2D	vp2;

		COLLISION_ENTITY::GetIntersectionPoint(v3, v4, *_v1, Normal, vp1);
		COLLISION_ENTITY::GetIntersectionPoint(v3, v4, *_v2, Normal, vp2);

		POINT2D v11 = vp1 - *_v1;
		POINT2D v22 = vp2 - *_v2;

		if( v11.Dot(v22) > 0)
			return IT_NO_INTERSECT;

		float	Factor = v11.GetLength();

		Factor = Factor / ( Factor + v22.GetLength() );
		
		POINT2D	Vect = (vp2 - vp1) * Factor;

		x = vp1.GetX() + Vect.GetX();
		y = vp1.GetY() + Vect.GetY();
	}

	Intersect.SetXY(x, y);

	if( MATH::Abs(_Diff) > MATH::Abs(_DiffY) )
	{
		if(_Diff > 0)
		{
			if( x > (_v1->GetX()+Epsilon) )	return IT_NO_INTERSECT;		// Inclusive on v1
			if( x < (_v2->GetX()-Epsilon) )	return IT_NO_INTERSECT;		// Inclusive on v2
		}
		else
		{
			if( x < (_v1->GetX()-Epsilon) )	return IT_NO_INTERSECT;
			if( x > (_v2->GetX()+Epsilon) )	return IT_NO_INTERSECT;
		}
	}
	else
	{
		if( _DiffY > 0 )
		{
			if( y > (_v1->GetY()+Epsilon) )	return IT_NO_INTERSECT;
			if( y < (_v2->GetY()-Epsilon) )	return IT_NO_INTERSECT;
		}
		else
		{
			if( y < (_v1->GetY()-Epsilon) )	return IT_NO_INTERSECT;
			if( y > (_v2->GetY()+Epsilon) )	return IT_NO_INTERSECT;
		}
	}

	float		YDiff34 = v3.GetY() - v4.GetY();
	if( MATH::Abs(XDiff34) > MATH::Abs(YDiff34) )
	{
		if(XDiff34 > 0)
		{
			if( x > (v3.GetX()+Epsilon) )	return IT_NO_INTERSECT;
			if( x < (v4.GetX()-Epsilon) )	return IT_NO_INTERSECT;
		}
		else
		{
			if( x < (v3.GetX()-Epsilon) )	return IT_NO_INTERSECT;
			if( x > (v4.GetX()+Epsilon) )	return IT_NO_INTERSECT;
		}
	}
	else
	{
		if( YDiff34 > 0 )
		{
			if( y > (v3.GetY()+Epsilon) )	return IT_NO_INTERSECT;
			if( y < (v4.GetY()-Epsilon) )	return IT_NO_INTERSECT;
		}
		else
		{
			if( y < (v3.GetY()-Epsilon) )	return IT_NO_INTERSECT;
			if( y > (v4.GetY()+Epsilon) )	return IT_NO_INTERSECT;
		}
	}

	if( MATH::Abs(_Diff) > MATH::Abs(_DiffY) )
	{
		if(_Diff > 0)
		{
			if( x > _v1->GetX() )			return IT_ALMOST;
			if( x < _v2->GetX() )			return IT_ALMOST;
		}
		else
		{
			if( x < _v1->GetX() )			return IT_ALMOST;
			if( x > _v2->GetX() )			return IT_ALMOST;
		}
	}
	else
	{
		if( _DiffY > 0 )
		{
			if( y > _v1->GetY() )			return IT_ALMOST;
			if( y < _v2->GetY() )			return IT_ALMOST;
		}
		else
		{
			if( y < _v1->GetY() )			return IT_ALMOST;
			if( y > _v2->GetY() )			return IT_ALMOST;
		}
	}

	if( MATH::Abs(XDiff34) > MATH::Abs(YDiff34))
	{
		if(XDiff34 > 0)
		{
			if( x > v3.GetX() )				return IT_ALMOST;
			if( x < v4.GetX() )				return IT_ALMOST;
		}
		else
		{
			if( x < v3.GetX() )				return IT_ALMOST;
			if( x > v4.GetX() )				return IT_ALMOST;
		}
	}
	else
	{
		if( YDiff34 > 0 )
		{
			if( y > v3.GetY() )				return IT_ALMOST;
			if( y < v4.GetY() )				return IT_ALMOST;
		}
		else
		{
			if( y < v3.GetY() )				return IT_ALMOST;
			if( y > v4.GetY() )				return IT_ALMOST;
		}
	}

	return IT_FULL;
}

//-----------------------------------------------------------------------------
//	Name:		PostCollide()
//	Object:		As the CollideAgainst() method can return anywhere, call this method just after
//				collision & repulsion have been done. For now updating LastPos. I guess it
//				would be nice to consider calling a virtual for each NODE in case a of repulsion
//				Maybe also make a difference between collide & repulse because we may have colliding
//				objects but without any repulsion (most likely zone tests for instance)
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::PostCollide(void)
{
	if(_nProcessOccured)
	{
		VECTOR NodePos;

		if( _DynamicMeshNode.HasHandle() )
		{
			NODE *pNode = static_cast<NODE*>(_DynamicMeshNode.GetCommunicator());
			pNode->GetGlobalPosition(NodePos);
		}
		else
			_pNode->GetGlobalPosition(NodePos);

		_LastPos.SetXY(NodePos.GetX(), NodePos.GetZ());
		_LastMtx = _pNode->GetGlobalMatrix();
	}
	if(_pSubEntity)
	{
		_pSubEntity->PostCollide();
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_ENTITY::UpdateBoundingBox(float fRadius)
{
	/////// This one will build a bounding box around the ray between last and cur pos
	if( _CurPos.GetX() < _LastPos.GetX() )
	{
		_bbMin.SetX( _CurPos.GetX() );
		_bbMax.SetX( _LastPos.GetX() );
	}
	else
	{
		_bbMin.SetX( _LastPos.GetX() );
		_bbMax.SetX( _CurPos.GetX() );
	}

	if( _CurPos.GetY() < _LastPos.GetY() )
	{
		_bbMin.SetY( _CurPos.GetY() );
		_bbMax.SetY( _LastPos.GetY() );
	}
	else
	{
		_bbMin.SetY( _LastPos.GetY() );
		_bbMax.SetY( _CurPos.GetY() );
	}
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::IsInBoundingBox(COLLISION_MESH *pColMesh)
{
	POINT2D &Origin	= pColMesh->GetOrigin();
	POINT2D	&Max	= pColMesh->GetMax();

	return IsInBoundingBox(Origin, Max);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	03-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int COLLISION_ENTITY::IsInBoundingBox(POINT2D &Origin, POINT2D &Max)
{
	if( Origin.GetX() > _bbMax.GetX() )
		return false;
	if( Origin.GetY() > _bbMax.GetY() )
		return false;
	if( Max.GetX() < _bbMin.GetX() )
		return false;
	if( Max.GetY() < _bbMin.GetY() )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:		
//	04-04-21:	RMA - Created
//-----------------------------------------------------------------------------
COLLISION_ENTITY &	COLLISION_ENTITY::operator=(const COLLISION_ENTITY & ci)
{
	_YSpeed = ci._YSpeed;
	_YAccel = ci._YAccel;
	_bGravityEnabled = ci._bGravityEnabled;
	_bNodeProcessedAsGroundPoint = ci._bNodeProcessedAsGroundPoint;

	int i;
	for (i = 0 ; i < MAX_GROUND_POINTS ; ++i)
	{
		_GPointsState[i] = ci._GPointsState[i];
		_LastContacts[i] = ci._LastContacts[i];
	}

	_IsActive = ci._IsActive;
	_bbMin = ci._bbMin;
	_bbMax = ci._bbMax;
	_nLoops = ci._nLoops;
	_MoveVector = ci._MoveVector;
	_nProcessOccured = ci._nProcessOccured;
	_RealLastPos = ci._RealLastPos;
	_LastPos = ci._LastPos;
	_CurPos = ci._CurPos;
	_SquareMoveLen = ci._SquareMoveLen;
	_NodeSG = ci._NodeSG;
	_NodeInstance = ci._NodeInstance;
	_DynamicMeshNode = ci._DynamicMeshNode;
	_pNode = ci._pNode;
	_pNext = ci._pNext;
	_nFlags = ci._nFlags;
	_nWeight = ci._nWeight;
	_bSlideFreezed = ci._bSlideFreezed;
	_bStaticTouched = ci._bStaticTouched;
	_nbStaticOccurences = ci._nbStaticOccurences;
	_pNotifier = ci._pNotifier;
	_LastPosColMeshCI = ci._LastPosColMeshCI;
	_NodePos = ci._NodePos;
	_sDebugInfo = ci._sDebugInfo;
	_Cyl = ci._Cyl;
	_Obb = ci._Obb;

	return  (*this);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
