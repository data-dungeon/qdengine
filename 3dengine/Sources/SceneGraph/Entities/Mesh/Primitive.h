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
//	CLASS:	PRIMITIVE
//	The PRIMITIVE class implements ...
//
//	05-10-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_PRIMITIVE_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_PRIMITIVE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(CommunicatorType/ComInstance)
#include	INCL_KRENDERER(Renderer/RendererTypeDefs)

namespace QDT
{
	namespace M3D
	{
		class	PRIMITIVE
		{
		public:
						PRIMITIVE();
						~PRIMITIVE();
							
			PRIMITIVE &	operator=(const PRIMITIVE & C);
			
			// Accessors
			_INLINE_ PRIMITIVE_TYPE	GetType() const;

			_INLINE_ Bool	GetPerPolygoneSorting() const;

			_INLINE_ UInt32	GetVertexBufferIdx() const;
			_INLINE_ UInt32	GetFirstVertex() const;
			_INLINE_ UInt32	GetNbrVertices() const;

			_INLINE_ UInt32	GetIndexBufferIdx() const;
			_INLINE_ UInt32	GetFirstIndex() const;
			_INLINE_ UInt32	GetNbrIndices() const;

			_INLINE_ UInt32	GetNbrJoints() const;
			_INLINE_ UInt32	GetJointIndex(UInt32 nIndex) const;

			_INLINE_ const COM_INSTANCE & GetShader() const;

			// Modifiors
			_INLINE_ void	SetType(PRIMITIVE_TYPE Type);

			_INLINE_ void	SetPerPolygoneSorting(Bool b);

			_INLINE_ void	SetVertexBufferIdx(UInt32 nVertexBufferIdx);
			_INLINE_ void	SetFirstVertex(UInt32 nFirstVertex);
			_INLINE_ void	SetNbrVertices(UInt32 nNbrVertices);

			_INLINE_ void	SetIndexBufferIdx(UInt32 nIndexBufferIdx);
			_INLINE_ void	SetFirstIndex(UInt32 nFirstIndex);
			_INLINE_ void	SetNbrIndices(UInt32 nNbrIndices);

			_INLINE_ void	SetNbrJoints(UInt32 nNbrJoints);
			_INLINE_ void	SetJointIndices(UInt32 * pJointIndices);

			_INLINE_ void	SetShader(const COM_INSTANCE & ciShader);

		private:

			void Copy(const PRIMITIVE & C);

			// Type
			PRIMITIVE_TYPE	_Type;

			// Flags
			Bool			_bPerPolygoneSorting; 

			// Vertex Buffer
			UInt32			_nVertexBufferIdx;
			UInt32			_nFirstVertex;
			UInt32			_nNbrVertices;

			// Index Buffer
			UInt32			_nIndexBufferIdx;
			UInt32			_nFirstIndex;
			UInt32			_nNbrIndices;

			// Skinning
			UInt32			_nNbrJoints;
			UInt32 *		_pJointIndices;

			// Material
			COM_INSTANCE	_ciShader;
		};
	}
}

#ifndef _DEBUG
	#include "Primitive.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_PRIMITIVE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
