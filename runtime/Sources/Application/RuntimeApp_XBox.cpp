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

//#define USE_XBOX_DEBUG_CONSOLE

#include <xtl.h>
#include <xonline.h>
#include <Xhv.h>

#ifdef USE_XBOX_DEBUG_CONSOLE
#include <XbDm.h>
#endif

#include	"Include.h"

//Include KERNEL
#include	INC_3DENGINE(Display)
#include	INC_3DENGINE(Display/Viewport)
#include	INC_KERNEL(/)
#include	INC_KRNCORE(Stream)
#include	INC_KRNCOM(Database)
#include	INC_KRNCOM(DataBank)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCORE(Display/RenderManager)
#include	INC_KRNCORE(File/Xbox)
#include	INC_KRNCORE(Timer)
#include	INC_KRNSERV(Console)
#include	INC_KRNSERV(Input)
#include	INC_KRNSERV(Sound)
#include	INC_KRNSERV(CommandParser)
#include	INC_KRNCORE(Memory)
#include	INC_KRNSERV(KernelObject)
#include	INC_KRNSERV(KernelModule)

//VM inclusion
#include	INC_VM(/)
#include	INC_VM(Container)
#include	INC_VM(ModuleTypes)
using namespace QDT::VM;

//Include moteur 3d
#include	INC_3DENGINE(M3D)

// Script Interface
#include	INC_SCPTINT( ModuleScriptTypes )
#include	INC_SCPTINT( ScriptManager )
using namespace QDT::SINT;

#ifndef	_DEBUG
	#include	INL_3DENGINE(M3D)
#endif

#ifndef USE_XBOX_DEBUG_CONSOLE
void CommandProcessorLoop(void) {}
#else
#define MAXRCMDLENGTH       256
typedef void (*RCMDHANDLER)(int argc, char *argv[]);

struct REMOTE_COMMAND
{
    CHAR*       strCommand;                    // Name of command
    RCMDHANDLER pfnHandler;                    // Handler function
    CHAR*       strHelp;                       // Description of command
};

static const	CHAR				g_strDebugConsoleCommandPrefix[] = "XCMD";
static			CRITICAL_SECTION	g_CriticalSection;
static			int					g_CmdId			= -1;
static			char				g_CmdStr[256];

extern int nAllocated;
extern int nCreated;
extern int nSTLHeapMemUsage;


void DebugCmd_DataBank(int argc, char **argv);
void DebugCmd_MemStats(int argc, char **argv);
void DebugCmd_SndStats(int argc, char **argv);
void DebugCmd_Blur(int argc, char **argv);
void STLHeapDumpToFile(void);

const REMOTE_COMMAND g_RemoteCommands[] =
{
    // Command,  Handler,     Help string
    { "DB",		DebugCmd_DataBank,  " Displays stats about databanks" },
	{ "MEM",	DebugCmd_MemStats,	" Displays mem stats"},
	{ "SND",	DebugCmd_SndStats,	" Displays DX sound stats"},
	{ "BLUR",	DebugCmd_Blur,		" Adjusts blur live :P"}
};

const DWORD g_dwNumRemoteCommands = (sizeof(g_RemoteCommands)/sizeof(g_RemoteCommands[0]));
#endif


				bool				bExit = false;
static			DWORD				LaunchType;
static			LD_DEMO				g_DemoLauncherInfo;
static			int					g_XTLallocs		= 0;
static			MANAGED_POOL		*g_4096Blocks	= 0;
static			MANAGED_POOL		*g_128Blocks	= 0;
#define	NB_POOL4096_BLOCKS 100
#define	NB_POOL128_BLOCKS 500


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

RUNTIME_APP	RUNTIME_APP::RA;

MODULE_SCRIPT_TYPES	MSType;
MOTEUR_3D_MODULE	M3D_Module;
KERNEL_MODULE		KrnModule;
VM_MODULE_TYPES		VmModule;


//-----------------------------------------------------------------------------
//	Name:		RUNTIME_APP constructor
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
RUNTIME_APP::RUNTIME_APP()
:
_bQuit( false )
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
//	Name:		GetRuntimeVersionString
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_APP::GetRuntimeVersionString() const
{
	QDT_STRING strTmp;

	strTmp.Format("Runtime XBOX (Build:%s%s)", KERNEL::GetVersionString().GetBuffer(), KERNEL::GetServicePackString().GetBuffer());
	
	return	(strTmp);
}

//-----------------------------------------------------------------------------
//	Name:		InitInstance
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RUNTIME_APP::InitInstance()
{
	bool	bInitIAMOk, bInitOsOk;

	try
	{
		DATABASE::SetDefaultName("BigFile_XBOX");
		DATABASE_SERVICE::EnableServerAccess( false );

		CLASS_DESCRIPTOR::InitAll();

		//init KRN , 3D Engine
		bInitOsOk	= InitOs();

		if( XGetLaunchInfo( &LaunchType, (PLAUNCH_DATA)&g_DemoLauncherInfo ) != ERROR_SUCCESS )
			LaunchType = LDT_FROM_DASHBOARD;

		if( LaunchType == LDT_TITLE )
		{
			if( g_DemoLauncherInfo.dwRunmode == 0x02 )  // RUNMODE_USERSELECTED = 0x02
			{
				KERNEL::Instance()->SetDemoMode( 0 );
			}
			else
				KERNEL::Instance()->SetDemoMode( 1 );

			if( g_DemoLauncherInfo.dwTimeout )
				KERNEL::Instance()->SetDemoGameplayTimeout(((float)g_DemoLauncherInfo.dwTimeout)*1000.0f);
			else
				KERNEL::Instance()->SetDemoGameplayTimeout(-1.0f);

			int		nSize	= strlen(g_DemoLauncherInfo.szLaunchedXBE);
			char	*pScan	= g_DemoLauncherInfo.szLaunchedXBE + nSize;
			
			while(nSize)
			{
				if(*pScan == '\\')
					break;
				pScan--;
				nSize--;
			}

			if(nSize)
				FILE_XBOX::SetGamePath(g_DemoLauncherInfo.szLaunchedXBE, nSize+1);
		}

		if( FILES_MANAGER_ABC::IsFileExist("nodatabank") )
			DATABASE_SERVICE::EnableDataBank( false );
		else
			DATABASE_SERVICE::EnableDataBank( true );

		CLASS_DESCRIPTOR::InitAll();

		//DISPLAY_MANAGER::Instance()->EnableShadows(_RuntimeHeader.GetShadows());
		DISPLAY_MANAGER::Instance()->EnableShadows( true );

		//	Init Script Interface
		SCRIPT_MANAGER::Instance();

		//build script stuff
		bInitIAMOk	= InitIAMRuntime();

		AddAdditionnalBigfiles();
		
		//now run
		if( bInitIAMOk && bInitOsOk ) 
		{
			//tell the VM to run
			VM_OBJECT::DisableIAMOutput(true);
			
			//start the execution
			VM_OBJECT::Instance()->Run();
		}
		else
		{
			//Get out we failed!
			return( FALSE );
		}
	}
	
	catch( ... )
	{
	}

	return( true );
}

//-----------------------------------------------------------------------------
//	Name:		InitOs
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitOs()
{
	QDT_STRING	strText;

	try
	{
		//set sound volume to 0 so you can here some music while testing
		/*if ( _RuntimeHeader.GetNoSound() == true )
		{
			SOUND::Instance()->SetBaseVolume(0);
		}*/

		//init Kernel
		KERNEL::OpenTools();
		KERNEL::Instance();
				
		// init M3D
		MOTEUR_3D::OpenTools();
		MOTEUR_3D::Instance();

		//RENDER_MANAGER::Instance()->EnableFullScreen(true);
	}

	catch( ... )
	{
	}

	return( true );
	
}

//-----------------------------------------------------------------------------
//	Name:		InitIAMRuntime
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_APP::InitIAMRuntime()
{
	QDT_STRING	strClassName, strMethodName;

	try
	{
		VM_OBJECT::Instance()->ConfigureForRuntime();
		VM_OBJECT::Instance()->ReadByteCode();

		//	Add an interpreter with a constructor...
		VM_OBJECT::Instance()->BootDefault();

		//tell the VM to run
		VM_OBJECT::DisableIAMOutput(true);
		
		return( true );
	}
	catch( ... )
	{
	}

	return( true );
}

//-----------------------------------------------------------------------------
//	Name:		ExitInstance
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RUNTIME_APP::ExitInstance()
{
	FILES_MANAGER::Flush();
	if( LaunchType == LDT_TITLE )
	{
		XLaunchNewImage(g_DemoLauncherInfo.szLauncherXBE, (LAUNCH_DATA*)&g_DemoLauncherInfo);
	}
	else
	{
		LD_LAUNCH_DASHBOARD	LauchInfo;
		LauchInfo.dwContext		= 0;
		LauchInfo.dwReason		= XLD_LAUNCH_DASHBOARD_MAIN_MENU;
		LauchInfo.dwParameter1	= 0;
		LauchInfo.dwParameter2	= 0;
		XLaunchNewImage(NULL, (LAUNCH_DATA*)&LauchInfo);
	}
	return true;
}

#ifdef _MASTER
void __cdecl std::_Throw(class std::exception const &fuck)
{
}

void (*std::_Raise_handler)(class std::exception const &) = 0;
#endif

//-----------------------------------------------------------------------------
//	Name:		OnIdle
//	Object:		
//	01-09-17:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RUNTIME_APP::OnIdle(long	lCount)
{
	BOOL bResult = TRUE;
	
	try
	{
	#ifndef _MASTER
		KERNEL::StartBoundFPS();
	#endif
		//reflesh all OS stuff
		DATABASE_MANAGER::Instance()->Loop();
		MODULE_ABC::Loop();

		MODULE_ABC::Display();

		//if asked to quit
		if (_bQuit)
		{
			KERNEL::Instance()->Quit();
		}
	
	#ifndef _MASTER
		KERNEL::WaitBoundFPS();
	#endif
	}

	catch( ... )
	{
		//Do Nothing
	}

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
//	Name:		ReadInifile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	RUNTIME_APP::ReadInifile()
{
}

#ifdef USE_XBOX_DEBUG_CONSOLE
//-----------------------------------------------------------------------------
//	Name:		DebugConsolePrintf
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
BOOL DebugConsolePrintf( const CHAR* strFormat, ... )
{
    // Copy command prefix into buffer
    CHAR strBuffer[MAXRCMDLENGTH];
    int length = _snprintf( strBuffer, MAXRCMDLENGTH, "%s!", g_strDebugConsoleCommandPrefix );

    // Format arguments
    va_list arglist;
    va_start( arglist, strFormat );
    _vsnprintf( strBuffer + length, MAXRCMDLENGTH - length, strFormat, arglist );
    va_end( arglist );

    // Send it out the string
    DmSendNotificationString( strBuffer );
    
    return TRUE;
}

//-----------------------------------------------------------------------------
//	Name:		dbgtolower
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
inline CHAR dbgtolower( CHAR ch )
{
    if( ch >= 'A' && ch <= 'Z' )
        return ch - ( 'A' - 'a' );
    else
        return ch;
}

//-----------------------------------------------------------------------------
//	Name:		dbgstrnicmp
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
BOOL dbgstrnicmp( const CHAR* str1, const CHAR* str2, int n )
{
    while( ( dbgtolower( *str1 ) == dbgtolower( *str2 ) ) && *str1 && n > 0 )
    {
        --n;
        ++str1;
        ++str2;
    }

    return( n == 0 || dbgtolower( *str1 ) == dbgtolower( *str2 ) );
}

//-----------------------------------------------------------------------------
//	Name:		dbgstrcpy
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
VOID dbgstrcpy( CHAR* strDest, const CHAR* strSrc )
{
    while( ( *strDest++ = *strSrc++ ) != 0 );
}

//-----------------------------------------------------------------------------
//	Name:		dbgstrlen
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
int dbgstrlen( const CHAR* str )
{
    const CHAR* strEnd = str;

    while( *strEnd )
        strEnd++;

    return strEnd - str;
}

//-----------------------------------------------------------------------------
//	Name:		DebugCmd_Blur
//	Object:		
//	05-02-23:	JPE - Created
//-----------------------------------------------------------------------------
void DebugCmd_Blur(int argc, char **argv)
{
	VIEWPORT_LIST	*pViewportList = DISPLAY_MANAGER::Instance()->GetViewportList();

	float	BlendingFactor, OffsetX, OffsetY, nbPasses;
	int		BlurType;

	sscanf(g_CmdStr, "blur %d %f %f %f %f", &BlurType, &BlendingFactor, &OffsetX, &OffsetY, &nbPasses);

	if(pViewportList)
	{
		int iViewport;

		for(iViewport=0; iViewport<pViewportList->GetNbrViewports(); iViewport++)
		{
			VIEWPORT *pViewport = pViewportList->GetViewport(iViewport);

			pViewport->SetBlurMode(BlurType);
			pViewport->SetBlurBlendingFactor(BlendingFactor);
			pViewport->SetBlurXOffset(OffsetX);
			pViewport->SetBlurYOffset(OffsetY);
			pViewport->SetBlurNbrPasses(nbPasses);
		}	
	}
}

//-----------------------------------------------------------------------------
//	Name:		DebugCmd_DataBank
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void DebugCmd_DataBank(int argc, char **argv)
{
	int nSize = DATA_BANK::GetNbLoadedDataBanks();

	DebugConsolePrintf("%d Databank loaded (%d)Kb", nSize, DATA_BANK::GetTotalLoadedSize() / 1024);
	for( int i = 0 ; i < nSize ; i++ )
	{
		DebugConsolePrintf("%s: %d Kb", DATA_BANK::GetLoadedDataBankName(i).GetBuffer(), DATA_BANK::GetLoadedDataBankSize(i) / 1024 );
	}
}

//-----------------------------------------------------------------------------
//	Name:		DebugCmd_DataBank
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void DebugCmd_MemStats(int argc, char **argv)
{
	MEM_STATS	MemStatStruct;
	MEM::GetMemStats(MemStatStruct);

	DebugConsolePrintf("Writing memory dump file");

	MEM::DumpMemoryImageOnFile();
	STLHeapDumpToFile();

	DebugConsolePrintf("Total alloc (XTL+headers) : %iKb", (g_XTLallocs+MemStatStruct._nTotalAllocatedMemHeader+5*1024*1024) / 1024);
	DebugConsolePrintf("XTL allocs : %iKb", g_XTLallocs / 1024);
	DebugConsolePrintf("STL allocs : %iKb / 5120Kb", nSTLHeapMemUsage / 1024);
	DebugConsolePrintf("Alloc+XTL: %iKb", (MemStatStruct._nSizeAllocatedMem+g_XTLallocs) / 1024);
	DebugConsolePrintf("Sum Allocated: %iKb", MemStatStruct._nSizeAllocatedMem / 1024);
	DebugConsolePrintf("Tot Headers: %iKb", MemStatStruct._nTotalAllocatedMemHeader / 1024);
	DebugConsolePrintf("Sum Free: %iKb", MemStatStruct._nSizeMaxMem / 1024);
	DebugConsolePrintf("Max allocable block: %iKb", MemStatStruct._nSizeMaxBlock / 1024);
	DebugConsolePrintf("Blocks (Free/Allocated): %i", MemStatStruct._nNbrBlocks);
	DebugConsolePrintf("Average Max used: %iKb", MemStatStruct._nMaxMemoryUsed / 1024);
	DebugConsolePrintf("Average current used: %iKb", MemStatStruct._nCurrentMemoryUsed / 1024);
	DebugConsolePrintf("Tot Allocator + Buffer: %iKb", MemStatStruct._nSizeMemAllocator / 1024);
	DebugConsolePrintf("Wasted: %iKb", MemStatStruct._nWastedSize / 1024);
	DebugConsolePrintf("---");
	DebugConsolePrintf("ComHandles: %iKb", COM_HANDLE::GetTotalAllocatedSize() / 1024 );
	DebugConsolePrintf("ByteCode: %iKb", ( BYTE_CODE::GetTotalAllocatedSize() ) / 1024 );
	DebugConsolePrintf("VM Structure: %iKb", ( CLASS_VM::GetTotalAllocatedSize() ) / 1024 );
	DebugConsolePrintf("Streaming: %iKb", COMMUNICATOR_PARTITION::GetTotalAllocatedSize() / 1024 );
	DebugConsolePrintf("ClassDescriptors: %iKb", CLASS_DESCRIPTOR::GetMemoryUsed() / 1024 );
	DebugConsolePrintf("VM Stack: Def:%i Max:%i NbTH: %i", INTERPRETER::GetDefaultStackSize(), INTERPRETER::GetMaxStackUsed(), SCHEDULER::GetNbrInterpreters());
	DebugConsolePrintf("MemPools: %iKb", MEM_POOL_HEADER::GetMemoryUsed() / 1024 );
	
	DebugConsolePrintf("SoundFx running : %d", nCreated);
	DebugConsolePrintf("Sound   running : %d", nAllocated);
}

//-----------------------------------------------------------------------------
//	Name:		DebugCmd_SndStats
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
void DebugCmd_SndStats(int argc, char **argv)
{
	DirectSoundDumpMemoryUsage(false);
}

//-----------------------------------------------------------------------------
//	Name:		DebugConsoleCmdProcessor
//	Object:		
//	04-09-01:	JPE - Created
//-----------------------------------------------------------------------------
HRESULT __stdcall DebugConsoleCmdProcessor( const	CHAR*		strCommand,
													CHAR*		strResponse, 
													DWORD		dwResponseLen,
													PDM_CMDCONT	pdmcc )
{
    strCommand += strlen(g_strDebugConsoleCommandPrefix) + 1;

    if( dbgstrnicmp( strCommand, "__connect__", 11 ) )
    {
        lstrcpynA( strResponse, "Connected to QDT_Engine", dwResponseLen );
        return XBDM_NOERR;
    }

    int CmdID = -1;

    for( DWORD i = 0; i < g_dwNumRemoteCommands; i++ )
    {
        if( dbgstrnicmp( g_RemoteCommands[i].strCommand, strCommand, dbgstrlen( g_RemoteCommands[i].strCommand ) ) )
        {
			CmdID = i;
			break;
        }
    }

    if( CmdID != -1 )
    {
        EnterCriticalSection( &g_CriticalSection );
        if( g_CmdId != -1 )
            dbgstrcpy( strResponse, "Cannot execute - previous command still pending" );
        else
		{
			g_CmdId = CmdID;
			dbgstrcpy(g_CmdStr, strCommand);
		}
		LeaveCriticalSection( &g_CriticalSection );
    }
    else
    {
		dbgstrcpy( strResponse, "QDT_Engine: unknown command" );
    }

    return XBDM_NOERR;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	04-10-12:	JPE - Created
//-----------------------------------------------------------------------------
void CommandProcessorLoop(void)
{
	if( g_CmdId != -1 )
	{
		int LocalId;
		
		EnterCriticalSection( &g_CriticalSection );
		LocalId = g_CmdId;
		g_CmdId = -1;
		LeaveCriticalSection( &g_CriticalSection );

		g_RemoteCommands[LocalId].pfnHandler( 0, 0 );
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Custom allocator
//	Object:		
//	03-05-12:	JPE - Created
//-----------------------------------------------------------------------------
SIZE_T WINAPI XMemSize( IN VOID* pAddress, IN DWORD dwAllocAttributes )
{
    return XMemSizeDefault( pAddress, dwAllocAttributes );
}

static	int	PhysAllocs = 0;
static	int	HeapAllocs = 0;
//-----------------------------------------------------------------------------
//	Name:		Custom allocator
//	Object:		
//	03-05-12:	JPE - Created
//-----------------------------------------------------------------------------
VOID WINAPI XMemFree( PVOID pBaseAddress, DWORD dwAllocAttributes )
{
	if( g_4096Blocks)
	{
		if( g_128Blocks->Free(pBaseAddress) )
			return;
		if( g_4096Blocks->Free(pBaseAddress) )
			return;
	}

	XALLOC_ATTRIBUTES*	pAttributes = (XALLOC_ATTRIBUTES*)&dwAllocAttributes;
	int					Size		= XMemSize(pBaseAddress, dwAllocAttributes);

	g_XTLallocs -= Size;

	if( pAttributes->dwMemoryType == XALLOC_MEMTYPE_HEAP )
		HeapAllocs -= Size;
	else
		PhysAllocs -= Size;

    XMemFreeDefault( pBaseAddress, dwAllocAttributes );

    return;
}

//-----------------------------------------------------------------------------
//	Name:		Custom allocator
//	Object:		
//	03-05-12:	JPE - Created
//-----------------------------------------------------------------------------
VOID* WINAPI XMemAlloc( SIZE_T dwSize, DWORD dwAllocAttributes )
{
    XALLOC_ATTRIBUTES* pAttributes = (XALLOC_ATTRIBUTES*)&dwAllocAttributes;
    VOID*              pvBuffer    = NULL;

	if( pAttributes->dwMemoryType == XALLOC_MEMTYPE_HEAP )
		HeapAllocs += dwSize;
	else
	{
		if(!g_4096Blocks)
		{
			g_4096Blocks	= (MANAGED_POOL*)0xffffffff;
			pvBuffer		= D3D_AllocContiguousMemory(4096*NB_POOL4096_BLOCKS, D3DTEXTURE_ALIGNMENT);
			g_4096Blocks	= new MANAGED_POOL(4096, NB_POOL4096_BLOCKS, pvBuffer);
			pvBuffer		= D3D_AllocContiguousMemory(128*NB_POOL128_BLOCKS, D3DTEXTURE_ALIGNMENT);
			g_128Blocks		= new MANAGED_POOL(128, NB_POOL128_BLOCKS, pvBuffer);
		}
		void *pRet = 0;
		if(dwSize < 128)
			pRet = g_128Blocks->Alloc();
		else if(dwSize < 4096)
			pRet = g_4096Blocks->Alloc();
		if(pRet)
			return pRet;
		PhysAllocs += dwSize;
	}

    pvBuffer	= XMemAllocDefault( dwSize, dwAllocAttributes );
	dwSize		= XMemSize(pvBuffer, dwAllocAttributes);

	g_XTLallocs += dwSize;

    return pvBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		main
//	Object:		
//	03-05-12:	JPE - Created
//-----------------------------------------------------------------------------
void __cdecl main(void) 
{
	RUNTIME_APP		runtime;

#ifdef USE_XBOX_DEBUG_CONSOLE
    DmRegisterCommandProcessor( g_strDebugConsoleCommandPrefix, DebugConsoleCmdProcessor );
    InitializeCriticalSection( &g_CriticalSection );
#endif

	runtime.InitInstance();

	while(bExit==false)
	{
		runtime.OnIdle(1);

#ifdef USE_XBOX_DEBUG_CONSOLE
		CommandProcessorLoop();
#endif
	}

	runtime.ExitInstance();
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

	sBFName = DATABASE::GetDefaultName();
	nCurNum = 1;
	bool bStop	= false;
	if	(sBFName.IsEmpty() == false)
	{
		while(bStop == false)
		{
			sNewName.Format("%s%i.idx", sBFName.GetBuffer(), nCurNum);
			if	(FILES_MANAGER_ABC::IsFileExist(sNewName))
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

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

