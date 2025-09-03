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
//	CLASS:	DISPLAY_DUMMIES
//
//	02-06-04:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KERNEL(Database/DatabaseManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

// For singleton
DISPLAY_DUMMIES*	DISPLAY_DUMMIES::_pInstance = NULL;

// For chunks
int					DISPLAY_DUMMIES::_nNbrChunks = 0;
OLD_CHUNK		DISPLAY_DUMMIES::_ChunksList[DISPLAY_DUMMIES::NBR_CHUNKS];

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_DUMMIES constructor
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_DUMMIES::DISPLAY_DUMMIES()
{
	_pInstance = this;
	
	DisplayBoundingBoxes(false);			
	DisplayBoundingSpheres(false);			
	DisplayLightBoundingVolumes(false);	
	DisplaySkeletonBoundingVolumes(false);	
	DisplayLights(false);					
	DisplayJoints(false);					
	DisplaySkeletons(false);				
	DisplayShadowVolumes(false);			
	DisplayQuadtrees(false);				
	DisplayParticleEmitters(false);		
	DisplayForceFields(false);				
	DisplayBoundingCylinders(false);		
	DisplayPhysic(false);		
	DisplayCollisionMap(false);		
}

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_DUMMIES destructor
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_DUMMIES::~DISPLAY_DUMMIES()
{
	_pInstance = this;
}

//-----------------------------------------------------------------------------
// Name:		DISPLAY_DUMMIES::Instance
// Object:		For singleton : Instance 
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_DUMMIES* DISPLAY_DUMMIES::Instance() 
{
	if (_pInstance == NULL) 
	{
		DATABASE_MANAGER::Instance()->CreateDynamic("DISPLAY_DUMMIES", M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_DISPLAY_DUMMIES);
	}
	
	return (_pInstance); 
}

//-----------------------------------------------------------------------------
// Name:		DISPLAY_DUMMIES::Suicide
// Object:		For singleton : suicide
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_DUMMIES::Suicide()
{	
	if (_pInstance != NULL) 
	{
		DATABASE_MANAGER::Instance()->Delete(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_DISPLAY_DUMMIES, Instance()->GetId());
	}

	_pInstance = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		ReceiveCommandProc
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_DUMMIES::ReceiveCommandProc(COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack)
{
	return GetDesc()->ReceiveCommandProc(this, CommandID, Stack);
}

//-----------------------------------------------------------------------------
//	Name:		GetDesc
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
DESC *	DISPLAY_DUMMIES::GetDesc() const
{
	return (DISPLAY_DUMMIES_DESC::Instance());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbChunk
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
int	DISPLAY_DUMMIES::GetNbChunk() const
{
	return (_nNbrChunks);
}

//-----------------------------------------------------------------------------
//	Name:		GetChunk
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
const OLD_CHUNK &	DISPLAY_DUMMIES::GetChunk(int	nIndex) const
{
	QDT_ASSERT(nIndex < _nNbrChunks);
	return _ChunksList[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_DUMMIES::Read(const OLD_CHUNK &	Chunk,
							  IO_STREAM &				Stream)
{
	if (Chunk == _ChunksList[0])
	{
		switch (Chunk.GetVersion())
		{
		case 1:
			{
				return (true);
			}
			break;
			
		default:
			// Wrong version
			return (false);
		}
	}

	// Unknown chunk
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_DUMMIES::Write(const OLD_CHUNK &	Chunk,
							   IO_STREAM &				Stream) const
{
	if (Chunk == _ChunksList[0])
	{
		switch (Chunk.GetVersion())
		{
		case 1:
			{	
				return (true);
			}
			break;
			
		default:
			// Wrong version
			return (false);
		}
	}

	// Unknown chunk
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetObject
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
void *	DISPLAY_DUMMIES::GetObject()
{
	return (this);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
