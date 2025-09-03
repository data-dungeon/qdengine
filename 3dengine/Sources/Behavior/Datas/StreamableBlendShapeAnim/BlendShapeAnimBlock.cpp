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
//	CLASS:	BLEND_SHAPE_ANIM_BLOCK
//
//	03-06-05:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapeAnimBlock.h"

#include	INCL_KCORE(Stream/MemStream)
#include	INCL_KMATH(Math/Math)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_ANIM_BLOCK constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIM_BLOCK::BLEND_SHAPE_ANIM_BLOCK(int			nID,
											   float		rStartTime,
											   float		rEndTime,
											   const char *	pBuffer,
											   int			nSize)
:
STREAM_BLOCK		(nID, rStartTime, rEndTime, pBuffer, nSize),
_rBlendingTime		(0.0f),
_pLastFrameWeights	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_ANIM_BLOCK destructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIM_BLOCK::~BLEND_SHAPE_ANIM_BLOCK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
MEM_STREAM *	BLEND_SHAPE_ANIM_BLOCK::GetBuffer()
{
	QDT_NOT_IMPLEMENTED();
	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferSize
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE_ANIM_BLOCK::GetBufferSize()
{
	QDT_NOT_IMPLEMENTED();
	return	(0);
}

//-----------------------------------------------------------------------------
//	Name:		FillStream
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIM_BLOCK::FillStream(MEM_STREAM *	pMemStream,
										   float		rStartTime,
										   float		rEndTime,
										   float		rDuration,
										   bool			bFillWhithBlank,
										   bool			bFillAtBegin,
										   bool			bCompressed)
{
	float	rSFrame, rEFrame;
	float	rBlockSFrame;
	int		nPos;
	int		nNbrFrames;
	int		i, n, nLastFrameStart;
	float	*pF;

	rBlockSFrame	= GetStartTime() * _rFPS;
	if ( bFillWhithBlank )
	{
		rSFrame			= rStartTime * _rFPS;
		rEFrame			= (rEndTime * _rFPS) - 1;
	}
	else
	{
		rSFrame			= MATH::Floor(rStartTime) * _rFPS;
		rEFrame			= (MATH::Ceil(rEndTime) * _rFPS);
	}

	nPos		= (int) ((rSFrame - rBlockSFrame) * _nNbrWeights);
	nNbrFrames	= (static_cast<int>(rEFrame - rSFrame)) + 1;

	pF	= GetWeights();
	pF	+= nPos;
	n	= nNbrFrames * _nNbrWeights;

	for (i = 0 ; i < n ; ++i)
	{
		(*pMemStream) << (pF[i]);
	}

	nLastFrameStart = n - _nNbrWeights;
	_pLastFrameWeights = new float[_nNbrWeights];
	for	(i = 0 ; i < _nNbrWeights ; ++i)
	{
		_pLastFrameWeights[i] = pF[nLastFrameStart + i];
	}

	if ( bFillWhithBlank )
	{
		if ( bFillAtBegin )
		{
			QDT_NOT_IMPLEMENTED();
		}
		else
		{
			float	rBlendingTimeRef;
			float	rBlankTime;

			rBlankTime = MATH::Ceil(rDuration - (rEndTime - rStartTime));
			nNbrFrames = (int) (rBlankTime * _rFPS);
			n = nNbrFrames * _nNbrWeights;

			rBlendingTimeRef = _rBlendingTime + 1.0f;
			while ((_rBlendingTime > 0.0f) && (n > 0))
			{
				int		nEachWeight;
				float	ri;

				ri = (rBlendingTimeRef - _rBlendingTime) / rBlendingTimeRef;

				for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
				{
					_pLastFrameWeights[nEachWeight] = (1.0f - ri) * _pLastFrameWeights[nEachWeight];
					(*pMemStream) << _pLastFrameWeights[nEachWeight];

					--n;
				}

				--_rBlendingTime;
			}

			if	(n > 0)
			{
				memset(_pLastFrameWeights, 0, _nNbrWeights * sizeof(float));

				for (i = 0 ; i < n ; ++i)
				{
					(*pMemStream) << 0;
				}
			}
		}
	}
	delete [] _pLastFrameWeights;
}

//-----------------------------------------------------------------------------
//	Name:		InitFromStream
//	Object:		
//	03-06-10:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIM_BLOCK::InitFromStream(const char *	pBuffer)
{
	_nNbrWeights = *( (int *)pBuffer );
	_rFPS = *( (float *)(pBuffer + sizeof(int)) );

	_pWeights = (float *)(pBuffer + 2 * sizeof(int));
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendingTime
//	Object:		
//	04-03-05:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIM_BLOCK::SetBlendingTime(float	r)
{
	_rBlendingTime = r;
}

//-----------------------------------------------------------------------------
//	Name:		GetLastBlendingTime
//	Object:		
//	04-03-05:	ELE - Created
//-----------------------------------------------------------------------------
float	BLEND_SHAPE_ANIM_BLOCK::GetLastBlendingTime() const
{
	return	(_rBlendingTime);
}

//-----------------------------------------------------------------------------
//	Name:		GetLastFrameWeights
//	Object:		
//	04-03-04:	ELE - Created
//-----------------------------------------------------------------------------
float *	BLEND_SHAPE_ANIM_BLOCK::GetLastFrameWeights()
{
	return	(_pLastFrameWeights);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
