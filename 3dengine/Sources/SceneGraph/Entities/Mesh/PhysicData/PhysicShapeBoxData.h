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
//	CLASS:	PHYSIC_SHAPE_BOX_DATA
//	The PHYSIC_SHAPE_BOX_DATA class implements ...
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPEBOXDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPEBOXDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)

namespace QDT
{
	namespace M3D
	{
		class	PHYSIC_SHAPE_BOX_DATA : public PHYSIC_SHAPE_DATA
		{
		public:

			PHYSIC_SHAPE_BOX_DATA();
			~PHYSIC_SHAPE_BOX_DATA();

			// Accessors
			_INLINE_ const VECTOR &	GetDimensions() const;

			// Modifiors
			_INLINE_ void	SetDimensions(const VECTOR & Dimensions);

			// Chunk
			void	ReadChunk(IO_STREAM & Stream);
			void	WriteChunk(IO_STREAM & Stream);

		private:

			VECTOR	_Dimensions;
		};
	}
}

#ifndef _DEBUG
	#include "PhysicShapeBoxData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPEBOXDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
