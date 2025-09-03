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
// CLASS: TRACK_DATA
//
//
//00-11-24 GHP Created: 
//*****************************************************************************

#ifndef __M3D_TRACK_DATA_H__
#define __M3D_TRACK_DATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(TRACK_INDEXES)

#include INCL_KCORE(Template/QDT_Vector)
#include INCL_KCORE(Template/QDT_DList)
#include INCL_KCORE(Time/TimeCode)
#include INCL_KSERV(AnimTools/QuaternionKey)
#include INCL_KSERV(AnimTools/VectorKey)
#include INCL_KSERV(AnimTools/Key3)
#include INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)
#include INCL_3DENGINE(Behavior/Datas/TopAnim)

namespace QDT
{
	namespace M3D
	{
		class TRACK_DATA
		{
		public :

			enum PACKETS_VERSION { DIALOG_VERSION = 1, CYBER_VERSION, };

			explicit					TRACK_DATA(); 
			~TRACK_DATA() ;

			void						Init();

			int							GetSizeOf() const;
			//=============================================================================
			// Transferring values from the outside ( during Export process )  or to the outside
			//=============================================================================
			void						SetRotations		(const QDT_VECTOR<QUATERNION_KEY> & vector);
			void						SetTranslation		(const QDT_VECTOR<KEY_3> & vector);
			void						SetTranslation		(const QDT_VECTOR<VECTOR_KEY> & vector);
			void						SetScale			(const QDT_VECTOR<KEY_3> & vector);
			void						SetAnimTops			(QDT_VECTOR<ANIM_TOP> & vAnimTops); 
			void						GetBaseMatrix		(MATRIX & res) const;
			void						GetFirstMatrix		(MATRIX & res) const; 
			int							SetMatrix			(const Float32 & rFrame, MATRIX & mat);
			void						SetAbsoluteRotation( QUATERNION& Q );
			void						SetAbsoluteTranslation( VECTOR& V );
			void						SetQuatKey(UInt32 i, QUATERNION_KEY& QK);
			void						SetTransKey(UInt32 i, VECTOR_KEY& VK);
			void						SetNbrQuatKeys( int n );
			void						SetNbrTransKeys( int n );


			//=============================================================================
			// The update call
			//=============================================================================
			void					GetMatrix			(const Float32 rFrame, MATRIX & M);
			void		 			GetRot				(const Float32 rValue, QUATERNION & Q);
			void					GetTrans			(const Float32 rValue, VECTOR & V);
			_INLINE_ void			GetRotTrans			(const Float32 rValue, QUATERNION_VECTOR & QV);
					 void			GetScale			(const Float32 rValue, VECTOR & S);
					 int			GetAnimTops			(const Float32 rFrame, QDT_DLIST<int> & vEvents);
					 int			GetNextAnimTop		(Float32 & rTime, int & nCode);
			_INLINE_ void			GetBaseTranslation	(VECTOR & vTrans) const;
			_INLINE_ void			GetBaseRotation		(QUATERNION & Quat) const;
			_INLINE_ UInt32			GetNbrRotKeys		()const;
			_INLINE_ UInt32			GetNbrTransKeys		()const;
			_INLINE_ UInt32			GetNbrScaleKeys		()const;
			_INLINE_ UInt32			GetNbrAnimTops		()const;
					 void			GetQuatKey			(UInt32 n, QUATERNION & Q) const;
			_INLINE_ void		 	GetTransKey			(UInt32 n, VECTOR& V) const;
			_INLINE_ void			GetAnimTop			(UInt32 n, ANIM_TOP& AT) const;
			_INLINE_ Float32		GetQuatTime			(UInt32 n)const;
			_INLINE_ Float32		GetTransTime		(UInt32 n)const;
			//=============================================================================
			// Flags
			//=============================================================================
			_INLINE_ bool					HasRotation() const ;
			_INLINE_ bool					HasTranslation() const ;
			_INLINE_ bool					HasScaling() const ;
			_INLINE_ bool					HasAnimTops() const ;

			//=============================================================================
			// IO calls
			//=============================================================================	
#ifndef _MASTER_ONLY_DATABANK
			void						ReadV1  ( class IO_STREAM & Stream ) ;
			void						ReadV2  ( class IO_STREAM & Stream ) ;
			void						ReadV3  ( class IO_STREAM & Stream ) ;
			void						ReadV4  ( class IO_STREAM & Stream ) ;
#endif

#ifndef _MASTER
			void						Write	( class IO_STREAM & Stream ) ;
#endif

			//=============================================================================
			// Iterators set tup and reset
			//=============================================================================
			_INLINE_ void				SetReverse(bool bReverse = false);
					 void				ResetTrack();
					 void				ClearTrackIndexes(); 
			_INLINE_ void				SetTrackIndexes(TRACK_INDEXES *pTrackIndexes);

			int							GetDataSizeToWrite(const TIME_CODE &TCBegin, const TIME_CODE &TCEnd) const;

		private :

					 bool				CheckFirstRot	( const Float32 rValue, QUATERNION& Q ) ;
					 bool				CheckLastRot	( const Float32 rValue, QUATERNION& Q ) ;
			_INLINE_ bool				CheckFirstTrans	( const Float32 rValue, VECTOR& V ) ;
			_INLINE_ bool				CheckLastTrans	( const Float32 rValue, VECTOR& V ) ;
			_INLINE_ bool				CheckFirstScale	( const Float32 rValue, VECTOR& S ) ;
			_INLINE_ bool				CheckLastScale	( const Float32 rValue, VECTOR& S ) ;
			_INLINE_ bool				CheckLastAnimTop(Float32	rFrame, QDT_DLIST<int> & vAnimTopsValues);

			//=============================================================================
			// Read data section and members
			//=============================================================================
#ifndef _MASTER_ONLY_DATABANK
			void						ReadKeysDataRotations	(IO_STREAM & Stream);
			void						ReadKeysDataTranslations(IO_STREAM & Stream);
			void						ReadKeysDataScaling		(IO_STREAM & Stream);
			void						ReadReferenceData		(IO_STREAM & Stream);
			void						ReadAnimTops			(IO_STREAM & Stream);
			void						OptimizeRotationKeys	();
			void						OptimizeTranslationKeys	();
			void						SkipTranslationKeys		();
			void						SkipRotationKeys		();
			void						RemoveHalfRotationKeys	();
#endif

			//=============================================================================
			// Write data section and members
			//=============================================================================
#ifndef _MASTER	
			void						WriteKeysDataRotations		(IO_STREAM & Stream);
			void						WriteKeysDataTranslations	(IO_STREAM & Stream);
			void						WriteKeysDataScaling		(IO_STREAM & Stream);
			void						WriteReferenceData			(IO_STREAM & Stream);
			void						WriteAnimTops				(IO_STREAM & Stream);
#endif

			//=============================================================================
			// Real members containing the animation data
			//=============================================================================

			VECTOR						_vAbsTrans; 
			VECTOR						_sAbsScale;
			QUATERNION					_AbsQuat;

/**
	[MBi - 2006.01.25] - 
		Disabled USE_COMPRESSED_QUATERNIONS for the E3 Demo project.
		It seems that this compression scheme is not lossless, and results in unwanted noise for the end defectors.
		For the demo purposes, we rather favor accuracy over memory footprint. Moreover, this REALLY should be 
		part of the export pipeline...
 */
// #define USE_COMPRESSED_QUATERNIONS
#define SKIP_KEYS_WHEN_POSSIBLE

#ifdef USE_COMPRESSED_QUATERNIONS
			QUATERNION_COMPRESSED *		_pRotKeys;
#else
			QUATERNION *				_pRotKeys;
#endif

			VECTOR *					_pTransKeys;
			VECTOR *					_pScaleKeys;	
			ANIM_TOP *					_pAnimTops;

			UInt16 *					_pnRotTimes;
			Float32 *					_prScaleTimes;

			TRACK_INDEXES			   *_pTrackIndexes;

			UInt32						_nNbrRotKeys;
			UInt32						_nNbrTransKeys;
			UInt32						_nNbrScaleKeys;
			UInt32						_nNbrAnimTops;
			int							_nLastVal; 
			Float32						_rFPS;
			char						_Padding[5];

			bool						_bReverse;
			bool						_bNeedReset;
		};
	}
}

#ifndef _DEBUG
	#include "TrackData.inl"
#endif

#endif // __M3D_TRACK_DATA_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
