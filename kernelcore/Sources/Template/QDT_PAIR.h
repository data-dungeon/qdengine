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
// CLASS: QDT_PAIR
//
//
// 01-01-09 DAC Created: 
// 02-05-23 JLT Some modifications :)
// 05-09-14 RMA Rewrited without stl!
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TEMPLATE_QDT_PAIR_H__
#define __KCORE_TEMPLATE_QDT_PAIR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KCORE
	{
		template <class	T, class U >
		class QDT_PAIR
		{
		public:
			
			_INLINE_ QDT_PAIR();
			_INLINE_ QDT_PAIR(const T& f, const U& s);
			_INLINE_ QDT_PAIR(const QDT_PAIR<T, U> & Pair);
			_INLINE_ ~QDT_PAIR();
			
			// Accessors
			_INLINE_ const T & GetFirst() const;
			_INLINE_ const U & GetSecond() const;
			_INLINE_ T & GetFirst();
			_INLINE_ U& GetSecond();
			
			// Mutators
			_INLINE_ void SetFirst(const T& f);
			_INLINE_ void SetSecond(const U& s);

			// Operators
			_INLINE_ QDT_PAIR & operator = (const QDT_PAIR<T,U>& Pair);
			_INLINE_ bool operator == (const QDT_PAIR<T,U> & Pair) const;
			_INLINE_ bool operator != (const QDT_PAIR<T,U> & Pair) const;
			_INLINE_ bool operator < (const QDT_PAIR<T, U> & Pair) const;
			
			_INLINE_ friend IO_STREAM &	operator << (IO_STREAM & Stream, const QDT_PAIR<T, U> & Pair)
			{
				Stream << Pair._First;
				Stream << Pair._Second; 
				return Stream;
			}

			
			_INLINE_ friend IO_STREAM &	operator >> (IO_STREAM & Stream, QDT_PAIR<T, U> & Pair)
			{
				Stream >> Pair._First;
				Stream >> Pair._Second; 
				return Stream;
			}

		private:

			T	_First;
			U	_Second;
		};

		#include "QDT_PAIR.inl"
	}
}

#endif //__KCORE_TEMPLATE_QDT_PAIR_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
