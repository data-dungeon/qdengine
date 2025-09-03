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
//	CLASS:	VIEW_PORT_LIST
//	The VIEW_PORT_LIST class implements ...
//
//	02-06-06:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_VIEWPORT_VIEWPORTLIST_H__
#define __DISPLAY_VIEWPORT_VIEWPORTLIST_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(VIEWPORT)
DECL_3DENGINE(RAY_CAST_PARAMETERS)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	VIEWPORT_LIST : public COMMUNICATOR
		{
		public:

			// Constructor & destructor
					VIEWPORT_LIST();
					VIEWPORT_LIST(const VIEWPORT_LIST &C);
				~VIEWPORT_LIST();

			// Operators
			VIEWPORT_LIST & operator = (const VIEWPORT_LIST & C);

			// Display
			void	Display() const;

			// List Management
			void	PushOnTop(const COM_INSTANCE &ViewportInst);
			bool	Remove(const COM_INSTANCE &ViewportInst);
			bool	InsertBefore(const COM_INSTANCE &ViewportToInsert, const COM_INSTANCE &ViewportInList);
			int		GetNbrViewports() const;
			void	Clear();

			VIEWPORT * GetViewport(int iViewport);

			// Ray cast
			void	RayCast(int	nScreenX, int nScreenY, RAY_CAST_PARAMETERS &	RayCastParameters);

			static void							Init(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();

			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

		private:

			void	Copy(const VIEWPORT_LIST &C);

			QDT_DLIST <COM_INSTANCE>	_dlViewports;
		};
	}
}

#endif // __DISPLAY_VIEWPORT_VIEWPORTLIST_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
