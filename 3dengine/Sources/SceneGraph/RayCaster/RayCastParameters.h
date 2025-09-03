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
// CLASS: RAY_CAST_PARAMETERS
//
//
//	01-02-27:	RMA - Created
//*****************************************************************************

//=============================================================================
// Name: RAY_CAST_PARAMETERS
//	01-02-27:	RMA - Created
//=============================================================================

#ifndef __M3D_SCENEGRAPH_RAYCASTER_RAYCASTPARAMETERS_H__
#define __M3D_SCENEGRAPH_RAYCASTER_RAYCASTPARAMETERS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)

namespace QDT
{
	namespace M3D
	{
		class RAY_CAST_PARAMETERS
		{

		public :

			enum
			{
				TEST_TRIGGER = 1 << 0,	// Not used yet
				CULLING,
			} ;

			/** Constructors & destructor */
											RAY_CAST_PARAMETERS();
    										RAY_CAST_PARAMETERS(const RAY & Ray);
											RAY_CAST_PARAMETERS(const RAY_CAST_PARAMETERS & RayCastParameters);
										~RAY_CAST_PARAMETERS();

			/** Operator = */
			RAY_CAST_PARAMETERS &			operator = (const RAY_CAST_PARAMETERS & RayCasterParameters);

			/** Reset all parameters */
			_INLINE_ void					Reset();
			/** Test if a triangle has been hit with distance comparaison */
			bool							TestTriangleHit(const TRIANGLE &TriangleHit, float	rDistance);

			/** Flags */
			_INLINE_ int					GetFlags(int nFlagsMask) const;
			_INLINE_ void					SetFlags(int nFlagsMask, int nFlagsSet);

			/** Accessors & Modifiors */
			_INLINE_ const RAY &			GetRay() const;
			_INLINE_ void					SetRay(const RAY & Ray);
			_INLINE_ const MATRIX &			GetGlobalMatrix() const;
			_INLINE_ void					SetGlobalMatrix(const MATRIX & Matrix);
			_INLINE_ const VECTOR			GetImpactPoint() const;
			_INLINE_ const int				GetImpactNodeID() const;
			_INLINE_ void					SetImpactNodeID(int ID);
			_INLINE_ float					GetImpactDistance() const;
			_INLINE_ void					SetImpactDistance(float rValue);
			_INLINE_ const VECTOR &			GetImpactNormal() const;
			_INLINE_ void					SetImpactNormal(const VECTOR & Normal);
			_INLINE_ const TRIANGLE &		GetImpactTriangle() const;
			_INLINE_ void					SetImpactTriangle(const TRIANGLE & Triangle);
			_INLINE_ int					GetNumTriangleHitted() const;
			_INLINE_ void					SetNumTriangleHitted(int nNum);
			_INLINE_ int					GetNumGeomHeaderHitted() const;
			_INLINE_ void					SetNumGeomHeaderHitted(int nNum);
			_INLINE_ void					SetPortalInNodeID(const int ID);
			_INLINE_ const int				GetPortalInNodeID();
			_INLINE_ void					SetCurrentNodeID(const int ID);
			_INLINE_ const int				GetCurrentNodeID();
			_INLINE_ bool					CatchPrimitiveSet() const;
			_INLINE_ void					SetCatchPrimitiveSet(bool b);

		private:

			/** Copy method to avoid code duplication in operator = & copy constructor */
			_INLINE_ void					Copy(const RAY_CAST_PARAMETERS & RayCastParameters) ;

			int								_nFlags;
			RAY								_Ray;
			int								_ImpactNodeID;			// Node which contains the triangle hit by the ray	
			MATRIX							_GlobalMatrix;			// The global matrix of the node we are testing
			float							_rImpactDistance ;		// distance of the impact
			VECTOR							_ImpactNormal;			// normal of the impact
			TRIANGLE						_ImpactTriangle ;		// The triangle of the impact
			int								_nNumGeomHeaderHitted;	// Numero of the strip hitted	
			int								_nNumTriangleHitted;	// Numero of the triangle hitted
			int								_PortalInNodeID;		// Portal to avoid because of unlimited recursion
			int								_CurrentNodeID;			// Index of the current node
			bool							_bCatchPrimitiveSet;
		};
	}
}

#ifndef _DEBUG
	#include "RayCastParameters.inl"
#endif

#endif // __M3D_SCENEGRAPH_RAYCASTER_RAYCASTPARAMETERS_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
