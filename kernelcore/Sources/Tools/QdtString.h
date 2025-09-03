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
//	CLASS:	QDT_STRING
//	The QDT_STRING class implements ...
//
//	01-12-11:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
// uncomment to know if you call QDT_STRING constructor twice on the same memory pointer creating memory leak.
// like a QDT_NEW_PLACED wrongly placed ? 
//#define QDT_STRING_TRACK_NEW_DELETE

#ifndef __KCORE_TOOLS_QDT_STRING_H__
#define __KCORE_TOOLS_QDT_STRING_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class QDT_CONST_STRING;
	}
}

DECL_KCORE(IO_STREAM);
DECL_KCORE(MEM_POOL_HEADER);

#include	INCL_KCORE(Mem/MemConfig)

#ifdef QDT_MEM_ENABLED
#define USE_NEW_MEMPOOL
#define ENABLE_QDT_CONST_STRING
#endif

#ifdef USE_NEW_MEMPOOL
class MEM_POOL_MANAGER;
#endif

#include INCL_KCORE(Template/QDT_DList)

namespace QDT
{
	namespace KCORE
	{
		class	QDT_STRING
		{
		public:
			static const QDT_STRING	CONST_EMPTY_STRING;

			explicit QDT_STRING(const int nReserve = 0);

			QDT_STRING(const QDT_STRING & sText);
#ifdef ENABLE_QDT_CONST_STRING
			QDT_STRING(const QDT_CONST_STRING & sText);
#endif
			QDT_STRING(const char * const psText);
			QDT_STRING(const char * const psText, const int nLength);
			explicit QDT_STRING(const char cChar, const int nRepeat = 1);

			void	Copy(const char * const pBuffer, const int nSize);

			void	Construct();
			void	Destruct();

			~QDT_STRING();

			void				Reserve(const int nSize = 0);
			void				Resize(const int nSize, const int nChar = ' ');

			/**	Return the length of the string */
			int					GetSize() const;
			void				UpdateSize();
			/**	Return the size reserved for the string */
			int					GetMaxSize() const;
			const char *		GetBuffer() const;
			
			char *				GetFullBufferPtr(); //Use with Care it's for speed optimization

			bool				IsEmpty() const;
			void				Empty(const bool bPartial = false);

			const char &		operator[](const int nPosition) const;
			char &				operator[](const int nPosition);

			operator const char * () const;

			//needed for elipse C function such as sprintf, otherwise it causes scratch when GetBuffer() is not explicitly called
			//operator char * ();

			QDT_STRING &		operator=(const QDT_STRING & sText);
#ifdef ENABLE_QDT_CONST_STRING
			QDT_STRING &		operator=(const QDT_CONST_STRING & sText);
#endif
			QDT_STRING &		operator=(const char * const psText);

			QDT_STRING			operator+(const QDT_STRING & sText) const;
#ifdef ENABLE_QDT_CONST_STRING
			QDT_STRING			operator+(const QDT_CONST_STRING & sText) const;
#endif
			
			//to avoid autocast from str to char * and sum (str + '!') -> str.GetBuffer() + '!' : troublesome address of buffer + ascii code for '!'
			QDT_STRING			operator+(const char c) const;	

			const QDT_STRING &	operator+=(const QDT_STRING & sText);
#ifdef ENABLE_QDT_CONST_STRING
			const QDT_STRING &	operator+=(const QDT_CONST_STRING & sText);
#endif
			const QDT_STRING &	operator+=(const char * const psText);
			const QDT_STRING &	operator+=(const bool bValue);
			const QDT_STRING &	operator+=(const char cChar);
			const QDT_STRING &	operator+=(const int nValue);
			const QDT_STRING &	operator+=(const float rValue);
			const QDT_STRING &	operator+=(const double	rValue);
			const QDT_STRING &	operator+=(const UInt64	& nValue);
			
			float				Atof() const;
			int					Atoi() const;
			long				Atol() const;
			bool				Atob() const;

			void				SetFromTime(const int nTimeMs);
			void				SetFromFileSize(const int nFileSizeBytes);

			bool				CompareNoCase(const QDT_STRING & sText) const;
			bool				CompareNoCase(const char * const psText) const;

			bool				operator==(const QDT_STRING & sText) const;
#ifdef ENABLE_QDT_CONST_STRING
			bool				operator==(const QDT_CONST_STRING & sText) const;
#endif
			bool				operator==(const char * const psText) const;
			bool				operator!=(const QDT_STRING & sText) const;
			bool				operator!=(const char * const psText) const;

			bool				operator<(const QDT_STRING & sText) const;
			bool				operator<(const char * const psText) const;
			bool				operator<=(const QDT_STRING & sText) const;
			bool				operator<=(const char * const psText) const;
			bool				operator>(const QDT_STRING & sText) const;
			bool				operator>(const char * const psText) const;
			bool				operator>=(const QDT_STRING & sText) const;
			bool				operator>=(const char * const psText) const;

			void				MakeUpper();
			QDT_STRING			ToUpper() const;
			void				MakeLower();
			QDT_STRING			ToLower() const;

			int					Replace(const QDT_STRING & sOld, const QDT_STRING & sNew);

			void				Format(const char * const psFormatText, ...);
			void				AppendFormat(const char * const psFormatText, ...);

			int					Find(const QDT_STRING & sText, const int nStartPos = 0) const;
			int					Find(const int nChar, const int nStartPos = 0) const;
			int					ReverseFind(const QDT_STRING & sText, const int nStartPos = 0) const;
			int					ReverseFind(const int nChar, const int nStartPos = 0) const;

			int					FindCount(const QDT_STRING & sText, const int nStartPos = 0) const;
			int					FindCount(const int nChar, const int nStartPos = 0) const;

			QDT_STRING			EraseMid(const int nFirst, const int nCount) const;
			QDT_STRING			EraseMid(const int nFirst) const;
			QDT_STRING			EraseLeft(const int nCount) const;
			QDT_STRING			EraseRight(const int nCount) const;

			QDT_STRING			ExtractMid(const int nFirst, const int nCount) const;
			QDT_STRING			ExtractMid(const int nFirst) const;
			QDT_STRING			ExtractLeft(const int nCount) const;
			QDT_STRING			ExtractRight(const int nCount) const;

			int					TrimLeft(const char cTrim = '\0');
			int					TrimRight(const char cTrim = '\0');

			void				Reverse();

			void				Read(QDT::KCORE::IO_STREAM & Stream);
			void				Write(QDT::KCORE::IO_STREAM & Stream) const;

			friend QDT::KCORE::IO_STREAM &	operator<<(QDT::KCORE::IO_STREAM & Stream, const QDT_STRING & sText)
			{
				sText.Write(Stream);
				return	(Stream);
			}

			friend QDT::KCORE::IO_STREAM &	operator>>(QDT::KCORE::IO_STREAM & Stream, QDT_STRING & sText)
			{
				sText.Read(Stream);
				return	(Stream);
			}

			void				ValueOf(const float rValue);
			static QDT_STRING	ToString(const float rValue);
			void				ValueOf(const int nValue);
			static QDT_STRING	ToString(const int nValue);
			void				ValueOf(const bool bValue);
			static QDT_STRING	ToString(const bool bValue);
			void				ValueOf(const double rValue);
			static QDT_STRING	ToString(const double rValue);
			void				ValueOf(const UInt64 & nValue);
			static QDT_STRING	ToString(const UInt64 & nValue);

			static bool			IsAlpha( unsigned char c );
			static bool			IsNumeric( unsigned char c );
			static bool			IsAlphaNumeric(unsigned char c );

			/** Creates a string from a char buffer
			*	Can be used to output a binary stream to a text format
			* @param	pBuffer	the char buffer to output as hexadecimal values
			* @param	nSize	the buffer size
			*/
			void				ConvertBufferToHexaString(const char * pBuffer, const int& nSize);
			
			/** Convert the string to a char buffer
			*	Can be used to get a binary stream from a text format
			* @param	pBuffer	the char buffer that will receive the binary stream (MUST be NULL)
			* @param	nSize	the buffer size that will receive the stream size
			*/
			void				ConvertHexaStringToBuffer(char *& pBuffer, int& nSize) const;

			//function that splits a full path file name
			static bool			SplitPathFileName(const QDT_STRING &sCompletePath, QDT_STRING &sPath, QDT_STRING &sName);

			void				WriteDependenciesBinary(IO_STREAM & Stream, int nThisOffset);
			void				InitBinary();

			static void			Print(IO_STREAM * pStream);
			static int			GetMemoryUsed();
			static int			GetFreeMem();

			static				void DumpAllocated();

		private:
			char				GetAt(const int nPosition) const;
			void				SetAt(const int nPosition, const int nChar);
			char *				GetBufferWithOffset(const int nPosition) const;

			void				NeedSpace(const int nSize);
			void				SetSize(const int nSize);
			void				SetMaxSize(const int nMaxSize);
			//void				SetBuffer(char * pBuffer);

			bool				Verify() const;

			void				ReleaseBuffer();

			char *				GetBufferPtr() const;	//not const counter part to GetBuffer() for internal use only

			enum
			{
				RATIO_INCREASE_BUFFER	=	5,
				RATIO_BASE				=	4,
			};

			char *						_pBuffer;
			int							_nSize;
			int							_nAllocSize;

#ifdef USE_NEW_MEMPOOL
			static MEM_POOL_MANAGER *	_pMPManager;
#else
			static MEM_POOL_HEADER *	_pMPH8;
			static MEM_POOL_HEADER *	_pMPH16;
			static MEM_POOL_HEADER *	_pMPH32;
			static MEM_POOL_HEADER *	_pMPH64;
			static MEM_POOL_HEADER *	_pMPH128;
			static MEM_POOL_HEADER *	_pMPH256;
			static MEM_POOL_HEADER *	_pMPH512;
#endif

		#ifdef QDT_STRING_TRACK_NEW_DELETE
		#define QDT_STRING_HASH_SIZE 8192
			static QDT_DLIST<QDT_STRING*> * pAllocated;
			static QDT_STRING *				pSTracked;
			void	AddAllocated(QDT_STRING * pS);
			void	RemoveAllocated(QDT_STRING * pS);
			int		HasCodeAllocated(QDT_STRING * pS);
		#endif
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
