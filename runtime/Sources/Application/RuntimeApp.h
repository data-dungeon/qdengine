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
//	CLASS:	RUNTIME_APP
//	The RUNTIME_APP class implements ...
//
//	01-09-17:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __RUNTIME_APPLICATION_RUNTIMEAPP_H__
#define __RUNTIME_APPLICATION_RUNTIMEAPP_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(ComBasis/Types)
#include	INCL_RUN(Application/RunTimeHeader)

#ifndef _MASTER
#include	INCL_RUN(Application/Tools/RenderWindowSubclass)
#endif	//_MASTER

namespace	QDT
{
	namespace	RUN
	{
		class	RUNTIME_APP : public WINDOW_APPLICATION
		{
		public:

										RUNTIME_APP();
			virtual					   ~RUNTIME_APP();

			static inline HWND			GetHWnd()	{ return RA._DisplayWindow; }

		protected:

			// Overridables
			virtual	BOOL				InitInstance();
					BOOL				ExitInstance();
					BOOL				OnIdle(long	lCount);
			virtual	int					Run();

					QDT_STRING			GetRuntimeVersionString() const;

					bool				InitOs();
					bool				InitIAMRuntime();

					void				ReadInifile();

					bool				ConnectToDB();

					void				CheckForAlternateIniFile();
					void				SetAlternateIniFileName(const QDT_STRING & IniFileName);
					const QDT_STRING &	GetAlternateIniFileName() const;

					void				AddAdditionnalBigfiles();
		private:

			static	int					LUA_Output(COM_PARAM P);
					int					LUA_RunFile(const QDT_STRING& sFilePath);

		private:
					HWND				_DisplayWindow;
					HINSTANCE			_AppInstance;

			static	RUNTIME_APP			RA;
												
					bool				_bQuit;
					RUNTIME_HEADER		_RuntimeHeader;
					
					QDT_STRING			_sAlternateIniFileName;
			void				CreateRenderWindow();

		#ifndef _MASTER
			RENDER_WINDOW_SUBCLASS	* _pWnd;
		#endif	//_MASTER
		};
	}
}


#endif //__RUNTIME_APPLICATION_RUNTIMEAPP_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
