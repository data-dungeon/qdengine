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
//	CLASS:	QDT_FLAGS_TEMPLATE
//	The QDT_FLAGS_TEMPLATE class implements ...
//
//	01-07-12:	GHP - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TOOLS_QDTFLAGS_H__
#define __KCORE_TOOLS_QDTFLAGS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM);

namespace QDT
{
	namespace KCORE
	{
		template <class	T>
		class	QDT_FLAGS_TEMPLATE
		{
		public:
			
				//-----------------------------------------------------------------------------
				//	Name:		QDT_FLAGS constructor
				//	Object:		
				//	04-06-29:	DAC - Created
				//-----------------------------------------------------------------------------
				QDT_FLAGS_TEMPLATE(const T nFlag = 0)
				{
					_nValue = nFlag;
				}

				//-----------------------------------------------------------------------------
				//	Name:		~QDT_FLAGS destructor
				//	Object:		
				//	04-06-29:	DAC - Created
				//-----------------------------------------------------------------------------
				~QDT_FLAGS_TEMPLATE()
				{
					// DO_NOTHING
				}

				//-----------------------------------------------------------------------------
				//	Name:		QDT_FLAGS constructor
				//	Object:		
				//	04-06-29:	DAC - Created
				//-----------------------------------------------------------------------------
				QDT_FLAGS_TEMPLATE( const QDT_FLAGS_TEMPLATE & flag)
				{
					Copy( flag) ;
				}

				//-----------------------------------------------------------------------------
				//	Name:		operator=
				//	Object:		
				//	04-06-29:	DAC - Created
				//-----------------------------------------------------------------------------
				QDT_FLAGS_TEMPLATE<T> &	operator=( const QDT_FLAGS_TEMPLATE<T> & flag)
				{
					Copy(flag);

					return	(*this);
				}

			//-----------------------------------------------------------------------------
			//	Name:		operator==
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			bool	operator==(const QDT_FLAGS_TEMPLATE<T> & nFlags) const
			{
				return	(AsValue() == nFlags.AsValue());
			}

			//-----------------------------------------------------------------------------
			//	Name:		operator!=
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			bool	operator!=(const QDT_FLAGS_TEMPLATE<T> & nFlags) const
			{
				return	(!((*this) == nFlags));
			}

			//-----------------------------------------------------------------------------
			//	Name:		Clear
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Clear()
			{
				_nValue = 0;
			}

			//-----------------------------------------------------------------------------
			//	Name:		GetFlags
			//	Object:		/** Get a flag mask */
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			T	GetFlags(const T nFlagsMask) const
			{
				return	(_nValue & nFlagsMask);
			}

			//-----------------------------------------------------------------------------
			//	Name:		SetFlags
			//	Object:		/** Set a flag mask */
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	SetFlags(const T nFlagsMask, const T nFlagsSet)
			{
				_nValue &= ~nFlagsMask;
				_nValue |=  nFlagsSet;
			}

			//-----------------------------------------------------------------------------
			//	Name:		AsValue
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			T	AsValue() const
			{
				return	(_nValue);
			}
			
			//-----------------------------------------------------------------------------
			//	Name:		AsInt
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			T	AsInt() const
			{
				return	((int)_nValue);
			}

			//-----------------------------------------------------------------------------
			//	Name:		Set
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Set(const T nFlagsSet)
			{
				SetFlags(nFlagsSet, nFlagsSet); 
			}

			//-----------------------------------------------------------------------------
			//	Name:		Set
			//	Object:		/** Set the flag 'nFlagsSet' if 'bSet' is 'true, else, remove it. */
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Set(const T nFlagsSet, const bool bSet)
			{
				if	(bSet)
				{
					Set(nFlagsSet);
				}
				else
				{
					Unset(nFlagsSet);
				}
			}
			
			//-----------------------------------------------------------------------------
			//	Name:		Unset
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Unset(const T nFlagsSet)
			{
				SetFlags(nFlagsSet, 0);
			}

			//-----------------------------------------------------------------------------
			//	Name:		IsSet
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			bool	IsSet(const T nFlagsSet) const
			{
				return	((GetFlags( nFlagsSet)) != 0);
			}

			//=============================================================================
			// IO calls
			//=============================================================================	
			
			//-----------------------------------------------------------------------------
			//	Name:		Read
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Read	(IO_STREAM & Stream)
			{
				Stream >> _nValue ;
			}

			//-----------------------------------------------------------------------------
			//	Name:		Write
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Write(IO_STREAM &	Stream) const 
			{
				Stream << _nValue ;
			}

		private:
			
			//-----------------------------------------------------------------------------
			//	Name:		Copy
			//	Object:		
			//	04-06-29:	DAC - Created
			//-----------------------------------------------------------------------------
			void	Copy(const QDT_FLAGS_TEMPLATE<T> & flag)
			{
				_nValue = flag._nValue;
			}

			T	_nValue ; 
		};

		template <class	T>
		IO_STREAM &	operator << (IO_STREAM & Stream, const QDT_FLAGS_TEMPLATE<T> & Flags)
		{
										Flags.Write(Stream);
										return (Stream);
		}

		template <class	T>
		IO_STREAM& operator >> (IO_STREAM & Stream, QDT_FLAGS_TEMPLATE<T> & Flags)
		{
										Flags.Read(Stream);
										return (Stream);
		}

		typedef QDT_FLAGS_TEMPLATE<int>		QDT_FLAGS;
		typedef QDT_FLAGS_TEMPLATE<short>	QDT_SHORT_FLAGS;
		typedef QDT_FLAGS_TEMPLATE<char>	QDT_CHAR_FLAGS;
	}
}

#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
