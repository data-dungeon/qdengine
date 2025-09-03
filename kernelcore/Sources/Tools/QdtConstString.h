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
//	CLASS:	QDT_CONST_STRING
//	The QDT_CONST_STRING class implements ...
//
//	01-12-11:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
// uncomment to know if you call QDT_CONST_STRING constructor twice on the same memory pointer creating memory leak.
// like a QDT_NEW_PLACED wrongly placed ? 
//#define QDT_STRING_TRACK_NEW_DELETE

#ifndef __KCORE_TOOLS_QDT_CONST_STRING_H__
#define __KCORE_TOOLS_QDT_CONST_STRING_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM);
DECL_KCORE(MEM_POOL_HEADER);

#include	INCL_KCORE(Tools/QdtString)

#include INCL_KCORE(Template/QDT_DList)

#ifndef ENABLE_QDT_CONST_STRING
#define QDT_CONST_STRING QDT_STRING

#else

namespace QDT
{
	namespace KCORE
	{
		class	QDT_CONST_STRING
		{
		public:
	
			friend class QDT_STRING;

			static const QDT_CONST_STRING	CONST_EMPTY_STRING;

			QDT_CONST_STRING(const QDT_CONST_STRING & sText)
			{
				_pBuffer = sText._pBuffer;
			}

			QDT_CONST_STRING(const char * const psText)
			{
				_pBuffer = psText;
			}

			void	Copy(const char * const pBuffer);
			void	Copy(const char * const pBuffer, const int nSize);

			void	Construct();
			void	Destruct();

			~QDT_CONST_STRING()
			{
			}

			void				Reserve(const int nSize = 0);
			void				Resize(const int nSize, const int nChar = ' ');

			/**	Return the length of the string */
			int					GetSize() const;
			void				UpdateSize();
			/**	Return the size reserved for the string */
			int					GetMaxSize() const;
			const char *		GetBuffer() const
			{
				return _pBuffer;
			}
			
			bool				IsEmpty() const;
			void				Empty(const bool bPartial = false);

			const char &		operator[](const int nPosition) const
			{
				QDT_ASSERT(nPosition >= 0);
				QDT_ASSERT(nPosition <= GetSize());
				return	(_pBuffer[nPosition]);
			}

			operator const char * () const;
			operator const char * ();

			//needed for elipse C function such as sprintf, otherwise it causes scratch when GetBuffer() is not explicitly called
			//operator char * ();
		
			float				Atof() const;
			int					Atoi() const;
			long				Atol() const;
			bool				Atob() const;

			void				SetFromTime(const int nTimeMs);
			void				SetFromFileSize(const int nFileSizeBytes);

			bool				CompareNoCase(const QDT_CONST_STRING & sText) const;
			bool				CompareNoCase(const char * const psText) const;

			bool				operator==(const QDT_CONST_STRING & sText) const;
			bool				operator==(const char * const psText) const;
			bool				operator!=(const QDT_CONST_STRING & sText) const;
			bool				operator!=(const char * const psText) const;

			bool				operator<(const QDT_CONST_STRING & sText) const;
			bool				operator<(const char * const psText) const;
			bool				operator<=(const QDT_CONST_STRING & sText) const;
			bool				operator<=(const char * const psText) const;
			bool				operator>(const QDT_CONST_STRING & sText) const;
			bool				operator>(const char * const psText) const;
			bool				operator>=(const QDT_CONST_STRING & sText) const;
			bool				operator>=(const char * const psText) const;

			void				MakeUpper();
			QDT_STRING			ToUpper() const;
			void				MakeLower();
			QDT_STRING			ToLower() const;

		//	int					Replace(const QDT_CONST_STRING & sOld, const QDT_CONST_STRING & sNew);

		//	void				Format(const char * const psFormatText, ...);

			int					Find(const QDT_CONST_STRING & sText, const int nStartPos = 0) const;
			int					Find(const int nChar, const int nStartPos = 0) const;
			int					ReverseFind(const QDT_CONST_STRING & sText, const int nStartPos = 0) const;
			int					ReverseFind(const int nChar, const int nStartPos = 0) const;

		/*	int					FindCount(const QDT_CONST_STRING & sText, const int nStartPos = 0) const;
			int					FindCount(const int nChar, const int nStartPos = 0) const;

			QDT_CONST_STRING			EraseMid(const int nFirst, const int nCount) const;
			QDT_CONST_STRING			EraseMid(const int nFirst) const;
			QDT_CONST_STRING			EraseLeft(const int nCount) const;
			QDT_CONST_STRING			EraseRight(const int nCount) const; */

			QDT_CONST_STRING			ExtractMid(const int nFirst, const int nCount) const;
			QDT_CONST_STRING			ExtractMid(const int nFirst) const;
			QDT_CONST_STRING			ExtractLeft(const int nCount) const;
			QDT_CONST_STRING			ExtractRight(const int nCount) const;

/*			int					TrimLeft(const char cTrim = '\0');
			int					TrimRight(const char cTrim = '\0');

			void				Reverse(); */

			void				Read(IO_STREAM & Stream);
			void				Write(IO_STREAM & Stream) const;

			friend IO_STREAM &	operator<<(IO_STREAM & Stream, const QDT_CONST_STRING & sText)
			{
				sText.Write(Stream);
				return	(Stream);
			}

			friend IO_STREAM &	operator>>(IO_STREAM & Stream, QDT_CONST_STRING & sText)
			{
				sText.Read(Stream);
				return	(Stream);
			}

/*			void				ValueOf(const float rValue);
			static QDT_CONST_STRING	ToString(const float rValue);
			void				ValueOf(const int nValue);
			static QDT_CONST_STRING	ToString(const int nValue);
			void				ValueOf(const bool bValue);
			static QDT_CONST_STRING	ToString(const bool bValue);
			void				ValueOf(const double rValue);
			static QDT_CONST_STRING	ToString(const double rValue); */

			static bool			IsAlpha( unsigned char c );
			static bool			IsNumeric( unsigned char c );
			static bool			IsAlphaNumeric(unsigned char c );

			//function that splits a full path file name
			static bool			SplitPathFileName(const QDT_CONST_STRING &sCompletePath, QDT_CONST_STRING &sPath, QDT_CONST_STRING &sName);

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
			//void				SetBuffer(char * pBuffer);

			bool				Verify() const;

			enum
			{
				RATIO_INCREASE_BUFFER	=	5,
				RATIO_BASE				=	4,
			};

			const char *						_pBuffer;

		#ifdef QDT_STRING_TRACK_NEW_DELETE
		#define QDT_STRING_HASH_SIZE 8192
			static QDT_DLIST<QDT_CONST_STRING*> * pAllocated;
			static QDT_CONST_STRING *				pSTracked;
			void	AddAllocated(QDT_CONST_STRING * pS);
			void	RemoveAllocated(QDT_CONST_STRING * pS);
			int		HasCodeAllocated(QDT_CONST_STRING * pS);
		#endif
		};
	}
}

#endif

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
