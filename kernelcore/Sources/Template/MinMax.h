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
// CLASS: MIN_MAX
//
//
//00-10-30 ONA Created: 
//*****************************************************************************

#ifndef __KCORE_TEMPLATE_MINMAX_H__
#define __KCORE_TEMPLATE_MINMAX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		// 2003-04-23 JPE undef min & max if exists because .NET defines it 
		#ifdef min
		#undef min
		#endif

		#ifdef max
		#undef max
		#endif

		// 2005-12-05 BFA added clamp & clip helpers
		template<class T>const T& QDT_CLAMP(const T& _IN, const T& _MIN, const T& _MAX)
		{
			if	(_IN < _MIN)
			{
				return	_MIN;
			}
			else
			{
				if	(_IN > _MAX)
				{
					return	_MAX;
				}
				else
				{
					return	_IN;
				}
			}
		}

		template<class T>void QDT_CLIP(T& _IN, const T& _MIN, const T& _MAX)
		{
			if	(_IN < _MIN)
			{
				_IN=_MIN;
			}
			else
			{
				if	(_IN > _MAX)
				{
					_IN=_MAX;
				}
				else
				{
					_IN=_IN;
				}
			}
		}

		template<class	T>
		T	min(const T	A,
				const T	B)
		{
			if	(A < B)
			{
				return	A;
			}
			else
			{
				return	B;
			}
		}

		// 2003-04-23 JPE  -> create an ambiguous call with .net too ...
		#if (_MSC_VER < 1300)
		#ifdef PLATFORM_PC
		template<class	T>
		const T&	min(const T&	A,
						const T&	B)
		{
			if	(A < B)
			{
				return	A;
			}
			else
			{
				return	B;
			}
		}
		#endif //PLATFORM_PC
		#endif // (_MSCVER < 1300)
		template<class	T>
		T	max(const T	A,
				const T	B)
		{
			if	(A < B)
			{
				return	B;
			}
			else
			{
				return	A;
			}
		}

		// 2003-04-23 JPE  -> create an ambiguous call with .net too ...
		#if (_MSC_VER < 1300)
		#ifdef PLATFORM_PC
		template<class	T>
		const T&	max(const T&	A,
						const T&	B)
		{
			if	(A < B)
			{
				return	B;
			}
			else
			{
				return	A;
			}
		}
		#endif
		#endif // (_MSCVER < 1300)
		#if defined(PLATFORM_PC) || defined(PLATFORM_XBOX)
		template<class	T>
		void	swap(T&	A,
					T&	B)
		{
			T	Temp(A);
			A = B;
			B = Temp;
		}
		#endif
	}
}

#endif //__KCORE_TEMPLATE_MINMAX_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
