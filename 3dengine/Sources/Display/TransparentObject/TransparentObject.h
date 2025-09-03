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
//	CLASS:	TRANSPARENT_OBJECT
//	The TRANSPARENT_OBJECT class implements ...
//
//	03-11-26:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_TRANSPARENTOBJECT_H__
#define __DISPLAY_TRANSPARENTOBJECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(ENTITY_ABC)

namespace QDT
{
	namespace M3D
	{
		class	TRANSPARENT_OBJECT
		{
		public:

									TRANSPARENT_OBJECT();
									TRANSPARENT_OBJECT(Float32 rCamDistance, ENTITY_ABC * pEntity, UInt32 nPrimitive, UInt8 uiPriority);
									TRANSPARENT_OBJECT(const TRANSPARENT_OBJECT & C);
									~TRANSPARENT_OBJECT();
			
			TRANSPARENT_OBJECT &	operator = (const TRANSPARENT_OBJECT & TransObj);
			bool					operator < (const TRANSPARENT_OBJECT & TransObj) const;

			void					Display();

			_INLINE_ void			SetCameraDistance(Float32 rDistance);
			_INLINE_ void			SetEntity(ENTITY_ABC * pEntity);
			_INLINE_ void			SetPrimitive(UInt32 nPrimitive);
			_INLINE_ void			SetPriority(UInt8 uiPriority);
			
			_INLINE_ Float32		GetCameraDistance() const;
			_INLINE_ ENTITY_ABC *	GetEntity();
			_INLINE_ UInt32			GetPrimitive() const;

		private:

			Float32			_rCameraDistance;
			ENTITY_ABC *	_pEntity;
			UInt32			_nPrimitive;
			UInt8			_uiPriority;
		};
	}
}

#ifndef _DEBUG
	#include	"TransparentObject.inl"
#endif

#endif // __DISPLAY_TRANSPARENTOBJECT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
