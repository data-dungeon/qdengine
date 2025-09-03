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
//	CLASS:	QDT_VECTOR
//	This class is in fact nothing else than a growable array.
//	It's here to replace our old wrapper which map to the stl vector.  
//	The stl::vector didn't satisfy us in term of memory control
//
//	05-08-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TEMPLATE_QDT_VECTOR_H__
#define __KCORE_TEMPLATE_QDT_VECTOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#if defined(PLATFORM_PC) || (!defined(QDT_MEM_INTERNAL))

#define QDT_INCLUDE_MEMORY
#include INCL_KCORE(Includes/Includes)

DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KCORE
	{
		// This enum are here to build correctly!
		// They sould disappear soon...
		#define VECTOR_START 	0
		#define VECTOR_LAST  	0
		#define VECTOR_END   	0
		//

		class QDT_VECTOR_ALLOCATOR;

		template <class T>
		class QDT_VECTOR
		{
		public:

			// This methods are here to build correctly!
			// They sould disappear soon...
			void Destroy() {}
			//

			class ITERATOR;

			_INLINE_ QDT_VECTOR();
			_INLINE_ QDT_VECTOR(UInt32 nSize, const T & initializationValue);
			_INLINE_ QDT_VECTOR(const QDT_VECTOR & vector);
			_INLINE_ ~QDT_VECTOR();
			
			// Get the number of element pushed in the vector
			_INLINE_ UInt32 GetSize() const;

			// This method clear the vector but not release memory!!
			_INLINE_ void Clear();

			// This method is reserved for pointer pushed in the vector
			// It call delete on all pointer before 
			_INLINE_ void ClearAll();

			//added direct access to array (at your own risk, unprotected)
			_INLINE_ T* GetArray()
			{
				return _pArray;
			}

			// Get an iterator to the first element
			_INLINE_ ITERATOR GetHead() const
			{
				return (ITERATOR(const_cast<QDT_VECTOR<T>*>(this), 0));
			}

			// Get an iterator to the last element
			_INLINE_ ITERATOR GetLast() const
			{
				return (ITERATOR(const_cast<QDT_VECTOR<T>*>(this), _nSize - 1));
			}

			// Get an iterator to the first element after the last one!
			_INLINE_ ITERATOR GetTail() const
			{
				return (ITERATOR(const_cast<QDT_VECTOR<T>*>(this), _nSize));
			}

			// Get an iterator to the specified element
			_INLINE_ ITERATOR GetIterator(UInt32 nIndex) const
			{
				return (ITERATOR(const_cast<QDT_VECTOR<T>*>(this), nIndex));
			}

			// Push an element at the end of the vector
			_INLINE_ void PushTail(const	T&	t);

			// Push an element n times at the end of the vector
			_INLINE_ void PushTail(const T & t, UInt32 nTimes);

			// Push a vector at the end of the vector
			_INLINE_ void PushTail(const QDT_VECTOR& v);

			// Push an element at the end of the vector if this element doesn't exist
			_INLINE_ ITERATOR PushOnce(const T & t)
			{
				ITERATOR Cur = Find(GetHead(), t);

				if (Cur == GetTail())
				{
					PushTail(t);
					Cur = GetLast();
				}

				return (Cur);
			}

			// Pop the last element of the vector
			_INLINE_ void PopTail();

			// Find the given element in the vector
			_INLINE_ ITERATOR Find(const ITERATOR& it, const T& t) const
			{
				QDT_ASSERT(it.IsValid());

				UInt32 i;
				for (i = it.GetIndex() ; i < _nSize ; ++i)
				{
					if (_pArray[i] == t)
					{
						break;
					}
				}

				return (ITERATOR(const_cast<QDT_VECTOR<T>*>(this), i));
			}

			// Find the given element in the vector
			_INLINE_ UInt32 Find(const UInt32& nStart, const T& t) const
			{
				//QDT_ASSERT(nStart>=0);
				QDT_ASSERT(nStart < _nSize);

				UInt32 i;
				for (i = nStart ; i < _nSize ; ++i)
				{
					if (_pArray[i] == t)
					{
						break;
					}
				}

				return (i);
			}

			// Check if the element exist in the QDT_VECTOR
			_INLINE_ bool IsExist(const T& t) const;

			// Memory management
			
				// Reserve has no effect if you try to reserve a vector with a size smaller of the allocated size
				// Return false is the allocation has no effect, otherwise true
				_INLINE_ bool Reserve(UInt32 nReserveSize);

				// Capacity is the allocated size (it's different from the size of the vector)
				_INLINE_ UInt32 GetCapacity() const;

				_INLINE_ UInt16 GetFirstAlloc() const;
				_INLINE_ UInt16 GetNextAlloc() const;
				_INLINE_ void		SetFirstAlloc(UInt16 n);
				_INLINE_ void		SetNextAlloc(UInt16 n);
				_INLINE_ void		Resize(UInt32 uSize);

				//
			//	Retourne true si le vector est vide
			_INLINE_ bool IsEmpty() const;

			// Get the index of the given iterator
			_INLINE_ UInt32 GetIndex(const ITERATOR& I) const;
			
			//	Operators
			_INLINE_ T & operator[](UInt32 nElementIndex);
			_INLINE_ const T & operator[](UInt32 nElementIndex) const;
			_INLINE_ QDT_VECTOR& operator+=(const	T&	t);
			_INLINE_ QDT_VECTOR& operator+=(const QDT_VECTOR<T>& v);
			_INLINE_ QDT_VECTOR& operator=(const QDT_VECTOR<T>& QV);
			_INLINE_ bool operator==(const QDT_VECTOR<T>& v) const;
			_INLINE_ bool operator!=(const QDT_VECTOR<T>& v) const;

			_INLINE_ friend IO_STREAM &	operator << (IO_STREAM & Stream, const QDT_VECTOR<T> & Vector)
			{
				Stream << Vector.GetSize();

				for (UInt32 i = 0 ; i < Vector.GetSize() ; ++i)
				{
					Stream << Vector[i];
				}

				return (Stream);
			}

			_INLINE_ friend IO_STREAM &	operator >> (IO_STREAM & Stream, QDT_VECTOR<T> & Vector)
			{
				//
				//	JPE 17/01/2006 :	Commented to avoid a compiler warning (variable used without having
				//						been initialized) / this method begins with QDT_FAIL() anyway.
				//

				
				QDT_FAIL();

//				UInt32 nSize;
//				//Stream >> nSize;

//				Vector.Reserve(nSize);
//
//				for (UInt32 i = 0 ; i < nSize ; ++i)
//				{
//					T t;
//					Stream >> t;
//					Vector.PushTail(t);
//				}

				return (Stream);
			}

			// They are the same methods as the operators, but they can be used in the debugger!
			_INLINE_ T & At(UInt32 nElementIndex);
			_INLINE_ const T & At(UInt32 nElementIndex) const;

			// WARNING!! The following methods make insertion and are very costly!!
			// If you have to use them, you should considere to use QDT_SLIST or QDT_DLIST instead.
				_INLINE_ ITERATOR InsertOnce(const ITERATOR&	I, const T& N)
				{
					ITERATOR Cur = Find(GetHead(),N);

					if (Cur == GetTail())
					{
						Cur = Insert(I, N);
					}

					return (Cur);
				}

				_INLINE_ void RemoveIndex(UInt32 i);
				_INLINE_ void RemoveIndexFast(UInt32 i);
				_INLINE_ ITERATOR Remove(const ITERATOR & it)
				{
					QDT_ASSERT(it.GetVector() == this);
					RemoveIndex(it.GetIndex());
					return (it);
				}

				_INLINE_ UInt32 Remove(const T& t, bool bRemoveAll = false);
				_INLINE_ ITERATOR Insert(const ITERATOR & it, const T & t)
				{
					QDT_ASSERT(it.IsValid());

					if (GetSize() > 0)
					{
						PushTail(*GetLast());
						UInt32 i = _nSize-1;
						for (; i > it.GetIndex() ; --i)
						{
							_pArray[i] = _pArray[i - 1];
						}

						_pArray[i] = t;
					}
					else
					{
						PushTail(t);
						return (GetHead());
					}

					return (it);
				}

				_INLINE_ ITERATOR InsertSort(const T& Element, 
											bool bAllowDoublon, 
											bool& bFound)
				{
					ITERATOR		Cur = GetHead();
					ITERATOR		End = GetTail();

					bFound = false;

					while	((Cur != End) && (*Cur < Element))
					{
						++Cur;
					}

					if (Cur != End)
					{
						bFound = (*Cur == Element);

						if	((bAllowDoublon == false) && (bFound == true))
						{
							return (Cur);
						}
					}

					Cur = Insert(Cur, Element);
					return	(Cur);
				}

				// Sort for vector of integer or class 
				_INLINE_ void	Sort();
				_INLINE_ void	QuickSort(Int32 Left, Int32 Right);
				_INLINE_ Int32	Partition(Int32 Left, Int32 Right, Int32 Pivot);
				_INLINE_ void	Swap(Int32 X, Int32 Y);
				_INLINE_ void	Swap(QDT_VECTOR<T> & V);
				// sort for vector of class's pointers.
				_INLINE_ void	SortPointers();
				_INLINE_ void	QuickSortPointers(Int32 Left, Int32 Right);
				_INLINE_ Int32	PartitionPointers(Int32 Left, Int32 Right, Int32 Pivot);
				//

			class ITERATOR
			{
			public:

				_INLINE_ ITERATOR(QDT_VECTOR * pVector = NULL, UInt32 nIndex = 0)
				:
				_pVector(pVector),
				_nIndex(nIndex)
				{
				}

				_INLINE_ ~ITERATOR()
				{
					// Do nothing
				}

				// Operators
				// Prefer the use of pre-increment than post-increment for optimal performances
				ITERATOR& operator++()
				{
					++_nIndex;
					QDT_ASSERT(IsValid() && "You exceed the QDT_VECTOR bounds!");
					return	(*this);
				}
				
				ITERATOR operator++(int)
				{
					ITERATOR Tmp = *this;
					++(*this);
					return	(Tmp);
				}
				
				ITERATOR& operator--()
				{
					--_nIndex;
					QDT_ASSERT(IsValid() && "You exceed the QDT_VECTOR bounds!");
					return	(*this);
				}
				
				ITERATOR operator--(int)
				{
					ITERATOR Tmp = *this;
					--(*this);
					return	(Tmp);
				}

				ITERATOR operator+(int n) const
				{
					ITERATOR Tmp = *this;
					Tmp._nIndex += n;
					QDT_ASSERT(IsValid() && "You exceed the QDT_VECTOR bounds!");
					return	(Tmp);
				}
				
				ITERATOR& operator+=(int n)
				{
					_nIndex += n;
					QDT_ASSERT(IsValid() && "You exceed the QDT_VECTOR bounds!");
					return	(*this);
				}

				ITERATOR operator-(int n) const
				{
					ITERATOR Tmp = *this;
					Tmp._nIndex -= n;
					QDT_ASSERT(IsValid() && "You exceed the QDT_VECTOR bounds!");
					return	(Tmp);
				}
				
				ITERATOR& operator-=(int n)
				{
					_nIndex -= n;
					QDT_ASSERT(IsValid() && "You exceed the QDT_VECTOR bounds!");
					return	(*this);
				}
				T&	operator*() const
				{
					QDT_ASSERT(IsValid() && "You try to access a vector from an invalid iterator!");
					QDT_ASSERT(_pVector != NULL);
					return	(_pVector->_pArray[_nIndex]);
				}

				ITERATOR& operator=(const ITERATOR&	I)
				{
					_pVector = I._pVector;
					_nIndex = I._nIndex;
					return	(*this);
				}

				bool operator==(const ITERATOR&	I) const
				{
					QDT_ASSERT(IsValid() && I.IsValid() && "You can't compare an iterator which is invalid!");
					QDT_ASSERT(_pVector == I._pVector && "You can't compare two iterators which belong to a different vector!");
					if (_nIndex == I._nIndex) return (true);
					return	(false);
				}

				bool operator!=(const ITERATOR&	I) const
				{
					QDT_ASSERT(IsValid() && I.IsValid() && "You can't compare an iterator which is invalid!");
					QDT_ASSERT(_pVector == I._pVector && "You can't compare two iterators which belong to a different vector!");
					if (_nIndex != I._nIndex) return (true);
					return	(false);
				}

				bool	operator>(const ITERATOR&	I) const
				{
					QDT_ASSERT(IsValid() && I.IsValid() && "You can't compare an iterator which is invalid!");
					QDT_ASSERT(_pVector == I._pVector && "You can't compare two iterators which belong to a different vector!");
					return	(_nIndex > I._nIndex);
				}
				
				bool	operator<(const ITERATOR&	I) const
				{
					QDT_ASSERT(IsValid() && I.IsValid() && "You can't compare an iterator which is invalid!");
					QDT_ASSERT(_pVector == I._pVector && "You can't compare two iterators which belong to a different vector!");
					return	(_nIndex < I._nIndex);
				}

				bool	operator>=(const ITERATOR&	I) const
				{
					QDT_ASSERT(IsValid() && I.IsValid() && "You can't compare an iterator which is invalid!");
					QDT_ASSERT(_pVector == I._pVector && "You can't compare two iterators which belong to a different vector!");
					return	(_nIndex >= I._nIndex);
				}
				
				bool	operator<=(const ITERATOR&	I) const
				{
					QDT_ASSERT(IsValid() && I.IsValid() && "You can't compare an iterator which is invalid!");
					QDT_ASSERT(_pVector == I._pVector && "You can't compare two iterators which belong to a different vector!");
					return	(_nIndex <= I._nIndex);
				}

				// An iterator can be invalid when it's not correctly initialized or when
				// the index exceed the range of the vector (the tail is however considered
				// as valid)
				bool IsValid() const
				{
					if (_pVector && _nIndex <= _pVector->_nSize)
					{
						return (true);
					}

					return (false);
				}

				UInt32 GetIndex() const
				{
					return (_nIndex);
				}

				QDT_VECTOR * GetVector() const
				{
					return (_pVector);
				}

			private:

				QDT_VECTOR * _pVector;
				UInt32		 _nIndex;
			};

			friend class QDT_VECTOR_ALLOCATOR;

		private:

			T *						_pArray;
			UInt32					_nSize;
			UInt32					_nAllocatedSize;
			UInt16					_nFirstAlloc;
			UInt16					_nNextAlloc;
		};

		#include "QDT_Vector.inl"
	}
}

#endif // defined(PLATFORM_PC) || (!defined(QDT_MEM_INTERNAL))

#endif //__KCORE_TEMPLATE_QDT_VECTOR_H__

//=============================================================================
//	CODEENDS HERE
//=============================================================================
