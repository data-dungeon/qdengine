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
//	The STATS_ENTITY_ABC class implements ...
//
//	06-03-24:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __STATS_ENTITY_ABC_H__
#define __STATS_ENTITY_ABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(TimeLog/TimeLog)
#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(Template/QDT_Vector)
#include	INCL_KMATH(Math/Color/Color)

class	STATS_ENTITY_ABC
{
public:
	struct STATS_COLOR_RANGE
	{
		Float32 _fStartValue;
		Float32 _fStopValue;
		COLOR	_color;
	};

	struct STATS_DATA
	{
		TIME_LOG*	_pTimeLog;
		COLOR		_color;
	};

	STATS_ENTITY_ABC();
	~STATS_ENTITY_ABC();

	void PushTimeLog( TIME_LOG* pTimeLog );

	void SetName( QDT_STRING name ) { _name = name; }
	QDT_STRING GetName(void) { return _name; }

	void SetRangeColor(Float32 fStart, Float32 fStop, COLOR color);
	COLOR GetColorForValue(Float32 fValue);

	void SetMinimumValue(Float32 fMinimumValue) { _fMinimumValue = fMinimumValue; }
	void SetMaximumValue(Float32 fMaximumValue) { _fMaximumValue = fMaximumValue; }

	void SetDisplayRange(Float32 fTop, Float32 fBottom, Float32 fLeft, Float32 fRight)
	{
		_fTop = fTop;
		_fBottom = fBottom;
		_fLeft = fLeft;
		_fRight = fRight;
	}
	Float32 GetDisplayRangeTop(void) { return _fTop; }
	Float32 GetDisplayRangeBottom(void) { return _fBottom; }
	Float32 GetDisplayRangeLeft(void) { return _fLeft; }
	Float32 GetDisplayRangeRight(void) { return _fRight; }

	virtual void Display(void) = 0;

	Bool IsTimeLogInRange(TIME_LOG* pTimeLog);

	QDT_STRING	GetFormatedString(QDT_STRING sDataName, COLOR nameColor, Float32 fValue, COLOR valueColor);
	QDT_STRING	ConvertColorToString(COLOR	color);

	void SetUseSize(Bool bUseSize) {_bUseSize = bUseSize;}
	void SetUseColoredText(Bool bUseColoredText) {_bUseColoredText = bUseColoredText;}

protected:
	QDT_VECTOR <STATS_DATA> _pTable;
	QDT_VECTOR <STATS_COLOR_RANGE> _colorRange;

private:
	Bool _bUseSize;
	Bool _bUseColoredText;
	UInt32 _currentColorIdx;

	Float32 _fMinimumValue;
	Float32 _fMaximumValue;

	Float32	_fTop;
	Float32	_fBottom;
	Float32	_fLeft;
	Float32	_fRight;

	QDT_STRING _name;

	static const COLOR* _colorTable[];
};

#endif //__STATS_ENTITY_ABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
