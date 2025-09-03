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
//	CLASS:	CONSTRAINT
//	The CONSTRAINT class implements ...
//
//	06-03-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __SCENEGRAPH_ENTITIES_CONSTRAINT_H__
#define __SCENEGRAPH_ENTITIES_CONSTRAINT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KPHYSICS	(I_PHYSIC_JOINT_CUSTOM)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		class	CONSTRAINT : public ENTITY_ABC
		{
		public:

			CONSTRAINT();
			~CONSTRAINT();

			/** For flags usage */
			enum
			{
				CONSTRAINT_INIT			= 1 << 0,
				CONSTRAINT_ALL_FLAGS	= 0xFFFFFFFF,
			};

			// From ENTITY_ABC
			void	Register();
			void	Unregister();
			void	Instanciate(COM_INSTANCE & Instance);
			void	InstanciateRec(COM_INSTANCE & Instance);
			void	PostInstanciate(const QDT_VECTOR < QDT_PAIR< COM_INSTANCE, COM_INSTANCE > > & vInstanciatedMeshes);

			// Initialization
			virtual void	Init();
			virtual void	Clear();

			// Accessors
			_INLINE_ void	InitConstraintData(const COM_INSTANCE & ConstraintDataInst);

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
			static void		CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			QDT_FLAGS	_nFlags;

			I_PHYSIC_JOINT_CUSTOM *	_pJointCustom;
			COM_INSTANCE			_ConstraintDataCI;

			COM_INSTANCE	_Actor1CI;
			COM_INSTANCE	_Actor2CI;
		};
	}
}

#ifndef _DEBUG
	#include	"Constraint.inl"
#endif

#endif // __SCENEGRAPH_ENTITIES_CONSTRAINT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
