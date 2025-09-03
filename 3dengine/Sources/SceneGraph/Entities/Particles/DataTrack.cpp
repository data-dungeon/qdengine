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
//	00-12-14	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__ ;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION (DATA_TRACK)
//
//	The DATA_TRACK class implements ...
//
//*****************************************************************************

#include	"include.h"

#ifndef _DEBUG

#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// IMPORTANT : pour generer le code associe aux templates
//-----------------------------------------------------------------------------
/*
void	Instanciate()
{
	DATA_TRACK<COLOR>	_ColorTrack;
	COLOR				_Color;

	_ColorTrack.Build(0);
	_ColorTrack.SetUniformTimeKeys();
	_ColorTrack.GetKey(0, _Color);
	_ColorTrack.GetKey(0);
	_ColorTrack.SetKey(0, _Color);
	_ColorTrack.GetTimeKey(0);
	_ColorTrack.SetTimeKey(0, 0.0f);
	_ColorTrack.GetNbKeys();
	_ColorTrack.GetKeyIndex(0.0f);
	_ColorTrack.GetValue(0.0f, 0, _Color);


	DATA_TRACK<POINT2D>		_Point2DTrack;
	POINT2D					_Point2D;

	_Point2DTrack.Build(0);
	_Point2DTrack.SetUniformTimeKeys();
	_Point2DTrack.GetKey(0, _Point2D);
	_Point2DTrack.GetKey(0);
	_Point2DTrack.SetKey(0, _Point2D);
	_Point2DTrack.GetTimeKey(0);
	_Point2DTrack.SetTimeKey(0, 0.0f);
	_Point2DTrack.GetNbKeys();
	_Point2DTrack.GetKeyIndex(0.0f);
	_Point2DTrack.GetValue(0.0f, 0, _Point2D);


	DATA_TRACK<SCALE3D>	_ScaleTrack;
	SCALE3D				_Scale;

	_ScaleTrack.Build(0);
	_ScaleTrack.SetUniformTimeKeys();
	_ScaleTrack.GetKey(0, _Scale);
	_ScaleTrack.GetKey(0);
	_ScaleTrack.SetKey(0, _Scale);
	_ScaleTrack.GetTimeKey(0);
	_ScaleTrack.SetTimeKey(0, 0.0f);
	_ScaleTrack.GetNbKeys();
	_ScaleTrack.GetKeyIndex(0.0f);
	_ScaleTrack.GetValue(0.0f, 0, _Scale);

}
*/

/*
//-----------------------------------------------------------------------------
// Name: DATA_TRACK constructor
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> DATA_TRACK<T>::DATA_TRACK()
:
_nNbKeys		(0),
_pKeysTab		(NULL),
_pTimeKeysTab	(NULL)
{

}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK destructor
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> DATA_TRACK<T>::~DATA_TRACK()
{
	if(_nNbKeys >= 0)
	{
		delete [] _pKeysTab;
		delete [] _pTimeKeysTab;
	}
}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::Build
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> void	DATA_TRACK<T>::Build(int NbKeys)
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

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::SetUniformTimeKeys
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> void	DATA_TRACK<T>::SetUniformTimeKeys()
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

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::GetKey
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> void	DATA_TRACK<T>::GetKey(int Key, T &Data) const
{
	QDT_ASSERT(Key < _nNbKeys);

	Data = _pKeysTab[Key];
}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::GetKey
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> T*	DATA_TRACK<T>::GetKey(int Key) const
{
	QDT_ASSERT(Key < _nNbKeys);

	return(&_pKeysTab[Key]);
}


//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::SetKey
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------		
template <class T> void	DATA_TRACK<T>::SetKey(int Key, const T &Data)
{
	QDT_ASSERT(Key < _nNbKeys);

	_pKeysTab[Key] = Data;
}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::GetTimeKey
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> float	DATA_TRACK<T>::GetTimeKey(int Key) const
{
	QDT_ASSERT(Key < _nNbKeys);

	return(_pTimeKeysTab[Key]);
}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::SetTimeKey
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> void	DATA_TRACK<T>::SetTimeKey(int Key, float TimeKey)
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

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::GetNbKeys
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> int		DATA_TRACK<T>::GetNbKeys() const
{
	return(_nNbKeys);
}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::GetKeyIndex
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> int	DATA_TRACK<T>::GetKeyIndex(float Time) const
{
	int		Key;

	QDT_ASSERT(Time >= 0.0f);
	QDT_ASSERT(Time <= 1.0f);

	if(_nNbKeys == 1)
	{
		return(0);
	}

	Key = 0;

	while(Time >= GetTimeKey(Key + 1))
	{		
		++Key;
	}

	return(Key);
}

//-----------------------------------------------------------------------------
// Name: DATA_TRACK<T>::GetValue
// Object: 
//00-12-13 JVA Created: 
//-----------------------------------------------------------------------------
template <class T> void	DATA_TRACK<T>::GetValue(float Time, int InterpolationDegre, T &Data) const
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
*/


//=============================================================================
// CODE ENDS HERE
//=============================================================================
