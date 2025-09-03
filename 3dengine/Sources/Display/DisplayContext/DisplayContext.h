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
//	CLASS:	DISPLAY_CONTEXT
//	The DISPLAY_CONTEXT class implements ...
//
//	02-06-07:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	DISPLAY_CONTEXT : public COM_OBJECT
{
public:

	/** Constructor & destructor */
						DISPLAY_CONTEXT();
					   ~DISPLAY_CONTEXT();

	/** Init this context */
	void				Init();

	/** Display this context */
	void				Display() const;

	/** Accessors */
	_INLINE_ const COLOR &			GetBackgroundColor() const		{ return (_BackgroundColor); }
	_INLINE_ const RENDER_MODE &	GetFullScreenMode() const		{ return (_FullScreenMode); }
			 unsigned int			GetWindowPosX() const;			
			 unsigned int			GetWindowPosY() const;			
			 unsigned int			GetWindowSizeX() const;			
			 unsigned int			GetWindowSizeY() const;			
	_INLINE_ RENDER_CONTEXT *		GetRenderContext() const		{ return (_pContext); }

	/** Modifiors */
	_INLINE_ void	SetBackgroundColor(const COLOR &C)				{ _BackgroundColor = C; }
	_INLINE_ void	SetRenderMode(const RENDER_MODE &Mode)			{ _FullScreenMode = Mode; }
		     void	SetWindowPos(unsigned int nX, unsigned int nY);
			 void	SetWindowSize(unsigned int nX, unsigned int nY);

private:

	/** Render context from KernelCore */
	RENDER_CONTEXT		 *_pContext;

	/** Render mode in full screen */
	RENDER_MODE			_FullScreenMode;

	/** List of viewports */
	COM_INSTANCE		_ViewportListInst;

	/** Background clear color */
	COLOR				_BackgroundColor;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
