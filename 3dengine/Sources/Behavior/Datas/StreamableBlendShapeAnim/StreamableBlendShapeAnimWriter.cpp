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
//	CLASS:	STREAMABLE_BLEND_SHAPE_ANIM_WRITER
//
//	03-06-05:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamableBlendShapeAnimWriter.h"
#include	INCL_KCORE(Stream/MemStream)
#include	INCL_KMATH(Math/Math)
#include	INCL_3DENGINE(Behavior/Datas/BlendShapeAnim/BlendShapePC)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnim)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM_WRITER constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM_WRITER::STREAMABLE_BLEND_SHAPE_ANIM_WRITER(float rDuration,
																	   float rTimeDurationPerPartition,
																	   const COM_INSTANCE& Streamable,
																	   const COM_INSTANCE& Offset)
:
STREAMABLE_PARTITION_WRITER(rDuration, rTimeDurationPerPartition, Streamable, Offset ),
_rBlendingTime		(0.0f),
_prBlendingWeights	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM_WRITER destructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM_WRITER::~STREAMABLE_BLEND_SHAPE_ANIM_WRITER()
{
	delete [] _prBlendingWeights;
}

//-----------------------------------------------------------------------------
//	Name:		WriteSpecificData
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::WriteSpecificData(MEM_STREAM *	pS,
															  int			nNumPartition,
															  float			rDuration )
{
#ifndef _MASTER
	BLEND_SHAPE_PC	*pBS;
	int				i, j, n, nWeights;
	float			rS, rE, rVal;
	float			rFS, rFE;
	float			rFPS;
	float			Weights[32];
	float			rCurrentFrame;

	pBS			= static_cast<BLEND_SHAPE_PC *>(_BlendShapePc.GetCommunicator());
	rFPS		= pBS->GetFPS();
	nWeights	= pBS->GetNbrWeights();

	QDT_ASSERT(nWeights <= 32);			//	Array size = 32, see declaration

	/////////////////////////////////////////
	rVal	= GetPartitionDuration();
	rS		= rVal * nNumPartition;
	rE		= rS + rDuration;
	rFS		= rS * rFPS;
	if	(nNumPartition < _nNbrPartition - 1)
	{
		rFE		= rE * rFPS;
	}
	else
	{
		rFE		= rE * rFPS - 1.0f;
	}
	n		= static_cast<int>(MATH::Ceil(rFE - rFS + 1.0f));
	rCurrentFrame = rFS;
	/////////////////////////////////////////

	for (i = 0 ; i < n ; ++i)
	{
		pBS->GetBSWeightsAt(rCurrentFrame + i, Weights, nWeights, false);
		for (j = 0; j < nWeights; ++j)
		{
			(*pS) << Weights[j];
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		WriteSpecificHeaderInfo
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::WriteSpecificHeaderInfo(MEM_STREAM *	pS,
																	int				nNumPartition)
{
#ifndef _MASTER
	(*pS)	<< _nNbrWeights;
	(*pS)	<< _rFPS;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		WriterSpecificInit
//	Object:		
//	03-06-10:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::WriterSpecificInit()
{
#ifndef _MASTER
	if (_BlendShapePc.HasHandle())
	{
		InitFromBlendShapePc();
	}
	else
	{
		InitFromStreamable();
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		InitFromStreamable
//	Object:		
//	03-06-10:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::InitFromStreamable()
{
	STREAMABLE_BLEND_SHAPE_ANIM	*pSBA;

	pSBA = static_cast<STREAMABLE_BLEND_SHAPE_ANIM *>(GetAttachedStreamable());

	_nNbrWeights	= pSBA->GetNbrWeights();
	_rFPS			= pSBA->GetFPS();
}

//-----------------------------------------------------------------------------
//	Name:		InitFromBlendShapePc
//	Object:		
//	03-06-10:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::InitFromBlendShapePc()
{
	BLEND_SHAPE_PC *pBS;

	pBS				= static_cast<BLEND_SHAPE_PC *>(_BlendShapePc.GetCommunicator());

	_nNbrWeights	= pBS->GetNbrWeights();
	_rFPS			= pBS->GetFPS();
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendShapeAnim
//	Object:		
//	03-06-10:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::SetBlendShapeAnim(const COM_INSTANCE &	Anim)
{
	_BlendShapePc = Anim;
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeAnim
//	Object:		
//	03-06-10:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::GetBlendShapeAnim() const
{
	return	(_BlendShapePc);
}

//-----------------------------------------------------------------------------
//	Name:		FillStreamWithBlank
//	Object:		
//	03-06-18:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::FillStreamWithBlank(MEM_STREAM *	pMemStream,
																float rDuration)
{
	float	rDurationInFrame;
	int		nSize, nIndex;
	rDurationInFrame = rDuration * _rFPS;

	nSize = static_cast<int>(MATH::Ceil(rDurationInFrame)) * _nNbrWeights;

	if	(_prBlendingWeights)
	{
		int		i;
		bool	bBlend = false;

		for	(i = 0 ; i < _nNbrWeights ; ++i)
		{
			if	(_prBlendingWeights[i] > 0.0f)
			{
				bBlend = true;
				break;
			}
		}

		if	(bBlend)
		{
			float	rBlendingTimeRef;

			rBlendingTimeRef = _rBlendingTime + 1.0f;
			while ((_rBlendingTime > 0.0f) && (nSize > 0))
			{
				int		nEachWeight;
				float	ri;

				ri = (rBlendingTimeRef - _rBlendingTime) / rBlendingTimeRef;

				for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
				{
					_prBlendingWeights[nEachWeight] = (1.0f - ri) * _prBlendingWeights[nEachWeight];
					(*pMemStream) << _prBlendingWeights[nEachWeight];

					--nSize;
				}

				--_rBlendingTime;
			}
		}
	}

	if	(nSize > 0)
	{
		memset(_prBlendingWeights, 0, _nNbrWeights * sizeof(float));

		for (nIndex = 0 ; nIndex < nSize ; ++nIndex)
		{
			(*pMemStream) << 0;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendingTime
//	Object:		
//	04-03-05:	ELE - Created
//-----------------------------------------------------------------------------
float	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::GetBlendingTime() const
{
	return	(_rBlendingTime);
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendingTime
//	Object:		
//	04-03-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::SetBlendingTime(float	r)
{
	_rBlendingTime = r;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendingWeights
//	Object:		
//	04-03-04:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRITER::SetBlendingWeights(float *	prWeights)
{
	if	(_prBlendingWeights == NULL)
	{
		_prBlendingWeights = new float[_nNbrWeights];
	}

	memcpy(_prBlendingWeights, prWeights, _nNbrWeights * sizeof(float));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
