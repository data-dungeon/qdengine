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
//	CLASS:	FILE_TEXT_STREAM
//	The FILE_TEXT_STREAM class implements ...
//
//	01-08-02:	VPI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_STREAM_FILETEXTSTREAM_H__
#define __KCORE_STREAM_FILETEXTSTREAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Stream/filestream)

namespace QDT
{
	namespace KCORE
	{
		class	FILE_TEXT_STREAM : public	FILE_STREAM
		{
		public:
			FILE_TEXT_STREAM();
			FILE_TEXT_STREAM(const char *pFilename, int nAccessType);

			bool	Open(const char * pFilename, int nAccessType);
			bool	IsEof() const;
			bool	Restart();	//return true if Ok
			
			int operator ==(const char *);
			IO_STREAM&	operator<<(const int);
			IO_STREAM&	operator<<(const long);
			IO_STREAM&	operator<<(const short);
			IO_STREAM&	operator<<(const char);
			IO_STREAM&	operator<<(const bool);
			IO_STREAM&	operator<<(const unsigned int);
			IO_STREAM&	operator<<(const unsigned long);
			IO_STREAM&	operator<<(const unsigned short);
			IO_STREAM&	operator<<(const unsigned char);
			IO_STREAM&	operator<<(const float);
			IO_STREAM&	operator<<(const double);
			IO_STREAM&	operator<<(const char * const);
			IO_STREAM& operator <<(IO_STREAM&);
			void		PutString(const char *);

			void		GetString(char *, int);

		//	void		GetLine(char * const pBuffer, const int nSizeBuffer, const char cDelimiter);
			IO_STREAM& operator >>(IO_STREAM&);

		#ifndef _MASTER
			virtual void	DebugOutput(const char *pTxt);
			virtual void	DebugOutput(unsigned char Char);
			virtual void	DebugOutput(int Int);
			virtual void	DebugOutputBinary(int Int);
		#else
			virtual void	DebugOutput(const char *pTxt){}
			virtual void	DebugOutput(unsigned char Char){}
			virtual void	DebugOutput(int Int){}
			virtual void	DebugOutputBinary(int Int){}
		#endif

			virtual int		GetRealocSize() const;
			virtual void	SetRealocSize(const int nRealocSize);

		protected:
			IO_STREAM&	operator>>(int &);
			IO_STREAM&	operator>>(long &);
			IO_STREAM&	operator>>(short &);
			IO_STREAM&	operator>>(char &);
			IO_STREAM&	operator>>(unsigned int &);
			IO_STREAM&	operator>>(unsigned long &);
			IO_STREAM&	operator>>(unsigned short &);
			IO_STREAM&	operator>>(unsigned char &);
			IO_STREAM&	operator>>(bool &);
			IO_STREAM&	operator>>(float &);
			IO_STREAM&	operator>>(double &);
			IO_STREAM&	operator>>(char *);
		};
	}
}

#endif //__KCORE_STREAM_FILETEXTSTREAM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
