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
// CLASS: JOINT
//
//
//00-10-27 JVA Created: 
//*****************************************************************************

#ifndef __SCENEGRAPH_ENTITIES_JOINT_H__
#define __SCENEGRAPH_ENTITIES_JOINT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)

namespace QDT
{
	namespace M3D
	{
		enum	JOINT_TYPE
		{
			JT_ROOT = 0,
			JT_HEAD,
			JT_LEFT_EYE,
			JT_RIGHT_EYE,
			JT_LEFT_PALM,
			JT_RIGHT_PALM,
			JT_LEFT_ARM,
			JT_LEFT_FORARM,
			JT_LEFT_WRIST,
			JT_LEFT_HAND,
			JT_RIGHT_ARM,
			JT_RIGHT_FORARM,
			JT_RIGHT_WRIST,
			JT_RIGHT_HAND,
			JT_LEFT_THIGH,
			JT_RIGHT_THIGH,
			JT_LEFT_SHIN,
			JT_RIGHT_SHIN,
			JT_LEFT_LEG,
			JT_RIGHT_LEG,
			JT_LEFT_KNEE,
			JT_RIGHT_KNEE,
			JT_STERNUM,
			JT_HAIR,
			NBR_JOINT_TYPES,
			JT_NONE,

			JT_MAX = 0xFFFFFFFF
		};

		class JOINT : public ENTITY_ABC
		{
		public:

			JOINT();
			~JOINT();

			enum CONSTANT
			{
				MAX_JOINT_PER_POINT = 6,			// nombre maximum de joint influent sur un point (vertex ou CV)
				MAX_GEOMS_PER_SKINCLUSTER = 1,		// nombre maximum de geometrie par SkinCluster
			} ;

			enum
			{
				NONE = -1,
				COLLISION_OBB = 0,
				COLLISION_CYLINDER,
			};

			/** Operator = */
			JOINT &					operator = (const JOINT & Joint);

			//Accessors
			_INLINE_ const OBB				GetCollisionOBB() const;
			_INLINE_ const CYLINDER			GetCollisionCylinder() const;
			_INLINE_ const BOUNDING_VOLUME*	GetCollisionVolume() const;
			_INLINE_ int					GetCollisionVolumeType() const;
			_INLINE_ int					GetIndex() const;
			_INLINE_ JOINT_TYPE				GetType() const;
			_INLINE_ bool					GetIsFoot() const;
			_INLINE_ float					GetDistanceFromFloor() const;

			_INLINE_ QDT_VECTOR <BOUNDING_VOLUME*> *	GetSoftBodyCollisionVolumes();


			//Modifiors
			_INLINE_ void					SetIndex( int i);
			_INLINE_ void					SetType(JOINT_TYPE Type);
			_INLINE_ void					SetIsFoot(bool bValue);
			_INLINE_ void					SetDistanceFromFloor(float rValue);
			_INLINE_ void					SetCollisionVolume(const OBB& pCV);
			_INLINE_ void					SetCollisionVolume(const CYLINDER& pCV);
			_INLINE_ void					SetCollisionVolumeType(int nType);
			_INLINE_ void					AddSoftBodyCollisionVolume(BOUNDING_VOLUME* bv);


			/** Derivation from ENTITY_ABC : Display */
			virtual void			Display(DISPLAY_PARAMETERS & DisplayParameters);
			virtual bool			TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:
			/** Copy method to avoid code duplication in operator = & in copy constructor */
			_INLINE_ void Copy(const JOINT & Joint) ;

			OBB								_CollisionOBB;
			CYLINDER						_CollisionCylinder;

			/** Index */
			int				_nIndex ;
			JOINT_TYPE		_Type;

			// Collision Volumes
			int								_nCollisionVType;
			//Collision with soft-body
			QDT_VECTOR <BOUNDING_VOLUME*>	_vCollisionVolumes;

			// Collision with floor
			float							_rDistanceFromFloor;
			bool							_bJointIsFoot;
	
			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunk
		#ifndef _MASTER_ONLY_DATABANK
			static void		CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER		
			static void		CM_WriteChunk5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		};
	}
}

#ifndef _DEBUG
	#include "Joint.inl"
#endif

#endif // __SCENEGRAPH_ENTITIES_JOINT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
