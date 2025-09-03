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
//	CLASS:	STREAMABLE_BLEND_SHAPE_ANIM
//
//	03-06-05:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamableBlendShapeAnim.h"

#include	INCL_KCOM(Streaming/CommunicatorPartitionDescriptor)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnimWriter)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/BlendShapeAnimBlock)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM::STREAMABLE_BLEND_SHAPE_ANIM()
:
STREAMABLE	(GetGenericCommunicatorDescriptor())
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM::STREAMABLE_BLEND_SHAPE_ANIM(CLASS_DESCRIPTOR * const	pCD)
:
STREAMABLE	(pCD)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM destructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM::~STREAMABLE_BLEND_SHAPE_ANIM()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

#ifndef _MASTER_ONLY_DATABANK
	pCD->AddReadChunk("SBSA_H", 1 ,CM_ReadHeaderChunkV1);
#endif
#ifndef _MASTER
	pCD->AddWriteChunk("SBSA_H", 1 ,CM_WriteHeaderChunkV1);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_PARTITION_DESCRIPTOR *	STREAMABLE_BLEND_SHAPE_ANIM::GetGenericCommunicatorDescriptor()
{
	return	((COMMUNICATOR_PARTITION_DESCRIPTOR*) COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAMABLE_BLEND_SHAPE_ANIM));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-10-07:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_PARTITION_DESCRIPTOR *	STREAMABLE_BLEND_SHAPE_ANIM::CreateGenericCommunicatorDescriptor()
{
	return (new  COMMUNICATOR_PARTITION_DESCRIPTOR(	QDT_COMMENT("STREAMABLE_BLEND_SHAPE_ANIM"), 
													MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAMABLE_BLEND_SHAPE_ANIM, 
													QDT_COMMENT("81D2F0F2-EB2F-95A4-7AC0-0DF9BCAAE782"),
													sizeof(STREAMABLE_BLEND_SHAPE_ANIM), 
													COMInit, 
													QDT_COMMENT("Streamed blend shape animation data"),
													STREAMABLE::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		GetClassGUID
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	STREAMABLE_BLEND_SHAPE_ANIM::GetClassGUID() const
{
	return	(_sClassGUID);
}

//-----------------------------------------------------------------------------
//	Name:		SetClassGUID
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM::SetClassGUID(const QDT_STRING &	sClassGUID)
{
	_sClassGUID = sClassGUID;
}

//-----------------------------------------------------------------------------
//	Name:		CreatePartitionWriter
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_PARTITION_WRITER *	STREAMABLE_BLEND_SHAPE_ANIM::CreatePartitionWriter()
{
	COM_INSTANCE	CI;

	return ( new STREAMABLE_BLEND_SHAPE_ANIM_WRITER( GetStreamDuration(), GetPartitionDuration(), COM_INSTANCE(this), CI ) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateBlock
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK * STREAMABLE_BLEND_SHAPE_ANIM::CreateBlock(int nID, float rStartTime, float rEndTime, const char * pBuffer, int nSize)
{
	BLEND_SHAPE_ANIM_BLOCK	*pB;

	pB = new BLEND_SHAPE_ANIM_BLOCK(nID, rStartTime, rEndTime, pBuffer, nSize);

	pB->InitFromStream(pBuffer);

	return (pB);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM::EM_Construct(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	STREAMABLE_BLEND_SHAPE_ANIM	*pSBSA = static_cast<STREAMABLE_BLEND_SHAPE_ANIM *>(pO);
	QDT_NEW_PLACED(pSBSA, STREAMABLE_BLEND_SHAPE_ANIM) ();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-03-01:	VMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM::EM_Construct_In_Memory(void * const	pO,
															void * const	pR,
															void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM::EM_Destruct(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	STREAMABLE_BLEND_SHAPE_ANIM	*pSBSA = static_cast<STREAMABLE_BLEND_SHAPE_ANIM *>(pO);	
	QDT_DELETE_PLACED(pSBSA, STREAMABLE_BLEND_SHAPE_ANIM);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadHeaderChunkV1
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	STREAMABLE_BLEND_SHAPE_ANIM::CM_ReadHeaderChunkV1(IO_STREAM &		Stream,
														  COMMUNICATOR *	pCommunicator)
{
	STREAMABLE_BLEND_SHAPE_ANIM * pStreamableBlendShapeAnim;
	pStreamableBlendShapeAnim = (STREAMABLE_BLEND_SHAPE_ANIM *) pCommunicator;

	pStreamableBlendShapeAnim->ReadSpecificStreamHeader(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteHeaderChunkV1
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	STREAMABLE_BLEND_SHAPE_ANIM::CM_WriteHeaderChunkV1(IO_STREAM &		Stream,
														   COMMUNICATOR *	pCommunicator)
{
	STREAMABLE_BLEND_SHAPE_ANIM * pStreamableBlendShapeAnim;
	pStreamableBlendShapeAnim = (STREAMABLE_BLEND_SHAPE_ANIM *) pCommunicator;

	pStreamableBlendShapeAnim->WriteSpecificStreamHeader(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReadSpecificStreamHeader
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM::ReadSpecificStreamHeader(IO_STREAM &	Stream)
{
	Stream >> _nFramesCount >> _nNbrWeights >> _rFPS;
}

//-----------------------------------------------------------------------------
//	Name:		WriteSpecificStreamHeader
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	STREAMABLE_BLEND_SHAPE_ANIM::WriteSpecificStreamHeader(IO_STREAM &	Stream)
{
	Stream << _nFramesCount << _nNbrWeights << _rFPS;
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
