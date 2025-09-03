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
// CLASS: TRACK_DATA
//
//	The TRACK_DATA class implements the geometric animation an instance of
//	track data  can hold all the geometric transformationsfor an object whether 
//	it's part of a hierarchy like a skeeton or a single object. those transformation are 
//	translation, rotation and scaling
//
//00-11-24 GHP Created: 
//*****************************************************************************

#include	"Root.h"
#include	"TrackData.h"

#include	INCL_KMATH(Math/Geometry/Quaternions/QuaternionCompressed)
#include	INCL_KSERV(AnimTools/ScaleKey)
#include	INCL_3DENGINE(Behavior/Datas/TrackIndexes)

#ifdef _DEBUG
	#include "TrackData.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: TRACK_DATA constructor
// Object: 
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
TRACK_DATA::TRACK_DATA() 
:
_vAbsTrans(),
_sAbsScale(),
_AbsQuat(),
_pRotKeys(NULL),
_pTransKeys(NULL),
_pScaleKeys(NULL),	
_pAnimTops(NULL),
_pnRotTimes(NULL),
_prScaleTimes(NULL),
_pTrackIndexes(NULL),
_nNbrRotKeys(0),
_nNbrTransKeys(0),
_nNbrScaleKeys(0),
_nNbrAnimTops(0),
_nLastVal(0),
_rFPS( 30.0f),
_bReverse(false),
_bNeedReset(true)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
// Name: TRACK_DATA destructor
// Object: 
//00-11-24 GHP Created: 
//-----------------------------------------------------------------------------
TRACK_DATA::~TRACK_DATA()
{
	if(_pRotKeys)
	{
		delete [] _pRotKeys;
		_pRotKeys = NULL;
	}
	
	if(_pTransKeys)
	{
		delete [] _pTransKeys;
		_pTransKeys = NULL;
	}

	if(_pScaleKeys)
	{
		delete [] _pScaleKeys;
		_pScaleKeys = NULL;
	}

	if(_pAnimTops)
	{
		delete [] _pAnimTops;
		_pAnimTops = NULL;
	}

	if(_pnRotTimes)
	{
		delete [] _pnRotTimes;
		_pnRotTimes = NULL;
	}

	if(_prScaleTimes)
	{
		delete [] _prScaleTimes; 
		_prScaleTimes = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-04-07:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::Init()
{
	_pTrackIndexes = NULL;
	_nNbrRotKeys = 0;
	_nNbrTransKeys = 0;
	_nNbrScaleKeys = 0;
	_nNbrAnimTops = 0;
	_bReverse	= false;
	_bNeedReset = true;
	_rFPS = 30.0f;
	_nLastVal = 0;
	_vAbsTrans = VECTOR( 0.0f, 0.0f, 0.0f );
	_AbsQuat = QUATERNION(1.0f, 0.0f, 0.0f, 0.0f);

	if(_pRotKeys)
	{
		delete [] _pRotKeys;
		_pRotKeys = NULL;
	}
	
	if(_pTransKeys)
	{
		delete [] _pTransKeys;
		_pTransKeys = NULL;
	}

	if(_pScaleKeys)
	{
		delete [] _pScaleKeys;
		_pScaleKeys = NULL;
	}

	if(_pAnimTops)
	{
		delete [] _pAnimTops;
		_pAnimTops = NULL;
	}

	if(_pnRotTimes)
	{
		delete [] _pnRotTimes;
		_pnRotTimes = NULL;
	}

	if(_prScaleTimes)
	{
		delete [] _prScaleTimes; 
		_prScaleTimes = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::GetMatrix
// Object:	This function will build the matrix 
//			the result is returned in the passed by reference matrix Res
//00-11-24	GHP Created: 
//01-02-13:	GHP Modified  sequencing of operation changed after the matrix class has been changed
//-----------------------------------------------------------------------------
void TRACK_DATA::GetMatrix(const Float32 rFrame, 
						   MATRIX & M)
{
	M.Identity();
	
	if (_bNeedReset) 
	{
		ResetTrack();
	}

	VECTOR		V;
	VECTOR		S;
	QUATERNION	Q;

	GetTrans(rFrame, V);
	GetScale(rFrame, S);
	GetRot(rFrame, Q);

	M.SetQuaternion(Q);
	M.SetTranslation(V);
	M.SetScale(S);
	
	QDT_ASSERT(M.HasNoScale() == true);
}

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::GetTrans 
// Object: Same as getrot on the translation
//00-11-29 GHP Created: 
//-----------------------------------------------------------------------------
void TRACK_DATA::GetTrans(const Float32 rValue, 
						  VECTOR & V)
{
	Float32			rT2, rT1;
	Float32			rInterCoef;
	unsigned int	nTransIndex	= 1;
	unsigned int	nOldTransIndex	= 0;

	V.SetXYZ(0.0f, 0.0f, 0.0f);

	static const Float32	rEpsilon = 0.01f;

	// If must be between two keys
	if (_nNbrTransKeys == 0)		return;
	if (CheckLastTrans(rValue, V))	return;
	if (CheckFirstTrans(rValue, V))	return;

	if ( _pTrackIndexes )
	{
		nTransIndex		= _pTrackIndexes->GetTransIndex();
		nOldTransIndex	= _pTrackIndexes->GetOldTransIndex();
	}

	if (nTransIndex >= _nNbrTransKeys)
	{
		ResetTrack();
		nTransIndex		= _pTrackIndexes->GetTransIndex();
		nOldTransIndex	= _pTrackIndexes->GetOldTransIndex();
	}
	
	rT1 = _pTransKeys[nOldTransIndex].GetPad();
	rT2	= _pTransKeys[nTransIndex].GetPad();

	if (rT1 <= rValue)
	{
		while (nTransIndex <  _nNbrTransKeys)
		{			
			if (rValue <= rT2)
			{
				rT1 = _pTransKeys[nOldTransIndex].GetPad();

				const VECTOR & V1 = _pTransKeys[nOldTransIndex];
				const VECTOR & V2 = _pTransKeys[nTransIndex];
				
				rInterCoef = (rValue - rT1) / (rT2 - rT1);		
				V.InterLinear(rInterCoef, V1, V2);
				
				if (_pTrackIndexes)
				{
					_pTrackIndexes->SetTransIndex(nTransIndex);
					_pTrackIndexes->SetOldTransIndex(nOldTransIndex);
				}
				
				return;
			}

			nOldTransIndex = nTransIndex;
			nTransIndex++;
			rT2	= _pTransKeys[nTransIndex].GetPad();
		}
	}
	else
	{
		while (nTransIndex >= 0)
		{				
			if (rT2 <= rValue)
			{
				rT1 = _pTransKeys[nOldTransIndex].GetPad();

				const VECTOR & V1 = _pTransKeys[nOldTransIndex];
				const VECTOR & V2 = _pTransKeys[nTransIndex];
				
				rInterCoef = (rValue - rT1) / (rT2 - rT1);
				V.InterLinear(rInterCoef, V1, V2);
				
				if (_pTrackIndexes)
				{
					_pTrackIndexes->SetTransIndex(nTransIndex);
					_pTrackIndexes->SetOldTransIndex(nOldTransIndex);
				}
				
				return;
			}

			nOldTransIndex = nTransIndex;
			nTransIndex--;
			rT2	= _pTransKeys[nTransIndex].GetPad();
		}
	}

	QDT_FAIL();
}

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::GetScale
// Object: same as translations on the scalings
//00-11-29 GHP Created: 
//-----------------------------------------------------------------------------
void TRACK_DATA::GetScale(const Float32 rValue,
						  VECTOR & S)
{	
	Float32			rT2, rT1;
	Float32			rTimeDelta;
	Float32			rInterCoef;
	VECTOR			S1, S2;
	unsigned int	nScaleIndex		= _pTrackIndexes->GetScaleIndex();
	unsigned int	nOldScaleIndex	= _pTrackIndexes->GetOldScaleIndex();

	static const	Float32 rEpsilon = 0.01f;

	// If must be between two keys
	if (_nNbrScaleKeys == 0)		return;
	if (CheckLastScale(rValue, S))	return;
	if (CheckFirstScale(rValue, S))	return;
	
	for (; nScaleIndex <  _nNbrScaleKeys; ++nScaleIndex)
	{
		rT2 = _prScaleTimes[nScaleIndex];
		rTimeDelta = MATH::Abs(rT2  - rValue);
		
		// almost equal
		if (rTimeDelta < rEpsilon)
		{
			S = _pScaleKeys[nScaleIndex];

			_pTrackIndexes->SetScaleIndex(nScaleIndex);
			_pTrackIndexes->SetOldScaleIndex(nOldScaleIndex);

			return;
		}

		// Interpolation is required
		rT1 = _prScaleTimes[nOldScaleIndex];
		if (rT1 < rValue && rValue < rT2 || rT2 < rValue && rValue < rT1)
		{
			S1 = _pScaleKeys[nOldScaleIndex];
			S2 = _pScaleKeys[nScaleIndex];
			
			rInterCoef = (rValue - rT1) / (rT2 - rT1);			
			S.Interpolate(rInterCoef, S1, S2);
			
			_pTrackIndexes->SetTransIndex(nScaleIndex);
			_pTrackIndexes->SetOldTransIndex(nOldScaleIndex);

			return;
		}
		
		nOldScaleIndex = nScaleIndex;
	}

	QDT_FAIL();
	return;
}

//-----------------------------------------------------------------------------
//	Name:		TRACK_DATA::GetRot
//	Object:		Returning in the quaternion the value of the rotation
//	Params :	+ rFrame is representing the time
//				+ Quternion will contain the value
//	KnowHow	:	First we check that they are rotation keys if not build a dummy one and returns
//				if they are rotation keys two case :
//					+ the frame passed matches a frame in the collection, returns it
//					+ The frame does not match interpolate the two closest
//
//  $$$$ OPTIMIZE How to look for the good key and on the SLERP part 
//  00-11-29 GHP Created: 
//-----------------------------------------------------------------------------
void TRACK_DATA::GetRot(const Float32 rValue, QUATERNION & Q) 
{ 
	Float32			rT1, rT2;
	Float32			rInterCoef;
	unsigned int	nRotIndex = 1;
	unsigned int	nOldRotIndex = 0;
	QUATERNION		Q1, Q2;

	Q.Id();

	if ( _pTrackIndexes )
	{
		nRotIndex		= _pTrackIndexes->GetRotIndex();
		nOldRotIndex	= _pTrackIndexes->GetOldRotIndex();
	}	

	// If must be between two keys
	if (_nNbrRotKeys == 0)			return;
	if (CheckLastRot(rValue, Q))	return;
	if (CheckFirstRot(rValue, Q))	return;
	
	if (nRotIndex >= _nNbrRotKeys)
	{
		ResetTrack();
		nRotIndex	 = _pTrackIndexes->GetRotIndex();
		nOldRotIndex = _pTrackIndexes->GetOldRotIndex();
	}
	
	rT1	= _pnRotTimes[nOldRotIndex];
	rT2	= _pnRotTimes[nRotIndex];
		
	if (rT1 <= rValue)
	{
		while (nRotIndex < _nNbrRotKeys)
		{
			if (rValue <= rT2)
			{
				rT1	= _pnRotTimes[nOldRotIndex];

				Q1 = _pRotKeys[nOldRotIndex];
				Q2 = _pRotKeys[nRotIndex];

				QDT_ASSERT(Q1.IsNormalized());
				QDT_ASSERT(Q2.IsNormalized());
				
				rInterCoef = (rValue - rT1) / (rT2 - rT1);					
				Q.BasicInterSLERP(rInterCoef, Q1, Q2);
				
				if ( _pTrackIndexes )
				{
					_pTrackIndexes->SetRotIndex(nRotIndex);
					_pTrackIndexes->SetOldRotIndex(nOldRotIndex);
				}
				return;
			}
			
			nOldRotIndex = nRotIndex;
			nRotIndex++;
			rT2	= _pnRotTimes[nRotIndex];
		}
	}
	else
	{
		while (nRotIndex >=  0)
		{				
			if (rT2 <= rValue)
			{
				rT1	= _pnRotTimes[nOldRotIndex];

				Q1 = _pRotKeys[nOldRotIndex];
				Q2 = _pRotKeys[nRotIndex];

				QDT_ASSERT(Q1.IsNormalized());
				QDT_ASSERT(Q2.IsNormalized());
				
				rInterCoef = (rValue - rT1) / (rT2 - rT1);					
				Q.BasicInterSLERP(rInterCoef, Q1, Q2);
				
				if ( _pTrackIndexes )
				{
					_pTrackIndexes->SetRotIndex(nRotIndex);
					_pTrackIndexes->SetOldRotIndex(nOldRotIndex);
				}
				return;
			}
			
			nOldRotIndex = nRotIndex;
			nRotIndex--;
			rT2	= _pnRotTimes[nRotIndex];	
		}	
	}

	QDT_FAIL();
}

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ReadV1
// Object: 
//00-11-27 GHP Created: 
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void TRACK_DATA::ReadV1(class IO_STREAM & Stream)
{
	ReadKeysDataRotations(Stream);
	ReadKeysDataTranslations(Stream);
	ReadKeysDataScaling(Stream);
}
#endif //  _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ReadV2
// Object: 
//00-11-27 GHP Created: 
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void TRACK_DATA::ReadV2(class IO_STREAM & Stream)
{
	ReadKeysDataRotations(Stream);
	ReadKeysDataTranslations(Stream);
	ReadKeysDataScaling(Stream);
	ReadAnimTops(Stream);
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ReadV2
// Object: 
//00-11-27 GHP Created: 
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void TRACK_DATA::ReadV3(class IO_STREAM & Stream)
{
	ReadKeysDataRotations(Stream);
	ReadKeysDataTranslations(Stream);
	ReadKeysDataScaling(Stream);
	ReadAnimTops(Stream);
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		ReadV4
//	Object:		
//	01-11-07:	cvi - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::ReadV4(class IO_STREAM & Stream)
{
	ReadKeysDataRotations(Stream);
	ReadKeysDataTranslations(Stream);
	ReadKeysDataScaling(Stream);
	ReadAnimTops(Stream);
	
	//	Absolute datas
	ReadReferenceData(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void TRACK_DATA::Write(class IO_STREAM & Stream) 
{
	WriteKeysDataRotations(Stream);
	WriteKeysDataTranslations(Stream);
	WriteKeysDataScaling(Stream);
	WriteAnimTops(Stream);

	//	Write absolute datas
	WriteReferenceData(Stream);
}
#endif

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ReadKeysDataRotations
// Object: 
//00-11-28 GHP Created: 
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void TRACK_DATA::ReadKeysDataRotations(class IO_STREAM & Stream)
{
	unsigned int i; 
	
	QDT_ASSERT(_pRotKeys == NULL); 

	Stream >> _nNbrRotKeys;
	
	if (_nNbrRotKeys > 0)
	{
		#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys = new QUATERNION_COMPRESSED[_nNbrRotKeys];
		#else
			_pRotKeys = new QUATERNION[_nNbrRotKeys];
		#endif

		_pnRotTimes = new unsigned short [_nNbrRotKeys];
	}

	for (i = 0 ; i <  _nNbrRotKeys ; ++i)
	{	
		Float32 r;
		Stream >> r;
		QDT_ENSURE(r < 65536);
		_pnRotTimes[i] = (unsigned short) r; 

		#ifdef USE_COMPRESSED_QUATERNIONS
			QUATERNION Q;
			Stream >> Q;
			_pRotKeys[i].Compress(Q);
		#else
			Stream >> _pRotKeys[i];
			QDT_ASSERT(_pRotKeys[i].IsNormalized());
		#endif	
	}

	OptimizeRotationKeys();
}
#endif //_MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		RemoveHalfRotationKeys
//	Object:		Remove rotation keys when possible
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::RemoveHalfRotationKeys()
{
	if (_nNbrRotKeys > 2)
	{
#ifdef USE_COMPRESSED_QUATERNIONS
		QDT_VECTOR <QUATERNION_COMPRESSED>		vNewRotationsKeys;
#else
		QDT_VECTOR <QUATERNION>		vNewRotationsKeys;
#endif 
		QDT_VECTOR <unsigned short>				vNewRotationsTimes;

		vNewRotationsKeys.PushTail(_pRotKeys[0]);
		vNewRotationsTimes.PushTail(_pnRotTimes[0]);

		for (UInt32 i = 0; i < _nNbrRotKeys - 2; ++i)
		{
			unsigned short n1 = _pnRotTimes[i];
			unsigned short n2 = _pnRotTimes[i+1];
			unsigned short n3 = _pnRotTimes[i+2];

			if (n1 + 2 == n3 && n2 % 2 == 1)
			{
				// Skip key
			}
			else
			{
				vNewRotationsKeys.PushTail(_pRotKeys[i + 1]);
				vNewRotationsTimes.PushTail(_pnRotTimes[i + 1]);
			}
		}

		vNewRotationsKeys.PushTail(_pRotKeys[_nNbrRotKeys - 1]);
		vNewRotationsTimes.PushTail(_pnRotTimes[_nNbrRotKeys - 1]);

		if (vNewRotationsKeys.GetSize() < _nNbrRotKeys)
		{
			delete [] _pRotKeys;
			delete [] _pnRotTimes;

			_nNbrRotKeys = vNewRotationsKeys.GetSize();

#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys = new QUATERNION_COMPRESSED [_nNbrRotKeys];
#else
			_pRotKeys = new QUATERNION [_nNbrRotKeys];
#endif 
			
			_pnRotTimes = new unsigned short [_nNbrRotKeys];

			for (UInt32 i = 0 ; i < _nNbrRotKeys ; ++i)
			{
				_pRotKeys[i] = vNewRotationsKeys[i];
				_pnRotTimes[i] = vNewRotationsTimes[i];
			}
		}
	}
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		OptimizeRotationKeys
//	Object:		Remove rotation keys when possible
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::OptimizeRotationKeys()
{
#ifdef USE_COMPRESSED_QUATERNIONS
	
	// C'est tout pourri, ne jamais utiliser ça!!
//	RemoveHalfRotationKeys();

	if (_nNbrRotKeys > 2)
	{
		unsigned int i;

		QDT_VECTOR <QUATERNION_COMPRESSED>	vNewRotationsKeys;
		QDT_VECTOR <unsigned short>			vNewRotationsTimes;

		vNewRotationsKeys.PushTail(_pRotKeys[0]);
		vNewRotationsTimes.PushTail(_pnRotTimes[0]);

		for (i = 0 ; i < _nNbrRotKeys - 2; ++i)
		{
			QUATERNION_COMPRESSED Q1 = _pRotKeys[i];
			QUATERNION_COMPRESSED Q2 = _pRotKeys[i + 1];
			QUATERNION_COMPRESSED Q3 = _pRotKeys[i + 2];

			if (Q1 != Q2 || Q1 != Q3)
			{
				vNewRotationsKeys.PushTail(_pRotKeys[i + 1]);
				vNewRotationsTimes.PushTail(_pnRotTimes[i + 1]);
			}
		}

		vNewRotationsKeys.PushTail(_pRotKeys[_nNbrRotKeys - 1]);
		vNewRotationsTimes.PushTail(_pnRotTimes[_nNbrRotKeys - 1]);

		if (vNewRotationsKeys.GetSize() < _nNbrRotKeys)
		{
			delete [] _pRotKeys;
			delete [] _pnRotTimes;

			_nNbrRotKeys = vNewRotationsKeys.GetSize();

			_pRotKeys = new QUATERNION_COMPRESSED [_nNbrRotKeys];
			_pnRotTimes = new unsigned short [_nNbrRotKeys];

			for (i = 0 ; i < _nNbrRotKeys ; ++i)
			{
				_pRotKeys[i] = vNewRotationsKeys[i];
				_pnRotTimes[i] = vNewRotationsTimes[i];
			}
		}
	}

	#ifdef SKIP_KEYS_WHEN_POSSIBLE
		SkipRotationKeys();
	#endif

#else
	if (_nNbrRotKeys > 2)
	{
		unsigned int i;

		QDT_VECTOR <QUATERNION>	vNewRotationsKeys;
		QDT_VECTOR <unsigned short>			vNewRotationsTimes;

		vNewRotationsKeys.PushTail(_pRotKeys[0]);
		vNewRotationsTimes.PushTail(_pnRotTimes[0]);

		for (i = 0 ; i < _nNbrRotKeys - 2; ++i)
		{
			QUATERNION Q1 = _pRotKeys[i];
			QUATERNION Q2 = _pRotKeys[i + 1];
			QUATERNION Q3 = _pRotKeys[i + 2];

			if (Q1 != Q2 || Q1 != Q3)
			{
				vNewRotationsKeys.PushTail(_pRotKeys[i + 1]);
				vNewRotationsTimes.PushTail(_pnRotTimes[i + 1]);
			}
		}

		vNewRotationsKeys.PushTail(_pRotKeys[_nNbrRotKeys - 1]);
		vNewRotationsTimes.PushTail(_pnRotTimes[_nNbrRotKeys - 1]);

		if (vNewRotationsKeys.GetSize() < _nNbrRotKeys)
		{
			delete [] _pRotKeys;
			delete [] _pnRotTimes;

			_nNbrRotKeys = vNewRotationsKeys.GetSize();

			_pRotKeys = new QUATERNION [_nNbrRotKeys];
			_pnRotTimes = new unsigned short [_nNbrRotKeys];

			for (i = 0 ; i < _nNbrRotKeys ; ++i)
			{
				_pRotKeys[i] = vNewRotationsKeys[i];
				_pnRotTimes[i] = vNewRotationsTimes[i];
			}
		}
	}

	#ifdef SKIP_KEYS_WHEN_POSSIBLE
	SkipRotationKeys();
	#endif
#endif
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		OptimizeTranslationKeys
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::OptimizeTranslationKeys()
{
	if (_nNbrTransKeys > 2)
	{
		unsigned int			i;
		QDT_VECTOR <VECTOR>		vNewTranslationsKeys;
		QDT_VECTOR <Float32>		vNewTranslationsTimes;

		vNewTranslationsKeys.PushTail(_pTransKeys[0]);
		vNewTranslationsTimes.PushTail(_pTransKeys[0].GetPad());

		for (i = 0 ; i < _nNbrTransKeys - 2; ++i)
		{
			VECTOR V1 = _pTransKeys[i];
			VECTOR V2 = _pTransKeys[i + 1];
			VECTOR V3 = _pTransKeys[i + 2];

			if (V1.GetX() != V2.GetX() || V1.GetX() != V3.GetX() ||
				V1.GetY() != V2.GetY() || V1.GetY() != V3.GetY() ||
				V1.GetZ() != V2.GetZ() || V1.GetZ() != V3.GetZ())
			{
				vNewTranslationsKeys.PushTail(_pTransKeys[i + 1]);
				vNewTranslationsTimes.PushTail(_pTransKeys[i + 1].GetPad());
			}	
		}

		vNewTranslationsKeys.PushTail(_pTransKeys[_nNbrTransKeys - 1]);
		vNewTranslationsTimes.PushTail(_pTransKeys[_nNbrTransKeys - 1].GetPad());

		if (vNewTranslationsKeys.GetSize() < _nNbrTransKeys)
		{
			delete [] _pTransKeys;

			_nNbrTransKeys = vNewTranslationsKeys.GetSize();

			_pTransKeys = new VECTOR [_nNbrTransKeys];

			for (i = 0 ; i < _nNbrTransKeys ; ++i)
			{
				_pTransKeys[i] = vNewTranslationsKeys[i];
				_pTransKeys[i].SetPad(vNewTranslationsTimes[i]);
			}
		}
	} 

#ifdef SKIP_KEYS_WHEN_POSSIBLE
	SkipTranslationKeys();
#endif
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		SkipTranslationKeys
//	Object:		
//	04-02-06:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::SkipTranslationKeys()
{
	if (_nNbrTransKeys > 2)
	{
		unsigned int			i;
		QDT_VECTOR <VECTOR>		vNewTranslationsKeys;
		QDT_VECTOR <Float32>		vNewTranslationsTimes;
		const Float32				rEpsilon = 0.001f;

		vNewTranslationsKeys.PushTail(_pTransKeys[0]);
		vNewTranslationsTimes.PushTail(_pTransKeys[0].GetPad());

		unsigned int nFirst = 0;

		for (i = 0 ; i < _nNbrTransKeys - 2; ++i)
		{
			VECTOR V1 = _pTransKeys[nFirst];
			VECTOR V2 = _pTransKeys[i + 1];
			VECTOR V3 = _pTransKeys[i + 2];
			Float32 rTime1 = _pTransKeys[nFirst].GetPad();
			Float32 rTime2 = _pTransKeys[i + 1].GetPad();
			Float32 rTime3 = _pTransKeys[i + 2].GetPad();

			Float32 rCoeff = (rTime2 - rTime1) / (rTime3 - rTime1);				

			VECTOR InterpolatedV2;
			InterpolatedV2.InterLinear(rCoeff, V1, V3);

			Float32 rDiffX = MATH::Abs(InterpolatedV2.GetX() - V2.GetX());
			Float32 rDiffY = MATH::Abs(InterpolatedV2.GetY() - V2.GetY());
			Float32 rDiffZ = MATH::Abs(InterpolatedV2.GetZ() - V2.GetZ());
			
			if (rDiffX > rEpsilon ||
				rDiffY > rEpsilon ||
				rDiffZ > rEpsilon)
			{
				vNewTranslationsKeys.PushTail(_pTransKeys[i + 1]);
				vNewTranslationsTimes.PushTail(_pTransKeys[i + 1].GetPad());
				++nFirst;
			}	
		}

		vNewTranslationsKeys.PushTail(_pTransKeys[_nNbrTransKeys - 1]);
		vNewTranslationsTimes.PushTail(_pTransKeys[_nNbrTransKeys - 1].GetPad());

		if (vNewTranslationsKeys.GetSize() < _nNbrTransKeys)
		{
			delete [] _pTransKeys;

			_nNbrTransKeys = vNewTranslationsKeys.GetSize();

			_pTransKeys = new VECTOR [_nNbrTransKeys];

			for (i = 0 ; i < _nNbrTransKeys ; ++i)
			{
				_pTransKeys[i] = vNewTranslationsKeys[i];
				_pTransKeys[i].SetPad(vNewTranslationsTimes[i]);
			}
		}
	} 
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		SkipRotationKeys
//	Object:		
//	04-02-06:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::SkipRotationKeys()
{
	if (_nNbrRotKeys > 2)
	{
		unsigned int i;
#ifdef USE_COMPRESSED_QUATERNIONS
		QDT_VECTOR <QUATERNION_COMPRESSED>	vNewRotationsKeys;
#else
		QDT_VECTOR <QUATERNION>	vNewRotationsKeys;
#endif
		QDT_VECTOR <Float32>					vNewRotationsTimes;
		const Float32 rEpsilon = 0.0005f;

		// Reference : Meurtre (tenue du couteau)
		// C'est a priori la prise d'objet le truc le plus chaud à effectuer
		// Epsilon = 0.001f -> 20.0% removed -> Pas de décalage visible
		// Epsilon = 0.002f -> 32.5% removed -> Leger decalage visible
		// Epsilon = 0.003f -> 41,4% removed -> Leger décalage visible
		// Epsilon = 0.004f -> 46,0% removed -> Decalage visible
		// Epsilon = 0.005f -> 52.9% removed -> Decalage visible

		// Bon, en observant attentivement, j'arrive quand même à voir
		// une perte de qualité donc je passe à 0.0005. Le gain n'est pas super
		// important, vers les 10-15% j'imagine, mais ça vaut quand même le coup.
		// Peut-être qu'il y a des animations sur lesquelles le gain est plus important

		vNewRotationsKeys.PushTail(_pRotKeys[0]);
		vNewRotationsTimes.PushTail(_pnRotTimes[0]);

		unsigned int nFirst = 0;

		for (i = 0 ; i < _nNbrRotKeys - 2; ++i)
		{
			QUATERNION Q1;
			QUATERNION Q2;
			QUATERNION Q3;

#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys[nFirst].Uncompress(Q1);
			_pRotKeys[i + 1].Uncompress(Q2);
			_pRotKeys[i + 2].Uncompress(Q3);
#else
			Q1 = _pRotKeys[nFirst];
			Q2 = _pRotKeys[i + 1];
			Q3 = _pRotKeys[i + 2];
#endif

			Float32 rTime1 = _pnRotTimes[nFirst];
			Float32 rTime2 = _pnRotTimes[i + 1];
			Float32 rTime3 = _pnRotTimes[i + 2];

			QUATERNION InterpolatedQ2;

			Float32 rCoeff = (rTime2 - rTime1) / (rTime3 - rTime1);					
			InterpolatedQ2.BasicInterSLERP(rCoeff, Q1, Q3);

			Float32 rDiffX = MATH::Abs(InterpolatedQ2.GetX() - Q2.GetX());
			Float32 rDiffY = MATH::Abs(InterpolatedQ2.GetY() - Q2.GetY());
			Float32 rDiffZ = MATH::Abs(InterpolatedQ2.GetZ() - Q2.GetZ());
			Float32 rDiffW = MATH::Abs(InterpolatedQ2.GetW() - Q2.GetW());

			if (rDiffX > rEpsilon ||
				rDiffY > rEpsilon ||
				rDiffZ > rEpsilon ||
				rDiffW > rEpsilon)
			{
				vNewRotationsKeys.PushTail(_pRotKeys[i + 1]);
				vNewRotationsTimes.PushTail(_pnRotTimes[i + 1]);
				++nFirst;
			}
		}

		vNewRotationsKeys.PushTail(_pRotKeys[_nNbrRotKeys - 1]);
		vNewRotationsTimes.PushTail(_pnRotTimes[_nNbrRotKeys - 1]);

		if (vNewRotationsKeys.GetSize() < _nNbrRotKeys)
		{
			delete [] _pRotKeys;
			delete [] _pnRotTimes;

			_nNbrRotKeys = vNewRotationsKeys.GetSize();
#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys = new QUATERNION_COMPRESSED [_nNbrRotKeys];
#else
			_pRotKeys = new QUATERNION [_nNbrRotKeys];
#endif
			_pnRotTimes = new unsigned short [_nNbrRotKeys];

			for (i = 0 ; i < _nNbrRotKeys ; ++i)
			{
				_pRotKeys[i] = vNewRotationsKeys[i];
				_pnRotTimes[i] = (unsigned short) vNewRotationsTimes[i];
			}
		}
	}
}
#endif //_MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void TRACK_DATA::WriteKeysDataRotations(class IO_STREAM & Stream) 
{
	unsigned int i;
	
	Stream << _nNbrRotKeys;
	
	for (i = 0 ; i < _nNbrRotKeys ; ++i)
	{
		Stream << static_cast<Float32>(_pnRotTimes[i]);
		
		#ifdef USE_COMPRESSED_QUATERNIONS
			QUATERNION Q;
			Q = _pRotKeys[i];
			Q.Write(Stream);
		#else
			_pRotKeys[i].Write(Stream);
		#endif
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteKeysDataTopAnims
//	Object:		
//	02-01-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	TRACK_DATA::WriteAnimTops(IO_STREAM & Stream)
{
	unsigned int i;
	
	Stream << 0; // Dummy
	Stream << _nNbrAnimTops;

	for (i = 0 ; i < _nNbrAnimTops ; ++i)
	{
		Stream  << _pAnimTops[i];
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadReferenceData
//	Object:		
//	01-11-07:	cvi - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::ReadReferenceData(class IO_STREAM &	Stream)
{
	Stream >> _AbsQuat;
	Stream >> _vAbsTrans;

	QDT_ASSERT(_AbsQuat.IsNormalized());
}
#endif // _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		WriteReferenceData
//	Object:		
//	01-11-07:	cvi - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	TRACK_DATA::WriteReferenceData(class IO_STREAM &	Stream)
{
	Stream << _AbsQuat;
	Stream << _vAbsTrans;
}
#endif

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ReadKeysDataTranslations
// Object: 
//00-11-28 GHP Created: 
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void TRACK_DATA::ReadKeysDataTranslations(class IO_STREAM & Stream)
{
	unsigned int i;
	Float32 rTime;
	
	QDT_ASSERT(_pTransKeys == NULL);

	Stream >> _nNbrTransKeys;

	if (_nNbrTransKeys > 0)
	{
		_pTransKeys = new VECTOR[_nNbrTransKeys];
	}

	for (i = 0 ; i < _nNbrTransKeys ; ++i)
	{
		Stream >> rTime;
		Stream >> _pTransKeys[i];
		_pTransKeys[i].SetPad(rTime);
	}

	OptimizeTranslationKeys();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadTopAnims
//	Object:		
//	02-01-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	TRACK_DATA::ReadAnimTops(IO_STREAM & Stream)
{
	// On lit les vieux tops d'anims bidon qui servent a rien (trigger)
	int nCount;
	Stream >> nCount;

	Int32 i;
	Float32 rDummy;;
	for (i = 0 ; i < nCount; ++i)
	{
		Stream >> rDummy;
		Stream >> rDummy;
	}

	// Tops d'anim (correspond aux anciens event)	
	ANIM_TOP		AnimTop;

	Stream >> _nNbrAnimTops;

	QDT_ASSERT(_pAnimTops == NULL);

	if (_nNbrAnimTops > 0)
	{
		_pAnimTops = new ANIM_TOP[_nNbrAnimTops];
	}

	for (i = 0 ; i < (Int32)_nNbrAnimTops ; ++i)
	{
		Stream >> _pAnimTops[i];
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void TRACK_DATA::WriteKeysDataTranslations( class IO_STREAM & Stream ) 
{
	Stream << _nNbrTransKeys;

	unsigned int i;
	for (i = 0 ; i < _nNbrTransKeys ; ++i)
	{
		Stream << _pTransKeys[i].GetPad();
		Stream << _pTransKeys[i];
	}
}
#endif

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ReadKeysDataScaling
// Object: 
//00-11-28 GHP Created: 
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void TRACK_DATA::ReadKeysDataScaling(class IO_STREAM & Stream)
{
	QDT_ASSERT(_pScaleKeys == NULL);

	Stream >> _nNbrScaleKeys;

	if (_nNbrScaleKeys > 0)
	{
		_prScaleTimes = new Float32 [_nNbrScaleKeys];
		_pScaleKeys = new VECTOR [_nNbrScaleKeys];
	}

	unsigned int i;
	for (i = 0 ; i <  _nNbrScaleKeys ; ++ i)
	{
		Stream >> _prScaleTimes[i];
		Stream >> _pScaleKeys[i];
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void TRACK_DATA::WriteKeysDataScaling( class IO_STREAM & Stream ) 
{
	Stream << _nNbrScaleKeys;
	
	unsigned int i;
	for (i = 0 ; i < _nNbrScaleKeys ; ++i)
	{
		Stream << _prScaleTimes[i];
		Stream << _pScaleKeys[i];
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetBaseMatrix
//	Object:		
//	01-04-11:	GHP - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::GetBaseMatrix(MATRIX& res) const 
{
	res.SetQuaternion(_AbsQuat); 
	res.SetTranslation(_vAbsTrans); 
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstMatrix
//	Object:		
//	01-08-27:	GHP - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::GetFirstMatrix(MATRIX &	res) const
{
	MATRIX BaseM;
	BaseM.Identity();

	if (_nNbrRotKeys > 0)
	{
		#ifdef USE_COMPRESSED_QUATERNIONS
			QUATERNION Q;
			Q = _pRotKeys[0];
			res.SetQuaternion(Q);
		#else	
			res.SetQuaternion(_pRotKeys[0]);
		#endif
	}
	if (_nNbrTransKeys > 0)
	{
		res.SetTranslation(_pTransKeys[0]);
	}

	GetBaseMatrix(BaseM);

	res.MulAffine(BaseM); 
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::SetRotations(const QDT_VECTOR<QUATERNION_KEY> & vRotKeys) 
{
	QDT_ASSERT(_pRotKeys == NULL);

	_nNbrRotKeys = vRotKeys.GetSize();
	if (_nNbrRotKeys > 0)
	{
		#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys = new QUATERNION_COMPRESSED[_nNbrRotKeys];
		#else
			_pRotKeys = new QUATERNION[_nNbrRotKeys];
		#endif

		_pnRotTimes = new unsigned short [_nNbrRotKeys];
	}
	
	unsigned int i;
	for (i = 0 ; i < _nNbrRotKeys ; ++i)
	{
		_pnRotTimes[i] = (unsigned short) vRotKeys[i].GetTime();

		#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys[i].Compress(vRotKeys[i].GetQuaternion());
		#else
			_pRotKeys[i] = vRotKeys[i].GetQuaternion();
		#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::SetTranslation(const QDT_VECTOR<KEY_3> & vTransKeys) 
{
	QDT_ASSERT(_pTransKeys == NULL);

	_nNbrTransKeys = vTransKeys.GetSize();
	if (_nNbrTransKeys > 0)
	{
		_pTransKeys = new VECTOR[_nNbrTransKeys];
	}
	
	unsigned int i;
	for (i = 0 ; i < _nNbrTransKeys ; ++i)
	{
		VECTOR_KEY Key(vTransKeys[i]);
		_pTransKeys[i] = Key.GetVector();
		_pTransKeys[i].SetPad(Key.GetTime());
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTranslation
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetTranslation(const QDT_VECTOR <VECTOR_KEY> & vTransKeys)
{
	QDT_ASSERT(_pTransKeys == NULL);

	_nNbrTransKeys = vTransKeys.GetSize();
	if (_nNbrTransKeys > 0)
	{
		_pTransKeys = new VECTOR[_nNbrTransKeys];
	}
	
	unsigned int i;
	for (i = 0 ; i < _nNbrTransKeys ; ++i)
	{
		_pTransKeys[i] = vTransKeys[i].GetVector();
		_pTransKeys[i].SetPad(vTransKeys[i].GetTime());
	}
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-04-13:	GHP - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::SetScale(const QDT_VECTOR<KEY_3> & vScaleKeys) 
{
	QDT_ASSERT(_pScaleKeys == NULL);

	_nNbrScaleKeys = vScaleKeys.GetSize();
	if (_nNbrScaleKeys > 0)
	{
		_pScaleKeys = new VECTOR[_nNbrScaleKeys];
		_prScaleTimes = new Float32[_nNbrScaleKeys];
	}
	
	unsigned int i;
	for (i = 0 ; i < _nNbrScaleKeys ; ++i)
	{
		SCALE_KEY Key(vScaleKeys[i]);
		_pScaleKeys[i] = Key.GetScale();
		_prScaleTimes[i] = Key.GetTime();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsoluteRotation
//	Object:		
//	02-01-25:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetAbsoluteRotation(QUATERNION &	Q)
{
	_AbsQuat = Q;
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsoluteTranslation
//	Object:		
//	02-01-25:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetAbsoluteTranslation(VECTOR &	V)
{
	_vAbsTrans = V;
}

//-----------------------------------------------------------------------------
//	Name:		SetQuatKey
//	Object:		
//	02-01-25:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetQuatKey(unsigned int i, 
							   QUATERNION_KEY &	QK)
{
	QDT_ASSERT(i < _nNbrRotKeys);

	#ifdef USE_COMPRESSED_QUATERNIONS
		_pRotKeys[i].Compress(QK.GetQuaternion());
	#else
		_pRotKeys[i] = QK.GetQuaternion();
	#endif

	_pnRotTimes[i] = (unsigned short) QK.GetTime();
}

//-----------------------------------------------------------------------------
//	Name:		SetTransKey
//	Object:		
//	02-01-25:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetTransKey(unsigned int i, 
								VECTOR_KEY & VK)
{
	QDT_ASSERT(i < _nNbrTransKeys);
	_pTransKeys[i] = VK.GetVector();
	_pTransKeys[i].SetPad(VK.GetTime());
}

//-----------------------------------------------------------------------------
//	Name:		SetAnimTops
//	Object:		
//	02-01-18:	RMA - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetAnimTops(QDT_VECTOR <ANIM_TOP> &	vAnimTops)
{
	QDT_ASSERT(_pAnimTops == NULL);

	_nNbrAnimTops = vAnimTops.GetSize();

	if (_nNbrAnimTops > 0)
	{
		_pAnimTops = new ANIM_TOP [_nNbrAnimTops];
	}

	unsigned int i;
	for (i = 0 ; i < _nNbrAnimTops ; ++i)
	{
		_pAnimTops[i] = vAnimTops[i];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetMatrix
//	Object:		
//	01-10-03:	GHP - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA::SetMatrix(const Float32 &	rFrame,
						  MATRIX &		mat)
{
	QUATERNION		Q;
	VECTOR			V;

	VECTOR_KEY		Vk; 
	QUATERNION_KEY	Qk;  

	mat.GetQuaternion( Q );
	V  = mat.GetTranslation(); 

	Vk.SetTime(rFrame);
	Qk.SetTime(rFrame);

	Vk.SetVector(V) ; 
	Qk.SetQuaternion(Q) ; 

	QDT_FAIL();
	/*
	_vTransKeys.PushTail(Vk);
	_vRotKeys.PushTail(Qk);*/ 

	return (_nNbrTransKeys);
}

//-----------------------------------------------------------------------------
//	Name:		GetSizeOf
//	Object:		
//	02-02-06:	DAC - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA::GetSizeOf() const
{
	int nSize;

	nSize = sizeof(TRACK_DATA);

	if	(_pTrackIndexes)
	{
		nSize += sizeof(TRACK_INDEXES);
	}

//#ifdef USE_COMPRESSED_QUATERNIONS
	nSize += sizeof(QUATERNION) * _nNbrRotKeys;
/*#else
	nSize += sizeof(COMPRESSED_QUATERNION) * _nNbrRotKeys;
#endif*/

	nSize += sizeof(VECTOR) * _nNbrTransKeys;
	nSize += sizeof(VECTOR) * _nNbrScaleKeys;
	nSize += sizeof(ANIM_TOP) * _nNbrAnimTops;
	nSize += sizeof(Float32) * (_nNbrRotKeys + _nNbrTransKeys + _nNbrScaleKeys);

	return	(nSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrQuatKeys
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetNbrQuatKeys(int	n)
{
	_nNbrRotKeys = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrTransKeys
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::SetNbrTransKeys(int	n)
{
	_nNbrTransKeys = n;
}

//-----------------------------------------------------------------------------
//	Name:		ResetTrack
//	Object:		
//	01-07-31:	GHP - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA::ResetTrack()
{
	_bNeedReset	= false;

	if (_bReverse)
	{
		_pTrackIndexes->SetRotIndex		(_nNbrRotKeys - 1);
		_pTrackIndexes->SetOldRotIndex	(_nNbrRotKeys - 1);
		_pTrackIndexes->SetTransIndex	(_nNbrTransKeys -1); 
		_pTrackIndexes->SetOldTransIndex(_nNbrTransKeys -1);  	
		_pTrackIndexes->SetScaleIndex	(_nNbrScaleKeys -1);
		_pTrackIndexes->SetOldScaleIndex(_nNbrScaleKeys -1);
	}
	else
	{
		_pTrackIndexes->SetRotIndex		(0);
		_pTrackIndexes->SetOldRotIndex	(0);	
		_pTrackIndexes->SetTransIndex	(0); 
		_pTrackIndexes->SetOldTransIndex(0); 	
		_pTrackIndexes->SetScaleIndex	(0); 
		_pTrackIndexes->SetOldScaleIndex(0);
	}
}

//-----------------------------------------------------------------------------
// Name: TRACK_DATA::ClearIterators
// Object: 
//01-05-11 GHP Created: 
//-----------------------------------------------------------------------------
void TRACK_DATA::ClearTrackIndexes() 
{		
	_pTrackIndexes->SetRotIndex		(0);
	_pTrackIndexes->SetOldRotIndex	(0);	
	_pTrackIndexes->SetTransIndex	(0); 
	_pTrackIndexes->SetOldTransIndex(0); 	
	_pTrackIndexes->SetScaleIndex	(0); 
	_pTrackIndexes->SetOldScaleIndex(0);
	
	_bNeedReset = true;
}

//-----------------------------------------------------------------------------
//	Name:		CheckFirstRot
//	Object:		If we are already behind the first frame return the first value 
//	01-04-18:	GHP - Created
//-----------------------------------------------------------------------------
bool TRACK_DATA::CheckFirstRot( const Float32 rValue, QUATERNION& Q ) 
{
	QDT_ASSERT(_pRotKeys);
	
	if (_pnRotTimes[0] >= rValue) 
	{
		Q = _pRotKeys[0];
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		CheckLastRot
//	Object:		If we are already behind the last frame return the last value 
//	01-04-18:	GHP - Created
//-----------------------------------------------------------------------------
bool TRACK_DATA::CheckLastRot(const Float32	rValue, 
							  QUATERNION &	Q) 
{
	QDT_ASSERT(_pRotKeys);

	if (_pnRotTimes[_nNbrRotKeys - 1] <= rValue)
	{
		#ifdef USE_COMPRESSED_QUATERNIONS
			_pRotKeys[_nNbrRotKeys - 1].Uncompress(Q);
		#else
			Q = _pRotKeys[_nNbrRotKeys - 1];
		#endif

		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetEvent
//	Object:		
//	01-06-14:	GHP - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA::GetAnimTops(Float32				rFrame, 
						    QDT_DLIST<int> &	vAnimTopsValues)
{
	vAnimTopsValues.Clear();

	// Check nbr anim tops
	if (_nNbrAnimTops == 0)	return (0);

	if (CheckLastAnimTop(rFrame, vAnimTopsValues)) return (vAnimTopsValues.GetSize());

	Float32 rT1, rT2, rVal1;
	
	UInt32 nAnimTopIndex	= _pTrackIndexes->GetAnimTopIndex();
	UInt32 nOldAnimTopIndex	= _pTrackIndexes->GetOldAnimTopIndex();
	
	for (; nAnimTopIndex < _nNbrAnimTops ; ++nAnimTopIndex)
	{
		rT2		= _pAnimTops[nAnimTopIndex].GetTime();
		rT1		= _pAnimTops[nOldAnimTopIndex].GetTime();

		rVal1	= _pAnimTops[nOldAnimTopIndex].GetValue();

		nOldAnimTopIndex = nAnimTopIndex;

		if (rFrame >= rT1  && rFrame < rT2) 
		{
			if (_nLastVal != rVal1)
			{
				_nLastVal = (int) rVal1;
				vAnimTopsValues.Push(_nLastVal, _bReverse);

				_pTrackIndexes->SetAnimTopIndex(nAnimTopIndex);
				_pTrackIndexes->SetOldAnimTopIndex(nOldAnimTopIndex);

				return (vAnimTopsValues.GetSize()); 
			}	
		}	
	}

	nAnimTopIndex = 0;

	_pTrackIndexes->SetAnimTopIndex(nAnimTopIndex);
	_pTrackIndexes->SetOldAnimTopIndex(nOldAnimTopIndex);

	return (vAnimTopsValues.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetNextTop
//	Object:		
//	01-07-26:	GHP - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA::GetNextAnimTop(Float32 &	rTime,
							   int &	nCode)
{
	Float32	rFrame = rTime;
	int		nSaveIndex;
	UInt32  nKeyIndex = _pTrackIndexes->GetAnimTopIndex(); 
	Float32	rT1;
	int		nVal1;
	
	rTime = 0.0f;

	// No elements no need to stay !
	if (_nNbrAnimTops == 0)	return (0);
	
	// saves iterators 
	nSaveIndex	= nKeyIndex; 
	
	for (; nKeyIndex < _nNbrAnimTops ; ++nKeyIndex)
	{
		rT1			= _pAnimTops[nKeyIndex].GetTime();
		nVal1		= _pAnimTops[nKeyIndex].GetValue();
		
		if (rFrame >= rT1) 
		{
			if (nVal1 != 0 && nCode != nVal1)
			{
				nCode = nVal1 ;
				// restores  iterators 
				nKeyIndex	= nSaveIndex;

				_pTrackIndexes->SetAnimTopIndex(nKeyIndex);

				return (1);
			}	
		}	
	}

	nKeyIndex	= nSaveIndex;
	_pTrackIndexes->SetAnimTopIndex(nKeyIndex);
	
	return (0);
}

//-----------------------------------------------------------------------------
//	Name:		GetQuatKey
//	Object:		
//	02-04-02:	CVI - Created
//-----------------------------------------------------------------------------
void TRACK_DATA::GetQuatKey(UInt32 n,
							QUATERNION & Q) const
{
	QDT_ASSERT(n < _nNbrRotKeys);
	Q = _pRotKeys[n];
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

