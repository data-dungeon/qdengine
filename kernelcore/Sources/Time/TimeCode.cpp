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
//	CLASS:	TIME_CODE
//
//	01-08-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(Time/TimeCode)
#include	INCL_KCORE(Stream/iostream)

//-----------------------------------------------------------------------------
//	Name:		TIME_CODE constructor
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
TIME_CODE::TIME_CODE()
:_nHours(0)
,_nMinutes(0)
,_nSeconds(0)
,_nCentiseconds(0)
,_nFramesCount(0)
,_nNext(0)
,_nPrev(0)
{

}
//-----------------------------------------------------------------------------
//	Name:		TIME_CODE constructor
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
TIME_CODE::TIME_CODE(const TIME_CODE &TC)
{
	(*this) = TC;
}

//-----------------------------------------------------------------------------
//	Name:		TIME_CODE constructor
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
TIME_CODE::TIME_CODE(	unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi,
						int FramesCount,
						int DataLength,
						unsigned int nNext, unsigned int nPrev )
:_nHours(Hours)
,_nMinutes(Minutes)
,_nSeconds(Seconds)
,_nCentiseconds(Centi)
,_nFramesCount(FramesCount)
,_nNext(nNext)
,_nPrev(nPrev)
{

}

//-----------------------------------------------------------------------------
//	Name:		~TIME_CODE destructor
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
TIME_CODE::~TIME_CODE()
{

}

//-----------------------------------------------------------------------------
//	Name:		~TIME_CODE destructor
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
TIME_CODE& TIME_CODE::operator=(const TIME_CODE &TC)
{
	if (this == &TC)
		return (*this);
	else
	{
		_nHours = TC._nHours;
		_nMinutes = TC._nMinutes;
		_nSeconds = TC._nSeconds;
		_nCentiseconds = TC._nCentiseconds;
		_nFramesCount = TC._nFramesCount;
		_nNext = TC._nNext;
		_nPrev = TC._nPrev;
		return (*this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetInfos
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::SetInfos(	unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi,
							int FramesCount)
{
	_nHours	= Hours;
	_nMinutes = Minutes;	
	_nSeconds = Seconds;	
	_nCentiseconds = Centi;
	_nFramesCount = FramesCount;
}

//-----------------------------------------------------------------------------
//	Name:		GetInfos
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::GetInfos(	unsigned char &Hours, unsigned char &Minutes, unsigned char &Seconds, unsigned char &Centi,
							int &FramesCount )
{
	Hours = _nHours;	
	Minutes = _nMinutes;
	Seconds	= _nSeconds;
	Centi = _nCentiseconds;
	FramesCount = _nFramesCount;	
}

//-----------------------------------------------------------------------------
//	Name:		SetTime
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::SetTime(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi)
{
	_nHours		= Hours;
	_nMinutes	= Minutes;	
	_nSeconds	= Seconds;	
}

//-----------------------------------------------------------------------------
//	Name:		GetTime
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::GetTime(unsigned char &Hours, unsigned char &Minutes, unsigned char &Seconds, unsigned char &Centi)
{
	Hours		= _nHours;	
	Minutes		= _nMinutes;
	Seconds		= _nSeconds;
}

//-----------------------------------------------------------------------------
//	Name:		AddTime
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::AddTime(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi)
{
	if (Centi>99)
		++_nSeconds;
	_nCentiseconds += Centi % 100;

	if (_nCentiseconds>99)
	{
		++_nSeconds;
		_nCentiseconds -= 99;
	}

	if (Seconds>59)
		++_nMinutes;
	_nSeconds += Seconds % 60;	

	if (_nSeconds>59)
	{
		++_nMinutes;
		_nSeconds -= 59;
	}

	if (Minutes>59)
		++_nHours;
	_nMinutes += Minutes % 60;
	
	if (_nMinutes>59)
	{
		++_nHours;
		_nMinutes -= 59;
	}
	
	_nHours	+= Hours;
}

//-----------------------------------------------------------------------------
//	Name:		AddTimeInCentiseconds
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::AddTimeInCentiseconds(int Centiseconds)
{
	unsigned char Hours = 0; 
	unsigned char Minutes = 0;
	unsigned char Seconds = 0;
	unsigned char Centi = 0;

	int RemainedTime = Centiseconds;

	Centi = Centiseconds % 100;

	RemainedTime = Centiseconds - int(Centi);
	RemainedTime /= 100;
	if (RemainedTime>0)
		Seconds = RemainedTime % 60;

	RemainedTime -= int(Seconds);
	RemainedTime /= 60;
	if (RemainedTime>0)
		Minutes = RemainedTime % 60;

	RemainedTime -= int(Minutes);
	RemainedTime /= 60;
	if (RemainedTime>0)
		Hours = RemainedTime % 60;

	AddTime(Hours, Minutes, Seconds, Centi);
}	

//-----------------------------------------------------------------------------
//	Name:		write
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::Write(IO_STREAM &s) const
{
	s << _nHours;
	s << _nMinutes;
	s << _nSeconds;
	s << _nCentiseconds;
	s << _nFramesCount;
	s << _nNext;
	s << _nPrev;
}

//-----------------------------------------------------------------------------
//	Name:		read
//	Object:		
//	01-08-02:	JLT - Created
//-----------------------------------------------------------------------------
void TIME_CODE::Read(IO_STREAM &s)
{
	s >> _nHours;
	s >> _nMinutes;
	s >> _nSeconds;
	s >> _nCentiseconds;
	s >> _nFramesCount;
	s >> _nNext;
	s >> _nPrev;
}

//-----------------------------------------------------------------------------
//	Name:		return the time in deciseconds
//	Object:		
//	01-08-14:	JLT - Created
//-----------------------------------------------------------------------------
unsigned int TIME_CODE::GetTime() const
{
	return (unsigned int)( _nHours * 360000L + _nMinutes * 6000L + _nSeconds * 100L + _nCentiseconds);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
