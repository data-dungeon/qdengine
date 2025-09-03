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
// CLASS: POINT2D_PACKED
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES0D_POINT2D_POINT2DPACKED_H__
#define __KMATH_MATH_SHAPES_SHAPES0D_POINT2D_POINT2DPACKED_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(POINT2D)
DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KMATH
	{
		class	POINT2D_PACKED
		{
		public:
			//	Default constructor
			_INLINE_ explicit POINT2D_PACKED(const bool bFlagInit = true);
			//	Copy constructor
			_INLINE_		  POINT2D_PACKED(const POINT2D_PACKED & P);
			//	Members constructor
			_INLINE_		  POINT2D_PACKED(const unsigned char cX, const unsigned char cY);
			//	Constructor from a POINT2D
			_INLINE_		  POINT2D_PACKED(const POINT2D & P);
			//	Destructor
			_INLINE_		 ~POINT2D_PACKED();

			//	Set from a POINT2D_PACKED
			_INLINE_ void			Set(const POINT2D_PACKED & P);
			//	Set from a POINT2D
			_INLINE_ void			Set(const POINT2D & P);


			//	Set its members
			_INLINE_ void			SetXY(const unsigned char cX, const unsigned char cY);
			_INLINE_ void			SetX(const unsigned char cX);
			_INLINE_ void			SetY(const unsigned char cY);
			//	Get its members
			_INLINE_ void			GetXY(unsigned char & cX, unsigned char & cY) const;
			_INLINE_ unsigned char	GetX() const;
			_INLINE_ unsigned char	GetY() const;


			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s)const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const POINT2D_PACKED &m)
			{
				m.Write(s);
				return s;

			}
			friend IO_STREAM& operator >>(IO_STREAM &s, POINT2D_PACKED &m)
			{
				m.Read(s);
				return s;
			}
			
		private:
			unsigned char	_cX, _cY;
		};
	}
}

#ifndef _DEBUG
#include "point2dpacked.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES0D_POINT2D_POINT2DPACKED_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
