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
//	CLASS:	STATS_ENTITY_ABC
//
//	06-03-24:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_3DENGINE(Display/Statistics/StatsEntityABC)
#include	INCL_KCORE(OS/OS)

const COLOR* STATS_ENTITY_ABC::_colorTable[] =
{
	&COLOR::GREY,
	&COLOR::RED,
	&COLOR::GREEN,
	&COLOR::BLUE,
	&COLOR::YELLOW,
	&COLOR::MAGNETA,
	&COLOR::CYAN,
	&COLOR::ORANGE,
	&COLOR::VIOLET,
	&COLOR::DARK_RED,
	&COLOR::DARK_GREEN,
	&COLOR::DARK_BLUE,
};

//-----------------------------------------------------------------------------
//	Name:		STATS_ENTITY_ABC constructor
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
STATS_ENTITY_ABC::STATS_ENTITY_ABC():
_name(""),
_fTop(0.f),
_fBottom(100.f),
_fLeft(0.f),
_fRight(100.f),
_fMinimumValue(QDT_FLOAT32_MIN),
_fMaximumValue(QDT_FLOAT32_MAX),
_bUseSize(false),
_currentColorIdx(0),
_bUseColoredText(false)
{
}

//-----------------------------------------------------------------------------
//	Name:		STATS_ENTITY_ABC destructor
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
STATS_ENTITY_ABC::~STATS_ENTITY_ABC()
{
		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		SetTimeLog
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
void	STATS_ENTITY_ABC::PushTimeLog(TIME_LOG *	pTimeLog)
{
	QDT_ASSERT(pTimeLog);

	STATS_DATA stats;

	stats._pTimeLog = pTimeLog;
	stats._color = *_colorTable[_currentColorIdx];

	_currentColorIdx = (_currentColorIdx+1)%(sizeof(_colorTable) / sizeof(COLOR*));

	_pTable.PushTail(stats);
}

//-----------------------------------------------------------------------------
//	Name:		SetRangeColor
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
void	STATS_ENTITY_ABC::SetRangeColor(Float32	fStart,
										Float32	fStop,
										COLOR	color)
{
	STATS_COLOR_RANGE newRange;
	
	newRange._fStartValue = fStart;
	newRange._fStopValue = fStop;
	newRange._color = color;

	_colorRange.PushTail(newRange);
}

//-----------------------------------------------------------------------------
//	Name:		GetColorForValue
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
COLOR	STATS_ENTITY_ABC::GetColorForValue(Float32	fValue)
{
	UInt32 i;
	UInt32 vectorSize = _colorRange.GetSize();

	for(i=0;i<vectorSize;i++)
	{
		if(fValue>=_colorRange[i]._fStartValue && fValue<_colorRange[i]._fStopValue)
		{
			return _colorRange[i]._color;
		}
	}

	return COLOR::WHITE;
}

//-----------------------------------------------------------------------------
//	Name:		IsTimeLogInRange
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
Bool	STATS_ENTITY_ABC::IsTimeLogInRange(TIME_LOG *	pTimeLog)
{
	Float32 fLatestValue = pTimeLog->GetLastElement()->_fValue;

	if(fLatestValue>=_fMinimumValue && fLatestValue<=_fMaximumValue)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetFormatedString
//	Object:		
//	06-03-28:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING	STATS_ENTITY_ABC::GetFormatedString(QDT_STRING sDataName, COLOR nameColor, Float32 fValue, COLOR valueColor)
{
	QDT_STRING formatedString;

	if(_bUseColoredText)
	{
		formatedString.Format("{C:%03d:%03d:%03d}", nameColor.GetRed(), nameColor.GetGreen(), nameColor.GetBlue());
	}
	else
	{
		formatedString.Format("{C:%03d:%03d:%03d}", COLOR::WHITE.GetRed(), COLOR::WHITE.GetGreen(), COLOR::WHITE.GetBlue());
	}
	formatedString += sDataName;
	formatedString += ": ";

	QDT_STRING colorString;
	colorString.Format("{C:%03d:%03d:%03d}", valueColor.GetRed(), valueColor.GetGreen(), valueColor.GetBlue());

	formatedString += colorString;

	if(_bUseSize)
	{
		formatedString += OS::GetHumanReadableSize(fValue);
	}
	else
	{
		formatedString += fValue;
	}

	return formatedString;
}

//-----------------------------------------------------------------------------
//	Name:		ConvertColorToString
//	Object:		
//	06-03-28:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING	STATS_ENTITY_ABC::ConvertColorToString(COLOR	color)
{
	QDT_STRING colorString;

	colorString.Format("{C:%03d:%03d:%03d}",color.GetRed(), color.GetGreen(), color.GetBlue());

	return colorString;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
