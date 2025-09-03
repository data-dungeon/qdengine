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
//	CLASS:	REMOTE_FILE_SERVICE_ABC
//	Cette classe est abstraite pour ne pas implémenter des fonctions qui font 
//	appel à Kernel network
//
//	06-03-20:	GRI - Created
//*****************************************************************************

#ifndef __REMOTE_FILE_SERVICE_ABC
#define __REMOTE_FILE_SERVICE_ABC

#pragma once

#define MAX_OPEN_FILE	40

class NET_MESSAGE;
class SBUFFER;
class SERVER_OPEN_FILE;
DECL_KCORE(QDT_STRING);
DECL_KCORE(REMOTE_FILE);

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
namespace QDT
{
	namespace KCORE
	{
		class	REMOTE_FILE_SERVICE_ABC
		{
		public:
			//virtual ~REMOTE_FILE_SERVICE_ABC(); // virtual destructor but implemented to make class abstract

			virtual	bool		GotALink() = 0;

			// OPEN
			virtual Int32		RemoteOpen(const char * const	pFileName,Int32	nDesiredAccess,REMOTE_FILE*	pRemoteFile) = 0;

			//	READ
			virtual void		RemoteRead(void* pBuffer,const UInt32 uPosition,const Int32 nSize,Int32 nHandleDistantFile) = 0;

	//	WRITE
	virtual void		RemoteWrite(void* pBuffer,const UInt32 uPosition,const Int32 nSize,Int32 nHandleDistantFile) = 0;

			// SEEK
			virtual void		SetPosition(UInt32 nOffset,Int32	nHandleClient) = 0 ;

			// CLOSE
			virtual	void		RemoteClose(Int32 nHandle) = 0;

			// IsExist
			virtual Int32		RemoteIsFileExist(const char * const	pFileName,REMOTE_FILE* pRemoteFile) = 0;

			virtual void		CallNetworkPrintf(char* szBuffer,...) = 0;
		};
	}
}

#endif 

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
