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
//	File	:	Facade.h
//	Category:	IAMAPPne\Sources\Interface2D3D\SpriteBank\
//
//	03-07-23:	FDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__IAMAPPNE_SOURCES_INTERFACE2D3D_SPRITEBANK_FACADE_H__
	#define	__IAMAPPNE_SOURCES_INTERFACE2D3D_SPRITEBANK_FACADE_H__

	#include	"..\Facade.h"

	#include	INC_KRNCOM(CommunicatorType)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"SpriteBank.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__IAMAPPNE_SOURCES_INTERFACE2D3D_SPRITEBANK_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
