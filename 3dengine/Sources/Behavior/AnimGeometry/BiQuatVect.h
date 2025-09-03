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
//	CLASS:	BI_QUAT_VECT
//	The BI_QUAT_VECT class implements ...
//
//	02-02-27:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BI_QUAT_VECT_H__
#define __M3D_BI_QUAT_VECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)

namespace QDT
{
	namespace M3D
	{
		class	BI_QUAT_VECT
		{
		public:

			BI_QUAT_VECT();
			explicit					BI_QUAT_VECT(const BI_QUAT_VECT & C);
										~BI_QUAT_VECT();
			BI_QUAT_VECT&				operator=(const BI_QUAT_VECT & C);
			void						Copy(const BI_QUAT_VECT & C);

		public:

			QUATERNION_VECTOR			_Absolute;
			QUATERNION_VECTOR			_Relative;

		};
	}
}

#endif // __M3D_BI_QUAT_VECT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
