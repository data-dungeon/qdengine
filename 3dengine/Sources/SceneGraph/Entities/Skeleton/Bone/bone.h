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
// CLASS: BONE
//
//
//00-11-09 GHP Created: 
//*****************************************************************************

//=============================================================================
// Name: BONE
//00-11-09 GHP Created: 
//=============================================================================

#ifndef __SCENEGRAPH_ENTITIES_SKELETON_BONE_H__
#define __SCENEGRAPH_ENTITIES_SKELETON_BONE_H__

DECL_3DENGINE(NODE)

#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KMATH(Math/Color/ColorFloat)
#include	INCL_KMATH(Math/Color/Color)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class BONE
		{
			enum 
			{
				BONE_NB_VERT = 5 ,
			}  ; 

		public:
								BONE() {}
								BONE(NODE *pFather, NODE *pSon) ;
								BONE(const BONE& Bone) ;  ; 
								~BONE();
			
			void				Display(const MATRIX & GlobalMatrix);

		private:
			NODE	*_pFather ; 
			NODE	*_pSon	  ;
			
			float			_rS_BASE ;  
			MATRIX			_fatherMat ;
			MATRIX			_sonMat ; 
			MATRIX			_rotMatrix ; 

			VECTOR			_refDir ;
			VECTOR			_normTip ; 
			COLOR			_col ;
			COLOR_FLOAT		_colFloat ;
			
			VECTOR			_org ;
			VECTOR			_tipPoint ;
			VECTOR			_p1 ;
			VECTOR			_p2 ;
			VECTOR			_p3 ;

			SEGMENT			_seg ; 
			
			SEGMENT			_s01 ; 
			SEGMENT			_s02 ; 
			SEGMENT			_s03 ; 
			
			SEGMENT			_s41 ; 
			SEGMENT			_s42 ; 
			SEGMENT			_s43 ; 
			
			SEGMENT			_s12  ; 
			SEGMENT			_s23 ; 
			SEGMENT			_s31 ; 

			VECTOR			_pPointsTab	[BONE_NB_VERT] ;
			VECTOR			_pNormalsTab[BONE_NB_VERT] ;
			COLOR			_pColorTab	[BONE_NB_VERT];
		};
	}
}

#endif // __SCENEGRAPH_ENTITIES_SKELETON_BONE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
