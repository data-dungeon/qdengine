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
// CLASS: DEBUG_STREAM
//
//
//00-12-12 FKE Created: 
//*****************************************************************************


//=============================================================================
// Name: DEBUG_STREAM
//00-12-12 FKE Created: 
//=============================================================================

#ifndef __KCORE_STREAM_DEBUGSTREAM_H__
#define __KCORE_STREAM_DEBUGSTREAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Stream/iostream)

namespace QDT
{
	namespace KCORE
	{
		class DEBUG_STREAM : public IO_STREAM
		{
		public:
			explicit DEBUG_STREAM(bool AccessType);
			virtual ~DEBUG_STREAM();

			bool	IsEof() const;
			bool	IsValid() const;

			bool	Restart();	//return true if Ok
			
			int operator ==(const char *);
			IO_STREAM& operator << (const int);
			IO_STREAM& operator << (const long);
			IO_STREAM& operator << (const short);
			IO_STREAM& operator << (const char);
			IO_STREAM& operator << (const bool);
			IO_STREAM& operator << (const unsigned int);
			IO_STREAM& operator << (const unsigned long);
			IO_STREAM& operator << (const unsigned short);
			IO_STREAM& operator << (const unsigned char);
			IO_STREAM& operator << (const float);
			IO_STREAM& operator << (const double);
			IO_STREAM& operator << (const char * const);
			IO_STREAM& operator << (IO_STREAM	&);
			void	GetStreamFast(IO_STREAM& S, int nSize);

			IO_STREAM& operator >> (int &);
			IO_STREAM& operator >> (long &);
			IO_STREAM& operator >> (short &);
			IO_STREAM& operator >> (char &);
			IO_STREAM& operator >> (unsigned int &);
			IO_STREAM& operator >> (unsigned long &);
			IO_STREAM& operator >> (unsigned short &);
			IO_STREAM& operator >> (unsigned char &);
			IO_STREAM& operator >> (bool &);
			IO_STREAM& operator >> (float &);
			IO_STREAM& operator >> (double &);
			IO_STREAM& operator >> (char *);
			IO_STREAM& operator >> (IO_STREAM &);
			void	GetLine(char * const pBuffer, const int nSizeBuffer, const char cDelimiter);
		//les seeks
			IO_STREAM& operator +(const unsigned int);
			IO_STREAM& operator -(const unsigned int);
			IO_STREAM& operator =(const unsigned int);

		#ifndef _MASTER
			void	DebugOutput(const char *pTxt);
			void	DebugOutput(unsigned char Char);
			void	DebugOutput(int Int);
			void	DebugOutputBinary(int Int);
		#else
			void	DebugOutput(const char *pTxt){}
			void	DebugOutput(unsigned char Char){}
			void	DebugOutput(int Int){}
			void	DebugOutputBinary(int Int){}
		#endif

			virtual int		GetRealocSize() const;
			virtual void	SetRealocSize(const int nRealocSize);

			void	GetString(char *, int);
			void	PutString(char *);

			void	SwitchWriteToRead();
			void	SwitchReadToWrite();
			UInt32		GetSize() const;
			UInt32		GetPos() const;
		private:

			UInt32	_nSize;
			UInt32	_nFlags;
		};
	}
}

#endif //__KCORE_STREAM_DEBUGSTREAM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
