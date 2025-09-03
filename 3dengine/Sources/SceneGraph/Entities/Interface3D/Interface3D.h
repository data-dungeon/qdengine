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
//	CLASS:	INTERFACE_3D
//	The INTERFACE_3D class implements ...
//
//	02-12-04:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_INTERFACE3D_H__
#define __M3D_SCENEGRAPH_ENTITIES_INTERFACE3D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)

namespace QDT
{
	namespace M3D
	{
		class	INTERFACE_3D : public ENTITY_ABC
		{

		public:

			enum
			{
				CMD_INT3D_GET_SIZE_X = CMD_ENTITY_LAST,
				CMD_INT3D_GET_SIZE_Y,
				CMD_INT3D_SET_SIZE_X,
				CMD_INT3D_SET_SIZE_Y,
				CMD_INT3D_ADD_IHM_ELEMENT,
				CMD_INT3D_REMOVE_IHM_ELEMENT,
				CMD_INT3D_SHOW,
				CMD_INT3D_LAST
			};

												INTERFACE_3D();
											~INTERFACE_3D();

			void								SetSizeX(float r);
			void								SetSizeY(float r);
			
			float								GetSizeX() const		{ return (_rSizeX); }
			float								GetSizeY() const		{ return (_rSizeY); }

			void								Show(bool b);
			void								Copy(const INTERFACE_3D & Interface);

			void								AddIhmElement(const COM_INSTANCE& Elt);
			void								RemoveIhmElement(const COM_INSTANCE& Elt);

			// ENTITY_ABC
			virtual void						Display(DISPLAY_PARAMETERS & DisplayParameters);
			virtual bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetSizeX(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetSizeY(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetSizeX(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetSizeY(void * const pO, void * const pR, void * const pP);
			static  void						EM_AddIhmElement(void * const pO, void * const pR, void * const pP);
			static  void						EM_RemoveIhmElement(void * const pO, void * const pR, void * const pP);
			static  void						EM_Show(void * const pO, void * const pR, void * const pP);

			void								UpdateBoundingBox();

			bool								_bHide;

			float								_rSizeX;
			float								_rSizeY;

			OBB									_BoundingBox;

			QDT_DLIST<COM_INSTANCE>				_IhmElements;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_INTERFACE3D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
