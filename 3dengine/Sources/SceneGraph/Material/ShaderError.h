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
//	CLASS:	SHADER_ERROR
//	The SHADER_ERROR class implements ...
//
//	05-11-09:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_MATERIAL_SHADERERROR_H__
#define __M3D_SCENEGRAPH_MATERIAL_SHADERERROR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)

#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)

namespace QDT
{
	namespace M3D
	{
		class SHADER_ERROR : public SHADER_ABC
		{
		public:

			SHADER_ERROR();
			~SHADER_ERROR();

			virtual UInt32 GetNbrPasses();
			virtual void SetPass(UInt32 uiPass, const VECTOR & Pos);

			static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();

			I_VERTEX_SHADER *	GetVertexShader() const;
			I_VERTEX_SHADER *	GetSkinnedVertexShader() const;
			I_PIXEL_SHADER *	GetPixelShader() const;

		private:

			static void ComInit(CLASS_DESCRIPTOR * const pCD);
			static void EM_Construct(void * const pO, void * const pR, void * const pP);
			static void EM_Destruct(void * const pO, void * const pR, void * const pP);

			void SetModelViewProj();
			void SetFocalPlane();
			void SetSkinningMatrices();

			UInt32				_uiCurrentVertexShaderRegister;

			I_VERTEX_SHADER *	_pSkinnedVertexShader;
			I_VERTEX_SHADER *	_pVertexShader;
			I_PIXEL_SHADER *	_pPixelShader;
		};
	}
}

#endif // __M3D_SCENEGRAPH_MATERIAL_SHADERERROR_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

