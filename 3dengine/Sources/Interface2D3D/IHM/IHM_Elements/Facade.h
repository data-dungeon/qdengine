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
//	Category:	IAMAPPne\Sources\Interface2D3D\IHM\IHM_Elements\
//
//	01-11-30:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__IAMAPPNE_SOURCES_INTERFACE2D3D_IHM_IHM_ELEMENTS_FACADE_H__
	#define	__IAMAPPNE_SOURCES_INTERFACE2D3D_IHM_IHM_ELEMENTS_FACADE_H__

	#include	"..\Facade.h"
	
	#include	INC_KRNMATH(Math\Color)
	#include	INC_KRNMATH(Math\Geometry\Shapes\Shapes0D\Point2D)
	
	using namespace QDT::KCORE;

	#include	DEC_3DENGINE(Interface2D3D\DisplayFont)
	#include	DEC_3DENGINE(Display)
	#include	DEC_3DENGINE(SceneGraph\Entities\Interface3D)

	namespace QDT
	{
		namespace M3D
		{
			#include	"IHM_ElementABC.h"

			#include	"IHM_ElementPoly\IHM_ElementPolyABC.h"
			#include	"IHM_ElementPoly\IHM_ElementQuad.h"
			#include	"IHM_ElementPoly\IHM_ElementTriangle.h"

			#include	"IHM_ElementRect\IHM_ElementRect.h"
			#include	"IHM_ElementRect\IHM_ElementBitmap.h"
			#include	"IHM_ElementRect\IHM_ElementText.h"
		}
	}
#endif	//	__IAMAPPNE_SOURCES_INTERFACE2D3D_IHM_IHM_ELEMENTS_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
