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
//	The TEXT_PARSER class implements ...
//
//	01-10-17:	GGO - Created
//	04-05-05:	RMA - Huge memory optimizations
//				This class was a memory hasher.
//				I've removed all the allocation with two pool for IHM_WORD
//				& IHM_TEXT_LINE.
//				The words in an line are now a fixed size array to avoid
//				the stl fragmentation pitfall. You can increase this size with
//				the define NBR_MAX_WORD_IN_A_LINE.
//				_vOrderedText is now static QDT_VECTOR to avoid permanent
//				new & delete caused by stl
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class VIEWPORT;
class IHM_WORD;

#define NBR_MAX_WORD_IN_A_LINE 64

class IHM_TEXT_LINE
{
public:
	
					IHM_TEXT_LINE();
				   ~IHM_TEXT_LINE();

	void			PushWord(IHM_WORD * pWord);
	unsigned int	GetNbrWords() const;
	IHM_WORD*		GetWord(unsigned int i) const;

	IHM_WORD*		_tpLine[NBR_MAX_WORD_IN_A_LINE];
	unsigned int	_nNbrWordUsed;
};

class	TEXT_PARSER
{
public:

	TEXT_PARSER(IHM_ELEMENT_TEXT *pElementText);
   ~TEXT_PARSER();

	static IHM_WORD *		AllocateWord();
	static IHM_TEXT_LINE *	AllocateTextLine();

	static void				UnallocateWord(IHM_WORD * pW);
	static void				UnallocateTextLine(IHM_TEXT_LINE * pTL);
	
	void					MoveFirstWord();
	const IHM_WORD *		GetCurrentWord();
	void					Process(VIEWPORT * pRenderingViewport);
	bool					MoveNextWord();
	void					ScrollUp(VIEWPORT * pRenderingViewport);
	void					ScrollDown(VIEWPORT * pRenderingViewport);
	void					Scroll(VIEWPORT * pRenderingViewport, const float rScrollPercent);
	void					ChangeTextColor(const COLOR &Color);
	int						GetMemorySize() const;
	int						GetNbLetters() const;
	void					SetVarticalAlign(const IHM_ELEMENT_TEXT::VALIGN VerticalAlign);
	void					SetScrollStep(const int nStep);
	void					SetScrollSpeed(const float rSpeed);
	bool					GetTextBoundingRect(VIEWPORT * pRenderingViewport, float & rLeft, float & rRight, float & rTop, float & rBottom, bool bRelative, bool bForceProcess);
	bool					IsTextCropped(VIEWPORT * pRenderingViewport);
	void					SetIhmElementText(IHM_ELEMENT_TEXT * pElementText);
	int						GetNbrLines();
	void					ApplyFindAndReplace(QDT_STRING & Text);

	static void				AddFindAndReplace(const QDT_STRING & Find, const QDT_STRING & Replace);
	static void				ClearFindAndReplace();


private:

	void				Clear();
	bool				ChangeWordProperties(VIEWPORT * pRenderingViewport);
	void				AlignCurrentLine(VIEWPORT * pRenderingViewport);
	void				PerformVerticalAlign(VIEWPORT * pRenderingViewport);
	bool				ChangeTextColor();
	void				GetTextFullRect(float & rAbsLeft, float & rAbsRight, float & rAbsTop, float & rAbsBottom);

	static void			GetWordBoundingRect(IHM_WORD * pWord, DISPLAY_FONT * pFont, float & rLeft, float & rRight, float & rTop, float & rBottomTmp);

	static MEM_POOL_HEADER *	_pWordPool;
	static MEM_POOL_HEADER *	_pTextLinePool;
	
	enum TEXT_PROPERTIES
	{
		NONE			= 0,
		ALIGN_LEFT		= ( 1 << 0 ),
		ALIGN_RIGHT		= ( 1 << 1 ),
		ALIGN_CENTERED	= ( 1 << 2 ),
		ALIGN_JUSTIFIED	= ( 1 << 3 ),
	};

	enum
	{
		SCROLL_UP_DOWN_RATIO	= 3000,
		BASE_HRES				= 512,
		BASE_VRES				= 448
	};

	QDT_FLAGS							_TextProperties;
	IHM_ELEMENT_TEXT *					_pElementText;
	static QDT_VECTOR<IHM_TEXT_LINE*>	_vOrderedText;
	int									_nCurrentLine;
	unsigned int						_nCurrentWord;
	IHM_WORD *							_pLastWord;
	const char *						_pCurrentPos;
	bool								_bNewParagraph;
	bool								_bOneLinedParagraph;
	int									_n1stLineIndent;
	float								_rTextSizePercent;
	float								_rLinePosY;
	bool								_bFirstLine;
	bool								_bLastLine;
	bool								_bEmptyLine;
	COLOR								_TextColor;
	float								_rScrollSpeed;
	int									_nScrollStep;
	float								_rVerticalPosPercent;

	static QDT_VECTOR <QDT_STRING>		_vFind;
	static QDT_VECTOR <QDT_STRING>		_vReplace;

};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

