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
//	Category:	IAMAPPne\Sources\Physic\
//
//	02-02-27:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__IAMAPPNE_SOURCES_PHYSIC_FACADE_H__
	#define	__IAMAPPNE_SOURCES_PHYSIC_FACADE_H__

	#include "..\Facade.h"

	#include	INC_KRNCOM(CommunicatorType)
	#include	INC_KRNCOM(ComBasis)

	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	DEC_KRNMATH(Math/Geometry/BoundingVolume)
	#include	DEC_KRNMATH(Math/Geometry/Matrix)

	#include	DEC_KRNSERV(Physic/PhysicalEntity)

	#include	DEC_3DENGINE(SceneGraph/Entities/Mesh/SoftBody)

	namespace QDT
	{
		namespace M3D
		{
			#include "PhysicManager.h"
		}
	}

#endif	//	__IAMAPPNE_SOURCES_PHYSIC_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
