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
//
//	01-08-24:	DAC - Created
//*****************************************************************************

#include "Root.h"
#include INCL_KCORE(Tools/QDT_StringTokenizer)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING_TOKENIZER constructor
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING_TOKENIZER::QDT_STRING_TOKENIZER()
{
}

//-----------------------------------------------------------------------------
//	Name:		~QDT_STRING_TOKENIZER destructor
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING_TOKENIZER::~QDT_STRING_TOKENIZER()
{
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING_TOKENIZER constructor
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING_TOKENIZER::QDT_STRING_TOKENIZER(const QDT_STRING_TOKENIZER &	qdtStrTokenizer)
{
	*this = qdtStrTokenizer;
}

	

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING_TOKENIZER &	QDT_STRING_TOKENIZER::operator=(const QDT_STRING_TOKENIZER &	qdtStrTokenizer)
{
	if( this != &qdtStrTokenizer)
	{
		Copy( qdtStrTokenizer );
	}
	return( *this );
}


	
//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_TOKENIZER::Copy(const QDT_STRING_TOKENIZER &	qdtStrTokenizer)
{
	_tVectTokens = 	qdtStrTokenizer._tVectTokens;
}


//-----------------------------------------------------------------------------
//	Name:		Tokenize
//	Object:		return the number of token found , access to token with operator[]
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
unsigned int	QDT_STRING_TOKENIZER::Tokenize(const QDT_STRING& strToTokenize, 
												const QDT_STRING& strDelim)
{
	_tVectTokens.Clear();

	int			nPos;
	QDT_STRING	strTmp;
	QDT_STRING	strTag;

	strTmp = strToTokenize;
	
	//remove space and extract Tags
	while	((strTmp.IsEmpty() == false) || (strTmp.Find(strDelim) != -1) )
	{
		nPos = strTmp.Find( strDelim );
	
		if	(nPos != -1)
		{
			//extract the tag
			strTag = strTmp.ExtractLeft( nPos );
		}
		else
		{
			strTag = strTmp;
		}

		//strTag.Replace(" ",""); //remove
		
		if( strTag.IsEmpty() != true )
		{
			AddToken( strTag );
		}
		
		//remove it
		if	(nPos != -1)
		{	
			strTmp = strTmp.EraseLeft( nPos + strDelim.GetSize() );
		}
		else
		{
			//remove the tag, it is the last one
			strTmp.Replace(strTag , "");
		}

	}

	return	(GetNbrTokens());

}

//-----------------------------------------------------------------------------
//	Name:		TokenizeFromMask
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
unsigned int	QDT_STRING_TOKENIZER::TokenizeFromMask(const QDT_STRING &	strToTokenize,
														const QDT_STRING		strMask)
{
	QDT_STRING	strTmp;
	QDT_STRING	strSeparator;
	QDT_STRING	strToken;
	int			nNbrTokens;
	int			nSeparatorSize;
	int			nPos;
	int			nEraseSize;
	bool		bRightTrim;

	nNbrTokens	= Tokenize( strMask, "*" );
	strTmp		= strToTokenize;
	
	if( nNbrTokens != 0 )
	{
		QDT_VECTOR< QDT_STRING> tVectSeparators;
		tVectSeparators = _tVectTokens;
		
		_tVectTokens.Clear();
		
		//read chars until separator found
		for(int i = 0; i < nNbrTokens; ++i )
		{
			strSeparator = tVectSeparators[i];
			
			//remove any leading or ending space \t \r \n
			nSeparatorSize = strSeparator.GetSize();

			bRightTrim = false;
			if	(nSeparatorSize > 1)
			{
				strSeparator.TrimLeft();
				
				//if we trimmed the right part
				if	(strSeparator.TrimRight() > 0)
				{
					bRightTrim = true;	
				}
			}
			nPos = strTmp.Find( strSeparator );

			//if found, extract left 
			if	(nPos != -1)
			{
				strToken = strTmp.ExtractLeft( nPos );	
				
				//if the token found is not empty
				if( strToken.IsEmpty() == false )
				{
					AddToken( strToken);
				}
				
				//clamp if over size
				if	(bRightTrim == true)
				{
					nEraseSize = nPos + nSeparatorSize;
				}
				else
				{
					nEraseSize = nPos + strSeparator.GetSize();
				}

				if	(nEraseSize > strTmp.GetSize())
				{
					nEraseSize = strTmp.GetSize();
				}

				strTmp = strTmp.EraseLeft( nEraseSize );
			}
			else
			{
				//a token can't be matched !
				return( 0 );
			}
		}

		//if there is something after the last separator, add it
		if	(strTmp.IsEmpty() == false)
		{
			AddToken( strTmp );
		}
	}
	
	return	(GetNbrTokens());
}


//-----------------------------------------------------------------------------
//	Name:		AddToken
//	Object:		
//	01-10-12:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_TOKENIZER::AddToken(const QDT_STRING &	strToken)
{
	_tVectTokens.PushTail( strToken);
}


//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-28:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING&	QDT_STRING_TOKENIZER::operator[](int	nTokenNbr) const
{
	return	(GetToken(nTokenNbr));
}

//-----------------------------------------------------------------------------
//	Name:		GetToken
//	Object:		
//	01-10-12:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING&	QDT_STRING_TOKENIZER::GetToken(const unsigned int	nTokenNbr) const
{
	return	(_tVectTokens[nTokenNbr]);
}

//-----------------------------------------------------------------------------
//	Name:		GetTokenPtr
//	Object:		
//	01-12-18:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING * const	QDT_STRING_TOKENIZER::GetTokenPtr(const unsigned int	nTokenNbr) const
{
	return	(&GetToken(nTokenNbr));
}


//-----------------------------------------------------------------------------
//	Name:		GetNbrTokens
//	Object:		
//	01-09-18:	DAC - Created
//-----------------------------------------------------------------------------
unsigned int	QDT_STRING_TOKENIZER::GetNbrTokens() const
{
	return	(_tVectTokens.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		
//	01-09-18:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_TOKENIZER::Dump(QDT_STRING &	strOutput)
{
	QDT_STRING		strTmp;
	unsigned int	i, nNbrTokens;
	
	nNbrTokens = _tVectTokens.GetSize();
	for	(i = 0 ; i < nNbrTokens ; ++i)
	{
		strTmp.Format("tok%i:%s\n",i,GetToken(i).GetBuffer());
		strOutput += strTmp;
	}
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
