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
//	CLASS:	NODE_ARRAY
//	The NODE_ARRAY class implements ...
//
//	06-01-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __SCENEGRAPH_NODE_ARRAY_NODE_H__
#define __SCENEGRAPH_NODE_ARRAY_NODE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(QUATERNION_VECTOR)
DECL_3DENGINE(NODE_CHILD)

#include	INCL_3DENGINE(SceneGraph/Node/Node)

namespace QDT
{
	namespace M3D
	{
		class	NODE_ARRAY : public NODE
		{
		public:
									NODE_ARRAY();
			virtual					~NODE_ARRAY();

			// NODE Interface
			virtual	int				GetPotentialNbrTracks() const;
			virtual const MATRIX &	GetIndexedGlobalMatrix(int nNum) const;
			virtual void			UpdateContainer(float rSpeedFactor);
			virtual void			UpdateGlobalMatrix();
			virtual void			UpdateLocalMatrix();
			virtual void			ForceUpdate(float rSpeedFactor = 0.0f);
			virtual void			EnableKinematic(Bool b);

			// Accessors
			_INLINE_ UInt32			GetNbrChildren() const;
			_INLINE_ NODE_CHILD *	GetChild(UInt32 uiIndex);

			// Modifiers
			_INLINE_ void	SetNbrChildren(UInt32 uiNbrChildren);
			_INLINE_ void	SetChild(NODE_CHILD * pChild);

			// Hierarchy
			void					Instanciate(NODE_ARRAY * pNewNodeArray);
			void					InstanciateRec(NODE_ARRAY * pNewNodeArray);

			// Animation
			const MATRIX &			GetIndexedContainerMatrix(int nNum) const;

			void					GetChildLocalPos(int nNumChild, VECTOR & OutPos) const;
			void					SetChildLocalPos(int nNumChild, const VECTOR & Pos);
			void					GetChildGlobalPos(int nNumChild, VECTOR & OutPos)const;
			void					GetChildAnimPos(int nNumChild, VECTOR & OutPos)const;
			void					SetChildAnimPos(int nNumChild, const VECTOR & Pos);
			void					GetChildLocalRot(int nNumChild, QUATERNION & OutQuat)const;
			void					SetChildLocalRot(int nNumChild, const QUATERNION & Quat);
			void					GetChildGlobalRot(int nNumChild, QUATERNION & OutQuat)const;
			void					GetChildAnimRot(int nNumChild, QUATERNION & OutQuat)const;
			void					SetChildAnimRot(int nNumChild, const QUATERNION & Quat);
			
			void					ResetChildren();

			// Physic
			void					MoveActors();
			void					RetrieveActorsPos();
			void					EnablePhysic(UInt32 nJointIndex, bool b);

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

			// Updates.
			void					UpdateAllMatrices( const MATRIX & RootMatrix );

		protected:

			void					UpdateAnimMatrices( );
			void					UpdateAnimMatrices( QUATERNION_VECTOR * pQV );
			
		private:
			
			// Communication
			static	void					EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void					EM_Destruct(void * const pO, void * const pR, void * const pP);

			static	void					CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			#ifndef _MASTER
			static	void					CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			#endif

			// Attributes
			UInt32			_uiNbrChildren;
			NODE_CHILD *	_pChild;
		};
	}
}

#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)

#ifndef _DEBUG
#include "NodeArray.inl"
#endif

#endif // __SCENEGRAPH_NODE_ARRAY_NODE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
