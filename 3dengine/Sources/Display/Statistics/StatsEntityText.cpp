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
//	CLASS:	STATS_ENTITY_TEXT
//
//	06-03-24:	VHA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Statistics/StatsEntityText)
#include	INCL_3DENGINE(Display/DisplayManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STATS_ENTITY_TEXT constructor
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
STATS_ENTITY_TEXT::STATS_ENTITY_TEXT() : STATS_ENTITY_ABC(),
_bShowTotal(false),
_bTotalSize(50.f)
{
//		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		STATS_ENTITY_TEXT destructor
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
STATS_ENTITY_TEXT::~STATS_ENTITY_TEXT()
{
		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
void	STATS_ENTITY_TEXT::Display()
{
	Float32 fTotal = 0.f;
	UInt32 i;
	QDT_STRING statsString = "{S:50}";

	for(i=0;i<_pTable.GetSize();i++)
	{
		TIME_LOG* pTimeLog = _pTable[i]._pTimeLog;

		if(!IsTimeLogInRange(pTimeLog))
			continue;

		Float32 fValue = pTimeLog->GetLastElement()->_fValue;

		statsString += GetFormatedString(pTimeLog->GetName(), _pTable[i]._color, fValue, GetColorForValue(fValue));
		statsString += "{P}";

		fTotal+=fValue;
	}

	if(_bShowTotal)
	{
		statsString += GetFormatedString("Total", COLOR::WHITE, fTotal, COLOR::WHITE);
	}

	DISPLAY_MANAGER::Instance()->AddDebugText(statsString,2,2);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
