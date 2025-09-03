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
//	CLASS:	GRAPHIC_OPTIONS
//
//	02-03-27:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KERNEL(Database\DatabaseManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

// For singleton
GRAPHIC_OPTIONS*	GRAPHIC_OPTIONS::_pInstance = NULL;

// For chunks
int					GRAPHIC_OPTIONS::_nNbrChunks = 0;
OLD_CHUNK		GRAPHIC_OPTIONS::_ChunksList[GRAPHIC_OPTIONS::NBR_CHUNKS];

//-----------------------------------------------------------------------------
//	Name:		GRAPHIC_OPTIONS constructor
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
GRAPHIC_OPTIONS::GRAPHIC_OPTIONS()
{
	_pInstance = this;

	EnableDynamicShadows(true);		
	EnableSelfShadowing(true);		
	EnableDynamicLighting(true);		
	EnableLOD(true);					
	EnableShadowsLOD(false);			
	SetTexturesDetail(GR_OP_HIGH_DETAIL);		
	SetPolygonesDetail(GR_OP_HIGH_DETAIL);		
}

//-----------------------------------------------------------------------------
//	Name:		GRAPHIC_OPTIONS destructor
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
GRAPHIC_OPTIONS::~GRAPHIC_OPTIONS()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
// Name:		GRAPHIC_OPTIONS::Instance
// Object:		For singleton : Instance 
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
GRAPHIC_OPTIONS* GRAPHIC_OPTIONS::Instance() 
{
	if (_pInstance == NULL) 
	{
		DATABASE_MANAGER::Instance()->CreateDynamic("GRAPHIC_OPTIONS", M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_GRAPHIC_OPTIONS);
	}
	
	return (_pInstance); 
}

//-----------------------------------------------------------------------------
// Name:		GRAPHIC_OPTIONS::Suicide
// Object:		For singleton : suicide
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void GRAPHIC_OPTIONS::Suicide()
{	
	if (_pInstance != NULL) 
	{
		DATABASE_MANAGER::Instance()->Delete(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_GRAPHIC_OPTIONS, Instance()->GetId());
	}

	_pInstance = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		ReceiveCommandProc
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	GRAPHIC_OPTIONS::ReceiveCommandProc(COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack)
{
	return GetDesc()->ReceiveCommandProc(this, CommandID, Stack);
}

//-----------------------------------------------------------------------------
//	Name:		GetDesc
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
DESC *	GRAPHIC_OPTIONS::GetDesc() const
{
	return (GRAPHIC_OPTIONS_DESC::Instance());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbChunk
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
int	GRAPHIC_OPTIONS::GetNbChunk() const
{
	return (_nNbrChunks);
}

//-----------------------------------------------------------------------------
//	Name:		GetChunk
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
const OLD_CHUNK &	GRAPHIC_OPTIONS::GetChunk(int	nIndex) const
{
	QDT_ASSERT(nIndex < _nNbrChunks);
	return _ChunksList[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-05-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	GRAPHIC_OPTIONS::Read(const OLD_CHUNK &	Chunk,
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
bool	GRAPHIC_OPTIONS::Write(const OLD_CHUNK &	Chunk,
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
void *	GRAPHIC_OPTIONS::GetObject()
{
	return (this);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
