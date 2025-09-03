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
//	CLASS:	STREAM_TRACK
//	The STREAM_TRACK class implements ...
//
//	03-05-28:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_TRACK_H__
#define __M3D_STREAM_TRACK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(MEM_STREAM)
DECL_KSERV(QUATERNION_KEY)
DECL_KSERV(VECTOR_KEY)
DECL_KSERV(SCALE_KEY)
DECL_3DENGINE(ANIM_TOP)
DECL_3DENGINE(TRACK_INDEXES)

#include INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_TRACK
		{
		public:
			//////////////////////////////////////////////
			STREAM_TRACK();
			~STREAM_TRACK();
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			_INLINE_	int							GetNbrQuatKeys()const;
			_INLINE_	int							GetNbrTransKeys()const;
			_INLINE_	int							GetNbrScaleKeys()const;
			_INLINE_	int							GetNbrAnimTops()const;
						const	QUATERNION_KEY&		GetQuatKey( int nNum )const;
						const	VECTOR_KEY&			GetTransKey( int nNum )const;
						const	SCALE_KEY&			GetScaleKey( int nNum )const;
						const	ANIM_TOP&			GetAnimTop( int nNum )const;
			_INLINE_	float						GetBaseFrame()const;
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			_INLINE_	void		SetNbrQuatKeys( int n );
			_INLINE_	void		SetNbrTransKeys( int n );
			_INLINE_	void		SetNbrScaleKeys( int n );
			_INLINE_	void		SetNbrAnimTops ( int n );
						void		SetQuatKey( const QUATERNION_KEY& QK, int n );
						void		SetTransKey( const VECTOR_KEY& VK, int n );
						void		SetScaleKey( const SCALE_KEY& SK, int n );
						void		SetAnimTop( const ANIM_TOP& AT, int n );
			_INLINE_	float		SetBaseFrame( float rVal );
			void					SetQuatBuffer( QUATERNION_KEY *pQK );
			void					SetTransBuffer( VECTOR_KEY *pVK );
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			void*				Init( void *pBuffer );
			QUATERNION			GetRot( TRACK_INDEXES& TrackIndex, float rFrame );
			VECTOR				GetTrans( TRACK_INDEXES& TrackIndex, float rFrame );
			VECTOR				GetScale( TRACK_INDEXES& TrackIndex, float rFrame );
			QUATERNION_VECTOR	GetRotTrans( TRACK_INDEXES& TrackIndex, float rFrame );
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			void				FillStream( MEM_STREAM *pS );
			//////////////////////////////////////////////

		protected:

			//////////////////////////////////////////////
			int					_nNbrQuatKeys;
			int					_nNbrTransKeys;
			int					_nNbrScaleKeys;
			int					_nNbrAnimTops;
			QUATERNION_KEY		*_pQuatKeys;
			VECTOR_KEY			*_pTransKeys;
			SCALE_KEY			*_pScaleKeys;
			ANIM_TOP			*_pAnimTops;
			//////////////////////////////////////////////
		};
	}
}

#ifndef _DEBUG
	#include "StreamTrack.inl"
#endif

#endif // __M3D_STREAM_TRACK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
