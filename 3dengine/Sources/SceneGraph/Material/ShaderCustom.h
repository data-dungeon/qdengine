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
//	CLASS:	SHADER_CUSTOM
//	The SHADER_CUSTOM class implements ...
//
//	05-11-09:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_MATERIAL_SHADERCUSTOM_H__
#define __M3D_SCENEGRAPH_MATERIAL_SHADERCUSTOM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KCOM(COM_INSTANCE)
DECL_3DENGINE(SHADER_CONTAINER)
DECL_3DENGINE(S_LIGHT_SOURCE)

#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)

#define MAX_TEXTURE 7
#define MAX_PASSES  8

namespace QDT
{
	namespace M3D
	{
		struct MATERIAL_PROPERTY
		{
			Float32		_f1, _f2, _f3, _f4; 
			QDT_STRING	_sName;
		};

		class SHADER_CUSTOM : public SHADER_ABC
        {
        public:
        
	        SHADER_CUSTOM();
	        ~SHADER_CUSTOM();
        
	        // Derviation from SHADER_ABC
	        virtual UInt32 GetNbrPasses();
	        virtual void SetPass(UInt32 uiPass, const VECTOR & Pos);
			virtual void Restore();
        
	        // Texture
	        void SetNbrTextures(UInt32 uiNbr);
	        void SetTexture(UInt32 i, const COM_INSTANCE & ciTexture);
	        
			// Vertex & Pixel shader
			void SetVertexShader(UInt8 * pShader, UInt32 uiSize);
	        void SetSkinnedVertexShader(UInt8 * pShader, UInt32 uiSize);
	        void SetPixelShader(UInt32 uiIndex, UInt32 uiDescription, UInt8 * pShader, UInt32 uiSize);
	        void SetNbrOfShaderContainer(UInt32 uiNbrOfContainers);

			// Configurable property (ex: diffuse, specular)
			void AddProperty(const QDT_STRING & sName, Float32 f1, Float32 f2, Float32 f3, Float32 f4);
			void ClearProperties() { _vProperties.Clear() ; }

			// For maya (directly receive pixel and vertex shader allready created)
					void				SetVertexShader(I_VERTEX_SHADER * pShader);
					void				SetPixelShader(UInt32 uiIndex, UInt32 uiDescription, I_PIXEL_SHADER * pShader);
					I_VERTEX_SHADER*	GetVertexShader();
					I_PIXEL_SHADER*		GetPixelShader(UInt32 uiIndex);
					void				AllocateContainers();
			inline	UInt32				GetContainersNb() { return _uiNbrOfShaderContainers; }
			
			// Flags
			void SetSupportLighting(Bool b);			
			void SetVSNeedEyePosition(Bool b);
	        void SetPSNeedEyeVector(Bool b);
			void SetPSNeedEyePosition(Bool b);
			void SetPSNeedObjectToViewMatrix(Bool b);
			void SetPSNeedObjectToWorldMatrix(Bool b);	
			void SetPSNeedSphericalHarmonics(Bool b);
        
	        static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
	        static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();
        
        private:
        
	        static void ComInit(CLASS_DESCRIPTOR * const pCD);
	        static void EM_Construct(void * const pO, void * const pR, void * const pP);
	        static void EM_Destruct(void * const pO, void * const pR, void * const pP);
	        static void	CM_ReadChunk0(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	        static void	CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk6(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteChunk6(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
        
	        void CreateShaders();
	        void SetModelViewProj();
			void SetFocalPlane();
			void SetEyePosition();
	        void SetSkinningMatrices();
	        void SetTextures();
			void PushConstants(UInt32 uiPass, const VECTOR & Pos);
	        void SetLights(UInt32 uiPass);
	        void SetSpotShadow(const S_LIGHT_SOURCE *);
			void PushMaterialProperties();

	        I_PIXEL_SHADER * FindShader(UInt32 uiDescription);
        
	        I_VERTEX_SHADER *	_pVertexShader;
	        I_VERTEX_SHADER *	_pSkinnedVertexShader;
	        UInt32				_uiNbrOfShaderContainers;
	        SHADER_CONTAINER *	_pShaderContainerArray;

			Bool				_bSupportLighting;
        
			// Constant requested
			Bool				_bVSNeedEyePosition;		// In object space
			Bool				_bPSNeedSphericalHarmonics; // In object space
	        Bool				_bPSNeedEyeVector;			// In object space
			Bool				_bPSNeedEyePosition;		// In object space
			Bool				_bPSNeedObjectToViewMatrix;
			Bool				_bPSNeedObjectToWorldMatrix;
			QDT_VECTOR <MATERIAL_PROPERTY> _vProperties;
			
	        UInt32				_uiNbrTextures;
	        COM_INSTANCE *		_pTextures;
        
	        UInt32				_uiCurrentPixelRegister;
	        UInt32				_uiCurrentVertexShaderRegister;
        
	        static I_PIXEL_SHADER * s_pPass[MAX_PASSES];
			static UInt32			s_uiNbrOfPasses;
        
	        // Used only for loading the shader
		        UInt32				_uiVertexShaderSize;
		        UInt8 *				_pRawVertexShader;
        
		        UInt32				_uiSkinnedVertexShaderSize;
		        UInt8 *				_pRawSkinnedVertexShader;
        
		        // This one is an array used for the SHADER_CONTAINER array
		        UInt32 *			_pPixelShaderSize;
		        UInt32 *			_pPixelShaderDescriptor;
		        UInt8 **			_ppRawPixelShader;
	        //
        };
	}
}

#endif // __M3D_SCENEGRAPH_MATERIAL_SHADERCUSTOM_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

