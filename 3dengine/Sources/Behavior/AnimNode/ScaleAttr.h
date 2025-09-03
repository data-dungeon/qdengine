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
//	CLASS:	SCALE_ATTR
//	The SCALE_ATTR class implements ...
//
//	01-10-11:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCALE_ATTR_H__
#define __M3D_SCALE_ATTR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class	SCALE_ATTR
		{
		public:
			
			SCALE_ATTR();
			explicit				SCALE_ATTR(const SCALE_ATTR & C);
			~SCALE_ATTR();

			SCALE_ATTR&				operator=(const SCALE_ATTR & C);
			void					Copy(const SCALE_ATTR & C);
			
			_INLINE_	void		SetTransScale( const VECTOR& v );
			void					ScaleVector( VECTOR& V ) const;
			
		private:

			VECTOR					_vTransScale;

		};
	}
}

#ifndef _DEBUG
	#include "ScaleAttr.inl"
#endif

#endif // #ifndef __M3D_SCALE_ATTR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
