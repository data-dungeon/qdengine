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
//	CLASS:	HIPS_ANIM
//	The HIPS_ANIM class implements ...
//
//	01-10-18:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_HIPS_ANIM_STRUCT_H__
#define __M3D_HIPS_ANIM_STRUCT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(STREAM_HANDLE)
DECL_3DENGINE(IANIM_WRAPPER)
DECL_3DENGINE(ITRACK_WRAPPER)

#include INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)

namespace QDT
{
	namespace M3D
	{
		class	HIPS_ANIM_STRUCT : public ANIM_NODE_STRUCT
		{
		public:
			
			HIPS_ANIM_STRUCT();
			explicit			HIPS_ANIM_STRUCT(const HIPS_ANIM_STRUCT & C);
			virtual				~HIPS_ANIM_STRUCT();
			HIPS_ANIM_STRUCT&	operator=(const HIPS_ANIM_STRUCT & C);
			void				Copy(const HIPS_ANIM_STRUCT & C);
		
						bool						ExecInstanciate( const COM_INSTANCE& Anim );
			virtual		void						InitAbsoluteTransfo();
			virtual		void						ReadRotTrans( float rTime );
			virtual		void						ReadRotation( float rTime, QUATERNION& Quat );
			virtual		void						ReadTranslation( float rTime, VECTOR& Vect );
			_INLINE_	const COM_INSTANCE&			GetAnim() const;
						STREAM_HANDLE*				GetStreamHandle();
		
		protected:
			_INLINE_	void						SetAnim( const COM_INSTANCE& Anim );								
			static		IANIM_WRAPPER*				CreateWrapperFromAnim( const COM_INSTANCE& Anim );
						bool						InstanciateAnim();
			virtual		void						ExecDelete();
						bool						CreateAndConfigureWrapper( const COM_INSTANCE& Anim );

		private:

			IANIM_WRAPPER							*_pAnimWrapper;
			COM_INSTANCE							_Anim;
			ITRACK_WRAPPER							*_pTrackWrapper;
			
		};
	}
}

#ifndef _DEBUG
	#include "HipsAnimStruct.inl"
#endif

#endif // __M3D_HIPS_ANIM_STRUCT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
