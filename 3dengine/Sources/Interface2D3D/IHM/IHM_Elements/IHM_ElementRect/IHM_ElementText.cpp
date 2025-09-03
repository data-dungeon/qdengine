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
//	CLASS:	IHM_ELEMENT_TEXT
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementText)
#include	INCL_3DENGINE(Interface2D3D/DisplayFont/DisplayFont)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(CommunicatorType/CommunicatorContainerDescriptor)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeString)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)

#define	__QDT_JAPANESE_LINE_CUT__

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void *							IHM_ELEMENT_TEXT::_pVTable			= NULL;
QDT_VECTOR<QDT_STRING_UNICODE>	IHM_ELEMENT_TEXT::_vFind;
QDT_VECTOR<QDT_STRING_UNICODE>	IHM_ELEMENT_TEXT::_vReplace;
const int						IHM_ELEMENT_TEXT::BASE_HRES			= 640;
const int						IHM_ELEMENT_TEXT::BASE_VRES			= 480;
const float						IHM_ELEMENT_TEXT::SAFE_MARGIN		= 0.2f; 
#ifndef _MASTER					
bool							IHM_ELEMENT_TEXT::_bExportDisplay	= false;
const COLOR						IHM_ELEMENT_TEXT::FLOOR_COLOR( 255, 255, 0, 0 );
const COLOR						IHM_ELEMENT_TEXT::INTERLINE_COLOR( 255, 0, 255, 0 );
#endif

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_TEXT constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TEXT::IHM_ELEMENT_TEXT()
:
IHM_ELEMENT_RECT	( GetGenericCommunicatorDescriptor() ),
_pDisplayedLetters	( NULL ),
_rScrollPercent		( 0 ),
_bCheckScroll		( false ),
_DefaultColor		( 255, 255, 255, 255 ),
_rForcedSize		( -1 )
{
	Show();
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_TEXT destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TEXT::~IHM_ELEMENT_TEXT()
{
	if ( _pDisplayedLetters != NULL )
	{
		delete[] _pDisplayedLetters;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Open(IHM_INTERFACE *		pInterfance,
							   IHM_PAGE *			pPage,
							   IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...	
}
	
//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Close(IHM_INTERFACE *		pInterfance,
								IHM_PAGE *			pPage,
								IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-10-01:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::DisplayElement(VIEWPORT *	pViewport)
{
//	QDT_Warning("IHM_ELEMENT_TEXT::DisplayElement() not implemented");
	//QDT_NOT_IMPLEMENTED();

	static	const float	INV_SIZE_X		= 1.0f / DISPLAY_FONT::FONT_TEXTURE_SIZE_X;
	static	const float	INV_SIZE_Y		= 1.0f / DISPLAY_FONT::FONT_TEXTURE_SIZE_Y;
//	RENDER_CONTEXT *	pRenderContext	= RENDER_MANAGER::Instance()->GetRenderContext();
	bool				bWired			= false;
//	bool				bOldAlphaBlend	= pRenderContext->IsAlphaBlendEnabled();
	int					nLetterIndex, nLettersCount, nMaterialIndex, nNbrMaterials, nDummyInt;
	COM_INSTANCE		MaterialCI;
	float				rUVSrcLeft, rUVSrcRight, rUVSrcTop, rUVSrcBottom;
	float				rScreenDestLeft, rScreenDestRight, rScreenDestTop, rScreenDestBottom;
	float				rScreenWidth, rScreenHeight, rUVWidth, rUVHeight;
	short				nLeft, nRight, nTop, nBottom, nDummyShort;
	char				nCurLetterMaterialIndex;

	if ( _FontCI.HasHandle() == false )
	{
		return;
	}

/*	if ( _bWired == false )
	{
		bWired = pRenderContext->IsWireFrameEnabled();
		pRenderContext->EnableWireFrame( false );
	} */

	/*SetRelativePosLeft(0.5f);		
	SetRelativePosRight(100.0f);	
	SetRelativePosTop(0.0f);		
	SetRelativePosBottom(100.0f);*/

	RENDER_CONTEXT::EnableDepthTest(false);

	DISPLAY_MANAGER::Instance()->EnableSkinning(false);
	DISPLAY_MANAGER::Instance()->SetWorldMatrix(MATRIX::IDENTITY);
	DISPLAY_MANAGER::Instance()->SetViewMatrix(MATRIX::IDENTITY);
	DISPLAY_MANAGER::Instance()->SetProjMatrix(MATRIX::IDENTITY);

	// Display the text. 
	ProcessText( nLettersCount, nDummyInt, pViewport );
	nNbrMaterials = GetFont()->GetMaterialsCount();
	for ( nMaterialIndex = 0; nMaterialIndex < nNbrMaterials; ++nMaterialIndex )
	{
		MaterialCI = GetFont()->GetMaterialCI( nMaterialIndex );
		SHADER_ABC* pMaterial = (SHADER_ABC *)MaterialCI.GetCommunicator();

		QDT_ASSERT(pMaterial->GetNbrPasses() == 1);

		pMaterial->SetPass(0, VECTOR(0,0,0));

		for ( nLetterIndex = 0; nLetterIndex < nLettersCount; ++nLetterIndex )
		{
			// This is an insecable space.
			if ( _pDisplayedLetters[ nLetterIndex ]._cLetter != 0x00A7 )
			{
				GetFont()->GetLetterDesc( _pDisplayedLetters[ nLetterIndex ]._cLetter, nCurLetterMaterialIndex, nLeft, nRight, nTop, nBottom, nDummyShort );

				if ( nMaterialIndex == nCurLetterMaterialIndex )
				{
					// The letter overlaps the ihm's rect (we don't process letters outside).
					if ( ( _pDisplayedLetters[ nLetterIndex ]._rScreenLeft   <= GetAbsolutePosRight( pViewport )  ) && 
						( _pDisplayedLetters[ nLetterIndex ]._rScreenRight  >= GetAbsolutePosLeft( pViewport )   ) && 
						( _pDisplayedLetters[ nLetterIndex ]._rScreenTop    <= GetAbsolutePosBottom( pViewport ) ) && 
						( _pDisplayedLetters[ nLetterIndex ]._rScreenBottom >= GetAbsolutePosTop( pViewport )    ) )
					{
						rScreenDestLeft		= _pDisplayedLetters[ nLetterIndex ]._rScreenLeft;
						rScreenDestRight	= _pDisplayedLetters[ nLetterIndex ]._rScreenRight;
						rScreenDestTop		= _pDisplayedLetters[ nLetterIndex ]._rScreenTop;
						rScreenDestBottom	= _pDisplayedLetters[ nLetterIndex ]._rScreenBottom;
						rScreenWidth		= rScreenDestRight - rScreenDestLeft;
						rScreenHeight		= rScreenDestBottom - rScreenDestTop;

						rUVSrcLeft			= ( nLeft   - 0.5f ) * INV_SIZE_X;
						rUVSrcRight			= ( nRight  + 1.5f ) * INV_SIZE_X;
						rUVSrcTop			= ( nTop    - 0.5f ) * INV_SIZE_Y;
						rUVSrcBottom		= ( nBottom + 1.5f ) * INV_SIZE_Y;
						rUVWidth			= rUVSrcRight - rUVSrcLeft;
						rUVHeight			= rUVSrcBottom - rUVSrcTop;

					/*	if ( rScreenDestLeft < GetAbsolutePosLeft( pViewport ) )
						{
							rUVSrcLeft			+= ( GetAbsolutePosLeft( pViewport ) - rScreenDestLeft ) * rUVWidth / rScreenWidth;
							rScreenDestLeft		= GetAbsolutePosLeft( pViewport );
						}
						if ( rScreenDestRight > GetAbsolutePosRight( pViewport ) )
						{
							rUVSrcRight			-= ( rScreenDestRight - GetAbsolutePosRight( pViewport ) ) * rUVWidth / rScreenWidth;
							rScreenDestRight	= GetAbsolutePosRight( pViewport );
						}
						if ( rScreenDestTop < GetAbsolutePosTop( pViewport ) )
						{
							rUVSrcTop			+= ( GetAbsolutePosTop( pViewport ) - rScreenDestTop ) * rUVHeight / rScreenHeight;
							rScreenDestTop		= GetAbsolutePosTop( pViewport );
						}
						if ( rScreenDestBottom > GetAbsolutePosBottom( pViewport ) )
						{
							rUVSrcBottom		-= ( rScreenDestBottom - GetAbsolutePosBottom( pViewport ) ) * rUVHeight / rScreenHeight;
							rScreenDestBottom	= GetAbsolutePosBottom( pViewport );
						}*/

						rScreenDestLeft /= INTERFACE_MANAGER::Instance()->GetWindowWidth();
						rScreenDestRight /= INTERFACE_MANAGER::Instance()->GetWindowWidth();
						rScreenDestTop /= -INTERFACE_MANAGER::Instance()->GetWindowHeight();
						rScreenDestBottom /= -INTERFACE_MANAGER::Instance()->GetWindowHeight();

						rScreenDestLeft*=2;
						rScreenDestRight*=2;
						rScreenDestTop*=2;
						rScreenDestBottom*=2;

						rScreenDestLeft-=1;
						rScreenDestRight-=1;
						rScreenDestTop+=1;
						rScreenDestBottom+=1;

						POINT2D uvList[4];
						POINT2D pointList[4];
						COLOR color[4];

						color[0] = _pDisplayedLetters[ nLetterIndex ]._Color;
						color[1] = _pDisplayedLetters[ nLetterIndex ]._Color;
						color[2] = _pDisplayedLetters[ nLetterIndex ]._Color;
						color[3] = _pDisplayedLetters[ nLetterIndex ]._Color;

						pointList[0].SetXY(rScreenDestLeft, rScreenDestTop);
						pointList[2].SetXY(rScreenDestLeft, rScreenDestBottom);
						pointList[1].SetXY(rScreenDestRight, rScreenDestTop);
						pointList[3].SetXY(rScreenDestRight, rScreenDestBottom);

						uvList[0].SetXY(rUVSrcLeft, rUVSrcTop);
						uvList[2].SetXY(rUVSrcLeft, rUVSrcBottom);
						uvList[1].SetXY(rUVSrcRight, rUVSrcTop);
						uvList[3].SetXY(rUVSrcRight, rUVSrcBottom);

						DISPLAY_MANAGER::Instance()->PushQuad(	pointList, 
																uvList, 
																color );
					}
				}
			}
		}

		DISPLAY_MANAGER::Instance()->FlushQuads();

		//pRenderContext->DisplayText();
	}
/*
	// Restore the render context.
	if ( _bWired == false )
	{
		pRenderContext->EnableWireFrame( bWired );
	}*/

	RENDER_CONTEXT::EnableDepthTest(true);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Display(DISPLAY_PARAMETERS &	DisplayParameters,
								  float					rSizeX,
								  float					rSizeY)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetText
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
const QDT_STRING_UNICODE &	IHM_ELEMENT_TEXT::GetText() const						
{
	return ( _sText );
}

//-----------------------------------------------------------------------------
//	Name:		SetText
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetText(const QDT_STRING_UNICODE & sText)
{
	unsigned short *		pBuffer;
	const unsigned short *	pInIt;
	unsigned short *		pOutIt;
	unsigned short			UnicodeChar;

	// Find hard coded unicode characters.
	pBuffer = new unsigned short[ sText.GetSize() + 1 ];
	pInIt	= sText.GetBuffer();
	pOutIt	= pBuffer;
	while ( *pInIt != 0 )
	{
		if ( ( *pInIt == '{' ) && ( *(pInIt + 1 ) == 'U' ) )
		{
			pInIt += 2;

			if ( *pInIt != ':' )
			{
				pInIt = SeekEndOfBalise( pInIt );
			}
			else
			{
				pInIt = ReadBaliseUnicodeChar( pInIt, UnicodeChar );

				if ( UnicodeChar != 0 )
				{
					*pOutIt = UnicodeChar;
					++pOutIt;
				}
			}
		}
		else
		{
			*pOutIt = *pInIt;
			++pInIt;
			++pOutIt;
		}
	}
	*pOutIt = 0;

	// Set the text.
	_sText = pBuffer;
	delete[] pBuffer;

	// Apply find and replace.
	ApplyFindAndReplace( _sText );

	// Allocate display letters.
	if ( _pDisplayedLetters != NULL )
	{
		delete[] _pDisplayedLetters;
	}
	_pDisplayedLetters = new DISPLAY_LETTER[ _sText.GetSize() ];
}

//-----------------------------------------------------------------------------
//	Name:		GetFontIndex
//	Object:		
//	02-09-11:	IAM - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	IHM_ELEMENT_TEXT::GetFontCI()
{
	return ( _FontCI );
}

//-----------------------------------------------------------------------------
//	Name:		GetFont
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
DISPLAY_FONT *	IHM_ELEMENT_TEXT::GetFont() const
{
	QDT_ASSERT( _FontCI.HasHandle() );

	return ( static_cast<DISPLAY_FONT *>( _FontCI.GetCommunicator() ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetFont
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetFontCI(const COM_INSTANCE & FontCI)			
{
	_FontCI = FontCI;
}

//-----------------------------------------------------------------------------
//	Name:		Scroll
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Scroll(const float	rScrollPercent)
{
	_bCheckScroll		= true;
	_rOldScrollPercent	= _rScrollPercent;
	_rScrollPercent		= rScrollPercent;
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticalAlignement
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetVerticalAlignement(const VALIGN	VerticalAlign)
{
	switch ( VerticalAlign )
	{
	case VALIGN_CENTERED:
		_rScrollPercent = 50.0f;
		break;

	case VALIGN_TOP:
		_rScrollPercent = 0.0f;
		break;

	case VALIGN_BOTTOM:
		_rScrollPercent = 100.0f;
		break;

	default:
		QDT_FAIL();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrLines
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_TEXT::GetNbrLines()
{
	int	nDummy, nNbrLines;

	ProcessText( nDummy, nNbrLines, NULL );

	return ( nNbrLines );
}

//-----------------------------------------------------------------------------
//	Name:		GetTextBoundingRect
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT::GetTextBoundingRect(float &	rLeft,
											  float &	rRight,
											  float &	rTop,
											  float &	rBottom,
											  bool		bRelative)
{
	int		nDummy, nLetterIndex, nNbrLetters;
	float	rRatioX, rRatioY;

	ProcessText( nNbrLetters, nDummy, NULL );

	if ( nNbrLetters <= 0 )
	{
		return ( false );
	}

	// Look for the bouning coordinates.
	rLeft	= _pDisplayedLetters[ 0 ]._rScreenLeft;
	rRight	= _pDisplayedLetters[ 0 ]._rScreenRight;
	rTop	= _pDisplayedLetters[ 0 ]._rScreenTop;
	rBottom	= _pDisplayedLetters[ 0 ]._rScreenBottom;

	for ( nLetterIndex = 1; nLetterIndex < nNbrLetters; ++nLetterIndex )
	{
		rLeft	= MATH::Min( rLeft,   _pDisplayedLetters[ nLetterIndex ]._rScreenLeft );
		rRight	= MATH::Max( rRight,  _pDisplayedLetters[ nLetterIndex ]._rScreenRight );
		rTop	= MATH::Min( rTop,    _pDisplayedLetters[ nLetterIndex ]._rScreenTop );
		rBottom	= MATH::Max( rBottom, _pDisplayedLetters[ nLetterIndex ]._rScreenBottom );
	}

	// Convert from screen coordinates to percentages.
	if ( bRelative == true )
	{
		rRatioX	= 100.0f / RENDER_CONTEXT::GetScreenWidth();
		rRatioY	= 100.0f / RENDER_CONTEXT::GetScreenHeight();

		rLeft	*= rRatioX;
		rRight	*= rRatioX;
		rTop	*= rRatioY;
		rBottom	*= rRatioY;

		rLeft	-= SAFE_MARGIN;
		rRight	+= SAFE_MARGIN;
		rTop	-= SAFE_MARGIN;
		rBottom	+= SAFE_MARGIN;
	}
	else
	{
		rRatioX	= RENDER_CONTEXT::GetScreenWidth() / 100.0f;
		rRatioY	= RENDER_CONTEXT::GetScreenHeight() / 100.0f;

		rLeft	-= rRatioX * SAFE_MARGIN;
		rRight	+= rRatioX * SAFE_MARGIN;
		rTop	-= rRatioY * SAFE_MARGIN;
		rBottom	+= rRatioY * SAFE_MARGIN;
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ProcessText
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ProcessText(int &			nDisplayLettersCount,
									  int &			nLinesCount,
									  VIEWPORT *	pViewport)
{
	int						nLineFirstLetterIndex, nHorzAlign, nFirstLineLetterIndex;
	const unsigned short *	pLetterIt;
	float					rNextLineTop, rTextSizePercent;
	COLOR					CurColor( _DefaultColor );
	bool					bLineReturn, bNewLine, bFirstLineReturn;

	// Init.
	bLineReturn				= false;
	rNextLineTop			= GetSafePosTop( pViewport );
	nLineFirstLetterIndex	= 0;
	nDisplayLettersCount	= 0;
	nLinesCount				= 0;
	nFirstLineLetterIndex	= 0;
	pLetterIt				= _sText.GetBuffer();
	rTextSizePercent		= _rForcedSize > 0 ? _rForcedSize : 100;
	bNewLine				= true;
	bFirstLineReturn		= true;
	nHorzAlign				= HALIGN_LEFT;

	// Check for everything to be OK.
	if ( ( _FontCI.HasHandle() == false ) || ( _sText.IsEmpty() == true ) )
	{
		return;
	}
	if ( ( GetSafePosLeft( pViewport ) >= GetSafePosRight( pViewport )  ) ||
		 ( GetSafePosTop( pViewport )  >= GetSafePosBottom( pViewport ) ) )
	{
		return;
	}

	// Pase the text.
	while ( *pLetterIt != 0 )
	{
		// Read balise.
		while ( ( *pLetterIt == '{' ) || ( *pLetterIt == 0xFF5B ) )
		{
			pLetterIt = ReadBalise( pLetterIt, rTextSizePercent, bLineReturn, nHorzAlign, CurColor );

			// Process line return.
			if ( bLineReturn == true )
			{
				if ( ( nDisplayLettersCount > 0 ) || ( bFirstLineReturn == false ) )
				{
					LineReturn( rNextLineTop, nLineFirstLetterIndex, nDisplayLettersCount, nLinesCount, nHorzAlign, rTextSizePercent, pViewport );

					bNewLine = true;
				}

				bLineReturn			= false;
				bFirstLineReturn	= false;
			}
		}

		// If we haven't reached the end of the text while reading a balise.
		if ( *pLetterIt != 0 )
		{
			if ( ProcessLetter( *pLetterIt, bNewLine, nDisplayLettersCount, rTextSizePercent, CurColor, pViewport ) == true )
			{
				++nDisplayLettersCount;
				bNewLine = false;

				// Perform line cutting.
				LineCut( rNextLineTop, nLineFirstLetterIndex, nDisplayLettersCount, nLinesCount, bNewLine, nHorzAlign, pViewport );
			}

			++pLetterIt;
		}
	}

	// Process last line.
	if ( nHorzAlign == HALIGN_JUSTIFIED )
	{
		nHorzAlign = HALIGN_LEFT;
	}
	LineReturn( rNextLineTop, nLineFirstLetterIndex, nDisplayLettersCount, nLinesCount, nHorzAlign, rTextSizePercent, pViewport );

	// And finally, perform vertical align.
	VerticalAlign( rNextLineTop - GetSafePosTop(  pViewport ), nDisplayLettersCount, pViewport );
}

//-----------------------------------------------------------------------------
//	Name:		ReadBalise
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
const unsigned short *	IHM_ELEMENT_TEXT::ReadBalise(const unsigned short *	pLetterIt,
													 float &				rTextSizePercent,
													 bool &					bLineReturn,
													 int &					nHorzAlign,
													 COLOR &				Color) const
{
	++pLetterIt;

	switch ( *pLetterIt )
	{
	case 'S':		// ANSI
	case 0xFF33:	// Unicode
		++pLetterIt;
		if ( ( *pLetterIt != ':' ) || ( *pLetterIt == 0xFF1A ) )
		{
			return ( SeekEndOfBalise( pLetterIt ) );
		}
		else
		{
			return ( ReadBaliseSize( pLetterIt, rTextSizePercent ) );
		}

	case 'P':		// ANSI
	case 0xFF30:	// Unicode
		bLineReturn = true;
		++pLetterIt;
		switch ( *pLetterIt )
		{
		case '}':		// ANSI
		case 0xFF5D:	// Unicode
			return ( ++pLetterIt );

		case ':':		// ANSI
		case 0xFF1A:	// Unicode
			return ( ReadBaliseHorzAlign( pLetterIt, nHorzAlign ) );

		default:
			return ( SeekEndOfBalise( pLetterIt ) );
		}

	case 'C':		// ANSI
	case 0xFF23:	// Unicode
		++pLetterIt;
		if ( ( *pLetterIt != ':' ) || ( *pLetterIt == 0xFF1A ) )
		{
			return ( SeekEndOfBalise( pLetterIt ) );
		}
		else
		{
			return ( ReadBaliseColor( pLetterIt, Color ) );
		}

	default:
		return ( SeekEndOfBalise( pLetterIt ) );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReadWrontBalise
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
const unsigned short *	IHM_ELEMENT_TEXT::SeekEndOfBalise(const unsigned short *	pLetterIt)
{
	while ( ( *pLetterIt != '}' ) && ( *pLetterIt != 0xFF5D ) )
	{
		if ( *pLetterIt == 0 )
		{
			return ( pLetterIt );
		}

		++pLetterIt;
	}

	QDT_ASSERT( ( *pLetterIt == '}' ) || ( *pLetterIt == 0xFF5D ) );
	++pLetterIt;

	return ( pLetterIt );
}

//-----------------------------------------------------------------------------
//	Name:		ReadBaliseSize
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
const unsigned short *	IHM_ELEMENT_TEXT::ReadBaliseSize(const unsigned short *	pLetterIt,
														 float &				rTextSizePercent) const
{
	int		nNumber;
	float	rOldTextSizePercent	= rTextSizePercent;

	if ( _rForcedSize > 0 )
	{
		return ( SeekEndOfBalise( pLetterIt ) );
	}

	QDT_ASSERT( ( *pLetterIt == ':' ) || ( *pLetterIt == 0xFF1A ) );
	++pLetterIt;

	rTextSizePercent = 0;
	while ( ( *pLetterIt != '}' ) && ( *pLetterIt != 0xFF5D ) )
	{
		if ( pLetterIt == NULL )
		{
			return ( pLetterIt );
		}

		if ( ReadNumber( *pLetterIt, nNumber ) == true )
		{
			rTextSizePercent *= 10;
			rTextSizePercent += nNumber;
		}
		else
		{
			rTextSizePercent = rOldTextSizePercent;
			return ( SeekEndOfBalise( pLetterIt ) );
		}
			
		++pLetterIt;
	}

	QDT_ASSERT( ( *pLetterIt == '}' ) || ( *pLetterIt == 0xFF5D ) );
	++pLetterIt;

	return ( pLetterIt );
}

//-----------------------------------------------------------------------------
//	Name:		ReadBaliseHorzAlign
//	Object:		
//	05-08-24:	GGO - Created
//-----------------------------------------------------------------------------
const unsigned short *	IHM_ELEMENT_TEXT::ReadBaliseHorzAlign(const unsigned short *	pLetterIt,
															  int &						nHorzAlign)
{
	++pLetterIt;

	switch ( *pLetterIt )
	{
	case 'L':		// ANSI
	case 0xFF2C:	// Unicode
		nHorzAlign = HALIGN_LEFT;
		break;

	case 'C':		// ANSI
	case 0xFF23:	// Unicode
		nHorzAlign = HALIGN_CENTERED;
		break;

	case 'R':		// ANSI
	case 0xFF32:	// Unicode
		nHorzAlign = HALIGN_RIGHT;
		break;

	case 'J':		// ANSI
	case 0xFF2A:	// Unicode
		nHorzAlign = HALIGN_JUSTIFIED;
		break;
	}

	return ( SeekEndOfBalise( pLetterIt ) );
}

//-----------------------------------------------------------------------------
//	Name:		ReadBaliseColor
//	Object:		
//	05-09-20:	GGO - Created
//-----------------------------------------------------------------------------
const unsigned short *	IHM_ELEMENT_TEXT::ReadBaliseColor(const unsigned short *	pLetterIt,
														  COLOR &					Color)
{
	int		nRed, nGreen, nBlue, nAlpha, nIndex, nNumber;
	COLOR	OldColor( Color );

	++pLetterIt;

	// Red.
	nRed = 0;
	for ( nIndex = 0; nIndex < 3; ++nIndex )
	{
		if ( ReadNumber( *pLetterIt, nNumber ) == false )
		{
			Color = OldColor;
			return ( SeekEndOfBalise( pLetterIt ) );
		}
		nRed *= 10;
		nRed += nNumber;
		++pLetterIt;
	}

	// Green.
	nGreen = 0;
	if ( ( *pLetterIt++ != ':' ) && ( *pLetterIt != 0xFF1A ) )
	{
		Color = OldColor;
		return ( SeekEndOfBalise( pLetterIt ) );
	}
	for ( nIndex = 0; nIndex < 3; ++nIndex )
	{
		if ( ReadNumber( *pLetterIt, nNumber ) == false )
		{
			Color = OldColor;
			return ( SeekEndOfBalise( pLetterIt ) );
		}
		nGreen *= 10;
		nGreen += nNumber;
		++pLetterIt;
	}

	// Blue.
	nBlue = 0;
	if ( ( *pLetterIt++ != ':' ) && ( *pLetterIt != 0xFF1A ) )
	{
		Color = OldColor;
		return ( SeekEndOfBalise( pLetterIt ) );
	}
	for ( nIndex = 0; nIndex < 3; ++nIndex )
	{
		if ( ReadNumber( *pLetterIt, nNumber ) == false )
		{
			Color = OldColor;
			return ( SeekEndOfBalise( pLetterIt ) );
		}
		nBlue *= 10;
		nBlue += nNumber;
		++pLetterIt;
	}

	// Alpha.
	if ( *pLetterIt == '}' )
	{
		nAlpha = 255;
	}
	else
	if ( ( *pLetterIt++ == ':' ) || ( *pLetterIt == 0xFF1A ) )
	{
		pLetterIt++;
		nAlpha = 0;
		for ( nIndex = 0; nIndex < 3; ++nIndex )
		{
			if ( ReadNumber( *pLetterIt, nNumber ) == false )
			{
				Color = OldColor;
				return ( SeekEndOfBalise( pLetterIt ) );
			}
			nAlpha *= 10;
			nAlpha += nIndex;
			++pLetterIt;
		}
	}
	else
	{
		Color = OldColor;
		return ( SeekEndOfBalise( pLetterIt ) );
	}

	// Check range.
	if ( ( nRed > 255 ) || ( nGreen > 255 ) || ( nBlue > 255 ) || ( nAlpha > 255 ) )
	{
		Color = OldColor;
		return ( SeekEndOfBalise( pLetterIt ) );
	}

	Color.SetARGB( nAlpha, nRed, nGreen, nBlue );

	return ( SeekEndOfBalise( pLetterIt ) );
}

//-----------------------------------------------------------------------------
//	Name:		ReadBaliseUnicodeChar
//	Object:		
//	05-09-20:	GGO - Created
//-----------------------------------------------------------------------------
const unsigned short *	IHM_ELEMENT_TEXT::ReadBaliseUnicodeChar(const unsigned short *	pLetterIt,
																unsigned short &		UnicodeChar)
{
	char	nIndex;

	++pLetterIt;

	UnicodeChar = 0;

	for ( nIndex = 0; nIndex < 4; ++nIndex )
	{
		UnicodeChar <<= 4;

		switch ( *pLetterIt++ )
		{
		case '0':
			UnicodeChar += 0x0;
			break;
		case '1':
			UnicodeChar += 0x1;
			break;
		case '2':
			UnicodeChar += 0x2;
			break;
		case '3':
			UnicodeChar += 0x3;
			break;
		case '4':
			UnicodeChar += 0x4;
			break;
		case '5':
			UnicodeChar += 0x5;
			break;
		case '6':
			UnicodeChar += 0x6;
			break;
		case '7':
			UnicodeChar += 0x7;
			break;
		case '8':
			UnicodeChar += 0x8;
			break;
		case '9':
			UnicodeChar += 0x9;
			break;
		case 'A':
		case 'a':
			UnicodeChar += 0xA;
			break;
		case 'B':
		case 'b':
			UnicodeChar += 0xB;
			break;
		case 'C':
		case 'c':
			UnicodeChar += 0xC;
			break;
		case 'D':
		case 'd':
			UnicodeChar += 0xD;
			break;
		case 'E':
		case 'e':
			UnicodeChar += 0xE;
			break;
		case 'F':
		case 'f':
			UnicodeChar += 0xF;
			break;
		default:
			UnicodeChar = 0;
			return ( SeekEndOfBalise( pLetterIt ) );
		}
	}

	if ( *pLetterIt == '}' )
	{
		return ( pLetterIt + 1 );
	}
	else
	{
		UnicodeChar = 0;
		return ( SeekEndOfBalise( pLetterIt ) );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReadNumber
//	Object:		
//	05-10-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT::ReadNumber(unsigned short	nLetter,
									 int &			nNumber)
{
	// ANSI
	if ( ( nLetter >= '0' ) && ( nLetter <= '9' ) )
	{
		nNumber = nLetter - '0';
		return ( true );
	}
	else
	// Unicode
	if ( ( nLetter >= 0xFF10 ) && ( nLetter <= 0xFF19 ) )
	{
		nNumber = nLetter - 0xFF10;
		return ( true );
	}
	else
	{
		return ( false );
	}		
}

//-----------------------------------------------------------------------------
//	Name:		IsSpace
//	Object:		
//	05-10-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT::IsSpace(unsigned short	nChar)
{
	return ( ( nChar == ' ' ) || ( nChar == 0x3000 ) );
}

//-----------------------------------------------------------------------------
//	Name:		ProcessLetter
//	Object:		
//	05-08-22:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT::ProcessLetter(short		cLetter,
										bool		bNewLine,
										int			nDisplayLettersCount,
										float		rTextSizePercent,
										COLOR		Color,
										VIEWPORT *	pViewport)
{
	QDT_ASSERT( rTextSizePercent >= 0 );
	QDT_ASSERT( nDisplayLettersCount >= 0 );
	QDT_ASSERT( _FontCI.HasHandle() );

	short	nLetterPixelWidth, nUVsLeft, nUVsRight, nDummy;
	float	rLetterLeft, rLetterRight;

	nLetterPixelWidth	= -1;

	if ( ( IsSpace( cLetter ) == true ) || ( cLetter == 0x00A7 ) ) // Insecable space.
	{
		nLetterPixelWidth = GetFont()->GetInterWordSpace();
	}
	else
	{
		GetFont()->GetLetterDesc( cLetter,
								  _pDisplayedLetters[ nDisplayLettersCount ]._nMaterialIndex, 
								  nUVsLeft, 
								  nUVsRight, 
								  nDummy, 
								  nDummy, 
								  nDummy );

		if ( _pDisplayedLetters[ nDisplayLettersCount ]._nMaterialIndex >= 0 )
		{
			nLetterPixelWidth	= nUVsRight - nUVsLeft + 1;
			QDT_ASSERT( nLetterPixelWidth > 0 );
		}
		else
		{
			return ( false );
		}
	}

	if ( ( nDisplayLettersCount == 0 ) || ( bNewLine == true ) )
	{
		rLetterLeft = GetSafePosLeft( pViewport );
	}
	else	
	{
		rLetterLeft = MATH::Max( GetSafePosLeft( pViewport ), _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenRight + ScaleX( GetFont()->GetInterLetterSpace(), rTextSizePercent, pViewport ) );
	}

	rLetterRight = rLetterLeft + ScaleX( nLetterPixelWidth, rTextSizePercent, pViewport );

	_pDisplayedLetters[ nDisplayLettersCount ]._cLetter				= cLetter;
	_pDisplayedLetters[ nDisplayLettersCount ]._rTextSizePercent	= rTextSizePercent;
	_pDisplayedLetters[ nDisplayLettersCount ]._Color				= Color;
	_pDisplayedLetters[ nDisplayLettersCount ]._rScreenLeft			= rLetterLeft;
	_pDisplayedLetters[ nDisplayLettersCount ]._rScreenRight		= rLetterRight;

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ScaleX
//	Object:		
//	05-08-22:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::ScaleX(float		rValue,
								 float		rTextSizePercent,
								 VIEWPORT * pViewport)
{
	rValue = rValue * 0.01 * rTextSizePercent * RENDER_CONTEXT::GetScreenWidth() / BASE_HRES;

	if ( pViewport != NULL )
	{
		rValue = rValue * 0.01 * pViewport->GetWidth();
	}

	return ( rValue );
}

//-----------------------------------------------------------------------------
//	Name:		ScaleY
//	Object:		
//	05-08-22:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::ScaleY(float		rValue,
								 float		rTextSizePercent,
								 VIEWPORT *	pViewport)
{
	rValue = rValue * 0.01 * rTextSizePercent * RENDER_CONTEXT::GetScreenHeight() / BASE_VRES;

	if ( pViewport != NULL )
	{
		rValue = rValue * 0.01 * pViewport->GetHeight();
	}

	return ( rValue );
}

//-----------------------------------------------------------------------------
//	Name:		MoveLastWordToNextLine
//	Object:		
//	05-08-24:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_TEXT::MoveLastWordToNextLine(int		nDisplayLettersCount,
											 VIEWPORT *	pViewport)
{
	float	rOffset;
	int		nLetterIndex, nWordStartIndex;

	// Move backward to the beginning the of current word.
	nWordStartIndex = nDisplayLettersCount - 1;
	while ( ( nWordStartIndex >= 0 ) &&
			( _pDisplayedLetters[ nWordStartIndex ]._rScreenRight > GetSafePosRight( pViewport ) ) &&
			( IsSpace( _pDisplayedLetters[ nWordStartIndex ]._cLetter ) == true ) )
	{
		--nWordStartIndex;
	}
	while ( ( nWordStartIndex >= 0 ) &&
			( _pDisplayedLetters[ nWordStartIndex ]._rScreenRight > GetSafePosRight( pViewport ) ) &&
			( IsSpace( _pDisplayedLetters[ nWordStartIndex ]._cLetter ) == false ) )
	{
		--nWordStartIndex;
	}
	++nWordStartIndex;

	QDT_ASSERT( nWordStartIndex < nDisplayLettersCount - 1 );

	// Align last word to the left and remove last line space.
	rOffset = _pDisplayedLetters[ nWordStartIndex ]._rScreenLeft - GetSafePosLeft( pViewport );
	for ( nLetterIndex = nWordStartIndex; nLetterIndex < nDisplayLettersCount; ++nLetterIndex )
	{	
		_pDisplayedLetters[ nLetterIndex - 1 ] = _pDisplayedLetters[ nLetterIndex ];

		_pDisplayedLetters[ nLetterIndex - 1 ]._rScreenLeft		-= rOffset;
		_pDisplayedLetters[ nLetterIndex - 1 ]._rScreenRight	-= rOffset;
	}

	return ( nWordStartIndex );
}

//-----------------------------------------------------------------------------
//	Name:		LineReturn
//	Object:		
//	05-08-25:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::LineReturn(float &	rNextLineTop,
									 int &		nLineFirstLetterIndex,
									 int &		nDisplayLettersCount,
									 int &		nLinesCount,
									 int		nHorzAlign,
									 float		rTextSizePercent,
									 VIEWPORT *	pViewport)
{
	int		nLetterIndex;
	bool	bMoveLastWord;
	float	rOffset;

	if ( nLineFirstLetterIndex == nDisplayLettersCount )
	{
		rNextLineTop	+= ScaleY( GetFont()->GetHeight(), rTextSizePercent, pViewport );
		nLinesCount		+= 1;
	}
	else
	// The the last word fits in the ihm's rect, just align the line and and next line position.
	if ( _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenRight < GetSafePosRight( pViewport ) )
	{
		// Align the last line.
		rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 1, rNextLineTop, nHorzAlign, pViewport );
		nLinesCount		+= 1;

		nLineFirstLetterIndex = nDisplayLettersCount;
	}
	else
	{
		// Try to move the last move onto the next line.
		bMoveLastWord	= false;
		nLetterIndex	= nDisplayLettersCount - 1;
		while ( nLetterIndex >= nLineFirstLetterIndex  )
		{
			if ( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true )
			{
				bMoveLastWord = true;
				break;
			}

			--nLetterIndex;
		}

		// The last word can't be moved (because there's a single word on the line).
		if ( bMoveLastWord == false )
		{
			// Align the last line.
			rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 1, rNextLineTop, nHorzAlign, pViewport );
			nLinesCount		+= 1;

			nLineFirstLetterIndex = nDisplayLettersCount;
		}
		else
		{
			QDT_ASSERT( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true );

			// If the line has first a space then a single word.
			if ( nLetterIndex == nLineFirstLetterIndex )
			{
				rNextLineTop += ScaleY( GetFont()->GetHeight(), _pDisplayedLetters[ nLetterIndex ]._rTextSizePercent, pViewport );
			}
			// Align previous line.
			else
			{
				rNextLineTop += AlignLine( nLineFirstLetterIndex, nLetterIndex - 1, rNextLineTop, nHorzAlign, pViewport );
				nLineFirstLetterIndex = nLetterIndex;
			}
			nLinesCount += 1;

			// Move last word the the next line.
			rOffset = _pDisplayedLetters[ nLineFirstLetterIndex + 1 ]._rScreenLeft - GetSafePosLeft( pViewport );
			for ( nLetterIndex = nLineFirstLetterIndex + 1; nLetterIndex < nDisplayLettersCount; ++nLetterIndex )
			{	
				_pDisplayedLetters[ nLetterIndex - 1 ] = _pDisplayedLetters[ nLetterIndex ];

				_pDisplayedLetters[ nLetterIndex - 1 ]._rScreenLeft		-= rOffset;
				_pDisplayedLetters[ nLetterIndex - 1 ]._rScreenRight	-= rOffset;
			}
			--nDisplayLettersCount;

			// Align last line.
			rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 1, rNextLineTop, nHorzAlign, pViewport );
			nLinesCount		+= 1;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		LineCut
//	Object:		
//	05-08-25:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::LineCut(float &		rNextLineTop,
								  int &			nLineFirstLetterIndex,
								  int &			nDisplayLettersCount,
								  int &			nLinesCount,
								  bool &		bNewLine,
								  int			nHorzAlign,
								  VIEWPORT *	pViewport)
{
	QDT_ASSERT( bNewLine == false );

#ifdef __QDT_JAPANESE_LINE_CUT__
	
	float	rWidth;

	if ( ( nDisplayLettersCount >= 2 ) &&
		 ( _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenRight >= GetSafePosRight( pViewport ) ) )
	{
		// Move last char to next line.
		rWidth	= _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenRight - _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenLeft;
		_pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenLeft		= GetSafePosLeft( pViewport );
		_pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenRight	= _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenLeft + rWidth;

		// Align the last line.
		rNextLineTop			+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 2, rNextLineTop, nHorzAlign, pViewport );
		nLinesCount				+= 1;
		nLineFirstLetterIndex	= nDisplayLettersCount - 1;
	}

#else

	bool	bMoveLastWord;
	int		nLetterIndex;
	float	rOffset;

	if ( ( nDisplayLettersCount >= 2 ) &&
		 ( IsSpace( _pDisplayedLetters[ nDisplayLettersCount - 1 ]._cLetter ) == true ) )
	{
		// The 2 last letters are spaces.
		if ( ( IsSpace( _pDisplayedLetters[ nDisplayLettersCount - 2 ]._cLetter ) == true ) &&
			 ( _pDisplayedLetters[ nDisplayLettersCount - 1 ]._rScreenRight >= GetSafePosRight( pViewport ) ) )
		{
			--nDisplayLettersCount;

			// Align the last line.
			rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 1, rNextLineTop, nHorzAlign, pViewport );
			nLinesCount		+= 1;

			nLineFirstLetterIndex = nDisplayLettersCount;

			bNewLine = true;
		}
		else
		// Perform usual line cut.
		if ( ( IsSpace( _pDisplayedLetters[ nDisplayLettersCount - 2 ]._cLetter ) == false ) &&
			 ( _pDisplayedLetters[ nDisplayLettersCount - 2 ]._rScreenRight >= GetSafePosRight( pViewport ) ) )
		{
			--nDisplayLettersCount;

			// Is it possible to move last word?
			bMoveLastWord			= false;
			nLetterIndex			= nDisplayLettersCount - 1;
			while ( nLetterIndex >= nLineFirstLetterIndex  )
			{
				if ( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true )
				{
					bMoveLastWord = true;
					break;
				}

				--nLetterIndex;
			}

			// The last word can't be moved (because there's a single word on the line).
			if ( bMoveLastWord == false )
			{
				// So we just align the last line.
				rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 1, rNextLineTop, nHorzAlign, pViewport );
				nLinesCount		+= 1;

				nLineFirstLetterIndex	= nDisplayLettersCount;
				bNewLine				= true;
			}
			else
			{
				QDT_ASSERT( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true );

				// Align the last line.
				rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nLetterIndex - 1, rNextLineTop, nHorzAlign, pViewport );
				nLinesCount		+= 1;

				// Move last word the the next line.
				nLineFirstLetterIndex	= nLetterIndex;
				rOffset					= _pDisplayedLetters[ nLineFirstLetterIndex + 1 ]._rScreenLeft - GetSafePosLeft( pViewport );
				for ( nLetterIndex = nLineFirstLetterIndex + 1; nLetterIndex <= nDisplayLettersCount; ++nLetterIndex )
				{	
					_pDisplayedLetters[ nLetterIndex - 1 ] = _pDisplayedLetters[ nLetterIndex ];

					_pDisplayedLetters[ nLetterIndex - 1 ]._rScreenLeft		-= rOffset;
					_pDisplayedLetters[ nLetterIndex - 1 ]._rScreenRight	-= rOffset;
				}

				// The last word doesn't fit in the ihm's rect.
				if ( _pDisplayedLetters[ nDisplayLettersCount - 2 ]._rScreenRight >= GetSafePosRight( pViewport ) )
				{
					--nDisplayLettersCount;

					rNextLineTop	+= AlignLine( nLineFirstLetterIndex, nDisplayLettersCount - 1, rNextLineTop, nHorzAlign, pViewport );
					nLinesCount		+= 1;

					nLineFirstLetterIndex = nDisplayLettersCount;

					bNewLine = true;
				}
			}
		}
	}

#endif
}

//-----------------------------------------------------------------------------
//	Name:		AlignLine
//	Object:		
//	05-08-22:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::AlignLine(int			nLineFirstLetterIndex,
									int			nLastLineLetterIndex,
									float		rNextLineTop,
									int			nHorzAlign,
									VIEWPORT *	pViewport)
{
	QDT_ASSERT( nLineFirstLetterIndex <= nLastLineLetterIndex );

	AlignLineHorz( nLineFirstLetterIndex, nLastLineLetterIndex, nHorzAlign, pViewport );

	return ( AlignLineVert( nLineFirstLetterIndex, nLastLineLetterIndex, rNextLineTop, pViewport ) );
}

//-----------------------------------------------------------------------------
//	Name:		AlignLineVert
//	Object:		
//	05-08-29:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::AlignLineVert(int			nLineFirstLetterIndex,
										int			nLastLineLetterIndex,
										float		rNextLineTop,
										VIEWPORT *	pViewport)
{
	int		nLetterIndex, nAbove, nBelow;
	float	rMaxSizePercent, rScreenFloor, rLineHeight;
	short	nUVsTop, nUVsBottom, nUVsFloor, nDummyShort;
	char	nDummyChar;

	// Compute the height of the current line.
	rMaxSizePercent = 0;
	for ( nLetterIndex = nLineFirstLetterIndex; nLetterIndex <= nLastLineLetterIndex; ++nLetterIndex )
	{
		rMaxSizePercent = MATH::Max( rMaxSizePercent, _pDisplayedLetters[ nLetterIndex ]._rTextSizePercent );
	}
	rLineHeight		= ScaleY( GetFont()->GetHeight(), rMaxSizePercent, pViewport );

	// Compute the floor of the current line.
	rScreenFloor	= rNextLineTop + ( GetFont()->GetHeight() - GetFont()->GetFloorOffset() ) * rLineHeight / GetFont()->GetHeight();

	// Align the letters vertically.
	for ( nLetterIndex = nLineFirstLetterIndex; nLetterIndex <= nLastLineLetterIndex; ++nLetterIndex )
	{
		if ( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true )
		{
			_pDisplayedLetters[ nLetterIndex ]._rScreenTop		= rScreenFloor;
			_pDisplayedLetters[ nLetterIndex ]._rScreenBottom	= rScreenFloor;
		}
		else
		{
			GetFont()->GetLetterDesc( _pDisplayedLetters[ nLetterIndex ]._cLetter,
								  	  nDummyChar, 
									  nDummyShort, 
									  nDummyShort, 
									  nUVsTop, 
									  nUVsBottom, 
									  nUVsFloor );

			nAbove	= nUVsFloor  - nUVsTop + 1;
			nBelow	= nUVsBottom - nUVsFloor;

			_pDisplayedLetters[ nLetterIndex ]._rScreenTop		= rScreenFloor - ScaleY( nAbove, _pDisplayedLetters[ nLetterIndex ]._rTextSizePercent, pViewport );
			_pDisplayedLetters[ nLetterIndex ]._rScreenBottom	= rScreenFloor + ScaleY( nBelow, _pDisplayedLetters[ nLetterIndex ]._rTextSizePercent, pViewport );
		}
	}

#if (defined PLATFORM_PC) && (!defined _MASTER)
	VECTOR				Pt1, Pt2;
	QDT_VECTOR<SEGMENT>	vSegments;

	if ( _bExportDisplay == true )
	{
		// Floor.
		Pt1.SetXYZ( 0,   rScreenFloor * 100 / RENDER_CONTEXT::GetScreenHeight(), 0.0f );
		Pt2.SetXYZ( 100, rScreenFloor * 100 / RENDER_CONTEXT::GetScreenHeight(), 0.0f );
		vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );
		QDT_NOT_IMPLEMENTED();
//		RENDER_MANAGER::Instance()->GetRenderContext()->Render2DSegments( vSegments, FLOOR_COLOR );
		vSegments.Clear();

		// Interline.
		Pt1.SetXYZ( 0,   ( rNextLineTop + rLineHeight ) * 100 / RENDER_CONTEXT::GetScreenHeight(), 0.0f );
		Pt2.SetXYZ( 100, ( rNextLineTop + rLineHeight ) * 100 / RENDER_CONTEXT::GetScreenHeight(), 0.0f );
		vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );
		QDT_NOT_IMPLEMENTED();
//		RENDER_MANAGER::Instance()->GetRenderContext()->Render2DSegments( vSegments, INTERLINE_COLOR );
		vSegments.Clear();
	}
#endif

	return ( rLineHeight );
}

//-----------------------------------------------------------------------------
//	Name:		AlignLineHorz
//	Object:		
//	05-08-29:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::AlignLineHorz(int			nLineFirstLetterIndex,
										int			nLastLineLetterIndex,
										int			nHorzAlign,
										VIEWPORT *	pViewport)
{
	int		nLetterIndex, nSpacesCount;
	float	rHorzOffset, rPerWordOffset;

	if ( nHorzAlign == HALIGN_JUSTIFIED )
	{
		// First we count the spaces.
		nSpacesCount = 0;
		for ( nLetterIndex = nLineFirstLetterIndex; nLetterIndex <= nLastLineLetterIndex; ++nLetterIndex )
		{
			if ( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true )
			{
				++nSpacesCount;
			}
		}

		// And then we process the letters.
		if ( nSpacesCount > 0 )
		{
			rPerWordOffset	= ( GetSafePosRight( pViewport ) - _pDisplayedLetters[ nLastLineLetterIndex ]._rScreenRight ) / nSpacesCount;
			rHorzOffset		= 0;

			for ( nLetterIndex = nLineFirstLetterIndex; nLetterIndex <= nLastLineLetterIndex; ++nLetterIndex )
			{
				if ( IsSpace( _pDisplayedLetters[ nLetterIndex ]._cLetter ) == true )
				{
					rHorzOffset += rPerWordOffset;
				}
				else
				{
					_pDisplayedLetters[ nLetterIndex ]._rScreenLeft		+= rHorzOffset;
					_pDisplayedLetters[ nLetterIndex ]._rScreenRight	+= rHorzOffset;
				}
			}
		}
	}
	else
	{
		// Compute horizontal offset.
		switch ( nHorzAlign )
		{
		case HALIGN_RIGHT:
			rHorzOffset = _pDisplayedLetters[ nLastLineLetterIndex ]._rScreenRight - GetSafePosRight( pViewport );
			break;
		case HALIGN_CENTERED:
			rHorzOffset = 0.5 * ( _pDisplayedLetters[ nLastLineLetterIndex ]._rScreenRight - GetSafePosRight( pViewport ) );
			break;
		default:
			QDT_FAIL();
		case HALIGN_LEFT:
			rHorzOffset = 0;
			break;
		}

		// Apply horizontal offset.
		for ( nLetterIndex = nLineFirstLetterIndex; nLetterIndex <= nLastLineLetterIndex; ++nLetterIndex )
		{
			_pDisplayedLetters[ nLetterIndex ]._rScreenLeft		-= rHorzOffset;
			_pDisplayedLetters[ nLetterIndex ]._rScreenRight	-= rHorzOffset;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		VerticalAlign
//	Object:		
//	05-08-30:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::VerticalAlign(float		rHeight,
										int			nDisplayLettersCount,
										VIEWPORT *	pViewport)
{
	int		nLetterIndex;
	float	rDiff, rOffset, rTextTop, rTextBottom, rTextHeight, rIhmHeight;

	// Update the scroll percent only of the text fits in the ihm's rect.
	if ( _bCheckScroll == true )
	{
		rTextTop	= _pDisplayedLetters[ 0 ]._rScreenTop;
		rTextBottom	= _pDisplayedLetters[ 0 ]._rScreenBottom;

		for ( nLetterIndex = 1; nLetterIndex < nDisplayLettersCount; ++nLetterIndex )
		{
			rTextTop	= MATH::Min( rTextTop,    _pDisplayedLetters[ nLetterIndex ]._rScreenTop );
			rTextBottom	= MATH::Max( rTextBottom, _pDisplayedLetters[ nLetterIndex ]._rScreenBottom );
		}

		rTextHeight	= rTextBottom - rTextTop;
		rIhmHeight	= GetSafePosBottom( pViewport ) - GetSafePosTop( pViewport );

		if ( rTextHeight < rIhmHeight )
		{
			_rScrollPercent = _rOldScrollPercent;
		}
		
		_bCheckScroll = false;
	}

	rDiff	= GetSafePosBottom( pViewport ) - GetSafePosTop( pViewport ) - rHeight;
	rOffset	= 0.01f * _rScrollPercent * rDiff;

	for ( nLetterIndex = 0; nLetterIndex < nDisplayLettersCount; ++nLetterIndex )
	{
		_pDisplayedLetters[ nLetterIndex ]._rScreenTop		+= rOffset;
		_pDisplayedLetters[ nLetterIndex ]._rScreenBottom	+= rOffset;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSafePosLeft
//	Object:		
//	05-08-29:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::GetSafePosLeft(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rLeft;

	rScreenRelativePos = GetRelativePosLeft() + SAFE_MARGIN;

	if ( pViewport != NULL )
	{
		rScreenRelativePos = pViewport->GetLeft() + pViewport->GetWidth() * rScreenRelativePos * 0.01f;
	}

	rLeft = RENDER_CONTEXT::GetScreenWidth() * 0.01f * rScreenRelativePos;

	return ( rLeft );
}

//-----------------------------------------------------------------------------
//	Name:		GetSafePosRight
//	Object:		
//	05-08-29:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::GetSafePosRight(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rRight;
	
	rScreenRelativePos = GetRelativePosRight() - SAFE_MARGIN;

	if ( pViewport != NULL )
	{
		rScreenRelativePos = pViewport->GetLeft() + pViewport->GetWidth() * rScreenRelativePos * 0.01f;
	}

	rRight = RENDER_CONTEXT::GetScreenWidth() * 0.01f * rScreenRelativePos;

	return ( rRight );
}

//-----------------------------------------------------------------------------
//	Name:		GetSafePosTop
//	Object:		
//	05-08-29:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::GetSafePosTop(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rTop;

	rScreenRelativePos = GetRelativePosTop() + SAFE_MARGIN;

	if ( pViewport != NULL )
	{
		rScreenRelativePos = pViewport->GetTop() + pViewport->GetHeight() * rScreenRelativePos * 0.01f;
	}

	rTop = RENDER_CONTEXT::GetScreenHeight() * 0.01f * rScreenRelativePos;

	return ( rTop );
}

//-----------------------------------------------------------------------------
//	Name:		GetSafePosBottom
//	Object:		
//	05-08-29:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_TEXT::GetSafePosBottom(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rBottom;
	
	rScreenRelativePos = GetRelativePosBottom() - SAFE_MARGIN;

	if ( pViewport != NULL )
	{
		rScreenRelativePos = pViewport->GetTop() + pViewport->GetHeight() * rScreenRelativePos * 0.01f;
	}

	rBottom = RENDER_CONTEXT::GetScreenHeight() * 0.01f * rScreenRelativePos;

	return ( rBottom );
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors( EM_Construct, EM_Destruct );

	pM = METHOD_DESCRIPTOR__Allocate("GetText", "Returns the text of the element.");
	METHOD_DESCRIPTOR__SetReturn(pM, "Text", "", TYPE_STRING::GetDescriptor());
	pCD->AddMethod(CMD_GET_TEXT, pM, EM_GetText);

	pM = METHOD_DESCRIPTOR::Allocate("SetText", "Set the text of the element.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pM->AddParameter(QDT_COMMENT("Text"), QDT_COMMENT(""), TYPE_STRING::GetDescriptor());
	pCD->AddCommand(CMD_SET_TEXT, pM, CM_SetText, EM_SetText);

	pM = METHOD_DESCRIPTOR__Allocate("SetTextUnicode", "Set the unicode text of the element.");
//	METHOD_DESCRIPTOR__AddParameter(pM, "Text", "", TYPE_STRING_UNICODE::GetDescriptor());
	pCD->AddMethod(CMD_SET_TEXT_UNICODE, pM, EM_SetTextUnicode);

	pM = METHOD_DESCRIPTOR__Allocate("GetFont", "Returns the font of the element.");
	METHOD_DESCRIPTOR__SetReturn(pM, "Font", "", DISPLAY_FONT::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GET_FONT, pM, EM_GetFont);

	pM = METHOD_DESCRIPTOR::Allocate("SetFont", "Set the font of the element.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pM->AddParameter(QDT_COMMENT("Font"), QDT_COMMENT(""), DISPLAY_FONT::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_SET_FONT, pM, CM_SetFont, EM_SetFont);

	pM = METHOD_DESCRIPTOR__Allocate("SetVerticalAlign", "Set the vertical alignement of the text.");
	METHOD_DESCRIPTOR__AddParameter(pM, "VAlign", "", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_VERTICAL_ALIGN, pM, EM_SetVerticalAlign);

	pM = METHOD_DESCRIPTOR__Allocate("ChangeTextColor", "Set the default color of the text.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Color", "", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_SET_DEFAULT_COLOR, pM, EM_SetDefaultColor);

	pM = METHOD_DESCRIPTOR__Allocate("Scroll", "Scroll the text within the ihm_elementtext.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rScrollPercent", "If set to 0, the text is at its top, and if 'nScrollPercent' is set to 100, the bottom of the text gets visible. Please alse note that negative percentage are allowed. So are ones larger than 100.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SCROLL, pM, EM_Scroll);

	pM = METHOD_DESCRIPTOR__Allocate("SetScrollSpeed", "Sets the speed the ihm_elementtext scrolls.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rScrollPercent", "Default is 1.0.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SCROLL_SPEED, pM, EM_SetScrollSpeed);

	pM = METHOD_DESCRIPTOR__Allocate("ScrollUp", "Scroll up the text in the ihm_elementtext. If the text fits entirely, does nothing.");
	pCD->AddMethod(CMD_SCROLL_UP, pM, EM_ScrollUp);

	pM = METHOD_DESCRIPTOR__Allocate("ScrollDown", "Scroll down the text in the ihm_elementtext. If the text fits entirely, does nothing.");
	pCD->AddMethod(CMD_SCROLL_DOWN, pM, EM_ScrollDown);

	pM = METHOD_DESCRIPTOR__Allocate("Copy", "Copy the IHM_ELEMENT.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "IhmElement", "The IhmElement to copy.", IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_COPY, pM, EM_Copy);

	pM = METHOD_DESCRIPTOR__Allocate("GetNbrLines", "Returns the numbers of lines of the text.");
	METHOD_DESCRIPTOR__SetReturn(pM, "nNbrLines", "The lines count.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GET_NBR_LINES, pM, EM_GetNbrLines);

	pM = METHOD_DESCRIPTOR__Allocate("GetTextBoundingRect", "Returns the smallest rectangle in which the displayed text can fit.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rLeft",	"Left position of the rectangle.",		TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rRight",	"Right position of the rectangle.",		TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rTop",	"Top position of the rectangle.",		TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rBottom",	"Bottom position of the rectangle.",	TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "bRelative",	"Relative or absolute coordinates.",				TYPE_BOOL::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pM, "bOk", "'false' if failed (for exemple on an empty text o).", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_TEXT_BOUNDING_RECT, pM, EM_GetTextBoundingRect);

	pM = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableTempMemPoolAllocation"), QDT_COMMENT("Enable Temp Mem Pool Allocation"));
	pM->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pM->SetStatic(true);
	pCD->AddMethod(CMD_ENABLE_TEMP_MEM_POOL_ALLOCATION, pM, EM_EnableTempMemPoolAllocation);

	pM = METHOD_DESCRIPTOR__Allocate("SetForcedSize", "Set the size of the ihm text, overriding the size balise.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rPercent", "-1 to remove.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_FORCED_SIZE, pM, EM_SetForcedSize);

#ifndef _MASTER
	pCD->AddEnum("VALIGN_CENTERED", VALIGN_CENTERED, "Text verticaly centered");
	pCD->AddEnum("VALIGN_TOP",		VALIGN_TOP,      "Text verticaly top centered.");
	pCD->AddEnum("VALIGN_BOTTOM",	VALIGN_BOTTOM,   "Text verticaly bottom centered.");
#endif
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Construct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	QDT_NEW_PLACED( static_cast<IHM_ELEMENT_TEXT *>( pO ), IHM_ELEMENT_TEXT)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Destruct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	static_cast<IHM_ELEMENT_TEXT *>( pO )->IHM_ELEMENT_TEXT::~IHM_ELEMENT_TEXT();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetText
//	Object:		
//	05-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetText(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	QDT_STRING *		sText;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	sText		= static_cast<QDT_STRING *>( pR );
	QDT_NEW_PLACED(sText, QDT_STRING)();

	*sText = pIhmText->GetText().ToString();

	QDT_Message( "*** IHM_ELEMENT_TEXT::GetText(): Unicode to ANSI conversion ***" );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetText
//	Object:		
//	05-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetText(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	QDT_STRING *		sText;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	sText		= static_cast<QDT_STRING *>( pP );

	pIhmText->SetText( *sText );
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetText
//	Object:		
//	06-03-15:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_TEXT::CM_SetText(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_TEXT);
	COM_INSTANCE pObjet;
	QDT_STRING pText;
	Stack.GetThis(pObjet);
	
	IHM_ELEMENT_TEXT	*pIhmText = static_cast<IHM_ELEMENT_TEXT *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, pText);
	pIhmText->SetText(pText);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetTextUnicode
//	Object:		
//	05-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetTextUnicode(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	QDT_STRING_UNICODE *		sText;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	sText		= static_cast<QDT_STRING_UNICODE *>( pP );

	pIhmText->SetText( *sText );
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetFont
//	Object:		
//	05-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetFont(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFont
//	Object:		
//	05-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetFont(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *		pObjet;
	IHM_ELEMENT_TEXT *	pIhmText;
	COM_INSTANCE *		pParam;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pParam		= static_cast<COM_INSTANCE *>( pP );

	pIhmText->SetFontCI( *pParam );
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetFont
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_TEXT::CM_SetFont(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_FONT);
	COM_INSTANCE pObjet;
	COM_INSTANCE Font;
	Stack.GetThis(pObjet);
	
	IHM_ELEMENT_TEXT	*pIhmText = static_cast<IHM_ELEMENT_TEXT *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, Font);
	pIhmText->SetFontCI(Font);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetVerticalAlign
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetVerticalAlign(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	int *				pParam;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pParam		= static_cast<int *>( pP );

	pIhmText->SetVerticalAlignement( (VALIGN)*pParam );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDefaultColor
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetDefaultColor(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	COLOR *				pParam;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pParam		= static_cast<COLOR *>( pP );

	pIhmText->SetDefaultColor( *pParam );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScrollSpeed
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetScrollSpeed(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_ScrollUp
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_ScrollUp(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_ScrollDown
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_ScrollDown(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Copy
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Copy(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Scroll
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Scroll(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	float *				pParam;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pParam		= static_cast<float *>( pP );

	pIhmText->Scroll( *pParam );
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetNbrLines
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetNbrLines(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	int *				pReturn;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pReturn		= static_cast<int *>( pR );

	*pReturn	= pIhmText->GetNbrLines();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTextBoundingRect
//	Object:		
//	05-09-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetTextBoundingRect(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	struct	PARAMS
	{
		float *	_rLeft;
		float *	_rRight;
		float *	_rTop;
		float *	_rBottom;
		bool	_bRelative;
	};
	PARAMS *			pParams;
	bool *				pReturn;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pReturn		= static_cast<bool *>( pR );
	pParams		= static_cast<PARAMS *>( pP );

	*pReturn	= pIhmText->GetTextBoundingRect( *pParams->_rLeft,
												 *pParams->_rRight,
												 *pParams->_rTop,
												 *pParams->_rBottom,
												 pParams->_bRelative );
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableTempMemPoolAllocation
//	Object:		
//	05-09-30:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_EnableTempMemPoolAllocation(void * const	pO,
														 void * const	pR,
														 void * const	pP)
{
	// Ok, for now, the mem pool allication doesn't exist anymore.
	// See if we need one with the new IHM_ELEMENT_TEXT.
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetImposedSize
//	Object:		
//	05-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetForcedSize(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmText;
	float *				pParam;
	
	pObjet		= static_cast<COM_INSTANCE *>( pO );
	pIhmText	= static_cast<IHM_ELEMENT_TEXT *>( pObjet->GetCommunicator() );
	pParam		= static_cast<float *>( pP );

	pIhmText->SetForcedSize( *pParam );
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-03:	GGO - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_TEXT::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_TEXT"), 
												   MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT, 
												   QDT_COMMENT("162017C8-CB6C-F6AC-5269-96BEF31BD1F1"),
												   sizeof(IHM_ELEMENT_TEXT),
												   ComInit, 
												   QDT_COMMENT("This ihm_element displays a text into the display window. If the element has no font set, the text can't be displayed. Please refer to PA010293.doc to get more informations on IHM_ELEMENT_TEXT. By default, text of the element is left aligned horizontally and centered vertically."),
												   IHM_ELEMENT_RECT::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EnableFloorDisplay
//	Object:		
//	05-08-30:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	IHM_ELEMENT_TEXT::EnableExportDisplay(const bool	bEnable)
{
	_bExportDisplay = bEnable;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetDefaultColor
//	Object:		
//	05-09-22:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetDefaultColor(const COLOR &	Color)
{
	_DefaultColor = Color;
}

//-----------------------------------------------------------------------------
//	Name:		SetImposedSize
//	Object:		
//	05-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetForcedSize(const float	rPercent)
{
	_rForcedSize = rPercent;
}

//-----------------------------------------------------------------------------
//	Name:		AddFindAndReplace
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::AddFindAndReplace(const QDT_STRING_UNICODE &	Find,
											const QDT_STRING_UNICODE &	Replace)
{
	_vFind.PushTail( Find );
	_vReplace.PushTail( Replace );
}

//-----------------------------------------------------------------------------
//	Name:		ClearFindAndReplace
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ClearFindAndReplace()
{
	_vFind.Clear();
	_vReplace.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ApplyFindAndReplace
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ApplyFindAndReplace(QDT_STRING_UNICODE &	sText)
{
	unsigned int i, nCount = _vFind.GetSize();

	if (nCount > 0)
	{
		// First pass to allow swapping between two elements
		for (i = 0 ; i < nCount ; ++i)
		{
			QDT_STRING_UNICODE sSwap("Tontontontontontondusera");
			sSwap +=  (int)i;
			sText.Replace(_vFind[i], sSwap);
		}

		// This is the replace loop, pay attention that we loop from the greatest to 
		// the smallest to avoid bug!
		for (i = nCount - 1 ; i > 0 ; i--)
		{
			QDT_STRING_UNICODE sSwap("Tontontontontontondusera");
			sSwap +=  (int)i;
			sText.Replace(sSwap, _vReplace[i]);
		}

		QDT_STRING_UNICODE sSwap("Tontontontontontondusera");
		sSwap +=  (int)i;
		sText.Replace(sSwap, _vReplace[i]);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


