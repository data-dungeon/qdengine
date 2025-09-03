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
//	05-11-14:	VMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"
#include	INCL_RUN(Application/RunTimeApp_ps3)

//Include KernelCom
#include	INCL_KCOM(Services/DataBaseService)
#include	INCL_KCOM(Services/ComService)
#include	INCL_KCOM(Services/LoadingScreenService)
#include	INCL_KCOM(Services/VmService)
#include	INCL_KCOM(ComBasis/ComStack)

//Include KernelServ
#include	INCL_KSERV(KernelObject/Kernel)
#include	INCL_KSERV(KernelModule/KernelModule)
#include	INCL_KSERV(Sound/Sound)

//Include KernelRenderer
#include	INCL_KRENDERER(Renderer/RenderContext)

//Include moteur 3d
#include INCL_3DENGINE(M3D/Moteur3D)
#include INCL_3DENGINE(M3D/Moteur3DModule)

// Include ScriptInterface
#include	INCL_SINT(ModuleScriptTypes/ModuleScriptTypes)
#include	INCL_SINT(ScriptManager/ScriptManager)

//Include KernelCore
#include	INCL_KCORE(Job/JobManager)
#include	INCL_KCORE(File/FilesManagerABC)
#include	INCL_KCORE(IceError/ErrorManager)
#include	INCL_KCORE(IceError/KCoreErrorModule)

#include	INCL_KNETWORK(ServiceInterface)

//Include KernelInput
#include	INCL_KINPUT(Input/PS3/PadManager_PS3)

#define	NUM_PHYSICAL_SPU	6
#define	NUM_RAW_SPU			1

	// comment for no network.
//#define ENABLE_NETWORK
#define USE_NETWORK_THREAD

// simple test on blue.lua, si vous ne vous appellez pas GRI et que la ligne suivante n'est pas
// commentée, envoyez un mail incendiaire à GRI ;-)
//#define TEST_REMOTE

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

RUNTIME_APP::RUNTIME_APP	RA;

//-----------------------------------------------------------------------------
//	Name:		RUNTIME_APP constructor
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::RUNTIME_APP()
{
}

//-----------------------------------------------------------------------------
//	Name:		RUNTIME_APP destructor
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::~RUNTIME_APP()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetDefaultClassToBoot
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetDefaultClassToBoot() const
{
	return ("Global");
}

//-----------------------------------------------------------------------------
//	Name:		GetDefaultByteCodeName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetDefaultByteCodeName() const
{
	return	("ByteCode");
}

//-----------------------------------------------------------------------------
//	Name:		GetDefaultLuaFileName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetDefaultLuaFileName() const
{
	return	("Default.lua");
}

//-----------------------------------------------------------------------------
//	Name:		GetDefaultBigFileName
//	Object:		
//	04-01-26:	VMA - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetDefaultBigFileName() const
{
	return	("BigFile");
}

//-----------------------------------------------------------------------------
//	Name:		GetRuntimeVersionString
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetRuntimeVersionString() const
{
	QDT_STRING strTmp;

	strTmp.Format("Runtime PS3 (Build %s%s)", KERNEL::GetVersionString().GetBuffer(), KERNEL::GetServicePackString().GetBuffer());
	
	return	(strTmp);
}

//-----------------------------------------------------------------------------
//	Name:		GetClassCmdLine
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetClassCmdLine() const
{
	return	("CLASS=");
}

//-----------------------------------------------------------------------------
//	Name:		GetBCCmdLine
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetBCCmdLine() const
{
	return	("BC=");
}

//-----------------------------------------------------------------------------
//	Name:		GetLUACmdLine
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetLUACmdLine() const
{
	return	("LUA=");
}

//-----------------------------------------------------------------------------
//	Name:		GetBFCmdLine
//	Object:		
//	04-01-26:	VMA - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetBFCmdLine() const
{
	return	("BF=");
}

//-----------------------------------------------------------------------------
//	Name:		SetClassToBoot
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetClassToBoot(const QDT_STRING &	strClassName)
{
	_strClassToBoot = strClassName;
}

//-----------------------------------------------------------------------------
//	Name:		GetClassToBoot
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	RUNTIME_APP::GetClassToBoot() const
{
	return	(_strClassToBoot);
}

//-----------------------------------------------------------------------------
//	Name:		SetBCName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetBCName(const QDT_STRING &	strBCName)
{
	_strBCName = strBCName;
}

//-----------------------------------------------------------------------------
//	Name:		SetLuaFileName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetLuaFileName(const QDT_STRING &	strLuaFileName)
{
	_strLuaFileName = strLuaFileName;
}

//-----------------------------------------------------------------------------
//	Name:		GetBCName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	RUNTIME_APP::GetBCName() const
{
	return	(_strBCName);
}

//-----------------------------------------------------------------------------
//	Name:		GetBCName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	RUNTIME_APP::GetLuaFileName() const
{
	return	(_strLuaFileName);
}

//-----------------------------------------------------------------------------
//	Name:		SetBFName
//	Object:		
//	04-01-26:	VMA - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetBFName(const QDT_STRING &	strBFName)
{
	_strBFName = strBFName;
}

//-----------------------------------------------------------------------------
//	Name:		GetBFName
//	Object:		
//	04-01-26:	VMA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	RUNTIME_APP::GetBFName() const
{
	return	(_strBFName);
}

//-----------------------------------------------------------------------------
//	Name:		InitInstance
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitInstance(int argc, char * argv[])
{
#ifdef MEM_DUMP
	#pragma	QDT_FIXME("attention DUMP INFO memoire activé sur le runtime")
	FILE_STREAM	FSB("runtime_memory_before_init.txt", FILE_STREAM::AT_CREATE);
	MEM::Dump(FSB, -1 & (~MEM::DF_STACK_ALLOCATED_NODE));
#endif

	ERROR_MANAGER::Create();
	ERROR_MANAGER::Instance()->AddModule(new KCORE_ERROR_MODULE());

	FILES_MANAGER_ABC::Create();
	DATABASE_SERVICE::Create();
	COM_SERVICE::Create();
	LOADING_STACK::Create();
	LOADING_SCREEN_SERVICE::Create();

	FILES_MANAGER_ABC::Instance()->Init();

	// [GRI] as we got the option of loading bigfile by network, the init is postponed
	// after InitOS.
	//DATABASE_SERVICE::Instance()->Init();

	JOB_MANAGER::Init(NUM_PHYSICAL_SPU, NUM_RAW_SPU);
	
	// init COM
	MODULE_ABC::InitAll();

	COM_MODULE			*pComModule = new COM_MODULE();
	KERNEL_MODULE		*pKrnModule = new KERNEL_MODULE();
	MOTEUR_3D_MODULE	*pM3D_Module = new MOTEUR_3D_MODULE();
	MODULE_SCRIPT_TYPES *pMSType = new MODULE_SCRIPT_TYPES();

	VM_SERVICE::Create();

	//process command line
	ReadCommandLine(argc, argv);

	bool bInitIAMOk = false;
	bool bInitOsOk	= false;

	//	Init KRN , 3D Engine
	bInitOsOk = InitOs();

	// Init Database & BigFile
	DATABASE_SERVICE::Instance()->Init();
	DATABASE_SERVICE::Instance()->EnableServerAccess(false);

	//	Init Script Interface
	SCRIPT_MANAGER::Instance();

	//build script stuff
	bInitIAMOk = InitIAMRuntime();
	
	//now run
	if( bInitIAMOk && bInitOsOk )  
	{
		// does nothing 
	}
	else
	{
		//Get out we failed!
		return false;
	}

	// create GAME_MANAGER instance
	COM_STACK Stack;
	Stack.RunScript("QDT.GAME_MANAGER.New(\"RT_GameManager\", 0)");			// Create GAME_MANAGER instance
	Stack.RunScript("GameManager = QDR.RT_GameManager:ActiveInstance()");	// Register it as 'fake' lua singleton

//----------------------------------------------------
// test de remote read
//----------------------------------------------------
#ifdef TEST_REMOTE
	QDT_FLAGS	Flags = FILES_MANAGER_ABC::FO_FILE_REMOTE;
	FILES_MANAGER_ABC::FM_ERROR eError;
	FILE_ABC* pFile = FILES_MANAGER_ABC::Instance()->Open("c:/TestScript/blue.lua",FILE_ABC::FM_CREATE_READ_WRITE,eError,Flags);
	// as the open is now blocking, test here if open
	if (! pFile)
	{
		QDT_Message("! Can not open File");
	}
	else
	{
		GetServiceInterface()->Run(0); // force le flush car on est hors boucle main.
		static int toto = 454;
		while(! pFile->IsIdle())
		{
			toto++;
		}
		// lauch the script if no error
		if ( pFile->GetLastError() == FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
		{
			LUA_RunFile(pFile);
			QDT_Message(":-) TEST_REMOTE blue OK !!");
		}
		else
		{
			QDT_Message("! TEST_REMOTE blue has failed");
		}
	}
#endif 

	// start LUA file to run
	if (!_strLuaFileName.IsEmpty())
	{
		LUA_RunFile(_strLuaFileName);
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		ExitInstance
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::ExitInstance()
{
	ERROR_MANAGER::Release();

//#ifdef ENABLE_NETWORK
//#ifdef USE_NETWORK_THREAD
//		SERVICE_INTERFACE::Instance()->EndThread();
//#endif
//#endif 
}

//-----------------------------------------------------------------------------
//	Name:		InitOs
//	Object:	 Init 3D Engine, Sound, Network
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitOs()
{
	//init Kernel
	KERNEL::OpenTools();

//	OS::SetAppHandle((long)m_hInstance);
	KERNEL::Instance();

	//make a temp netsettings
	//QDT_NET_SETTINGS NetSettings;
#ifdef ENABLE_NETWORK
	KERNEL::Instance()->InitNetwork(_strFixedIPArg,_strRemoteFileServingRootDir);
#ifdef USE_NETWORK_THREAD
	SERVICE_INTERFACE::Instance()->StartThread();
#endif
	KERNEL::Instance()->LaunchQDTServices();
#endif 

	RENDER_CONTEXT::Init();

	// init M3D
	MOTEUR_3D::OpenTools();
	MOTEUR_3D::Instance();

	// init sound
	SOUND::Instance();
		
	return( true );	
}

//-----------------------------------------------------------------------------
//	Name:		InitIAMRuntime
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitIAMRuntime()
{
	QDT_STRING		strClassName;
	QDT_STRING		strMethodName;
	QDT_STRING		strBCName;
	QDT_STRING		strLuaFileName;
	QDT_STRING		strBFName;

	if	(GetBCName().IsEmpty())
	{
		strBCName = GetDefaultByteCodeName();
	}
	else
	{
		strBCName = GetBCName();
	}

	if	(GetLuaFileName().IsEmpty())
	{
		_strLuaFileName = GetDefaultLuaFileName();
	}
	else
	{
		_strLuaFileName = GetLuaFileName();
	}

	if	(GetBFName().IsEmpty())
	{
		strBFName = GetDefaultBigFileName();
	}
	else
	{
		strBFName = GetBFName();
	}

	COM_SERVICE::Instance()->SetDefaultName(strBFName);

//	DATABASE_MANAGER::Instance();

//	DYNAMIC_ALLOCATOR::GetMemPoolHeader();

	return( true );
}

//-----------------------------------------------------------------------------
//	Name:		OnIdle
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::OnIdle(long	lCount)
{
#ifndef _MASTER
	KERNEL::StartBoundFPS();
#endif

	//DATABASE_MANAGER::Instance()->Loop();
	
	// New COM Loop/Display
	MODULE_ABC::Loop();

	MODULE_ABC::Display();

#ifndef _MASTER
	KERNEL::WaitBoundFPS();
#endif

	if ( PAD_MANAGER_PS3::GetInstance()->GetPad(0)->IsButtonPressed(PAD_PS3::BT_R3) )
	{
		LUA_RunFile("Immediate.lua");
	}

#ifdef ENABLE_NETWORK
	GetServiceInterface()->Run(0);
#endif 
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		ReadCommandLine
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::ReadCommandLine(int		argc,
									 char *		argv[])
{
	int			i, nPos;
	QDT_STRING	strTmp;
	QDT_STRING	strClassName, strBCName, strIPAddress, strBFName, strLuaFileName;

	//__argc = 1 , is exe name
	for	(i = 1; i < argc; ++i)
	{
		strTmp = argv[i];

		//search for class
		nPos = strTmp.Find(GetClassCmdLine());
		if	(nPos != -1)
		{
			strClassName = strTmp.EraseLeft(nPos + GetClassCmdLine().GetSize());
			SetClassToBoot( strClassName );
		}

		//search for ByteCode
		nPos = strTmp.Find(GetBCCmdLine());
		if	(nPos != -1)
		{
			strBCName = strTmp.EraseLeft(nPos + GetBCCmdLine().GetSize());
			SetBCName(strBCName);
		}

		//search for ByteCode
		nPos = strTmp.Find(GetLUACmdLine());
		if	(nPos != -1)
		{
			strLuaFileName = strTmp.EraseLeft(nPos + GetLUACmdLine().GetSize());
			SetLuaFileName(strLuaFileName);
		}

		//search for BigFile
		nPos = strTmp.Find(GetBFCmdLine());
		if	(nPos != -1)
		{
			strBFName = strTmp.EraseLeft(nPos + GetBFCmdLine().GetSize());
			SetBFName(strBFName);
		}

		//search for fixed ip address
		QDT_STRING strFixedIPCommandLine = "FIXEDIP=";
		nPos = strTmp.Find(strFixedIPCommandLine);
		if	(nPos != -1)
		{
			_strFixedIPArg = strTmp.EraseLeft(nPos + strFixedIPCommandLine.GetSize());
		}

		// search for file serving root directory
		QDT_STRING strRemoteFileRootDirCommandLine = "REMOTEROOTDIR=";
		nPos = strTmp.Find(strRemoteFileRootDirCommandLine);
		if	(nPos != -1)
		{
			_strRemoteFileServingRootDir = strTmp.EraseLeft(nPos + strRemoteFileRootDirCommandLine.GetSize());
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		LUA_RunFile
//	Object:		
//	05-12-05:	NHO - Created
//-----------------------------------------------------------------------------
int RUNTIME_APP::LUA_RunFile(const char* sFilePath)
{
	COM_STACK Stack;

	Stack.RegisterFunction("print", LUA_Output);

	int nLUARunStatus = Stack.RunFile(sFilePath);
	if (nLUARunStatus != 0)
	{
		// error while parsing
		//QDT_STRING sMessage = Stack.GetErrorStr();
		//QDT_Error(sMessage);
	}
	else
	{
		QDT_Message("LUA File \"%s\" runned.", sFilePath);
	}

	return nLUARunStatus;
}

//-----------------------------------------------------------------------------
//	Name:		LUA_RunFile
//	Object:	the file passed in arg can be a remote one.	
//	06-03-14:	GRI - Created
//-----------------------------------------------------------------------------
int	RUNTIME_APP::LUA_RunFile(FILE_ABC *	pFile)
{
	COM_STACK Stack;

	Stack.RegisterFunction("print", LUA_Output);

	int nLUARunStatus = 	Stack.RunFile(pFile);
	if (nLUARunStatus != 0)
	{
		// error while parsing
		QDT_STRING sMessage = Stack.GetErrorStr();
		QDT_Error(sMessage);
	}
	else
	{
		QDT_Message("LUA File runned");
		//QDT_Message("LUA File \"%s\" runned.", sFilePath);
	}

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

//-----------------------------------------------------------------------------
//	Name:		main
//	Object:		
//	05-11-14:	VMA - Created
//-----------------------------------------------------------------------------
int	main(int argc, char * argv[])
{
	RUNTIME_APP Runtime;

	Runtime.InitInstance(argc, argv);	

	while (1)
	{
		Runtime.OnIdle(1);
	}

}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

