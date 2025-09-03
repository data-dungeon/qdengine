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
// CLASS: Frustrum
//
//	This class implements an Axis Aligned Bounding Box.
//
//00-11-09 GGO Created: 
//*****************************************************************************

//=============================================================================
// Name: Frustrum
//00-11-09 GGO Created: 
//=============================================================================
		
#ifndef __KMATH_MATH_SHAPES_SHAPES3D_FRUSTRUM_FRUSTRUM_H__
#define __KMATH_MATH_SHAPES_SHAPES3D_FRUSTRUM_FRUSTRUM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(MATRIX)
DECL_KMATH(AABB)
DECL_KMATH(OBB)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)

namespace QDT
{
	namespace KMATH
	{
		class	FRUSTRUM
		{
		public :

			enum CLIPPING_PLANES
			{
				CP_RIGHT = 0,
				CP_LEFT,
				CP_BOTTOM,
				CP_TOP,
				CP_FAR,
				CP_NEAR,

				PLANES_COUNT,
			};

		public:

			/** Default constructor. */
			_INLINE_					FRUSTRUM();
			/** Copy constructor. */
			_INLINE_					FRUSTRUM(const FRUSTRUM & Frustrum);
			/** Build a frustrum from a camera, assuming default position and orientation. */
			_INLINE_					FRUSTRUM(Float32 rFocalAngle, Float32 rNearDistance, Float32 rFarDistance, Float32 rAspectRatio, Float32 fScale = 1.0f);
			/** Build a frustrum from the maya camera parameters. */
										FRUSTRUM(Float32 fHorizontalFieldOfView, Float32 fAspectRatio, Float32 fViewportAspectRatio, Float32 fNearClippingDistance, Float32 fFarClippingDistance, Float32 fScale);
			/** Build a frustrum from a projection matrix. */
										FRUSTRUM(const MATRIX & mProj);
			/** Destructor. */
			_INLINE_				   ~FRUSTRUM();

			/** Assignation operator. */
			const FRUSTRUM &			operator = (const FRUSTRUM & Frustrum);

			/** Initialisation. */
			void						Set(Float32 rFocalAngle, Float32 rNearDistance, Float32 rFarDistance, Float32 rAspectRatio, Float32 fScale);

			void						CreateFromProjMatrix(const MATRIX & Matrix, Float32 fNear, Float32 fFar);

			/** Accessor. */
			_INLINE_ const VECTOR &		GetOrigin() const;
			_INLINE_ const VECTOR &		GetFrontDirection() const;
			_INLINE_ const VECTOR &		GetLeftDirection() const;
			_INLINE_ const VECTOR &		GetTopDirection() const;
			_INLINE_ Float32				GetNear() const;
			_INLINE_ Float32				GetFar() const;
			_INLINE_ Float32				GetLeft() const;
			_INLINE_ Float32				GetTop() const;
			_INLINE_ const VECTOR &		GetNormalTop() const;
			_INLINE_ const VECTOR &		GetNormalBottom() const;
			_INLINE_ const VECTOR &		GetNormalLeft() const;
			_INLINE_ const VECTOR &		GetNormalRight() const;
			_INLINE_ const HEXAHEDRON &	GetHexahedron() const;

			/** Modifier */
			_INLINE_ void				SetFar(Float32 rFar);
			_INLINE_ void				SetNear(Float32 rNear);
			_INLINE_ void				SetLeft(Float32 rLeft);
			_INLINE_ void				SetTop(Float32 rTop);
			_INLINE_ void				SetLeftDirection(const VECTOR & vLeftDir);
			_INLINE_ void				SetTopDirection(const VECTOR & vTopDir);
			_INLINE_ void				SetFrontDirection(const VECTOR & vFrontDir);

			/** Intersections */
			bool						Intersect(const FRUSTRUM & Frustum) const;

			/** Transform */
					void				Transform(const MATRIX &Matrix, bool bInvertLeftRight = false);

			/** Given an OBB, try to reduce the size of the frustrum to best fit the OBB if it is completely inside the frustrum. */
					void				Reduce(const OBB &obb);
			/** Given an AABB, try to reduce the size of the frustrum to best fit the AABB if it is completely inside the frustrum. */
					void				Reduce(const AABB &aabb);

			/** Display */
					void				Display(COLOR_FLOAT &Color) const;

			/** Stream */
			void						Read(IO_STREAM & Stream);
			void						Write(IO_STREAM & Stream) const;
			friend IO_STREAM &			operator << (IO_STREAM & Stream, const  FRUSTRUM & Frustrum)
			{
				Frustrum.Write(Stream);
				return Stream;
			}
			friend IO_STREAM &			operator >> (IO_STREAM & Stream, FRUSTRUM & Frustrum)
			{
				Frustrum.Read(Stream);
				return Stream;
			}

			/** */
			_INLINE_ PLANE GetClippingPlane(const CLIPPING_PLANES index) const;
			/** */
			_INLINE_ int GetClippingPlanesCount() const;

			/** */
			void ExtractClippingPlanes();
			void ExtractFarClippingPlane();
			void ExtractNearClippingPlane();

			void GetPoints(VECTOR (& Points)[8]) const;
			void GetPoints2(VECTOR (& Points)[8]) const;

		private :
			/** Compute the normal gieven the geometry of the frustrum. */
			void ComputeNormals();
			/** Given a set of 8 points, try to reduce the size of the frustrum to best fit the points. */
			void Reduce(VECTOR (& Points)[8]);
			
		private:

			VECTOR		_Origin;

			VECTOR		_LeftDirection;
			VECTOR		_TopDirection;
			VECTOR		_FrontDirection;

			VECTOR		_NormalTop;
			VECTOR		_NormalBottom;
			VECTOR		_NormalLeft;
			VECTOR		_NormalRight;

			Float32		_rNear;
			Float32		_rFar;
			Float32		_rLeft;
			Float32		_rTop;

			PLANE		_ClippingPlanes[PLANES_COUNT];
			HEXAHEDRON	_Hexahedron;
		};
	}
}

#ifndef _DEBUG
#include	"Frustrum.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES3D_FRUSTRUM_FRUSTRUM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
