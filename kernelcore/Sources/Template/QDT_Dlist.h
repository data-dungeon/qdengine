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
//	CLASS:	QDT_list
//
//	05-08-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TEMPLATE_QDT_DLIST_H__
#define __KCORE_TEMPLATE_QDT_DLIST_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		template <class T>
		class QDT_DLIST
		{
		public:

			class ITERATOR;

		private:

			class NODE;

		public:

			_INLINE_ QDT_DLIST();
			_INLINE_ QDT_DLIST(const QDT_DLIST & List);
			_INLINE_ ~QDT_DLIST();

			// Get the number of element pushed in the list
			_INLINE_ UInt32 GetSize() const;

			// This method clear the list but not release memory!!
			_INLINE_ void Clear();

			// This method is reserved for pointer pushed in the list
			// It call delete on all pointer before 
			_INLINE_ void ClearAll();

			// Get an iterator to the first element
			_INLINE_ ITERATOR GetHead() const
			{
				return (ITERATOR(const_cast<QDT_DLIST<T>*>(this), _pFirst));
			}

			// Get an iterator to the last element
			_INLINE_ ITERATOR GetLast() const
			{
				return (ITERATOR(const_cast<QDT_DLIST<T>*>(this), _pLast));
			}
			
			// Get an iterator to the first element after the last one!
			_INLINE_ ITERATOR GetTail() const
			{
				return (ITERATOR(const_cast<QDT_DLIST<T>*>(this), NULL));
			}

			// Push an element at the end of the list
			_INLINE_ void PushTail(const T&	t);
			
			// Push a list at the end of the list
			_INLINE_ void PushTail(const QDT_DLIST& List);

			// Pop the last element of the list
			_INLINE_ void PopTail();

			//	Retourne true si la list est vide
			_INLINE_ Bool IsEmpty() const;

			// Push an element at the end of the list
			_INLINE_ void InsertTail(const T& t);

			// Push an element at the end of the list
			_INLINE_ void InsertTail(const QDT_DLIST& List);

			// Insert un element en tête de liste
			_INLINE_ void InsertHead(const T & N);

			// Insert une liste en tête de liste
			_INLINE_ void InsertHead(QDT_DLIST& List);

			// Retire l'element en tête de liste
			_INLINE_ void RemoveHead();

			// Insère un element en queue ou en tete de liste
			_INLINE_ void Push(const T & Elt, Bool bReverse);

			// Search for N in the list after the given Iterator
			_INLINE_ ITERATOR FindRef(const ITERATOR& I, const T& t)
			{
				ITERATOR	Cur = I;
				ITERATOR	End = GetTail();

				while(Cur != End)
				{
					if (*(*Cur) == *t)
					{
						break;
					}

					++Cur;
				}
				return (Cur);
			}

			// Inverse les elements de la liste
			_INLINE_ void Reverse();

			// Insère un élément avant l'iterateur donné
			_INLINE_ ITERATOR Insert(const ITERATOR& I, const T& t)
			{
				if (I.GetNode() == NULL) // Tail!
				{
					PushTail(t);
					return (GetLast());
				}

				NODE * pNode = new NODE(t);

				NODE * pAfter = I.GetNode();
				NODE * pBefore = I.GetNode()->GetPrev();

				if (pBefore)
				{
					pBefore->SetNext(pNode);
				}
				else
				{
					_pFirst = pNode;
				}

				pAfter->SetPrev(pNode);

				pNode->SetPrev(pBefore);
				pNode->SetNext(pAfter);

				++_nSize;

				return (ITERATOR(this, pNode));
			}

			// Insère une liste avant l'iterateur donné
			_INLINE_ ITERATOR Insert(const ITERATOR& I, const QDT_DLIST& List)
			{
				// Note : this can be greatly optimized!!!

				if (I.GetNode() == NULL) // Tail!
				{
					PushTail(List);
					return (I);
				}

				ITERATOR it = List.GetHead();
				ITERATOR tail = List.GetTail();

				while (it != tail)
				{
					Insert(I, *it);
					++it;
				}

				return (I);
			}

			// Find an element in the list! return tail if the element has not been found
			_INLINE_ ITERATOR Find(const ITERATOR& Start, const T& Elt) const
			{
				ITERATOR Cur = Start;
				ITERATOR End = GetTail();

				while (Cur != End)
				{
					if ((*Cur) == Elt)
					{
						break;	
					}

					++Cur;
				}

				return (Cur);
			}

			// Insère un élément avant l'iterateur donné, en assurant qu'il n'y a pas de doublons
			_INLINE_ ITERATOR InsertOnce(const ITERATOR& I, const T& Elt)
			{
				ITERATOR	Cur = Find(GetHead(), Elt);

				if (Cur == GetTail())
				{
					Cur = Insert(I, Elt);
				}

				return (Cur);
			}

			// Insère un élément dans la liste en le triant (necessite operateur < et == définit pour la classe template)
			_INLINE_ ITERATOR InsertSort(const T& N, 
				Bool  bAllowDup, 
				Bool& bIsIn)
			{
				ITERATOR		Cur = GetHead();
				ITERATOR		End = GetTail();

				bIsIn = false;

				while ( (Cur != End) && (*Cur < N) )
				{
					++Cur;
				}

				if (Cur != End)
				{
					bIsIn = (*Cur == N);

					if	( (bAllowDup == false) && (bIsIn == true) )
					{
						return ( Cur );
					}
				}

				return (Insert(Cur, N));
			}

			// Insert une liste en triant entre eux les deux listes!
			_INLINE_ Bool InsertSort(const QDT_DLIST& QD, Bool bAllowDup);

			// Retire un element de la liste
			_INLINE_ Bool Remove(const T& Elt);
			_INLINE_ void Remove(const ITERATOR	& I);

			// Sort
			_INLINE_ void Sort();

			// Operators
			_INLINE_ QDT_DLIST& operator+=(const	T&	t);
			_INLINE_ QDT_DLIST& operator+=(const QDT_DLIST<T>& List);
			_INLINE_ QDT_DLIST& operator=(const QDT_DLIST<T>& List);
			_INLINE_ Bool operator == (const QDT_DLIST<T>& List) const;

			class ITERATOR
			{
			public:

				_INLINE_ ITERATOR(QDT_DLIST * pList = NULL, NODE * pNode = NULL)
				:
				_pNode(pNode),
				_pList(pList)
				{
					// Do nothing
				}

				// Operators
				// Prefer the use of pre-increment than post-increment for optimal performances
				_INLINE_ ITERATOR& operator++()
				{
					QDT_ASSERT(_pNode != NULL && "You exceed the QDT_DLIST bous!");
					_pNode = _pNode->GetNext();
					return	(*this);
				}
				
				_INLINE_ ITERATOR operator++(int)
				{
					ITERATOR Tmp = *this;
					++(*this);
					return	(Tmp);
				}

				_INLINE_ ITERATOR operator+(int n) const
				{
					ITERATOR Tmp = *this;
					int i = 0;
					while (i < n)
					{
						++Tmp;
						++i;
					}
					return	(Tmp);
				}
				
				_INLINE_ ITERATOR& operator+=(int n)
				{
					int i = 0;
					while (i < n)
					{
						++(*this);
						++i;
					}
					return	(*this);
				}
				
				_INLINE_ ITERATOR& operator--()
				{
					QDT_ASSERT(_pNode != _pList->GetHead().GetNode());

					if	(_pNode)
					{
						_pNode = _pNode->GetPrev();
						return	(*this);
					}
						
					_pNode = _pList->GetLast().GetNode();
					return	(*this);
				}
		
				_INLINE_ ITERATOR operator--(int)
				{
					ITERATOR Tmp = *this;
					--(*this);
					return	(Tmp);
				}

				_INLINE_ ITERATOR operator-(int n) const
				{
					ITERATOR Tmp = *this;
					int i = 0;
					while (i < n)
					{
						--Tmp;
						++i;
					}
					return	(Tmp);
				}
				
				_INLINE_ ITERATOR& operator-=(int n)
				{
					int i = 0;
					while (i < n)
					{
						--(*this);
						++i;
					}
					return	(*this);
				}

				ITERATOR& operator = (const ITERATOR & I)
				{
					_pNode = I._pNode;
					_pList = I._pList;
					return	(*this);
				}

				T&	operator*() const
				{
					QDT_ASSERT(_pNode && "You try to access a QDT_DLIST from an invalid iterator!");
					return	(_pNode->GetData());
				}

				Bool operator!=(const ITERATOR&	I) const
				{
					QDT_ASSERT(_pList == I._pList && "You can't compare two iterators which belong to a different list!");
					return (_pNode != I._pNode);
				}

				Bool operator==(const ITERATOR&	I) const
				{
					QDT_ASSERT(_pList == I._pList && "You can't compare two iterators which belong to a different list!");
					return (_pNode == I._pNode);
				}

				NODE * GetNode() const
				{
					return (_pNode);
				}

			private:

				NODE *		_pNode;
				QDT_DLIST *	_pList;
			};

		private:

			class NODE
			{
			public:

				_INLINE_ NODE(const T & data)
				{
					_pPrev = NULL;
					_pNext = NULL;
					_Data = data;
				};

				_INLINE_ void SetPrev(NODE * pPrev)		{ _pPrev = pPrev; }
				_INLINE_ void SetNext(NODE * pNext)		{ _pNext = pNext; }
				_INLINE_ void SetData(const T & data)	{ _Data = data; }
				_INLINE_ NODE * GetNext() const			{ return (_pNext); }
				_INLINE_ NODE * GetPrev() const			{ return (_pPrev); }
				_INLINE_ T &	GetData()				{ return (_Data); }

			private:

				T	   _Data;
				NODE * _pPrev;
				NODE * _pNext;
			};

			NODE *  _pFirst;
			NODE *  _pLast;
			UInt32  _nSize;
		};

		#include "QDT_DLIST.inl"
	}
}

#endif //__KCORE_TEMPLATE_QDT_DLIST_H__

//=============================================================================
//	CODEENDS HERE
//=============================================================================
