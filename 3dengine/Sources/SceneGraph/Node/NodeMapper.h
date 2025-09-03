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
//	CLASS:	NODE_MAPPER
//	The NODE_MAPPER class implements ...
//
//	04-05-13:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __SCENEGRAPH_NODE_NODEMAPPER_H__
#define __SCENEGRAPH_NODE_NODEMAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	NODE_MAPPER : public COMMUNICATOR
		{
		public:

			// Constructor & destructor
			NODE_MAPPER();
			NODE_MAPPER(COM_HANDLE * pHandle);
			~NODE_MAPPER();

			COM_INSTANCE & GetNode(unsigned int i) const;
			COM_INSTANCE & GetEntity(unsigned int i) const;

		#ifndef _MASTER_ONLY_DATABANK
			void			AllocateMemory(unsigned int i);
		#endif

		#ifndef _MASTER
			void			SetNode(unsigned int i, const COM_INSTANCE & Node);
			void			SetEntity(unsigned int i, const COM_INSTANCE & Node);
		#endif

			void			ApplyMap();

			static void*	_pVTable;
			
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();
			static void							Init(CLASS_DESCRIPTOR * const pCD);

		#ifndef _MASTER_ONLY_DATABANK
			static void				CM_ReadChunkMatInfos(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

		#ifndef _MASTER	
			static void				CM_WriteChunkMatInfos(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

		private:

			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			unsigned int	_nNbrNodes;
			COM_INSTANCE *	_pNodes;
			COM_INSTANCE *	_pEntities;
		};
	}
}

#endif // __SCENEGRAPH_NODE_NODEMAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
