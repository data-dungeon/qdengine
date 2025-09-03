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

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//Include KERNEL
#include	INC_KERNEL(/)
#include	INC_KRNCORE(Stream)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(Database)
#include	INC_KRNCORE(Display/RenderManager)
#include	INC_KRNCORE(File/PS2)
#include	INC_KRNSERV(Console)
#include	INC_KRNSERV(Input)
#include	INC_KRNSERV(Sound)
#include	INC_KRNSERV(CommandParser)
#include	INC_KRNCORE(Memory)
#include	INC_KRNSERV(KernelObject)
#include	INC_KRNSERV(KernelModule)
#include	INC_KRNCOM(DataBank)

//VM inclusion
#include	INC_VM(\)
#include	INC_VM(Container)
#include	INC_VM(ComObject)
#include	INC_VM(ModuleTypes)

//Include moteur 3d
#include INC_3DENGINE(M3D)
#include INC_3DENGINE(Display)
#include INC_3DENGINE(Interface2D3D/IHM/IHM_Elements)


// Script Interface
#include	INC_SCPTINT(ModuleScriptTypes)
#include	INC_SCPTINT(ScriptManager)
#include	INC_SCPTINT(GameConfig)

#include INC_KRNCORE(Math/Geometry/Matrix)
#include INC_KRNCORE(Math/Geometry/More)
#include INC_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point4D)
#include INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
#include INC_KRNCORE(Math/Geometry/Quaternions)

#ifndef	_DEBUG
	#include	INL_3DENGINE(M3D)
	#include	INL_3DENGINE(Display)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/More)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point4D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Quaternions)
	#include	INL_KRNCORE(Display\RenderManager)
	#include	INL_SCPTINT(GameConfig)
#endif

#ifdef PS2_DEMO
	#include <libscedemo.h>
#endif

	using namespace QDT::KCORE;
	using namespace QDT::KSERV;
	using namespace QDT::KRN;
	using namespace QDT::M3D;
	using namespace QDT::VM;
	using namespace QDT::SINT;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

RUNTIME_APP	RUNTIME_APP::RA;


char		RUNTIME_APP::_s_sElfArgs[ 128 ] __attribute__((section(".args"))) = "CLASS=Global BF=BigFile IP=192.168.0.124";

/*
MODULE_SCRIPT_TYPES	MSType;
MOTEUR_3D_MODULE	M3D_Module;
KERNEL_MODULE		KrnModule;
VM_MODULE_TYPES		VmModule;
*/


//******* DUMP INFO MEMOIRE ***************/
//#define MEM_DUMP
//******* DUMP INFO MEMOIRE ***************/

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

	strTmp.Format("Runtime PS2 (Build %s%s)", KERNEL::GetVersionString().GetBuffer(), KERNEL::GetServicePackString().GetBuffer());
	
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
//	Name:		GetDBCmdLine
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetDBCmdLine() const
{
	return	("DB");
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowCmdLine
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetWindowCmdLine() const
{
	return	("WINDOW");
}

//-----------------------------------------------------------------------------
//	Name:		GetSoundCmdLine
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetSoundCmdLine() const
{
	return	("NOSOUND");
}


//-----------------------------------------------------------------------------
//	Name:		GetIPCmdLine
//	Object:		
//	03-04-23:	VMA - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetIPCmdLine() const
{
	return	("IP=");
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
//	Name:		GetNoShadowsCmdLine
//	Object:		
//	04-05-11:	SBE - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetNoShadowsCmdLine() const
{
	return	("NOSHADOWS");
}

//-----------------------------------------------------------------------------
//	Name:		GetNoDatabankCmdLine
//	Object:		
//	04-05-11:	SBE - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetNoDatabankCmdLine() const
{
	return	("NODATABANK");
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
//	Name:		GetBCName
//	Object:		
//	02-03-04:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	RUNTIME_APP::GetBCName() const
{
	return	(_strBCName);
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
//	Name:		SetIPAddress
//	Object:		
//	03-04-23:	VMA - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetIPAddress(const QDT_STRING &	strIPAddress)
{
	FILE_PS2_ATMON::SetHostIPAddress( strIPAddress.GetBuffer() );
}

//-----------------------------------------------------------------------------
//	Name:		RUNTIME_APP constructor
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::RUNTIME_APP()
:
_bQuit( false ),
_bFullScreen(true)
{

}

//-----------------------------------------------------------------------------
//	Name:		~RUNTIME_APP destructor
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::~RUNTIME_APP()
{

}

//-----------------------------------------------------------------------------
//	Name:		InitOs
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitOs()
{
	#if defined(PS2_PROD_BUILD)
		RENDER_MANAGER::SetDefaultScreenSignal(RENDER_MANAGER::NTSC);		
	#else
		RENDER_MANAGER::SetDefaultScreenSignal(RENDER_MANAGER::PAL); // passer en NTCS pr le Build US/Jap..
		//RENDER_MANAGER::SetDefaultScreenSignal(RENDER_MANAGER::NTSC);		
	#endif

	KERNEL::SetInitSound(false);

	// initialize all COM types
	CLASS_DESCRIPTOR::InitAll();

	//init Kernel
	KERNEL::OpenTools();

//	OS::SetAppHandle((long)m_hInstance);
	KERNEL::Instance();

	//set the text of the window
	RENDER_MANAGER::Instance()->GetRenderContext()->SetRuntimeVersionString(GetRuntimeVersionString());

	//M3D_DISPLAY config , size and window mode
	int nNbrModes = RENDER_MANAGER::Instance()->GetRenderContext()->GetAdapterModesCount( 0 );

/*	for( int i = 0; i < nNbrModes ; ++i )
	{
		RENDER_MODE renderMode;
		RENDER_MANAGER::Instance()->GetRenderContext()->GetAdapterMode(0, i, renderMode );

		if( ( renderMode.Depth == 4 )	&& 
			( renderMode.Width == 640 )  &&
			( renderMode.Height == 448 )
			)
		{
			RENDER_MANAGER::Instance()->SelectCurrentAdapterMode( i );
			break;
		}
	}*/

	//	No "WINDOW" on command line so enable fullscreen
	DISPLAY_MANAGER::Instance()->SetFullScreen( true );

	DISPLAY_MANAGER::Instance()->EnableShadows(AreShadowsEnabled());

	// init M3D
	MOTEUR_3D::OpenTools();
	MOTEUR_3D::Instance();	
	
	#if defined(PS2_PROD_BUILD)
		DATABASE_SERVICE::EnableDataBank(false);
	#elif defined(PS2_CDVDBOOT_BUILD)
		DATABASE_SERVICE::EnableDataBank(true);
		QDT::KSERV::INPUT::Instance()->EnableVibration(true);
		#ifdef _MASTER_FINAL_BUILD
			GAME_CONFIG::Instance()->SetCheatMode(false);
		#else
			GAME_CONFIG::Instance()->SetCheatMode(true);
		#endif
	#else
		// enable databank a true par defaut (modifiable par ligne de commande : NODATABANK)
		QDT::KSERV::INPUT::Instance()->EnableVibration(true);
		GAME_CONFIG::Instance()->SetCheatMode(true);
	#endif	

	return( true );	
}

//-----------------------------------------------------------------------------
//	Name:		InitIAMRuntime
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitIAMRuntime()
{
	QDT_STRING		strClassName;
	QDT_STRING		strMethodName;
	QDT_STRING		strBCName;
	QDT_STRING		strBFName;

	#if defined(PS2_CDVDBOOT_BUILD)
		strBCName = GetDefaultByteCodeName();
		strBFName = GetDefaultBigFileName();			
	#else
		if	(GetBCName().IsEmpty())
		{
			strBCName = GetDefaultByteCodeName();
		}
		else
		{
			strBCName = GetBCName();
		}

		if	(GetBFName().IsEmpty())
		{
			strBFName = GetDefaultBigFileName();
		}
		else
		{
			strBFName = GetBFName();
		}
	#endif

	DATABASE::SetDefaultName(strBFName);

	VM_OBJECT::ConfigureForRuntime();

	//Read bytecode
	VM_OBJECT::Instance()->ReadByteCode();

	//	Add an interpreter with a constructor...
	VM_OBJECT::Instance()->BootDefault();

	//tell the VM to run
	VM_OBJECT::DisableIAMOutput(true);

	SCHEDULER::ReserveInterpreters();
	DYNAMIC_ALLOCATOR::GetMemPoolHeader();

	IHM_ELEMENT_TEXT::ReserveParser();

	return( true );
}

//-----------------------------------------------------------------------------
//	Name:		InitInstance
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitInstance(int argc, char *argv[]) 
{
	bool bResult = FALSE;
	
#ifdef MEM_DUMP
	#pragma	QDT_FIXME("attention DUMP INFO memoire activé sur le runtime")
	FILE_STREAM	FSB("runtime_memory_before_init.txt", FILE_STREAM::AT_CREATE);
	MEM::Dump(FSB, -1 & (~MEM::DF_STACK_ALLOCATED_NODE));
#endif

	// init COM
	MODULE_ABC::InitAll();

	COM_MODULE			*pComModule = new COM_MODULE();
	IAM_MODULE_TYPES	*pIAMModule = new IAM_MODULE_TYPES();
	MODULE_SCRIPT_TYPES *pMSType = new MODULE_SCRIPT_TYPES();
	MOTEUR_3D_MODULE	*pM3D_Module = new MOTEUR_3D_MODULE();
	KERNEL_MODULE		*pKrnModule = new KERNEL_MODULE();
	VM_MODULE_TYPES		*pVmModule = new VM_MODULE_TYPES();
	//MODULE_NON_REGRESSION	*pMNR = new MODULE_NON_REGRESSION();


//	DATABASE_MANAGER::EnableDatabase(false);
	EnableShadows(true);
	DATABASE_SERVICE::EnableDataBank(true);

	//process command line
#if defined PS2_HOST
	ReadCommandLine(argc, argv);
#elif defined PS2_ATMON || defined PS2_CDVD
	ReadElfArguments( _s_sElfArgs );
#endif

	bool bInitIAMOk = false;
	bool bInitOsOk = false;

	// 
	DATABASE_SERVICE::EnableServerAccess(false);

	//	Init KRN , 3D Engine
	bInitOsOk = InitOs();

	//	Init Script Interface
	SCRIPT_MANAGER::Instance();

	//build script stuff
	bInitIAMOk = InitIAMRuntime();
	
	//now run
	if( bInitIAMOk && bInitOsOk )  
	{
		//close console during execution, too much garbage in it ,
		//slow the thing down..
		//CONSOLE::Instance()->Close();
		//CONSOLE::Suicide();

		//tell the VM to run
		VM_OBJECT::DisableIAMOutput(true);

		//start the execution
		VM_OBJECT::Instance()->Run();
		
		//show the window
/*		HWND DispHwnd = (HWND) RENDER_MANAGER::Instance()->GetDisplayWindow(); 
		::SendMessage( DispHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
		::ShowWindow( DispHwnd, SW_SHOW );
		::SetFocus( DispHwnd );*/
	}
	else
	{
		//Get out we failed!
		return( FALSE );
	}

	return true;//( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		ExitInstance
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::ExitInstance()
{
/*	bool bResult = FALSE;

	// Destroy Modules
	MOTEUR_3D::Suicide();
	KERNEL::Suicide() ;
	MOTEUR_3D::CloseTools();
	KERNEL::CloseTools();
	

#ifdef MEM_DUMP	
#pragma	QDT_FIXME("attention DUMP INFO memoire activé sur le runtime")
	FILE_STREAM	FSA("runtime_memory_after_init.txt", FILE_STREAM::AT_CREATE);
	QDT::KRN::MEM::Dump(FSA, -1 & (~MEM::DF_STACK_ALLOCATED_NODE));
#endif

//		 bResult = WINDOW_APPLICATION::ExitInstance();

	return ( bResult );*/
}

//-----------------------------------------------------------------------------
//	Name:		OnIdle
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::OnIdle(long	lCount)
{

#ifndef _MASTER
		KERNEL::StartBoundFPS();
#endif

	bool bResult = TRUE;

//	WINDOW_APPLICATION::OnIdle(lCount);

	//reflesh all OS stuff
	DATABASE_MANAGER::Instance()->Loop();
	
	// New COM Loop/Display
	MODULE_ABC::Loop();

//	SCRIPT_MANAGER::Instance()->Loop(); //??? plante le skinning !! a revoir apres un rebuild all pas le temps de le faire

	MODULE_ABC::Display();

	//if asked to quit
	if (_bQuit)
	{
		KERNEL::Instance()->Quit();
	}

#ifndef _MASTER
		KERNEL::WaitBoundFPS();
#endif

#if	0
	static int	nCount = 0;
	++nCount;

	char	sBuffer[256];

	if	((nCount % 200) == 0)
	{
		sprintf(sBuffer, "file_%09d.txt", nCount);

		FILE_TEXT_STREAM	F(sBuffer, FILE_STREAM::AT_CREATE);

		MEM::Dump(F, MEM::DF_ALLOCATED_NODE | MEM::DF_FREE_NODE | MEM::DF_SIZE_NODE | MEM::DF_STATE_NODE | MEM::DF_HEADER_STATS | MEM::DF_SOURCE_ALLOCATED_NODE);
	}
#endif

	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		Run
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
int	RUNTIME_APP::Run()
{
	return 1;
}

//-----------------------------------------------------------------------------
//	Name:		ReadCommandLine
//	Object:		
//	01-12-13:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::ReadCommandLine(int argc, char *argv[]) 
{
	int			i, nPos;
	QDT_STRING	strTmp;
	QDT_STRING	strClassName, strBCName, strIPAddress, strBFName;

	//__argc = 1 , is exe name
	for	(i = 1; i < argc; ++i)
	{
		strTmp = argv[i];

		//go for Window mode
		if	(strTmp == GetWindowCmdLine())
		{
			SetFullScreen(false);
		}
		
		//when you want database connection to be active
		if	(strTmp == GetDBCmdLine())
		{
//			DATABASE_MANAGER::EnableDatabase(true);
		}

		//set sound volume to 0 so you can here some music while testing
		if	(strTmp == GetSoundCmdLine())
		{
//			SOUND::Instance()->SetBaseVolume(0);
		}

		if	(strTmp == GetNoShadowsCmdLine())
		{
			EnableShadows(false);
		}

		if	(strTmp == GetNoDatabankCmdLine())
		{
			DATABASE_SERVICE::EnableDataBank(false);
		}		

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

		//search for IP address
		nPos = strTmp.Find(GetIPCmdLine());
		if	(nPos != -1)
		{
			strIPAddress = strTmp.EraseLeft(nPos + GetIPCmdLine().GetSize());
			SetIPAddress(strIPAddress);
		}

		//search for BigFile
		nPos = strTmp.Find(GetBFCmdLine());
		if	(nPos != -1)
		{
			strBFName = strTmp.EraseLeft(nPos + GetBFCmdLine().GetSize());
			SetBFName(strBFName);
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		SetFullScreen
//	Object:		
//	02-05-28:	VMA - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::ReadElfArguments(char* args)
{
	char sClassName[128];
	char sIPAddress[128];
	sscanf(args, "CLASS=%s IP=%s", sClassName, sIPAddress );
	
	SetClassToBoot( sClassName );
	
	SetIPAddress(sIPAddress);
}

//-----------------------------------------------------------------------------
//	Name:		SetFullScreen
//	Object:		
//	01-12-13:	DAC - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::SetFullScreen(const bool	bFlag)
{
	_bFullScreen = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		IsFullScreen
//	Object:		
//	01-12-13:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::IsFullScreen() const
{
	return	(_bFullScreen);
}

//-----------------------------------------------------------------------------
//	Name:		EnableShadows
//	Object:		
//	04-05-11:	SBE - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::EnableShadows(const bool	bFlag)
{
	_bShadowsEnabled = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		AreShadowsEnabled
//	Object:		
//	04-05-11:	SBE - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::AreShadowsEnabled() const
{
	return _bShadowsEnabled;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
int main(int argc, char *argv[]) 
{
	RUNTIME_APP runtime;

	runtime.InitInstance(argc, argv);	

//	MEM_STATS	MemStatStruct;
//	MEM::GetMemStats(MemStatStruct);
//	printf("*************************\n Mem Free = %dko\n*************************\n", MemStatStruct._nTotalAllocatedMemHeader / 1024);

#ifdef PS2_DEMO
	unsigned short language, playmode, to_inactive, to_gameplay;

	sceDemoStart(argc, argv, &language, NULL, &playmode, &to_inactive, &to_gameplay, NULL, NULL, NULL );

	KERNEL::Instance()->SetDemoLanguage( (language==SCE_DEMO_LANGUAGE_FRENCH)?LOCALIZATION_LANG_FRA:LOCALIZATION_LANG_ENG );
	KERNEL::Instance()->SetDemoMode((int)playmode);
	KERNEL::Instance()->SetDemoInactivityTimeout((float)to_inactive);
	KERNEL::Instance()->SetDemoGameplayTimeout((float)to_gameplay);
#endif

	while (1)
	{
		runtime.OnIdle(1);
	}

}