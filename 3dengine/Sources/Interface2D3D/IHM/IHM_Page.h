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
//	CLASS:	IHM_PAGE
//	The IHM_PAGE class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMPAGE_H__
#define __M3D_INTERFACE2D3D_IHM_IHMPAGE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(IHM_INTERFACE)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	IHM_PAGE : public COMMUNICATOR
		{
		public:

			enum
			{
				CMD_SET_ACTIVE_ELEMENTS_LIST_INDEX = CMD_COM_USER_START,
				CMD_SET_ACTIVE_ELEMENTS_LIST,
				CMD_GET_NBR_ELEMENTS_LISTS,
				CMD_GET_ACTIVE_ELEMENTS_LIST_INDEX,
				CMD_GET_ACTIVE_ELEMENTS_LIST,
				CMD_ADD_ELEMENTS_LIST,
				CMD_GET_ELEMENTS_LIST,
				CMD_IS_VISIBLE,
				CMD_SHOW,
				CMD_HIDE,
				CMD_DM_LAST
			};	
			
			IHM_PAGE();
			virtual    ~IHM_PAGE();

			//==== COMMON IHM FUNCTIONS =====

			/** Interface initialization. */
			void		Open(IHM_INTERFACE *pInterface);
			/** Interfaces pages management. */
			void		Handle(IHM_INTERFACE *pInterface, int nEvent);
			/** Time handling. */
			void		HandleTime(IHM_INTERFACE *pInterface, const float rFrameTimeMs);
			/** Handle mouse. */
			void		HandleMouse(IHM_INTERFACE *pInterface, int &nEvent, int nMousePosX, int nMousePosY);
			/** Close the interface. */
			void		Close(IHM_INTERFACE *pInterface);
			/** Display the interface. */
			void		Display(IHM_INTERFACE *pInterface);

			//====

			/** Retrieve the active elements list (NULL if none). */
			COM_INSTANCE		GetActiveElementsList() const;
			/** Get the index of the active elements list (-1 if none). */
			int					GetActiveElementsListIndex() const;
			/** Set the active elements list. */
			bool				SetActiveElementsList(const COM_INSTANCE &ElementList);
			/** Set the active elements list using its position in the '_ElementLists' vector (-1 for none). */
			bool				SetActiveElementsList(const int nElementsListIndex);
			/** Add an elements list. */
			void				AddElementsList(const COM_INSTANCE &ElementList);
			/** Remove an elements list from the page. */
			bool				RemoveElementsList(const COM_INSTANCE &ElementList);
			/** Get the number of elements lists. */
			int					GetNbrElementsLists() const;
			/** Get an elements list. */
			COM_INSTANCE		GetElementsList(const int nIndex) const;


			/** Returns 'true' if the page is visible. */
			bool				IsVisible();
			/** Show the page. */
			void				Show();
			/** Hide the page. */
			void				Hide();
			/** Remove the elements list of the current page. */
			void				Clear();

			/** Set the parent page of the current elements list. */
			void				SetParentContainer(IHM_INTERFACE * pParent);
			/** Get the parent page of the current elements list. */
			IHM_INTERFACE *		GetParentContainer() const;
			bool				HasParentContainer();
			
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			enum
			{
				IS_VISIBLE = (1 << 0),
				HAS_PARENT_CONTAINER = (1 << 1),
			};

			int								_nActiveElementsListIndex;
			QDT_VECTOR<COM_INSTANCE>		_ElementsLists;
			QDT_FLAGS						_Flags;
			IHM_INTERFACE *					_pParentContainer;

			static void			Init(CLASS_DESCRIPTOR * const pCD);
			
			static void			EM_Construct(void * const pO, void * const pR, void * const pP);
			static void			EM_Destruct(void * const pO, void * const pR, void * const pP);
			

			static void			EM_SetActiveElementsListIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_SetActiveElementsList(void * const pO, void * const pR, void * const pP);
			static void			EM_GetNbrElementsLists(void * const pO, void * const pR, void * const pP);
			static void			EM_GetActiveElementsListIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_GetActiveElementsList(void * const pO, void * const pR, void * const pP);
			static int			CM_AddElementsList(COM_PARAM P);
			static void			EM_AddElementsList(void * const pO, void * const pR, void * const pP);
			static void			EM_GetElementsList(void * const pO, void * const pR, void * const pP);
			static void			EM_IsVisible(void * const pO, void * const pR, void * const pP);
			static void			EM_Show(void * const pO, void * const pR, void * const pP);
			static int			CM_Show(COM_PARAM P);
			static void			EM_Hide(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMPAGE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
