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
//	CLASS:	GENERIC_KEY
//	The GENERIC_KEY class implements ...
//
//	02-06-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_GENERIC_KEY_H__
#define __M3D_GENERIC_KEY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Stream\IOStream)

namespace QDT
{
	namespace M3D
	{
template <class T>
class	GENERIC_KEY
{
public:

	/** Constructor */
						GENERIC_KEY()
						{
							// Do nothing
						}

	/** Constructor with value */
						GENERIC_KEY(float rTime, const T& tValue)
						{
							_rTime = rTime;
							_tValue = tValue;
						}
	
	/** Constructor with value and tangent*/
						GENERIC_KEY(float rTime, const T& tValue, const T& tInTangent, const T& tOutTangent)
						{
							_rTime			= rTime;
							_tValue			= tValue;
							_tInTangent		= tInTangent;
							_tOutTangent	= tOutTangent;
						}

	/** Copy constructor */
						GENERIC_KEY(const GENERIC_KEY & Key)
						{
							Copy(Key);
						}

	/** Destructor */
					   ~GENERIC_KEY()
						{
							// Do nothing
						}

	/** Operator = */
	GENERIC_KEY &		operator = (const GENERIC_KEY & Key)
						{
							if (this != &Key)
							{
								Copy(Key);
							}

							return (*this);
						}

	/** Operator < */
	bool				operator < (const GENERIC_KEY & Key) const
						{
							return (_rTime < Key.GetTime());
						}

	/** operator > */
	bool				operator > (const GENERIC_KEY & Key) const
						{
							return (_rTime > Key.GetTime());
						}	

	/** Accessors */
	const T &			GetValue() const			{ return (_tValue); }
	const T &			GetInTangent() const		{ return (_tInTangent); }
	const T &			GetOutTangent() const		{ return (_tOutTangent); }
	float				GetTime() const				{ return (_rTime); }

	/** Modifiors */
	void				SetValue(const T& tValue)			{ _tValue = tValue; }
	void				SetInTangent(const T& tTangent)		{ _tInTangent = tTangent; }
	void				SetOutTangent(const T& tTangent)	{ _tOutTangent = tTangent; }
	void				SetTime(float rTime)				{ _rTime = rTime; }

	/** Operator << */
	friend IO_STREAM&	operator << (IO_STREAM & Stream, const GENERIC_KEY & Key)
						{
							Stream << Key.GetTime();
							Stream << Key.GetValue();
							Stream << Key.GetInTangent();
							Stream << Key.GetOutTangent();
							return (Stream);
						}

	/** Operator >> */
	friend IO_STREAM&	operator >> (IO_STREAM & Stream, GENERIC_KEY & Key)
						{
							Stream >> Key.GetTime();
							Stream >> Key.GetValue();
							Stream >> Key.GetInTangent();
							Stream >> Key.GetOutTangent();
							return (Stream);
						}

private:

	/** Copy */
	void				Copy(const GENERIC_KEY & Key)
						{
							_rTime			= Key.GetTime();
							_tValue			= Key.GetValue();
							_tInTangent		= Key.GetInTangent();
							_tOutTangent	= Key.GetOutTangent();
						}

	T					_tValue;
	T					_tInTangent;
	T					_tOutTangent;
	float				_rTime;
};
}
}

#endif // __M3D_GENERIC_KEY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
