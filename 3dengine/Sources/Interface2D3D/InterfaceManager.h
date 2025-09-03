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
//	CLASS:	INTERFACE_MANAGER
//	The INTERFACE_MANAGER class implements ...
//
//	01-09-24:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACEMANAGER_H__
#define __M3D_INTERFACEMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCOM(SINGLETON_DESCRIPTOR)

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KMATH(Math/Color/ColorFloat)
#include INCL_KSERV(Input/DeviceObserver)

namespace QDT
{
	namespace M3D
	{
		class	INTERFACE_MANAGER
		{
		public:

			enum
			{
				CMD_ENABLE_IHM_DEBUG_DISPLAY = CMD_USER_START,
				CMD_IS_IHM_DEBUG_DISPLAY_ENABLED,
				CMD_INTERFACE_MANAGER_LAST,
			};

			enum	EVENT
			{
				ON_UP		= (1 << 0),
				ON_DOWN		= (1 << 1),
				ON_LEFT		= (1 << 2),
				ON_RIGHT	= (1 << 3),
				ON_OK		= (1 << 4),
				ON_CANCEL	= (1 << 5),
				ON_LBUTTON	= (1 << 6),
			};

										INTERFACE_MANAGER();
										~INTERFACE_MANAGER();

			/** For singleton : Instance */
			static INTERFACE_MANAGER*	Instance();
			/** For singleton : Suicide */
			static void					Suicide();

			/** Update the time of every interface element. */
			void						Loop(float rTime);
			void						Update();
			void						Display();
			void						AddInterface(const COM_INSTANCE &Interface);
			void						RemoveInterface(const COM_INSTANCE &Interface);
			bool						IsInterfaceActivated(const COM_INSTANCE &Interface);
			void						SetMousePosX(int nX);
			void						SetMousePosY(int nY);
			int							GetMousePosX() const;
			int							GetMousePosY() const;
			void						SetInputValue(int nInputValue);
			void						GetWindowSize(int &nWidth, int &nHeight) const;
			int							GetWindowWidth() const;
			int							GetWindowHeight() const;
			void						AddAlwaysOnTopElement(const COM_INSTANCE &Element);

			// To protect observers.
			//INDEX_INT*					GetObjNotToDestroy(const unsigned int nObjectIndex) const;
			//unsigned int				GetNbrObjNotToDestroy() const;

			// Public for an easier access from the callback.
			bool						_bLButtonDown;

			// Debug display.
		#ifndef _MASTER
			void						EnableIHMDebugDisplay(const bool bEnable);
			bool						IsIHMDebugDisplayEnabled() const;
		#endif

			COLOR_FLOAT					GetIHMDebugDisplayColor() const;

			static SINGLETON_DESCRIPTOR *	GetSingletonDescriptor();
			static SINGLETON_DESCRIPTOR *	CreateSingletonDescriptor();

		protected:

			void		ClearAllInterfaces();

		private:

			void						HandleInterfaces(float rTime);
			void						DisplayInterfaces();

			QDT_VECTOR<COM_INSTANCE>	_vAlwaysOnTopElements;
			QDT_DLIST<COM_INSTANCE>		_lstInterfaces;
			DEVICE_OBSERVER				_MouseObserver;
			DEVICE_OBSERVER				_KeyboardObserver;
			int							_nMousePosX;
			int							_nMousePosY;
			int							_nInputValue;
			int							_nWindowWidth, _nWindowHeight;
			bool						_bIHMDebugDisplayEnable;
			
			DATABASE *					_pLocalDatabase;

			// Communication
			static void						COM_Init(CLASS_DESCRIPTOR * const pCD);
													
			static void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static void						EM_Destruct(void * const pO, void * const pR, void * const pP);
													
			// Commands								
		#ifndef _MASTER
			static void						EM_EnableIHMDebugDisplay(void * const pO, void * const pR, void * const pP);
			static void						EM_IsIHMDebugDisplayEnabled(void * const pO, void * const pR, void * const pP);
		#endif
		};
	}
}

#endif // __M3D_INTERFACEMANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
