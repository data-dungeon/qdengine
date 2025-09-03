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
//	CLASS:	MEM_BIG_CHUNK
//
//	06-06-23:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetRawChunkPtr
//	Object:		
//	06-06-23:	VHA - Created
//-----------------------------------------------------------------------------
char *	MEM_BIG_CHUNK::GetRawChunkPtr()
{
	return ((char*)this)+sizeof(MEM_BIG_CHUNK);
}

//-----------------------------------------------------------------------------
//	Name:		GetRawEndChunkPtr
//	Object:		
//	06-07-05:	VHA - Created
//-----------------------------------------------------------------------------
char *	MEM_BIG_CHUNK::GetRawEndChunkPtr()
{
	return GetRawChunkPtr()+_uFullSize;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
