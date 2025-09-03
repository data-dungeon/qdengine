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
//	CLASS:	RAY_CASTER
//	The RAY_CASTER class implements ...
//
//	01-07-26:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_RAYCASTER_RAYCASTER_H__
#define __M3D_SCENEGRAPH_RAYCASTER_RAYCASTER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(RAY_CAST_PARAMETERS)

#include INCL_KCOM(ComBasis/ClassDescriptor)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class	RAY_CASTER
		{
		public:
			enum
			{
				CMD_RC_CAST_RAY_FROM_SCREEN = CMD_USER_START,
				CMD_RC_HAS_HIT_SOMETHING,
				CMD_RC_GET_IMPACT_POINT,
				CMD_RC_GET_IMPACT_NODE,
				CMD_RC_GET_IMPACT_DISTANCE,
				CMD_RC_GET_IMPACT_NORMAL,
				CMD_RC_CAST_RAY_FROM_VIEWPORT,
				CMD_RC_LAST_COMMAND
			};

			RAY_CASTER();
			virtual ~RAY_CASTER();

			/** Cast ray */
			void								CastRayFromScreen(int nScreenX, int nScreenY, bool bCulling);
			void								CastRayFromViewport(int nScreenX, int nScreenY, const COM_INSTANCE &Viewport, bool bCulling);

			/** Accessors */
			_INLINE_ bool						HasHitSomething() const;
			_INLINE_ const VECTOR				GetImpactPoint() const;
			_INLINE_ COM_INSTANCE 				GetImpactNodeInst() const;
			_INLINE_ float						GetImpactDistance() const;
			_INLINE_ const VECTOR &				GetImpactNormal() const;

			static CLASS_DESCRIPTOR *			GetClassDescriptor();
			static CLASS_DESCRIPTOR *			CreateClassDescriptor();

		private:

			RAY_CAST_PARAMETERS *				_pRayCastParameters;

			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);

			static void	EM_CastRayFromScreen(void * const pO, void * const pR, void * const pP);
			static void	EM_HasHitSomething(void * const pO, void * const pR, void * const pP);
			static void	EM_GetImpactPoint(void * const pO, void * const pR, void * const pP);
			static void	EM_GetImpactNode(void * const pO, void * const pR, void * const pP);
			static void	EM_GetImpactDistance(void * const pO, void * const pR, void * const pP);
			static void	EM_GetImpactNormal(void * const pO, void * const pR, void * const pP);
			static void	EM_CastRayFromViewport(void * const pO, void * const pR, void * const pP);

			static void					Init(CLASS_DESCRIPTOR * const pCD);
		};
	}
}

#include INCL_3DENGINE(M3D/Moteur3DModule)
#include INCL_3DENGINE(SceneGraph/RayCaster/RayCastParameters)
#include INCL_KCOM(CommunicatorType/ComInstance)

#ifndef _DEBUG
	#include "RayCaster.inl"
#endif

#endif // __M3D_SCENEGRAPH_RAYCASTER_RAYCASTER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
