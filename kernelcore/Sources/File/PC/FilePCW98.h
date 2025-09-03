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
//	CLASS:	FILE_PC_W98
//	The FILE_PC_W98 class implements ...
//
//	02-02-14:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_FILE_PC_FILEPCW98_H__
#define __KCORE_FILE_PC_FILEPCW98_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(File/FileABC)

namespace	QDT
{
	namespace KCORE
	{
		class	FILE_PC_W98 : public FILE_ABC
		{
		public:
			
			enum
			{
				SECTOR_SIZE = 512
			};
				
			FILE_PC_W98();
			~FILE_PC_W98();

			int	 GetSectorSize() const;
			bool Open(const char * const pFileName, const F_MODE eMode, const bool bSharing);
			bool Close();
			bool IsIdle() const;
			bool IsExist(const char * const pFileName) const;
			bool IsWriteProtected(const char * const pFileName) const;
			bool Seek(const F_SEEK eInitialPosition, const unsigned int nOffset);
			bool ReadInternal(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
			bool Read(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
			bool Write(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
			bool Delete(const char * const pFileName) const;
			bool Rename(const char * const pFileName) const;
			bool UpdateRead();
			void Loop();
			unsigned int GetCurrentReadPosition() const;
			bool Break();

		protected:
			void				SetName(const QDT_STRING & sName);
			const QDT_STRING & GetName() const;

		private:
			
			void*				_pFile;
			QDT_STRING			_sName;
			UInt32				_nReadSize;
			char*				_pReadBuffer;
			UInt32				_nReadPos;
			UInt32				_nCurPos;
			Float32				_rLastReadTime;
		};
	}
}

#endif //__KCORE_FILE_PC_FILEPCW98_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
