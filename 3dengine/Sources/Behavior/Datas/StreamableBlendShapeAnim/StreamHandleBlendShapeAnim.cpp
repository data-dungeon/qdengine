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
//	CLASS:	STREAM_HANDLE_BLEND_SHAPE_ANIM
//
//	03-06-05:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamHandleBlendShapeAnim.h"
#include	INCL_KCORE(Streaming/StreamBlock)
#include	INCL_KMATH(Math/Math)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnim)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/BlendShapeAnimBlock)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_HANDLE_BLEND_SHAPE_ANIM constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_BLEND_SHAPE_ANIM::STREAM_HANDLE_BLEND_SHAPE_ANIM()
:
_StreamableBlendShapeAnimInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_HANDLE_BLEND_SHAPE_ANIM destructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_BLEND_SHAPE_ANIM::~STREAM_HANDLE_BLEND_SHAPE_ANIM()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendShapeAnimInst
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_BLEND_SHAPE_ANIM::SetBlendShapeAnimInst(const COM_INSTANCE &	BlendShapeAnimInst)
{
	_StreamableBlendShapeAnimInst = BlendShapeAnimInst;
}

//-----------------------------------------------------------------------------
//	Name:		Play
//	Object:		
//	03-06-11:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_BLEND_SHAPE_ANIM::Play()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Stop
//	Object:		
//	03-06-11:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_BLEND_SHAPE_ANIM::Stop()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Pause
//	Object:		
//	03-06-11:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_BLEND_SHAPE_ANIM::Pause()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	03-06-11:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_BLEND_SHAPE_ANIM::Resume()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		IsPlaying
//	Object:		
//	03-06-11:	ELE - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE_BLEND_SHAPE_ANIM::IsPlaying()
{
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		GetBSWeightsAt
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE_BLEND_SHAPE_ANIM::GetBSWeightsAt(Float32		rCurrentFrame,
													   Float32 *		prWeights,
													   UInt32	WeightCount,
													   bool			bInterpolate)
{
	if	(GetIsManaged())
	{
		return	(GetManagedBSWeights(prWeights, WeightCount, bInterpolate));
	}
	else
	{
		return	(GetBSWeights(rCurrentFrame, prWeights, WeightCount, bInterpolate));
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetBSWeights
//	Object:		
//	03-06-19:	AMA - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE_BLEND_SHAPE_ANIM::GetBSWeights(Float32			rCurrentFrame,
													 Float32 *		prWeights,
													 UInt32	WeightCount,
													 bool			bInterpolate)
{
	int							nID;
	Float32						rTime;
	bool						bValidBlock, bLoaded;
	STREAMABLE_BLEND_SHAPE_ANIM	*pStreamable;
	STREAM_BLOCK				*pB;
	
	pStreamable = static_cast<STREAMABLE_BLEND_SHAPE_ANIM *>(_StreamableBlendShapeAnimInst.GetCommunicator());

	rTime = rCurrentFrame / pStreamable->GetFPS();
	
	bValidBlock = false;

	pB = GetCurrentBlock();
	if (pB)
	{
		if (rTime >= pB->GetStartTime() && rTime <= pB->GetEndTime())
		{
			bValidBlock = true;
		}
	}

	if	(bValidBlock == false)
	{
		nID = pStreamable->GetBlockIndexFromTime(rTime);
		pB	= pStreamable->QueryBlock(nID, true, bLoaded);
		SetCurrentBlock(pB);
	}

	UInt32 nNbrWeights = (unsigned  int)pStreamable->GetNbrWeights();

	if (WeightCount > nNbrWeights)
	{
		WeightCount = nNbrWeights;
	}
			
	if ((rCurrentFrame < 0.0f) || (rCurrentFrame >= pStreamable->GetNbrFrames()))
	{
		memset(prWeights, 0, WeightCount * sizeof(Float32));
	}
	else
	{
		BLEND_SHAPE_ANIM_BLOCK	*pBSB = static_cast<BLEND_SHAPE_ANIM_BLOCK *>(pB);

		int		nStartFrame = (int)(pB->GetStartTime() * pStreamable->GetFPS());
		Float32	*pContrib;

		pContrib = pBSB->GetWeights();
		pContrib += ((int)rCurrentFrame - nStartFrame) * nNbrWeights;

		if	(bInterpolate)
		{
			Float32	*pContribNext = pContrib + nNbrWeights;
			Float32	rInterCoeff = rCurrentFrame - MATH::Floor(rCurrentFrame);
			Float32	rInterCoeff2 = 1.0f - rInterCoeff;

			for (UInt32 i = 0 ; i < WeightCount ; ++i)
			{
				prWeights[i] = (rInterCoeff2 * pContrib[i]) + (rInterCoeff * pContribNext[i]);
			}
		}
		else
		{
			for (UInt32 i = 0 ; i < WeightCount ; ++i)
			{
				prWeights[i] = pContrib[i];
			}
		}
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		GetManagedBSWeights
//	Object:		
//	03-06-19:	AMA - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE_BLEND_SHAPE_ANIM::GetManagedBSWeights(Float32 *			prWeights,
															UInt32	WeightCount,
															bool			bInterpolate)
{
	int							nID;
	Float32						rCurrentTime = GetTheCurrentTime();

#ifdef	PLATFORM_PS2
	// Fix pour la synchro labiale sur PS2
	// (ATTENTION, fonctionne pour fahrenheit car la taille des blocs est supérieur à 200ms
	// et le bloc n+1 est toujours chargé en mémoire (n+2 en cours) )
	rCurrentTime += 0.2f;
#elif PLATFORM_PC
	rCurrentTime += 0.175f;
#endif

	STREAMABLE_BLEND_SHAPE_ANIM	*pStreamable;
	STREAM_BLOCK				*pB;
	
	pStreamable = static_cast<STREAMABLE_BLEND_SHAPE_ANIM *>(_StreamableBlendShapeAnimInst.GetCommunicator());

	nID = pStreamable->GetBlockIndexFromTime(rCurrentTime);
	pB	= pStreamable->GetBlock(nID);

	if ( pB == NULL )
	{
		return (false);
	}
	
	UInt32 nNbrWeights = (unsigned  int)pStreamable->GetNbrWeights();

	if (WeightCount > nNbrWeights)
	{
		WeightCount = nNbrWeights;
	}
	
	Float32	rStreamDuration = pStreamable->GetStreamDuration();
	if (rCurrentTime >= rStreamDuration)
	{
		rCurrentTime = rStreamDuration;
	}

	BLEND_SHAPE_ANIM_BLOCK	*pBSB = static_cast<BLEND_SHAPE_ANIM_BLOCK *>(pB);

	int		nStartFrame = (int)(pB->GetStartTime() * pStreamable->GetFPS());
	Float32	*pContrib;
	Float32	rCurrentFrame = rCurrentTime * pStreamable->GetFPS();

	pContrib = pBSB->GetWeights();
	pContrib += ((int)rCurrentFrame - nStartFrame) * nNbrWeights;

	if (rCurrentFrame >= pStreamable->GetNbrFrames())
	{
		bInterpolate = false;
	}

	if	(bInterpolate)
	{			
		Float32	*pContribNext = pContrib + nNbrWeights;
		Float32	rInterCoeff = rCurrentFrame - MATH::Floor(rCurrentFrame);
		Float32	rInterCoeff2 = 1.0f - rInterCoeff;

		for (UInt32 i = 0 ; i < WeightCount ; ++i)
		{
			prWeights[i] = (rInterCoeff2 * pContrib[i]) + (rInterCoeff * pContribNext[i]);
		}
	}
	else
	{
		for (UInt32 i = 0 ; i < WeightCount ; ++i)
		{
			prWeights[i] = pContrib[i];
		}
	}

	return	(true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

