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
//	Message.cpp
//
//	05-09-19:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Core/Message)

#include	<Signal.h>
#include	INCL_KCORE(Core/IOutput)
#include	INCL_KCORE(Tools/QdtFlags)
#include	INCL_KCORE(OS/OS)
#include	INCL_KCORE(IceError/ErrorManager)
#include	INCL_KCORE(IceError/KCoreError)
#include	INCL_KCORE(IceError/KCoreErrorModule)

#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

// gri
#include	INCL_KCORE(Thread/Mutex)

#define BUFFER_SIZE 4096
#define MAX_CALL_STACK					40
#define CALL_STACK_START_OFFSET			2
#define CALL_STACK_FUNCTION_NAME_SIZE	256

//static MUTEX s_MutexForQDTMessage;

Int32 g_nCoreCurrentThread = 0;		// 0 = main thread
OPTIONAL_FUNC_CONTAINER::OutputFunc OPTIONAL_FUNC_CONTAINER::_pOutputFunc=0;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#define IDD_DIALOG_ASSERT               101
#define IDC_EDIT_LINE                   1000
#define IDC_EDIT_FILE                   1001
#define IDC_EDIT_CONDITION              1002
#define IDC_BUTTON_IGNORE				1003
#define IDC_EDIT_MESSAGE                1004
#define IDC_BUTTON_IGNOREALL            1005
#define IDC_BUTTON_COPY_CLIPBOARD       1006
#define IDC_BUTTON_STOP                 1007
#define IDC_BUTTON_BREAK                1008
#define IDC_COMBO_CALLSTACK             1009
#define IDC_BUTTON_OPENSOURCE           1010
#define IDC_STATIC (-1)

#define DLG_TITLE			L"Assertion Failed"
#define DLG_FONT			L"MS Sans Serif"
#define DLG_ITEM_TEXT_0		L"&Ignore"
#define DLG_ITEM_TEXT_1		L"&Stop"
#define DLG_ITEM_TEXT_2		L"Info"
#define DLG_ITEM_TEXT_3		L""
#define DLG_ITEM_TEXT_4		L"Line"
#define DLG_ITEM_TEXT_5		L""
#define DLG_ITEM_TEXT_6		L"File"
#define DLG_ITEM_TEXT_7		L"Condition"
#define DLG_ITEM_TEXT_8		L""
#define DLG_ITEM_TEXT_9		L"failed"
#define DLG_ITEM_TEXT_10	L""
#define DLG_ITEM_TEXT_11	L"Message"
#define DLG_ITEM_TEXT_12	L"Ignore &All"
#define DLG_ITEM_TEXT_13	L"Copy to clipboard"
#define DLG_ITEM_TEXT_14	L"&Break"
#define DLG_ITEM_TEXT_15	L""
#define DLG_ITEM_TEXT_16	L"Call Stack"
#define DLG_ITEM_TEXT_17	L"&Open cpp/h"

#define DLG_NB_ITEM   18

template <int TitleSize>
struct SDlgItem
{
	// If use my struct instead of DLGTEMPLATE, or else (for some strange reason) it is not DWORD aligned !!
	DWORD style; DWORD dwExtendedStyle; short x; short y; short cx; short cy; WORD id;
	WORD ch; WORD c; WORD t[TitleSize]; WORD dummy;
};
#define SDLGITEM(TEXT,V) SDlgItem<sizeof(TEXT)/2> V;

struct SDlgData
{
	DLGTEMPLATE dlt;
	WORD _menu; WORD _class; WCHAR _title[sizeof(DLG_TITLE)/2]; WORD pointSize; WCHAR _font[sizeof(DLG_FONT)/2];
	SDLGITEM(DLG_ITEM_TEXT_0, i0);
	SDLGITEM(DLG_ITEM_TEXT_1, i1);
	SDLGITEM(DLG_ITEM_TEXT_2, i2);
	SDLGITEM(DLG_ITEM_TEXT_3, i3);
	SDLGITEM(DLG_ITEM_TEXT_4, i4);
	SDLGITEM(DLG_ITEM_TEXT_5, i5);
	SDLGITEM(DLG_ITEM_TEXT_6, i6);
	SDLGITEM(DLG_ITEM_TEXT_7, i7);
	SDLGITEM(DLG_ITEM_TEXT_8, i8);
	SDLGITEM(DLG_ITEM_TEXT_9, i9);
	SDLGITEM(DLG_ITEM_TEXT_10, i10);
	SDLGITEM(DLG_ITEM_TEXT_11, i11);
	SDLGITEM(DLG_ITEM_TEXT_12, i12);
	SDLGITEM(DLG_ITEM_TEXT_13, i13);
	SDLGITEM(DLG_ITEM_TEXT_14, i14);
	SDLGITEM(DLG_ITEM_TEXT_15, i15);
	SDLGITEM(DLG_ITEM_TEXT_16, i16);
	SDLGITEM(DLG_ITEM_TEXT_17, i17);
};

static SDlgData g_DialogRC =
{
	// Dialog Header
	{DS_MODALFRAME|DS_3DLOOK|DS_SETFONT|DS_CENTER|WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_VISIBLE,0,DLG_NB_ITEM,0,0,317,151},
		0,0,DLG_TITLE,8,DLG_FONT,

	// Button Ignore
	{BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,0,7,131,60,13,IDC_BUTTON_IGNORE,0xFFFF,0x0080,DLG_ITEM_TEXT_0,0},

	// Button Stop
	{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,0,253,131,60,13,IDC_BUTTON_STOP,0xFFFF,0x0080,DLG_ITEM_TEXT_1,0},

	// Group "Info"
	{BS_GROUPBOX|WS_CHILD|WS_VISIBLE,0,7,7,306,119,IDC_STATIC,0xFFFF,0x0080,DLG_ITEM_TEXT_2,0},

	// Edit Line
	{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,50,48,25,13,IDC_EDIT_LINE,0xFFFF,0x0081,DLG_ITEM_TEXT_3,0},

	// Label "Line"
	{WS_CHILD|WS_VISIBLE,0,11,50,14,8,IDC_STATIC,0xFFFF,0x0082,DLG_ITEM_TEXT_4,0},

	// Edit File
	{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,50,32,200,13,IDC_EDIT_FILE,0xFFFF,0x0081,DLG_ITEM_TEXT_5,0},

	// Label "File"
	{WS_CHILD|WS_VISIBLE,0,11,34,12,8,IDC_STATIC,0xFFFF,0x0082,DLG_ITEM_TEXT_6,0},

	// Label "Condition"
	{WS_CHILD|WS_VISIBLE,0,11,18,30,8,IDC_STATIC,0xFFFF,0x0082,DLG_ITEM_TEXT_7,0},

	// Edit Condition
	{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,50,16,200,13,IDC_EDIT_CONDITION,0xFFFF,0x0081,DLG_ITEM_TEXT_8,0},

	// Label "failed"
	{WS_CHILD|WS_VISIBLE,0,256,18,18,8,IDC_STATIC,0xFFFF,0x0082,DLG_ITEM_TEXT_9,0},

	// Edit Message
	{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,50,64,256,12,IDC_EDIT_MESSAGE,0xFFFF,0x0081,DLG_ITEM_TEXT_10,0},

	// Label "Message"
	{WS_CHILD|WS_VISIBLE,0,11,66,30,8,IDC_STATIC,0xFFFF,0x0082,DLG_ITEM_TEXT_11,0},

	// Button IgnoreAll
	{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,0,89,131,60,13,IDC_BUTTON_IGNOREALL,0xFFFF,0x0080,DLG_ITEM_TEXT_12,0},

	// Button Copy to clipboard
	{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,0,92,102,72,12,IDC_BUTTON_COPY_CLIPBOARD,0xFFFF,0x0080,DLG_ITEM_TEXT_13,0},

	// Button Break
	{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,0,171,131,60,13,IDC_BUTTON_BREAK,0xFFFF,0x0080,DLG_ITEM_TEXT_14,0},

	// Combo CallStack
	{CBS_DROPDOWNLIST|WS_VSCROLL|WS_TABSTOP|WS_VISIBLE,0,50,79,256,300,IDC_COMBO_CALLSTACK,0xFFFF,0x0085,DLG_ITEM_TEXT_15,0},

	// Label "Call Stack"
	{WS_CHILD|WS_VISIBLE,0,11,81,35,17,IDC_STATIC,0xFFFF,0x0082,DLG_ITEM_TEXT_16,0},

	// Button "Open cpp/h"
	{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,0,256,32,50,13,IDC_BUTTON_OPENSOURCE,0xFFFF,0x0080,DLG_ITEM_TEXT_17,0},

};

struct ASSERTINFO
{
	struct FUNCTIONCALLED
	{
		char Function[CALL_STACK_FUNCTION_NAME_SIZE];
	};

	const char* Title;
	const char* Condition;
	const char* File;
	int			Line;
	const char* Message;

	FUNCTIONCALLED	CallStack[MAX_CALL_STACK];
	int				CallStackDepth;

	enum USERCHOICE
	{
		UC_NO_CHOICE =0,
		UC_IGNORE,
		UC_IGNOREALL,
		UC_BREAK,
		UC_STOP
	} UserChoice;
};

//-----------------------------------------------------------------------------
//	Name:		AssertDlgProc
//	Object:		
//	06-05-19:	BFA - Created
//-----------------------------------------------------------------------------
static BOOL CALLBACK AssertDlgProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
	static ASSERTINFO* pAssertInfo= NULL;
	switch (msg)
	{
	case WM_INITDIALOG:
		{
			//fill in window
			pAssertInfo= (ASSERTINFO*)lParam;
			SetWindowText(hWnd, pAssertInfo->Title);
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_CONDITION), pAssertInfo->Condition);
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_FILE), pAssertInfo->File);
			char szLine[30]= "";
			sprintf(szLine, "%d", pAssertInfo->Line);
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_LINE), szLine);
			if (pAssertInfo->Message && pAssertInfo->Message[0]!='\0')
				SetWindowText(GetDlgItem(hWnd,IDC_EDIT_MESSAGE), pAssertInfo->Message);
			else
				SetWindowText(GetDlgItem(hWnd,IDC_EDIT_MESSAGE), "No message");

			HWND hWndCombo =GetDlgItem(hWnd,IDC_COMBO_CALLSTACK);

			int i;
			for (i=0; i<pAssertInfo->CallStackDepth; i++)
			{
				SendMessage(hWndCombo,CB_INSERTSTRING,i,(long)pAssertInfo->CallStack[i].Function);
			}
			SendMessage(hWndCombo,CB_SETCURSEL,0,0);
		}
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON_OPENSOURCE:
				ShellExecute(0,0,pAssertInfo->File,NULL,NULL,SW_SHOWNORMAL);
				break;
			case IDC_BUTTON_COPY_CLIPBOARD:
				{
					//build String
					char	MessageBuffer[BUFFER_SIZE];
					sprintf(MessageBuffer,	"**********************\n"
											"* Assertion Failed ! *\n"
											"**********************\n"
											"  Condition : %s\n"
											"  Message :   %s\n"
											"  File :      %s\n"
											"  Line :      %d\n"
											"  CallStack :\n",
											pAssertInfo->Condition, pAssertInfo->Message, pAssertInfo->File, pAssertInfo->Line);

					int i;
					for (i=0; i<pAssertInfo->CallStackDepth; i++)
					{
						strcat(MessageBuffer, "              ");
						strcat(MessageBuffer, pAssertInfo->CallStack[i].Function);
						strcat(MessageBuffer, "\n");
					}

					QDT_Message(MessageBuffer);

					HGLOBAL clipbuffer;
					char* buffer;
					OpenClipboard(hWnd);
					EmptyClipboard();
					clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(MessageBuffer)+1);
					buffer = (char*)GlobalLock(clipbuffer);
					strcpy(buffer, LPCSTR(MessageBuffer));
					GlobalUnlock(clipbuffer);
					SetClipboardData(CF_TEXT,clipbuffer);
					CloseClipboard();
				}
				break;
			case IDC_BUTTON_IGNORE:
				pAssertInfo->UserChoice =ASSERTINFO::UC_IGNORE;
				EndDialog(hWnd, 0);
				break;
			case IDC_BUTTON_IGNOREALL:
				pAssertInfo->UserChoice =ASSERTINFO::UC_IGNOREALL;
				EndDialog(hWnd, 0);
				break;
			case IDC_BUTTON_BREAK:
				pAssertInfo->UserChoice =ASSERTINFO::UC_BREAK;
				EndDialog(hWnd, 0);
				break;
			case IDC_BUTTON_STOP:
				pAssertInfo->UserChoice =ASSERTINFO::UC_STOP;
				EndDialog(hWnd, 1);
				break;
			};
			break;
		}
	default:
		return FALSE;
	};
	return TRUE;

}

//-----------------------------------------------------------------------------
//	Name:		QDT_Message
//	Object:		
//	05-09-19:	RMA - Created
//-----------------------------------------------------------------------------
void QDT::KCORE::QDT_Message(const char* pTextMessage, ...)
{
	//s_MutexForQDTMessage.Lock();
	if	(pTextMessage != NULL)
	{
		char Buffer[BUFFER_SIZE] ="";

		va_list	Args;
		
		va_start(Args, pTextMessage);
		_vsnprintf(Buffer, BUFFER_SIZE, pTextMessage, Args);
		va_end(Args);
		
		if (g_nCoreCurrentThread == 0 )		// only for main thread.
		{
			I_OUTPUT::Push(Buffer);
			if (OPTIONAL_FUNC_CONTAINER::_pOutputFunc) 
			{
				OPTIONAL_FUNC_CONTAINER::_pOutputFunc(Buffer);
			}
		}
	}
	//s_MutexForQDTMessage.Unlock();
}


//-----------------------------------------------------------------------------
//	Name:		QDT_MessageThreadSafe
//	Object:	Because the fonction QDT_Message make a call to GetWindowText wich crash the thread
//	06-03-21:	GRI - Created
//-----------------------------------------------------------------------------
void QDT::KCORE::QDT_MessageThreadSafe(const char* pTextMessage, ...)
{
	if	(pTextMessage != NULL)
	{
		char Buffer[BUFFER_SIZE] ="";

		va_list	Args;
		
		va_start(Args, pTextMessage);
		_vsnprintf(Buffer, BUFFER_SIZE, pTextMessage, Args);
		va_end(Args);
	}
}

//-----------------------------------------------------------------------------
//	Name:		QDT_Warning
//	Object:		
//	05-09-19:	RMA - Created
//-----------------------------------------------------------------------------
void QDT::KCORE::QDT_Warning(const char* pTextWarning, ...)
{
	char Buffer[BUFFER_SIZE] ="";

	strcpy(Buffer, "[WARNING] : ");

	if	(pTextWarning != NULL)
	{	
		va_list	Args;

		va_start(Args, pTextWarning);
		_vsnprintf(Buffer + strlen(Buffer),BUFFER_SIZE, pTextWarning, Args);
		va_end(Args);
	}

	// Register the warning
	if	(ERROR_MANAGER::IsInstancied())
	{
		KCORE_ERROR * pErr = static_cast<KCORE_ERROR*>( ERROR_MANAGER::Instance()->NewError(ERROR_MANAGER::MI_KCORE, KCORE_ERROR_MODULE::KE_QDT_WARNING));
        if(pErr)
        {
			pErr->SetAdditionnalErrorText(Buffer);
        }
	}

	// old notification mechanism, to be replaced by ERROR_MANAGER handling at application level ie iam, runtime
	I_OUTPUT::Push(Buffer,true);

	if (OPTIONAL_FUNC_CONTAINER::_pOutputFunc) 
	{
		OPTIONAL_FUNC_CONTAINER::_pOutputFunc(Buffer);
	}
}

//-----------------------------------------------------------------------------
//	Name:		QDT_Error
//	Object:		
//	05-09-19:	RMA - Created
//-----------------------------------------------------------------------------
void QDT::KCORE::QDT_Error(const char* pTextError, ...)
{
	int				Result;
	va_list			Args;
	char			LocalBuffer[2048];
	char			Buffer[BUFFER_SIZE] ="";
	unsigned int	i;
	void			*pAddress;

	strcpy(Buffer, "");

	if	(pTextError != NULL)
	{	
		va_start(Args, pTextError);
		_vsnprintf(Buffer, BUFFER_SIZE, pTextError, Args);
		va_end(Args);
	}

	strncat(Buffer, "\nCall-stack :\n", sizeof(Buffer));

	for(i = 0 ; i < 32 ; ++i)
	{
		pAddress = OS::GetCallStack(i);
		if	(pAddress == NULL)
		{
			break;
		}
		sprintf(LocalBuffer, "\n	%08X : ", pAddress);
		strncat(Buffer, LocalBuffer, sizeof(Buffer));

		OS::GetFunctionName(pAddress, LocalBuffer, sizeof(LocalBuffer));
		strncat(Buffer, LocalBuffer, sizeof(LocalBuffer));
	}

	//	Le buffer est rempli jusqu'à ce qu'une call-stack vide ait été détectée
	//	Ou bien qu'une exception se soit lancée...

	//	on affiche le message sur la commande de debug de Visual C++
	QDT_OutputDebugString(Buffer);
	QDT_OutputDebugString("\n");

	// Register the error
	if	(ERROR_MANAGER::IsInstancied())
	{
		KCORE_ERROR * pErr = static_cast<KCORE_ERROR*>( ERROR_MANAGER::Instance()->NewError(ERROR_MANAGER::MI_KCORE, KCORE_ERROR_MODULE::KE_QDT_ERROR));
        if(pErr)
        {
			pErr->SetAdditionnalErrorText(Buffer);
        }
	}

	strncat(Buffer, "\n\nClick ABORT if you want to quit this application.\nClick RETRY if you want to debug.\nClick IGNORE if you want to continue.", sizeof(Buffer));

	//	Pour l'instant on passe par une MessageBox voir après suivant la plate-forme faire une interface correspondante
	if	(OS::IsLogOutputEnabled() == false)
	{
		if	(OS::IsEnableNoMessageBoxOn() == false)
		{
			Result = ::MessageBox(NULL, Buffer, "Quantic Dream Tools", MB_ABORTRETRYIGNORE | MB_DEFBUTTON3);
		}
		else
		{
			//	Removes dialog when qdt_error comes , bomb to desktop instead (no chbiiiing)
			Result = IDABORT;
			
			//	Report this as an exception so that all logging routine are called
			OS::ReportException(-1, NULL);
		}

		switch(Result)
		{
		case	IDABORT:
			//	On sort directement de l'application sans rien réinitialiser !
			exit(0);
			break;
		case	IDRETRY:
			//	On va lancer une interruption pour permettre au debuggeur de reprendre la main.
			__asm
			{
				int	3
			}
			break;
		case	IDIGNORE:
			break;
		}
	}
	else
	{
		//	On ignore l'erreur et on continue comme si de rien était.
		I_OUTPUT::Push(Buffer);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertIDToQDT
//	Object:		
//	02-06-20:	DAC - Created
//-----------------------------------------------------------------------------
int ConvertIDToQDT(const int	nValue)
{
	switch(nValue)
	{ 
		default:
		{
			QDT_FAIL();
			return	(-1);
		}

		case IDABORT:
		{
			return	(QDT_IDABORT);
		}

		case IDCANCEL:
		{
			return	(QDT_IDCANCEL);
		}
		
		case IDIGNORE:
		{
			return	(QDT_IDIGNORE);
		}
		
		case IDNO:
		{
			return	(QDT_IDNO);
		}
		
		case IDOK:
		{
			return	(QDT_IDOK);
		}

		case IDRETRY:
		{
			return	(QDT_IDRETRY);
		}
		
		case IDYES:
		{
			return	(QDT_IDYES);
		}
	}

	QDT_FAIL();
	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertQdtToMB
//	Object:		
//	02-06-20:	DAC - Created
//-----------------------------------------------------------------------------
int ConvertQdtToMB(const int nMask)
{
	QDT_FLAGS Mask(nMask);
	QDT_FLAGS Result;

	if	(Mask.IsSet(QDT_ABORTRETRYIGNORE)) 
	{
		Result.Set(MB_ABORTRETRYIGNORE);
	}

	if	(Mask.IsSet(QDT_OK)) 
	{
		Result.Set(MB_OK);
	}
	
	if	(Mask.IsSet(QDT_OKCANCEL)) 
	{
		Result.Set(MB_OKCANCEL);
	}
	
	if	(Mask.IsSet(QDT_RETRYCANCEL)) 
	{
		Result.Set(MB_RETRYCANCEL);
	}

	if	(Mask.IsSet(QDT_YESNO)) 
	{
		Result.Set(MB_YESNO);
	}
	
	if	(Mask.IsSet(QDT_YESNOCANCEL)) 
	{
		Result.Set(MB_YESNOCANCEL);
	}

	if	(Mask.IsSet(QDT_ICONEXCLAMATION)) 
	{
		Result.Set(MB_ICONEXCLAMATION);
	}

	if	(Mask.IsSet(QDT_ICONWARNING))
	{
		Result.Set(MB_ICONWARNING);
	}
	
	if	(Mask.IsSet(QDT_ICONINFORMATION))
	{
		Result.Set(MB_ICONINFORMATION);
	}

	if	(Mask.IsSet(QDT_ICONASTERISK))
	{
		Result.Set(MB_ICONASTERISK);
	}

	if	(Mask.IsSet(QDT_ICONQUESTION))
	{
		Result.Set(MB_ICONQUESTION);
	}
	
	if	(Mask.IsSet(QDT_ICONSTOP))
	{
		Result.Set(MB_ICONSTOP);
	}

	if	(Mask.IsSet(QDT_ICONERROR))
	{
		Result.Set(MB_ICONERROR);
	}
	
	if	(Mask.IsSet(QDT_ICONHAND))
	{
		Result.Set(MB_ICONHAND);
	}
	
	if	(Mask.IsSet(QDT_ICONHAND))
	{
		Result.Set(MB_ICONHAND);
	}

	if	(Mask.IsSet(QDT_DEFBUTTON1))
	{
		Result.Set(MB_DEFBUTTON1);
	}
	
	if	(Mask.IsSet(QDT_DEFBUTTON2))
	{
		Result.Set(MB_DEFBUTTON2);
	}

	if	(Mask.IsSet(QDT_DEFBUTTON3))
	{
		Result.Set(MB_DEFBUTTON3);
	}

	if	(Mask.IsSet(QDT_DEFBUTTON4))
	{
		Result.Set(MB_DEFBUTTON4);
	}

	if	(Mask.IsSet(QDT_APPLMODAL))
	{
		Result.Set(MB_APPLMODAL);
	}

	if	(Mask.IsSet(QDT_SETFOREGROUND))
	{
		Result.Set(MB_SETFOREGROUND);
	}

	if	(Mask.IsSet(QDT_TOPMOST))
	{
		Result.Set(MB_TOPMOST);
	}

	return	(Result.AsInt());
}

//-----------------------------------------------------------------------------
//	Name:		QDT_MessageBox
//	Object:		
//	02-06-20:	DAC - Created
//-----------------------------------------------------------------------------
int	QDT::KCORE::QDT_MessageBox(int hWnd, const char* pText, const char* pCaption, const int nFlags)
{
	int nRetVal;

	if	(OS::IsEnableNoMessageBoxOn() == false)
	{
		nRetVal = ::MessageBox(reinterpret_cast<HWND>(hWnd), pText, pCaption, ConvertQdtToMB(nFlags));
	}
	else
	{
		QDT_Message(pText);
		nRetVal = IDABORT;
		//	Report this as an exception so that all logging routine are called
		OS::ReportException(-1, NULL);
	}

	return (ConvertIDToQDT(nRetVal));
}

//-----------------------------------------------------------------------------
//	Name:		QDT_OutputDebugString
//	Object:		
//	02-06-21:	DAC - Created
//-----------------------------------------------------------------------------
void QDT::KCORE::QDT_OutputDebugString(const char* pOutputString)
{
	OutputDebugString(pOutputString);

	if (OPTIONAL_FUNC_CONTAINER::_pOutputFunc) 
	{
		OPTIONAL_FUNC_CONTAINER::_pOutputFunc(pOutputString);
	}
}

//-----------------------------------------------------------------------------
//	Name:		g_QDTCustomAssertFuncDefault
//	Object:		
//	06-05-19:	BFA - Created
//-----------------------------------------------------------------------------
bool QDT::KCORE::g_QDTCustomAssertFuncDefault(const char* pTitle, const char* pCondition, const char* pMessage, const char* pFile, int nLine, bool* pbIgnoreAll)
{
	ASSERTINFO AssertInfo;
	AssertInfo.Condition	=pCondition;
	AssertInfo.File			=pFile;
	AssertInfo.Line			=nLine;
	AssertInfo.Message		=pMessage;
	AssertInfo.UserChoice	=ASSERTINFO::UC_NO_CHOICE;
	AssertInfo.Title		=pTitle;

	//fill callstack
	unsigned int i=0;
	void* pAddress = OS::GetCallStack(i+CALL_STACK_START_OFFSET);
	while (pAddress!=NULL && i<MAX_CALL_STACK)
	{
		char Function[CALL_STACK_FUNCTION_NAME_SIZE];
		OS::GetFunctionName(pAddress, Function, sizeof(Function));
		sprintf(AssertInfo.CallStack[i].Function, "%08X : %s", pAddress, Function);
		i++;
		pAddress = OS::GetCallStack(i+CALL_STACK_START_OFFSET);
	}
	AssertInfo.CallStackDepth =i;

	int er = DialogBoxIndirectParam((HINSTANCE)GetModuleHandle(NULL), (DLGTEMPLATE*)&g_DialogRC, GetDesktopWindow(), AssertDlgProc, (LPARAM)&AssertInfo);

	switch(AssertInfo.UserChoice)
	{
	case ASSERTINFO::UC_STOP:
		raise(SIGABRT);
		exit(EXIT_FAILURE);
		return false;

	case ASSERTINFO::UC_BREAK:
		return true;

	case ASSERTINFO::UC_IGNOREALL:
		(*pbIgnoreAll)= true;
		return false;

	case ASSERTINFO::UC_IGNORE:
		return false;

	}

	return false;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
