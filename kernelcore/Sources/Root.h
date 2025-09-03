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
//	File	:	Root.h
//	Category:	Only here to avoid ../../../../ in inclusion paths
//
//	05-11-09:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef QDT_LIB_ID
#define QDT_LIB_ID MEM_LIBID_KERNELCORE
#endif

#include "../../Root.h"

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
#undef QDT_NEW
#define QDT_NEW new
#define QDT_NEW_REDEFINED ::new(QDT_LIB_ID,1)
#define new QDT_NEW_REDEFINED
#endif //QDT_MEM_USE_EXTRA_DEBUG_INFO

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
