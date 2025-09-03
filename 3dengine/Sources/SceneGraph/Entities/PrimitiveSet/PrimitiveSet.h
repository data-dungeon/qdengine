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
//	CLASS:	PRIMITIVE_SET
//	The PRIMITIVE_SET class implements ...
//
//	02-12-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_PRIMITIVESET_H__
#define __M3D_SCENEGRAPH_ENTITIES_PRIMITIVESET_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)
#include	INCL_KMATH(Math/Color/Color)
#include	INCL_KRENDERER(Renderer/RendererTypeDefs)

namespace QDT
{
	namespace M3D
	{
		class	PRIMITIVE_SET : public ENTITY_ABC
		{
		public:

			enum
			{
				CMD_PUSH_LINE = CMD_ENTITY_LAST,
				CMD_CLEAR,
				CMD_PSET_LAST,
			};

											PRIMITIVE_SET();
											~PRIMITIVE_SET();

			virtual void					Register(void);
			virtual void					Unregister(void);

			void							Clear(void);
			void							Copy(const PRIMITIVE_SET& PrimSet);

			void							PushLine(const VECTOR& vStart, const VECTOR& vEnd, const COLOR& Color, const bool bBuildBoxForRaycast =  false);
			void							PushTriangle(const VECTOR& vPt1, const VECTOR& vPt2, const VECTOR& vPt3, const COLOR& Color, const bool bBuildBoxForRaycast =  false);

			void							PushBox(const VECTOR& vMin, const VECTOR& vMax, const COLOR& Color, const bool bBuildBoxForRaycast =  false);
			void							PushCone(const VECTOR& vTop, const VECTOR& vAxe, const Float32 rRadius, const Float32 rLength, const COLOR& Color, const Int32 nSubdiv =10, const bool bBuildBoxForRaycast =  false);
			void							PushCylinder(const VECTOR& vCenter, const VECTOR& vCylDir, const Float32 rCylRadius, const Float32 rCylHeight, const COLOR& Color, const Int32 nSubdiv =10, const bool bBuildBoxForRaycast =  false);
			void							PushSphere(const VECTOR& vCenter, const Float32 rRadius, const COLOR& Color, const Int32 nSubdiv =10, const bool bBuildBoxForRaycast =  false);
			void							PushCircle(const VECTOR& vCenter, const VECTOR& Normal, const Float32 rRadius, const COLOR& Color, const Int32 nSubdiv =10, const bool bBuildBoxForRaycast =  false);

			//bool							RayCast(RAY_CAST_PARAMETERS& RayCastParameters, bool& bContinue);

			// ENTITY_ABC
			virtual void					Display(void);

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor(void);
			static COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor(void);

		private:

			void							PushBoxForIntersection(const VECTOR & Min, const VECTOR & Max);
			void							UpdateMinMax(const VECTOR & Pt1, const VECTOR & Pt2, const VECTOR & Pt3, VECTOR & Min, VECTOR & Max);
			void							UpdateMinPoint(const VECTOR & Pt, VECTOR & Min);
			void							UpdateMaxPoint(const VECTOR & Pt, VECTOR & Max);

		private:

			static	void					EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void					EM_Destruct(void * const pO, void * const pR, void * const pP);
			static  void					EM_PushLine(void * const pO, void * const pR, void * const pP);
			static  void					EM_Clear(void * const pO, void * const pR, void * const pP);

			QDT_VECTOR <DEBUG_VERTEX>		_vLines;
			QDT_VECTOR <DEBUG_VERTEX>		_vTriangles;

			QDT_VECTOR <TRIANGLE>			_vBoxForIntersection;

		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_PRIMITIVESET_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
