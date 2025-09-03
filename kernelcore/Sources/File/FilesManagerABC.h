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
//	CLASS:	FILES_MANAGER_ABC
//	The FILES_MANAGER_ABC class implements ...
//
//	03-05-21:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_FILE_FILESMANAGER_H__
#define __KCORE_FILE_FILESMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)

#include INCL_KCORE(File/FileABC)
#include INCL_KCORE(File/RemoteFileService/RemoteFileServiceABC)
namespace QDT
{
	namespace KCORE
	{
		class	FILES_MANAGER_ABC
		{
		public:
			
			enum FM_ERROR
			{
				FM_FILE_NO_ERROR	= 0,
				FM_FILE_NOT_FOUND	= -1,
				FM_READ_FAILED		= -2,
				FM_OPEN_FAILED		= -3,
				FM_CLOSE_FAILED		= -4,
			};

			// @griadd 
			enum FILE_OPTION
			{
				FO_FILE_SHARE			= 0x2,
				FO_FILE_REMOTE		= 0x4
			};

			static void					Create();
			static void					Release();
			static FILES_MANAGER_ABC *	Instance();
			static void					SetInstance(FILES_MANAGER_ABC * pInstance);
			static bool					Exist();
			void								SetRemoteFileService(REMOTE_FILE_SERVICE_ABC* pRemoteFileService) { _pRemoteFileService = pRemoteFileService; }
			REMOTE_FILE_SERVICE_ABC*	GetRemoteFileService() { return _pRemoteFileService; }
			
			void		Init();
			
			//FILE_ABC*	Open(const char * const pFileName, const FILE_ABC::F_MODE eMode, FM_ERROR &eError, const bool bShare = false);
			FILE_ABC*	Open(const char * const pFileName, const FILE_ABC::F_MODE eMode, FM_ERROR &eError, const QDT_FLAGS&	rFlags = 0);			
			
			FM_ERROR	Close(FILE_ABC* pFile);
			FM_ERROR	Delete(const char * const pFileName);
			FM_ERROR	Rename(const char * const pFileName);
			bool		FileCopy(const QDT_STRING & sSrcName, const QDT_STRING & sDestName);
			bool		IsFileExist(const char * const pFileName);
			bool		IsFileWriteProtected(const char * const pFileName);
			bool		IsFileReadOnly(const char * const pFileName);
			bool		IsDirectoryExist(const char * const pDirName);
			bool		CreateDir(const char * const pDirName);

			int			GetFileSize(const char * const pFileName);

			bool		DeepCopy(const QDT_STRING & sSrcDir, const QDT_STRING & sDestDir, const bool bInteractive = true);
			bool		DeleteDir(const QDT_STRING & sSrcDir, const QDT_STRING & sExtMask, const bool bWarn = true, const bool bDelSrcDir = false);

			void			GetModuloPositionAndSize(UInt32 nPosition, UInt32 nSize, UInt32 & nModuloPosition, UInt32 & nModuloSize);

			bool	GetEnableEmulateCD();
			void	SetEnableEmulateCD( bool bVal );

			bool	GetWinNT();

			void	SetRemoteMode(bool bRemoteMode);
			bool	GetRemoteMode() { return _bRemoteMode; }

		protected:
			FILES_MANAGER_ABC();
			~FILES_MANAGER_ABC();

		private:
			bool			_bRemoteMode;			// if true, all the calls are in remote mode. //zog
			bool			_bEnableEmulateCD;
			bool			_bWinNT;
			REMOTE_FILE_SERVICE_ABC		* _pRemoteFileService;
			static	FILES_MANAGER_ABC *	_pInstance;
		};
	}
}

#endif //__KCORE_FILE_FILESMANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
