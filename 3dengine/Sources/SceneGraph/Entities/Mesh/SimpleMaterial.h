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
//	CLASS:	SIMPLE_MATERIAL
//	The SIMPLE_MATERIAL class implements ...
//
//	05-10-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_SIMPLEMATERIAL_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_SIMPLEMATERIAL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_3DENGINE(SIMPLE_TEXTURE)

namespace QDT
{
	namespace M3D
	{
		class	SIMPLE_MATERIAL
		{
		public:
			SIMPLE_MATERIAL();
			~SIMPLE_MATERIAL();

			void	InitDisplay() const;

			const I_VERTEX_SHADER *	GetVertexShader() const;
			const I_PIXEL_SHADER *	GetPixelShader() const;
			const SIMPLE_TEXTURE *	GetTexture() const;
				void				SetVertexShader(I_VERTEX_SHADER * pVertexShader);
				void				SetPixelShader(I_PIXEL_SHADER * pPixelShader);
				void				SetTexture(SIMPLE_TEXTURE * pTexture);

		private:

			I_VERTEX_SHADER *	_pVertexShader;
			I_PIXEL_SHADER *	_pPixelShader;
			SIMPLE_TEXTURE *	_pTexture;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_SIMPLEMATERIAL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
