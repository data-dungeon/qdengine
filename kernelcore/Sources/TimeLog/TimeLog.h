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
//	CLASS:	TIME_LOG
//	The TIME_LOG class implements ...
//
//	06-03-23:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __TIME_LOG_H__
#define __TIME_LOG_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Tools/QdtString)

class	TIME_LOG
{
public:
	struct S_TIME_LOG_ELEMENT
	{
		Float64 _fTime;
		Float64 _fValue;
	};
	TIME_LOG();
	~TIME_LOG();

	void		SetName(QDT_STRING name);
	QDT_STRING	GetName(void);

	Float32 GetRange(void) { return _fRange; };
	Float32 GetLogStep(void) { return _fLogStep; };
	UInt32	GetLogSize(void) { return _iLogSize; };
	S_TIME_LOG_ELEMENT* GetLastElement(void) {return &_pLog[_iCurrentPositionInLog];}
	S_TIME_LOG_ELEMENT* GetEntryIdx(UInt32 iEntry);
	Float64 GetValueAtTime(Float64 fTime);

	void Init(Float32 fRange, Float32 fLogStep);
	void Reset();
	void AddValue(Float64 fTime, Float64 fValue);

private:
	QDT_STRING _name;
	Float32 _fRange;
	Float32 _fLogStep;
	UInt32 _iLogSize;
	UInt32 _iCurrentPositionInLog;
	S_TIME_LOG_ELEMENT*	_pLog;
};

#endif //__TIME_LOG_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
