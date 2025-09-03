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
//
//	06-02-16:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_LIB_INFO_H__
#define __MEM_LIB_INFO_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

struct MEM_LIB_INFO
{
	UInt32 _uCurrentUsage;
	UInt32 _uPeakUsage;
	Float64 _fTimeOfPeak;
};

#endif // __MEM_LIB_INFO_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
