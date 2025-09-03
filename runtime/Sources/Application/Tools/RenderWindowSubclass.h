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
//	CLASS:	RENDER_WINDOW_SUBCLASS
//	The RENDER_WINDOW_SUBCLASS class implements ...
//
//	04-01-19:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __RUNTIME_APPLICATION_TOOLS_RENDERWINDOWSUBCLASS_H__
#define __RUNTIME_APPLICATION_TOOLS_RENDERWINDOWSUBCLASS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifndef	_MASTER

#include	INC_MFC_WRAPPER(Toolkit/DynamicDialog)

namespace	QDT
{
	namespace	RUN
	{
		class	RENDER_WINDOW_SUBCLASS : public WND
		{
		public:

			RENDER_WINDOW_SUBCLASS(HWND HWnd);
			~RENDER_WINDOW_SUBCLASS();

		protected:

			BOOL	OnCommand(WPARAM	wParam, LPARAM	lParam);

			afx_msg	void	OnRButtonDown(UINT nFlags,CPoint point);
			
			DECLARE_MESSAGE_MAP()
			
			void	SpawnPopup();
			void	DumpWorldsAndSint();
			void	DumpStrings();
		};
	}
}

#endif	//_MASTER
#endif //__RUNTIME_APPLICATION_TOOLS_RENDERWINDOWSUBCLASS_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
