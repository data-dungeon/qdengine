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
//	CLASS:	COLOR_INT
//	The COLOR_INT class implements ...
//
//	02-07-09:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_COLOR_COLORINT_H__
#define __KMATH_MATH_COLOR_COLORINT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
#include	INCL_KMATH(Math/Color/Color)

namespace QDT
{
	namespace KMATH
	{				
		class	COLOR_INT
		{
		public:

			_INLINE_				COLOR_INT()									{ _nAlpha = _nRed = _nGreen = _nBlue = 0; }			
			_INLINE_				COLOR_INT(int nA, int nR, int nG, int nB)
									{
										_nAlpha = nA;
										_nRed	= nR;
										_nGreen	= nG;
										_nBlue	= nB;
									}
			_INLINE_				COLOR_INT(const COLOR_INT & C)				{ Copy(C); }
			_INLINE_				COLOR_INT(const COLOR & C)					{ Copy(C); }
			_INLINE_			   ~COLOR_INT()									{}
			
			_INLINE_ COLOR_INT &	operator = (const COLOR_INT & C)	
									{ 
										if (this != &C)	Copy(C);
										return (*this);
									}

			_INLINE_ COLOR_INT &	operator = (const COLOR & C)
									{
										Copy(C);
										return (*this);
									}

			_INLINE_ COLOR_INT 		operator + (const COLOR_INT & C) const
									{
										int nAlpha	= _nAlpha	+ C._nAlpha;
										int nRed	= _nRed		+ C._nRed;
										int nGreen	= _nGreen	+ C._nGreen;
										int nBlue	= _nBlue	+ C._nBlue;

										return (COLOR_INT(nAlpha, nRed, nGreen, nBlue));
									}

			_INLINE_ COLOR_INT 		operator - (const COLOR_INT & C) const
									{
										int nAlpha	= _nAlpha	- C._nAlpha;
										int nRed	= _nRed		- C._nRed;
										int nGreen	= _nGreen	- C._nGreen;
										int nBlue	= _nBlue	- C._nBlue;

										return (COLOR_INT(nAlpha, nRed, nGreen, nBlue));
									}

			_INLINE_ COLOR_INT		operator * (float r) const
									{
										int n = static_cast<int>(r * 255.0f);

										int nAlpha	= (_nAlpha	* n) >> 8;
										int nRed	= (_nRed	* n) >> 8;
										int nGreen	= (_nGreen	* n) >> 8;
										int nBlue	= (_nBlue	* n) >> 8;
										
										return (COLOR_INT(nAlpha, nRed, nGreen, nBlue));
									}

			_INLINE_ COLOR_INT		operator / (float r) const
									{
										float rDiv = 1.0f / r;

										return (*this * rDiv);
									}

			_INLINE_ void			Born()
									{
										if (_nAlpha < 0) _nAlpha = 0;
										if (_nRed	< 0) _nRed	 = 0;
										if (_nGreen	< 0) _nGreen = 0;
										if (_nBlue	< 0) _nBlue	 = 0;

										if (_nAlpha > 255) _nAlpha	= 255;
										if (_nRed	> 255) _nRed	= 255;
										if (_nGreen	> 255) _nGreen	= 255;
										if (_nBlue	> 255) _nBlue	= 255;
									}

			_INLINE_ int			GetAlpha() const	{ return (_nAlpha); }
			_INLINE_ int			GetRed() const		{ return (_nRed); }
			_INLINE_ int			GetGreen() const	{ return (_nGreen); }
			_INLINE_ int			GetBlue() const		{ return (_nBlue); }

			void					Read(IO_STREAM &s);
			void					Write(IO_STREAM &s) const;


			friend IO_STREAM&		operator <<(IO_STREAM &s, const COLOR_INT &a)
									{
										a.Write(s);
										return s;
									}

			friend IO_STREAM&		operator >>(IO_STREAM &s, COLOR_INT &a)
									{
										a.Read(s);
										return s;
									}


		private:

			_INLINE_ void			Copy(const COLOR_INT & C)
									{
										_nAlpha = C._nAlpha;
										_nRed	= C._nRed;
										_nGreen = C._nGreen;
										_nBlue	= C._nBlue;
									}

			_INLINE_ void			Copy(const COLOR & C)
									{
										_nAlpha = C.GetAlpha();
										_nRed	= C.GetRed();
										_nGreen = C.GetGreen();
										_nBlue	= C.GetBlue();
									}

			int	_nAlpha;
			int _nRed;
			int _nGreen;
			int _nBlue;
		};
	}
}

#endif //__KMATH_MATH_COLOR_COLORINT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
