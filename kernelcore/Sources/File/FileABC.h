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
//	CLASS:	FILE_ABC
//	The FILE_ABC class implements ...
//
//	02-02-14:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_FILE_FILEABC_H__
#define __KCORE_FILE_FILEABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtFlags)

namespace QDT
{
	namespace KCORE
	{
		class	FILE_ABC
		{
		public:
			FILE_ABC();
			virtual ~FILE_ABC();

			enum F_MODE
			{
				FM_READ					= 0x1,
				FM_WRITE				= 0x2,
				FM_READ_WRITE			= FM_READ + FM_WRITE,
				FM_CREATE				= 0x4,
				FM_CREATE_READ_WRITE	= FM_CREATE + FM_READ + FM_WRITE,
				FM_SAVEGAME				= 0x8,
				FM_REMOTE					= 0x10,
			};

			enum F_SEEK
			{
				FS_BEGIN,
				FS_CURRENT,
				FS_END,
			};

			enum F_STATUS
			{
				FS_OPEN		=	(1 << 0),
				FS_IS_IDLE	=	(1 << 1),
			};

			enum F_SYNC
			{
				FS_WAIT		=	(1 << 0),
				FS_NO_WAIT	=	(1 << 1),
			};

			enum
			{
				BUFFER_SIZE					= 1, //256 * 1024,
			};

			virtual int	 GetSectorSize() const = 0;
			virtual bool IsIdle() const = 0;
			virtual bool Open(const char * const pFileName, const F_MODE eMode, const bool bSharing = false) = 0;
			virtual bool Close() = 0;
			virtual bool IsExist(const char * const pFileName) const = 0;
			virtual bool IsWriteProtected(const char * const pFileName) const = 0;
			virtual bool Seek(const F_SEEK eInitialPosition, const unsigned int nOffset) = 0;
			virtual bool Read(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT) = 0;
		#ifdef PLATFORM_PC
			virtual bool ReadAndUncompress(void *  pBuffer, const unsigned int nPosition, const int nCompressedSize, const int nUncompressedSize, const F_SYNC eSync = FS_NO_WAIT);
		#endif
			virtual bool Write(void *  pBuffer, const unsigned int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT ) = 0;
			virtual bool Delete(const char * const pFileName) const = 0;
			virtual void Loop() = 0;
			virtual unsigned int GetCurrentReadPosition() const = 0;
			virtual bool Break() = 0;
			
			void		GetModuloPositionAndSize(UInt32 nPosition, UInt32 nSize, UInt32 & nModuloPosition, UInt32 & nModuloSize);
			int			GetSize() const;
			unsigned int		GetCurrentPosition() const;

			int			GetLastError() const;

		protected:
			
			void	SetSize(const int nSize);
			void	SetPosition(const unsigned int nPosition);
			
			bool	TestStatus(int nMask) const;
			void	EnableStatus(int nStatus);
			void	DisableStatus(int nStatus);

		private:

			int				_nSize;
			unsigned int	_nPosition;
			QDT_FLAGS		_fStatus;

			static	 char	s_ReadBuffer[ 2 * BUFFER_SIZE ];

		protected:
			mutable int		_nLastError;
		};
	}
}

#endif //__KCORE_FILE_FILEABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

