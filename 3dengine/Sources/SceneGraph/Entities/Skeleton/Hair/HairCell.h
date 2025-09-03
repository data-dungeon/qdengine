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
//	The HAIR_CELL class implements ...
//
//	05-12-02:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__HAIR_CELL_H__
#define __HAIR_CELL_H__

#ifdef USE_PRAGMA_ONCE
	#pragma once
#endif

DECL_KPHYSICS(I_PHYSIC_SCENE);
DECL_KPHYSICS(I_PHYSIC_JOINT_CUSTOM);
DECL_KPHYSICS(I_PHYSIC_DYNAMIC_ACTOR);
DECL_KPHYSICS(I_PHYSIC_MATERIAL);
DECL_3DENGINE(NODE_CHILD);

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class	HAIR_CELL
		{
		public:
						HAIR_CELL();
			explicit	HAIR_CELL(NODE_CHILD * pNodeChild);
						~HAIR_CELL();

					 void			Init(NODE_CHILD * pHeadNodeChild, I_PHYSIC_SCENE * pScene, UInt32 uiLockedLevelsLimit);
					 void			CreateSideConstraint(HAIR_CELL * pCell);

					 void			EnableKinematic(Bool b, UInt32 uiLockedLevelsLimit);
					 void			SetLastLockedLevel(UInt32 nLevel);

					 void			SetMass(Float32 rMaxMass, Float32 rMinMass, Float32 rMassStep,
											Int32 nLevelLimit);
					 void			SetDamping(Float32 rMaxLinearDamping, Float32 rMinLinearDamping, Float32 rLinearDampingStep,
											   Float32 rMaxAngularDamping, Float32 rMinAngularDamping, Float32 rAngularDampingStep,
											   Int32 nLevelLimit);
					 void			SetSwingLimit(Float32 rMaxSwingLimit, Float32 rMinSwingLimit, Float32 rSwingLimitStep,
												  Int32 nLevelLimit);
					 void			SetSpring(Float32 rMaxSpring, Float32 rMinSpring, Float32 rSpringStep,
											  Float32 rMaxDamping, Float32 rMinDamping, Float32 rDampingStep,
											  Int32 nLevelLimit);
					 void			SetBounciness(Float32 rMaxBounciness, Float32 rMinBounciness, Float32 rBouncinessStep,
												  Int32 nLevelLimit);
					 void			SetFriction(Float32 rMaxFriction, Float32 rMinFriction, Float32 rFrictionStep,
												Int32 nLevelLimit);

			_INLINE_ void			SetMass(Float32 rMass);
			_INLINE_ void			SetDamping(Float32 rLinearDamping, Float32 rAngularDamping);
			_INLINE_ void			SetSwingLimit(Float32 rSwingLimit);
			_INLINE_ void			SetSpring(Float32 rSpring, Float32 rDamping);
			_INLINE_ void			SetBounciness(Float32 rBounciness);
			_INLINE_ void			SetFriction(Float32 rFriction);

			_INLINE_ void			SetLevel(UInt32 uiLevel);
			_INLINE_ UInt32			GetLevel() const;
			_INLINE_ void			SetFather(HAIR_CELL * pFather);
			_INLINE_ HAIR_CELL *	GetFather();
					 void			AddChild(HAIR_CELL * pChild);
			_INLINE_ HAIR_CELL *	GetFirstChild();
			_INLINE_ void			SetBrother(HAIR_CELL * pBrother);
			_INLINE_ HAIR_CELL *	GetBrother();
			_INLINE_ void			SetNextSameLevel(HAIR_CELL * pCell);
			_INLINE_ HAIR_CELL *	GetNextSameLevel();

			_INLINE_ const NODE_CHILD *	GetNodeChild() const;

			_INLINE_ const VECTOR &	GetAxis() const;

			_INLINE_ I_PHYSIC_DYNAMIC_ACTOR *	GetActor();

		private:

			UInt32		_uiLevel;

			HAIR_CELL *	_pFather;
			HAIR_CELL *	_pFirstChild;
			HAIR_CELL *	_pBrother;
			HAIR_CELL *	_pNextSameLevel;

			NODE_CHILD * _pNodeChild;

			VECTOR	_Axis;

			// Dynamics
			I_PHYSIC_SCENE *			_pScene;
			I_PHYSIC_DYNAMIC_ACTOR *	_pActor;
			I_PHYSIC_MATERIAL *			_pMaterial;
			I_PHYSIC_JOINT_CUSTOM *		_pJoint;

			I_PHYSIC_DYNAMIC_ACTOR *	_pSideConstraintActor;
			I_PHYSIC_JOINT_CUSTOM *		_pSideConstraintJoint[2];

			Float32 _rMass;
			Float32 _rLinearDamping;
			Float32 _rAngularDamping;	
			Float32	_rSwingLimit;
			Float32	_rSpringValue;
			Float32	_rSpringDamping;
			Float32	_rBounciness;
			Float32 _rFriction;
		};
	}
}

#include	INCL_KPHYSICS(Physics/IPhysicDynamicActor)
#include	INCL_KPHYSICS(Physics/IPhysicMaterial)
#include	INCL_KPHYSICS(Physics/IPhysicJointCustom)

#ifndef _DEBUG
#include	"HairCell.inl"
#endif

#endif // __HAIR_CELL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
