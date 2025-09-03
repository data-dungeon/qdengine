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
//	CLASS:	PHYSIC_SHAPE_DATA
//	The PHYSIC_SHAPE_DATA class implements ...
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPEDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPEDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

namespace QDT
{
	namespace M3D
	{
		class	PHYSIC_SHAPE_DATA
		{
		public:

			enum	PHYSIC_SHAPE_DATA_TYPE
			{
				PSDT_BOX,
				PSDT_SPHERE,
				PSDT_CAPSULE,
				PSDT_TRIANGLE,
			};

					PHYSIC_SHAPE_DATA(PHYSIC_SHAPE_DATA_TYPE Type);
			virtual ~PHYSIC_SHAPE_DATA();

			// Accessors
			_INLINE_ PHYSIC_SHAPE_DATA_TYPE	GetType() const;
			_INLINE_ const MATRIX &			GetLocalMatrix() const;

			// Modifiors
			_INLINE_ void	SetLocalMatrix(const MATRIX & LocalMatrix);

			// Chunk
			virtual void	ReadChunk(IO_STREAM & Stream);
			virtual void	WriteChunk(IO_STREAM & Stream);
					
		protected:

			PHYSIC_SHAPE_DATA_TYPE	_Type;

			MATRIX	_LocalMatrix;
		};
	}
}

#ifndef _DEBUG
	#include "PhysicShapeData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_PHYSICSHAPEDATAABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
