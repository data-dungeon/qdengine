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
//	CLASS:	MESH_DATA
//	The MESH_DATA class implements ...
//
//	05-11-03:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_MESHDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_MESHDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(PRIMITIVE)
DECL_KMATH(OBB)
DECL_KMATH(SPHERE)
DECL_KRENDERER(S_VERTEX_ELEMENT)
DECL_KRENDERER(I_VERTEX_DECLARATION)
DECL_KRENDERER(I_VERTEX_BUFFER);
DECL_KRENDERER(I_INDEX_BUFFER);
DECL_3DENGINE(FACES_TO_SORT)

#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/FlowData)
#include	INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	MESH_DATA : public COMMUNICATOR
		{
		public:

			// Constructors & Destructor
			MESH_DATA();
			~MESH_DATA();

			// Operators
			MESH_DATA &	operator=(const MESH_DATA & C);

			void	Init();
			void	Clear();

			// Accessors
			_INLINE_ UInt32				GetNbrVertexBuffers() const;
			_INLINE_ UInt32				GetNbrVertices(UInt32 nIndex) const;
			_INLINE_ UInt32				GetVertexSize(UInt32 nIndex) const;
			_INLINE_ void *				GetVertices(UInt32 nIndex) const;
			_INLINE_ UInt32				GetNbrVertexElements(UInt32 nIndex) const;
			_INLINE_ S_VERTEX_ELEMENT *	GetVertexElements(UInt32 nIndex) const;
			_INLINE_ UInt32				GetNbrIndexBuffers() const;
			_INLINE_ UInt32				GetNbrIndices(UInt32 nIndex) const;
			_INLINE_ UInt16 *			GetIndices(UInt32 nIndex) const;
			_INLINE_ UInt32				GetNbrPrimitives() const;
					 const PRIMITIVE &	GetPrimitive(UInt32 nIndex) const;
					 PRIMITIVE &		GetPrimitive(UInt32 nIndex);
					 const OBB &		GetLocalBoundingBox(UInt32 nIndex) const;
					 const SPHERE &		GetLocalBoundingSphere(UInt32 nIndex) const;
			_INLINE_ QDT_FLAGS			GetDisplayFlags(UInt32 nIndex) const;
			_INLINE_ UInt32				GetNbrFacesToSort() const; 
			const FACES_TO_SORT &		GetFacesToSort(UInt32 nIndex) const;
			 FACES_TO_SORT &			GetFacesToSort(UInt32 nIndex);
			_INLINE_ UInt32				GetNbrFlowData() const;
			_INLINE_ FLOW_DATA*			GetFlowData(UInt32 iFlowDataIndex) const;

			_INLINE_ I_VERTEX_DECLARATION *	GetVertexDecl(UInt32 nIndex);
			_INLINE_ I_VERTEX_BUFFER *		GetVertexBuffer(UInt32 nIndex);
			_INLINE_ I_INDEX_BUFFER *		GetIndexBuffer(UInt32 nIndex);
			
			// Modifiors
			_INLINE_ void	SetNbrVertexBuffers(UInt32 uiNbrVertexBuffers);
			_INLINE_ void	SetVertices(UInt32 nIndex, UInt32 uiNbrVertices, UInt32 uiVertexSize, void * pVertices);
			_INLINE_ void	SetVertexElements(UInt32 nIndex, UInt32 uiNbrVertexElements, S_VERTEX_ELEMENT * pVertexElements);
			_INLINE_ void	SetNbrIndexBuffers(UInt32 uiNbrIndexBuffers);
			_INLINE_ void	SetIndices(UInt32 nIndex, UInt32 uiNbrIndices, UInt16 * pIndices);
					 void	SetNbrPrimitives(UInt32 uiNbrPrimitives);
					 void	SetPrimitive(UInt32 nIndex, const PRIMITIVE & Primitive);
					 void	SetBoundingBox(UInt32 nIndex, const OBB & BoundingBox);
					 void	SetBoundingSphere(UInt32 nIndex, const SPHERE & BoundingSphere);
					 void   SetNbrFacesToSort(UInt32 uiNbrFacesToSort);
					 void	SetDisplayFlags(UInt32 nIndex, const QDT_FLAGS & Flags);
					 void	SetCastShadows(UInt32 nIndex, Bool b);
					 void	SetReceiveShadows(UInt32 nIndex, Bool b);
					 void	SetVisible(UInt32 nIndex, Bool b);
					 void	SetNbrFlowData(UInt32 iFlowDataNb);
					 void	SetFlowData(UInt32 iFlowDataIndex, FLOW_DATA* pFlowData);


			UInt32	ComputeVertexSize(UInt32 nIndex) const;

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			void	Copy(const MESH_DATA & C);

			// For vertex buffers
			UInt32					_uiNbrVertexBuffers;
			UInt32 *				_puiNbrVertices;
			UInt32 *				_puiVertexSizes;
			void **					_pVertices;

			// For vertex declaration
			UInt32 *				_puiNbrVertexElement;
			S_VERTEX_ELEMENT **		_pVertexElement;

			// For index buffers
			UInt32					_uiNbrIndexBuffers;
			UInt32					_uiNbrFacesToSort;
			UInt32 *				_puiNbrIndices;
			UInt16 **				_pIndices;

			// Primitives
			UInt32					_uiNbrPrimitives;
			PRIMITIVE *				_pPrimitives;
			OBB	*					_pLocalBoundingBoxes;
			SPHERE	*				_pLocalBoundingSpheres;
			QDT_FLAGS *				_pfDisplayFlags;

			// Dynamic datas
			// Vertex buffers
			I_VERTEX_DECLARATION **	_pVertexDecl;
			I_VERTEX_BUFFER **		_pVertexBuffer;

			// Index buffers
			I_INDEX_BUFFER **		_pIndexBuffer;

			// Faces to sort (used in per polygone sorting)
			FACES_TO_SORT *			_pFacesToSort;

			// FlowData
			UInt32					_iFlowDataNb;
			FLOW_DATA**				_pFlowDataArray;

			Bool					_bInit;

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunks
			static void		CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk6(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk6(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		};
	}
}

#ifndef _DEBUG
	#include "MeshData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_MESHDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

