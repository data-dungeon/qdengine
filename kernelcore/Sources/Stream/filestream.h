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
// CLASS: FILE_STREAM
//
//
//00-12-12 FKE Created: 
//*****************************************************************************


//=============================================================================
// Name: FILE_STREAM
//00-12-12 FKE Created: 
//=============================================================================

#ifndef __KCORE_STREAM_FILESTREAM_H__
#define __KCORE_STREAM_FILESTREAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(FILE_ABC)

#include INCL_KCORE(Tools/QdtFlags)
#include INCL_KCORE(Stream/iostream)

namespace QDT
{
	namespace KCORE
	{
		class FILE_STREAM:public IO_STREAM
		{
		public:
			enum	ACCESS_TYPE
			{
				AT_READ_ONLY	=	(1 << 0),
				AT_CREATE		=	(1 << 1),
				AT_MODIFY		=	(1 << 2),
				AT_TEXT			=	(1 << 8)
			};

			enum	CACHE_MODE
			{
				CM_FILLCACHE,
				CM_NO_FILLCACHE
			};

			enum 
			{
				DEFAULT_CACHE_SIZE = 16 * 1024
			};

			struct RelocationData
			{	
				int nPointerOffset;
				int nDataOffset;
			};

			explicit FILE_STREAM();
			explicit FILE_STREAM(const Int8 *pFilename, ACCESS_TYPE AccessType, const Bool bSharing = false);
			virtual ~FILE_STREAM();

			int operator ==(const char *);
			IO_STREAM& operator << (const Int8);
			IO_STREAM& operator << (const Int16);
			IO_STREAM& operator << (const Int32);
			IO_STREAM& operator << (const Int64);
			IO_STREAM& operator << (const Bool);
			IO_STREAM& operator << (const UInt8);
			IO_STREAM& operator << (const UInt16);
			IO_STREAM& operator << (const UInt32);
			IO_STREAM& operator << (const UInt64);
			IO_STREAM& operator << (const Float32);
			IO_STREAM& operator << (const Float64);
			IO_STREAM& operator << (const Int8 * const);
			IO_STREAM& operator << (IO_STREAM&);
			void	GetStreamFast(IO_STREAM& S, Int32 nSize);
			void	GetString(Int8 *, Int32);
			UInt32	GetBuffer(void *pBuffer, Int32 nSize);
			void	GetBufferFast(void *pBuffer, Int32 nSize);
			
			IO_STREAM& operator >> (Int8 &);
			IO_STREAM& operator >> (Int16 &);
			IO_STREAM& operator >> (Int32 &);
			IO_STREAM& operator >> (Int64 &);
			IO_STREAM& operator >> (UInt8 &);
			IO_STREAM& operator >> (UInt16 &);
			IO_STREAM& operator >> (UInt32 &);
			IO_STREAM& operator >> (UInt64 &);
			IO_STREAM& operator >> (Bool &);
			IO_STREAM& operator >> (Float32 &);
			IO_STREAM& operator >> (Float64 &);
			IO_STREAM& operator >> (Int8 *);
			IO_STREAM& operator >> (IO_STREAM&);

			void	GetLine(char * const pBuffer, const Int32 nSizeBuffer, const Int8 cDelimiter);

		#ifndef _MASTER
			virtual void	DebugOutput(const Int8 *pTxt);
			virtual void	DebugOutput(UInt8 Char);
			virtual void	DebugOutput(Int32 Int);
			virtual void	DebugOutputBinary(Int32 Int);
		#else
			virtual void	DebugOutput(const Int8 *pTxt){}
			virtual void	DebugOutput(UInt8 Char){}
			virtual void	DebugOutput(Int32 Int){}
			virtual void	DebugOutputBinary(Int32 Int){}
		#endif

		//les seeks
			IO_STREAM& operator +(const UInt32);
			IO_STREAM& operator -(const UInt32);
			IO_STREAM& operator =(const UInt32);

			void	PutString(const Int8 *);
			void	PutBuffer(const void *pBuffer, Int32 nSize);
			void	PutBufferFast(const void *pBuffer, Int32 nSize);
			void	ZeroBufferFast(Int32 nSize);
			
			void	SwitchWriteToRead();
			void	SwitchReadToWrite();
			UInt32	GetSize() const;
			UInt32	GetPos() const;
			Bool	IsEof() const;
			Bool	IsValid() const;
			Bool	Restart();	//return true if Ok
			void	AddPadding( Int32 nPadding );
			void	AddRelocationLink( UInt32 nPointerOffset, UInt32 nDataOffset );
			void	ApplyRelocation();
			Int32	GetFirstRelocatedPointerOffset( );
			Int32	GetNbRelocatedLongJumps();
			void	WriteLongJumpRelocationTable( IO_STREAM& Stream );

			virtual Int32		GetRealocSize() const;
			virtual void	SetRealocSize(const Int32 nRealocSize);

			
			virtual Bool	Open(const Int8 *pFilename, Int32 nAccessType, Int32 nCacheSize = DEFAULT_CACHE_SIZE, CACHE_MODE CacheMode = CM_FILLCACHE, const QDT_FLAGS&	rFlags = 0);

			void	Close();
			void	Flush();

			const Int8*	GetFileName() const;

		protected:
			void		SetChars(const char * const,Int32);
			void		GetChars(char *,Int32);

			void		SetValid(const Bool bFlag);
			Bool		GetValidState() const;

			void		CloseFile();
			void		FlushFile();
			void		FillBuffer();

			UInt32			_nSize;
			UInt32			_nOffset;
			UInt32			_nBufferSize;
			UInt32			_nBufferSizeMask;
			Char		*	_pcBuffer;
			Char		*	_pcAllocBuffer;
			UInt32			_nFlags;
			Char		*	_pcFilename;
			Bool			_bValid;
			FILE_ABC	*	_pFile;
		};
	}
}

#endif //__KCORE_STREAM_FILESTREAM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================


