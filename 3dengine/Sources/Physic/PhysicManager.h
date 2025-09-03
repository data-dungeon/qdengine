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
//	CLASS:	PHYSIC_MANAGER
//	The PHYSIC_MANAGER class implements ...
//
//	02-02-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_PHYSIC_PHYSICMANAGER_H__
#define __M3D_PHYSIC_PHYSICMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCOM(SINGLETON_DESCRIPTOR)
DECL_KMATH(VECTOR)
DECL_KPHYSICS(I_PHYSIC_SCENE)
DECL_3DENGINE(NODE)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class PHYSIC_MANAGER
		{
		public :

			enum
			{
				CMD_PM_PAUSE = CMD_USER_START,
				CMD_PM_LAST,
			};

			PHYSIC_MANAGER();
			~PHYSIC_MANAGER();
			
			static PHYSIC_MANAGER*	Instance();
			static void				Suicide();

			void	SetPause(bool b);

			virtual void	Loop();

			// Physics Nodes
			void	AddNodeWithPhysics(NODE * pNode);
			void	RemoveNodeWithPhysics(NODE * pNode);
			void	UnregisterNodeWithPhysics(I_PHYSIC_SCENE * pPhysicScene);

			static SINGLETON_DESCRIPTOR *	GetSingletonDescriptor();
			static SINGLETON_DESCRIPTOR *	CreateSingletonDescriptor();

		private :

			// Update
			void	UpdateNodesWithPhysics();
			void	MoveNodesWithPhysics();

			// Communication
			static void		COM_Init(CLASS_DESCRIPTOR * const pCD);

			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static int		CM_Pause(COM_PARAM	P);

			static PHYSIC_MANAGER *	_pInstance;
			
			bool	_bPause;

			QDT_DLIST<NODE *>	_vNodesWithPhysics;
		};
	}
}

#endif // __M3D_PHYSIC_PHYSICMANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
