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
//	CLASS:	IHM_ELEMENT
//	The IHM_ELEMENT class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTABC_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(IHM_ELEMENTS_LIST)
DECL_3DENGINE(IHM_ELEMENT_TEXT)
DECL_3DENGINE(IHM_INTERFACE)
DECL_3DENGINE(IHM_PAGE)
DECL_3DENGINE(VIEWPORT)
DECL_3DENGINE(INTERFACE_3D)

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KCOM(ComBasis/Types)

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENT_ABC : public COMMUNICATOR
		{
		public:
			enum
			{
				CMD_SET_OPENING_PAGE = CMD_COM_USER_START,
				CMD_ADD_LEFT_RIGHT_ELEMENT,
				CMD_GET_OPENING_PAGE,
				CMD_IS_VISIBLE,
				CMD_SHOW,
				CMD_HIDE,
				CMD_ON_SET_ACTIVE,
				CMD_ON_SET_INACTIVE,
				CMD_IS_ACTIVE,
				CMD_ON_MOUSE_IN,
				CMD_ON_MOUSE_OUT,
				CMD_SET_SCROLL_UP_BUTTON_FOR,
				CMD_SET_SCROLL_DOWN_BUTTON_FOR,
				CMD_SET_ALWAYS_INACTIVE,
				CMD_ON_MOUSE_CLICK,
				CMD_ON_MOUSE_MOVE,
				CMD_DISPLAY_ELEMENT,
				CMD_SET_ALWAYS_ON_TOP,
				CMD_SET_BLINKING,
				CMD_SET_BLINKING_TIME,
				CMD_RESIZE_ON_169,
				CMD_LAST
			};

							IHM_ELEMENT_ABC(CLASS_DESCRIPTOR * const pCD);
			virtual        ~IHM_ELEMENT_ABC();


			//==== COMMON IHM FUNCTIONS =====

			/** Interface initialization. */
			virtual	void		Open(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList) = 0;
			/** Interfaces pages management. */
					void		Handle(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList, int nEvent);
			/** Interfaces pages management. */
					void		HandleMouse(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList, int& nEvent, int nMousePosX, int nMousePosY);
			/** Time handling. */
					void		HandleTime(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList, const float rFrameTimeMs);
			/** Close the interface. */
			virtual	void		Close(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList) = 0;
			/** Display the element. */
					void		Display(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
			/** Add the elements to the list of elements that have to be displayed. */
			virtual	void		DisplayElement(VIEWPORT * pViewport) = 0;
			/** Debug display of the element. */
		#ifndef _MASTER
			virtual void		DebugDisplay(VIEWPORT * pViewport) = 0;
		#endif

			/** Set the element as an opening page element and set the page to be opened when the user activate the current element. */
			void				SetOpeningPage(IHM_PAGE * pOpeningPage);
			/** Set the element as an opening page element and set the page to be opened when the user activate the current element. The sound is the one played when the user activates the elements. */
			//void				SetOpeningPage(IHM_PAGE * pOpeningPage, SOUND_DATA *pOpeningPageSound);
			/** Set the element as a left-right element and add an element the user can parse trough using the left and right events. */
			void				AddLeftRightElement(IHM_ELEMENT_ABC * pLeftRightElement);
			/** Retrieve the selection page. */
			IHM_PAGE *			GetOpeningPage();
			/** If the element is an opening page element, set the sound that will be played when then user activates the current element. */
			//void				SetOpeningPageSound(SOUND_DATA *pSound);
			/** Returns true is the element is an opening page element. */
			bool				IsOpeningPageElement();
			/** Returns true is the element is a left-right element. */
			bool				IsLeftRightElement();
			/** Set the sound that has to be played when the user switches between elements. MUST BE CALLED WHEN THE ELEMENT HAS AT LEAST A LEFT-RIGHT ELEMENT. */
			//void				SetLeftRightSound(SOUND_DATA *pSound);

			/** Returns 'true' if the point whose coordinates are 'nPointX' and 'nPointY' is inside the element displayed on screen. */
			virtual bool		IsPointInElement(VIEWPORT * pViewport, int nPointX, int nPointY) = 0;
			/** If the element is an opning page one, play the sound associated with it (if any). */
			void				PlayOpeningPageSound();

			/** This function is called each time the current element is activated. */
			void				OnSetActive();
			/** This function is called each time the current element is disactivated. */
			void				OnSetInactive();
			/** Is the current element active? */
			bool				IsActive();
			/** If set to 'true', the element will receive no input at all. For example, use it to display bitmap on screen that should not diseappear when the user clicks on it. */
			void				SetAlwaysInactive(const bool bAlwaysInactive);
			/** Return 'true' if the element is always inactive. */
			bool				IsAlwaysInactive() const;

			/** If set to 'true', the element is displayed after the other ones. It is therefore always visible. */
			void				SetAlwaysOnTop(const bool bAlwaysOnTop);
			/** Return 'true' if the element is always on top. */
			bool				IsAlwaysOnTop() const;

			/** Make the current button the one that the user will use to scroll up the IHM_ELEMENT_TEXT. */
			void				SetScrollUpButtonFor(IHM_ELEMENT_TEXT * pTextElementInstance);
			/** Make the current button the one that the user will use to scroll down the IHM_ELEMENT_TEXT. */
			void				SetScrollDownButtonFor(IHM_ELEMENT_TEXT * pTextElementInstance);

			/** Allow scripters to personalize the element behaviour on a left mouse click. */
			void				OnMouseClick(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
			/** Allow scripters to personalize the element behaviour on a left mouse getting in element. */
			void				OnMouseIn(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
			/** Allow scripters to personalize the element behaviour on a left mouse getting out element. */
			void				OnMouseOut(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
			/** Allow scripters to personalize the element behaviour on mouse moving (WARNING: the event is catched even is the mouse isn't on the element, it's not MFC-like!). */
			void				OnMouseMove(int nMousePosX, int nMousePosY);

			/** Make the element blink. */
			void				SetBlinking(const bool bBlink);
			/** Is the element blinking? */
			bool				IsBlinking() const;
			/** Change the blinking time. */
			void				SetBlinkingTime(const float rBlinkTimeMS);
								
			virtual float		GetZDepth();
			virtual void		SetZDepth(const float rZ);
			virtual bool		IsUnder2DInterface() const;
			void				SetVerticesToReset(bool bValue);
		/*			
			// Set the viewport where the element is displayed.
			void				SetViewport(VIEWPORT * pViewport);
			// Get the viewport where the element is displayed.
			VIEWPORT *			GetViewport() const;

			bool				HasViewport() const;
		*/
			void				SetInterface3D(INTERFACE_3D *pInterface);
			INTERFACE_3D *		GetInterface3D() const;

			/** Get the INDEX_INT of the element. */
			//INDEX_INT*			GetIndexInt();
			/** Returns 'true' if the element is visible. */
			bool				IsVisible() const;
			/** Show the element. */
			void				Show();
			/** Hide the element. */
			void				Hide();

			void				SetWire(bool bValue);

			/** Returns 'true' if the type id is one of a element. */
			static bool			IsTypeElementId(OBJECT_TYPE TypeId);

			/** Set the parent elements list of the current element. */
			void				SetParentContainer(IHM_ELEMENTS_LIST * pParent);
			/** Get the parent elements list of the current element. */
			IHM_ELEMENTS_LIST *	GetParentContainer() const;
			/** Get the interface containing the element. */
			IHM_INTERFACE *		GetInterfaceContainer() const;
			bool				HasInterfaceContainer() const;

			bool				HasParentContainer() const;
			void				UnsetParentContainer();

			void				Set169Resizing(bool b);

			void				SetLoadingScreenIhm(bool b);
			bool				GetLoadingScreenIhm() const;

		//	virtual void		Display(DISPLAY_PARAMETERS& DisplayParameters, float rSizeX, float rSizeY) = 0;

			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		protected:

			/** Clear the flags OPENING_PAGE, LEFT_RIGHT_ELEMENT, etc..., but not IS_VISIBLE, IS_ACTIVE, not IS_MOUSE_ON. */
			void				ClearFlags();
			/** Check is the current element is valid. */
			bool				IsValid();

			enum
			{
				IS_VISIBLE				= (1 << 0),
				IS_ACTIVE				= (1 << 1),
				IS_MOUSE_ON				= (1 << 2),
				OPENING_PAGE_ELEMENT	= (1 << 3),
				LEFT_RIGHT_ELEMENT		= (1 << 4),
				SCROLLING_BUTTON_UP		= (1 << 5),
				SCROLLING_BUTTON_DOWN	= (1 << 6),
				ALWAYS_INACTIVE			= (1 << 7),
				ALWAYS_ON_TOP			= (1 << 8),
				BLINKING				= (1 << 9),
				HAS_VIEWPORT			= (1 << 10),
				HAS_PARENT_CONTAINER	= (1 << 11),
				RESIZE_ON_169			= (1 << 12),
				STATE_169				= (1 << 13),
				LOADING_SCREEN_IHM		= (1 << 14)
			};


			// For the blinking.
			float							_rBlinkingTime;
			float							_rCurBlinkTime;
			float							_rZDepth;


			// For an opening page element.
			IHM_PAGE *						_pOpeningPage;
			//SOUND_DATA *					_pOpeningPageSound;
			// For a left-right element.
			//SOUND_DATA *					_pLeftRightSound;
			int								_nVisibleLeftRightElement;
			QDT_VECTOR<IHM_ELEMENT_ABC *>	_LeftRightElements;
			// Scrolling element.
			IHM_ELEMENT_TEXT *				_pScrollingTextElement;
			// Generic member variables.
			QDT_FLAGS						_Flags;
			// Parent elements list.
			IHM_ELEMENTS_LIST *				_pParentContainer;

		//	VIEWPORT *						_pViewport;
			INTERFACE_3D *					_pInterface3D;

			bool							_bVerticesToReset;
			bool							_bWired;

		private:

			/** Shall the element be displayed if blinking? */
			bool				HasToBeDisplayed() const;

			static void			Init(CLASS_DESCRIPTOR * const pCD);
			
			static void			EM_SetOpeningPage(void * const pO, void * const pR, void * const pP);
			static void			EM_AddLeftRightElement(void * const pO, void * const pR, void * const pP);
			static void			EM_GetOpeningPage(void * const pO, void * const pR, void * const pP);
			static void			EM_IsVisible(void * const pO, void * const pR, void * const pP);
			static void			EM_Show(void * const pO, void * const pR, void * const pP);
			static void			EM_Hide(void * const pO, void * const pR, void * const pP);
			static void			EM_OnSetActive(void * const pO, void * const pR, void * const pP);
			static void			EM_OnSetInactive(void * const pO, void * const pR, void * const pP);
			static void			EM_IsActive(void * const pO, void * const pR, void * const pP);
			static void			EM_OnMouseIn(void * const pO, void * const pR, void * const pP);
			static void			EM_OnMouseOut(void * const pO, void * const pR, void * const pP);
			static void			EM_SetScrollUpButtonFor(void * const pO, void * const pR, void * const pP);
			static void			EM_SetScrollDownButtonFor(void * const pO, void * const pR, void * const pP);
			static void			EM_SetAlwaysInactive(void * const pO, void * const pR, void * const pP);
			static void			EM_OnMouseClick(void * const pO, void * const pR, void * const pP);
			static void			EM_OnMouseMove(void * const pO, void * const pR, void * const pP);
			static void			EM_DisplayElement(void * const pO, void * const pR, void * const pP);
			static void			EM_SetAlwaysOnTop(void * const pO, void * const pR, void * const pP);
			static void			EM_SetBlinking(void * const pO, void * const pR, void * const pP);
			static void			EM_SetBlinkingTime(void * const pO, void * const pR, void * const pP);
			static void			EM_ResizeOn169(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
