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
//	06-01-12:	SBE - Created
//*****************************************************************************

#define INCL_KCORE_MAINJOB(OUT, IN) <KernelCore/Sources/Job/Template_Out##OUT##In##IN.h>

#if defined(PLATFORM_PS3) && defined(SPU)
	#define CREATE_OUT_STATIC_ARRAY(TYPE) TYPE g_pOut[2][COUNT_BY_LOOP] __attribute__ ((aligned(128)));
	#define CREATE_IN_STATIC_ARRAY(NB_PARAM, TYPE) TYPE g_pIn##NB_PARAM[2][COUNT_BY_LOOP] __attribute__ ((aligned(128)));
#else
	#define CREATE_OUT_STATIC_ARRAY(TYPE)
	#define CREATE_IN_STATIC_ARRAY(NB_PARAM, TYPE)
#endif

