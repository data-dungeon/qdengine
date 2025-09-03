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
//	CLASS:	ENGINE_TEXTURE_FILE
//	The ENGINE_TEXTURE_FILE class implements ...
//
//	05-11-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREFILE_H__
#define __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREFILE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer/RendererTypeDefs)
#include INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)

namespace QDT
{
	namespace M3D
	{
		class ENGINE_TEXTURE_FILE : public ENGINE_TEXTURE_ABC
		{
		public:

			ENGINE_TEXTURE_FILE();
			~ENGINE_TEXTURE_FILE();

			// Modifiors
			void SetFormat(PIXEL_FORMAT eFormat);
			void SetWidth(UInt32 uiWidth);
			void SetHeight(UInt32 uiHeight);
			void SetDepth(UInt32 uidepth);
			void SetCubeMap(Bool b);
			void SetNbrMipmaps(UInt32 uiNbr);
			void SetRawData(UInt32 uiCubeFace, UInt32 uiLevel, UInt8 * pRawData, UInt32 uiSize); 
			void SetFilter(FILTER_TYPE eFilter);
			void SetAddressU(ADDRESS_TYPE eAddress);
			void SetAddressV(ADDRESS_TYPE eAddress);
			void SetTexture(I_TEXTURE * pTex);

			static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();

			void CreateTexture();

			
			virtual I_TEXTURE* GetTexture() {return _pTexture;}

		private:

			static void ComInit(CLASS_DESCRIPTOR * const pCD);
			static void EM_Construct(void * const pO, void * const pR, void * const pP);
			static void EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void	CM_ReadChunk0(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			PIXEL_FORMAT	_eFormat;
			UInt32			_uiWidth;
			UInt32			_uiHeight;
			UInt32			_uiDepth;
			UInt32			_uiNbrMipmaps;
			UInt32 *		_ppuiSize[6];
			UInt8 **		_ppRawData[6];

			FILTER_TYPE		_eFilter;
			ADDRESS_TYPE	_eAddressU;
			ADDRESS_TYPE	_eAddressV;
			Bool			_bCubeMap;

			I_TEXTURE *	_pTexture;
		};
	}
}

#endif // __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREFILE_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

