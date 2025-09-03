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
//	CLASS:	TRACK_DATA_WRAPPER
//	The TRACK_DATA_WRAPPER class implements ...
//
//	01-10-17:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TRACK_DATA_WRAPPER_H__
#define __M3D_TRACK_DATA_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(TRACK_DATA)

#include INCL_3DENGINE(Behavior/AnimWrapper/ITrackWrapper)
#include INCL_3DENGINE(Behavior/Datas/TrackIndexes)

namespace QDT
{
	namespace M3D
	{
		class	TRACK_DATA_WRAPPER : public ITRACK_WRAPPER
		{
		public:

			TRACK_DATA_WRAPPER();
			explicit					TRACK_DATA_WRAPPER(const TRACK_DATA_WRAPPER & C);
			virtual						~TRACK_DATA_WRAPPER();
			TRACK_DATA_WRAPPER&			operator=(const TRACK_DATA_WRAPPER & C);
			void						Copy(const TRACK_DATA_WRAPPER & C);
			
			//	virtual functions
			virtual void						GetBaseRotation( QUATERNION& Q )const;
			virtual void						GetRotation( float rTime, QUATERNION& Q );
			virtual void						GetBaseTranslation( VECTOR& V )const;
			virtual void						GetTranslation( float rTime, VECTOR& V );
			virtual int							GetNbrRotKeys()const;
			virtual int							GetNbrTransKeys()const;
			virtual int							GetTrackNbrFrames()const;
			virtual int							GetTrackIndex()const;
			virtual int							GetAnimTops(const float rFrame, QDT_DLIST<int>& vEvents );	
			virtual const ANIM_PC*				GetAnimPc()const;
			virtual void						GetRotTrans(float rTime, QUATERNION_VECTOR & QV);

					void						SetTrack( TRACK_DATA *pTrack );
					void						SetTrackIndex( int nIndex );
					void						SetAnim(ANIM_PC *pAnim );
			

		protected:

			
					const TRACK_DATA*			GetTrack()const;
			

		private:
			
			TRACK_DATA					*_pTrack;
			ANIM_PC						*_pAnimPc;
			int							 _nTrackIndex;		// --> A renommer par _nNumTrack
			// pour eviter la confusion avec les
			// indexes sur la track

			TRACK_INDEXES				_TrackIndex;	// + tard -> enlever le pointeur	
			
		};
	}
}

#endif // __M3D_TRACK_DATA_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
