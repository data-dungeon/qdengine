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
//	CLASS:	SORTED_OBJECT
//	The SORTED_OBJECT class implements ...
//
//	04-07-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_SORTEDOBJECT_H__
#define __DISPLAY_SORTEDOBJECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(ENTITY_ABC)

#include	INCL_KCORE(Template/QDT_Vector)
#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		class	SORTED_OBJECT
		{
		public:

			SORTED_OBJECT();
			SORTED_OBJECT(const SORTED_OBJECT & C);
			SORTED_OBJECT(ENTITY_ABC * pEntity, UInt32 nPrimitive);
			~SORTED_OBJECT();

			SORTED_OBJECT &	operator=(const SORTED_OBJECT & C);

			_INLINE_ ENTITY_ABC *	GetEntity()	{ return (_pEntity); }
			_INLINE_ void			Display()	{ _pEntity->Display(_vPrimitives); }

					 void	AddPrimitive(UInt32 nPrimitive);

		private:

			ENTITY_ABC *		_pEntity;
			QDT_VECTOR<UInt32>	_vPrimitives;
		};
	}
}

#endif // __DISPLAY_SORTEDOBJECT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
