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
//	CLASS:	FILTER
//	The FILTER class implements ...
//
//	04-01-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_VIEWPORT_FILTER_H__
#define __DISPLAY_VIEWPORT_FILTER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(FILTER_LAYER)
DECL_3DENGINE(FILTER_SCRATCH)
DECL_3DENGINE(VIEWPORT)

#include INCL_KCOM(CommunicatorType/CommunicatorContainer)

namespace QDT
{
	namespace M3D
	{
		class	FILTER : public COMMUNICATOR_CONTAINER
		{
		public:
			
			// Constructor & destructor
														FILTER();
													~FILTER();

			// Modifiors
			_INLINE_ void								PushLayer(const COM_INSTANCE & ciLayer);
			_INLINE_ void								PushScratch(const COM_INSTANCE & ciScratch);
			_INLINE_ void								ClearLayers();
			_INLINE_ void								ClearScratches();

			// Accessors
			_INLINE_ const COM_INSTANCE &				GetLayerInst(unsigned int i) const;
			_INLINE_ const COM_INSTANCE &				GetScratchInst(unsigned int i) const;
			_INLINE_ FILTER_LAYER *						GetLayer(unsigned int i) const;
			_INLINE_ FILTER_SCRATCH *					GetScratch(unsigned int i) const;
			_INLINE_ unsigned int						GetNbrLayers() const;
			_INLINE_ unsigned int						GetNbrScratches() const;

			// Display
			void										Display(VIEWPORT *pViewport);

			// Communication
			static void									Init(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_CONTAINER_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_CONTAINER_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			enum
			{
				CMD_PUSH_LAYER = CMD_COM_USER_START,
			};

			// Commands
			static void									EM_Construct(void * const pO, void * const pR, void * const pP);
			static void									EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void									EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void									EM_PushLayer(void * const pO, void * const pR, void * const pP);
		#ifndef _MASTER_ONLY_DATABANK
			static void									CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static void									CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

			static void*								_pVTable;

			QDT_VECTOR<COM_INSTANCE>					_vFilterLayers;
			QDT_VECTOR<COM_INSTANCE>					_vFilterScratches;
		};
	}
}

#ifndef _DEBUG
	#include	"Filter.inl"
#endif

#endif // __DISPLAY_VIEWPORT_FILTER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
