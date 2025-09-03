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
//	Category:	3DEngine\Sources\SceneGraph\Entities\Particles\
//
//	01-11-19:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_PARTICLES_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_PARTICLES_FACADE_H__

	#include	"..\Facade.h"
	
	#include	DEC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Circle)
	#include	DEC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Disc)
	#include	DEC_KRNMATH(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	DEC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Triangle)

	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/OBB)

	#include	INC_3DENGINE(SceneGraph/Entities/ForceFields)
	#include	INC_3DENGINE(SceneGraph/Material)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"ParticleShape.h"
			#include	"ParticleShapeSprite.h"
			#include	"ParticleSystem.h"
			#include	"ParticleEmitter.h"
		}
	}

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_PARTICLES_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
