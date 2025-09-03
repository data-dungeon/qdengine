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
//	CLASS:	IHM_ELEMENTS_LIST
//	The IHM_ELEMENTS_LIST class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTSLIST_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTSLIST_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/Communicator)

DECL_3DENGINE(IHM_INTERFACE);
DECL_3DENGINE(IHM_PAGE);

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENTS_LIST : public COMMUNICATOR
		{
		public:

			enum
			{
				CMD_GET_NBR_ELEMENTS = CMD_COM_USER_START,
				CMD_GET_ELEMENT,
				CMD_GET_ACTIVE_ELEMENT,
				CMD_GET_ACTIVE_ELEMENT_INDEX,
				CMD_SET_ACTIVE_ELEMENT,
				CMD_SET_ACTIVE_ELEMENT_INDEX,
				CMD_ADD_ELEMENT,
				CMD_SET_ELEMENT_OPENING_PAGE,
				CMD_IS_VISIBLE,
				CMD_SHOW,
				CMD_HIDE,
				CMD_SWAP_ELEMENTS,
				CMD_REMOVE_ELEMENT_INDEX,
				CMD_REMOVE_ELEMENT,
				//CMD_COPY,
				CMD_DM_LAST
			};	


					IHM_ELEMENTS_LIST();
			virtual ~IHM_ELEMENTS_LIST();


			//==== COMMON IHM FUNCTIONS =====

			/** Interface initialization. */
			void		Open(IHM_INTERFACE *pInterface, IHM_PAGE *pPage);
			/** Interfaces pages management. */
			void		Handle(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, int nEvent);
			/** handle mouse. */
			void		HandleMouse(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, int &nEvent, int nMousePosX, int nMousePosY);
			/** Time handling. */
			void		HandleTime(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, const float rFrameTimeMs);
			/** Close the interface. */
			void		Close(IHM_INTERFACE *pInterface, IHM_PAGE *pPage);
			/** Display the interface. */
			void		Display(IHM_INTERFACE *pInterface, IHM_PAGE *pPage);

			/** Returns the numbers of elements in the elements list. */
			int					GetNbrElements() const;
			/** Retrieve an element of the elements list. */
			COM_INSTANCE		GetElement(const int nElementIndex) const;
			/** Retrieve the active element. */
			COM_INSTANCE		GetActiveElement() const;
			/** Retrieve the index of the active element (-1 if none). */
			int					GetActiveElementIndex() const;
			/** Set the active element. */
			bool				SetActiveElement(const COM_INSTANCE &Element);
			/** Set the active element using its position in the 'Elements' vector. */
			bool				SetActiveElement(const int nElementIndex);
			/** Add an element. */
			void				AddElement(const COM_INSTANCE &Element);
			/** Remove an element from the current elements list. */
			bool				RemoveElement(const int nElementIndex);
			/** Remove an element from the current elements list. */
			bool				RemoveElement(const COM_INSTANCE &Element);
			/** Set the opening page of an element of the current elements list. */
			void				SetElementOpeningPage(int nElementIndex, IHM_PAGE * pOpeningPage);
			/** Set the sound that has to be played when the user switches between elements. MUST BE CALLED WHEN THE ELEMENTS LIST HAS AT LEAST A LEFT-RIGHT ELEMENT. */
			//void				SetSwitchingSound(SOUND_DATA *pSound);
			/** Get the switching sound. */
			//SOUND_DATA*			GetSwitchingSound() const;
			/** Set the default sound that is played when an action occurs. */
			//void				SetActionSound(SOUND_DATA *pSound);
			/** Get the action sound. */
			//SOUND_DATA*			GetActionSound() const;
			/** Swap two elements in the elements list. Use this if you want to change the display order of the element of the elements list. */
			void				SwapElements(const int n1stElementIndex, const int n2ndElementIndex);

			/** Set the parent page of the current elements list. */
			void				SetParentContainer(IHM_PAGE * pParent);
			/** Get the parent page of the current elements list. */
			IHM_PAGE *			GetParentContainer() const;
			bool				HasParentContainer();

			/** Returns 'true' if the elements list is visible. */
			bool				IsVisible();
			/** Show the elements list. */
			void				Show();
			/** Hide the elements list. */
			void				Hide();
			/** Clear the elements in the elements list. */
			void				Clear();
				
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			enum
			{
				IS_VISIBLE = 0x00000001,
			};

			//SOUND_DATA *				_pSwitchingSound;
			//SOUND_DATA *				_pActionSound;
			int							_nActiveElementIndex;
			QDT_VECTOR<COM_INSTANCE>	_Elements;
			QDT_FLAGS					_Flags;
			IHM_PAGE *	 				_pParentContainer;
			bool						_bHasParentContainer;

			static void			Init(CLASS_DESCRIPTOR * const pCD);
			
			static void			EM_Construct(void * const pO, void * const pR, void * const pP);
			static void			EM_Destruct(void * const pO, void * const pR, void * const pP);
			
			static int			CM_GetNbrElements(COM_PARAM P);
			static int			CM_AddElement(COM_PARAM P);

			static void			EM_GetNbrElements(void * const pO, void * const pR, void * const pP);
			static void			EM_GetElement(void * const pO, void * const pR, void * const pP);
			static void			EM_GetActiveElement(void * const pO, void * const pR, void * const pP);
			static void			EM_GetActiveElementIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_SetActiveElement(void * const pO, void * const pR, void * const pP);
			static void			EM_SetActiveElementIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_AddElement(void * const pO, void * const pR, void * const pP);
			static void			EM_SetElementOpeningPage(void * const pO, void * const pR, void * const pP);
			static void			EM_IsVisible(void * const pO, void * const pR, void * const pP);
			static void			EM_Show(void * const pO, void * const pR, void * const pP);
			static void			EM_Hide(void * const pO, void * const pR, void * const pP);
			static void			EM_SwapElements(void * const pO, void * const pR, void * const pP);
			static void			EM_RemoveElementIndex(void * const pO, void * const pR, void * const pP);
			static void			EM_RemoveElement(void * const pO, void * const pR, void * const pP);
			static void			EM_Copy(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTSLIST_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
