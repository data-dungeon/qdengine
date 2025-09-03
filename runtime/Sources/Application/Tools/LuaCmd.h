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
//	CLASS:	LUA_CMD
//
//	05-12-06:	NHO - Created
//*****************************************************************************

#ifndef __RUNTIME_APPLICATION_TOOLS_LUACMD_H__
#define __RUNTIME_APPLICATION_TOOLS_LUACMD_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(Tools/Qdt_List)
#include INCL_KCORE(Template/Singleton)
#include INCL_KSERV(LuaRemoteService/LuaRemoteService)
#include INCL_KSERV(LuaExecService/LuaExecService)

namespace QDT
{
	namespace RUN
	{
		class LUA_CMD
		{
			// TEST_SERVICE le service doit pourvoir accéder LUA_RunFile
			// en attendant mieux : une fonction permet de piloter de manière externe.
			//friend class LUA_REMOTE_SERVICE;   //@gritodo virer çà.
			class LUA_COMMAND : public QDT_LIST_ELEMENT
			{
			public:
				QDT_STRING		_sEditText;
				QDT_STRING		_sOutputText;
			};

			public:
											LUA_CMD(HWND hParent);
				virtual 				~LUA_CMD();
				static void			ToggleDlg(HWND hParent);
				static void			AddOutput(const char *strOutput);
				void						UpdateRemoteLUA();

				//static LUA_CMD*	GetInstance() { return _pInstance; }

			protected:
				void						Destroy(void);
				void						SetEdit(const QDT_STRING& sText);
				void						SetOutput(const QDT_STRING& sText);
				void						ClearEdit();
				void						ClearOutput();
				void						Output(const QDT_STRING& sText);
						LRESULT				WrpWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
						LRESULT				WrpEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
				static	LRESULT CALLBACK	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
				static	LRESULT CALLBACK	EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
				void						LUA_ProcessCmd(const QDT_STRING& strCmd);
				int							LUA_RunFile(const QDT_STRING& sFilePath,bool bClient = false);
				int							LUA_RunFileRemote(const QDT_STRING& sFilePath);

				static	LUA_CMD*		_pInstance;
				HINSTANCE		_hInst;
				HFONT			_hWndFont;
				RECT			_WndRect;
				HWND			_hWnd;
				HWND			_hWndEdit;
				HWND			_hWndOutput;
				HWND			_hParent;
				LONG			_nDefEditProc;
				QDT_LIST		_CommandList;
				Int32			_nCommandListIndex;
		};
	}
}

//TEST_SERVICE ajout d'un accesseur global
//inline
//LUA_CMD* GetLUACommand() { return QDT::RUN::LUA_CMD::GetInstance();}

#endif //__RUNTIME_APPLICATION_TOOLS_LUACMD_H__