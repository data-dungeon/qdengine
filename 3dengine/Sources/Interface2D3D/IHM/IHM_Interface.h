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
//	CLASS:	IHM_INTERFACE
//	The IHM_INTERFACE class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMINTERFACE_H__
#define __M3D_INTERFACE2D3D_IHM_IHMINTERFACE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(VIEWPORT)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	IHM_INTERFACE : public COMMUNICATOR
		{
		public:

			enum
			{
				CMD_ADD_PAGE = CMD_COM_USER_START,
				CMD_SET_ACTIVE_PAGE_INDEX,
				CMD_SET_ACTIVE_PAGE,
				CMD_GET_NBR_PAGES,
				CMD_GET_ACTIVE_PAGE_INDEX,
				CMD_GET_ACTIVE_PAGE,
				CMD_DISPLAY,
				CMD_ACTIVATE,
				CMD_DISACTIVATE,
				CMD_CLEAR_ALL,
				CMD_IS_ACTIVATED,
				CMD_IS_VISIBLE,
				CMD_SHOW,
				CMD_HIDE,
				CMD_ACTIVATE_INPUT,
				CMD_ARE_INPUT_ACTIVATED,
				CMD_SET_VIEWPORT,
				CMD_COPY,
				CMD_SET_BACKGROUND_MATERIAL,
				CMD_DM_LAST
			};	

						IHM_INTERFACE();
			virtual    ~IHM_INTERFACE();


			//==== COMMON IHM FUNCTIONS =====

			/** Interface initialization. */
			void		Open();
			/** Interfaces pages management. */
			void		Handle(int nEvent);
			/** Time management of the interface. */
			void		HandleTime(const float rFrameTimeMs);
			/** Handle mouse events. */
			void		HandleMouse(int &nEvent, int nMousePosX, int nMousePosY);
			/** Close the interface. */
			void		Close();
			/** Display the interface. */
			void		Display();

			/** Get the index of the active page (-1 if none). */
			int			GetActivePageIndex() const;
			/** Retrieve the active page (NULL if none). */
			COM_INSTANCE GetActivePage() const;
			/** Set the active page (NULL if none). */
			bool		SetActivePage(const COM_INSTANCE &Page);
			/** Set the active page using its position in the '_pPages' vector. */
			bool		SetActivePage(const int nPageIndex);
			/** Add a page in the interface. */
			void		AddPage(const COM_INSTANCE &PageToAdd);
			/** Remove a page from the interface. */
			bool		RemovePage(const COM_INSTANCE &Page);
			/** Returns the number of pages in the interface. */
			int			GetNbrPages();
			/** Add the interface into the interface manager. */
			void		Activate();
			/** Remove the interface into the interface manager. */
			void		Disactivate();
			/** Returns true if the interface is activated (in the interface manager). */
			bool		IsActivated();
			/** Get a page. */
			COM_INSTANCE	GetPage(int nPageIndex) const;
			/** Remove all objects in the interface. */
			void		ClearAll();

			/** Set a background material in case you want one to be displayed. */
			void		SetBackgroundMaterial(const COM_INSTANCE &	MaterialInst);
			
			/** Returns 'true' if the elements list is visible. */
			bool		IsVisible();
			/** Show the elements list. */
			void		Show();
			/** Hide the elements list. */
			void		Hide();

			/** Active inputs. If set to false, no inputs are handled. By default, inputs are activated. */
			void		ActivateInput(const bool bActivate);
			/** Returns true if the inputs are activated on the current interface. */
			bool		AreInputActivated() const;

			/** Set the viewport where the interface and all of its elements are displayed. */
			void		SetViewport(VIEWPORT * pViewport);
			/** Get the viewport where the interface and all of its elements are displayed. */
			VIEWPORT *	GetViewport() const;

			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			void		DisplayBackgroundPicture(VIEWPORT *	pViewport);


			friend class IHM_INTERFACE_DESC;

			enum
			{
				IS_VISIBLE		= 1 << 0,
				INPUT_ACTIVATED	= 1 << 1,
			};


			QDT_VECTOR<COM_INSTANCE> _Pages;
			int						_nActivePageIndex;
			QDT_FLAGS				_Flags;
			VIEWPORT *				_pViewport;


			COM_INSTANCE			_BackGrndIhmElmtQuad;

		//	INDEX_INT*				_pBackgroundMaterialIndexInt;
		//	int						_nBackgroundMaterialId;

			static void			Init(CLASS_DESCRIPTOR * const pCD);
			
			static void			EM_Construct(void * const pO, void * const pR, void * const pP);
			static void			EM_Destruct(void * const pO, void * const pR, void * const pP);
			
			static int			CM_AddPage(COM_PARAM P);
			static int			CM_Activate(COM_PARAM P);

			static void			EM_AddPage(void * const pO, void * const pR, void * const pP);
			static void			EM_SetActivePageIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_SetActivePage(void * const pO, void * const pR, void * const pP);
			static void			EM_GetNbrPages(void * const pO, void * const pR, void * const pP);
			static void			EM_GetActivePageIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_GetActivePage(void * const pO, void * const pR, void * const pP);
			static void			EM_Display(void * const pO, void * const pR, void * const pP);
			static void			EM_Activate(void * const pO, void * const pR, void * const pP);
			static void			EM_Disactivate(void * const pO, void * const pR, void * const pP);
			static void			EM_ClearAll(void * const pO, void * const pR, void * const pP);
			static void			EM_IsActivated(void * const pO, void * const pR, void * const pP);
			static void			EM_SetBackgroundMaterial(void * const pO, void * const pR, void * const pP);
			static void			EM_IsVisible(void * const pO, void * const pR, void * const pP);
			static void			EM_Show(void * const pO, void * const pR, void * const pP);
			static void			EM_Hide(void * const pO, void * const pR, void * const pP);
			static void			EM_ActivateInput(void * const pO, void * const pR, void * const pP);
			static void			EM_AreInputActivated(void * const pO, void * const pR, void * const pP);
			static void			EM_SetViewport(void * const pO, void * const pR, void * const pP);
			static void			EM_Copy(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMINTERFACE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
