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
//	CLASS:	TEXT_PARSER
//
//	01-10-17:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\..\Include.h"
#include	INC_KRNCORE(Memory)
#include	INC_3DENGINE(Display/Viewport)
#include	INC_3DENGINE(Interface2D3D/DisplayFont)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

MEM_POOL_HEADER *			TEXT_PARSER::_pWordPool = NULL;
MEM_POOL_HEADER *			TEXT_PARSER::_pTextLinePool = NULL;
QDT_VECTOR<IHM_TEXT_LINE*>	TEXT_PARSER::_vOrderedText;

QDT_VECTOR <QDT_STRING>		TEXT_PARSER::_vFind;
QDT_VECTOR <QDT_STRING>		TEXT_PARSER::_vReplace;

//-----------------------------------------------------------------------------
//	Name:		TEXT_PARSER constructor
//	Object:		
//	01-10-17:	GGO - Created
//-----------------------------------------------------------------------------
TEXT_PARSER::TEXT_PARSER(IHM_ELEMENT_TEXT *pElementText)
:
_pLastWord			( NULL ),
_pElementText		( pElementText ),
_bNewParagraph		( true ),
_bOneLinedParagraph	( false ),
_TextProperties		( NONE ),
_n1stLineIndent		( 0 ),
_rTextSizePercent	( 100 ),
_TextColor			( 255, 255, 255, 255 ),
_rVerticalPosPercent( 50.0f ),
_rScrollSpeed		( 1.0f )
{	
}

//-----------------------------------------------------------------------------
//	Name:		TEXT_PARSER destructor
//	Object:		
//	01-10-17:	GGO - Created
//-----------------------------------------------------------------------------
TEXT_PARSER::~TEXT_PARSER()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SetIhmElementText
//	Object:		
//	03-12-19:	VMA - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::SetIhmElementText(IHM_ELEMENT_TEXT *	pElementText)
{
	_pElementText = pElementText;
}

//-----------------------------------------------------------------------------
//	Name:		AllocateWord
//	Object:		
//	04-05-05:	RMA - Created
//-----------------------------------------------------------------------------
IHM_WORD *	TEXT_PARSER::AllocateWord()
{
	IHM_WORD	*pNew;

	if	(_pWordPool == NULL)
	{
		_pWordPool = new MEM_POOL_HEADER(sizeof(IHM_WORD), 370, 256); // 256 256
	}

	pNew = static_cast<IHM_WORD*>(_pWordPool->Allocate());
	
	QDT_NEW_PLACED(pNew, IHM_WORD)();

	return (pNew);
}

//-----------------------------------------------------------------------------
//	Name:		AllocateWord
//	Object:		
//	04-05-05:	RMA - Created
//-----------------------------------------------------------------------------
IHM_TEXT_LINE * TEXT_PARSER::AllocateTextLine()
{
	IHM_TEXT_LINE	*pNew;

	if	(_pTextLinePool == NULL)
	{
		_pTextLinePool = new MEM_POOL_HEADER(sizeof(IHM_TEXT_LINE), 150, 64); // 64 64
	}

	pNew = static_cast<IHM_TEXT_LINE*>(_pTextLinePool->Allocate());
	
	QDT_NEW_PLACED(pNew, IHM_TEXT_LINE)();

	return (pNew);
}

//-----------------------------------------------------------------------------
//	Name:		AllocateWord
//	Object:		
//	04-05-05:	RMA - Created
//-----------------------------------------------------------------------------
void TEXT_PARSER::UnallocateWord(IHM_WORD * pW)
{
	if	(pW)
	{
		static_cast<IHM_WORD*>(pW)->IHM_WORD::~IHM_WORD();
		_pWordPool->Deallocate(pW);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AllocateWord
//	Object:		
//	04-05-05:	RMA - Created
//-----------------------------------------------------------------------------
void TEXT_PARSER::UnallocateTextLine(IHM_TEXT_LINE * pTL)
{
	if	(pTL)
	{
		static_cast<IHM_TEXT_LINE*>(pTL)->IHM_TEXT_LINE::~IHM_TEXT_LINE();
		_pTextLinePool->Deallocate(pTL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ApplyFindAndReplace
//	Object:		
//	05-03-21:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::ApplyFindAndReplace(QDT_STRING & Text)
{
	unsigned int i, nCount = _vFind.GetSize();

	if (nCount > 0)
	{
		// First pass to allow swapping between two elements
		for (i = 0 ; i < nCount ; ++i)
		{
			QDT_STRING sSwap("Tontontontontontondusera");
			sSwap +=  (int)i;
			Text.Replace(_vFind[i], sSwap);
		}

		// This is the replace loop, pay attention that we loop from the greatest to 
		// the smallest to avoid bug!
		for (i = nCount - 1 ; i > 0 ; i--)
		{
			QDT_STRING sSwap("Tontontontontontondusera");
			sSwap +=  (int)i;
			Text.Replace(sSwap, _vReplace[i]);
		}

		QDT_STRING sSwap("Tontontontontontondusera");
		sSwap +=  (int)i;
		Text.Replace(sSwap, _vReplace[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Process
//	Object:		
//	01-10-18:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::Process(VIEWPORT *	pRenderingViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	unsigned char				cCurrentLetter;
	const DISPLAY_FONT_LETTER  *pLetterDescriptor;
	float						rCharPixelSize;
	float						rOldPosX = 0, rOldPosY = 0, rOldPixelSize = 0;
	IHM_TEXT_LINE			   *pCurrentLine	=  NULL;
	IHM_WORD				   *pCurrentWord	=  NULL;
	DISPLAY_FONT			   *pFont			= _pElementText->GetFont();
	float						rResolutionRatioX;
	int							nSpacesCount;

	// First we clear the previous text.
	Clear();

	// Set the current text position to the beginning.
	QDT_STRING Text = _pElementText->GetText();
	ApplyFindAndReplace(Text);

	_pCurrentPos = Text.GetBuffer();

	// Reset some flags.
	_rTextSizePercent	= 100.0f;
	_n1stLineIndent		= 0;
	_bFirstLine			= true;
	_bNewParagraph		= true;
	_bEmptyLine			= true;
	_bLastLine			= false;
	rResolutionRatioX	= (float)RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX() / (float)BASE_HRES;

	if ( pRenderingViewport != NULL )
	{
		_rTextSizePercent *= pRenderingViewport->GetWidth() / 100.0f;
	}

	// Parse the text.
	while ( _pCurrentPos != 0 )
	{
		// Save the size and position of the previous word.
		if ( pCurrentWord != NULL )
		{
			rOldPosX		= pCurrentWord->_rPosX;
			rOldPosY		= pCurrentWord->_rPosY;
			rOldPixelSize	= pCurrentWord->_rPixelSize;
		}

		// The beginning of the word.
		nSpacesCount = 0;
		while ( *_pCurrentPos == ' ' )
		{
			++_pCurrentPos;
			++nSpacesCount;
		}
		if ( *_pCurrentPos == 0 )
		{
			break;
		}

		// Create a new IHM_WORD
		pCurrentWord									=  AllocateWord();
		pCurrentWord->_pBuffer							= _pCurrentPos;
		pCurrentWord->_StringAttributes._Color			= _TextColor;
		pCurrentWord->_StringAttributes._rSizePercent	= _rTextSizePercent;

		// Its length (in pixels and letters number).
		pCurrentWord->_nLetterNbr	= 0;
		pCurrentWord->_rPixelSize	= 0;
		while ( ( *_pCurrentPos != ' ' ) && ( *_pCurrentPos != 0 ) && ( *_pCurrentPos != '{' ) )
		{
			// We increase the pixel size of the word.
			cCurrentLetter		= *_pCurrentPos;

			// Insecable space.
			if ( cCurrentLetter == 167 )
			{
				pCurrentWord->_rPixelSize += rResolutionRatioX * _rTextSizePercent * pFont->GetWidth() / 300.0f;
			}
			// Displayable charactere.
			else
			{
			pLetterDescriptor	=   pFont->GetLetterDescriptor(cCurrentLetter - DISPLAY_FONT_OFFSET);
			rCharPixelSize		=	pLetterDescriptor->_nRight - pLetterDescriptor->_nLeft + 1;
			rCharPixelSize		+=	pFont->GetInterLetterOffset();
			rCharPixelSize		*=  rResolutionRatioX * _rTextSizePercent * 0.01f;

			pCurrentWord->_rPixelSize += rCharPixelSize;
			}

			++pCurrentWord->_nLetterNbr;
			++_pCurrentPos;
		}

		// If non emtpy word, set the word position and add it into the word list '_vOrderedText'.
		if ( pCurrentWord->_nLetterNbr > 0 )
		{
			if ( _bNewParagraph == true )
			{
				// Insert a new line.
				pCurrentLine = AllocateTextLine();
				_vOrderedText.PushTail( pCurrentLine );

				pCurrentWord->_rPosX	= _pElementText->GetAbsolutePosLeft(pRenderingViewport) + _n1stLineIndent;
				_bNewParagraph			= false;
			}
			else
			{
				pCurrentWord->_rPosX = rOldPosX + rOldPixelSize + nSpacesCount * rResolutionRatioX * _rTextSizePercent * pFont->GetWidth() / 300.0f;
				pCurrentWord->_rPosY = rOldPosY;

				// Carriage return if outside the IHM_TEXT_ELEMENT.
				// (except if the paragraph is onelined).
				if ( ( pCurrentWord->_rPosX + pCurrentWord->_rPixelSize > _pElementText->GetAbsolutePosRight(pRenderingViewport) ) &&
					 ( _bOneLinedParagraph == false ) )
				{
					// Align the current lign depending of the current text alignement.
					AlignCurrentLine(pRenderingViewport);

					// Insert new line.
					pCurrentLine = AllocateTextLine();
					_vOrderedText.PushTail( pCurrentLine );

					pCurrentWord->_rPosX = _pElementText->GetAbsolutePosLeft(pRenderingViewport);
				}
			}

			// We insert the word in the last line.
			_pLastWord = pCurrentWord;
			pCurrentLine->PushWord( pCurrentWord );

			_bEmptyLine	= false;
		}
		else
		{
			UnallocateWord(pCurrentWord);
		}

		// New balise?
		while ( ( *_pCurrentPos == '{' ) && ( *_pCurrentPos != 0 ) )
		{
			if ( ChangeWordProperties( pRenderingViewport ) == false )
			{
				return;
			}
		}
	}

	// We need to align the last line.
	_bLastLine = true;
	AlignCurrentLine(pRenderingViewport);

	// And we do the vertical alignement of the whole text.
	PerformVerticalAlign(pRenderingViewport);*/
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::Clear()
{
	unsigned int i;
	for (i = 0 ; i < _vOrderedText.GetSize() ; ++i)
	{
		UnallocateTextLine(_vOrderedText[i]);
	}

	_vOrderedText.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		MoveFirstWord
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::MoveFirstWord()
{
	if ( _vOrderedText.GetSize() > 0 )
	{
		QDT_ASSERT(_vOrderedText[0]->GetNbrWords() > 0 );

		_nCurrentLine = 0;
		_nCurrentWord = 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		MoveNextWord
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
bool	TEXT_PARSER::MoveNextWord()
{
	// Empty text.
	if ( _vOrderedText.GetSize() == 0 )
	{
		return ( false );
	}

	// The last word.
	if ( _vOrderedText[_nCurrentLine]->GetWord(_nCurrentWord) == _pLastWord )
	{
		return ( false );
	}

	// Move the first, and, if needed, jump to next line.
	++_nCurrentWord;
	
	if (_nCurrentWord == _vOrderedText[_nCurrentLine]->GetNbrWords())
	{
		++_nCurrentLine;
		_nCurrentWord = 0;
	}
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentWord
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
const IHM_WORD *	TEXT_PARSER::GetCurrentWord()
{
	if ( _vOrderedText.GetSize() == 0 )
	{
		return ( NULL );
	}

	QDT_ASSERT( _nCurrentLine < _vOrderedText.GetSize() );

	return (_vOrderedText[_nCurrentLine]->GetWord(_nCurrentWord));
}

//-----------------------------------------------------------------------------
//	Name:		ChangeWordProperties
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
bool	TEXT_PARSER::ChangeWordProperties(VIEWPORT * pRenderingViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	QDT_ASSERT( *_pCurrentPos == '{' );

	// The window size.
	int	nWindowWidth = (float)RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX();

	++_pCurrentPos;

	switch ( *_pCurrentPos )
	{
	// New paragraph.
	//***************
	case 'P':

		++_pCurrentPos;

		// We must align the last line of the previous paragraph (if previous
		// paragraph was justified, its last line must be left aligned).
		if ( _TextProperties.IsSet( ALIGN_JUSTIFIED ) == true )
		{
			_TextProperties.Clear();
			_TextProperties.Set( ALIGN_LEFT );
		}

		// Align the line.
		AlignCurrentLine(pRenderingViewport);

		// Change some flags
		_bNewParagraph		= true;
		_bOneLinedParagraph	= false;
		_bEmptyLine			= true;

		// We remove previous alignement informations.
		_TextProperties.Clear();

		// Alignement informations?
		if ( *_pCurrentPos == ':' )
		{
			++_pCurrentPos;

			switch ( *_pCurrentPos )
			{
			case 'L':
				_TextProperties.Set( ALIGN_LEFT );
				break;

			case 'R':
				_TextProperties.Set( ALIGN_RIGHT );
				break;

			case 'C':
				_TextProperties.Set( ALIGN_CENTERED );
				break;

			case 'J':
				_TextProperties.Set( ALIGN_JUSTIFIED );
				break;

			default:
				return ( false );
			}
			++_pCurrentPos;

			// One lined paragraph.
			if ( *_pCurrentPos == 'O' )
			{
				_bOneLinedParagraph = true;
				++_pCurrentPos;
			}

			// 1st line indentation;
			if ( *_pCurrentPos == ':' )
			{
				_n1stLineIndent = 0;

				++_pCurrentPos;

				while ( ( *_pCurrentPos != ':' ) && ( *_pCurrentPos != '}' ) )
				{
					_n1stLineIndent *= 10;

					if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
					{
						return ( false );
					}

					_n1stLineIndent += *_pCurrentPos - '0';

					++_pCurrentPos;
				}

				// We convert from relative permil into pixels.
				_n1stLineIndent = _n1stLineIndent * nWindowWidth / 1000;
			}
			else
			{
				_n1stLineIndent = 0;
			}
		}

		break;

	// New text size.
	//***************
	case 'S':

		++_pCurrentPos;

		if ( *_pCurrentPos != ':' )
		{
			return ( false );
		}

		while ( *_pCurrentPos != '}' )
		{
			_rTextSizePercent = 0;

			++_pCurrentPos;

			while ( *_pCurrentPos != '}' )
			{
				_rTextSizePercent *= 10.0f;

				if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
				{
					return ( false );
				}

				_rTextSizePercent += *_pCurrentPos - '0';

				++_pCurrentPos;
			}

			if ( pRenderingViewport != NULL )
			{
				_rTextSizePercent  = _rTextSizePercent * pRenderingViewport->GetWidth() / 100.0f;
			}
		}

		break;

	// New text color.
	//****************
	case 'C':

		++_pCurrentPos;

		if ( *_pCurrentPos != ':' )
		{
			return ( false );
		}
		else
		{
			++_pCurrentPos;
		}

		if ( ChangeTextColor() == false )
		{
			return ( false );
		}

		break;

	default:
		return ( false );
	}

	// Closing bracket present?
	if ( *_pCurrentPos == '}' )
	{
		++_pCurrentPos;
		return ( true );
	}
	else
	{
		return ( false );
	}*/

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		AlignCurrentLine
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::AlignCurrentLine(VIEWPORT * pRenderingViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	IHM_TEXT_LINE *	pCurrentLine;
	unsigned int	i, nWordNbr;
	DISPLAY_FONT *	pFont;
	IHM_WORD *		pLastLineWord;
	float			rOffset, rResolutionRatioX, rResolutionRatioY;

	// Init.
	pFont				= _pElementText->GetFont();
	pCurrentLine		=  NULL;
	nWordNbr			=  0;
	rResolutionRatioX	= (float)RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX() / (float)BASE_HRES;
	rResolutionRatioY	= (float)RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeY() / (float)BASE_VRES;

	// The fist line Y position.
	if ( _bFirstLine == true )
	{
		if ( _bEmptyLine == true )
		{
			return;
		}

		_rLinePosY  = _pElementText->GetAbsolutePosTop(pRenderingViewport) + rResolutionRatioY * _rTextSizePercent * pFont->GetVerticalOffsetHeight() / 100.0f;

		_bFirstLine = false;
	}
	else
	{
		_rLinePosY += rResolutionRatioY * _rTextSizePercent * pFont->GetHeight() / 100.0f;
	}


	// Empty line.
	if ( _bEmptyLine == false )
	{
		// Init some variables.
		pCurrentLine	= *(_vOrderedText.GetLast());
		nWordNbr		= pCurrentLine->GetNbrWords();

		// Set the Y position of the line.
		for ( i = 0; i < nWordNbr; ++i )
		{
			pCurrentLine->GetWord(i)->_rPosY = _rLinePosY;
		}
	}

	// If the line is empty, return.
	if ( _bEmptyLine == true )
	{
		return;
	}

	// What is the current alignement?
	if ( _TextProperties.IsSet( ALIGN_LEFT ) == true )
	{
		// We do nothing since the text already is left aligned.
		return;
	}
	// Last line is left aligned, even with a justified alignment.
	else
	if ( ( _bLastLine == true ) && ( _TextProperties.IsSet( ALIGN_JUSTIFIED ) == true ) )
	{
		// We do nothing since the text already is left aligned.
		return;
	}
	else
	{
		pLastLineWord	= pCurrentLine->GetWord(pCurrentLine->GetNbrWords()-1);
		rOffset			= _pElementText->GetAbsolutePosRight(pRenderingViewport);
		rOffset			-= pLastLineWord->_rPosX;
		rOffset			-= pLastLineWord->_rPixelSize;
		rOffset			+= pFont->GetInterLetterOffset() * rResolutionRatioX;

		// Right alignement.
		if ( _TextProperties.IsSet( ALIGN_RIGHT ) )
		{
			for ( i = 0; i < nWordNbr; ++i )
			{
				pCurrentLine->GetWord(i)->_rPosX += rOffset;
			}
		}
		else
		// Justified alignement.
		if ( _TextProperties.IsSet( ALIGN_JUSTIFIED ) )
		{
			// If there is a single word in the line, the word is left aligned.
			if ( nWordNbr > 1 )
			{
				for ( i = 0; i < nWordNbr; ++i )
				{
					pCurrentLine->GetWord(i)->_rPosX +=  i * rOffset / ( nWordNbr - 1 );
				}
			}
		}
		else
		// Centered alignement.
		if ( _TextProperties.IsSet( ALIGN_CENTERED ) )
		{
			for ( i = 0; i < nWordNbr; ++i )
			{
				pCurrentLine->GetWord(i)->_rPosX += rOffset / 2;
			}
		}
	}*/
}


//-----------------------------------------------------------------------------
//	Name:		PerformVertivalAlign
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::PerformVerticalAlign(VIEWPORT * pRenderingViewport)
{
	// Empty text.
	if ( _vOrderedText.GetSize() == 0 )
	{
		return;
	}

	IHM_TEXT_LINE					*pCurrentLine;
	float			rTextTop, rTextBottom, rTextHeight;
	float			rIhmTop, rIhmBottom, rIhmHeight, rOffset;
	DISPLAY_FONT *	pFont;
	int				nWord, nLine, nWordNbr, nLineNbr;

	// Retrieve the font.
	pFont		= _pElementText->GetFont();

	// We look for the text y pos.
	rTextTop	= _pElementText->GetAbsolutePosTop( pRenderingViewport );
	rTextBottom	= _rLinePosY - pFont->GetMinVerticalOffset();
	rTextHeight	=  rTextBottom - rTextTop;

	// The Y position of the ihm_elementtext.
	rIhmTop		= _pElementText->GetAbsolutePosTop( pRenderingViewport );
	rIhmBottom	= _pElementText->GetAbsolutePosBottom( pRenderingViewport );
	rIhmHeight	=  rIhmBottom - rIhmTop;

	// The offset the text has to scroll to match the scroll position
	rOffset		= 0.01f * _rVerticalPosPercent * ( rIhmHeight - rTextHeight );

	nLineNbr	= _vOrderedText.GetSize();
	for ( nLine = 0; nLine < nLineNbr; ++nLine )
	{
		pCurrentLine	= _vOrderedText[ nLine ];
		nWordNbr		=  pCurrentLine->GetNbrWords();

		for ( nWord = 0; nWord < nWordNbr; ++nWord )
		{
			pCurrentLine->GetWord( nWord )->_rPosY += rOffset;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ScrollUp
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::ScrollUp(VIEWPORT * pRenderingViewport)
{
	int		nTextMin, nTextMax, nRectMin, nRectMax, nHiddenTextLength;
	float	rLeft, rRight, rTop, rBottom;

	if ( _vOrderedText.IsEmpty() == true )
	{
		Process( pRenderingViewport );
	}

	GetTextFullRect( rLeft, rRight, rTop, rBottom );
	nTextMin	= rTop;
	nTextMax	= rBottom;

	nRectMin	= _pElementText->GetAbsolutePosTop(pRenderingViewport);
	nRectMax	= _pElementText->GetAbsolutePosBottom(pRenderingViewport);

	// Don't scroll if the text fits.
	if ( ( nTextMin >= nRectMin ) && ( nTextMax <= nRectMax ) )
	{
		return;
	}

	nHiddenTextLength = nRectMin - nTextMin + nTextMax - nRectMax;


	_rVerticalPosPercent -= SCROLL_UP_DOWN_RATIO * _rScrollSpeed / (float)nHiddenTextLength;

	if ( _rVerticalPosPercent < 0.0f )
	{
		_rVerticalPosPercent = 0.0f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ScrollDown
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::ScrollDown(VIEWPORT * pRenderingViewport)
{
	int		nTextMin, nTextMax, nRectMin, nRectMax, nHiddenTextLength;
	float	rLeft, rRight, rTop, rBottom;

	if ( _vOrderedText.IsEmpty() == true )
	{
		Process( pRenderingViewport );
	}

	GetTextFullRect( rLeft, rRight, rTop, rBottom );
	nTextMin	= rTop;
	nTextMax	= rBottom;

	nRectMin	= _pElementText->GetAbsolutePosTop(pRenderingViewport);
	nRectMax	= _pElementText->GetAbsolutePosBottom(pRenderingViewport);

	// Don't scroll if the text fits.
	if ( ( nTextMin >= nRectMin ) && ( nTextMax <= nRectMax ) )
	{
		return;
	}

	nHiddenTextLength = nRectMin - nTextMin + nTextMax - nRectMax;


	_rVerticalPosPercent += SCROLL_UP_DOWN_RATIO * _rScrollSpeed / (float)nHiddenTextLength;

	if ( _rVerticalPosPercent > 100.0f )
	{
		_rVerticalPosPercent = 100.0f;
	}
}


//-----------------------------------------------------------------------------
//	Name:		Scroll
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::Scroll(VIEWPORT *	pRenderingViewport,
							const float	rScrollPercent)
{
	float	rTextMin, rTextMax, rRectMin, rRectMax, rDummy;

	Process( pRenderingViewport );

	GetTextFullRect( rDummy, rDummy, rTextMin, rTextMax );
	rRectMin	= _pElementText->GetAbsolutePosTop( pRenderingViewport );
	rRectMax	= _pElementText->GetAbsolutePosBottom( pRenderingViewport );

	// Don't scroll if the text fits.
	if ( ( rTextMin >= rRectMin ) && ( rTextMax <= rRectMax ) )
	{
		return;
	}

	_rVerticalPosPercent = rScrollPercent;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrLines
//	Object:		
//	04-10-13:	GGO - Created
//-----------------------------------------------------------------------------
int	TEXT_PARSER::GetNbrLines()
{
	if ( _vOrderedText.IsEmpty() == true )
	{
		Process( NULL );
	}

	return ( _vOrderedText.GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		ChangeTextColor
//	Object:		
//	01-10-24:	GGO - Created
//-----------------------------------------------------------------------------
bool	TEXT_PARSER::ChangeTextColor()
{
	int nRed, nGreen, nBlue, nAlpha;


	// Red.
	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nRed = 100 * ( *_pCurrentPos - '0' );
	++_pCurrentPos;

	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nRed += 10 * ( *_pCurrentPos - '0' );
	++_pCurrentPos;

	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nRed += *_pCurrentPos - '0';
	++_pCurrentPos;

	if ( nRed > 255 )
	{
		return ( false );
	}

	// Green.
	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nGreen = 100 * ( *_pCurrentPos - '0' );
	++_pCurrentPos;

	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nGreen += 10 * ( *_pCurrentPos - '0' );
	++_pCurrentPos;

	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nGreen += *_pCurrentPos - '0';
	++_pCurrentPos;

	if ( nGreen > 255 )
	{
		return ( false );
	}
	
	// Blue.
	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nBlue = 100 * ( *_pCurrentPos - '0' );
	++_pCurrentPos;

	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nBlue += 10 * ( *_pCurrentPos - '0' );
	++_pCurrentPos;

	if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
	{
		return ( false );
	}
	nBlue += *_pCurrentPos - '0';
	++_pCurrentPos;

	if ( nBlue > 255 )
	{
		return ( false );
	}


	// Alpha.
	if ( *_pCurrentPos == ':' )
	{
		++_pCurrentPos;
	
		if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
		{
			return ( false );
		}
		nAlpha = 100 * ( *_pCurrentPos - '0' );
		++_pCurrentPos;

		if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
		{
			return ( false );
		}
		nAlpha += 10 * ( *_pCurrentPos - '0' );
		++_pCurrentPos;

		if ( ( *_pCurrentPos < '0' ) || ( *_pCurrentPos > '9' ) )
		{
			return ( false );
		}
		nAlpha += *_pCurrentPos - '0';
		++_pCurrentPos;

		if ( nAlpha > 255 )
		{
			return ( false );
		}
	}
	else
	{
		nAlpha = 255;
	}


	// We change the text color.
	_TextColor.SetARGB( nAlpha, nRed, nGreen, nBlue );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		ChangeTextColor
//	Object:		
//	01-12-07:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::ChangeTextColor(const COLOR &	Color)
{
	_TextColor = Color;
}


//-----------------------------------------------------------------------------
//	Name:		SetVarticalAlign
//	Object:		
//	02-04-03:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::SetVarticalAlign(const IHM_ELEMENT_TEXT::VALIGN	VerticalAlign)
{
	switch ( VerticalAlign )
	{
	case IHM_ELEMENT_TEXT::VALIGN_CENTERED:
		_rVerticalPosPercent = 50.0f;
		break;

	case IHM_ELEMENT_TEXT::VALIGN_TOP:
		_rVerticalPosPercent = 0.0f;
		break;

	case IHM_ELEMENT_TEXT::VALIGN_BOTTOM:
		_rVerticalPosPercent = 100.0f;
		break;

	default:
		QDT_FAIL();
	}
}


//-----------------------------------------------------------------------------
//	Name:		SetScrollStep
//	Object:		
//	02-04-03:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::SetScrollStep(const int	nStep)
{
	_nScrollStep = nStep;
}


//-----------------------------------------------------------------------------
//	Name:		GetMemorySize
//	Object:		
//	02-01-21:	GGO - Created
//-----------------------------------------------------------------------------
int	TEXT_PARSER::GetMemorySize() const
{
	IHM_TEXT_LINE					*pCurrentLine;
	QDT_VECTOR<IHM_WORD*>::ITERATOR	CurrentWordIt;
	unsigned int					i, nLineNbr	= _vOrderedText.GetSize();
	int								nResult = 0;

	nResult += _vOrderedText.GetCapacity();
	for (i = 0; i < nLineNbr; ++i)
	{
		pCurrentLine = _vOrderedText[i];
		
		nResult	+= NBR_MAX_WORD_IN_A_LINE * sizeof(IHM_WORD *);
		nResult	+= pCurrentLine->GetNbrWords() * sizeof(IHM_WORD);
	}
	
	return ( nResult );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbLetters
//	Object:		
//	03-06-12:	VMA - Created
//-----------------------------------------------------------------------------
int	TEXT_PARSER::GetNbLetters() const
{
	IHM_TEXT_LINE					*pCurrentLine;
	unsigned int					nWordNbr;
	unsigned int					nIdxLine, nLineNbr	= _vOrderedText.GetSize(), nIdxWord;
	int								nResult = 0;

	for (nIdxLine = 0; nIdxLine < nLineNbr; ++nIdxLine)
	{
		pCurrentLine	= _vOrderedText[nIdxLine];
		nWordNbr		=  pCurrentLine->GetNbrWords();

		for (nIdxWord = 0; nIdxWord < nWordNbr; ++nIdxWord)
		{
			nResult	+= pCurrentLine->GetWord(nIdxWord)->_nLetterNbr;
		}
	}
	
	return ( nResult );
}

//-----------------------------------------------------------------------------
//	Name:		IsTextCropped
//	Object:		
//	04-11-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	TEXT_PARSER::IsTextCropped(VIEWPORT *	pRenderingViewport)
{
	static	const float	EPSILON = 0.1f;

	float	rLeft, rRight, rTop, rBottom, rRectLeft, rRectRight, rRectTop, rRectBottom;

	Process( pRenderingViewport );

	if ( _vOrderedText.IsEmpty() == true )
	{
		return ( false );
	}

	GetTextFullRect( rLeft, rRight, rTop, rBottom );
	rRectLeft	= 	_pElementText->GetAbsolutePosLeft(   pRenderingViewport );
	rRectRight	= 	_pElementText->GetAbsolutePosRight(  pRenderingViewport );
	rRectTop	= 	_pElementText->GetAbsolutePosTop(    pRenderingViewport );
	rRectBottom	= 	_pElementText->GetAbsolutePosBottom( pRenderingViewport );

	return ( ( rRectLeft - EPSILON > rLeft ) || ( rRectRight  + EPSILON < rRight  ) ||
			 ( rRectTop  - EPSILON > rTop  ) || ( rRectBottom + EPSILON < rBottom ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetTextTopBottom
//	Object:		
//	02-04-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	TEXT_PARSER::GetTextBoundingRect(VIEWPORT *	pRenderingViewport,
										 float &	rLeft,
										 float &	rRight,
										 float &	rTop,
										 float &	rBottom,
										 bool		bRelative,
										 bool		bForceProcess)
{
	QDT_NOT_IMPLEMENTED();
/*
	if ( bForceProcess == true )
	{
		Process( pRenderingViewport );
	}

	if ( _vOrderedText.IsEmpty() == true )
	{
		return ( false );
	}

	GetTextFullRect( rLeft, rRight, rTop, rBottom );

	rLeft	= MATH::Max( rLeft,   _pElementText->GetAbsolutePosLeft(   pRenderingViewport ) );
	rRight	= MATH::Min( rRight,  _pElementText->GetAbsolutePosRight(  pRenderingViewport ) );
	rTop	= MATH::Max( rTop,    _pElementText->GetAbsolutePosTop(    pRenderingViewport ) );
	rBottom	= MATH::Min( rBottom, _pElementText->GetAbsolutePosBottom( pRenderingViewport ) );

	if ( bRelative == true )
	{
		rLeft	*= 100.0f / RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX();
		rRight	*= 100.0f / RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX();
		rTop	*= 100.0f / RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeY();
		rBottom	*= 100.0f / RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeY();
	}
*/
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetTextFullRect
//	Object:		
//	04-11-25:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::GetTextFullRect(float &	rAbsLeft,
									 float &	rAbsRight,
									 float &	rAbsTop,
									 float &	rAbsBottom)
{
	IHM_TEXT_LINE *	pCurrentLine;
	IHM_WORD *		pCurrentWord;
	int				nNbrLines, nLineIndex, nNbrWords, nWordIndex;
	DISPLAY_FONT *	pFont;
	float			rLeftTmp, rRightTmp, rTopTmp, rBottomTmp;
	bool			bFirst;

	pFont		= _pElementText->GetFont();
	bFirst		= true;
	nNbrLines	= _vOrderedText.GetSize();
	for ( nLineIndex = 0; nLineIndex < nNbrLines; ++nLineIndex )
	{
		pCurrentLine	= _vOrderedText[ nLineIndex ];
		nNbrWords		= pCurrentLine->GetNbrWords();

		for ( nWordIndex = 0; nWordIndex < nNbrWords; ++nWordIndex )
		{
			pCurrentWord = pCurrentLine->GetWord( nWordIndex );

			if ( bFirst == true )
			{
				GetWordBoundingRect( pCurrentWord, pFont, rAbsLeft, rAbsRight, rAbsTop, rAbsBottom );
				bFirst	= false;
			}
			else
			{
				GetWordBoundingRect( pCurrentWord, pFont, rLeftTmp, rRightTmp, rTopTmp, rBottomTmp );

				rAbsLeft	= MATH::Min( rAbsLeft,   rLeftTmp );
				rAbsRight	= MATH::Max( rAbsRight,  rRightTmp );
				rAbsTop		= MATH::Min( rAbsTop,    rTopTmp );
				rAbsBottom	= MATH::Max( rAbsBottom, rBottomTmp );
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetWordBoundingRect
//	Object:		
//	04-11-23:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::GetWordBoundingRect(IHM_WORD *		pWord,
										 DISPLAY_FONT *	pFont,
										 float &		rLeft,
										 float &		rRight,
										 float &		rTop,
										 float &		rBottom)
{
	QDT_NOT_IMPLEMENTED();
/*
	QDT_ASSERT( ( pWord != NULL ) && ( pFont != NULL ) );

	float	rHeight;

	rHeight	= pWord->_StringAttributes._rSizePercent * pFont->GetHeight() / 100.0f;
	rHeight = rHeight * RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeY() / (float)BASE_VRES;

	rBottom	= pWord->_rPosY;
	rTop	= rBottom - rHeight;

	rLeft	= pWord->_rPosX;
	rRight	= rLeft + pWord->_rPixelSize;*/
}

//-----------------------------------------------------------------------------
//	Name:		SetScrollSpeed
//	Object:		
//	02-04-11:	GGO - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::SetScrollSpeed(const float	rSpeed)
{
	_rScrollSpeed = rSpeed;
}

//-----------------------------------------------------------------------------
//	Name:		IHM_TEXT_LINE
//	Object:		
//	04-05-15:	RMA - Created
//-----------------------------------------------------------------------------
IHM_TEXT_LINE::IHM_TEXT_LINE()
{
	_nNbrWordUsed = 0;
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_TEXT_LINE
//	Object:		
//	04-03-11:	SBE - Created
//-----------------------------------------------------------------------------
IHM_TEXT_LINE::~IHM_TEXT_LINE()
{
	unsigned int i;
	for (i = 0 ; i < _nNbrWordUsed ; ++i)
	{	
		TEXT_PARSER::UnallocateWord(_tpLine[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushWord
//	Object:		
//	04-05-15:	RMA - Created
//-----------------------------------------------------------------------------
void IHM_TEXT_LINE::PushWord(IHM_WORD * pWord)
{
	if (_nNbrWordUsed < NBR_MAX_WORD_IN_A_LINE-1)
	{
		_tpLine[_nNbrWordUsed] = pWord;
		++_nNbrWordUsed;
	}
	else
	{
		QDT_Warning(QDT_COMMENT("Max word in a line exceeded!!!"));
		QDT_Warning(QDT_COMMENT("You can increase this value in TextParser.h line 28"));
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushWord
//	Object:		
//	04-05-15:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	IHM_TEXT_LINE::GetNbrWords() const				
{ 
	return (_nNbrWordUsed); 
}
	
//-----------------------------------------------------------------------------
//	Name:		PushWord
//	Object:		
//	04-05-15:	RMA - Created
//-----------------------------------------------------------------------------
IHM_WORD*		IHM_TEXT_LINE::GetWord(unsigned int i) const	
{ 
	QDT_ASSERT(i < _nNbrWordUsed);
	return (_tpLine[i]); 
}

//-----------------------------------------------------------------------------
//	Name:		AddFindAndReplace
//	Object:		
//	05-03-21:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::AddFindAndReplace(const QDT_STRING & Find, 
									   const QDT_STRING & Replace)
{
	_vFind.PushTail(Find);
	_vReplace.PushTail(Replace);
}

//-----------------------------------------------------------------------------
//	Name:		ClearFindAndReplace
//	Object:		
//	05-03-21:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXT_PARSER::ClearFindAndReplace()
{
	_vFind.Clear();
	_vReplace.Clear();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
