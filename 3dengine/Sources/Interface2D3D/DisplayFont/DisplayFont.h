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
//	CLASS:	DISPLAY_FONT_DATA
//	The DISPLAY_FONT_DATA class implements ...
//
//	02-09-11:	IAM - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_DISPLAYFONT_H__
#define __M3D_INTERFACE2D3D_DISPLAYFONT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/CommunicatorContainer)

namespace QDT
{
	namespace M3D
	{
		class	DISPLAY_FONT : public COMMUNICATOR_CONTAINER
		{
		public:

			enum	DFA_ALPHABET
			{
				DFA_NONE,
				DFA_LATIN,
				DFA_JAPANESE
			};

												DISPLAY_FONT();
											~DISPLAY_FONT();

					void						PushFontMaterialCI(const COM_INSTANCE & MaterialCI);
					const COM_INSTANCE &		GetMaterialCI(const int nIndex) const;
					int							GetMaterialsCount() const;

					void						SetAlphabet(const DFA_ALPHABET Alphabet);
					DFA_ALPHABET				GetAlphabet() const;

					void						SetLetterDesc(unsigned short cLetter, char nTextureIndex, short nLeft, short nRight, short nTop, short nBottom, short nFloor);
					void						GetLetterDesc(unsigned short cLetter, char & nTextureIndex, short & nLeft, short & nRight, short & nTop, short & nBottom, short & nFloor) const;

					void						SetInterLetterSpace(const int nSpace);
					int							GetInterLetterSpace() const;

					void						SetInterWordSpace(const int nSpace);
					int							GetInterWordSpace() const;

					void						SetFloorOffset(const int nOffset);
					int							GetFloorOffset() const;

					void						SetHeight(const int nHeight);
					int							GetHeight() const;

			static	int							GetLettersCount(DFA_ALPHABET Alphabet);

					void						BuildFromStaticDebugFont();

			// Communication
			static	void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static	COMMUNICATOR_CONTAINER_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static	COMMUNICATOR_CONTAINER_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		public:

			static	const int				LATIN_LETTER_FIRST;
			static	const int				LATIN_LETTER_LAST;
			static	const int				FONT_TEXTURE_SIZE_X;
			static	const int				FONT_TEXTURE_SIZE_Y;
			static	const int				JAPANESE_CHARACTERS_COUNT;
			// Don't forget to update the value of 'JAPANESE_CHARACTERS_COUNT' too 
			// in 'DisplayFont.cpp' when adding new japanese characters.
			static	const unsigned short	JAPANESE_MAPPING[ 1666 ];
			static	unsigned short			JAPANESE_MAPPING_INV[ 256 ];


		private:

			// Memory.
			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunks
		#ifndef _MASTER_ONLY_DATABANK
			static	void						CM_ReadChunkV5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static	void						CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif


		private:

					struct	LETTER_DESC
					{
						char	_nMaterialIndex;
						short	_nLeft;
						short	_nRight;
						short	_nTop;
						short	_nBottom;
						short	_nFloor;
					};
					DFA_ALPHABET						_Alphabet;
					int									_nInterLetterSpace;
					int									_nInterWordSpace;
					int									_nFloorOffset;
					int									_nHeight;
					LETTER_DESC *						_pLetterDescs;
					int									_nMaterialsCount;
					COM_INSTANCE *						_pMaterialsCI;
		};
	}
}

#endif // __M3D_INTERFACE2D3D_DISPLAYFONT_H__
 
//=============================================================================
//	CODE ENDS HERE
//=============================================================================

