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
//	The IHM_ELEMENT_TEXT class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTTEXT_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTTEXT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(DISPLAY_FONT);
DECL_3DENGINE(DISPLAY_PARAMETERS)

#include INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementRect)
#include INCL_KCORE(Tools/QDTStringUnicode)
#include INCL_KMATH(Math/Color/Color)

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENT_TEXT : public IHM_ELEMENT_RECT
		{
		public:
			
			enum
			{
				CMD_GET_TEXT = IHM_ELEMENT_RECT::CMD_LAST,
				CMD_SET_TEXT,
				CMD_SET_TEXT_UNICODE,
				CMD_GET_FONT,
				CMD_SET_FONT,
				CMD_SET_VERTICAL_ALIGN,
				CMD_SET_DEFAULT_COLOR,
				CMD_SCROLL,
				CMD_SET_SCROLL_SPEED,
				CMD_SCROLL_UP,
				CMD_SCROLL_DOWN,
				CMD_COPY,
				CMD_GET_NBR_LINES,
				CMD_GET_TEXT_BOUNDING_RECT,
				CMD_ENABLE_TEMP_MEM_POOL_ALLOCATION,
				CMD_SET_FORCED_SIZE,
				CMD_LAST
			};

			enum VALIGN
			{
				VALIGN_CENTERED	= 0,
				VALIGN_TOP		= 1,
				VALIGN_BOTTOM	= 2,
			};

												IHM_ELEMENT_TEXT();
			virtual							   ~IHM_ELEMENT_TEXT();

			//==== COMMON IHM FUNCTIONS =====
			virtual	void						Open( IHM_INTERFACE * pInterfance, IHM_PAGE * pPage, IHM_ELEMENTS_LIST * pElementsList);
			virtual	void						Close(IHM_INTERFACE * pInterfance, IHM_PAGE * pPage, IHM_ELEMENTS_LIST * pElementsList);
			virtual	void						DisplayElement(VIEWPORT * pViewport);
			virtual void						Display(DISPLAY_PARAMETERS & DisplayParameters, float rSizeX, float rSizeY);

					const QDT_STRING_UNICODE &	GetText() const;
					void						SetText(const QDT_STRING_UNICODE & Text);

					const COM_INSTANCE &		GetFontCI();
					void						SetFontCI(const COM_INSTANCE & Font);

					void						Scroll(const float rScrollPercent);
					void						SetVerticalAlignement(const VALIGN VerticalAlign);

					int							GetNbrLines();
					bool						GetTextBoundingRect(float & rLeft, float & rRight, float & rTop, float & rBottom, bool bRelative = true);

			static	void						AddFindAndReplace(const QDT_STRING_UNICODE & Find, const QDT_STRING_UNICODE & Replace);
			static	void						ClearFindAndReplace();

					void						SetDefaultColor(const COLOR & Color);
					void						SetForcedSize(const float rPercent);

			static	COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static	COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();
			static	void						ComInit(CLASS_DESCRIPTOR * const pCD);

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);

			static	void						EM_GetText(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetText(void * const pO, void * const pR, void * const pP);
			static	int							CM_SetText(COM_PARAM P);
			static	void						EM_SetTextUnicode(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetFont(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetFont(void * const pO, void * const pR, void * const pP);
			static	int							CM_SetFont(COM_PARAM P);
			static	void						EM_SetVerticalAlign(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetDefaultColor(void * const pO, void * const pR, void * const pP);
			static	void						EM_Scroll(void * const pO, void * const pR, void * const pP);
			static 	void						EM_SetScrollSpeed(void * const pO, void * const pR, void * const pP);
			static 	void						EM_ScrollUp(void * const pO, void * const pR, void * const pP);
			static 	void						EM_ScrollDown(void * const pO, void * const pR, void * const pP);
			static 	void						EM_Copy(void * const pO, void * const pR, void * const pP);
			static 	void						EM_GetNbrLines(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetTextBoundingRect(void * const pO, void * const pR, void * const pP);
			static	void						EM_EnableTempMemPoolAllocation(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetForcedSize(void * const pO, void * const pR, void * const pP);

		#ifndef _MASTER	
			static	void						EnableExportDisplay(const bool bEnable);
		#endif


		private:

					DISPLAY_FONT *				GetFont() const;

					void						ProcessText(int & nDisplayLettersCount, int & nLinesCount, VIEWPORT * pViewport);
					bool						ProcessLetter(short cLetter, bool bNewLine, int nDisplayLettersCount, float rTextSizePercent, COLOR Color, VIEWPORT * pViewport);
					float						AlignLine(int nLineFirstLetterIndex, int nLastLineLetterIndex, float rNextLineTop, int nHorzAlign, VIEWPORT * pViewport);
					float						AlignLineVert(int nLineFirstLetterIndex, int nLastLineLetterIndex, float rNextLineTop, VIEWPORT * pViewport);
					void						AlignLineHorz(int nLineFirstLetterIndex, int nLastLineLetterIndex, int nHorzAlign, VIEWPORT * pViewport);
					int							MoveLastWordToNextLine(int nDisplayLettersCount, VIEWPORT * pViewport);
					void						LineReturn(float & rNextLineTop, int & nLineFirstLetterIndex, int & nDisplayLettersCount, int & nLinesCount, int nHorzAlign, float rTextSizePercent, VIEWPORT * pViewport);
					void						LineCut(float & rNextLineTop, int & nLineFirstLetterIndex, int & nDisplayLettersCount, int & nLinesCount, bool & bNewLine, int nHorzAlign, VIEWPORT * pViewport);
					void						VerticalAlign(float rHeight, int nDisplayLettersCount, VIEWPORT * pViewport);

			static	float						ScaleX(float rValue, float rTextSizePercent, VIEWPORT * pViewport);
			static	float						ScaleY(float rValue, float rTextSizePercent, VIEWPORT * pViewport);

					float						GetSafePosLeft(VIEWPORT * pViewport) const;
					float						GetSafePosRight(VIEWPORT * pViewport) const;
					float						GetSafePosTop(VIEWPORT * pViewport) const;
					float						GetSafePosBottom(VIEWPORT * pViewport) const;

			static	void						ApplyFindAndReplace(QDT_STRING_UNICODE & sText);

					const unsigned short *		ReadBalise(const unsigned short * pLetterIt, float & rTextSizePercent, bool & bLineReturn, int & nHorzAlign, COLOR & Color) const;
			static	const unsigned short *		SeekEndOfBalise(const unsigned short * pLetterIt);
					const unsigned short *		ReadBaliseSize(const unsigned short * pLetterIt, float & rTextSizePercent) const;
			static	const unsigned short *		ReadBaliseHorzAlign(const unsigned short * pLetterIt, int & nHorzAlign);
			static	const unsigned short *		ReadBaliseColor(const unsigned short * pLetterIt, COLOR & Color);
			static	const unsigned short *		ReadBaliseUnicodeChar(const unsigned short * pLetterIt, unsigned short & UnicodeChar);
			static	bool						ReadNumber(unsigned short nLetter, int & nNumber);
			static	bool						IsSpace(unsigned short nChar);


		private:

			enum
			{
				HALIGN_LEFT,
				HALIGN_RIGHT,
				HALIGN_CENTERED,
				HALIGN_JUSTIFIED
			};

			struct DISPLAY_LETTER
			{
				unsigned short	_cLetter;
				char			_nMaterialIndex;
				COLOR			_Color;
				float			_rTextSizePercent;
				float			_rScreenLeft;
				float			_rScreenRight;
				float			_rScreenTop;
				float			_rScreenBottom;
			};

					QDT_STRING_UNICODE				_sText;
					COM_INSTANCE					_FontCI;
					DISPLAY_LETTER *				_pDisplayedLetters;
					float							_rScrollPercent;
					// The 2 following variables are used to scroll only if needed.
					float							_rOldScrollPercent;
					bool							_bCheckScroll;
					COLOR							_DefaultColor;
					float							_rForcedSize;
			static	QDT_VECTOR<QDT_STRING_UNICODE>	_vFind;
			static	QDT_VECTOR<QDT_STRING_UNICODE>	_vReplace;
			static	void *							_pVTable;
			static	const int						BASE_HRES;
			static	const int						BASE_VRES;
			static	const float						SAFE_MARGIN;
		#ifndef _MASTER	
			static	bool						_bExportDisplay;
			static	const COLOR					FLOOR_COLOR;
			static	const COLOR					INTERLINE_COLOR;
		#endif
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTTEXT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
