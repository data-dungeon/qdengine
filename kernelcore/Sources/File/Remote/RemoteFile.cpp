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
//	CLASS:	REMOTE_FILE
//
//	02-02-14:	GRI - Created 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(File/Remote/RemoteFile)
#include	INCL_KCORE(File/FilesManagerABC)

#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(OS/OS)

#ifdef PLATFORM_PC
#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)
#endif 

#define CHECK_SERVICE_OK 	if (! IsServiceOk()) { QDT_ASSERT(false); }

#define NET_VERBOSE

//-----------------------------------------------------------------------------
//	Name:		FILE_PC constructor
//	Object:		
//	02-02-14:	GRI - Created
//-----------------------------------------------------------------------------
REMOTE_FILE::REMOTE_FILE():
	_nReadSize(0),
	_nReadPos(0),
	_nCurPos(0),
	_pReadBuffer(0),
	_rLastReadTime(0.f),
	_bIdle(true),
	_nHandleClient(-1),
	_bBlocked(false)
	//_nLastError(FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
{
	QDT_ASSERT(FILES_MANAGER_ABC::Instance() );
	_pRemoteFileService = FILES_MANAGER_ABC::Instance()->GetRemoteFileService();
}

//-----------------------------------------------------------------------------
//	Name:		FILE_PC destructor
//	Object:		
//	02-02-14:	GRI - Created
//-----------------------------------------------------------------------------
REMOTE_FILE::~REMOTE_FILE()
{
}

//-----------------------------------------------------------------------------
//	Name:			Open
//	Object:		Not Blocking -
//						Check the prerequesites (service up etc.)
//	02-02-18:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Open(const char * const	pFileName,
												const F_MODE			eMode,
												const bool			bSharing)
{
#ifdef PLATFORM_PC
	QDT_ASSERT( ! TestStatus(F_STATUS::FS_OPEN) );
#endif 
	
	if ( ! IsServiceOk() )
	{
		_pRemoteFileService->CallNetworkPrintf(" ! REMOTE_FILE::Open : Service not available ??");
		return false;
	}

	SetName(pFileName);

#ifdef PLATFORM_PC
	if (  TestStatus(F_STATUS::FS_OPEN) )	// error if already open
	{
		//QDT_STRING sError;
		//sError.Format("File % allready open:", pFileName );
		_pRemoteFileService->CallNetworkPrintf("File %s allready open:", pFileName);
		return false;
	}
	EnableStatus(F_STATUS::FS_OPEN);
#endif 

	Int32 nDesiredAccess = 0;
	nDesiredAccess			= eMode;

	// ask the service to open the file in remote.
	// We keep a handle correponding to that file.
	_nHandleClient = _pRemoteFileService->RemoteOpen(pFileName,nDesiredAccess,this);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:	Blocking or not blocking (depends on F_SYNC flag)	
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Read(void*	pBuffer,
												const UInt32	uPosition,
												const Int32		nSize,
												const F_SYNC	eSync)
{
	if ( !_pRemoteFileService || !_pRemoteFileService->GotALink()	) 
	{
		_pRemoteFileService->CallNetworkPrintf("No link => no transfer");
		_bBlocked = false;
		return false;
	}
	QDT_ASSERT(_nHandleClient != -1);

	// ask the service if it safe to read.
	//_pRemoteFileService->WaitNoOP();
	
	static volatile int toto = 0;
	//while (_bBlocked)
	//{
	//	toto++;
	//	_pRemoteFileService->CallNetworkPrintf("Je suis bloqué !!!");
	//}
	//_bBlocked = true;

	_pRemoteFileService->CallNetworkPrintf("Read in progress !!!");

	while(!_bIdle)
	{
		_pRemoteFileService->CallNetworkPrintf("Je suis bloqué !!!");
	}
	//_pRemoteFileService->CallNetworkPrintf("Je ne suis pas bloqué !!!");

	// ask the service to read.
	_pRemoteFileService->RemoteRead(pBuffer,uPosition,nSize,_nHandleClient);

	// if not blocking, return true, user have to wait until idle.
	//if (eSync == FS_WAIT )
	if (false)
	{
		_bBlocked = false;
		return true;
	}
	
	//  blocking : wait the block being read.
#ifdef NET_VERBOSE
		//_pRemoteFileService->CallNetworkPrintf(".REMOTE_FILE : Blocking read");
#endif 

	while(!_bIdle)
	{
		toto++;
		//_pRemoteFileService->CallNetworkPrintf("Je suis bloqué !!!");
	}
	
	// return from remote read
	if (GetLastError() != FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
	{
		_bBlocked = false;
		return false;
	}
	_bBlocked = false;
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	02-02-18:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Close()
{
#ifdef PLATFORM_PC
	QDT_ASSERT( TestStatus(F_STATUS::FS_OPEN) == true );
#endif 
	if ( ! IsServiceOk() )
	{
		_pRemoteFileService->CallNetworkPrintf("! Close() ! service not ok");
	}

	_pRemoteFileService->RemoteClose(_nHandleClient);
	while(!_bIdle)
	{
	}

	if (GetLastError() != FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
	{
		_pRemoteFileService->CallNetworkPrintf("! Error on File Close");
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		IsExist
//	Object:		
//	06-03-24:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::IsExist(const char * const	pFileName) const
{
	if ( ! _pRemoteFileService->GotALink()	) 
	{
		_pRemoteFileService->CallNetworkPrintf("! Not linked ");
		return false;
	}

	QDT_ASSERT(_bIdle);

	// ask the service.
	#ifdef NET_VERBOSE
	_pRemoteFileService->CallNetworkPrintf(".REMOTE_IS_EXIST : Blocking op");
	#endif 
		
	// [GRI]	: j'enleve le const ici, le service n'est pas const il va taper dans bIdle
	_pRemoteFileService->RemoteIsFileExist(pFileName,const_cast<REMOTE_FILE*>(this) );

	static int nToto = 0;
	while(!_bIdle)
	{
		nToto++;
	}

	// finished or timeOut
	if (GetLastError() != FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
	{
		_pRemoteFileService->CallNetworkPrintf("! File not exists or TimeOut : %s",pFileName);
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Rename
//	Object:		
//	03-12-05:	GRI - Created
//-----------------------------------------------------------------------------
//bool	REMOTE_FILE::Rename(const char * const	pFileName) const
//{
//	QDT_ASSERT(false);		// not implemented
//}

//-----------------------------------------------------------------------------
//	Name:		GetSectorSize
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
int		REMOTE_FILE::GetSectorSize() const
{
	// for compatibility; carefull 0 does not work at all.
	return 	1;
}

//-----------------------------------------------------------------------------
//	Name:		SetName
//	Object:		
//	03-09-10:	VPI - Created
//-----------------------------------------------------------------------------
void	REMOTE_FILE::SetName(const QDT_STRING &	sName)
{
	_sName = sName;
}

//-----------------------------------------------------------------------------
//	Name:		GetName
//	Object:		
//	03-09-10:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	REMOTE_FILE::GetName() const
{
	return	(_sName);
}

//-----------------------------------------------------------------------------
//	Name:		IsWriteProtected
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::IsWriteProtected(const char * const	pFileName) const
{
	QDT_NOT_IMPLEMENTED();
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Seek
//	Object:	Non blocking
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Seek(const F_SEEK				eInitialPosition,
												const unsigned int	uOffset)
{
	_pRemoteFileService->CallNetworkPrintf("Le seek ne sert a rien dans l'implémentation actuelle, enlever les appels ");
	return true;

	QDT_ASSERT(false);		// pas censé être utilisé.
	CHECK_SERVICE_OK;

	UInt32 uSize = GetSize();
	UInt32 uCurrentPosition = GetCurrentPosition();
	UInt32 uPosition = 0;
	
	switch (eInitialPosition)
	{
	case FS_BEGIN:	
		QDT_ASSERT( uOffset >= 0 );
		uPosition = uOffset;
		break;
	case FS_CURRENT:
		QDT_ASSERT( ( uCurrentPosition + uOffset ) < uSize );
		QDT_ASSERT( ( uCurrentPosition + uOffset ) >= 0 );
		uPosition = uCurrentPosition + uOffset;
		break;
	case FS_END:
		QDT_ASSERT( uOffset == 0 );
		uPosition = uSize + uOffset;
		break;
	}

		//@gritodo 2
	//_pRemoteFileService->SetPosition(uOffset,_nHandleClient);
	SetPosition(uOffset);

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Write(void *							pBuffer,
												 const UInt32				uPosition,
												 const int					nSize,
												 const F_SYNC				eSync)
{
	if ( ! _pRemoteFileService->GotALink()	) 
	{
		_pRemoteFileService->CallNetworkPrintf("No link => no transfer");
		return false;
	}
	QDT_ASSERT(_nHandleClient != -1);
	while(!_bIdle)
	{
		;
	}

	bool bResult = false;
	UInt32 uBytesWritten = 0;
	Int32 nLastError = 0;

	// ask the service to write.
	_pRemoteFileService->RemoteWrite(pBuffer,uPosition,nSize,_nHandleClient);

	// if not blocking, return true, user have to wait until idle.
	if (eSync != FS_WAIT )
	{
		return true;
	}

	//  blocking : wait the block being written.
#ifdef NET_VERBOSE
	_pRemoteFileService->CallNetworkPrintf(".REMOTE_FILE : Blocking write");
#endif 

	static int toto = 0;
	while(!_bIdle)
	{
		toto++;
	}

	// return from remote write
	if (GetLastError() != FILES_MANAGER_ABC::FM_FILE_NO_ERROR)
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Delete(const char * const	pFileName) const
{
	QDT_NOT_IMPLEMENTED();
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
void	REMOTE_FILE::Loop()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentReadPosition
//	Object:		Obsolete 
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
UInt32	REMOTE_FILE::GetCurrentReadPosition() const
{
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		Break
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
bool	REMOTE_FILE::Break()
{
	QDT_NOT_IMPLEMENTED();
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		IsServiceOk
//	Object:		
//	06-03-09:	GRI - Created
//-----------------------------------------------------------------------------
inline
bool	REMOTE_FILE::IsServiceOk()
{
	//if (_pRemoteFileService->IsRunning() )
	//{
	//	_pRemoteFileService->CallNetworkPrintf("Opening of a file but service not running");
	//	return false;
	//}

		//@gritodo 2
	//if ( ! _pRemoteFileService->IsLinked() ) 
	//{
	//	_pRemoteFileService->CallNetworkPrintf("Opening of a file but service has no link");
	//	return false;
	//}
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		SetIdle
//	Object:		atomic multithread function : The two operations must be done alltogether.
//	06-03-09:	GRI - Created
//-----------------------------------------------------------------------------
int s_nForLock = 0;
void	REMOTE_FILE::SetIdle(bool bIdle,Int32 nError)
{
	_nLastError = nError;
	s_nForLock = nError + 1;
	// @gri : le but est d'empêcher une optimisation du compilo qui ferait que idle est positionné
	// avant error. Si quelqu'un a un meilleur truc, je suis preneur.
	if ( (s_nForLock-1) == _nLastError )
	{
		_bIdle = bIdle; 
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

