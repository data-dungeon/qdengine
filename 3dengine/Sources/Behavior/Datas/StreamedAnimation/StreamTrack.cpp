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
//	CLASS:	STREAM_TRACK
//
//	03-05-28:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamTrack.h"

#include	INCL_KCORE(Stream/MemStream)
#include	INCL_KSERV(AnimTools/QuaternionKey)
#include	INCL_KSERV(AnimTools/VectorKey)
#include	INCL_KSERV(AnimTools/ScaleKey)
#include	INCL_3DENGINE(Behavior/Datas/TopAnim)
#include	INCL_3DENGINE(Behavior/Datas/TrackIndexes)

#ifdef _DEBUG
	#include "StreamTrack.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK::STREAM_TRACK()
:
_nNbrQuatKeys(0),
_nNbrTransKeys(0),
_nNbrScaleKeys(0),
_nNbrAnimTops(0),
_pQuatKeys(NULL),
_pTransKeys(NULL),
_pScaleKeys(NULL),
_pAnimTops(NULL)
{
}


//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK destructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK::~STREAM_TRACK()
{
	_pQuatKeys	= NULL;
	_pTransKeys = NULL;
	_pScaleKeys = NULL;
	_pAnimTops	= NULL;
}


//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void*	STREAM_TRACK::Init( void *pBuffer )
{
	char				*pI;
	IntPtr				nPos;
	IntPtr				nTempPos;
	int					*pConvInt;

	pI			= static_cast<char*> ( pBuffer );
	pConvInt	= static_cast<int*> ( pBuffer );

	//	Init integer params
	_nNbrQuatKeys	= *pConvInt;pConvInt++;
	_nNbrTransKeys	= *pConvInt;pConvInt++;
	_nNbrScaleKeys	= *pConvInt;pConvInt++;
	_nNbrAnimTops	= *pConvInt;pConvInt++;
	//	Update buffer pointer
	pI				= ( char*) ( pConvInt );

	//	Alignement
	nPos		= ( IntPtr ) ( pI );
	nTempPos	= nPos;
	nTempPos	>>= 4;
	nTempPos	<<= 4;

	if ( nTempPos != nPos )
	{
		nTempPos += 0x10;
		pI += ( nTempPos - nPos );
	}
	
	//
	if ( _nNbrQuatKeys > 0 )
	{
		_pQuatKeys = (QUATERNION_KEY*) ( pI );
		pI +=  ( _nNbrQuatKeys * sizeof(QUATERNION_KEY) );
	}
	else
	{
		_pQuatKeys = NULL;
	}

	//
	if ( _nNbrTransKeys > 0 )
	{
		_pTransKeys = (VECTOR_KEY*) ( pI );
		pI +=  ( _nNbrTransKeys * sizeof(VECTOR_KEY) );
	}
	else
	{
		_pTransKeys = NULL;
	}

	//
	if ( _nNbrScaleKeys > 0 )
	{
		_pScaleKeys = (SCALE_KEY*) ( pI );
		pI +=  ( _nNbrScaleKeys * sizeof(SCALE_KEY) );
	}
	else
	{
		_pScaleKeys = NULL;
	}

	//
	if ( _nNbrAnimTops > 0 )
	{
		_pAnimTops = (ANIM_TOP*) ( pI );
		pI +=  ( _nNbrAnimTops * sizeof(ANIM_TOP) );
	}
	else
	{
		_pAnimTops = NULL;
	}
	
	//
	return ( static_cast<void*> ( pI ) );

}


//-----------------------------------------------------------------------------
//	Name:		GetRot
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
QUATERNION	STREAM_TRACK::GetRot(TRACK_INDEXES& TrackIndex, float	rFrame)
{
	int					nStart;
	QUATERNION			Q;
	QUATERNION			Q1, Q2;
	float				rT1, rT2;
	float				rT;
	float				rCoeff;
	int					i;

	/////////////////////////////////////////
	if ( _nNbrQuatKeys <= 0 )
	{
		Q.Id();
		return ( Q );
	}
	if ( rFrame >= _pQuatKeys[_nNbrQuatKeys - 1].GetTime() )
	{
		Q = _pQuatKeys[_nNbrQuatKeys - 1].GetQuaternion();
//		QDT_Message("Dépassement intervalle : rFrame = %f, rMaxTime = %f \n", rFrame, _pQuatKeys[_nNbrQuatKeys - 1].GetTime() );
		return ( Q );
	}
	if ( rFrame <= _pQuatKeys[0].GetTime() )
	{
		Q = _pQuatKeys[0].GetQuaternion();
//		QDT_Message("Dépassement intervalle : rFrame = %f, rMaxTime = %f \n", rFrame, _pQuatKeys[0].GetTime() );
		return ( Q );
	}
	/////////////////////////////////////////
	
	/////////////////////////////////////////
	nStart	= TrackIndex.GetOldRotIndex();
	if ( nStart >= _nNbrQuatKeys - 1 )
	{
		nStart = 0;
	}
	else
	{
		rT		= _pQuatKeys[nStart].GetTime();
		if ( rFrame < rT )
		{
			nStart = 0;
		}
	}
	/////////////////////////////////////////

	/////////////////////////////////////////

	for ( i = nStart; i < _nNbrQuatKeys - 1; ++i )
	{
		if ( _pQuatKeys[i+1].GetTime() < rFrame )
		{
			continue;
		}

		TrackIndex.SetOldRotIndex( i );
		TrackIndex.SetRotIndex( i + 1 );

		rT2 = _pQuatKeys[i + 1].GetTime();
		rT1 = _pQuatKeys[i].GetTime();

		Q1 = _pQuatKeys[i].GetQuaternion();
		Q2 = _pQuatKeys[i+1].GetQuaternion();

		rCoeff = ( rFrame - rT1 ) / ( rT2 - rT1 );
		Q.BasicInterSLERP( rCoeff, Q1, Q2 );
		return ( Q );
	}
	QDT_Message(QDT_COMMENT("Dépassement not found : rFrame = %f, rMaxTime = %f \n"), rFrame, _pQuatKeys[0].GetTime() );
	Q = _pQuatKeys[_nNbrQuatKeys - 1].GetQuaternion();
	return ( Q );
}

//-----------------------------------------------------------------------------
//	Name:		GetTrans
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
VECTOR	STREAM_TRACK::GetTrans(TRACK_INDEXES& TrackIndex, float	rFrame)
{
	int					nStart;
	VECTOR				V;
	VECTOR				V1, V2;
	float				rT1, rT2;
	float				rT;
	float				rCoeff;
	int					i;

	/////////////////////////////////////////
	if ( _nNbrTransKeys <= 0 )
	{
		V.SetXYZ( 0.0f, 0.0f, 0.0f );
		return ( V );
	}
	if ( rFrame >= _pTransKeys[_nNbrTransKeys - 1].GetTime() )
	{
		V = _pTransKeys[_nNbrTransKeys - 1].GetVector();
		return ( V );
	}
	if ( rFrame <= _pTransKeys[0].GetTime() )
	{
		V = _pTransKeys[0].GetVector();
		return ( V );
	}
	/////////////////////////////////////////


	/////////////////////////////////////////
	nStart	= TrackIndex.GetOldTransIndex();
	if ( nStart >= _nNbrTransKeys - 1 )
	{
		nStart = 0;
	}
	else
	{
		rT		= _pTransKeys[nStart].GetTime();
		if ( rFrame < rT )
		{
			nStart = 0;
		}
	}
	/////////////////////////////////////////

	/////////////////////////////////////////

	for ( i = nStart; i < _nNbrTransKeys - 1; ++i )
	{
		if ( _pTransKeys[i + 1].GetTime() < rFrame )
		{
			continue;
		}

		TrackIndex.SetOldTransIndex( i );
		TrackIndex.SetTransIndex( i + 1 );

		rT2 = _pTransKeys[i + 1].GetTime();
		rT1 = _pTransKeys[i].GetTime();

		V1 = _pTransKeys[i].GetVector();
		V2 = _pTransKeys[i+1].GetVector();

		rCoeff = ( rFrame - rT1 ) / ( rT2 - rT1 );
		V.InterLinear( rCoeff, V1, V2 );
		return ( V );
	}

	V = _pTransKeys[_nNbrTransKeys - 1].GetVector();
	return ( V );
}

//-----------------------------------------------------------------------------
//	Name:		GetScale
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
VECTOR	STREAM_TRACK::GetScale(TRACK_INDEXES& TrackIndex, float	rFrame)
{
	VECTOR	S;

	return ( S );
}

//-----------------------------------------------------------------------------
//	Name:		GetRotTrans
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR	STREAM_TRACK::GetRotTrans(TRACK_INDEXES& TrackIndex, float	rFrame)
{
	QUATERNION_VECTOR	QV;
	QUATERNION			Q;
	VECTOR				V;

	if ( _pQuatKeys )
	{
		QV._Quat = GetRot( TrackIndex, rFrame );
	}
	if ( _pTransKeys )
	{
		QV._Vect = GetTrans( TrackIndex, rFrame );
	}

	return ( QV );
}

//-----------------------------------------------------------------------------
//	Name:		SetQuatBuffer
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetQuatBuffer(QUATERNION_KEY *	pQK)
{
	_pQuatKeys = pQK;
}

//-----------------------------------------------------------------------------
//	Name:		SetTransBuffer
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetTransBuffer(VECTOR_KEY *	pVK)
{
	_pTransKeys = pVK;
}

//-----------------------------------------------------------------------------
//	Name:		FillStream
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::FillStream(MEM_STREAM *	pS)
{
	int				i;
	QUATERNION_KEY	QK;
	VECTOR_KEY		VK;
	QUATERNION		Q;
	VECTOR			V;
	float			w,x,y,z;
	float			rTime;
	int				nBufferPos;
	int				nTempBufferPos;
	char			c = 0;
	int				nDump = 0;

	(*pS) << _nNbrQuatKeys;
	(*pS) << _nNbrTransKeys;
	(*pS) << _nNbrScaleKeys;
	(*pS) << _nNbrAnimTops;

	//	Alignement
	nBufferPos		= pS->GetPos();
	nTempBufferPos	= nBufferPos;

	nTempBufferPos >>= 4;
	nTempBufferPos <<= 4;
	if ( nTempBufferPos != nBufferPos ) 
	{
		nTempBufferPos += 0x10;
		for ( i = 0; i < nTempBufferPos - nBufferPos; ++i )
		{
			(*pS) << c;
		}
	}

	// 
	for ( i = 0; i < _nNbrQuatKeys; ++i )
	{
		QK		= GetQuatKey( i );
		rTime	= QK.GetTime();
		Q		= QK.GetQuaternion();
		Q.GetWXYZ( w, x, y, z );
		(*pS) << x;
		(*pS) << y;
		(*pS) << z;
		(*pS) << w;
		(*pS) << rTime;
		(*pS) << nDump;
		(*pS) << nDump;
		(*pS) << nDump;
	}

	//
	for ( i = 0; i < _nNbrTransKeys; ++i )
	{
		VK		= GetTransKey( i );
		V		= VK.GetVector();
		rTime	= VK.GetTime();
		V.GetXYZ( x, y, z );

		(*pS) << x;
		(*pS) << y;
		(*pS) << z;
		(*pS) << (0.0f);	//	padding
		(*pS) << rTime;
		(*pS) << nDump;
		(*pS) << nDump;
		(*pS) << nDump;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetQuatKey
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const QUATERNION_KEY &	STREAM_TRACK::GetQuatKey(int	nNum) const
{
	return ( _pQuatKeys[nNum] );
}

//-----------------------------------------------------------------------------
//	Name:		GetTransKey
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const VECTOR_KEY &	STREAM_TRACK::GetTransKey(int	nNum) const
{
	return ( _pTransKeys[nNum] );
}

//-----------------------------------------------------------------------------
//	Name:		GetScaleKey
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const SCALE_KEY &	STREAM_TRACK::GetScaleKey(int	nNum) const
{
	return ( _pScaleKeys[nNum] );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimTop
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const ANIM_TOP &	STREAM_TRACK::GetAnimTop(int	nNum) const
{
	return ( _pAnimTops[nNum] );
}

//-----------------------------------------------------------------------------
//	Name:		SetQuatKey
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetQuatKey(const QUATERNION_KEY &	QK,
								 int					n)
{
	_pQuatKeys[n] = QK;
}

//-----------------------------------------------------------------------------
//	Name:		SetTransKey
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetTransKey(const VECTOR_KEY &	VK,
								  int n						)
{
	_pTransKeys[n] = VK;
}

//-----------------------------------------------------------------------------
//	Name:		SetScaleKey
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetScaleKey(const SCALE_KEY &	SK,
								  int n					)
{
	_pScaleKeys[n] = SK;
}

//-----------------------------------------------------------------------------
//	Name:		SetAnimTop
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK::SetAnimTop(const ANIM_TOP &	AT,
								 int n					)
{
	_pAnimTops[n] = AT;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
