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
//	CLASS:	RAGDOLL_ACTOR
//	The RAGDOLL_ACTOR class implements ...
//
//	06-03-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_SKELETON_RAGDOLL_RAGDOLLACTOR_H__
#define __M3D_SCENEGRAPH_ENTITIES_SKELETON_RAGDOLL_RAGDOLLACTOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(PHYSIC_SHAPE_DATA)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class	RAGDOLL_ACTOR
		{
		public:

			RAGDOLL_ACTOR();
			~RAGDOLL_ACTOR();

			// Accessors
			_INLINE_ Float32					GetDensity() const;
			_INLINE_ Float32					GetMass() const;
			_INLINE_ const VECTOR & 			GetCenterOfMass() const;
			_INLINE_ Float32					GetLinearDamping() const;
			_INLINE_ Float32					GetAngularDamping() const;
			_INLINE_ Bool						IsInfluencedByGravity() const;
			_INLINE_ UInt32						GetNbrSolverIterations() const;
			_INLINE_ UInt32						GetNbrShapesData() const;
			_INLINE_ const PHYSIC_SHAPE_DATA *	GetShapeData(UInt32 nIndex) const;
			_INLINE_ Float32					GetBounciness() const;
			_INLINE_ Float32					GetStaticFriction() const;
			_INLINE_ Float32					GetDynamicFriction() const;

			// Modifors
			_INLINE_ void	SetDensity(Float32 rDensity);
			_INLINE_ void	SetMass(Float32 rMass);
			_INLINE_ void	SetCenterOfMass(const VECTOR & CenterOfMass);
			_INLINE_ void	SetLinearDamping(Float32 LinearDamping);
			_INLINE_ void	SetAngularDamping(Float32 AngularDamping);
			_INLINE_ void	SetInfluencedByGravity(Bool bGravity);
			_INLINE_ void	SetNbrSolverIterations(UInt32 nNbrSolverIterations);
			_INLINE_ void	SetNbrShapesData(UInt32 nNbrShapesData);
			_INLINE_ void	SetShapeData(UInt32 nIndex, PHYSIC_SHAPE_DATA * pShapeData);
			_INLINE_ void	SetBounciness(Float32 rBounciness);
			_INLINE_ void	SetStaticFriction(Float32 rStaticFriction);
			_INLINE_ void	SetDynamicFriction(Float32 rDynamicFriction);

			// Chunk
			void	ReadChunk(IO_STREAM & Stream);
			void	WriteChunk(IO_STREAM & Stream);

		private:

			Float32	_rDensity;
			Float32	_rMass;

			VECTOR	_CenterOfMass;

			Float32	_LinearDamping;
			Float32	_AngularDamping;

			Bool	_bGravity;

			UInt32	_nNbrSolverIterations;

			// Shapes
			UInt32					_nNbrShapesData;
			PHYSIC_SHAPE_DATA **	_pShapesData;

			// Material
			Float32	_rBounciness;
			Float32	_rStaticFriction;
			Float32	_rDynamicFriction;
		};
	}
}

#ifndef _DEBUG
	#include "RagdollActor.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_SKELETON_RAGDOLL_RAGDOLLACTOR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
