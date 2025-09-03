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
// CLASS: BLEND_SHAPE_PC
//
//
//	01-08-16	ELE - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapePC.h"
#include	INCL_KMATH(Math/Math)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: BLEND_SHAPE_PC constructor
// Object: Default constructor
//01-08-16 ELE Created: 
//-----------------------------------------------------------------------------
BLEND_SHAPE_PC::BLEND_SHAPE_PC()
:
COMMUNICATOR		(GetGenericCommunicatorDescriptor()),
_pContribArray		(NULL),
_rFPS				(0.f),
_nContribArraySize	(0),
_nFramesCount		(0),
_nPacketsVersion	(0),
_nFormantCount		(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
// Name: BLEND_SHAPE_PC destructor
// Object: Destructor
//01-08-16 ELE Created: 
//-----------------------------------------------------------------------------
BLEND_SHAPE_PC::~BLEND_SHAPE_PC()
{
	delete [] _pContribArray;
}

//-----------------------------------------------------------------------------
// Name: BLEND_SHAPE_PC::GetNbFrames
// Object: 
// 01-08-16 ELE Created: 
//-----------------------------------------------------------------------------
UInt32 BLEND_SHAPE_PC::GetNbFrames()
{
	return (_nFramesCount) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetBSWeights
//	Object:		
//	01-07-05:	ELE - Created
//-----------------------------------------------------------------------------
bool BLEND_SHAPE_PC::GetBSWeightsAt(Float32			rCurrentFrame,
									Float32 *			prWeights,
									UInt32	WeightCount,
									bool			bInterpolate)
{
	QDT_ASSERT(_pContribArray);

	if (WeightCount > (unsigned  int)_nFormantCount)
	{
		WeightCount = _nFormantCount;
	}
	
	if (!_pContribArray)
	{
		memcpy(prWeights, 0, sizeof(Float32) * WeightCount);
		
		return (false);
	}
	
	Float32	*pContrib;
				
	if ((rCurrentFrame < 0.0f) || (rCurrentFrame >= _nFramesCount))
	{
		memset(prWeights, 0, WeightCount * sizeof(Float32));
	}
	else
	{
		pContrib = _pContribArray + (int)rCurrentFrame * _nFormantCount;

		if	(bInterpolate)
		{
			Float32	*pContribNext = pContrib + _nFormantCount;
			Float32	rInterCoeff = rCurrentFrame - MATH::Floor(rCurrentFrame);
			Float32	rInterCoeff2 = 1.0f - rInterCoeff;

			for (UInt32 i = 0 ; i < (UInt32)_nFormantCount ; ++i)
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

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		SetContribArray
//	Object:		
//	02-03-07:	ELE - Created
//-----------------------------------------------------------------------------
void BLEND_SHAPE_PC::SetContribArray(const Float32 *Array, int size, int FormantCount)
{
	if	(_pContribArray)
	{
		delete [] _pContribArray;
	}

	_nFormantCount = FormantCount;

	_nContribArraySize = (size / FormantCount) * _nFormantCount;
	
	_pContribArray = new Float32 [_nContribArraySize];
	memset(_pContribArray, 0, _nContribArraySize * sizeof(Float32));

	int i, j;
	int FramesCount = size / FormantCount;
	for (i=0; i<FramesCount; ++i)
	{
		for (j=0; j<FormantCount; ++j)
		{
			_pContribArray[i * _nFormantCount + j] = Array[i * FormantCount + j];
		}
	}

	_nFramesCount = _nContribArraySize / _nFormantCount;
}

//-----------------------------------------------------------------------------
//	Name:		COMInit
//	Object:		
//	03-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PC::COMInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("BLSHDATA", 1 ,CM_ReadChunk1);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("BLSHDATA", 1 ,CM_WriteChunk1);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-27:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	BLEND_SHAPE_PC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE_PC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	BLEND_SHAPE_PC::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("BLEND_SHAPE_PC"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE_PC, 
											QDT_COMMENT("07E15AC5-B9B6-060F-957F-79BC4561E95D"), 
											sizeof(BLEND_SHAPE_PC), 
											COMInit, 
											QDT_COMMENT("Base data blend shape animation class")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PC::EM_Construct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	QDT_NEW_PLACED(static_cast<BLEND_SHAPE_PC *>(pO), BLEND_SHAPE_PC)();
	QDT_ASSERT(pO);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-12-19:	VMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PC::EM_Construct_In_Memory(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PC::EM_Destruct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	QDT_DELETE_PLACED(static_cast<BLEND_SHAPE_PC *>(pO), BLEND_SHAPE_PC);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-03-27:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	BLEND_SHAPE_PC::CM_ReadChunk1(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	BLEND_SHAPE_PC *pBS = static_cast<BLEND_SHAPE_PC *>(pCommunicator);

	int i;

	Stream >> pBS->_nPacketsVersion;
	
	Stream >> pBS->_nFormantCount;

	Stream >> pBS->_nContribArraySize;
	
	pBS->_nFramesCount = pBS->_nContribArraySize / pBS->_nFormantCount;
	
	delete [] pBS->_pContribArray;
	pBS->_pContribArray = new Float32 [pBS->_nContribArraySize];

	for (i = 0 ; i < pBS->_nContribArraySize ; ++i)
	{
		Stream >> pBS->_pContribArray[i];
	}
		
	Stream >> pBS->_rFPS;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	03-03-27:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	BLEND_SHAPE_PC::CM_WriteChunk1(IO_STREAM &		Stream,
									   COMMUNICATOR *	pCommunicator)
{
	BLEND_SHAPE_PC *pBS = static_cast<BLEND_SHAPE_PC *>(pCommunicator);

	int i;

	Stream << pBS->_nPacketsVersion;
	
	Stream << pBS->_nFormantCount;

	Stream << pBS->_nContribArraySize;
	
	for (i = 0 ; i < pBS->_nContribArraySize ; ++i)
	{
		Stream << pBS->_pContribArray[i];
	}

	Stream << pBS->_rFPS;
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
