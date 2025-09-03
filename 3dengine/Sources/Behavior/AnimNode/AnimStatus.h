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
//	CLASS:	ANIM_STATUS
//	The ANIM_STATUS class implements ...
//
//	01-10-11:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_STATUS_H__
#define __M3D_ANIM_STATUS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtFlags)

namespace QDT
{
	namespace M3D
	{
		class	ANIM_STATUS
		{
		public:

			
			enum	ANIM_FLAG
			{
				ANIM_ABSOLUTE			= 1<<0,
				ANIM_TELEPORT			= 1<<1,
				ANIM_LOOP				= 1<<2,
				ANIM_PINGPONG			= 1<<3,
				ANIM_PREEMPTIVE			= 1<<4,
				ANIM_MIX				= 1<<5,
				ANIM_RELCOR				= 1<<6,
				ANIM_BLENDED			= 1<<7,
				ANIM_DELEGATE_BLENDING	= 1<<8,
				ANIM_ENABLE_DELETE		= 1 << 9,
				ANIM_PERSISTANT			= 1 << 10,
			};
			
			ANIM_STATUS();
			explicit	ANIM_STATUS(const ANIM_STATUS & C);
			~ANIM_STATUS();

			ANIM_STATUS&				operator=(const ANIM_STATUS & C);
			void						Copy(const ANIM_STATUS & C);
			
			_INLINE_	bool				IsAbsolute()const;
			_INLINE_	bool				IsRelative()const;
			_INLINE_	bool				IsTeleport()const;
			_INLINE_	bool				IsLoop()const;
			_INLINE_	bool				IsPingPong()const;
			_INLINE_	bool				IsPreemptive()const;
			_INLINE_	bool				IsMix()const;
			_INLINE_	bool				IsCompose()const;
			_INLINE_	bool				IsRelCor()const;
			_INLINE_	bool				IsBlended()const;
			_INLINE_	bool				IsDelegateBlended()const;
			_INLINE_	bool				CanBeDelete()const;
			_INLINE_	bool				IsPersistant()const;
			

			_INLINE_	void				SetAbsolute( bool bVal );
			_INLINE_	void				SetRelative( bool bVal );
			_INLINE_	void				SetTeleport( bool bVal );
			_INLINE_	void				SetLoop( bool b );
			_INLINE_	void				SetPingPong( bool b );
			_INLINE_	void				SetPreemptive( bool b );
			_INLINE_	void				SetMix( bool b );
			_INLINE_	void				SetCompose( bool b );
			_INLINE_	void				SetRelCor( bool b );
			_INLINE_	void				SetBlended( bool b );
			_INLINE_	void				SetDelegateBlended( bool b );
			_INLINE_	void				EnableDeletation( bool b = true );
			_INLINE_	void				SetPersistant( bool b = true );
			
			void		Reset();
			
		protected:
			
		private:
			
			QDT_FLAGS					_nFlags;
			
		};
	}
}

#ifndef _DEBUG
	#include "AnimStatus.inl"
#endif

#endif // #ifndef __M3D_ANIM_STATUS_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
