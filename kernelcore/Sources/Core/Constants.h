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
//	CLASS:	CONSTANTS
//	The CONSTANTS class implements ...
//
//	05-09-21:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#ifndef __KCORE_CORE_CONSTANTS_H__
#define __KCORE_CORE_CONSTANTS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{

		#ifndef	NULL
			#define	NULL 0
		#endif

		#define		QDT_INT8_MAX		127
		#define		QDT_INT8_MIN		-128
		#define		QDT_UINT8_MAX		255
		#define		QDT_INT16_MAX		32767
		#define		QDT_INT16_MIN		-32768
		#define		QDT_UINT16_MAX		65535
		#define		QDT_UINT32_MAX		4294967295
		#define		QDT_INT32_MAX		2147483647
		#define		QDT_INT32_MIN		-2147483648

		#define		QDT_FLOAT32_MAX		3.402823466e+38F		// Maximum positive value
		#define		QDT_FLOAT32_MIN		1.175494351e-38F		// Minimum positive value
		#define		QDT_FLOAT64_MAX		1.7976931348623158e+308	// Maximum positive value
		#define		QDT_FLOAT64_MIN		2.2250738585072014e-308	// Minimum positive value
	}
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
