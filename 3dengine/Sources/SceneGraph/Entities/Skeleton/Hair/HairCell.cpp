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
//	CLASS:	HAIR_CELL
//
//	05-12-02:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Hair/HairCell)

#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KPHYSICS(Physics/IPhysicShapeCapsule)
#include	INCL_KPHYSICS(Physics/IPhysicShapeSphere)
#include	INCL_KPHYSICS(Physics/IPhysicDynamicActor)
#include	INCL_KPHYSICS(Physics/IPhysicMaterial)
#include	INCL_KPHYSICS(Physics/IPhysicJointCustom)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)

#ifdef _DEBUG
#include	"HairCell.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		HAIR_CELL constructor
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL::HAIR_CELL()
:
_uiLevel				(0),
_pFather				(NULL),
_pFirstChild			(NULL),
_pBrother				(NULL),
_pNextSameLevel			(NULL),
_pNodeChild				(NULL),
_Axis					(0.0f, -1.0f, 0.0f),
_pScene					(NULL),
_pActor					(NULL),
_pMaterial				(NULL),
_pJoint					(NULL),
_pSideConstraintActor	(NULL),
_rMass					(1.0f),
_rLinearDamping			(0.0f),
_rAngularDamping		(0.0f),
_rSwingLimit			(MATH::PI()),
_rSpringValue			(0.0f),
_rSpringDamping			(0.0f),
_rBounciness			(0.0f),
_rFriction				(0.2f)

{
	_pSideConstraintJoint[0] = NULL;
	_pSideConstraintJoint[1] = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		HAIR_CELL constructor
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL::HAIR_CELL(NODE_CHILD *	pNodeChild)
:
_uiLevel				(0),
_pFather				(NULL),
_pFirstChild			(NULL),
_pBrother				(NULL),
_pNextSameLevel			(NULL),
_pNodeChild				(pNodeChild),
_Axis					(0.0f, -1.0f, 0.0f),
_pScene					(NULL),
_pActor					(NULL),
_pMaterial				(NULL),
_pJoint					(NULL),
_pSideConstraintActor	(NULL),
_rMass					(1.0f),
_rLinearDamping			(0.0f),
_rAngularDamping		(0.0f),
_rSwingLimit			(MATH::PI()),
_rSpringValue			(0.0f),
_rSpringDamping			(0.0f),
_rBounciness			(0.0f),
_rFriction				(0.2f)
{
	_pSideConstraintJoint[0] = NULL;
	_pSideConstraintJoint[1] = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		HAIR_CELL destructor
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL::~HAIR_CELL()
{
	if	(_pScene)
	{
		if	(_pActor)
		{
			_pScene->UnregisterDynamicActor(_pActor);
		}

		if	(_pMaterial)
		{
			_pScene->UnregisterMaterial(_pMaterial);
		}

		if	(_pJoint)
		{
			_pScene->UnregisterJoint(_pJoint);
		}

		if	(_pSideConstraintActor)
		{
			_pScene->UnregisterDynamicActor(_pSideConstraintActor);
		}

		if	(_pSideConstraintJoint[0])
		{
			_pScene->UnregisterJoint(_pSideConstraintJoint[0]);
		}

		if	(_pSideConstraintJoint[1])
		{
			_pScene->UnregisterJoint(_pSideConstraintJoint[1]);
		}
	}

	delete	(_pActor);
	delete	(_pMaterial);
	delete	(_pJoint);
	delete	(_pSideConstraintActor);
	delete	(_pSideConstraintJoint[0]);
	delete	(_pSideConstraintJoint[1]);

	delete	(_pBrother);
	delete	(_pFirstChild);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-12-05:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::Init(NODE_CHILD *		pHeadNodeChild,
						I_PHYSIC_SCENE *	pScene,
						UInt32				uiLockedLevelsLimit)
{
	if	(pScene)
	{
		if	(_uiLevel > uiLockedLevelsLimit)
		{
			_pScene = pScene;

			_pMaterial = PHYSIC_CONTEXT::Instance()->CreateMaterial();
			_pScene->RegisterMaterial(_pMaterial);
			_pMaterial->SetBounciness(_rBounciness);
			_pMaterial->SetDynamicFriction(_rFriction);
			_pMaterial->SetStaticFriction(_rFriction);

			VECTOR		Position;

			_pActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();
			_pActor->SetUserData(I_PHYSIC_ACTOR::DATA_TYPE_HAIR_CELL, this);

			_pNodeChild->GetGlobalMatrix().GetPosition(Position);

			if	(_pFirstChild == NULL)
			{
				float	rRadius = 0.1f;

				I_PHYSIC_SHAPE_SPHERE * pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();
				pSphere->SetRadius(rRadius);
				pSphere->SetMaterial(_pMaterial);

				_pActor->PushShape(pSphere);
			}
			else
			{
				float		rAxisLength, rHeight, rRadius = 0.5f;
				VECTOR		RefAxis(0.0f, -1.0f, 0.0f);
				QUATERNION	Rotation;
				VECTOR		LocalPos, ChildPosition;
				MATRIX		LocalMatrix = MATRIX::IDENTITY;
				HAIR_CELL *	pChild = _pFirstChild;

				while	(pChild)
				{
					pChild->GetNodeChild()->GetGlobalMatrix().GetPosition(ChildPosition);
					
					_Axis.Sub(ChildPosition, Position);
					LocalPos.Mul(_Axis, 0.5f);
					rAxisLength = _Axis.Normalize();
					rHeight = rAxisLength - (2.0f * rRadius);
					if	(rHeight < 0.0f)
					{
						rRadius = rAxisLength * 0.5f - 0.005f;
						rHeight = 0.01f;
					}

					Rotation.SetRotation(RefAxis, _Axis);
					LocalMatrix.SetQuaternion(Rotation);
					LocalMatrix.SetTranslation(LocalPos);

					I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
					pCapsule->SetRadius(rRadius);
					pCapsule->SetHeight(rHeight);
					pCapsule->SetLocalMatrix(LocalMatrix);
					pCapsule->SetMaterial(_pMaterial);

					_pActor->PushShape(pCapsule);

					pChild = pChild->GetBrother();
				}
			}

			_pActor->SetCollisionGroup(GROUP_HAIR);
			_pActor->SetMass(_rMass);
			_pActor->SetSolverIterationCount(128);
			_pActor->SetGlobalMatrix(_pNodeChild->GetGlobalMatrix());
			_pActor->EnableKinematicMode(true);
			_pActor->SetLinearDamping(_rLinearDamping);
			_pActor->SetAngularDamping(_rAngularDamping);

			_pScene->RegisterDynamicActor(_pActor);

			_pJoint = PHYSIC_CONTEXT::Instance()->CreateJointCustom();
			if	(_uiLevel > uiLockedLevelsLimit + 1)
			{
				_pJoint->SetActor1(_pFather->GetActor());
			}
			else
			{
				_pJoint->SetActor1(pHeadNodeChild->GetDynamicActor());
			}
			_pJoint->SetActor2(_pActor);
			_pJoint->SetGlobalAnchor(Position);
			_pJoint->SetGlobalAxis(_Axis);
			_pJoint->EnableCollision(false);
			_pJoint->EnableProjection(true);
			_pJoint->SetProjectionDistance(0.5f);
			_pJoint->SetProjectionAngle(0.5f);
			_pJoint->SetXMotionType(JOINT_CUSTOM_MOTION_LOCKED);
			_pJoint->SetYMotionType(JOINT_CUSTOM_MOTION_LOCKED);
			_pJoint->SetZMotionType(JOINT_CUSTOM_MOTION_LOCKED);
			_pJoint->SetSwing1MotionType(JOINT_CUSTOM_MOTION_LIMITED);
			_pJoint->SetSwing2MotionType(JOINT_CUSTOM_MOTION_LIMITED);
			_pJoint->SetTwistMotionType(JOINT_CUSTOM_MOTION_LOCKED);
			_pJoint->SetSwing1LimitValue(_rSwingLimit);
			_pJoint->SetSwing1LimitRestitution(0.0f);
			_pJoint->SetSwing1LimitSpring(0.0f);
			_pJoint->SetSwing1LimitDamper(0.0f);
			_pJoint->SetSwing2LimitValue(_rSwingLimit);
			_pJoint->SetSwing2LimitRestitution(0.0f);
			_pJoint->SetSwing2LimitSpring(0.0f);
			_pJoint->SetSwing2LimitDamper(0.0f);

			QUATERNION	Q;
			Q.Id();

			_pJoint->SetSwingDriveType(JOINT_CUSTOM_DRIVE_POSITION);
			_pJoint->SetSwingDriveSpring(_rSpringValue);
			_pJoint->SetSwingDriveDamping(_rSpringDamping);
			_pJoint->SetDriveOrientation(Q);

			_pScene->RegisterJoint(_pJoint);

			_pNodeChild->SetDynamicActor(_pActor);
			_pNodeChild->EnableKinematic(_pActor->IsKinematicModeEnabled());
		}

		if	(_pFirstChild)
		{
			_pFirstChild->Init(pHeadNodeChild, pScene, uiLockedLevelsLimit);
		}

		if	(_pBrother)
		{
			_pBrother->Init(pHeadNodeChild, pScene, uiLockedLevelsLimit);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateSideConstraint
//	Object:		
//	06-03-06:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::CreateSideConstraint(HAIR_CELL *	pCell)
{
	float		rAxisLength, rHeight, rRadius = 0.5f;
	VECTOR		Position, CellPosition, LocalPos, Axis, RefAxis(0.0f, -1.0f, 0.0f);
	MATRIX		LocalMatrix = MATRIX::IDENTITY;
	QUATERNION	Rotation;

	_pNodeChild->GetGlobalMatrix().GetPosition(Position);
	pCell->GetNodeChild()->GetGlobalMatrix().GetPosition(CellPosition);
	Axis.Sub(CellPosition, Position);

	LocalPos.Mul(Axis, 0.5f);
	rAxisLength = Axis.Normalize();
	rHeight = rAxisLength - (2.0f * rRadius);
	if	(rHeight < 0.0f)
	{
		rRadius = rAxisLength * 0.5f - 0.005f;
		rHeight = 0.01f;
	}

	Rotation.SetRotation(RefAxis, Axis);
	LocalMatrix.SetQuaternion(Rotation);
	LocalMatrix.SetTranslation(LocalPos);

	I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
	pCapsule->SetRadius(rRadius);
	pCapsule->SetHeight(rHeight);
	pCapsule->SetLocalMatrix(LocalMatrix);

	_pSideConstraintActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();
	_pSideConstraintActor->PushShape(pCapsule);
	_pSideConstraintActor->SetCollisionGroup(GROUP_HAIR);
	_pSideConstraintActor->SetMass(_rMass);
	_pSideConstraintActor->SetSolverIterationCount(128);
	_pSideConstraintActor->SetGlobalMatrix(_pNodeChild->GetGlobalMatrix());
	_pSideConstraintActor->EnableKinematicMode(true);
	_pSideConstraintActor->SetLinearDamping(_rLinearDamping);
	_pSideConstraintActor->SetAngularDamping(_rAngularDamping);
	//_pScene->RegisterDynamicActor(_pSideConstraintActor);

	_pSideConstraintJoint[0] = PHYSIC_CONTEXT::Instance()->CreateJointCustom();
	_pSideConstraintJoint[0]->SetActor1(_pActor);
	_pSideConstraintJoint[0]->SetActor2(pCell->GetActor());//_pSideConstraintActor);
	_pSideConstraintJoint[0]->SetGlobalAnchor(Position + LocalPos);
	_pSideConstraintJoint[0]->SetGlobalAxis(Axis);
	_pSideConstraintJoint[0]->EnableCollision(false);
	_pSideConstraintJoint[0]->EnableProjection(true);
	_pSideConstraintJoint[0]->SetProjectionDistance(1.0f);
	_pSideConstraintJoint[0]->SetProjectionAngle(1.0f);
	_pSideConstraintJoint[0]->SetXMotionType(JOINT_CUSTOM_MOTION_LIMITED);
	_pSideConstraintJoint[0]->SetYMotionType(JOINT_CUSTOM_MOTION_FREE);//JOIN_CUSTOM_MOTION_LOCKED);
	_pSideConstraintJoint[0]->SetZMotionType(JOINT_CUSTOM_MOTION_FREE);//JOIN_CUSTOM_MOTION_LOCKED);
	_pSideConstraintJoint[0]->SetSwing1MotionType(JOINT_CUSTOM_MOTION_LIMITED);//JOIN_CUSTOM_MOTION_FREE);
	_pSideConstraintJoint[0]->SetSwing2MotionType(JOINT_CUSTOM_MOTION_LIMITED);//JOIN_CUSTOM_MOTION_FREE);
	_pSideConstraintJoint[0]->SetTwistMotionType(JOINT_CUSTOM_MOTION_LIMITED);//JOIN_CUSTOM_MOTION_FREE);
	_pSideConstraintJoint[0]->SetLinearLimitValue(0.0f);
	_pSideConstraintJoint[0]->SetLinearLimitRestitution(0.0f);
	_pSideConstraintJoint[0]->SetLinearLimitSpring(10.0f);
	_pSideConstraintJoint[0]->SetLinearLimitDamper(1.0f);
	_pSideConstraintJoint[0]->SetSwing1LimitValue(MATH::PI() * 0.1f);
	_pSideConstraintJoint[0]->SetSwing1LimitRestitution(0.0f);
	_pSideConstraintJoint[0]->SetSwing1LimitSpring(50.0f);
	_pSideConstraintJoint[0]->SetSwing1LimitDamper(5.0f);
	_pSideConstraintJoint[0]->SetSwing2LimitValue(MATH::PI() * 0.1f);
	_pSideConstraintJoint[0]->SetSwing2LimitRestitution(0.0f);
	_pSideConstraintJoint[0]->SetSwing2LimitSpring(50.0f);
	_pSideConstraintJoint[0]->SetSwing2LimitDamper(5.0f);
	_pSideConstraintJoint[0]->SetTwistLowLimitValue(-MATH::PI() * 0.1f);
	_pSideConstraintJoint[0]->SetTwistLowLimitRestitution(0.0f);
	_pSideConstraintJoint[0]->SetTwistLowLimitSpring(0.0f);
	_pSideConstraintJoint[0]->SetTwistLowLimitDamper(0.0f);
	_pSideConstraintJoint[0]->SetTwistHighLimitValue(MATH::PI() * 0.1f);
	_pSideConstraintJoint[0]->SetTwistHighLimitRestitution(0.0f);
	_pSideConstraintJoint[0]->SetTwistHighLimitSpring(0.0f);
	_pSideConstraintJoint[0]->SetTwistHighLimitDamper(0.0f);
	_pScene->RegisterJoint(_pSideConstraintJoint[0]);

	_pSideConstraintJoint[1] = PHYSIC_CONTEXT::Instance()->CreateJointCustom();
	_pSideConstraintJoint[1]->SetActor1(_pSideConstraintActor);
	_pSideConstraintJoint[1]->SetActor2(pCell->GetActor());
	_pSideConstraintJoint[1]->SetGlobalAnchor(Position);
	_pSideConstraintJoint[1]->SetGlobalAxis(Axis);
	_pSideConstraintJoint[1]->EnableCollision(false);
	_pSideConstraintJoint[1]->EnableProjection(true);
	_pSideConstraintJoint[1]->SetProjectionDistance(0.5f);
	_pSideConstraintJoint[1]->SetProjectionAngle(0.5f);
	_pSideConstraintJoint[1]->SetXMotionType(JOINT_CUSTOM_MOTION_LOCKED);
	_pSideConstraintJoint[1]->SetYMotionType(JOINT_CUSTOM_MOTION_LOCKED);
	_pSideConstraintJoint[1]->SetZMotionType(JOINT_CUSTOM_MOTION_LOCKED);
	_pSideConstraintJoint[1]->SetSwing1MotionType(JOINT_CUSTOM_MOTION_FREE);
	_pSideConstraintJoint[1]->SetSwing2MotionType(JOINT_CUSTOM_MOTION_FREE);
	_pSideConstraintJoint[1]->SetTwistMotionType(JOINT_CUSTOM_MOTION_FREE);
	//_pScene->RegisterJoint(_pSideConstraintJoint[1]);
	
}

//-----------------------------------------------------------------------------
//	Name:		EnableKinematic
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::EnableKinematic(Bool		b,
								   UInt32	uiLockedLevelsLimit)
{
	if	(_pActor && (_uiLevel > uiLockedLevelsLimit))
	{
		_pActor->EnableKinematicMode(b);
		_pNodeChild->EnableKinematic(b);

		if	(_pSideConstraintActor)
		{
			_pSideConstraintActor->EnableKinematicMode(b);
		}
	}

	if	(_pFirstChild)
	{
		_pFirstChild->EnableKinematic(b, uiLockedLevelsLimit);
	}

	if	(_pBrother)
	{
		_pBrother->EnableKinematic(b, uiLockedLevelsLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetLastLockedLevel
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetLastLockedLevel(UInt32	nLevel)
{
/*	if	(_pJoint)
	{
		if	(_uiLevel > nLevel)
		{
			_pJoint->SetXMotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetYMotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetZMotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetSwing1MotionType(JOIN_CUSTOM_MOTION_LIMITED);
			_pJoint->SetSwing2MotionType(JOIN_CUSTOM_MOTION_LIMITED);
			_pJoint->SetTwistMotionType(JOIN_CUSTOM_MOTION_LOCKED);
		}
		else
		{
			_pJoint->SetXMotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetYMotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetZMotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetSwing1MotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetSwing2MotionType(JOIN_CUSTOM_MOTION_LOCKED);
			_pJoint->SetTwistMotionType(JOIN_CUSTOM_MOTION_LOCKED);

			//_pJoint->GetActor1()->EnableKinematicMode(true);
			//_pJoint->GetActor2()->EnableKinematicMode(true);
		}
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetLastLockedLevel(nLevel);
	}

	if	(_pBrother)
	{
		_pBrother->SetLastLockedLevel(nLevel);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetMass(Float32	rMaxMass,
						   Float32	rMinMass,
						   Float32	rMassStep,
						   Int32	nLevelLimit)
{
	if	(nLevelLimit < 0)
	{
		_rMass = rMaxMass - (rMassStep * _uiLevel);
	}
	else
	{
		if	(_uiLevel < static_cast<UInt32>(nLevelLimit))
		{
			_rMass = rMaxMass;
		}
		else
		{
			_rMass = rMinMass;
		}
	}

	if	(_pActor)
	{
		_pActor->SetMass(_rMass);

		if	(_pSideConstraintActor)
		{
			_pSideConstraintActor->SetMass(_rMass);
		}
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetMass(rMaxMass, rMinMass, rMassStep, nLevelLimit);
	}

	if	(_pBrother)
	{
		_pBrother->SetMass(rMaxMass, rMinMass, rMassStep, nLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDamping
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetDamping(Float32	rMaxLinearDamping,
							  Float32	rMinLinearDamping,
							  Float32	rLinearDampingStep,
							  Float32	rMaxAngularDamping,
							  Float32	rMinAngularDamping,
							  Float32	rAngularDampingStep,
							  Int32		nLevelLimit)
{
	if	(nLevelLimit < 0)
	{
		_rLinearDamping = rMaxLinearDamping - (rLinearDampingStep * _uiLevel);
		_rAngularDamping = rMaxAngularDamping - (rAngularDampingStep * _uiLevel);
	}
	else
	{
		if	(_uiLevel < static_cast<UInt32>(nLevelLimit))
		{
			_rLinearDamping = rMaxLinearDamping;
			_rAngularDamping = rMaxAngularDamping;
		}
		else
		{
			_rLinearDamping = rMinLinearDamping;
			_rAngularDamping = rMinAngularDamping;
		}
	}

	if	(_pActor)
	{
		_pActor->SetLinearDamping(_rLinearDamping);
		_pActor->SetAngularDamping(_rAngularDamping);

		if	(_pSideConstraintActor)
		{
			_pSideConstraintActor->SetLinearDamping(_rLinearDamping);
			_pSideConstraintActor->SetAngularDamping(_rAngularDamping);
		}
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetDamping(rMaxLinearDamping, rMinLinearDamping, rLinearDampingStep,
								 rMaxAngularDamping, rMinAngularDamping, rAngularDampingStep,
								 nLevelLimit);
	}

	if	(_pBrother)
	{
		_pBrother->SetDamping(rMaxLinearDamping, rMinLinearDamping, rLinearDampingStep,
							  rMaxAngularDamping, rMinAngularDamping, rAngularDampingStep,
							  nLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingLimit
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetSwingLimit(Float32	rMaxSwingLimit,
								 Float32	rMinSwingLimit,
								 Float32	rSwingLimitStep,
								 Int32		nLevelLimit)
{
	if	(nLevelLimit < 0)
	{
		_rSwingLimit = rMaxSwingLimit - (rSwingLimitStep * _uiLevel);
	}
	else
	{
		if	(_uiLevel < static_cast<UInt32>(nLevelLimit))
		{
			_rSwingLimit = rMaxSwingLimit;
		}
		else
		{
			_rSwingLimit = rMinSwingLimit;
		}
	}

	if	(_pJoint)
	{
		_pJoint->SetSwing1LimitValue(_rSwingLimit);
		_pJoint->SetSwing2LimitValue(_rSwingLimit);
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetSwingLimit(rMaxSwingLimit, rMinSwingLimit, rSwingLimitStep, nLevelLimit);
	}

	if	(_pBrother)
	{
		_pBrother->SetSwingLimit(rMaxSwingLimit, rMinSwingLimit, rSwingLimitStep, nLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSpring
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetSpring(Float32	rMaxSpring,
							 Float32	rMinSpring,
							 Float32	rSpringStep,
							 Float32	rMaxDamping,
							 Float32	rMinDamping,
							 Float32	rDampingStep,
							 Int32		nLevelLimit)
{
	if	(nLevelLimit < 0)
	{
		_rSpringValue = rMaxSpring - (rSpringStep * _uiLevel);
		_rSpringDamping = rMaxDamping - (rDampingStep * _uiLevel);
	}
	else
	{
		if	(_uiLevel < static_cast<UInt32>(nLevelLimit))
		{
			_rSpringValue = rMaxSpring;
			_rSpringDamping = rMaxDamping;
		}
		else
		{
			_rSpringValue = rMinSpring;
			_rSpringDamping = rMinDamping;
		}
	}

	if	(_pJoint)
	{
		_pJoint->SetSwingDriveSpring(_rSpringValue);
		_pJoint->SetSwingDriveDamping(_rSpringDamping);
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetSpring(rMaxSpring, rMinSpring, rSpringStep,
								rMaxDamping, rMinDamping, rDampingStep,
								nLevelLimit);
	}

	if	(_pBrother)
	{
		_pBrother->SetSpring(rMaxSpring, rMinSpring, rSpringStep,
							 rMaxDamping, rMinDamping, rDampingStep,
							 nLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBounciness
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetBounciness(Float32 rMaxBounciness,
								 Float32 rMinBounciness,
								 Float32 rBouncinessStep,
								 Int32 nLevelLimit)
{
	if	(nLevelLimit < 0)
	{
		_rBounciness = rMaxBounciness - (rBouncinessStep * _uiLevel);
	}
	else
	{
		if	(_uiLevel < static_cast<UInt32>(nLevelLimit))
		{
			_rBounciness = rMaxBounciness;
		}
		else
		{
			_rBounciness = rMinBounciness;
		}
	}

	if	(_pMaterial)
	{
		_pMaterial->SetBounciness(_rBounciness);
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetBounciness(rMaxBounciness, rMinBounciness, rBouncinessStep, nLevelLimit);
	}

	if	(_pBrother)
	{
		_pBrother->SetBounciness(rMaxBounciness, rMinBounciness, rBouncinessStep, nLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFriction
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetFriction(Float32 rMaxFriction,
							   Float32 rMinFriction,
							   Float32 rFrictionStep,
							   Int32 nLevelLimit)
{
	if	(nLevelLimit < 0)
	{
		_rFriction = rMaxFriction - (rFrictionStep * _uiLevel);
	}
	else
	{
		if	(_uiLevel < static_cast<UInt32>(nLevelLimit))
		{
			_rFriction = rMaxFriction;
		}
		else
		{
			_rFriction = rMinFriction;
		}
	}

	if	(_pMaterial)
	{
		_pMaterial->SetDynamicFriction(_rFriction);
		_pMaterial->SetStaticFriction(_rFriction);
	}

	if	(_pFirstChild)
	{
		_pFirstChild->SetFriction(rMaxFriction, rMinFriction, rFrictionStep, nLevelLimit);
	}

	if	(_pBrother)
	{
		_pBrother->SetFriction(rMaxFriction, rMinFriction, rFrictionStep, nLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddChild
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::AddChild(HAIR_CELL *	pChild)
{
	pChild->SetFather(this);
	pChild->SetLevel(_uiLevel + 1);

	if	(_pFirstChild)
	{
		HAIR_CELL *	pCell = _pFirstChild;
		while	(pCell->GetBrother() != NULL)
		{
			pCell = pCell->GetBrother();
		}

		pCell->SetBrother(pChild);
	}
	else
	{
		_pFirstChild = pChild;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
