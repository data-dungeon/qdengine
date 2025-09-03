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
//	Category:	IAMAPPne\Sources\Interface2D3D\IHM\
//
//	01-10-01:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__IAMAPPNE_SOURCES_INTERFACE2D3D_IHM_FACADE_H__
	#define	__IAMAPPNE_SOURCES_INTERFACE2D3D_IHM_FACADE_H__

	#include	"..\Facade.h"

	#include	DEC_KRNSERV(GenericDisplay)	
	#include	DEC_KRNSERV(Sound)
	#include	INC_KRNCORE(Tools)		// For QDT_STRING and QDT_FLAGS

	namespace QDT
	{
		namespace M3D
		{
			#include	"IHM_Interface.h"
			#include	"IHM_Page.h"
			#include	"IHM_ElementsList.h"
		}
	}


#endif	//	__IAMAPPNE_SOURCES_INTERFACE2D3D_IHM_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
