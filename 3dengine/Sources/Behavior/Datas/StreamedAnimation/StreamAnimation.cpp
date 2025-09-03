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
//	CLASS:	STREAM_ANIMATION
//
//	03-05-28:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamAnimation.h"

#include	INCL_KCOM(Streaming/CommunicatorPartitionDescriptor)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamTrackHeader)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamableAnimWriter)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/AnimBlock)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include "StreamAnimation.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_ANIMATION constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------

STREAM_ANIMATION::STREAM_ANIMATION()
:
STREAMABLE(GetGenericCommunicatorDescriptor()),
_nNbrFrames(0),
_nNbrTracks(0),
_pTracks(NULL),
_rFPS (30.0f)
{
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_ANIMATION constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_ANIMATION::STREAM_ANIMATION(CLASS_DESCRIPTOR * const	pCD)
:
STREAMABLE( pCD ),
_nNbrFrames( 0 ),
_nNbrTracks( 0 ),
_pTracks( NULL ),
_rFPS ( 30.0f )
{
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_ANIMATION destructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_ANIMATION::~STREAM_ANIMATION()
{
	_pTracks = NULL;		//	We'll have to seee wether to delete the array or not
}

///////////////////////////////////////////////////////////////
//	COMMUNICATOR
///////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

#ifndef _MASTER_ONLY_DATABANK
	pCD->AddReadChunk("SA_H", 1 ,CM_ReadHeaderChunkV1);
#endif
#ifndef _MASTER	
	pCD->AddWriteChunk("SA_H", 1 ,CM_WriteHeaderChunkV1);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_PARTITION_DESCRIPTOR *	STREAM_ANIMATION::GetGenericCommunicatorDescriptor()
{
	return ( (COMMUNICATOR_PARTITION_DESCRIPTOR *)COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAM_ANIMATION) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_PARTITION_DESCRIPTOR *	STREAM_ANIMATION::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_PARTITION_DESCRIPTOR(	QDT_COMMENT("STREAM_ANIMATION"), 
													MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAM_ANIMATION, 
													QDT_COMMENT("627D7423-83DC-35DE-4259-1E7F2358D2B9"), 
													sizeof(STREAM_ANIMATION), 
													COMInit, 
													QDT_COMMENT("Streamed animation data"), STREAMABLE::GetGenericCommunicatorDescriptor() ));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	STREAM_ANIMATION::GetClassGUID() const
{
	return ( _sClassGUID );
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::EM_Construct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	STREAM_ANIMATION	*pA = static_cast<STREAM_ANIMATION*> ( pO );
	QDT_NEW_PLACED( pA, STREAM_ANIMATION ) ();

}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-03-01:	VMA - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::EM_Construct_In_Memory(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::EM_Destruct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	STREAM_ANIMATION	*pA = static_cast<STREAM_ANIMATION*> ( pO );
	
	QDT_DELETE_PLACED( pA, STREAM_ANIMATION );
}


//-----------------------------------------------------------------------------
//	Name:		CreateBlock
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK * STREAM_ANIMATION::CreateBlock(int nID, float rStartTime, float rEndTime, const char * pBuffer, int nSize)
{
	ANIM_BLOCK	*pB;

	pB = new ANIM_BLOCK(nID, rStartTime, rEndTime, pBuffer, nSize);

	pB->InitTracks( pBuffer );

	return ( static_cast<STREAM_BLOCK*>(pB) );
}

//-----------------------------------------------------------------------------
//	Name:		CreatePartitionWriter
//	Object:		
//	03-06-04:	VMA - Created
//-----------------------------------------------------------------------------
STREAMABLE_PARTITION_WRITER *	STREAM_ANIMATION::CreatePartitionWriter()
{
	COM_INSTANCE	CI;

	return ( new STREAMABLE_ANIM_WRITER( GetStreamDuration(), GetPartitionDuration(), COM_INSTANCE(this), CI ) );
}

//-----------------------------------------------------------------------------
//	Name:		ReadSpecificStreamHeader
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	STREAM_ANIMATION::ReadSpecificStreamHeader(IO_STREAM &	Stream)
{
	//////////////////////
	Stream >> _nNbrFrames;
	Stream >> _nNbrTracks;
	Stream >> _rFPS;
	//////////////////////

	//////////////////////
	ReadTrackHeader( Stream );
	//////////////////////

}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteSpecificStreamHeader
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	STREAM_ANIMATION::WriteSpecificStreamHeader(IO_STREAM &	Stream)
{
	//////////////////////
	Stream << _nNbrFrames;
	Stream << _nNbrTracks;
	Stream << _rFPS;
	//////////////////////

	//////////////////////
	WriteTrackHeader( Stream );
	//////////////////////
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadTrackHeader
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	STREAM_ANIMATION::ReadTrackHeader(IO_STREAM &	Stream)
{
	int		i;

	/////////////////////////////////////
	_pTracks = new STREAM_TRACK_HEADER[_nNbrTracks];
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		Stream >> _pTracks[i];
	}
	/////////////////////////////////////
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteTrackHeader
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	STREAM_ANIMATION::WriteTrackHeader(IO_STREAM &	Stream)
{
	int		i;

	/////////////////////////////////////
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		Stream << _pTracks[i];
	}
	/////////////////////////////////////
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_ReadHeaderChunkV1
//	Object:		
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	STREAM_ANIMATION::CM_ReadHeaderChunkV1(IO_STREAM &		Stream,
											   COMMUNICATOR *	pCommunicator)
{
	STREAM_ANIMATION	*pS;

	pS = static_cast<STREAM_ANIMATION*> ( pCommunicator );
	pS->ReadSpecificStreamHeader( Stream );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteHeaderChunkV1
//	Object:		²
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	STREAM_ANIMATION::CM_WriteHeaderChunkV1(IO_STREAM &		Stream,
												COMMUNICATOR *	pCommunicator)
{
	STREAM_ANIMATION	*pS;

	pS = static_cast<STREAM_ANIMATION*> ( pCommunicator );
	pS->WriteSpecificStreamHeader( Stream );

}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetTrackHeaderArray
//	Object:		
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::SetTrackHeaderArray(STREAM_TRACK_HEADER *	pST)
{
	_pTracks = pST;
}

//-----------------------------------------------------------------------------
//	Name:		GetTrackHeader
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_HEADER *	STREAM_ANIMATION::GetTrackHeader(int	nNum) const
{
	return ( &(_pTracks[nNum]) );
}

//-----------------------------------------------------------------------------
//	Name:		SetTrackHeader
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION::SetTrackHeader(const STREAM_TRACK_HEADER &	TH,
										 int							nNumTrack)
{
	_pTracks[nNumTrack] = TH;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
