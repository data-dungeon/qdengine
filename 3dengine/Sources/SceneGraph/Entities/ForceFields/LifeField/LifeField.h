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
//	CLASS:	LIFE_FIELD
//	The LIFE_FIELD class implements ...
//
//	02-01-24:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_FORCEFIELDS_LIFEFIELD_H__
#define __M3D_SCENEGRAPH_ENTITIES_FORCEFIELDS_LIFEFIELD_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/ForceFieldABC)

namespace QDT
{
	namespace M3D
	{
		class LIFE_FIELD : public FORCE_FIELD_ABC
		{
		public:

			enum
			{
				CMD_LIFE_FIELD_LAST = CMD_FORCE_FIELD_LAST,
			};

			explicit		LIFE_FIELD();
							~LIFE_FIELD();

			/** Derivation from FORCE_FIELD_ABC : Get the force to apply to a physic body */
		//	virtual void	GetForceToApply(const PARTICLE & Particle, VECTOR & Force) const;
		//	virtual void	GetForceToApply(PARTICLE_SPRITE_ARRAY * pArray, unsigned int i, VECTOR & Force) const;

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunk
			static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

		#ifndef _MASTER
			static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		};
	}
}

#ifndef	_DEBUG
	#include	"LifeField.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_FORCEFIELDS_LIFEFIELD_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
