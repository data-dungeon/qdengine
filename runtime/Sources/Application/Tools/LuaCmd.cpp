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

#include	"Root.h"
#include	INCL_RUN(Application/Tools/LuaCmd)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)
#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(File/FilesManagerABC)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(File/LoadingStack)
#include	INCL_KSERV(KernelObject/Kernel)

#define 	WND_SIZE_X				640
#define 	WND_SIZE_Y				240
#define 	FONT_HEIGHT				10
#define 	FONT_FACE				"Lucida Console"
#define 	EDIT_MAXLENGTH			1024
#define		OUTPUT_MAXLENGTH		EDIT_MAXLENGTH*2

#define 	EDIT_CONTROLS			101
#define 	STATIC_CONTROLS			102

#define		COMMANDSTACK_LENGTH		10

#define TEST_SERVICES

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

LUA_CMD*	LUA_CMD::_pInstance		= NULL;

//-----------------------------------------------------------------------------
//	Name:		LUA_CMD Constructor
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
LUA_CMD::LUA_CMD(HWND hParent)
{
	OPTIONAL_FUNC_CONTAINER::_pOutputFunc = (OPTIONAL_FUNC_CONTAINER::OutputFunc)AddOutput;

	LOGFONT dFont;
	dFont.lfHeight				= FONT_HEIGHT;
	dFont.lfWidth					= 0;
	dFont.lfEscapement		= 0;
	dFont.lfOrientation		= 0;
	dFont.lfWeight				= FW_NORMAL;
	dFont.lfItalic				= FALSE;
	dFont.lfUnderline			= FALSE;
	dFont.lfStrikeOut			= FALSE;
	dFont.lfCharSet				= ANSI_CHARSET;
	dFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	dFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	dFont.lfQuality				= DEFAULT_QUALITY;
	dFont.lfPitchAndFamily		= DEFAULT_PITCH|FF_DONTCARE;
	strcpy(dFont.lfFaceName, FONT_FACE);
	_hWndFont = CreateFontIndirect(&dFont);

	RECT wndrect;
	wndrect.top					= 0;
	wndrect.left				= 0;
	wndrect.bottom				= WND_SIZE_Y;
	wndrect.right				= WND_SIZE_X;
	AdjustWindowRect(&wndrect, WS_VISIBLE|WS_OVERLAPPED|WS_SYSMENU, FALSE);

	WNDCLASSEX WndClassEx;
	WndClassEx.cbSize			= sizeof(WNDCLASSEX);
	WndClassEx.style			= 0;
	WndClassEx.lpfnWndProc		= WndProc;
	WndClassEx.cbClsExtra		= 0;
	WndClassEx.cbWndExtra		= 0;
	WndClassEx.hInstance		= _hInst;
	WndClassEx.hIcon			= 0;
	WndClassEx.hCursor			= 0;
	WndClassEx.hbrBackground	= (HBRUSH)GetStockObject( GRAY_BRUSH );
	WndClassEx.lpszMenuName		= 0;
	WndClassEx.lpszClassName	= "CRuntimeLUACmd";
	WndClassEx.hIconSm			= 0;
	RegisterClassEx( &WndClassEx );

	RECT FatherRect;
	if (hParent!=NULL)
	{
		GetWindowRect(hParent, &FatherRect);
	}
	else
	{
		memset(&FatherRect, 0, sizeof(RECT));
	}

	_hWnd = CreateWindowEx(		WS_EX_TOOLWINDOW|WS_EX_WINDOWEDGE, 
								"CRuntimeLUACmd", 
								"LUA Runtime command line",
								WS_VISIBLE|WS_OVERLAPPED|WS_SYSMENU,
								FatherRect.left+20, FatherRect.top+20,
								wndrect.right-wndrect.left, wndrect.bottom-wndrect.top,
								hParent, NULL, NULL, NULL );

	GetClientRect(_hWnd, &wndrect);

	SetWindowLong( _hWnd, GWL_USERDATA,	(long)this );

	_hWndEdit = CreateWindowEx(		WS_EX_STATICEDGE, 
									"EDIT",
									"LUA Runtime command line : Edit",
									ES_AUTOHSCROLL|ES_AUTOVSCROLL|WS_VISIBLE|WS_CHILDWINDOW|ES_LEFT|ES_MULTILINE|WS_VSCROLL,
									0, 0, 
									wndrect.right-wndrect.left, (wndrect.bottom-wndrect.top)/3,
									_hWnd, (HMENU)EDIT_CONTROLS, NULL, NULL );

	SetWindowLong( _hWndEdit, GWL_USERDATA,	(long)this );
	_nDefEditProc = GetWindowLong( _hWndEdit, GWL_WNDPROC );
	SetWindowLong( _hWndEdit, GWL_WNDPROC, (LONG)EditProc);

	SendMessage( _hWndEdit, EM_LIMITTEXT, EDIT_MAXLENGTH, 0 );
	SendMessage( _hWndEdit, WM_SETFONT, (WPARAM)_hWndFont, (LPARAM)TRUE );
	SendMessage( _hWndEdit, WM_SETTEXT, 0, (LPARAM)"" );

	_hWndOutput = CreateWindowEx(	WS_EX_STATICEDGE, 
									"EDIT",
									"LUA Runtime command line : Output",
									ES_AUTOHSCROLL|ES_AUTOVSCROLL|WS_VISIBLE|WS_CHILDWINDOW|ES_LEFT|ES_MULTILINE|WS_VSCROLL|ES_READONLY,
									0, (wndrect.bottom-wndrect.top)/3, 
									wndrect.right-wndrect.left, 2*(wndrect.bottom-wndrect.top)/3,
									_hWnd, (HMENU)EDIT_CONTROLS, NULL, NULL );
	
	SendMessage( _hWndOutput, EM_LIMITTEXT, OUTPUT_MAXLENGTH, 0 );
	SendMessage( _hWndOutput, WM_SETFONT, (WPARAM)_hWndFont, (LPARAM)TRUE );
	SendMessage( _hWndOutput, WM_SETTEXT, 0, (LPARAM)"" );

	UpdateWindow( _hWnd );
	ShowWindow( _hWnd, SW_SHOW );

	_nCommandListIndex = -1;
}

//-----------------------------------------------------------------------------
//	Name:		Destructor
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
LUA_CMD::~LUA_CMD()
{
	_pInstance = NULL;
	DestroyWindow( _hWnd );
	OPTIONAL_FUNC_CONTAINER::_pOutputFunc = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		WndProc
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
LRESULT CALLBACK LUA_CMD::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LUA_CMD	*pInst = (LUA_CMD*)GetWindowLong(hWnd, GWL_USERDATA);
	if(pInst)
	{
		return pInst->WrpWndProc(hWnd, msg, wParam, lParam);
	}

	// 
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
//	Name:		WrpWndProc
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
LRESULT LUA_CMD::WrpWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		delete this;
		break;
	}

	// zzz : ici anciennement init des qdt services

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
//	Name:		EditProc
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
LRESULT CALLBACK LUA_CMD::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LUA_CMD	*pInst = (LUA_CMD*)GetWindowLong(hWnd, GWL_USERDATA);
	if (pInst)
	{
		return pInst->WrpEditProc(hWnd, msg, wParam, lParam);
	}

	return -1;
}

//-----------------------------------------------------------------------------
//	Name:		WrpEditProc
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
LRESULT LUA_CMD::WrpEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		if (((int)wParam == VK_RETURN) 
			&& ((GetKeyState(VK_LCONTROL) & 0x8000) || (GetKeyState(VK_RCONTROL) & 0x8000)))
		{
			char sEditText[EDIT_MAXLENGTH];
			GetWindowText(_hWndEdit, sEditText, EDIT_MAXLENGTH);

			QDT_STRING sCmdLine=sEditText;

			LUA_COMMAND* pCmd;

			if (_CommandList.GetNbElements() >= COMMANDSTACK_LENGTH)
			{
				pCmd = (LUA_COMMAND*)_CommandList.GetQueue();
				_CommandList.DeleteElement(pCmd);
			}

			pCmd = new LUA_COMMAND;
			pCmd->_sEditText = sCmdLine;

			_nCommandListIndex = -1;

			ClearEdit();
			ClearOutput();

			if (sCmdLine.Find("run:")==0)
			{
				QDT_STRING sFileName = sCmdLine.EraseLeft(4);
				sFileName.TrimRight();
				int nValue = LUA_RunFile(sFileName);

				// Test du service du RemoteFile : si on ne trouve pas on cherche à rapatrier.
#ifdef TEST_SERVICES
				if (nValue == -1)
				{
					//FILE_ABC*	Open(const char * const pFileName, const FILE_ABC::F_MODE eMode, FM_ERROR &eError, const QDT_FLAGS&	rFlags = 0);
					//("SaveGame.sav", FILE_ABC::FM_CREATE_READ_WRITE, eError )
					nValue = LUA_RunFileRemote(sFileName);
				}
#endif 
			}
			else
			{
				LUA_ProcessCmd(sCmdLine);
			}

			char sOutputText[OUTPUT_MAXLENGTH];
			GetWindowText(_hWndOutput, sOutputText, OUTPUT_MAXLENGTH);

			QDT_STRING sOutput=sOutputText;

			pCmd->_sOutputText = sOutput;
			_CommandList.InsertHead(pCmd);

			return 0;
		}

		if ((((int)wParam == VK_UP) || ((int)wParam == VK_DOWN)) 
			&& ((GetKeyState(VK_LCONTROL) & 0x8000) || (GetKeyState(VK_RCONTROL) & 0x8000)))
		{
			if (_CommandList.GetNbElements()>0)
			{
				if (((int)wParam == VK_UP))
				{
					++_nCommandListIndex;
					if (_nCommandListIndex >= _CommandList.GetNbElements())
					{
						_nCommandListIndex = 0;
					}
				}
				else
				{
					--_nCommandListIndex;
					if (_nCommandListIndex<0)
					{
						_nCommandListIndex = _CommandList.GetNbElements()-1;
					}
				}

				LUA_COMMAND* pCmd = (LUA_COMMAND*)_CommandList.GetIndex(_nCommandListIndex);
				SetEdit(pCmd->_sEditText);
				SetOutput(pCmd->_sOutputText);
			}
			else
			{
				SetOutput("No command recorded.");
			}
		}

 		break;
	}

	return CallWindowProc((WNDPROC)_nDefEditProc, hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
//	Name:		LUA_ProcessCmd
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::LUA_ProcessCmd(const QDT_STRING& strCmd)
{

	if (strCmd.IsEmpty()==false)
	{
		SYSTEMTIME SystemTime;
		GetSystemTime(&SystemTime);

		QDT_STRING sOutput;
		sOutput.Format("[%04d/%02d/%02d-%02d:%02d:%02d] Running LUA commands:", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,  SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
		Output(sOutput);
		Output(strCmd);
		Output("");

		LUA_EXEC_SERVICE* pService = KERNEL::Instance()->GetLuaExecService();
		if (pService)
		{
			// si on se considere comme serveur, on va envoyer cette commande
			pService ->LUA_ExecCMD(strCmd);
		}

		COM_STACK Stack;
		if (Stack.RunScript(strCmd) == 0)
		{
			Output("No error");
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Output
//	Object:		
//	05-12-06:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::Output(const QDT_STRING& sText)
{
	char sOutput[OUTPUT_MAXLENGTH];
	GetWindowText(_hWndOutput, sOutput, OUTPUT_MAXLENGTH);

	QDT_STRING sOutputText(sOutput);

	int nEndSize = sOutputText.GetSize()+sText.GetSize()+2;
	if (nEndSize > OUTPUT_MAXLENGTH)
	{
		sOutputText.EraseLeft(nEndSize-OUTPUT_MAXLENGTH);
	}

	sOutputText += sText;
	sOutputText += "\r\n";

	SetWindowText(_hWndOutput, sOutputText.GetFullBufferPtr());
}

//-----------------------------------------------------------------------------
//	Name:		ToggleDlg
//	Object:		
//	05-12-07:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::ToggleDlg(HWND hParent)
{
	if (_pInstance)
	{
		delete _pInstance;
		_pInstance = NULL;
	}
	else
	{
		_pInstance = new LUA_CMD(hParent);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddOutput
//	Object:		
//	05-12-19:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::AddOutput(const char* strOutput)
{
	QDT_STRING sOutput(strOutput);
	if (_pInstance)
	{
		_pInstance->Output(sOutput);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ClearOutput
//	Object:		
//	05-12-21	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::ClearOutput()
{
	SetOutput("");
}

//-----------------------------------------------------------------------------
//	Name:		ClearEdit
//	Object:		
//	05-12-21:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::ClearEdit()
{
	SetEdit("");
}

//-----------------------------------------------------------------------------
//	Name:		SetEdit
//	Object:		
//	05-12-21:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::SetEdit(const QDT_STRING& sText)
{
	SetWindowText(_hWndEdit, sText);
}

//-----------------------------------------------------------------------------
//	Name:		SetOutput
//	Object:		
//	05-12-21:	NHO - Created
//-----------------------------------------------------------------------------
void LUA_CMD::SetOutput(const QDT_STRING& sText)
{
	SetWindowText(_hWndOutput, sText);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateRemoteLUA
//	Object:		
//	06-02-16:	GRI - Created
//-----------------------------------------------------------------------------
void	LUA_CMD::UpdateRemoteLUA()		// TEST_SERVICE
{
	//if (!_pLuaRemoteService) /// zzz
	//{
	//	return;
	//}
	//LUA_LOAD_REQUEST* pRequest = _pLuaRemoteService->GetFirstRequest();
	//while (pRequest)
	//{
	//	LUA_RunFile(pRequest->_strFileToLoad);
	//	_pLuaRemoteService->GetNextRequest();
	//}
}

//-----------------------------------------------------------------------------
//	Name:		LUA_RunFile
//	Object:		
//	05-12-21:	NHO - Created
//-----------------------------------------------------------------------------
int LUA_CMD::LUA_RunFile(const QDT_STRING& sFilePath,bool bClient)
{
	QDT_STRING sRealPath;
	COM_STACK Stack;

	if (FILES_MANAGER_ABC::Instance()->IsFileExist(sFilePath)==false)
	{
		char sWorkingDirectory[256];
		LOADING_STACK::Instance()->GetWorkingDirectory(sWorkingDirectory);
		sRealPath = sWorkingDirectory;
		sRealPath += "\\";
		sRealPath += sFilePath;
		if (FILES_MANAGER_ABC::Instance()->IsFileExist(sRealPath)==false)
		{
			// error while searching for file
			QDT_STRING sText = "Can\'t open LUA file \"";
			sText += sRealPath;
			sText += "\"";
			SetOutput(sText);
			return -1;
		}
	}
	else
	{
		sRealPath = sFilePath;
	}

	//  le fichier existe.
	//	ajout d'un test de service.
#ifdef TEST_SERVICES
	if ( !bClient)
	{
		// si on se considere comme serveur, on va envoyer cette commande
		LUA_REMOTE_SERVICE* pService = KERNEL::Instance()->GetLuaRemoteService();
		if (pService)
		{
			// si on se considere comme serveur, on va envoyer cette commande
			pService ->LUA_RunFile(sRealPath);
		}
	}
#endif 

	QDT_STRING sOutputText = "Running LUA File \"";
	sOutputText += sRealPath;
	sOutputText +"\".";
	SetOutput(sOutputText);

	int nLUARunStatus = Stack.RunFile(sRealPath);
	if (nLUARunStatus != 0)
	{
		// error while parsing file
//		QDT_STRING sMessage = Stack.GetErrorStr();
//		QDT_Error(sMessage);
	}
	
	sOutputText = "LUA File \"";
	sOutputText += sRealPath;
	sOutputText += "\" runned.";
	SetOutput(sOutputText);

	return nLUARunStatus;
}


//-----------------------------------------------------------------------------
//	Name:		LUA_RunFileRemote
//	Object:		Test - ne fontionne que avec le working directory.
//	05-12-21:	GRI - Created
//-----------------------------------------------------------------------------
int LUA_CMD::LUA_RunFileRemote(const QDT_STRING& sFilePath)
{
	QDT_STRING sRealPath;
	COM_STACK Stack;


	QDT_FLAGS	Flags = FILES_MANAGER_ABC::FO_FILE_REMOTE;
	FILES_MANAGER_ABC::FM_ERROR eError;
	FILE_ABC* pFile = FILES_MANAGER_ABC::Instance()->Open(sFilePath,FILE_ABC::FM_CREATE_READ_WRITE,eError,Flags);

	if (!pFile)
	{
		return -1;
	}
	
	bool bExit = false;
	while( ! bExit ) 
	{
		// @gritodo 2
		//SERVICE_INTERFACE::Instance()->Run();
		if (pFile->IsIdle() || ( eError != FILES_MANAGER_ABC::FM_FILE_NO_ERROR ))
		{
			// no thread for the moment, @gritodo 
			bExit = true;
			continue;
		}
	}

	if (eError != FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
	{
		FILES_MANAGER_ABC::Instance()->Close(pFile);
		return -1;
	}

	QDT_STRING sOutputText = "Remote : Getting and Running LUA File \"";
	sOutputText += sRealPath;
	sOutputText +"\".";
	SetOutput(sOutputText);

	int nLUARunStatus = Stack.RunFile(pFile);
	if (nLUARunStatus != 0)
	{
		//	error while parsing file
		//	QDT_STRING sMessage = Stack.GetErrorStr();
		//	QDT_Error(sMessage);
	}
	sOutputText = "LUA File \"";
	sOutputText += sRealPath;
	sOutputText += "\" runned.";
	SetOutput(sOutputText);

	FILES_MANAGER_ABC::Instance()->Close(pFile);
	return nLUARunStatus;
}

