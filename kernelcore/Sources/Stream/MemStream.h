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
// CLASS: MEM_STREAM
//
//
//00-12-12 FKE Created: 
//*****************************************************************************

//=============================================================================
// Name: MEM_STREAM
//00-12-12 FKE Created: 
//=============================================================================

#ifndef __KCORE_STREAM_MEMSTREAM_H__
#define __KCORE_STREAM_MEMSTREAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//DECL_KCORE(MEM_POOL_HEADER)

#include INCL_KCORE(File/FileABC)
#include INCL_KCORE(Stream/iostream)
#include INCL_KCORE(Template/QDT_Vector)

namespace QDT
{
	namespace KCORE
	{
		class MEM_STREAM : public IO_STREAM
		{
		public:
			enum MEMORY_TYPE
			{
				MAIN_MEMORY, 
				VIDEO_MEMORY,
				SOUND_MEMORY,
				MAX_MEMORY_TYPE
			};

			enum LOD_TYPE
			{
				LOW_DETAIL, 
				MIDDLE_DETAIL,
				HIGH_DETAIL,
				ENVMAP,
				NONE,
				MAX_LOD_TYPE
			};

			struct RelocationData
			{	
				int nPointerOffset;
				int nDataOffset;
			};

			explicit MEM_STREAM();
			explicit MEM_STREAM(char *pBuf, int Size);
			explicit MEM_STREAM(int nBaseSize);
			virtual ~MEM_STREAM();

			void	Reserve(UInt32 nSize);

			void	Clear();
			bool	Restart();
			bool	IsEof() const;
			bool	IsValid() const;

			virtual bool	IsMemStream() const;

			// Added by CVI
			void	InstanciateStream( const MEM_STREAM& Stream );
			//

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
			void	GetStreamFast(IO_STREAM& S, int nSize);
			void	PutString(const char *);
			void	PutBuffer(const void *pBuffer, int nSize);
			void	PutBufferFast(const void *pBuffer, int nSize);
			void	ZeroBufferFast(int nSize);

			IO_STREAM& operator >> (UInt8 &);
			IO_STREAM& operator >> (UInt16 &);
			IO_STREAM& operator >> (UInt32 &);
			IO_STREAM& operator >> (UInt64 &);
			IO_STREAM& operator >> (Int8 &);
			IO_STREAM& operator >> (Int16 &);
			IO_STREAM& operator >> (Int32 &);
			IO_STREAM& operator >> (Int64 &);
			IO_STREAM& operator >> (Bool &);
			IO_STREAM& operator >> (Float32 &);
			IO_STREAM& operator >> (Float64 &);
			IO_STREAM& operator >> (Int8 *);
			IO_STREAM& operator >> (IO_STREAM&);

			int operator ==(const char *);
			
			void	GetString(char *, int);
			UInt32	GetBuffer(void *pBuffer, int nSize);
			void	GetBufferFast(void *pBuffer, int nSize);
			void	GetLine(char * const pBuffer, const int nSizeBuffer, const char cDelimiter);

			void	GetFromFile(FILE_ABC* pFile, UInt32 nSize, FILE_ABC::F_SYNC Sync, UInt32 nPos = 0);
			void	PutToFile(FILE_ABC* pFile, FILE_ABC::F_SYNC Sync, UInt32 nPos = 0);


			void DebugOutput(const char *pTxt) {}
			void DebugOutput(UChar Char) {}
			void DebugOutput(int Int) {}
			void DebugOutputBinary(int Int) {}

		//les seeks
			IO_STREAM& operator +(const UInt32);
			IO_STREAM& operator -(const UInt32);
			IO_STREAM& operator =(const UInt32);


			void	SwitchWriteToRead();
			void	SwitchReadToWrite();
			UInt32	GetSize() const;
			UInt32	GetBufferSize() const;

			virtual int		GetRealocSize() const;
			virtual void	SetRealocSize(const int nRealocSize);

			UInt32	GetPos() const;
			void *	GetBuffer();
			const void *	GetBuffer() const;
			void *	GetCurrentReadPtr();
			void	SetStatus( int nStatus );
			int 	GetStatus() const;
			void	SetReadStatus( bool b = true );
			void	SetWriteStatus( bool b = true );
			bool	IsWrite();
			bool 	IsExternalBuffer() const;
			void	AddPadding( int nPadding );
		#ifndef _MASTER
			void	AddRelocationLink( UInt32 nPointerOffset, UInt32 nDataOffset );
			void	AddLongRelocationLink( UInt32 nPointerOffset, UInt32 nDataOffset );
			void	ApplyRelocation();
			int		GetFirstRelocatedPointerOffset( );
			int		GetNbRelocatedLongJumps();
			void	WriteLongJumpRelocationTable( IO_STREAM& Stream );

			static MEM_STREAM*	GetRawStream(MEMORY_TYPE eMemoryType, LOD_TYPE eLODType);
			static void			CleanRawStreams();
			static void			AddRawPointer(int nPointerOffset, void* pRawData, int nSizeRawData, MEMORY_TYPE eMemoryType, LOD_TYPE eLODType, int nPadding = 16);
		#endif

			UInt32	CompressTo(MEM_STREAM & Dest) const;
			UInt32	UnCompressTo(MEM_STREAM & Dest, UInt32 DestUncompressSize) const;

		protected:
			virtual void SetChars(const char *,int);
			virtual void GetChars(char *,int);

			void	Align(int);
			void	GetSpace();

			// Optimize memory
			static char * AllocateBuffer(int nSize);
			static void FreeBuffer(char * pBuffer, int nSize);

			// NHO : Commente pour verifier son utilite....
			//static MEM_POOL_HEADER * _pMemPool;

			UInt32		_nSize;
			UInt32		_nBufferSize;
			UInt32		_nRealocSize;
			UInt32		_nOffset;
			Int8 *		_pcBuffer;
			Int32		_nFlags;

		#ifndef _MASTER
			QDT_VECTOR<RelocationData>	RelocationDataList;
			QDT_VECTOR<RelocationData>	LongJumpRelocationDataList;
			static MEM_STREAM*			_RawStreams[MAX_MEMORY_TYPE][MAX_LOD_TYPE];
		#endif
		};
	}
}

#endif //__KCORE_STREAM_MEMSTREAM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================

