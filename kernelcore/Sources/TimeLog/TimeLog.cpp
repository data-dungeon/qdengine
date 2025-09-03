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
//
//	06-03-23:	VHA - Created
//*****************************************************************************

#include "root.h"

#include	INCL_KCORE(TimeLog/TimeLog)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TIME_LOG constructor
//	Object:		
//	06-03-23:	VHA - Created
//-----------------------------------------------------------------------------
TIME_LOG::TIME_LOG() :
_fRange(0.f),
_iLogSize(0),
_iCurrentPositionInLog(0),
_pLog(NULL),
_name("")
{
}

//-----------------------------------------------------------------------------
//	Name:		TIME_LOG destructor
//	Object:		
//	06-03-23:	VHA - Created
//-----------------------------------------------------------------------------
TIME_LOG::~TIME_LOG()
{
	if(_pLog)
	{
		delete[] _pLog;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
void	TIME_LOG::Reset()
{
	UInt32 i;

	for(i=0;i<_iLogSize;i++)
	{
		_pLog[i]._fTime = 0.f;
		_pLog[i]._fValue = 0.f;
	}

	_iCurrentPositionInLog = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
void	TIME_LOG::Init(Float32 fRange, Float32 fLogStep)
{
	_fRange = fRange;
	_fLogStep = fLogStep;

	_iLogSize = (UInt32)(fRange/fLogStep);

	_pLog = new S_TIME_LOG_ELEMENT[_iLogSize];

	Reset();
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
void	TIME_LOG::AddValue(Float64 fTime, Float64 fValue)
{
	S_TIME_LOG_ELEMENT* pLastElement = &_pLog[_iCurrentPositionInLog];

	if(fTime - pLastElement->_fTime >= _fLogStep)
	{
		_iCurrentPositionInLog = (_iCurrentPositionInLog+1)%_iLogSize;

		S_TIME_LOG_ELEMENT* pCurrentElement = &_pLog[_iCurrentPositionInLog];

		pCurrentElement->_fTime = fTime;
		pCurrentElement->_fValue = fValue;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetEntryIdx
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
TIME_LOG::S_TIME_LOG_ELEMENT *	TIME_LOG::GetEntryIdx(UInt32	iEntry)
{
	UInt32 iTrueIndex = (_iCurrentPositionInLog+1+iEntry)%_iLogSize;

	return &_pLog[iTrueIndex];
}

//-----------------------------------------------------------------------------
//	Name:		GetValueAtTime
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
Float64	TIME_LOG::GetValueAtTime(Float64	fTime)
{
	UInt32 i;

	if(fTime <= GetEntryIdx(0)->_fTime)
	{
		return GetEntryIdx(0)->_fTime;
	}

	for(i=0;i<_iLogSize-1;i++)
	{
		S_TIME_LOG_ELEMENT* pElement1 = GetEntryIdx(i);
		S_TIME_LOG_ELEMENT* pElement2 = GetEntryIdx(i+1);

		if((pElement1->_fTime < fTime) && (pElement2->_fTime >= fTime))
		{
			Float64 fTimeDiff = pElement2->_fTime - pElement1->_fTime;
			Float64 fValueDiff = pElement2->_fValue - pElement1->_fValue;
			Float64 fInterpolatedValue = pElement1->_fValue+((fValueDiff*(fTime-pElement1->_fTime))/fTimeDiff);

			return fInterpolatedValue;
		}
	}

	return GetEntryIdx(_iLogSize-1)->_fTime;
}

//-----------------------------------------------------------------------------
//	Name:		SetName
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
void	TIME_LOG::SetName(QDT_STRING	name)
{
	_name = name;
}

//-----------------------------------------------------------------------------
//	Name:		GetName
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING	TIME_LOG::GetName(void	)
{
	return _name;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
