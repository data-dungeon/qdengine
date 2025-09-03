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
//	CLASS:	RECTANGLE
//	The RECTANGLE class implements ...
//
//	01-10-22:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_RECTANGLE_RECTANGLE_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_RECTANGLE_RECTANGLE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KMATH
	{
		class	RECTANGLE
		{
		public:

			_INLINE_				RECTANGLE();
			_INLINE_				RECTANGLE(int nLeft, int nRight, int nTop, int nBottom);
			_INLINE_				RECTANGLE(const RECTANGLE & C);
			_INLINE_			   ~RECTANGLE();

						RECTANGLE &	operator=(const RECTANGLE & C);
						void		Copy(const RECTANGLE & C);

			_INLINE_	int			GetLeft()   const;
			_INLINE_	int			GetRight()  const;
			_INLINE_	int			GetTop()    const;
			_INLINE_	int			GetBottom() const;

			_INLINE_	void		SetLeft(  const int nLeft);
			_INLINE_	void		SetRight( const int nRight);
			_INLINE_	void		SetTop(   const int nTop);
			_INLINE_	void		SetBottom(const int nBottom);

		private:

			int		_nLeft;
			int		_nRight;
			int		_nTop;
			int		_nBottom;
		};

	}
}

#ifndef _DEBUG
#include "Rectangle.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_RECTANGLE_RECTANGLE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
