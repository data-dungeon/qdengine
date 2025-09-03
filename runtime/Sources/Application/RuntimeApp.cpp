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
//
//	01-09-17:	DAC - Created
//*****************************************************************************

//test
#include	"Root.h"
#include	INCL_RUN(Application/RunTimeApp)

#include	INCL_RUN(Application/Tools/LuaCmd)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_KCORE(OS/OS)
#include	INCL_KSERV(Sound/Sound)
#include	INCL_KCORE(Timer/TimerManager)
#include	INCL_KCOM(Database/Database)
#include	INCL_KCORE(Mem/Mem)
#include	INCL_KCORE(IceError/ErrorManager)
#include	INCL_KCORE(IceError/KCoreErrorModule)
#include	INCL_KCOM(Services/ComService)
#include	INCL_KCOM(Services/LoadingScreenService)
#include	INCL_KCOM(Services/VMService)
#include	INCL_KCOM(Services/LoadingsRecorder)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(KernelComManager/KernelComManager)
#include	INCL_KSERV(KernelObject/Kernel)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(M3D/Moteur3D)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_SINT(ModuleScriptTypes/ModuleScriptTypes )
#include	INCL_SINT(ScriptManager/ScriptManager)
#include	INCL_SINT(GameConfig/GameConfig)
#include	INCL_SINT(GameManager/GameManager)
#include	INCL_SQLDB(IDatabaseInterface)
#include	INCL_KINPUT(Input/ForInputABC)

// comment for no network.
//#define ENABLE_NETWORK
#define USE_NETWORK_THREAD

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

RUNTIME_APP		RUNTIME_APP::RA;

//******* DUMP INFO MEMOIRE ***************/
//#define MEM_DUMP
//******* DUMP INFO MEMOIRE ***************/
/*COM_MODULE			ComModule;
MODULE_SCRIPT_TYPES	MSType;
MOTEUR_3D_MODULE	M3D_Module;
KERNEL_MODULE		KrnModule;
VM_MODULE_TYPES		VmModule;*/

#define WINDOW_NAME "Renderer"

//-----------------------------------------------------------------------------
//	Name:		RUNTIME_APP WndProc
//	Object:		a simple WndProc that validates a window and kills it when the
//				time comes
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch(iMsg)
    {
        case WM_ERASEBKGND:
            return 0;
			break;

        case WM_DESTROY:
			KERNEL::Instance()->Quit();
			PostQuitMessage(0);
            return 0;
			break;

		case WM_SYSCOMMAND:	
			if (wParam == SC_CLOSE)
			{
				KERNEL::Instance()->Quit();
				PostQuitMessage(0);
				return 0;
			}
			break;

		case WM_ACTIVATE:
			{
				bool bActive = wParam != WA_INACTIVE;
				return 0;
			}
			break;

#ifndef _MASTER
		case WM_KEYDOWN:
		{
			// Open or close LUA Command line dialog (CTRL+SHIFT+C)
			if ( ((int)wParam == 'C') && (GetKeyState(VK_LCONTROL) & 0x8000) && (GetKeyState(VK_LSHIFT) & 0x8000) )
			{
				LUA_CMD::ToggleDlg(RUNTIME_APP::GetHWnd());
				return 0;
			}

			break;
		}
#endif //_MASTER

		case WM_SYSKEYDOWN:
			// Swap from fullscreen mode to windowed mode or the opposite (ALT+RETURN)
			if ( ((int)wParam == VK_RETURN) && (GetKeyState(VK_LMENU) & 0x8000) )
			{
				DISPLAY_MANAGER::Instance()->SwapFullscreenMode();
				return 0;
			}
			break;

		case WM_SIZE:
			DISPLAY_MANAGER::Instance()->ChangeWindowSize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOVE:
			DISPLAY_MANAGER::Instance()->ChangeWindowPos(LOWORD(lParam), HIWORD(lParam));
			break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
} // WndProc

//-----------------------------------------------------------------------------
//	Name:		RUNTIME_APP constructor
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::RUNTIME_APP()
:
#ifndef _MASTER
_pWnd	(NULL),
#endif
_bQuit	(false)//,
{

}

//-----------------------------------------------------------------------------
//	Name:		~RUNTIME_APP destructor
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::~RUNTIME_APP()
{
#ifndef _MASTER
	delete _pWnd;
#endif
	/*if	(_pRemoteServices)
	{
		REMOTE_SERVICES_MANAGER::UnRegisterRemoteService(_pRemoteServices);
		delete _pRemoteServices;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		GetRuntimeVersionString
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetRuntimeVersionString() const
{
	QDT_STRING strTmp;
	QDT_STRING sBuild;
#ifdef _DEBUG
	sBuild = "Debug";
#else
	sBuild = "Release";
#endif
	
	strTmp.Format("Runtime PC (%s:%s%s)", sBuild.GetBuffer(), KERNEL::GetVersionString().GetBuffer(), KERNEL::GetServicePackString().GetBuffer());
	
	return	(strTmp);
}

//-----------------------------------------------------------------------------
//	Name:		InitInstance
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RUNTIME_APP::InitInstance()
{
	BOOL		bResult;
	bool		bInitIAMOk, bInitOsOk, bRecord;
	
	bResult		= FALSE;
	
	//QDT_TRY
	{
#ifdef MEM_DUMP
		#pragma	QDT_FIXME("attention DUMP INFO memoire activé sur le runtime")
		FILE_STREAM	FSB("runtime_memory_before_init.txt", FILE_STREAM::AT_CREATE);
		QDT::KCORE::MEM::Dump(FSB, -1 & (~MEM::DF_STACK_ALLOCATED_NODE));
#endif

		MEM::CreateInstance();

		ERROR_MANAGER::Create();
		ERROR_MANAGER::Instance()->AddModule(new KCORE_ERROR_MODULE());

		IDATABASE_INTERFACE::InitDLL(IDATABASE_INTERFACE::DLL_VERSION, MEM::GetInstance());
		I_MFCWRAPPER_INTERFACE::InitDLL(I_MFCWRAPPER_INTERFACE::DLL_VERSION, MEM::GetInstance());

		FILES_MANAGER_ABC::Create();
		DATABASE_SERVICE::Create();
		COM_SERVICE::Create();
		LOADING_STACK::Create();
		LOADING_SCREEN_SERVICE::Create();
		FILES_MANAGER_ABC::Instance()->Init();
		
		

		// @gritodo : ici on va autoriser ou non l'activation du réseau et des remotes services associés.
		// KERNEL::Init(IsRemoteEnabled());

		// init COM
		MODULE_ABC::InitAll();

		COM_MODULE			*pComModule		= new COM_MODULE();
		KERNEL_MODULE		*pKrnModule		= new KERNEL_MODULE();
		MOTEUR_3D_MODULE	*pM3D_Module	= new MOTEUR_3D_MODULE();
		MODULE_SCRIPT_TYPES *pMSType		= new MODULE_SCRIPT_TYPES();

		VM_SERVICE::Create();
		KERNEL_COM_MANAGER::Instance();

		//	Get alternate inifile to use if specified on command line
		CheckForAlternateIniFile();

		// Read inifile. (gri: utile ce commentaire ;-) ) 
		ReadInifile();

		// QDT_Error when force loading object whiile in databank mode.
		COM_HANDLE::SetQdtErrorOnForceLoad(_RuntimeHeader.GetQdtErrorOnForceLoad());
		
		DATABASE_SERVICE::Instance()->EnablePS2Emulation(_RuntimeHeader.GetEnablePS2Emulation());

		//	Enable Bigfile retrieve logging
		COM_SERVICE::Instance()->EnableRetrieveLog(_RuntimeHeader.GetEnableRetrieveLog());

		//	Enable some flags (output stuff)
		OS::EnableDisplayLoadingMsg( _RuntimeHeader.GetEnableLoadingMsg());
		OS::EnableNoMessageBox(_RuntimeHeader.GetEnableNoMessageBox());
		
		//	 Game 
		GAME_CONFIG::Instance()->SetCheatMode(_RuntimeHeader.GetEnableCheat());

		//	Enable/Disable databank

//		DATABASE_SERVICE::Instance()->CleanVersionPriority();
//		DATABASE_SERVICE::Instance()->PushVersionPriority(DATABASE_ENUMS::VERSION_PS2);
//		DATABASE_SERVICE::Instance()->PushVersionPriority(DATABASE_ENUMS::VERSION_PC);
		
		// USMode
		KERNEL::SetUSMode(_RuntimeHeader.GetUSMode());

		//init KERNEL , 3D Engine, Network
		bInitOsOk	= InitOs();
		
		// Auto Add Additionnal Bigfiles
		AddAdditionnalBigfiles();

		// Vibration
		QDT::KSERV::INPUT::Instance()->EnableVibration(_RuntimeHeader.GetEnableVibration());

		bRecord = _RuntimeHeader.GetRecordObjectLoadings();
		if	(bRecord)
		{
			//	Disabled by default, record is started using menu
			LOADINGS_RECORDER::RecordLoadings(false);
		}

#ifndef _MASTER
		if	(bRecord || _RuntimeHeader.GetEnableDebugMenu())
		{
			_pWnd = new RENDER_WINDOW_SUBCLASS(_DisplayWindow);
		}
#endif

		//DISPLAY_MANAGER::Instance()->EnableShadows(_RuntimeHeader.GetShadows());

		//	Init Script Interface
		SCRIPT_MANAGER::Instance();

		//Do the MFC Stuff
		m_pMainWnd = new FRAME_WINDOW;

		if(_RuntimeHeader.GetViewerMode() == false)
		{
			//build script stuff
//			bInitIAMOk	= InitIAMRuntime();
			bInitIAMOk	= true;  // NHO : Unactivated for LUA 
		}
		
		bResult = WINDOW_APPLICATION::InitInstance();

		// create GAME_MANAGER instance
		COM_STACK Stack;
		Stack.RunScript("QDT.GAME_MANAGER.New(\"RT_GameManager\", 0)");			// Create GAME_MANAGER instance
		Stack.RunScript("GameManager = QDR.RT_GameManager:ActiveInstance()");	// Register it as 'fake' lua singleton

		//now run
		if( bInitOsOk && bResult ) 
		{
			//show the window
			::SendMessage( _DisplayWindow, WM_ACTIVATE, WA_ACTIVE, NULL);
			::ShowWindow( _DisplayWindow, SW_SHOW );
			::SetFocus( _DisplayWindow );

#ifndef _MASTER
			// start LUA file to run
			QDT_STRING sLUAFileName;
			sLUAFileName = _RuntimeHeader.GetLUAFileName();
			if (sLUAFileName!="")
			{
				LUA_RunFile(sLUAFileName);
			}
#endif

			/*if (_pViewerDlg != NULL)
			{
				WND * pRenderWindow = static_cast<WND*>( WND::FromHandle( DispHwnd ) );
				_pViewerDlg->DisplayWindow(pRenderWindow);
			}*/
		}
		else
		{
			//Get out we failed!
			return( FALSE );
		}

	}
//	QDT_UNHANDLED_CATCH

	return( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderWindow
//	Object:		
//	06-03-30:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::CreateRenderWindow()
{
	// create render window
	static int	wx = 1024;
	static int	wy = 768;
	static int	x = 20;
	static int	y = 20;

	_AppInstance = (HINSTANCE)(OS::GetAppHandle());

	WNDCLASSEX wndclass =
	{
		sizeof(wndclass),                   // cbSize
		CS_HREDRAW | CS_VREDRAW,            // style
		WndProc,                            // lpfnWndProc
		0,                                  // cbClsExtra
		0,                                  // cbWndExtra
		_AppInstance,                       // hInstance
		::LoadIcon(NULL, IDI_APPLICATION),    // hIcon
		::LoadCursor(NULL, IDC_ARROW),        // hCursor
		NULL,                               // hBackground
		NULL,                               // lpszMenuName
		"Renderer",							// lpszClassName
		::LoadIcon(NULL, IDI_APPLICATION)     // hIconSm
	};

	RegisterClassEx(&wndclass);

	_DisplayWindow = CreateWindow(WINDOW_NAME,        // lpClassName
								WINDOW_NAME,        // lpWindowName
								WS_OVERLAPPEDWINDOW,   // dwStyle
								x,         // x
								y,         // y
								wx,					// nWidth
								wy,					// nHeight
								NULL,                  // hwndParent
								NULL,                  // hMenu
								_AppInstance,             // hInstance
								NULL);                 // lpParam

//		ShowWindow(_DisplayWindow, iCmdShow);
//		UpdateWindow(_DisplayWindow);
	::SetWindowLong(_DisplayWindow, GWL_USERDATA, 1);
	OS::SetWinHandle((long)_DisplayWindow);

	RENDER_CONTEXT::Init(_DisplayWindow, wx, wy, false);
	RENDER_CONTEXT::ChangeWindowPos(x, y);
}


//-----------------------------------------------------------------------------
//	Name:		InitOs
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitOs()
{
	QDT_STRING		strText;

	QDT_TRY
	{
		KERNEL::OpenTools();

		// Database and Bigfile handling
		ConnectToDB();
		
		//	Specify the data version order (retrieve etc... PC/PS2/XBOX)
		QDT_STRING sDataVersion = _RuntimeHeader.GetDataVersion();
#ifndef _MASTER		
		DATABASE_SERVICE::Instance()->CleanVersionPriority();
		DATABASE_SERVICE::Instance()->PushVersionPriority(DATABASE_SERVICE::Instance()->GetVersionId(sDataVersion));
		DATABASE_SERVICE::Instance()->PushVersionPriority(DATABASE_ENUMS::VERSION_PC);
#endif

		CreateRenderWindow();

		//init Kernel
		OS::SetAppHandle((long)m_hInstance);
		KERNEL::Instance();

#ifdef ENABLE_NETWORK
		// Init Network
		QDT_STRING strFake;
		KERNEL::Instance()->InitNetwork(strFake,strFake);
#ifdef USE_NETWORK_THREAD
		SERVICE_INTERFACE::Instance()->StartThread();
#endif
		KERNEL::Instance()->LaunchQDTServices();
#endif

		

		// init M3D
		MOTEUR_3D::OpenTools();
		MOTEUR_3D::Instance();

		// mute all sounds  so you can here some music while testing
		if ( _RuntimeHeader.GetNoSound() == true )
		{
			SOUND::Instance()->SetMuteAll(true);
		}

		if	( _RuntimeHeader.GetWindowOnTop() )
		{
			::SetWindowPos( _DisplayWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		}
	}
	QDT_UNHANDLED_CATCH

	return( true );
	
}

//-----------------------------------------------------------------------------
//	Name:		InitIAMRuntime
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitIAMRuntime()
{
	// Does nothing for the moment	
	return( true );
}

//-----------------------------------------------------------------------------
//	Name:		ExitInstance
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RUNTIME_APP::ExitInstance()
{
	BOOL bResult = FALSE;

		// delete active GAME_MANAGER's instance
		GAME_MANAGER* pGM = GAME_MANAGER::ActiveInstance();
		if (pGM != NULL)
		{
			pGM->Clear();
		}

		//flush all read tasks
		LOADING_STACK::Instance()->Flush();
		
		//	Clean script interface
		SCRIPT_MANAGER::Suicide();

		//kill
		TIMER_MANAGER::Instance()->PauseTimers();

		VM_SERVICE::Release();
		MOTEUR_3D::Release();
		KERNEL::Release();
		MOTEUR_3D::CloseTools();
		KERNEL::CloseTools();
		RENDER_CONTEXT::Release();

#ifdef ENABLE_NETWORK
#ifdef USE_NETWORK_THREAD
		SERVICE_INTERFACE::Instance()->EndThread();
#endif
#endif 

		//	Close statics base
		// Disable cascaded delete, cause trouble on exit, dont call DATABASE::PopCascadedCleaningCtx() before exit
		/*COM_SERVICE::Instance()->PushCascadedCleaningCtx(false);	
		COM_SERVICE::Instance()->CleanStaticDatabase();
		COM_SERVICE::Instance()->DeleteStaticDatabaseWithoutBigFile();
		COM_SERVICE::Instance()->CleanDynamicDatabase();
		COM_SERVICE::Instance()->PopCascadedCleaningCtx();*/


		//	Destruct all remaining objects (should be called in last)
		COM_SERVICE::Release();
		DATABASE_SERVICE::Release();
		FILES_MANAGER_ABC::Release();
		LOADING_STACK::Release();
		LOADING_SCREEN_SERVICE::Release();

		ERROR_MANAGER::Release();

		// End of story, no com is usable passed this call !
		CLASS_DESCRIPTOR::CloseAll();

		// Disable cascaded delete, cause trouble on exit, with some remaining objects :(
//		COM_INSTANCE::SetForceNoCascadedDestruct(true);

		QDT_STRING::DumpAllocated();

#ifdef MEM_DUMP	
#pragma	QDT_FIXME("attention DUMP INFO memoire activé sur le runtime")
		FILE_STREAM	FSA("runtime_memory_after_init.txt", FILE_STREAM::AT_CREATE);
		QDT::KCORE::MEM::Dump(FSA, -1 & (~MEM::DF_STACK_ALLOCATED_NODE));
#endif

/*		if (_pViewerDlg != NULL)
		{
			_pViewerDlg->DestroyWindow();
			delete _pViewerDlg;
			_pViewerDlg = NULL;
		}*/


		 bResult = WINDOW_APPLICATION::ExitInstance();
//	}
//	QDT_UNHANDLED_CATCH

// 	IDATABASE_INTERFACE::ExitDLL();
	I_MFCWRAPPER_INTERFACE::ExitDLL();

	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		OnIdle
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RUNTIME_APP::OnIdle(long	lCount)
{
	BOOL bResult = TRUE;

//	QDT_TRY
	{
#ifndef _MASTER
		KERNEL::StartBoundFPS();
#endif

		WINDOW_APPLICATION::OnIdle(lCount);

		//reflesh all OS stuff
		MODULE_ABC::Loop();

		MODULE_ABC::Display();

#ifdef ENABLE_NETWORK
		// for the flush and the timeOuts.
		//GetServiceInterface()->Run(0);
#endif 

		//if asked to quit
		if (_bQuit)
		{
			KERNEL::Instance()->Quit();
		}	
#ifndef _MASTER
		KERNEL::WaitBoundFPS();
#endif
	}
//	QDT_UNHANDLED_CATCH

	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		Run
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
int	RUNTIME_APP::Run()
{
	int nRetCode = 0;

//	QDT_TRY
//	{
		nRetCode = WINDOW_APPLICATION::Run();	
//	}
//	QDT_UNHANDLED_CATCH


	return ( nRetCode );
}

//-----------------------------------------------------------------------------
//	Name:		ReadInifile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::ReadInifile()
{
	QDT_STRING	sIniFileName;
	char		S[_MAX_PATH];

	//	Get alternative one if any
	sIniFileName = GetAlternateIniFileName();
	
	if	(sIniFileName.IsEmpty())
	{	
		//	if none, default to exename.ini*/
	OS::GetProjectFileName( S, _MAX_FNAME );

	sIniFileName = S;
	sIniFileName = sIniFileName.EraseRight( 3 );
	sIniFileName += "ini";
	}

	if ( FILES_MANAGER_ABC::Instance()->IsFileExist( sIniFileName ) == true )
	{
		_RuntimeHeader.ReadFromFile( sIniFileName );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConnectToDB
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::ConnectToDB()
{
	//	BigFile name
	QDT_STRING sBFName;
	sBFName = _RuntimeHeader.GetBigFileName();
	if	(sBFName.IsEmpty())
	{
		QDT_Error("Runtime: empty bigfile name");
	}
	else
	{
		COM_SERVICE::Instance()->SetDefaultName(sBFName);
	}

	bool bDBConnect = _RuntimeHeader.GetDatabaseConnect();
	//when you want database connection to be active
	DATABASE_SERVICE::Instance()->EnableServerAccess( bDBConnect );
	if	(bDBConnect)
	{	
		QDT_STRING sDatabase = _RuntimeHeader.GetDatabaseName();
		QDT_STRING sUserTrigram = _RuntimeHeader.GetUserDBLogin();

		DATABASE_SERVICE::Instance()->SetCurrentUserName(sUserTrigram);

		if ( DATABASE_SERVICE::Instance()->IsConnected() == false )
		{
			if ( DATABASE_SERVICE::Instance()->Open(sDatabase, "Runtime") == false )
			{
				DATABASE_SERVICE::Instance()->Close();
				QDT_STRING Msg;
				Msg.Format("Connection to the data source failed for user %s: %s", sUserTrigram.GetBuffer(), sDatabase.GetBuffer());
				::MessageBox( NULL, Msg, "Database connect Error!", MB_OK );
				return	(false);
			}
			
			//	Instanciate for bigfile opening
			/*DATABASE_MANAGER::Instance();
			
			//	Allow Bigfile sharing so we can update it from the outside
			DATABASE_MANAGER::Instance()->EnableBigFileSharing(true);*/

			DATABASE_SERVICE::Instance()->Init();
			(COM_SERVICE::Instance()->GetCurrentStaticDatabase())->EnableSharing(true);

			return	(true);
		}
		else 
		{
			if ( DATABASE_SERVICE::Instance()->ChangeDatabase(sDatabase, "Runtime")  == false)	
			{
				QDT_STRING Msg;
				Msg.Format("Connection to the data source failed for user %s: %s", sUserTrigram.GetBuffer(), sDatabase.GetBuffer());
				::MessageBox(NULL, Msg, "Database connect Error!", MB_OK );
				return	(false);
			}
			
			DATABASE_SERVICE::Instance()->Init();
			(COM_SERVICE::Instance()->GetCurrentStaticDatabase())->EnableSharing(true);
			//	Instanciate for bigfile opening
			/*DATABASE_MANAGER::Instance();
			
			//	Allow Bigfile sharing so we can update it from the outside
			DATABASE_MANAGER::Instance()->EnableBigFileSharing(true);*/

			return	(true);
		}
	}
	else
	{
		DATABASE_SERVICE::Instance()->Init();
	}
	
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		SetAlternateIniFileName
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetAlternateIniFileName(const QDT_STRING &	IniFileName)
{
	_sAlternateIniFileName = IniFileName;
}

//-----------------------------------------------------------------------------
//	Name:		GetAlternateIniFileName
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	RUNTIME_APP::GetAlternateIniFileName() const
{
	return	(_sAlternateIniFileName);
}

//-----------------------------------------------------------------------------
//	Name:		CheckForAlternateIniFile
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::CheckForAlternateIniFile()
{
	QDT_STRING sTmp;
	
	//	Check first argument
	sTmp = __targv[1];

	SetAlternateIniFileName(sTmp);
}

//-----------------------------------------------------------------------------
//	Name:		AddAdditionnalBigfiles
//	Object:		Search for bigfile1, bigfile2, bigfile3 etc.. and add them
//	04-06-09:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::AddAdditionnalBigfiles()
{
	QDT_STRING	sBFName;
	int			nCurNum;
	QDT_STRING	sNewName;

	sBFName = COM_SERVICE::Instance()->GetDefaultName();
	nCurNum = 1;
	bool bStop	= false;
	if	(sBFName.IsEmpty() == false)
	{
		while(bStop == false)
		{
			sNewName.Format("%s%i.idx", sBFName.GetBuffer(), nCurNum);
			if	(FILES_MANAGER_ABC::Instance()->IsFileExist(sNewName))
			{
				//add idx
				KERNEL::Instance()->AddIdx(sNewName);
				++nCurNum;
			}
			else
			{
				bStop = true;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		LUA_RunFile
//	Object:		
//	05-12-05:	NHO - Created
//-----------------------------------------------------------------------------
int RUNTIME_APP::LUA_RunFile(const QDT_STRING& sFilePath)
{
	COM_STACK Stack;
	Stack.RegisterFunction("print", LUA_Output);

	QDT_STRING sRealPath;
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
			QDT_Error("Can\'t open LUA file \"%s\"", sRealPath);
			return -1;
		}
	}
	else
	{
		sRealPath = sFilePath;
	}

	QDT_Message("Running LUA File \"%s\".", sRealPath);
	int nLUARunStatus = Stack.RunFile(sRealPath);
	if (nLUARunStatus != 0)
	{
		// error while parsing file
//		QDT_STRING sMessage = Stack.GetErrorStr();
//		QDT_Error(sMessage);
	}
	QDT_Message("LUA File \"%s\" runned.", sRealPath);

	return nLUARunStatus;
}

//-----------------------------------------------------------------------------
//	Name:		LUA_Output
//	Object:		
//	05-12-05:	NHO - Created
//-----------------------------------------------------------------------------

int	RUNTIME_APP::LUA_Output(COM_PARAM P)
{
	QDT_STRING sMessage;
	COM_STACK Stack(P);
	Stack.GetStackValue(sMessage);

	QDT_Message(sMessage);
	return 0;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

