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
//	CLASS:	FRAME
//	The FRAME class implements ...
//
//	02-06-10:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_FRAME_FRAME_H__
#define __KMATH_MATH_FRAME_FRAME_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class FRAME
		{
		protected:
			MATRIX _pTf;   
			MATRIX _fTp;	
			VECTOR _Trans;  
			FRAME *_pParent;

		public:
			/** */
			FRAME(const VECTOR &Trans = VECTOR(0,0,0), FRAME *pParent = NULL);
			/** */
			virtual ~FRAME();

			/** */
			VECTOR GetTranslation() const;
			/** */
			void SetTranslation(const VECTOR &v);
			
			/**
			* Get/Set rotation matrix ( transform from this to parent frame )
			* or rotation matrix in parent coords
			*/

			/** */
			MATRIX GetFrameToParent() const;
			/** */
		//	void SetFrameToParent(const MATRIX &M);

			/**
			* Get/Set rotation matrix ( transfrom from parent to this frame)
			*/
			/** */
			MATRIX GetParentToFrame() const;
			/** */
		//	void SetParentToFrame(const MATRIX &M);
			
			/** */
			void Apply(VECTOR &v) const;
			/** */	
			VECTOR Apply(const VECTOR &src) const;

			/** */
			FRAME* GetParent();
			/** */
			void SetParent(FRAME *pParent);
			
		};
	}
}

#ifndef _DEBUG
#include "Frame.inl"
#endif

#endif //__KMATH_MATH_FRAME_FRAME_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
