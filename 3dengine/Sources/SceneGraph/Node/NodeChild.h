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
//	CLASS:	NODE_CHILD
//	The NODE_CHILD class implements ...
//
//	06-01-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __SCENEGRAPH_NODE_CHILD_NODE_H__
#define __SCENEGRAPH_NODE_CHILD_NODE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(QUATERNION_VECTOR)
DECL_KPHYSICS(I_PHYSIC_DYNAMIC_ACTOR)
DECL_KPHYSICS(I_PHYSIC_MATERIAL)
DECL_3DENGINE(NODE_ARRAY)
DECL_3DENGINE(HAIR_CELL)

#include	INCL_3DENGINE(SceneGraph/Entities/Joint/Joint)

namespace QDT
{
	namespace M3D
	{
		class	NODE_CHILD
		{
		public:
			NODE_CHILD();
			~NODE_CHILD();

			// Operators
			NODE_CHILD &	operator=(const NODE_CHILD & N);

			// Accessors
			_INLINE_ JOINT_TYPE					GetType() const;
			_INLINE_ Int32						GetIndex() const;
			_INLINE_ Int32						GetFatherIndex() const;
			_INLINE_ UInt32						GetChildIndex(UInt32 nIndex) const;
			_INLINE_ MATRIX &					GetLocalMatrix();
			_INLINE_ const MATRIX &				GetLocalMatrix() const;
			_INLINE_ MATRIX &					GetAnimMatrix();
			_INLINE_ const MATRIX &				GetAnimMatrix() const;
			_INLINE_ const MATRIX &				GetGlobalMatrix() const;
			_INLINE_ NODE_ARRAY *				GetNodeArray() const;
			_INLINE_ I_PHYSIC_DYNAMIC_ACTOR *	GetDynamicActor();
			_INLINE_ I_PHYSIC_MATERIAL *		GetPhysicMaterial();
			_INLINE_ HAIR_CELL *				GetHairCell();

			// Modifiors
			_INLINE_ void	SetType(JOINT_TYPE nType);
			_INLINE_ void	SetLocalMatrix(const MATRIX & LocalMatrix);
			_INLINE_ void	SetIndex(Int32 iIndex);
			_INLINE_ void	SetFatherIndex(Int32 iIndex);
			_INLINE_ void	SetNbrChildren(UInt32 nNbrChildren);
			_INLINE_ void	SetChildren(UInt32 * pChildren);
			_INLINE_ void	SetNodeArray(NODE_ARRAY * pNodeArray);
			_INLINE_ void	SetDynamicActor(I_PHYSIC_DYNAMIC_ACTOR * pActor);
			_INLINE_ void	SetHairCell(HAIR_CELL * pHairCell);

			// Animation
			void	UpdateAnimMatrix(QUATERNION_VECTOR * pQV);
			void	UpdateGlobalMatrix(const MATRIX & FatherMatrix);

			// Physic
			I_PHYSIC_DYNAMIC_ACTOR *	CreatePhysicDynamicActor();
			void						DeletePhysicDynamicActor();
			I_PHYSIC_MATERIAL *			CreatePhysicMaterial();
			void						DeletePhysicMaterial();
			void						EnableKinematic(Bool b);
			void						EnablePhysic(Bool b);
			void						MoveActor();
			void						RetrieveActorPos(const MATRIX & FatherMatrix);

			// Communication
			void	Read(IO_STREAM & Stream);
			void	Write(IO_STREAM & Stream);

		private:
			
			void	Copy(const NODE_CHILD & N);

			// Type
			JOINT_TYPE	_nType;

			// Matrices
			MATRIX	_LMatrix;
			MATRIX	_AnimMatrix;
			MATRIX	_GMatrix;
			
			// Hierarchie
			Int32		_iIndex;
			Int32		_iFatherIndex;
			UInt32		_uiNbrChildren;
			UInt32 *	_pChildren;
			
			// Container
			NODE_ARRAY *	_pNodeArray;

			// Physic
			I_PHYSIC_DYNAMIC_ACTOR *	_pDynamicActor;
			I_PHYSIC_MATERIAL *			_pPhysicMaterial;
			bool						_bKinematic;

			// Hair
			HAIR_CELL *	_pHairCell;
		};	
	}
}

#ifndef _DEBUG
#include "NodeChild.inl"
#endif

#endif // __SCENEGRAPH_NODE_CHILD_NODE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
