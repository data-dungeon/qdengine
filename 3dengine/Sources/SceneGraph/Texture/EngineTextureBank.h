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
//	CLASS:	ENGINE_TEXTURE_BANK
//	The ENGINE_TEXTURE_BANK class implements a texture that 
//
//	06-03-28:	FBO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREBANK_H__
#define __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREBANK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer/RendererTypeDefs)
#include INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)

namespace QDT
{
	namespace M3D
	{
		struct ENGINE_TEXTURE_BANK_LINK
		{
			COM_INSTANCE _rMeshDataCI;
			COM_INSTANCE _rTextureCI;
		};

		class ENGINE_TEXTURE_BANK : public ENGINE_TEXTURE_ABC
		{
		public:
			ENGINE_TEXTURE_BANK();
			~ENGINE_TEXTURE_BANK();

			void			SetLinksNb(UInt32 iLinksNb);
			void			SetLink(UInt32 iLinkIndex, COM_INSTANCE& rMeshDataCI, COM_INSTANCE& rTextureCI);            
			COM_INSTANCE&	GetLinkTextureInstance(UInt32 iLinkIndex);

			virtual I_TEXTURE* GetTexture();

			// Do not call this method
			I_TEXTURE * GetTexture(COMMUNICATOR& rMeshData) const { return (NULL); }

			static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();

		protected:
			UInt32						_iLinksNb;
			ENGINE_TEXTURE_BANK_LINK*	_pLinksArray;

			static void ComInit(CLASS_DESCRIPTOR * const pCD);
			static void EM_Construct(void * const pO, void * const pR, void * const pP);
			static void EM_Destruct(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREBANK_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

