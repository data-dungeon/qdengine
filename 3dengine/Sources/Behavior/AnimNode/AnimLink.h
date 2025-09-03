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
//	CLASS:	ANIM_LINK
//	The ANIM_LINK class implements ...
//
//	01-10-11:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_LINK_H__
#define __M3D_ANIM_LINK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(CONTAINER)
DECL_3DENGINE(ANIM_NODE)
DECL_3DENGINE(ANIM_NODE_STRUCT)

#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	ANIM_LINK
		{
		public:

			enum	PLACE_POS
			{
				POS_END = -2,
				POS_FIRST = -1,
			};
			
			ANIM_LINK();
			~ANIM_LINK();

			explicit								ANIM_LINK(const ANIM_LINK & C);
			ANIM_LINK&								operator=(const ANIM_LINK & C);
			void									Copy(const ANIM_LINK & C);
			
			//	Accessors
			_INLINE_	const COM_INSTANCE&			GetPrevSeq()const ;
			_INLINE_	const COM_INSTANCE&			GetNextSeq()const ;
			_INLINE_	const COM_INSTANCE&			GetChild( int nNumChild )const ;
			_INLINE_	CONTAINER*					GetContainer()const;
			_INLINE_	void						SetPrevSeq( const COM_INSTANCE& CI );
			_INLINE_	void						SetNextSeq( const COM_INSTANCE& CI );
			_INLINE_	void						SetContainer( CONTAINER *pC );
			
			void									Reset();
			void									RemoveChild( const COM_INSTANCE& CI );
			int										GetChildPos( int nChildIndex )const;							
			void									AddSequence( const COM_INSTANCE& CI, ANIM_NODE *pHolder );
			void									InsertSequence( const COM_INSTANCE& CI, ANIM_NODE *pHolder );
			void									RemoveSequence( const COM_INSTANCE& CI );
			bool									IsPlayingAnim(const COM_INSTANCE& pIndex)const;

		protected:		
			
			void									ResetChildList();
			_INLINE_	int							GetChildIndex( int nPos ) const;
			_INLINE_	int							GetNbrChildren() const;

		private:
			
			COM_INSTANCE					_PrevSeq;
			COM_INSTANCE					_NextSeq;
			QDT_VECTOR<COM_INSTANCE>		_ChildList;
			CONTAINER						*_pContainer;
			QDT_VECTOR<int>					_nChildIndexlist;

		};
	}
}

#ifndef _DEBUG
	#include "AnimLink.inl"
#endif

#endif //__M3D_ANIM_LINK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
