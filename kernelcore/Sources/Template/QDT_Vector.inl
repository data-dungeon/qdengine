//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR constructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::QDT_VECTOR()
:
_pArray(NULL),
_nSize(0),
_nAllocatedSize(0),
_nFirstAlloc(8),
_nNextAlloc(4)
{
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR constructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::QDT_VECTOR(UInt32 nSize, 
						  const T & initializationValue)
:
_pArray(NULL),
_nSize(0),
_nAllocatedSize(0),
_nFirstAlloc(4),
_nNextAlloc(4)
{
	_nSize = nSize;

	if (_nAllocatedSize < _nSize)
	{
		Reserve(_nSize);
	}

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		MEM::DebugNextPtrMutexLock();
		MEM::SetNextReturnPtr(&_pArray[i]);
		new T (initializationValue);
#else
		new (&_pArray[i]) T (initializationValue);
#endif
#else
		QDT_NEW_PLACED(&_pArray[i], T)(initializationValue);
#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR constructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::QDT_VECTOR(const QDT_VECTOR & vector)
:
_pArray(NULL),
_nSize(0),
_nAllocatedSize(0),
_nFirstAlloc(vector._nFirstAlloc),
_nNextAlloc(vector._nNextAlloc)
{
	PushTail(vector);
}

//-----------------------------------------------------------------------------
//	Name:		QDT_VECTOR destructor
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>::~QDT_VECTOR()
{
	Clear();

	if(_pArray)
	{
		delete [] (UInt8*)_pArray;
		_pArray = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetSize() const
{
	return (_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Clear()
{
	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		QDT_DELETE_PLACED(&_pArray[i], T);
	}

	_nSize = 0;
}

//-----------------------------------------------------------------------------
//	Name:		ClearAll
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::ClearAll()
{
	ITERATOR	It = GetHead();
	ITERATOR	ItEnd = GetTail();
	for (; It != ItEnd; ++It)
	{
		delete(*It);
	}

	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PushTail(const	T& t)
{
	if (_nSize >= _nAllocatedSize)
	{
		if (_nSize)
		{
			// Patch to avoid mass reallocate of QDT_VECTOR (4 elements by 4) => we reallocate 20% of the precedent allocated size
			UInt32 nNextAlloc = UInt32(0.2f * _nAllocatedSize); 
			if (nNextAlloc == 0) nNextAlloc = _nNextAlloc;
			Reserve(_nAllocatedSize + nNextAlloc);			
		}
		else
		{
			Reserve(_nAllocatedSize + _nFirstAlloc);
		}
	}

#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		MEM::DebugNextPtrMutexLock();
		MEM::SetNextReturnPtr(&_pArray[_nSize++]);
		new T(t);
#else
		new (&_pArray[_nSize++]) T(t);
#endif
#else
		QDT_NEW_PLACED(&_pArray[_nSize++], T)(t);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PushTail(const QDT_VECTOR& v)
{
	unsigned int nOldSize = _nSize;
	unsigned int nSumSize = _nSize + v._nSize;

	if (_nSize + v._nSize > _nAllocatedSize)
	{
		Reserve(_nSize + v._nSize);
	}

	for (unsigned int i = nOldSize ; i < nSumSize ; ++i)
	{
#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		MEM::DebugNextPtrMutexLock();
		MEM::SetNextReturnPtr(&_pArray[i]);
		new T (v._pArray[i-nOldSize]);
#else
		new (&_pArray[i]) T (v._pArray[i-nOldSize]);
#endif
#else
		QDT_NEW_PLACED(&_pArray[i], T)(v._pArray[i-nOldSize]);
#endif
	}

	_nSize += v._nSize;
}

//-----------------------------------------------------------------------------
//	Name:		PushTail
//	Object:		
//	05-08-22:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PushTail(const T & t, UInt32 nTimes)
{
	if (_nSize + nTimes > _nAllocatedSize)
	{
		Reserve(_nSize + nTimes);
	}

	UInt32 nCount =  _nSize + nTimes;
	for (UInt32 i = _nSize ; i < nCount ; ++i)
	{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		MEM::DebugNextPtrMutexLock();
		MEM::SetNextReturnPtr(&_pArray[i]);
		new T (t);
#else
		new (&_pArray[i]) T (t);
#endif
#else
		QDT_NEW_PLACED(&_pArray[i], T)(t);
#endif
	}
	
	_nSize += nTimes;
}

//-----------------------------------------------------------------------------
//	Name:		PopTail
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::PopTail()
{
	QDT_DELETE_PLACED(&_pArray[_nSize-1], T);
	_nSize--;
}

//-----------------------------------------------------------------------------
//	Name:		IsExist
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::IsExist(const T& t) const
{
	return (Find(GetHead(), t) != GetTail());
}

//-----------------------------------------------------------------------------
//	Name:		Reserve
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		T * pTempArray;

#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
		pTempArray = (T *) new UInt8 [nReserveSize * sizeof(T)];
		//pTempArray = (T *) MEM::GetInstance()->AllocateAlign(__alignof(T), nReserveSize * sizeof(T));
#else
		/*if(__alignof__(T) > 16)
		{
			pTempArray = (T *) MEM::GetInstance()->AllocateAlign(__alignof__(T), nReserveSize * sizeof(T));
		}
		else*/
		{
			pTempArray = (T *) new UInt8 [nReserveSize * sizeof(T)];
		} 
#endif		

		if (_pArray)
		{
			for (unsigned int i = 0 ; i < _nSize ; ++i)
			{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
				MEM::DebugNextPtrMutexLock();
				MEM::SetNextReturnPtr(&pTempArray[i]);
				new T (_pArray[i]);
#else
				new (&pTempArray[i]) T(_pArray[i]);
#endif		
#else	
				QDT_NEW_PLACED(&pTempArray[i], T)(_pArray[i]);
#endif
	
				QDT_DELETE_PLACED(&_pArray[i], T);
			}

			delete [] (UInt8 *)_pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Resize
//	Object:	Use resize on simple object, not tested yet.
//					Tests must be run on PS3, placement new problem with templates + 
//					QDT_NEW_PLACED (Visual C++ 7.0 )
//	01-02-16:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Resize(UInt32 uNewSize)
{
	UInt32 uActualSize = GetSize();
	if ( uNewSize < uActualSize ) 
	{
		for (UInt32 u = (uActualSize-1); u > uNewSize; u++)
		{
			T* ptr = &(_pArray[u]);
			QDT_DELETE_PLACED(ptr,T);
		}
	}
	else
	{
		Reserve(uNewSize);
		for (UInt32 u = (uActualSize); u < uNewSize; u++)
		{
#if defined (PLATFORM_PC) || defined (PLATFORM_X360)
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			MEM::DebugNextPtrMutexLock();
			MEM::SetNextReturnPtr(&_pArray[u]);
			new T;
#else
			new (&_pArray[u]) T;
#endif		
#else
			QDT_NEW_PLACED(&_pArray[u], T);
#endif
		}
  }
	_nSize = uNewSize;
}

//-----------------------------------------------------------------------------
//	Name:		GetCapacity
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetCapacity() const
{
	return (_nAllocatedSize);
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::IsEmpty() const
{
	return (_nSize == 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
T & QDT_VECTOR<T>::operator[](UInt32 nElementIndex)
{
	QDT_ASSERT(nElementIndex < _nAllocatedSize);
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
const T & QDT_VECTOR<T>::operator[](UInt32 nElementIndex) const
{
	QDT_ASSERT(nElementIndex < _nAllocatedSize);
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>& QDT_VECTOR<T>::operator+=(const T& t)
{
	PushTail(t);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>& QDT_VECTOR<T>::operator+=(const QDT_VECTOR<T>& v)
{
	PushTail(v);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
QDT_VECTOR<T>& QDT_VECTOR<T>::operator=(const QDT_VECTOR<T>& v)
{
	Clear();
	PushTail(v);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::operator==(const QDT_VECTOR& v) const
{
	if (_nSize != v._nSize)
	{
		return (false);
	}

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		if (_pArray[i] != v._pArray[i])
		{
			return (false);
		}
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
bool QDT_VECTOR<T>::operator!=(const QDT_VECTOR& v) const
{
	if (_nSize != v._nSize)
	{
		return (true);
	}

	for (UInt32 i = 0 ; i < _nSize ; ++i)
	{
		if (_pArray[i] != v._pArray[i])
		{
			return (true);
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		At
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
T & QDT_VECTOR<T>::At(UInt32 nElementIndex)
{
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		At
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
const T & QDT_VECTOR<T>::At(UInt32 nElementIndex) const
{
	return (_pArray[nElementIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIndex
//	Object:		Note GRI : Attention , on n'appelle pas le destructor ???
//				Note RMA : Fixed
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::RemoveIndex(UInt32 i)
{
	QDT_ASSERT(i < GetSize());

	for (UInt32 index = i ; index < GetSize() - 1 ; ++index)
	{
		_pArray[index] = _pArray[index+1];
	}

	QDT_DELETE_PLACED(&_pArray[GetSize() - 1], T);
	--_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIndexFast ( fast but Sort Not Stable ) 
//	Object:	Complexity of one but be carreful than the inner order change.
//			!! change vector element order !!	
//			Note that the element that replace the current one have a higher index
//			of the current be erase. So you can do  : 
//
//			/* Int32 nSize = _vector.GetSize();*/ do NOT precompute SIZE as it changed
//			for(index = 0;index < _vector.GetSize();)
//			{
//				if ( _vector[index] == DELETE_ME )
//				{
//					_vector.RemoveIndexFast(index);
//					/* index++*/  DON'T INCREMENT
//				}
//				else
//				{
//					index++;	// do increment !! 
//				}
//			}
//					
//				
//	06-01-31:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::RemoveIndexFast(UInt32 u)
{
	QDT_ASSERT(u < _nSize);
	QDT_DELETE_PLACED(&_pArray[u], T);
	_pArray[u] = _pArray[_nSize-1];
	--_nSize;
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32	QDT_VECTOR<T>::Remove(const T&	t,
							  bool bRemoveAll)
{
	UInt32 nbr = 0;
	ITERATOR	Cur = GetHead();

	while (Cur != GetTail())
	{
		if ((*Cur) == t)
		{
			Cur = Remove(Cur);
			++nbr;
			if ((bRemoveAll == false) || (Cur == GetTail()))
			{
				break;				
			}
		}
		else
		{
			++Cur;
		}
	}

	return (nbr);
}

//-----------------------------------------------------------------------------
//	Name:		Remove
//	Object:		
//	05-08-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt32 QDT_VECTOR<T>::GetIndex(const ITERATOR& I) const
{
	return (I.GetIndex());
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstAlloc
//	Object:		
//	05-09-26:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt16 QDT_VECTOR<T>::GetFirstAlloc() const
{
	return (_nFirstAlloc);
}

//-----------------------------------------------------------------------------
//	Name:		GetNextAlloc
//	Object:		
//	05-09-26:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
UInt16 QDT_VECTOR<T>::GetNextAlloc() const
{
	return (_nNextAlloc);
}

//-----------------------------------------------------------------------------
//	Name:		SetFirstAlloc
//	Object:		
//	05-09-26:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::SetFirstAlloc(UInt16 n)
{
	_nFirstAlloc = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetNextAlloc
//	Object:		
//	05-09-26:	RMA - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::SetNextAlloc(UInt16 n)
{
	_nNextAlloc = n;
}

//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:		Does not work with a vector of pointers !	
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Sort()
{
	Int32	nSize = GetSize();

	if	(nSize == 2)
	{
		if	(_pArray[1] < _pArray[0])
		{
			Swap(0, 1);
		}
	}
	else
	{
		if	(nSize > 2)
		{
			QuickSort(0, nSize - 1);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		QuickSort
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::QuickSort(Int32	Left,
							  Int32	Right)
{
	if	(Right > Left)
	{
		Int32	Pivot = Left + ((Right - Left) / 2);
		Int32	NewPivot = Partition(Left, Right, Pivot);
		QuickSort(Left, NewPivot - 1);
		QuickSort(NewPivot + 1, Right);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Partition
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
Int32 QDT_VECTOR<T>::Partition(Int32	Left,
							   Int32	Right,
							   Int32	Pivot)
{
	T	PivotValue = _pArray[Pivot];
	Swap(Pivot, Right);					// Move pivot to end
	Int32	i, StoreIndex = Left;
	for	(i = Left ; i < Right ; ++i)
	{
		if	(_pArray[i] < PivotValue)
		{
			Swap(StoreIndex, i);
			StoreIndex++;
		}
	}
	Swap(Right, StoreIndex);			// Move pivot to its final place
	return	(StoreIndex);
}


//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:	Use it when you got a vector of pointers.	
//	06-04-20:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::SortPointers()
{
	Int32	nSize = GetSize();
	if	(nSize == 2)
	{
		if	( *(_pArray[1]) < *(_pArray[0]))
		{
			Swap(0, 1);
		}
	}
	else
	{
		if	(nSize > 2)
		{
			QuickSortPointers(0, nSize - 1);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		QuickSortPointers
//	Object:		
//	06-01-26:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::QuickSortPointers(Int32	Left,
									  Int32	Right)
{
	if	(Right > Left)
	{
		Int32	Pivot = Left + ((Right - Left) / 2);
		Int32	NewPivot = PartitionPointers(Left, Right, Pivot);
		QuickSortPointers(Left, NewPivot - 1);
		QuickSortPointers(NewPivot + 1, Right);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Partition
//	Object:		
//	06-01-26:	GRI - Created
//-----------------------------------------------------------------------------
template <class T>
Int32 QDT_VECTOR<T>::PartitionPointers(Int32	Left,
									   Int32	Right,
									   Int32	Pivot)
{
	T	PivotValue = _pArray[Pivot];
	Swap(Pivot, Right);					// Move pivot to end
	Int32	i, StoreIndex = Left;
	for	(i = Left ; i < Right ; ++i)
	{
		if	( (*_pArray[i]) < (*PivotValue) )
		{
			Swap(StoreIndex, i);
			StoreIndex++;
		}
	}
	Swap(Right, StoreIndex);			// Move pivot to its final place
	return	(StoreIndex);
}

//-----------------------------------------------------------------------------
//	Name:		Swap
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
template <class T>
void QDT_VECTOR<T>::Swap(Int32	X,
						 Int32	Y)
{
	T	Temp = _pArray[X];
	_pArray[X] = _pArray[Y];
	_pArray[Y] = Temp;
}

//-----------------------------------------------------------------------------
//	Name:		Swap
//	Object:		swap attributes between 2 vectors (avoid unuseful and expensive copy)
//	06-02-08:	SBE - Created
//-----------------------------------------------------------------------------
template <class T>
void	QDT_VECTOR<T>::Swap(QDT_VECTOR<T> &	V)
{
	T *		pArray			= _pArray;
	UInt32	nSize			= _nSize;
	UInt32	nAllocatedSize	= _nAllocatedSize;
	UInt16	nFirstAlloc		= _nFirstAlloc;
	UInt16	nNextAlloc		= _nNextAlloc;

	_pArray			= V._pArray;
	_nSize			= V._nSize;
	_nAllocatedSize = V._nAllocatedSize;
	_nFirstAlloc	= V._nFirstAlloc;
	_nNextAlloc		= V._nNextAlloc;

	V._pArray			= pArray;
	V._nSize			= nSize;
	V._nAllocatedSize	= nAllocatedSize;
	V._nFirstAlloc		= nFirstAlloc;
	V._nNextAlloc		= nNextAlloc;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
