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
//	CLASS:	RENDER_WINDOW
//	The RENDER_WINDOW class implements ...
//
//	02-06-07:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	RENDER_WINDOW : COM_OBJECT
{
public:

						RENDER_WINDOW();
					   ~RENDER_WINDOW();

	void				CreateWindow();

	void				Display();

private:

	RENDER_CONTEXT	   *_pContext;

	bool				_bFullScreen;

	// FullScreen
	RENDER_MODE			_FullscreenMode;

	// Window
	unsigned int		_nSizeX;
	unsigned int		_nSizeY;
	unsigned int		_nPosX;
	unsigned int		_nPosY;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
