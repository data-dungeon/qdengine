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
//	CLASS:	FILE_PS3
//	The FILE_PS3 class implements ...
//
//	05-11-15:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_FILE_PS3_FILEPS3_H__
#define __KCORE_FILE_PS3_FILEPS3_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(File/FileABC)
#include INCL_KCORE(Thread/Mutex)

namespace	QDT
{
	namespace KCORE
	{
		class	FILE_PS3_HOST : public FILE_ABC
		{
		public:

			enum
			{
				SECTOR_SIZE = 512
			};
			
			FILE_PS3_HOST();
			~FILE_PS3_HOST();

			virtual int	 GetSectorSize() const;
			virtual bool IsIdle() const;
			virtual bool Open(const char * const pFileName, const F_MODE eMode, const bool bSharing = false);
			virtual bool Close();
			virtual bool IsExist(const char * const pFileName) const;
			virtual bool IsWriteProtected(const char * const pFileName) const;
			virtual bool Seek(const F_SEEK eInitialPosition, const unsigned int nOffset);
			virtual bool Read(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
			virtual bool Write(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT );
			virtual bool Delete(const char * const pFileName) const;
			virtual void Loop();
			virtual unsigned int GetCurrentReadPosition() const;
			virtual bool Break();

			static void SetHomePath(const char* newHome);

		private:
			static char _homePath[256];
			Int32		_nHandle;
			MUTEX		_Mutex;
		};
	}
}

#endif //__KCORE_FILE_PS3_FILEPS3_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
