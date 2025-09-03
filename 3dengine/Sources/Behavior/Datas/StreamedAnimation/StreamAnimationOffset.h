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
//	CLASS:	STREAM_ANIMATION_OFFSET
//	The STREAM_ANIMATION_OFFSET class implements ...
//
//	03-04-02:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_ANIMATION_OFFSET_H__
#define __M3D_STREAM_ANIMATION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(Streaming/OffsetPartition)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_ANIMATION_OFFSET : public OFFSET_PARTITION
		{
		public:

			enum
			{
				//CMD_ON_EVENT = CMD_COM_USER_START,
				//CMD_SET_EVENT_LEVEL,
				//CMD_LAST,
			};

			STREAM_ANIMATION_OFFSET();
			~STREAM_ANIMATION_OFFSET();

			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);

			static void*					_pVTable;
		};
	}
}

#endif // __M3D_STREAM_ANIMATION_OFFSET_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
