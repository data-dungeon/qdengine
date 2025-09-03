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
//	CLASS:	QDT_STRING_UNICODE
//	The QDT_STRING_UNICODE class implements ...
//
//	03-05-19:	NCA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#ifndef __KCORE_TOOLS_QDT_STRING_UNICODE_H__
#define __KCORE_TOOLS_QDT_STRING_UNICODE_H__

#ifndef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM);
DECL_KCORE(MEM_POOL_HEADER);

#include INCL_KCORE(Tools/QdtString)

namespace QDT
{
	namespace KCORE
	{

		class	QDT_STRING_UNICODE
		{
		public:

			enum
			{
				MAX_STRING_UNICODE_LENGTH = 65536
			};


			explicit QDT_STRING_UNICODE(const int nReserve = 0);
			QDT_STRING_UNICODE(const QDT_STRING & sText);
			QDT_STRING_UNICODE(const QDT_STRING_UNICODE & sText);
			QDT_STRING_UNICODE(const unsigned short * const psText);

			void	Copy( const unsigned short * const pBuffer );
			void	Copy( const unsigned short * const pBuffer, const int nSize );
			void	Copy( const QDT_STRING_UNICODE & sString );
			void	Copy( const QDT_STRING & sQDTString );

			~QDT_STRING_UNICODE();

			void						WriteDependenciesBinary(IO_STREAM & Stream, int nThisOffset);
			void						InitBinary();

			static int					UnicodeStrLen( const unsigned short * pBuffer );

			void						CleanAnsi(QDT_STRING_UNICODE & sResult) const;

			void						ReleaseBuffer();

			void						Reserve(const int nSize = 0);
			void						Resize(const int nSize, const unsigned short nChar = L' ');

			int							Find(const QDT_STRING_UNICODE & sText, const int nStartPos = 0) const;
			int							Replace(const QDT_STRING_UNICODE & sOld, const QDT_STRING_UNICODE & sNew);

			QDT_STRING_UNICODE			ExtractMid(const int nFirst, const int nCount) const;
			QDT_STRING_UNICODE			ExtractMid(const int nFirst) const;

			/**	Return the length of the string */
			int							GetSize() const;
			void						UpdateSize();
			/**	Return the size reserved for the string */
			int							GetMaxSize() const;
			const unsigned short *		GetBuffer() const;

			QDT_STRING					ToString() const; // Warning this method looses information if the unicode string is not ansi

			bool						IsEmpty() const;
			void						Empty(const bool bPartial = false);

			const unsigned short &		operator[](const int nPosition) const;
			unsigned short &			operator[](const int nPosition);

			//needed for elipse C function such as sprintf, otherwise it causes scratch when GetBuffer() is not explicitly called
			//operator unsigned short * ();
			operator const unsigned short * () const;


			QDT_STRING_UNICODE &		operator=(const QDT_STRING_UNICODE & sText);
			QDT_STRING_UNICODE &		operator=(const unsigned short * const psText);
			QDT_STRING_UNICODE &		operator=(const QDT_STRING & sText);

			QDT_STRING_UNICODE			operator+(const QDT_STRING_UNICODE & sText) const;
			
			//to avoid autocast from str to  * and sum (str + '!') -> str.GetBuffer() + '!' : troublesome address of buffer + ascii code for '!'
			//QDT_STRING_UNICODE			opercharator+(const char c) const;	

			const QDT_STRING_UNICODE &	operator+=(const QDT_STRING_UNICODE & sText);
			const QDT_STRING_UNICODE &	operator+=(const unsigned short * const psText);
			const QDT_STRING_UNICODE &	operator+=(const unsigned short nChar);
			
			bool				Compare(const QDT_STRING_UNICODE & sText) const;
			bool				Compare(const unsigned short * const psText) const;

			bool				operator==(const QDT_STRING_UNICODE & sText) const;
			bool				operator==(const unsigned short * const psText) const;
			bool				operator!=(const QDT_STRING_UNICODE & sText) const;
			bool				operator!=(const unsigned short * const psText) const;

				
			void				Read(IO_STREAM & Stream);
			void				Write(IO_STREAM & Stream) const;
			

			friend IO_STREAM &	operator<<(IO_STREAM & Stream, const QDT_STRING_UNICODE & sText)
			{
				sText.Write(Stream);
				return	(Stream);
			}

			friend IO_STREAM &	operator>>(IO_STREAM & Stream, QDT_STRING_UNICODE & sText)
			{
				sText.Read(Stream);
				return	(Stream);
			}

		private:
			unsigned short		GetAt(const int nPosition) const;
			void				SetAt(const int nPosition, const unsigned short nChar);
			unsigned short *	GetBufferWithOffset(const int nPosition) const;
			unsigned short *	GetBufferPtr() const;	//not const counter part to GetBuffer() for internal use only

			void				SetSize(const int nSize);
			void				SetMaxSize(const int nMaxSize);

			void				NeedSpace(const int nSize);
			
			bool				Verify() const;
			
			enum
			{
				RATIO_INCREASE_BUFFER	=	5,
				RATIO_BASE				=	4,
			};

			unsigned short *			_pBuffer;
			int							_nSize;
			int							_nAllocSize;

			static unsigned short		EmptyBuffer;
			static MEM_POOL_HEADER *	_pMPH8;
			static MEM_POOL_HEADER *	_pMPH16;
			static MEM_POOL_HEADER *	_pMPH32;
			static MEM_POOL_HEADER *	_pMPH64;
			static MEM_POOL_HEADER *	_pMPH128;
			static MEM_POOL_HEADER *	_pMPH256;
			static MEM_POOL_HEADER *	_pMPH512;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
