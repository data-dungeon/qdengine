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
//	Category:	Viewer2e\Sources\Interface2D3D\
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__VIEWER2E_SOURCES_INTERFACE2D3D_FACADE_H__
	#define	__VIEWER2E_SOURCES_INTERFACE2D3D_FACADE_H__

	#include	"..\Facade.h"

	#include	INC_KRNCOM(ComBasis)
	#include	INC_KRNCOM(CommunicatorType)
	#include	INC_KRNCOM(BaseType)
	#include	INC_KRNCOM(OldCom/Communication)
	using namespace QDT::KCOM;


	namespace QDT
	{
		namespace M3D
		{
			#include	"InterfaceManager.h"
		}
	}

#endif	//	__VIEWER2E_SOURCES_INTERFACE2D3D_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
