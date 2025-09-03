//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QDT_DLIST Constructor
//	Object:		
//	05-08-17:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_DLIST<T>::QDT_DLIST()
:
_pFirst(NULL),
_pLast(NULL),
_nSize(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		QDT_DLIST Constructor
//	Object:		
//	05-08-17:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_DLIST<T>::QDT_DLIST(const QDT_DLIST & List)
:
_pFirst(NULL),
_pLast(NULL),
_nSize(NULL)
{
	PushTail(List);
}

//-----------------------------------------------------------------------------
//	Name:		QDT_DLIST Destructor
//	Object:		
//	05-08-17:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_DLIST<T>::~QDT_DLIST()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	05-08-17:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_DLIST<T>::GetSize() const
{
	return (_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-17:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::Clear()
{
	NODE * pNode = _pFirst;
	while (pNode)
	{
		NODE * pNext = pNode->GetNext();
		delete pNode;
		pNode = pNext;
	}

	_pFirst = NULL;
	_pLast  = NULL;
	_nSize = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-17:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::ClearAll()
{
	NODE * pNode = _pFirst;
	while (pNode)
	{
		NODE * pNext = pNode->GetNext();
		delete pNode->GetData();
		delete pNode;
		pNode = pNext;
	}

	_pFirst = NULL;
	_pLast  = NULL;
	_nSize = 0;
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::PushTail(const	T&	t)
{
	NODE * pNode = new NODE(t);
	if (_pFirst == NULL)
	{
		_pFirst = pNode;
		_pLast = pNode;
	}
	else
	{
		_pLast->SetNext(pNode);
		pNode->SetPrev(_pLast);
		_pLast = pNode;
	}

	++_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::PushTail(const QDT_DLIST& List)
{
	ITERATOR it = List.GetHead();
	ITERATOR tail = List.GetTail();

	while (it != tail)
	{
		PushTail(*it);
		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PopTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::PopTail()
{
	QDT_ASSERT(IsEmpty() == false && "Can't pop the last element of an empty list!");

	NODE * pNode = _pLast;
	_pLast = pNode->GetPrev();
	delete pNode;
	--_nSize;

	if (_pLast == NULL)
	{
		_pFirst = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_DLIST<T>::IsEmpty() const
{
	return (GetSize() == 0);
}

//-----------------------------------------------------------------------------
//	Name:		InsertHead
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::InsertHead(const T & t)
{
	NODE * pNode = new NODE(t);
	if (_pFirst == NULL)
	{
		_pFirst = pNode;
		_pLast = pNode;
	}
	else
	{
		pNode->SetNext(_pFirst);
		_pFirst->SetPrev(pNode);
		_pFirst = pNode;
	}

	++_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		InsertHead
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::InsertHead(QDT_DLIST& List)
{
	if (List.GetSize() > 0)
	{
		ITERATOR it = List.GetLast();
		ITERATOR head = List.GetHead();

		while (it != head)
		{
			InsertHead(*it);
			--it;
		}

		InsertHead(*it);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Reverse
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::Reverse()
{
	NODE * pNode = _pFirst;
	while (pNode)
	{
		// Swap element
		NODE * pTemp = pNode->GetPrev();
		pNode->SetPrev(pNode->GetNext());
		pNode->SetNext(pTemp);

		pNode = pNode->GetPrev();
	}

	_pFirst = _pLast;
	_pLast = _pFirst;
}

//-----------------------------------------------------------------------------
//	Name:		RemoveHead
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::RemoveHead()
{
	QDT_ASSERT(GetSize() > 0);
	NODE * pNodeToRemove = _pFirst;
	_pFirst = _pFirst->GetNext();
	
	if (_pFirst)
	{
		_pFirst->SetPrev(NULL);
	}

	--_nSize;

	delete pNodeToRemove;
}

//-----------------------------------------------------------------------------
//	Name:		InsertTail
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::InsertTail(const T& t)
{
	PushTail(t);
}

//-----------------------------------------------------------------------------
//	Name:		InsertTail
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::InsertTail(const QDT_DLIST& List)
{
	PushTail(List);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_DLIST<T>& QDT_DLIST<T>::operator+=(const T&	t)
{
	PushTail(t);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_DLIST<T>& QDT_DLIST<T>::operator+=(const QDT_DLIST<T>& List)
{
	PushTail(List);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------	
template <class T>
QDT_DLIST<T>& QDT_DLIST<T>::operator=(const QDT_DLIST<T>& List)
{
	Clear();
	PushTail(List);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		InsertSort
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------	
template <class T>
bool QDT_DLIST<T>::InsertSort(const QDT_DLIST&	QD,
							  bool				bAllowDup)
{
	ITERATOR		Cur		= QD.GetHead();
	ITERATOR		End		= QD.GetTail();
	bool			bIsIn	= false;
	bool			bTmp;

	for ( ; Cur != End; ++Cur)
	{
		InsertSort(*Cur, bAllowDup, bTmp);
		bIsIn |= bTmp;
	}

	return ( bIsIn );
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------	
template <class T>
bool QDT_DLIST<T>::Remove(const T& Elt)
{
	ITERATOR it;
	it = Find(GetHead(), Elt);
	if (it!= GetTail())
	{
		Remove(it);
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------	
template <class T>
void QDT_DLIST<T>::Remove(const ITERATOR& I)
{
	QDT_ASSERT(I.GetNode());

	if (I.GetNode()->GetPrev())
	{
		I.GetNode()->GetPrev()->SetNext(I.GetNode()->GetNext());
	}

	if (I.GetNode()->GetNext())
	{
		I.GetNode()->GetNext()->SetPrev(I.GetNode()->GetPrev());
	}

	if (I.GetNode() == _pFirst)
	{
		_pFirst = _pFirst->GetNext();
	}

	if (I.GetNode() == _pLast)
	{
		_pLast = _pLast->GetPrev();
	}

	delete I.GetNode();
	--_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-13:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_DLIST<T>::operator == (const QDT_DLIST<T>& List) const
{
	if (GetSize() != List.GetSize())
	{
		return (false);
	}

	ITERATOR it1 = GetHead();
	ITERATOR it2 = List.GetHead();
	ITERATOR itTail = GetTail();

	while (it1 != itTail)
	{
		if ((*it1 != *it2))
		{
			return (false);
		}

		++it1;
		++it2;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-14:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::Push(const T & Elt, 
						bool bReverse)
{
	if (bReverse)
	{
		InsertHead(Elt);
	}
	else
	{
		PushTail(Elt);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-09-14:	RMA - Created
//				This is a common bubble sort... 
//				I suppose it can be greatly optimized!
//-----------------------------------------------------------------------------
template <class T>
void QDT_DLIST<T>::Sort()
{
	if (GetSize() > 2)
	{
		ITERATOR itX = GetHead();
		ITERATOR itY = itX;
		++itY;

		ITERATOR itTail = GetTail();
		ITERATOR itTailMinusOne = itTail;
		--itTailMinusOne;

		while (itX != itTail)
		{
			while (itY != itTailMinusOne)
			{
				NODE * pNodeY = itY.GetNode();
				NODE * pNodeYPlusOne = itY.GetNode()->GetNext();

				if (pNodeYPlusOne->GetData() < pNodeY->GetData())
				{
					T holder = pNodeYPlusOne->GetData();	
					pNodeYPlusOne->SetData(pNodeY->GetData());
					pNodeY->SetData(holder);
				}
				
				++itY;
			}
	
			++itX;
		}
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
