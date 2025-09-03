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
//	CLASS:	HAIR
//	The HAIR class implements ...
//
//	05-12-02:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__HAIR_H__
#define __HAIR_H__

#ifdef USE_PRAGMA_ONCE
	#pragma once
#endif

// New inclusions
DECL_3DENGINE(HAIR_CELL)
DECL_3DENGINE(NODE_CHILD)
DECL_KPHYSICS(I_PHYSIC_SCENE)

#include	INCL_KCORE(Template/QDT_Vector)

namespace QDT
{
	namespace M3D
	{
		class	HAIR
		{
		public:
			HAIR();
			~HAIR();

			void	Init(I_PHYSIC_SCENE * pScene);
			void	EnableKinematic(Bool b);
			void	SetLastLockedLevel(UInt32 nLevel);

			void	SetMass(Float32 rMaxMass, Float32 rMinMass, Int32 uiLevelLimit);
			void	SetDamping(Float32 rMaxLinearDamping, Float32 rMinLinearDamping, Float32 rMaxAngularDamping, Float32 rMinAngularDamping, Int32 uiLevelLimit);
			void	SetSwingLimit(Float32 rMaxSwingLimit, Float32 rMinSwingLimit, Int32 uiLevelLimit);
			void	SetSpring(Float32 rMaxSpring, Float32 rMinSpring, Float32 rMaxDamping, Float32 rMinDamping, Int32 uiLevelLimit);
			void	SetBounciness(Float32 rMaxBounciness, Float32 rMinBounciness, Int32 uiLevelLimit);
			void	SetFriction(Float32 rMaxFriction, Float32 rMinFriction, Int32 uiLevelLimit);

			void	SetMassByLevel(UInt32 nLevel, Float32 rMass);
			void	SetDampingByLevel(UInt32 nLevel, Float32 rLinearDamping, Float32 rAngularDamping);
			void	SetSwingLimitByLevel(UInt32 nLevel, Float32 rSwingLimit);
			void	SetSpringByLevel(UInt32 nLevel, Float32 rSpring, Float32 rDamping);
			void	SetBouncinessByLevel(UInt32 nLevel, Float32 rBounciness);
			void	SetFrictionByLevel(UInt32 nLevel, Float32 rFriction);

					 void	AddCell(NODE_CHILD *	pNodeChild);
			_INLINE_ void	SetHead(NODE_CHILD *	pHeadNodeChild);

		private:

			void	InitSideConstraints();

			HAIR_CELL *				_pFirstRoot;
			QDT_VECTOR<HAIR_CELL *>	_HairCellsByLevel;

			NODE_CHILD *	_pHeadNodeChild;

			UInt32	_uiLockedLevelsLimit;
		};
	}
}

#ifndef _DEBUG
#include	"Hair.inl"
#endif

#endif // __HAIR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
