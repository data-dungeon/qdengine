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
// CLASS: IO_STREAM
//
//
//00-12-12 FKE Created: 
//*****************************************************************************


//=============================================================================
// Name: IO_STREAM
//00-12-12 FKE Created: 
//=============================================================================
//opérateur << ->écriture dans le stream
//opérateur >> ->lecture dans le stream
//SwitchWriteToRead : flush, Ferme le stream puis le réouvre en écriture
//
//
//
//A propos du format des données:
//	pour les long et short :
//		en little indian (0xffeeddcc s'écrit en mémoire - cc dd ee ff -)
//
//	pour les flottant: 
//		32 bits répartis ainsi:
//		S EEEEEEEE MMMMMMMMMMMMMMMMMMMMMMM
//		ou 1 bit de signe, 8 bits d'exposant, 23 bits de mantisse (norme IEEE)
// on a :	0 -> 0
//			0x7f800000 = +infini
//			0xff800000 = -infini
//			(> 0x7f80000) = +Not A Number
//			(< 0xff80000) = -Not A Number
//	pour les doubles: 
//		64 bits répartis ainsi:
//		S EEEEEEEEEEE MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//		ou 1 bit de signe, 11 bits d'exposant, 52 bits de mantisse (norme IEEE)
// on a :	0 -> 0
//			0x7ff0000000000000 = +infini
//			0xfff0000000000000 = -infini
//			(> 0x7ff000000000000) = +Not A Number
//			(< 0xfff000000000000) = -Not A Number
//
//pour les chaines de caractère :
//		fin d'une ligne 2 char (ligne suivante + retour chariot) : 0xd 0xa 
//		fin d'une chaine : 0 (norme C)

#ifndef __KCORE_STREAM_IOSTREAM_H__
#define __KCORE_STREAM_IOSTREAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class IO_STREAM
		{
		public:

			virtual Bool IsEof() const = 0;
			virtual Bool IsValid() const = 0;

			virtual ~IO_STREAM() {}

		//Istream	
			
			virtual IO_STREAM& operator << (const Int8) = 0;
			virtual IO_STREAM& operator << (const Int16) = 0;
			virtual IO_STREAM& operator << (const Int32) = 0;
			virtual IO_STREAM& operator << (const Int64) = 0;
			virtual IO_STREAM& operator << (const Bool) = 0;
			virtual IO_STREAM& operator << (const UInt8) = 0;
			virtual IO_STREAM& operator << (const UInt16) = 0;
			virtual IO_STREAM& operator << (const UInt32) = 0;
			virtual IO_STREAM& operator << (const UInt64) = 0;
			virtual IO_STREAM& operator << (const Float32) = 0;
			virtual IO_STREAM& operator << (const Float64) = 0;
			virtual IO_STREAM& operator << (const Int8 * const) = 0;
			virtual IO_STREAM& operator << (IO_STREAM&) = 0;
			virtual void GetString(Int8 *pTxt, Int32 Size) = 0;
			virtual	UInt32	GetBuffer(void *pBuffer, Int32 nSize) = 0;	// returns how many bytes have been read

		//Ostream
			virtual Int32 operator ==(const char *) = 0;
			virtual IO_STREAM& operator >> (Int8 &) = 0;
			virtual IO_STREAM& operator >> (Int16 &) = 0;
			virtual IO_STREAM& operator >> (Int32 &) = 0;
			virtual IO_STREAM& operator >> (Int64 &) = 0;
			virtual IO_STREAM& operator >> (UInt8 &) = 0;
			virtual IO_STREAM& operator >> (UInt16 &) = 0;
			virtual IO_STREAM& operator >> (UInt32 &) = 0;
			virtual IO_STREAM& operator >> (UInt64 &) = 0;
			virtual IO_STREAM& operator >> (Bool &) = 0;
			virtual IO_STREAM& operator >> (Float32 &) = 0;
			virtual IO_STREAM& operator >> (Float64 &) = 0;
			virtual IO_STREAM& operator >> (Int8 *) = 0;
			virtual IO_STREAM& operator >> (IO_STREAM&) = 0;
			virtual void GetStreamFast(IO_STREAM& S, Int32 nSize) = 0;
			virtual void GetBufferFast(void *pBuffer, Int32 nSize) = 0;
			virtual void PutString(const char *pTxt) = 0;
			virtual	void PutBuffer(const void *pBuffer, Int32 nSize) = 0;
			virtual void PutBufferFast(const void *pBuffer, Int32 nSize) = 0;
			virtual void ZeroBufferFast(Int32 nSize) = 0;

			virtual void DebugOutput(const Int8 *pTxt) = 0;
			virtual void DebugOutput(UInt8 Char) = 0;
			virtual void DebugOutput(Int32 Int) = 0;
			virtual void DebugOutput(Float32 Float) {};
			virtual void DebugOutputBinary(Int32 Int) = 0;

		//les seeks
			virtual IO_STREAM& operator +(const UInt32) = 0;
			virtual IO_STREAM& operator -(const UInt32) = 0;
			virtual IO_STREAM& operator =(const UInt32) = 0;
			
			virtual int		GetRealocSize() const = 0;
			virtual void	SetRealocSize(const Int32 nRealocSize) = 0;

			virtual void SwitchWriteToRead() = 0;
			virtual void SwitchReadToWrite() = 0;
			virtual Bool Restart() = 0;	//return true if Ok
			virtual UInt32 GetSize() const = 0;
			virtual UInt32 GetPos() const = 0;

			virtual void	GetLine(char * const pBuffer, const Int32 nSizeBuffer, const char cDelimiter) = 0;

			virtual Bool	IsFileTextStreamDebug()	{ return (false); }
			virtual Bool	IsMemStream() const { return	(false); }

		#ifndef _MASTER
			virtual void	AddPadding( Int32 nPadding ) = 0;
			virtual void	AddRelocationLink( UInt32 nPointerOffset, UInt32 nDataOffset ) = 0;
			virtual Int32	GetFirstRelocatedPointerOffset( ) = 0;
			virtual void	ApplyRelocation() = 0;
			virtual Int32	GetNbRelocatedLongJumps() = 0;
			virtual void	WriteLongJumpRelocationTable( IO_STREAM& Stream ) = 0;
		#endif

			enum
			{
				READ = 1 << 0,				// = 1
				WRITE = 1 << 1,				// = 2
				EXTERNAL_BUFFER = 1 << 2,	// = 4
				READWRITE = READ + WRITE,	// = 3
				BASE_SIZE = 4096,
			};

		protected:
		};
	}
}

#endif //__KCORE_STREAM_IOSTREAM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
