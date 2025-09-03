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
//	CLASS:	STREAMABLE_ANIM_WRITER
//
//	03-05-28:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamableAnimWriter.h"

#include	INCL_KCORE(Stream/MemStream)
#include	INCL_3DENGINE(Behavior/Datas/AnimPC)
#include	INCL_3DENGINE(Behavior/Datas/TrackData)
#include	INCL_3DENGINE(Behavior/Datas/TrackIndexes)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/AnimBlock)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamTrackHeader)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamAnimation)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamTrack)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_ANIM_WRITER constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_ANIM_WRITER constructor
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
STREAMABLE_ANIM_WRITER::STREAMABLE_ANIM_WRITER(float				rDuration,
											   float				rTimeDurationPerPartition,
											   const COM_INSTANCE &	Streamable,
											   const COM_INSTANCE &	Offset)
											   :
STREAMABLE_PARTITION_WRITER( rDuration, rTimeDurationPerPartition, Streamable, Offset )
{
	_pCurrentBlock	= NULL;
	_nNbrTracks		= 0;
	_pTrackHeader	= NULL;
	_nNbrFrames		= 0;
}




//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_ANIM_WRITER destructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAMABLE_ANIM_WRITER::~STREAMABLE_ANIM_WRITER()
{
	if ( _AnimPc.HasHandle() )
	{
		_AnimPc.Release();
	}

	if ( _pCurrentBlock )
	{
		delete _pCurrentBlock;
	}

	if ( _pTrackHeader )
	{
		delete [] _pTrackHeader;
	}

	ResetTracks();
}

//-----------------------------------------------------------------------------
//	Name:		SetAnim
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::SetAnim(const COM_INSTANCE &	Anim)
{
	_AnimPc = Anim;
}

//-----------------------------------------------------------------------------
//	Name:		GetAnim
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	STREAMABLE_ANIM_WRITER::GetAnim() const
{
	return ( _AnimPc );
}


//-----------------------------------------------------------------------------
//	Name:		WriteSpecificData
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::WriteSpecificData(MEM_STREAM *	pS,
												  int			nNumPartition,
												  float			rDuration)
{
#ifndef _MASTER
	float		rS, rE, rVal;


	/////////////////////////////////////////
	rVal	= GetPartitionDuration();		
	rS		= rVal * nNumPartition;
	rE		= rS + rDuration;
	/////////////////////////////////////////

	/////////////////////////////////////////
	BuildAllTracks( rS, rE );
	WriteAllTracks( pS );
	/////////////////////////////////////////
#endif
}

//-----------------------------------------------------------------------------
//	Name:		WriteSpecificHeaderInfo
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::WriteSpecificHeaderInfo(MEM_STREAM *	pS,
														int				nNumPartition)
{
	
}


//-----------------------------------------------------------------------------
//	Name:		InitTrackHeader
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::InitTrackHeader()
{
	////////////////////////////////////////////////////
	if ( _AnimPc.HasHandle() )
	{
		InitFromAnimPc();
	}
	else
	{
		InitFromStreamAnimation();
	}

	_nNbrFrames = (int) (GetFPS() * GetDuration());

	////////////////////////////////////////////////////
	STREAM_ANIMATION	*pS = static_cast<STREAM_ANIMATION*> ( GetAttachedStreamable() );
	pS->SetNbrFrames( _nNbrFrames );
	pS->SetNbrTracks( _nNbrTracks );
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetFPS
//	Object:		
//	04-03-17:	AMA - Created
//-----------------------------------------------------------------------------
float	STREAMABLE_ANIM_WRITER::GetFPS()
{
	float rFPS = 0.0f;
	if ( _AnimPc.HasHandle() )
	{
		ANIM_PC		*pAnim;
		pAnim = static_cast<ANIM_PC*> ( _AnimPc.GetCommunicator() );
        rFPS = pAnim->GetFPS();
	}
	else
	{
		STREAM_ANIMATION	*pSA;
		pSA = static_cast<STREAM_ANIMATION*> ( GetAttachedStreamable() );
		QDT_ASSERT( pSA );
		rFPS = pSA->GetBaseFrameRate();
	}

	return ( rFPS );
}

//-----------------------------------------------------------------------------
//	Name:		InitFromAnimPc
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::InitFromAnimPc()
{
	ANIM_PC		*pAnim;

	////////////////////////////////////////////////////
	pAnim = static_cast<ANIM_PC*> ( _AnimPc.GetCommunicator() );
	_nNbrTracks = pAnim->GetNbTracks();
	_nNbrFrames = pAnim->GetNbFrames();
	if ( _pTrackHeader )	delete[] _pTrackHeader;
	_pTrackHeader = new STREAM_TRACK_HEADER[_nNbrTracks];
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	InitAllTrackHeader();
	////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		InitFromStreamAnimation
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::InitFromStreamAnimation()
{
	STREAM_ANIMATION	*pSA;

	//////////////////////////////////////////
	pSA = static_cast<STREAM_ANIMATION*> ( GetAttachedStreamable() );
	QDT_ASSERT( pSA );
	//////////////////////////////////////////

	//////////////////////////////////////////
	_nNbrTracks = pSA->GetNbrTracks();
	_nNbrFrames = pSA->GetNbrFrames();
	if ( _pTrackHeader )	delete[] _pTrackHeader;
	_pTrackHeader = new STREAM_TRACK_HEADER[_nNbrTracks];
	//////////////////////////////////////////	

	//////////////////////////////////////////	
	CopyTrackHeader();
	//////////////////////////////////////////	
}

//-----------------------------------------------------------------------------
//	Name:		CopyTrackHeader
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::CopyTrackHeader()
{
	int							i;
	STREAM_TRACK_HEADER			*pSTH;
	STREAM_TRACK_HEADER			*pSTH2;
	STREAM_ANIMATION			*pSA;

	//////////////////////////////////////////
	pSA = static_cast<STREAM_ANIMATION*> ( GetAttachedStreamable() );
	QDT_ASSERT( pSA );
	//////////////////////////////////////////

	//////////////////////////////////////////
	pSTH = _pTrackHeader;
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		/////////////////////////////////
		pSTH2 = pSA->GetTrackHeader( i );
		QDT_ASSERT( pSTH2 );
		/////////////////////////////////

		/////////////////////////////////
		*pSTH = *pSTH2;
		pSTH++;
		/////////////////////////////////
	}
	//////////////////////////////////////////
}



//-----------------------------------------------------------------------------
//	Name:		InitAllTrackHeader
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::InitAllTrackHeader()
{
	int						i;
	TRACK_DATA				*pT;
	STREAM_TRACK_HEADER		*pSTH;
	ANIM_PC					*pAnim;
	QUATERNION				Q;
	VECTOR					V;
	VECTOR					S;


	////////////////////////////////////////////////////
	pAnim	= static_cast<ANIM_PC*> ( _AnimPc.GetCommunicator() );
	pSTH	= _pTrackHeader;
	////////////////////////////////////////////////////


	////////////////////////////////////////////////////
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		///////////////////////////
		pT = pAnim->GetTrack(i);
		pT->GetBaseRotation(Q);
		pT->GetBaseTranslation(V);
		S = VECTOR(1.0f, 1.0f, 1.0f);				//	No scale
		///////////////////////////

		///////////////////////////
		pSTH->SetAbsRot(Q);
		pSTH->SetAbsTrans(V);
		pSTH->SetAbsScale(S);
		///////////////////////////

		///////////////////////////
		pSTH++;											//	Next track header
		///////////////////////////
	}
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	STREAM_ANIMATION	*pS = static_cast<STREAM_ANIMATION*> ( GetAttachedStreamable() );
	/*pS->SetNbrFrames( _nNbrFrames );
	pS->SetNbrTracks( _nNbrTracks );*/
	pS->SetTrackHeaderArray( _pTrackHeader );
	_pTrackHeader = NULL;
	////////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		BuildAllTracks
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::BuildAllTracks(float	rStartTime,
											   float	rEndTime)
{
	int			i;

	/////////////////////////////////////////////////
	ResetTracks();
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		BuildSingleTrack( i, rStartTime, rEndTime );
	}
	/////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		BuildSingleTrack
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::BuildSingleTrack(int	nNum,
												 float	rS,
												 float	rE)
{
	STREAM_TRACK			*pST;
	TRACK_DATA				*pT;
	ANIM_PC					*pA;
	TRACK_DATA				T;
	int						nNbrQuatKeys;
	int						nNbrTransKeys;
	QUATERNION_KEY			QK;
	VECTOR_KEY				VK;
	bool					bDeleteTrackData;
	STREAMABLE				*pS = GetAttachedStreamable();
	float					rTemporisation = pS->GetTemporisation();

	///////////////////////////////////////////////////////////
	if ( MustLoadPartitions() == false )
	{
		bDeleteTrackData = false;
		pA = static_cast<ANIM_PC*> ( _AnimPc.GetCommunicator() );
		pT = pA->GetTrack( nNum );
	}
	else
	{
		float	rSS, rEE;

		rSS	= rS - rTemporisation;
		rEE	= rE - rTemporisation;
		bDeleteTrackData = true;
		pT = GetTrackData( nNum, rSS, rEE );
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	rS = rS * 30.0f;
	rE = rE * 30.0f;
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	pST = new STREAM_TRACK();
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	nNbrQuatKeys	= FillQuat( pT, T, rS, rE );
	nNbrTransKeys	= FillTrans( pT, T, rS, rE );
	pST->SetNbrQuatKeys( nNbrQuatKeys );
	pST->SetNbrTransKeys( nNbrTransKeys );
	AddStreamTrack( pST );
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	FillQuatBuffer( pST, T, nNbrQuatKeys );
	FillTransBuffer( pST , T, nNbrTransKeys );
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	if ( bDeleteTrackData )	delete pT;
	///////////////////////////////////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		FillQuatBuffer
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::FillQuatBuffer(STREAM_TRACK *	pST,
											   TRACK_DATA &		T,
											   int				nNbrQuatKeys)
{
	int					i;
	QUATERNION_KEY		*pQK;
	QUATERNION_KEY		QK;

	////////////////////////////////////////
	if ( nNbrQuatKeys == 0 )
	{
		pST->SetQuatBuffer( NULL );
		return;
	}
	////////////////////////////////////////

	////////////////////////////////////////
	pQK = new QUATERNION_KEY[nNbrQuatKeys];
	for ( i = 0; i < nNbrQuatKeys; ++i )
	{
		QUATERNION Q;
		T.GetQuatKey(i, Q);
		QK.SetQuaternion(Q);
		QK.SetTime(T.GetQuatTime(i));
		pQK[i] = QK;
	}
	////////////////////////////////////////

	////////////////////////////////////////
	pST->SetQuatBuffer( pQK );
	////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		FillTransBuffer
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::FillTransBuffer(STREAM_TRACK *	pST,
												TRACK_DATA &	T,
												int				nNbrTransKeys)
{
	int					i;
	VECTOR_KEY			*pVK;
	VECTOR_KEY			VK;

	////////////////////////////////////////
	if ( nNbrTransKeys == 0 )
	{
		pST->SetTransBuffer( NULL );
		return;
	}
	pVK = new VECTOR_KEY[nNbrTransKeys];
	for ( i = 0; i < nNbrTransKeys; ++i )
	{
		VECTOR V;
		T.GetTransKey(i, V);
		VK.SetVector(V);
		VK.SetTime(T.GetTransTime(i));
		pVK[i] = VK;
	}
	////////////////////////////////////////

	////////////////////////////////////////
	pST->SetTransBuffer( pVK );
	////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		FillQuat
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAMABLE_ANIM_WRITER::FillQuat(TRACK_DATA *   pT, 
									 TRACK_DATA &	T,
									 float			rS,
									 float			rE)
{
	QDT_VECTOR <QUATERNION_KEY> vQuatKeys;
	TRACK_INDEXES				TrackIdx;
	QUATERNION_KEY				QK;
	QUATERNION					Q;
	int							nNbrQuatKeys;
	int							nMax;
	int							nCount;
	STREAMABLE *				pS = NULL;
	float						rTemporisation;
	float						rTime;
	float						rLastTime;
	


	////////////////////
	nMax = pT->GetNbrRotKeys();
	if ( nMax <= 0 )
	{
		return ( 0 );
	}
	////////////////////

	////////////////////
	rTemporisation = 0.0f;
	if ( MustLoadPartitions() == false )
	{
		pS = GetAttachedStreamable();
		QDT_ASSERT( pS );
		rTemporisation = pS->GetTemporisation() * 30.0f;
	}
	////////////////////

	////////////////////
	//	Init first key
	nNbrQuatKeys = 0;
	pT->SetTrackIndexes( &TrackIdx );
	pT->GetRot( rS - rTemporisation, Q );
	QK.SetQuaternion( Q );
	QK.SetTime( rS );
	rLastTime = rS;
	vQuatKeys.PushTail(QK);
	nNbrQuatKeys++;
	////////////////////

	////////////////////
	nCount = TrackIdx.GetRotIndex();
	while ( nCount < nMax )
	{
		////////////////////
		QUATERNION Q;
		pT->GetQuatKey(nCount, Q);
		QK.SetQuaternion(Q);
		QK.SetTime(pT->GetQuatTime(nCount));
		rTime = QK.GetTime() + rTemporisation;
		QK.SetTime(rTime);
		if (QK.GetTime() > rE)
		{
			break;
		}
		////////////////////

		////////////////////
		if ( MATH::Abs( rTime - rLastTime ) > MATH::EPSILON() )
		{
			vQuatKeys.PushTail(QK);
			nNbrQuatKeys++;
			rLastTime = rTime;
		}
		nCount++;
		////////////////////
	}
	////////////////////

	////////////////////
	if ( QK.GetTime() < rE )
	{
		if ( MATH::Abs( rE - rLastTime ) > MATH::EPSILON() )
		{
			QK.SetTime( rE );
			vQuatKeys.PushTail(QK);
			nNbrQuatKeys++;
		}
	}
	else
	{
		if (MATH::Abs( rE - rLastTime ) > MATH::EPSILON())
		{
			pT->GetRot(rE, Q);
			QK.SetQuaternion(Q);
			QK.SetTime(rE);
			vQuatKeys.PushTail(QK);
			nNbrQuatKeys++;
		}
	}
	////////////////////

	T.SetRotations(vQuatKeys);

	////////////////////
	return ( nNbrQuatKeys );
	////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		FillTrans
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAMABLE_ANIM_WRITER::FillTrans(TRACK_DATA* pT, 
									  TRACK_DATA &	T,
									  float			rS,
									  float			rE)
{
	QDT_VECTOR<VECTOR_KEY>	vTransKeys;
	TRACK_INDEXES			TrackIdx;
	VECTOR_KEY				VK;
	VECTOR					V;
	int						nNbrTransKeys;
	int						nMax;
	int						nCount;
	STREAMABLE				*pS = NULL;
	float					rTemporisation;
	float					rTime;
	float					rLastTime;



	////////////////////
	nMax = pT->GetNbrTransKeys();
	if ( nMax <= 0 )
	{
		return ( 0 );
	}
	////////////////////

	////////////////////
	rTemporisation = 0.0f;
	if ( MustLoadPartitions() == false )
	{
		pS = GetAttachedStreamable();
		QDT_ASSERT( pS );
		rTemporisation = pS->GetTemporisation() * 30.0f;
	}
	////////////////////

	////////////////////
	//	Init first key
	nNbrTransKeys = 0;
	pT->SetTrackIndexes( &TrackIdx );
	pT->GetTrans(rS, V);
	VK.SetVector(V);
	VK.SetTime(rS);
	rLastTime = rS;
	vTransKeys.PushTail(VK);
	nNbrTransKeys++;
	////////////////////

	////////////////////
	nCount = TrackIdx.GetTransIndex();
	while ( nCount < nMax )
	{
		////////////////////
		VECTOR V;
		pT->GetTransKey(nCount, V);
		VK.SetVector(V);
		VK.SetTime(pT->GetTransTime(nCount));
		rTime = VK.GetTime() + rTemporisation;
		VK.SetTime( rTime );
		if (VK.GetTime() > rE)
		{
			break;
		}
		////////////////////

		////////////////////
		if ( MATH::Abs( rTime - rLastTime ) > MATH::EPSILON() )
		{
			vTransKeys.PushTail( VK );
			nNbrTransKeys++;
			rLastTime = rTime;
		}
		nCount++;
		////////////////////
	}
	////////////////////

	////////////////////
	if ( VK.GetTime() < rE )
	{
		if ( MATH::Abs( rE - rLastTime ) > MATH::EPSILON() )
		{
			VK.SetTime( rE );
			vTransKeys.PushTail( VK );
			nNbrTransKeys++;
		}
	}
	else
	{
		if ( MATH::Abs( rE - rLastTime ) > MATH::EPSILON() )
		{
			pT->GetTrans( rE, V );
			VK.SetVector( V );
			VK.SetTime( rE );
			vTransKeys.PushTail( VK );
			nNbrTransKeys++;
		}
	}
	////////////////////
	T.SetTranslation( vTransKeys );

	////////////////////
	return ( nNbrTransKeys );
	////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		FillAnimTop
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAMABLE_ANIM_WRITER::FillAnimTop(TRACK_DATA* pT, 
											TRACK_DATA &	T,
											float			rS,
											float			rE)
{
	/* no anim top for now */
	return ( 0 );
}



//-----------------------------------------------------------------------------
//	Name:		AddStreamTrack
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::AddStreamTrack(STREAM_TRACK *	pST)
{
	_vCurrentTrack.PushTail( pST );
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamTrack
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK *	STREAMABLE_ANIM_WRITER::GetStreamTrack(int	nNum)
{
	return ( _vCurrentTrack[nNum] );
}

//-----------------------------------------------------------------------------
//	Name:		ResetTracks
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::ResetTracks()
{
	if ( _vCurrentTrack.IsEmpty() == false )
	{
		_vCurrentTrack.ClearAll();
	}
}


//-----------------------------------------------------------------------------
//	Name:		WriteAllTracks
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::WriteAllTracks(MEM_STREAM *	pS)
{
#ifndef _MASTER
	int				i;
	STREAM_TRACK	*pST;
	
	(*pS) << _nNbrTracks;
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		pST = GetStreamTrack( i );
		pST->FillStream( pS );
	}
#endif
}


//-----------------------------------------------------------------------------
//	Name:		WriteFromBlocks
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::WriteFromBlocks(MEM_STREAM *	pS,
												int				nNumPartition,
												float			rDuration)
{
#ifndef _MASTER
	float			rS, rE;

	/////////////////////////////////////////
	GetTimeFromPartitionIndex( nNumPartition, rS, rE );
	rE = rS + rDuration;
	/////////////////////////////////////////

	/////////////////////////////////////////
	BuildAllTracks( rS, rE );
	WriteAllTracks( pS );
	/////////////////////////////////////////
#endif
}


//-----------------------------------------------------------------------------
//	Name:		GetTrackData
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
TRACK_DATA *	STREAMABLE_ANIM_WRITER::GetTrackData(int	nNum,
													 float	rS,
													 float	rE)
{
	TRACK_DATA					*pT;
	ANIM_BLOCK					*pB;
	STREAMABLE					*pS;
	int							nBS, nBE;
	bool						bLoaded;
	STREAM_TRACK				*pST;
	float						rS2, rE2;
	int							nNumBlock;
	int							nCountT, nCountQ;
	int							nMaxT, nMaxQ;
	QUATERNION_KEY				QK;
	VECTOR_KEY					VK;
	VECTOR						V;
	QUATERNION					Q;
	TRACK_INDEXES				TrackIdx;
	float						rTime;
	float						rTemporisation;
	QDT_VECTOR<VECTOR_KEY>		vTransKeys;
	QDT_VECTOR<QUATERNION_KEY>	vQuaternionKey;

	//////////////////////////////
	pS				= GetAttachedStreamable();
	rTemporisation	= pS->GetTemporisation() * 30.0f;
	pT				= new TRACK_DATA();
	pST				= NULL;
	if ( rE <= 0.0f )	return ( pT );		//	Return empty track
	//////////////////////////////

	//////////////////////////////
	nBS = pS->GetBlockIndexFromTime( rS );
	if ( nBS < 0 )	nBS = 0;
	nBE = pS->GetBlockIndexFromTime( rE );
	if ( nBE < 0 )	nBE = 0;
	//////////////////////////////

	///////////////////////////////////////////////////////////
	rS2 = rS *30.0f;
	rE2 = rE *30.0f;
	///////////////////////////////////////////////////////////

	//////////////////////////////
	nMaxT			= 0;
	nMaxQ			= 0;
	nCountT			= 0;
	nCountQ			= 0;
	//////////////////////////////

	//////////////////////////////
	nNumBlock		= nBS;
	while ( nNumBlock <= nBE )
	{
		///////////////////////////////////////
		pB = static_cast<ANIM_BLOCK*> ( pS->QueryBlock( nNumBlock, true, bLoaded ) );
		QDT_ASSERT( bLoaded );
		///////////////////////////////////////

		///////////////////////////////////////
		pST		= pB->GetTrack( nNum );
		nMaxT	= pST->GetNbrTransKeys();
		nCountT = 0;
		///////////////////////////////////////

		///////////////////////////////////////
		while ( nCountT < nMaxT )
		{
			VK = pST->GetTransKey( nCountT );
			if ( VK.GetTime() < rS2 )
			{
				nCountT++;
				continue;
			}
			if ( VK.GetTime() > rE2 )
			{
				break;
			}
			if ( vTransKeys.GetSize() == 0 )
			{
				V = pST->GetTrans( TrackIdx, rS2 );
				VK.SetVector( V );
				VK.SetTime( rS2 );
			}
			rTime = VK.GetTime();
			rTime += rTemporisation;
			VK.SetTime( rTime );
			//
			vTransKeys.PushTail(VK);
			nCountT++;
		}
		///////////////////////////////////////

		///////////////////////////////////////
		if ( nMaxT > 0 && vTransKeys.GetSize() == 0 )
		{
			V = pST->GetTrans( TrackIdx, rS2 );
			VK.SetVector( V );
			VK.SetTime( rS2 + rTemporisation );
			vTransKeys.PushTail( VK );
		}
		///////////////////////////////////////

		///////////////////////////////////////
		nNumBlock++;
		///////////////////////////////////////
	}
	//////////////////////////////

	//////////////////////////////
	if ( vTransKeys.GetSize() > 0 )
	{
		V = pST->GetTrans( TrackIdx, rE2 );
		VK.SetVector( V );
		VK.SetTime( rE2 + rTemporisation);
		vTransKeys.PushTail( VK );
		pT->SetTranslation( vTransKeys );
	}
	//////////////////////////////

	//////////////////////////////
	nNumBlock		= nBS;
	while ( nNumBlock <= nBE )
	{
		///////////////////////////////////////
		pB = static_cast<ANIM_BLOCK*> ( pS->QueryBlock( nNumBlock, true, bLoaded ) );
		QDT_ASSERT( bLoaded );
		///////////////////////////////////////

		///////////////////////////////////////
		pST		= pB->GetTrack( nNum );
		nMaxQ	= pST->GetNbrQuatKeys();
		nCountQ = 0;
		///////////////////////////////////////

		///////////////////////////////////////
		while ( nCountQ < nMaxQ )
		{
			QK = pST->GetQuatKey( nCountQ );
			if ( QK.GetTime() < rS2 )
			{
				nCountQ++;
				continue;
			}
			if ( QK.GetTime() > rE2 )
			{
				break;
			}
			if ( vQuaternionKey.GetSize() == 0 )
			{
				Q = pST->GetRot( TrackIdx, rS2 );
				QK.SetQuaternion( Q );
				QK.SetTime( rS2 );
			}
			rTime = QK.GetTime();
			rTime += rTemporisation;
			QK.SetTime( rTime );
			//
			vQuaternionKey.PushTail( QK );
			nCountQ++;
		}
		///////////////////////////////////////

		///////////////////////////////////////
		if ( nMaxQ > 0 && vQuaternionKey.GetSize() == 0 )
		{
			Q = pST->GetRot( TrackIdx, rS2 );
			QK.SetQuaternion( Q );
			QK.SetTime( rS2 + rTemporisation );
			vQuaternionKey.PushTail( QK );
		}
		///////////////////////////////////////

		///////////////////////////////////////
		nNumBlock++;
		///////////////////////////////////////
	}
	//////////////////////////////

	//////////////////////////////
	if ( vQuaternionKey.GetSize() > 0 )
	{
		if ( pST->GetNbrQuatKeys() > 0 )
		{
			Q = pST->GetRot( TrackIdx, rE2 );
			QK.SetQuaternion( Q );
		}
		else
		{
			int	nNumQ = vQuaternionKey.GetSize()-1;
			QK = vQuaternionKey[nNumQ];
		}
		QK.SetTime( rE2 + rTemporisation );
		vQuaternionKey.PushTail(QK);
		pT->SetRotations(vQuaternionKey);
	}
	//////////////////////////////

	//////////////////////////////
	return ( pT );
	//////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		WriterSpecificInit
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::WriterSpecificInit()
{
#ifndef _MASTER
	InitTrackHeader();
#endif
}

//-----------------------------------------------------------------------------
//	Name:		FillStreamWithBlank
//	Object:		
//	03-06-18:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_ANIM_WRITER::FillStreamWithBlank(MEM_STREAM *	pMemStream,
													float rDuration)
{
	QDT_NOT_IMPLEMENTED();
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
