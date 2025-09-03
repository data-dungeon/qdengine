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
//	CLASS:	PHYSIC_JOINT
//	The PHYSIC_JOINT class implements ...
//
//	05-02-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_JOINTS_PHYSICJOINT_H__
#define __M3D_SCENEGRAPH_JOINTS_PHYSICJOINT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(SCENE_GRAPH)
DECL_KMATH(VECTOR)
DECL_KPHYSICS(I_PHYSIC_JOINT)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class PHYSIC_JOINT : public COMMUNICATOR
		{
		public:

			enum
			{
				CMD_SET_NODE1 = CMD_COM_USER_START,
				CMD_SET_NODE2,
				CMD_SET_GLOBAL_ANCHOR,
				CMD_SET_GLOBAL_AXIS,
				CMD_ENABLE_COLLISION,
				CMD_PHYSIC_JOINT_LAST,
			};

			PHYSIC_JOINT();
			PHYSIC_JOINT(COMMUNICATOR_DESCRIPTOR * pCD);
			virtual ~PHYSIC_JOINT();

			void SetNode1(const COM_INSTANCE & ciNode1);
			void SetNode2(const COM_INSTANCE & ciNode2);
			void SetGlobalAnchor(const VECTOR & Pos);
			void SetGlobalAxis(const VECTOR & Axis);
			void EnableCollision(bool bState);

			void RegisterJoint(SCENE_GRAPH * pSceneGraph);
			void UnregisterJoint();

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

		protected:

			I_PHYSIC_JOINT *	_pJoint; // Interface to the physic SDK
			SCENE_GRAPH *		_pSceneGraph;

		private:

			//	Commands
			static	void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetNode1(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetNode2(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetGlobalAnchor(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetGlobalAxis(void * const pO, void * const pR, void * const pP);
			static  void		EM_EnableCollision(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_SCENEGRAPH_JOINTS_PHYSICJOINT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
