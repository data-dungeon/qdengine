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
//	CLASS:	MOTEUR_3D
//	The M3D class implements ...
//
//	02-07-10:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_MOTEUR3D_H__
#define __M3D_MOTEUR3D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(SCENE_GRAPH);
DECL_3DENGINE(DISPLAY_MANAGER);
DECL_3DENGINE(BEHAVIOR_MANAGER);
DECL_3DENGINE(PARTICLE_SYSTEM_MANAGER);
DECL_KCOM(SINGLETON_DESCRIPTOR);

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KMATH(Math/Geometry/Matrix/Matrix)

namespace QDT
{
	namespace M3D
	{
		class	MOTEUR_3D
		{
		public:
			enum
			{
				CMD_M3D_ADD_IHM_ELEMENT = CMD_USER_START,
				CMD_M3D_REMOVE_IHM_ELEMENT,
				CMD_M3D_DUMP_SCENE_GRAPH,
				CMD_PAUSE,
				CMD_RESUME,
				CMD_CLEAR_IHM_ELEMENT,
				CMD_M3D_ADD_IHM_ELEMENT_EX,
				CMD_SAVE_STATE,
				CMD_RESTORE_STATE,
				CMD_CLEAR_STATE,
				CMD_RESET_ENGINE,
				CMD_SAVE_DEBUG_PHYSICS_FILE,
				CMD_ADD_VIDEO_IHM_ELEMENT,
				CMD_REMOVE_VIDEO_IHM_ELEMENT,
				CMD_CLEAR_VIDEO_IHM_ELEMENT,
				CMD_ADD_FIND_AND_REPLACE,
				CMD_CLEAR_FIND_AND_REPLACE,
				CMD_M3D_LAST
			};

			/** For singleton  */
			_INLINE_ static MOTEUR_3D *			Instance();
			_INLINE_ static void				Release();
			_INLINE_ static bool				IsInstancied();

			void								Pause();
			void								Continue();

			/** Scene graph management */
			void								AddSceneGraph(SCENE_GRAPH * pSG);
			void								RemoveSceneGraph(SCENE_GRAPH * pSG);
		#ifndef _MASTER
			void								DumpSceneGraph(const QDT_STRING & sFileName);
		#endif
			const QDT_DLIST<SCENE_GRAPH *> &	GetSceneGraphsList() const;

			/** Viewport Management */
			void								AddIhmElement(const COM_INSTANCE &Element, bool bUnderInterface = true);
			void								RemoveIhmElement(const COM_INSTANCE &Element);
			void								ClearIhmElements();

			/** For all modules : OpenTools (initialize module) */
			static	void						OpenTools();
			static  void						CloseTools();

			/** Derivating COM_OBJECT */
			virtual void						Loop();
					void						EnableLoop(const bool bEnableLoop = true);
			virtual void						Display();

			void								SaveState();
			void								RestoreState();
			void								ClearState();

			void								ResetEngine();

			void								SaveDebugPhysicsFile(const QDT_STRING & sFileName) const;

			static SINGLETON_DESCRIPTOR *		GetSingletonDescriptor();
			static SINGLETON_DESCRIPTOR *		CreateSingletonDescriptor();

		private:
			MOTEUR_3D();
			virtual ~MOTEUR_3D();

			/** Scene graphs list*/
			QDT_DLIST <SCENE_GRAPH *>			_SceneGraphsList;
			bool								_bEnableLoop;

			static void							Init(CLASS_DESCRIPTOR * const pCD);

			static void							EM_AddIhmElement(void * const pO, void * const pR, void * const pP);
			static int							CM_AddIhmElement(COM_PARAM P);
			static void							EM_RemoveIhmElement(void * const pO, void * const pR, void * const pP);
			static int							CM_RemoveIhmElement(COM_PARAM P);
			static void							EM_ClearIhmElements(void * const pO, void * const pR, void * const pP);
		#ifndef _MASTER
			static void							EM_DumpSceneGraph(void * const pO, void * const pR, void * const pP);
		#endif
			static void							EM_Pause(void * const pO, void * const pR, void * const pP);
			static void							EM_Resume(void * const pO, void * const pR, void * const pP);
			static void							EM_Construct(void * const pO, void * const pR, void * const pP);
			static void							EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void							EM_Loop(void * const pO, void * const pR, void * const pP);
			static void							EM_Display(void * const pO, void * const pR, void * const pP);
			static void							EM_AddIhmElementEx(void * const pO, void * const pR, void * const pP);
			static int							CM_AddIhmElementEx(COM_PARAM P);
			static void							EM_SaveState(void * const pO, void * const pR, void * const pP);
			static void							EM_RestoreState(void * const pO, void * const pR, void * const pP);
			static void							EM_ClearState(void * const pO, void * const pR, void * const pP);
			static void							EM_ResetEngine(void * const pO, void * const pR, void * const pP);
			static void							EM_SaveDebugPhysicsFile(void * const pO, void * const pR, void * const pP);
			static void							EM_AddVideoIhmElement(void * const pO, void * const pR, void * const pP);
			static void							EM_RemoveVideoIhmElement(void * const pO, void * const pR, void * const pP);
			static void							EM_ClearVideoIhmElement(void * const pO, void * const pR, void * const pP);
			static void							EM_AddFindAndReplace(void * const pO, void * const pR, void * const pP);
			static void							EM_ClearFindAndReplace(void * const pO, void * const pR, void * const pP);

			// For save state
			QDT_VECTOR <COM_INSTANCE>			_vNodes;
			QDT_VECTOR <MATRIX>					_vGlobalMatrices;
			QDT_VECTOR <COM_INSTANCE>			_vParents;
		};
	}
}

#include	INCL_KCOM(ComBasis/SingletonDescriptor)

#ifndef _DEBUG
#include "Moteur3D.inl"
#endif

#endif // __M3D_MOTEUR3D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
