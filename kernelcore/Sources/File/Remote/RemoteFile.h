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
//	The REMOTE_FILE class implements the management of a file wich exist on
//	a distant machine : PS3, XBOX.
//	06-03-06:	GRI - Created 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_REMOTE_FILE_H__
#define __KCORE_REMOTE_FILE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(File/FileABC)
#include INCL_KCORE(File/RemoteFileService/RemoteFileServiceABC)

class		 REMOTE_FILE_SERVICE;		// SetSize, SetIdle


namespace	QDT
{
	namespace KCORE
	{
		class	REMOTE_FILE : public FILE_ABC
		{
			friend class ::REMOTE_FILE_SERVICE_ABC;
			friend class ::REMOTE_FILE_SERVICE;		// SetSize, SetIdle, _nLastError
		public:

			REMOTE_FILE();
			~REMOTE_FILE();

			bool	Open(const char * const pFileName, const F_MODE eMode, const bool bSharing);
			bool	Close();
			bool	Read(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
			bool	IsIdle() const { return _bIdle; }
			void	SetIdle(bool bIdle,Int32 nError);		// atomic multithread
			bool	IsExist(const char * const	pFileName) const;
			
			
		private:
			int	 GetSectorSize() const;
			bool IsWriteProtected(const char * const pFileName) const;
			bool Seek(const F_SEEK eInitialPosition, const unsigned int nOffset);
			bool Write(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
			bool Delete(const char * const pFileName) const;
			void Loop();
			UInt32 GetCurrentReadPosition() const;
			bool Break();

		protected:
			void		SetName(const QDT_STRING & sName);
			const		QDT_STRING & GetName() const;
			bool		IsServiceOk();

		private:
			
			Int32				_nHandleClient; 			//differs from the (void*	_Handle) of the usual file.
			QDT_STRING	_sName;
			UInt32			_nReadSize;
			Char*				_pReadBuffer;
			UInt32			_nReadPos;
			UInt32			_nCurPos;
			Float32			_rLastReadTime;
			volatile bool		_bIdle;
			volatile bool		_bBlocked;
			REMOTE_FILE_SERVICE_ABC* _pRemoteFileService;
		};
	}
}

#endif //__KCORE_REMOTE_FILE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
