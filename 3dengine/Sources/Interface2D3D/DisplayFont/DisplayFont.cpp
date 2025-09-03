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
//	CLASS:	DISPLAY_FONT
//
//	05-08-16:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/DisplayFont/DisplayFont)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(CommunicatorType/CommunicatorContainerDescriptor)
#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)
#include	INCL_3DENGINE(Interface2D3D/DisplayFont/DisplayFontDebug)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFile)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderColorPerVertex)
//=============================================================================
//	CODE STARTS HERE
//=============================================================================

const int	DISPLAY_FONT::LATIN_LETTER_FIRST		= 33;
const int	DISPLAY_FONT::LATIN_LETTER_LAST			= 255;
const int	DISPLAY_FONT::FONT_TEXTURE_SIZE_X		= 256;
const int	DISPLAY_FONT::FONT_TEXTURE_SIZE_Y		= 256;
const int	DISPLAY_FONT::JAPANESE_CHARACTERS_COUNT	= 1666;
unsigned short	DISPLAY_FONT::JAPANESE_MAPPING_INV[ 256 ];
	   
#include	"JapaneseMapping.h"

//---- -------------------------------------------------------------------------
//	Na me:		DISPLAY_FONT constructor
//	Object:		
//	05-08-16:	GGO - Created
//-----------------------------------------------------------------------------
DISPLAY_FONT::DISPLAY_FONT()
:
COMMUNICATOR_CONTAINER	(GetGenericCommunicatorDescriptor()),
_Alphabet				( DFA_NONE ),
_nInterLetterSpace		( -3 ),
_nInterWordSpace		( 10 ),
_nFloorOffset			( 5 ),
_nHeight				( 25 ),
_nMaterialsCount		( 0 ),
_pMaterialsCI			( NULL ),
_pLetterDescs			( NULL )
{
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphabet
//	Object:		
//	05-09-12:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::SetAlphabet(const DFA_ALPHABET	Alphabet)
{
	int	nLetterIndex, nLettersCount;

	nLettersCount	= GetLettersCount( Alphabet );
	if ( nLettersCount <= 0 )
	{
		QDT_FAIL();
		return;
	}

	if ( _pLetterDescs != NULL )
	{
		delete[] _pLetterDescs;
	}
	_pLetterDescs = new LETTER_DESC[ nLettersCount ];

	for ( nLetterIndex = 0; nLetterIndex < nLettersCount; ++nLetterIndex )
	{
		_pLetterDescs[ nLetterIndex ]._nMaterialIndex	= -1;
		_pLetterDescs[ nLetterIndex ]._nLeft			= -1;
		_pLetterDescs[ nLetterIndex ]._nRight			= -1;
		_pLetterDescs[ nLetterIndex ]._nTop				= -1;
		_pLetterDescs[ nLetterIndex ]._nBottom			= -1;
		_pLetterDescs[ nLetterIndex ]._nFloor			= -1;
	}

	_Alphabet = Alphabet;
}

//-----------------------------------------------------------------------------
//	Name:		GetAlphabet
//	Object:		
//	05-09-12:	GGO - Created
//-----------------------------------------------------------------------------
DISPLAY_FONT::DFA_ALPHABET	DISPLAY_FONT::GetAlphabet() const
{
	return ( _Alphabet );
}

//-----------------------------------------------------------------------------
//	Name:		~DISPLAY_FONT destructor
//	Object:		
//	05-08-16:	GGO - Created
//-----------------------------------------------------------------------------
DISPLAY_FONT::~DISPLAY_FONT()
{
	if ( _pMaterialsCI != NULL )
	{
		delete[] _pMaterialsCI;
	}

	if ( _pLetterDescs != NULL )
	{
		delete[] _pLetterDescs;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushNewFontTexture
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::PushFontMaterialCI(const COM_INSTANCE &	MaterialCI)
{
	int				nIndex;
	COM_INSTANCE *	pTmp;

	if ( MaterialCI.HasHandle() == false )
	{
		QDT_FAIL();
		return;
	}

	++_nMaterialsCount;
	QDT_ASSERT( _nMaterialsCount > 0 );

	pTmp			= _pMaterialsCI;
	_pMaterialsCI	= new COM_INSTANCE[ _nMaterialsCount ];
	for ( nIndex = 0; nIndex < _nMaterialsCount - 1; ++nIndex )
	{
		_pMaterialsCI[ nIndex ] = pTmp[ nIndex ];
	}

	_pMaterialsCI[ _nMaterialsCount - 1 ] = MaterialCI;
	if ( pTmp != NULL )
	{
		delete[] pTmp;
	}

	PushComHandle( MaterialCI.GetHandle() );
}

//-----------------------------------------------------------------------------
//	Name:		SetLetterDesc
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::SetLetterDesc(unsigned short	cLetter,
									   char				nMaterialIndex,
									   short			nLeft,
									   short			nRight,
									   short			nTop,
									   short			nBottom,
									   short			nFloor)
{
	int				nMapIndex;
	LETTER_DESC *	pLetterDescIt = NULL;

	switch ( _Alphabet )
	{
	case DFA_LATIN:
		if ( ( cLetter >= LATIN_LETTER_FIRST ) && ( cLetter <= LATIN_LETTER_LAST ) &&
			 ( nLeft <= nRight ) && ( nTop <= nBottom ) )
		{
			pLetterDescIt = _pLetterDescs + ( cLetter - LATIN_LETTER_FIRST );
		}
		break;

	case DFA_JAPANESE:
		for ( nMapIndex = 0; nMapIndex < JAPANESE_CHARACTERS_COUNT; ++nMapIndex )
		{
			if ( JAPANESE_MAPPING[ nMapIndex ] == cLetter )
			{
				pLetterDescIt = _pLetterDescs + nMapIndex;
				break;
			}
		}
		break;
	}

	if ( pLetterDescIt != NULL )
	{
		pLetterDescIt->_nMaterialIndex	= nMaterialIndex;
		pLetterDescIt->_nLeft			= nLeft;
		pLetterDescIt->_nRight			= nRight;
		pLetterDescIt->_nTop			= nTop;
		pLetterDescIt->_nBottom			= nBottom;
		pLetterDescIt->_nFloor			= nFloor;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetLetterDesc
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::GetLetterDesc(unsigned short	cLetter,
									   char &			nMaterialIndex,
									   short &			nLeft,
									   short &			nRight,
									   short &			nTop,
									   short &			nBottom,
									   short &			nFloor) const
{
	int				nMapIndex;
	LETTER_DESC *	pLetterDescIt = NULL;

	switch ( _Alphabet )
	{
	case DFA_LATIN:
		if ( ( cLetter >= LATIN_LETTER_FIRST ) && ( cLetter <= LATIN_LETTER_LAST ) )
		{
			pLetterDescIt = _pLetterDescs + ( cLetter - LATIN_LETTER_FIRST );
		}
		break;

	case DFA_JAPANESE:
		if ( cLetter < 256 )
		{
			pLetterDescIt = _pLetterDescs + JAPANESE_MAPPING_INV[cLetter];
		}
		else
		{
			for ( nMapIndex = 0; nMapIndex < JAPANESE_CHARACTERS_COUNT; ++nMapIndex )
			{
				if ( JAPANESE_MAPPING[ nMapIndex ] == cLetter )
				{
					pLetterDescIt = _pLetterDescs + nMapIndex;
					break;
				}
			}
		}

		break;
	}

	if ( pLetterDescIt != NULL )
	{
		nMaterialIndex	= pLetterDescIt->_nMaterialIndex;
		nLeft			= pLetterDescIt->_nLeft;
		nRight			= pLetterDescIt->_nRight;
		nTop			= pLetterDescIt->_nTop;
		nBottom			= pLetterDescIt->_nBottom;
		nFloor			= pLetterDescIt->_nFloor;
	}
	else
	{
		nMaterialIndex	= -1;
		nLeft			= -1;
		nRight			= -1;
		nTop			= -1;
		nBottom			= -1;
		nFloor			= -1;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetInterLetterSpace
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::SetInterLetterSpace(const int	nSpace)
{
	_nInterLetterSpace = nSpace;
}

//-----------------------------------------------------------------------------
//	Name:		GetInterLetterSpace
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_FONT::GetInterLetterSpace() const
{
	return ( _nInterLetterSpace );
}

//-----------------------------------------------------------------------------
//	Name:		SetInterWordSpace
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::SetInterWordSpace(const int	nSpace)
{
	_nInterWordSpace = nSpace;
}

//-----------------------------------------------------------------------------
//	Name:		GetInterWordSpace
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_FONT::GetInterWordSpace() const
{
	return ( _nInterWordSpace );
}

//-----------------------------------------------------------------------------
//	Name:		SetFloorOffset
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::SetFloorOffset(const int	nOffset)
{
	_nFloorOffset = nOffset;
}

//-----------------------------------------------------------------------------
//	Name:		GetFloorOffset
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_FONT::GetFloorOffset() const
{
	return ( _nFloorOffset );
}

//-----------------------------------------------------------------------------
//	Name:		SetHeight
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::SetHeight(const int	nHeight)
{
	_nHeight = nHeight;
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-08-19:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_FONT::GetHeight() const
{
	return ( _nHeight );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterialCI
//	Object:		
//	05-08-18:	GGO - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	DISPLAY_FONT::GetMaterialCI(const int	nIndex) const
{
	QDT_ASSERT( ( nIndex >= 0 ) && ( nIndex < _nMaterialsCount ) );

	return ( _pMaterialsCI[ nIndex ] );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterialsCount
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_FONT::GetMaterialsCount() const
{
	return ( _nMaterialsCount );	
}

//-----------------------------------------------------------------------------
//	Name:		GetLettersCount
//	Object:		
//	05-08-16:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_FONT::GetLettersCount(DFA_ALPHABET Alphabet)
{
	switch ( Alphabet )
	{
	case DFA_LATIN:
		return ( LATIN_LETTER_LAST - LATIN_LETTER_FIRST + 1 );

	case DFA_JAPANESE:
		return ( JAPANESE_CHARACTERS_COUNT );

	default:
		QDT_FAIL();
		return ( -1 );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors( EM_Construct, EM_Destruct );

		// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "FONT", 5, CM_ReadChunkV5 );
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk( "FONT", 5, CM_WriteChunk );
#endif

	memset( JAPANESE_MAPPING_INV, 0, 256 * sizeof(unsigned short) );

	for ( unsigned short idx = 0 ; idx < JAPANESE_CHARACTERS_COUNT ; idx++ )
	{
		if ( JAPANESE_MAPPING[idx] < 256 )
		{
			JAPANESE_MAPPING_INV[JAPANESE_MAPPING[idx]] = idx;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	DISPLAY_FONT::GetGenericCommunicatorDescriptor()
{
	return ( (COMMUNICATOR_CONTAINER_DESCRIPTOR *) COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_FONT ));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-03:	GGO - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	DISPLAY_FONT::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_CONTAINER_DESCRIPTOR(	QDT_COMMENT("DISPLAY_FONT"),
													MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_FONT,
													QDT_COMMENT("1715EDCA-3EC7-3C7D-2157-C8D3003B4071"),
													sizeof(DISPLAY_FONT),
													ComInit,
													QDT_COMMENT("Class for display fonts"),
													COMMUNICATOR_CONTAINER::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::EM_Construct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	QDT_NEW_PLACED( static_cast<DISPLAY_FONT *>( pO ), DISPLAY_FONT )();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::EM_Destruct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	QDT_DELETE_PLACED( static_cast<DISPLAY_FONT *>( pO ), DISPLAY_FONT );
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkV5
//	Object:		
//	05-08-18:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	DISPLAY_FONT::CM_ReadChunkV5(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	COM_INSTANCE		MaterialCI;
	DISPLAY_FONT *	pFont;
	int					nLetterIndex, nLettersCount, nMaterialIndex, nAlphabet, nMaterialsCount;

	pFont = static_cast<DISPLAY_FONT *>( pCommunicator );

	// The alphabet.
	Stream >> nAlphabet;
	pFont->SetAlphabet( (DFA_ALPHABET)nAlphabet );

	// Font properties.
	Stream >> pFont->_nInterLetterSpace;
	Stream >> pFont->_nInterWordSpace;
	Stream >> pFont->_nFloorOffset;
	Stream >> pFont->_nHeight;

	// Letters descriptors.
	nLettersCount = GetLettersCount( (DFA_ALPHABET)nAlphabet );
	for ( nLetterIndex = 0; nLetterIndex < nLettersCount; ++nLetterIndex )
	{
		Stream >> pFont->_pLetterDescs[ nLetterIndex ]._nMaterialIndex;
		Stream >> pFont->_pLetterDescs[ nLetterIndex ]._nLeft;
		Stream >> pFont->_pLetterDescs[ nLetterIndex ]._nRight;
		Stream >> pFont->_pLetterDescs[ nLetterIndex ]._nTop;
		Stream >> pFont->_pLetterDescs[ nLetterIndex ]._nBottom;
		Stream >> pFont->_pLetterDescs[ nLetterIndex ]._nFloor;
	}

	// The maps.
	Stream >> nMaterialsCount;
	for ( nMaterialIndex = 0; nMaterialIndex < nMaterialsCount; ++nMaterialIndex )
	{
		Stream >> MaterialCI;
		pFont->PushFontMaterialCI( MaterialCI );
	}

	///// code de generation de la font de debug...
/*	{
		int i;
		FILE* fHandle = fopen("c:\\dumpFont.txt", "w+");
		for(i=0;i<nLettersCount;i++)
		{
			if(pFont->_pLetterDescs[ i ]._nMaterialIndex != -1)
			{
				fprintf(fHandle,"{ %d, %d, %d, %d, %d, %d, %d },\n",	i, 
																		pFont->_pLetterDescs[ i ]._nMaterialIndex,
																		pFont->_pLetterDescs[ i ]._nLeft,
																		pFont->_pLetterDescs[ i ]._nRight,
																		pFont->_pLetterDescs[ i ]._nTop,
																		pFont->_pLetterDescs[ i ]._nBottom,
																		pFont->_pLetterDescs[ i ]._nFloor );

			}
		}

		fclose(fHandle);
	}*/
	/////
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	05-08-18:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	DISPLAY_FONT::CM_WriteChunk(IO_STREAM &		Stream,
									   COMMUNICATOR *	pCommunicator)
{
	DISPLAY_FONT *	pFont;
	int					nLetterIndex, nLettersCount, nMaterialIndex;

	pFont = static_cast<DISPLAY_FONT *>( pCommunicator );

	// Alphabet.
	Stream << pFont->_Alphabet;

	// Font properties.
	Stream << pFont->_nInterLetterSpace;
	Stream << pFont->_nInterWordSpace;
	Stream << pFont->_nFloorOffset;
	Stream << pFont->_nHeight;

	// Letters descriptors.
	nLettersCount = GetLettersCount( pFont->_Alphabet );
	for ( nLetterIndex = 0; nLetterIndex < nLettersCount; ++nLetterIndex )
	{
		Stream << pFont->_pLetterDescs[ nLetterIndex ]._nMaterialIndex;
		Stream << pFont->_pLetterDescs[ nLetterIndex ]._nLeft;
		Stream << pFont->_pLetterDescs[ nLetterIndex ]._nRight;
		Stream << pFont->_pLetterDescs[ nLetterIndex ]._nTop;
		Stream << pFont->_pLetterDescs[ nLetterIndex ]._nBottom;
		Stream << pFont->_pLetterDescs[ nLetterIndex ]._nFloor;
	}

	//The maps.
	Stream << pFont->_nMaterialsCount;
	for ( nMaterialIndex = 0; nMaterialIndex < pFont->_nMaterialsCount; ++nMaterialIndex )
	{
		Stream << pFont->_pMaterialsCI[ nMaterialIndex ];
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		BuildFromStaticDebugFont
//	Object:		
//	06-03-20:	VHA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_FONT::BuildFromStaticDebugFont(void	)
{
	COM_INSTANCE ShaderCI;
	COM_INSTANCE TextureCI;
	SetAlphabet(DFA_JAPANESE);

	int i;
	int numEntryToParse = sizeof(DISPLAY_FONT_DEBUG_TABLE) / sizeof(DISPLAY_FONT_DEBUG_STRUCT);

	for(i=0;i<numEntryToParse;i++)
	{
		int nLetterIndex = DISPLAY_FONT_DEBUG_TABLE[i]._entryIndex;

		_pLetterDescs[ nLetterIndex ]._nMaterialIndex	=	DISPLAY_FONT_DEBUG_TABLE[i]._nMaterialIndex;
		_pLetterDescs[ nLetterIndex ]._nLeft			=	DISPLAY_FONT_DEBUG_TABLE[i]._nLeft;
		_pLetterDescs[ nLetterIndex ]._nRight			=	DISPLAY_FONT_DEBUG_TABLE[i]._nRight;
		_pLetterDescs[ nLetterIndex ]._nTop				=	DISPLAY_FONT_DEBUG_TABLE[i]._nTop;
		_pLetterDescs[ nLetterIndex ]._nBottom			=	DISPLAY_FONT_DEBUG_TABLE[i]._nBottom;
		_pLetterDescs[ nLetterIndex ]._nFloor			=	DISPLAY_FONT_DEBUG_TABLE[i]._nFloor;
	}

	TextureCI.SetHandle( ENGINE_TEXTURE_FILE::GetGenericCommunicatorDescriptor()->Create() );
	ENGINE_TEXTURE_FILE* pTexture	= static_cast<ENGINE_TEXTURE_FILE *>( TextureCI.GetCommunicator() );

	pTexture->SetFormat(PF_A8R8G8B8);
	pTexture->SetNbrMipmaps(1);
	pTexture->SetWidth(256);
	pTexture->SetHeight(256);
	pTexture->SetAddressU(AT_WRAP);
	pTexture->SetAddressV(AT_WRAP);
	pTexture->SetFilter(FT_BILINEAR);
	pTexture->SetCubeMap(false);
	char* tempData = new char[256*256*4];
	memcpy(tempData, DEBUG_FONT_TEXTURE, 256*256*4);
	pTexture->SetRawData(0, 0, (UInt8*)tempData, 256*256*4);
	pTexture->CreateTexture();

	ShaderCI.SetHandle( SHADER_COLOR_PER_VERTEX::GetGenericCommunicatorDescriptor()->Create() );
	SHADER_COLOR_PER_VERTEX* pShader = static_cast<SHADER_COLOR_PER_VERTEX *>( ShaderCI.GetCommunicator() );
	pShader->SetBlending( SB_ALPHA );
	pShader->SetTexture(TextureCI);

	PushFontMaterialCI(ShaderCI);

	SetHeight(25);
	SetInterLetterSpace(-3);
	SetInterWordSpace(10);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================

