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
//	CLASS:	SHADER_ABC
//	The SHADER_ABC class implements the interface for all shaders.
//  The implementation include the management of the blending which is commun
//  to all shaders. 
//	A shader must define the following methods to work correctly:
//		virtual UInt32 GetNbrPasses() = 0;
//		virtual void SetPass(UInt32 uiPass) = 0;
//
//	05-11-09:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_MATERIAL_SHADERABC_H__
#define __M3D_SCENEGRAPH_MATERIAL_SHADERABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_3DENGINE(SORTED_OBJECT)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		enum SHADER_BLENDING
		{
			SB_DECAL,
			SB_ALPHA,
			SB_INVALPHA,
			SB_ADDITIVE,
			SB_SUBTRACTIVE,
		};

		class SHADER_ABC : public COMMUNICATOR
		{
		public:

			enum
			{
				CMD_SET_BLENDING_MODE = CMD_COM_USER_START,
				CMD_LAST_SHADERABC
			};

			SHADER_ABC(COMMUNICATOR_DESCRIPTOR *pCD);
			virtual ~SHADER_ABC();

			virtual UInt32 GetNbrPasses() = 0;
			virtual void SetPass(UInt32 uiPass, const VECTOR & Pos) = 0;
			virtual void Restore();

			// Mutators
			void SetBlending(SHADER_BLENDING eBlend);
			void SetAlphaTest(Bool bAlphaTest);
			void SetAlphaTestValue(Float32 rAlphaTestValue);
			void SetDoubleSided(Bool b);
			void SetDoubleRendering(Bool b);
			void SetSupportShadowing(Bool b);
			void SetTransparencyPriority(UInt8 ucPriority);

			QDT_VECTOR<SORTED_OBJECT> &	GetObjectsToDisplay();
			Bool						IsTransparent() const;
			Bool						GetDoubleSided() const;
			Bool						GetDoubleRendering() const;
			UInt8						GetTransparencyPriority() const;

			static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();

		protected:

			void				PrepareBlending(UInt32 uiPass);
			Bool				NeedSeparateDepthPass() const; // For depth of field!

			static void			ComInit(CLASS_DESCRIPTOR * const pCD);
			static void			EM_Construct(void * const pO, void * const pR, void * const pP);
			static void			EM_Destruct(void * const pO, void * const pR, void * const pP);
			static int			CM_SetBlendingMode(COM_PARAM P);

			QDT_VECTOR<SORTED_OBJECT>	_vDisplayedObjects;
			SHADER_BLENDING				_eBlending;
			Float32						_rAlphaTestValue;
			Bool						_bDoubleSided;
			Bool						_bDoubleRendering; // Faces are displayed back then front
			Bool						_bEnableAlphaTest;
			Bool						_bSupportShadowing;
			UInt8						_uiTransparencyPriority;
		};
	}
}

#endif // __M3D_SCENEGRAPH_MATERIAL_SHADERABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
