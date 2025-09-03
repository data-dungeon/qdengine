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
//	CLASS:	RENDER_CONTEXT
//	The RENDER_CONTEXT class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

struct S_RECTANGLE
{
	void SetRect(UInt32 x1, UInt32 y1, UInt32 x2, UInt32 y2)
	{
		m_uiLeft = x1;
		m_uiTop = y1;
		m_uiRight = x2;
		m_uiBottom = y2;
	}

	void SetRectEmpty()
	{
		m_uiLeft = 0;
		m_uiTop = 0;
		m_uiRight = 0;
		m_uiBottom = 0;
	}

	UInt32 m_uiLeft;
	UInt32 m_uiTop;
    UInt32 m_uiRight;
    UInt32 m_uiBottom;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
