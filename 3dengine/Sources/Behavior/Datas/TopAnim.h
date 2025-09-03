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
//	CLASS:	ANIM_TOP
//	The TOP_ANIM class implements ...
//
//	02-01-14:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_TOP_H__
#define __M3D_ANIM_TOP_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace M3D
	{
		class	ANIM_TOP
		{
		public:

			_INLINE_						ANIM_TOP()												{};
			_INLINE_						ANIM_TOP(float rTime, int nValue)						{ _rTime = rTime; _nValue = nValue; }
			_INLINE_					   ~ANIM_TOP()												{};

			// Operators
			_INLINE_	ANIM_TOP &			operator =  (const ANIM_TOP & Top)						{ _rTime = Top._rTime; _nValue = Top._nValue; return (*this); }
			_INLINE_	bool				operator <  (const ANIM_TOP & Top) const				{ return (_rTime < Top._rTime); }	
			_INLINE_	friend IO_STREAM&	operator << (IO_STREAM & Stream, const ANIM_TOP & Top)	{ Top.Write(Stream); return (Stream); }
			_INLINE_	friend IO_STREAM&	operator >> (IO_STREAM & Stream, ANIM_TOP & Top)		{ Top.Read(Stream); return (Stream); }

			// Accessors
			_INLINE_	float				GetTime() const											{ return (_rTime);	};
			_INLINE_	int					GetValue() const										{ return (_nValue); };

			// Modifiors
			_INLINE_	void				SetTime(float rTime)									{ _rTime = rTime;	};
			_INLINE_	void				SetValue(int nValue)									{ _nValue = nValue; };

			// Streams
			_INLINE_	void				Read(IO_STREAM & Stream)								{ Stream >> _rTime >> _nValue; }
			_INLINE_	void				Write(IO_STREAM & Stream) const							{ Stream << _rTime << _nValue; }

		private:

			float		_rTime;
			int			_nValue;
		};
	}
}

#endif // __M3D_ANIM_TOP_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
