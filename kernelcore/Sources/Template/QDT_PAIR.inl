//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Constructor
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class	T, class U >
QDT_PAIR<T, U>::QDT_PAIR()
{
	// Do nothing	
}

//-----------------------------------------------------------------------------
//	Name:		Constructor
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class	T, class U >
QDT_PAIR<T, U>::QDT_PAIR(const T& f, 
						 const U& s)
:
_First(f),
_Second(s)
{
	// Do nothing	
}

//-----------------------------------------------------------------------------
//	Name:		Constructor
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
QDT_PAIR<T, U>::QDT_PAIR(const QDT_PAIR<T, U> & Pair) 
:
_First(Pair._First),
_Second(Pair._Second)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Destructor
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
QDT_PAIR<T, U>::~QDT_PAIR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator =
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
QDT_PAIR<T, U>& QDT_PAIR<T, U>::operator=(const QDT_PAIR<T,U>& Pair)
{
	_First = Pair._First;
	_Second = Pair._Second;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator ==
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
bool QDT_PAIR<T, U>::operator==(const QDT_PAIR<T,U> & Pair) const
{
	return (_First == Pair._First && _Second == Pair._Second); 
}

//-----------------------------------------------------------------------------
//	Name:		operator !=
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
bool QDT_PAIR<T, U>::operator != (const QDT_PAIR<T,U> & Pair) const
{
	return (_First != Pair._First || _Second != Pair._Second); 
}

//-----------------------------------------------------------------------------
//	Name:		operator <
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
bool QDT_PAIR<T, U>::operator<(const QDT_PAIR<T, U>& Pair) const
{ 
	return (_First < Pair._First || (!(_First < Pair._First) && (_Second < Pair._Second))); 
}

//-----------------------------------------------------------------------------
//	Name:		GetFirst
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
const T & QDT_PAIR<T, U>::GetFirst() const
{
	return ( _First );
}

//-----------------------------------------------------------------------------
//	Name:		GetSecond
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
const U & QDT_PAIR<T, U>::GetSecond() const
{
	return ( _Second );
}

//-----------------------------------------------------------------------------
//	Name:		GetFirst
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
T& QDT_PAIR<T, U>::GetFirst()
{
	return ( _First );
}


//-----------------------------------------------------------------------------
//	Name:		GetSecond
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
U& QDT_PAIR<T, U>::GetSecond()
{
	return ( _Second );
}

//-----------------------------------------------------------------------------
//	Name:		SetFirst
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
void QDT_PAIR<T, U>::SetFirst(const T& f)
{
	_First = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetSecond
//	Object:		
//	02-05-23:	JLT - Created
//-----------------------------------------------------------------------------
template <class T, class U>
void QDT_PAIR<T, U>::SetSecond(const U& s)
{
	_Second = s;
}

//=============================================================================
//	CODEENDS HERE
//=============================================================================
