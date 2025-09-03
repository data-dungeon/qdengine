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

class	TEXT_PARSER;

class	IHM_ELEMENT_TEXT : public IHM_ELEMENT_RECT
{
public:
	
	enum
	{
		CMD_GET_TEXT = IHM_ELEMENT_RECT::CMD_LAST,
		CMD_SET_TEXT,
		CMD_GET_FONT,
		CMD_SET_FONT,
		CMD_SET_VERTICAL_ALIGN,
		CMD_CHANGE_TEXT_COLOR,
		CMD_SCROLL,
		CMD_SET_SCROLL_SPEED,
		CMD_SCROLL_UP,
		CMD_SCROLL_DOWN,
		CMD_COPY,
		CMD_GET_NBR_LINES,
		CMD_GET_TEXT_BOUNDING_RECT,
		CMD_LAST
	};

	enum VALIGN
	{
		VALIGN_CENTERED	= 0,
		VALIGN_TOP		= 1,
		VALIGN_BOTTOM	= 2,
	};

	/** Default constructor. */
								IHM_ELEMENT_TEXT();
	/** Destructor. */
	virtual					   ~IHM_ELEMENT_TEXT();

	//==== COMMON IHM FUNCTIONS =====

	/** Interface initialization. */
	virtual	void				Open( IHM_INTERFACE *pInterfance, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
	/** Close the interface. */
	virtual	void				Close(IHM_INTERFACE *pInterfance, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
	/** Display. */
	virtual	void				DisplayElement(VIEWPORT * pViewport);
	/** Display. */
	virtual void				Display(DISPLAY_PARAMETERS& DisplayParameters, float rSizeX, float rSizeY);

	/** Returns the text of the element. */
	const QDT_STRING &			GetText() const;
	/** Set the text of the element. */
	void						SetText(const QDT_STRING & Text);
	/** Set the vertical alignment of the text. */
	void						SetVerticalAlignement(const VALIGN VerticalAlign);

	/** Returns the font of the element. */
	DISPLAY_FONT*				GetFont();
	const COM_INSTANCE &		GetFontInst();
	/** Set the font of the element. */
	void						SetFont(const COM_INSTANCE & Font);

	/** Scroll up the context of the element if it is clipped. */
	void						ScrollUp(VIEWPORT * pViewport);
	/** Scroll down the context of the element if it is clipped. */
	void						ScrollDown(VIEWPORT * pViewport);
	/** Scroll the text within the ihm_elementtext. Please note that negative percentage are allowed. So are ones larger than 100. */
	void						Scroll(VIEWPORT * pViewport, const float rScrollPercent);

	/** OBSOLETE : Set the scroll offset. */
	void						SetScrollStep(const int rStep);
	/** Set the scroll speed of the element. */
	void						SetScrollSpeed(const float rSpeed);

	int							GetNbrLines() const;

	/** Change the color of all the words in the current IHM_ELEMENT_TEXT. */
	void						ChangeTextColor(const COLOR &Color);

	bool						GetTextBoundingRect(VIEWPORT * pRenderingViewport, float & rLeft, float & rRight, float & rTop, float & rBottom, bool bRelative);
	bool						IsTextCropped(VIEWPORT * pRenderingViewport);

	void						ProcessParser( VIEWPORT * pRenderingViewport );

	static void					ReserveParser();

	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

private:

	enum
	{
		DEFAULT_SCROLL_STEP	= 50,
	};


	QDT_STRING					_Text;
	COM_INSTANCE				_Font;
	TEXT_PARSER*				_pTextParser;

	static void			Init(CLASS_DESCRIPTOR * const pCD);
	
	static void			EM_Construct(void * const pO, void * const pR, void * const pP);
	static void			EM_Destruct(void * const pO, void * const pR, void * const pP);
	
	static void			EM_GetText(void * const pO, void * const pR, void * const pP);
	static void			EM_SetText(void * const pO, void * const pR, void * const pP);
	static void			EM_GetFont(void * const pO, void * const pR, void * const pP);
	static void			EM_SetFont(void * const pO, void * const pR, void * const pP);
	static void			EM_SetVerticalAlign(void * const pO, void * const pR, void * const pP);
	static void			EM_ChangeTextColor(void * const pO, void * const pR, void * const pP);
	static void			EM_Scroll(void * const pO, void * const pR, void * const pP);
	static void			EM_SetScrollSpeed(void * const pO, void * const pR, void * const pP);
	static void			EM_ScrollUp(void * const pO, void * const pR, void * const pP);
	static void			EM_ScrollDown(void * const pO, void * const pR, void * const pP);
	static void			EM_Copy(void * const pO, void * const pR, void * const pP);
	static void			EM_GetNbrLines(void * const pO, void * const pR, void * const pP);
	static void			EM_GetTextBoundingRect(void * const pO, void * const pR, void * const pP);
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
