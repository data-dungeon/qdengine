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
//	CLASS:	RAGDOLL_DATA
//	The RAGDOLL_DATA class implements ...
//
//	06-03-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_SKELETON_RAGDOLL_RAGDOLLDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_SKELETON_RAGDOLL_RAGDOLLDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(RAGDOLL_ACTOR)
DECL_3DENGINE(RAGDOLL_CONSTRAINT)

#include	INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	RAGDOLL_DATA : public COMMUNICATOR
		{
		public:

			RAGDOLL_DATA();
			~RAGDOLL_DATA();

			// Accessors
			_INLINE_ UInt32						GetNbrActors() const;
			_INLINE_ const RAGDOLL_ACTOR *		GetActor(UInt32 uiActorIndex) const;
			_INLINE_ UInt32						GetNbrConstraints() const;
			_INLINE_ const RAGDOLL_CONSTRAINT &	GetConstraint(UInt32 uiConstraintIndex) const;

			// Modifors
			_INLINE_ void	SetActors(UInt32 nNbrActors, RAGDOLL_ACTOR ** pActors);
			_INLINE_ void	SetConstraints(UInt32 nNbrConstraints, RAGDOLL_CONSTRAINT * pConstraints);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunks
			static void		CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			// Actors (colliding shapes)
			UInt32				_uiNbrActors;
			RAGDOLL_ACTOR **	_pActors;

			// Constraints
			UInt32					_uiNbrConstraints;
			RAGDOLL_CONSTRAINT *	_pConstraints;
		};
	}
}

#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollActor)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollConstraint)

#ifndef _DEBUG
	#include "RagdollData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_SKELETON_RAGDOLL_RAGDOLLDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
