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
//	CLASS:	QDT_STRING_TOKENIZER
//	The QDT_STRING_TOKENIZER class implements ...
//
//	01-08-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TOOLS_QDT_STRINGTOKENIZER_H__
#define __KCORE_TOOLS_QDT_STRINGTOKENIZER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(Template/QDT_Vector)

namespace QDT
{
	namespace KCORE
	{
		class	QDT_STRING_TOKENIZER
		{
		public:
			QDT_STRING_TOKENIZER();
			QDT_STRING_TOKENIZER(const QDT_STRING_TOKENIZER& qdtStrTokenizer);
			~QDT_STRING_TOKENIZER();

			QDT_STRING_TOKENIZER& operator= ( const QDT_STRING_TOKENIZER& qdtStrTokenizer);

			unsigned int	TokenizeFromMask( const QDT_STRING& strToTokenize, const QDT_STRING strMask);

			unsigned int Tokenize( const QDT_STRING& strToTokenize, const QDT_STRING& strDelim );
			
			const QDT_STRING&	operator[]( int nTokenNbr ) const;

			unsigned int				GetNbrTokens() const;
			const QDT_STRING&			GetToken(const unsigned int nTokenNbr) const;
			const QDT_STRING * const	GetTokenPtr(const unsigned int nTokenNbr) const;

			static QDT_STRING	GetMatchAllDelimiter() { return("*"); }

			void	Dump(QDT_STRING& strOutput);

		protected:
			void	Copy ( const QDT_STRING_TOKENIZER& qdtStrTokenizer);
			void	AddToken(const QDT_STRING& strToken);

		private:

			QDT_VECTOR< QDT_STRING > _tVectTokens;
		};
	}
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
