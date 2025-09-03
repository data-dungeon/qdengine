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
//	CLASS:	PHYSIC_SHAPE_TRIANGLE_DATA
//	The PHYSIC_SHAPE_TRIANGLE_DATA class implements ...
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPETRIANGLEDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPETRIANGLEDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)

namespace QDT
{
	namespace M3D
	{
		class	PHYSIC_SHAPE_TRIANGLE_DATA : public PHYSIC_SHAPE_DATA
		{
		public:

			enum	PHYSIC_SHAPE_TRIANGLE_TYPE
			{
				PSTT_CONVEX,
				PSTT_NONCONVEX,
			};

			PHYSIC_SHAPE_TRIANGLE_DATA();
			~PHYSIC_SHAPE_TRIANGLE_DATA();

			// Accessors
			_INLINE_ PHYSIC_SHAPE_TRIANGLE_TYPE	GetType() const;
			_INLINE_ UInt32						GetGeometricDataSize() const;
			_INLINE_ const void *				GetGeometricData() const;

			// Modifiors
			_INLINE_ void	SetType(PHYSIC_SHAPE_TRIANGLE_TYPE Type);
			_INLINE_ void	SetGeometricDataSize(UInt32 nSize);
			_INLINE_ void	SetGeometricData(void * pGeometricData);

			// Chunk
			void	ReadChunk(IO_STREAM & Stream);
			void	WriteChunk(IO_STREAM & Stream);

		private:

			PHYSIC_SHAPE_TRIANGLE_TYPE	_Type;

			UInt32	_GeometricDataSize;
			void *	_pGeometricData;
		};
	}
}

#ifndef _DEBUG
	#include "PhysicShapeTriangleData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPETRIANGLEDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
