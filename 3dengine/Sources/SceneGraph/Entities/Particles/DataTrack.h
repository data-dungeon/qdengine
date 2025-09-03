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
// CLASS: DATA_TRACK
//
//
//00-12-14 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: DATA_TRACK
//00-12-14 JVA Created: 
//=============================================================================

template <class T> class DATA_TRACK 
{
public:
	
	DATA_TRACK()
	:
	_nNbKeys		(0),
	_pKeysTab		(NULL),
	_pTimeKeysTab	(NULL)
	{

	}

	~DATA_TRACK()
	{
		if(_nNbKeys >= 0)
		{
			delete [] _pKeysTab;
			delete [] _pTimeKeysTab;
		}
	}

	void	Copy(const DATA_TRACK<T> & DataTrack)
	{
		int nKeyIndex, nKeyCount;

		nKeyCount = DataTrack.GetNbKeys();

		delete[] _pKeysTab;
		delete[] _pTimeKeysTab;

		_pKeysTab		= new T[nKeyCount];
		_pTimeKeysTab	= new float[nKeyCount];

		for ( nKeyIndex = 0; nKeyIndex < nKeyCount; ++nKeyIndex )
		{
			_pKeysTab[ nKeyIndex ]		= DataTrack._pKeysTab[ nKeyIndex ];
			_pTimeKeysTab[ nKeyIndex ]	= DataTrack._pTimeKeysTab[ nKeyIndex ];
		}
	}

	DATA_TRACK<T> &	operator = (const DATA_TRACK<T> & DataTrack)
	{
		if ( this != &DataTrack )
		{
			Copy( DataTrack );
		}

		return ( *this );
	}

	void	Build(int NbKeys)
	{
		T		*pTab;
		int		MinMax;
		int		i;
		
		QDT_ASSERT(NbKeys > 0);

		pTab = new T[NbKeys];

		if(_nNbKeys > 0)
		{
			MinMax = MATH::Min(NbKeys, _nNbKeys);

			for(i = 0;  i < MinMax; ++i)
			{
				pTab[i] = _pKeysTab[i]; 
			}

			delete [] _pKeysTab;
		}

		_pKeysTab = pTab;


		if(_nNbKeys > 0)
		{
			delete [] _pTimeKeysTab;
		}

		_pTimeKeysTab = new float[NbKeys];

		_nNbKeys = NbKeys;

		SetUniformTimeKeys();
	}

	void	SetUniformTimeKeys()
	{
		float	Step;
		int		i;
		
		QDT_ASSERT(_nNbKeys > 0);

		if(_nNbKeys == 1)
		{
			Step = 0.0f;
		}
		else
		{
			Step = 1.0f / (_nNbKeys - 1);
		}

		for(i = 0; i < _nNbKeys; ++i)
		{
			SetTimeKey(i, Step * i);	
		}
	}

	void	GetKey(int Key, T &Data) const
	{
		QDT_ASSERT(Key < _nNbKeys);

		Data = _pKeysTab[Key];
	}

	T*	GetKey(int Key) const
	{
		QDT_ASSERT(Key < _nNbKeys);

		return(&_pKeysTab[Key]);
	}

	void	SetKey(int Key, const T &Data)
	{
		QDT_ASSERT(Key < _nNbKeys);

		_pKeysTab[Key] = Data;
	}


	float	GetTimeKey(int Key) const
	{
		QDT_ASSERT(Key < _nNbKeys);

		return(_pTimeKeysTab[Key]);
	}

	void	SetTimeKey(int Key, float TimeKey)
	{
		QDT_ASSERT(Key < _nNbKeys);
		QDT_ASSERT(TimeKey >= 0.0f);
		QDT_ASSERT(TimeKey <= 1.0f);

		if(Key == 0)
		{
			_pTimeKeysTab[Key] = TimeKey;
		}
		else
		{
			if(TimeKey < _pTimeKeysTab[Key - 1])
			{
				_pTimeKeysTab[Key] = _pTimeKeysTab[Key - 1];
			}
			else
			{
				_pTimeKeysTab[Key] = TimeKey;
			}
		}
	}

	int		GetNbKeys() const
	{
		return(_nNbKeys);
	}

	int	GetKeyIndex(float Time) const
	{
		int		Key;

		QDT_ASSERT(Time >= 0.0f);
		QDT_ASSERT(Time <= 1.0f);

		if(_nNbKeys == 1)
		{
			return(0);
		}

		Key = 0;

		while(Time > GetTimeKey(Key + 1))
		{		
			++Key;
		}

		return(Key);
	}

	void	GetValue(float Time, int InterpolationDegre, T &Data) const
	{	
		int	Key;

		QDT_ASSERT(InterpolationDegre >= 0);
		QDT_ASSERT(InterpolationDegre <= 1);

		if(_nNbKeys == 1)
		{
			GetKey(0, Data);
			return;
		}

		Key = GetKeyIndex(Time);

		if(InterpolationDegre == 0)
		{
			GetKey(Key, Data);
		}
		else if(InterpolationDegre == 1)
		{
			float	TimeKey1;
			float	TimeKey2;
			float	TimeRatio;

			TimeKey1 = GetTimeKey(Key);
			TimeKey2 = GetTimeKey(Key + 1);
		
			TimeRatio = (Time - TimeKey1) / (TimeKey2 - TimeKey1);

			Data.Interpolate(TimeRatio, *GetKey(Key), *GetKey(Key + 1));
		}
	}

	void	Read(IO_STREAM &s)
	{
		int	i;

		s >> _nNbKeys;
		for	(i = 0 ; i < _nNbKeys ; ++i)
		{
			s >> _pKeysTab[i];
			s >> _pTimeKeysTab[i];
		}
	}

	void	Write(IO_STREAM &s) const
	{
		int	i;

		s << _nNbKeys;
		for	(i = 0 ; i < _nNbKeys ; ++i)
		{
			s << _pKeysTab[i];
			s << _pTimeKeysTab[i];
		}
	}

	friend IO_STREAM& operator <<(IO_STREAM &s, const DATA_TRACK<T> &a)
	{
		a.Write(s);
		return s;
	}
	friend IO_STREAM& operator >>(IO_STREAM &s, DATA_TRACK<T> &a)
	{
		a.Read(s);
		return s;
	}

private:

	int			_nNbKeys;

	T			*_pKeysTab;
	float		*_pTimeKeysTab;

};

inline void	DATA_TRACK<VECTOR>::SetKey(int Key, const VECTOR &Data)
{
	QDT_ASSERT(Key < _nNbKeys);

	_pKeysTab[Key].SlowAssign( Data );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
