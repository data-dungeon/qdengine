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
// CLASS: RT_OUTPUT
//
//
//04-01-16 DAC Created: 
//*****************************************************************************

#include	"Root.h"
#include	INCL_RUN(Application/Export/RtOutput)

#include	INCL_RUN(Application/Export/ExportProgressBarDlg)

//=============================================================================
// CODE STARTS HERE
//=============================================================================


//-----------------------------------------------------------------------------
// Name: RT_OUTPUT constructor
// Object: 
// 04-01-16 DAC Created: 
//-----------------------------------------------------------------------------
RT_OUTPUT::RT_OUTPUT(EXPORT_PROGRESS_BAR_DLG * pGUI)
{
	QDT_ASSERT( pGUI );
	_pGUI = pGUI;
}


//-----------------------------------------------------------------------------
// Name: RT_OUTPUT destructor
// Object: 
// 04-01-16 DAC Created: 
//-----------------------------------------------------------------------------
RT_OUTPUT::~RT_OUTPUT()
{

}
    

//-----------------------------------------------------------------------------
// Name: Output
// Object: 
//04-01-16 DAC Created: 
//-----------------------------------------------------------------------------
void RT_OUTPUT::Output(const char *pcString, const bool bWarning)
{
	try
	{
		QDT_STRING strMsg( pcString );
		_pGUI->ConsoleOutput( strMsg );
	}
	catch( ... )
	{
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
