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
//	CLASS:	ENGINE_TEXTURE_ABC
//	The ENGINE_TEXTURE_ABC class implements ...
//
//	05-11-09:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREABC_H__
#define __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_TEXTURE)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class ENGINE_TEXTURE_ABC : public COMMUNICATOR
		{
		public:

			ENGINE_TEXTURE_ABC(COMMUNICATOR_DESCRIPTOR *pCD);
			~ENGINE_TEXTURE_ABC();

			virtual I_TEXTURE* GetTexture() = 0;

			static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();

		protected:

			static void ComInit(CLASS_DESCRIPTOR * const pCD);
			static void EM_Construct(void * const pO, void * const pR, void * const pP);
			static void EM_Destruct(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREABC_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

