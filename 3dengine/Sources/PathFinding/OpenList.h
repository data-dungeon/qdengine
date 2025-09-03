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
//	CLASS:	OPEN_LIST
//	The OPEN_LIST class implements ...
//
//	02-04-08:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

template <class T>
class	QDT_MIN_HEAP
{
public:

	/** Constructor & destructor */
	QDT_MIN_HEAP()
	{
		// Do nothing
	}
								
	~QDT_MIN_HEAP()
	{
		_vElts.Clear();
	}	

	/** Reset the heap */
	void	Reset()
	{
		_vElts.Clear();
	}

	/** Insert a element in the heap */
	void	Insert(const T & Elt)
	{
		int nNumElt = _vElts.GetSize();
		
		_vElts.PushTail(Elt);
		
		while ((nNumElt != 0) && (_vElts[nNumElt] < _vElts[GetParent(nNumElt)]))
		{
			swap (_vElts[nNumElt], _vElts[GetParent(nNumElt)]);
			nNumElt = GetParent(nNumElt);
		}
	}

	/** Remove the min */
	T		RemoveMin()
	{
		QDT_ASSERT(!IsEmpty());
		
		T Min = *_vElts.GetHead();
		
		swap(_vElts[0], *_vElts.GetLast());
		
		_vElts.PopTail();

		if ((_vElts.IsEmpty()) || (_vElts.GetSize() == 1)) return (Min);	

		if (_vElts.GetSize() == 2)
		{
			if (_vElts[0] > _vElts[1])
			{
				swap(_vElts[0], _vElts[1]);
			}
			return (Min);
		}

		unsigned int nNumElt = 0;
		
		while ((nNumElt < _vElts.GetSize()) && ((_vElts[nNumElt] > _vElts[GetLeftChild(nNumElt)]) || (_vElts[nNumElt] > _vElts[GetRightChild(nNumElt)])))
		{		
			if (_vElts[GetLeftChild(nNumElt)] < _vElts[GetRightChild(nNumElt)])
			{
				swap(_vElts[nNumElt], _vElts[GetLeftChild(nNumElt)]);
				nNumElt = GetLeftChild(nNumElt);
			}
			else
			{
				swap(_vElts[nNumElt], _vElts[GetRightChild(nNumElt)]);
				nNumElt = GetRightChild(nNumElt);
			}

			if (GetLeftChild(nNumElt) >= _vElts.GetSize()) break;

			if (GetRightChild(nNumElt) >= _vElts.GetSize())
			{
				if (_vElts[nNumElt] >  _vElts[GetLeftChild(nNumElt)])
				{
					swap(_vElts[nNumElt], _vElts[GetLeftChild(nNumElt)]);
				}
				break;
			}
		}
		
		return (Min);
	}

	/** Size */
	int		GetSize() const
	{
		return (_vElts.GetSize());
	}

	/** Is empty ? */
	bool	IsEmpty() const
	{
		return (_vElts.IsEmpty());
	}

	/** Reservation */
	void	Reserve(int nNbrElts)
	{
		_vElts.Reserve(nNbrElts);
	}

private:

	unsigned int		GetLeftChild(unsigned int nNumElt) const	{	return ((nNumElt << 1) + 1); }
	unsigned int		GetRightChild(unsigned int nNumElt) const	{	return ((nNumElt << 1) + 2); }
	unsigned int		GetParent(unsigned int nNumElt) const		{	return ((nNumElt - 1) >> 1); }

	QDT_VECTOR <T>	_vElts;
};

class	OPEN_LIST
{
public:
									OPEN_LIST();
								   ~OPEN_LIST();

	void							Clear();
	PATH_NODE*						Insert(const PATH_NODE & PathNode);
	int								GetSize();
	bool							IsEmpty();
	PATH_NODE*						RemoveMin();
	
private:

	enum
	{
		MAX_PATH_NODE = 16000,
	};

	QDT_MIN_HEAP <PATH_NODE_PTR>	_mhOpenList;
	int								_nNbrPathNode;
	PATH_NODE						_tPathNode[MAX_PATH_NODE];
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
