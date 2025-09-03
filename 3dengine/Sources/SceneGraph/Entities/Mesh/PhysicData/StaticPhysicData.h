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
//	CLASS:	STATIC_PHYSIC_DATA
//	The STATIC_PHYSIC_DATA class implements ...
//
//	06-02-16:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_STATICPHYSICDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_STATICPHYSICDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	STATIC_PHYSIC_DATA : public COMMUNICATOR
		{
		public:

			STATIC_PHYSIC_DATA();
			~STATIC_PHYSIC_DATA();

			// Accessors
			_INLINE_ UInt32						GetGeometricDataSize();
			_INLINE_ const void *				GetGeometricData() const;

			// Modifiors
			_INLINE_ void	SetGeometricDataSize(UInt32 nSize);
			_INLINE_ void	SetGeometricData(void * pGeometricData);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunks
			static void		CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			
			// For static collisions
			UInt32	_GeometricDataSize;
			void *	_pGeometricData;
		};
	}
}

#ifndef _DEBUG
	#include "StaticPhysicData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_PHYSICDATA_STATICPHYSICDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
