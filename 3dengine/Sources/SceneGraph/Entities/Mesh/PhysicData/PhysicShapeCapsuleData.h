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
//	CLASS:	PHYSIC_SHAPE_CAPSULE_DATA
//	The PHYSIC_SHAPE_CAPSULE_DATA class implements ...
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPECAPSULEDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPECAPSULEDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)

namespace QDT
{
	namespace M3D
	{
		class	PHYSIC_SHAPE_CAPSULE_DATA : public PHYSIC_SHAPE_DATA
		{
		public:

			PHYSIC_SHAPE_CAPSULE_DATA();
			~PHYSIC_SHAPE_CAPSULE_DATA();

			// Accessors
			_INLINE_ Float32	GetRadius() const;
			_INLINE_ Float32	GetHeight() const;

			// Modifiors
			_INLINE_ void	SetRadius(Float32 rRadius);
			_INLINE_ void	SetHeight(Float32 rHeight);

			// Chunk
			void	ReadChunk(IO_STREAM & Stream);
			void	WriteChunk(IO_STREAM & Stream);

		private:

			Float32	_rRadius;
			Float32	_rHeight;
		};
	}
}

#ifndef _DEBUG
	#include "PhysicShapeCapsuleData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPECAPSULEDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
