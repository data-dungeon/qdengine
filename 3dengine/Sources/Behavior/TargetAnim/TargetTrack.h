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
//	CLASS:	TARGET_TRACK
//	The TARGET_TRACK class implements ...
//
//	03-11-13:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TARGET_TRACK_H__
#define __M3D_TARGET_TRACK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(TARGET_ANIM_STRUCT)

#include INCL_KMATH(Math/Geometry/Quaternions/Quaternion)

namespace QDT
{
	namespace M3D 
	{
		class	TARGET_TRACK
		{
		protected:

			struct TARGET_STRUCT
			{
				QUATERNION		_TargetDeltaQuat;
				float			_rDeltaX;
				float			_rDeltaY;
				float			_rDeltaZ;
				float			_rX;
				float			_rY;
				float			_rZ;
				float			_rBestAngleX;
				float			_rBestAngleY;
				float			_rDeltaTime;
				float			_rAbsX;
				float			_rAbsY;
				float			_rAbsZ;
				float			_rSaveX;
				float			_rSaveY;
			};

			enum RANGE_STATUS
			{
				IN_RANGE = 0,
				OUT_RANGE = 1
			};

		public:

			TARGET_TRACK();
			~TARGET_TRACK();
		
			void		Update( const VECTOR& TargetPoint, float rDelay, TARGET_ANIM_STRUCT *pT );
			void		AddInduction( const QUATERNION& Q );
			void		ResetParameters();
			void		InitFromPrevious( TARGET_TRACK	*pPrevTK );

			void		ResetInduction()						
			{ 
				_Induction.Id();				
			}

			bool		IsInduction()							
			{ 
				return ( _nIsTarget == 0 );	
			}

			void		SetInducted( int nVal )
			{
				if ( nVal )
					_nIsTarget = 0;
				else
					_nIsTarget = 1;
			}

			void		SetAcceleration( const VECTOR& V )
			{
				_rAccX = V.GetX();
				_rAccY = V.GetY();
				_rAccZ = V.GetZ();
			}

			void		SetInductionWeight( float rW )			
			{ 
				_rInductionWeight = rW;		
			}

			void		SetBoneType( int nType )				
			{ 
				_nBoneType = nType;			
			}

			int			GetBoneType()const						
			{ 
				return  ( _nBoneType );		
			}

			void		SetParentIndex( int nIndex )			
			{ 
				_nParentIndex = nIndex;		
			}

			void		SetTrackIndex( int nIndex )				
			{ 
				_nTrackIndex	= nIndex;		
			}

			void		SetRange( const VECTOR& V )
			{
				_rMaxX = V.GetX();
				_rMaxY = V.GetY();
				_rMaxZ = V.GetZ();
			}

			void		SetMaxSpeed( const VECTOR& V )
			{
				_rMaxVx = V.GetX();
				_rMaxVy = V.GetY();
				_rMaxVz	= V.GetZ();
			}

			const QUATERNION&	GetResultQuat()
			{
				return ( _AnimQuat );
			}

			int	GetTrackIndex()const
			{
				return ( _nTrackIndex );
			}

		protected:

			void		ComputeTargetRotation( TARGET_ANIM_STRUCT *pT, const VECTOR& Pt, TARGET_STRUCT *pTS );
			void		ComputeBaseRotation( TARGET_ANIM_STRUCT *pT, TARGET_STRUCT *pTS );	
			void		ComputeSpeed2(  TARGET_STRUCT *pTS  );
			void		ComputeAbsoluteDelta(  TARGET_ANIM_STRUCT *pT,TARGET_STRUCT *pTS, const VECTOR& TargetPoint );	
			void		IncludePropWeight(  TARGET_ANIM_STRUCT *pT, TARGET_STRUCT *pTS );
			void		BuildResultQuat(  TARGET_STRUCT *pTS, TARGET_ANIM_STRUCT *pT );
			void		ClampAngle( TARGET_STRUCT *pTS );		
			void		ProcessInductionQuat(  TARGET_ANIM_STRUCT *pT, TARGET_STRUCT *pTS );
			void		GetRootGlobalMat(   TARGET_ANIM_STRUCT *pT, MATRIX& Mat );
			void		GetMatInRootCoord(   TARGET_ANIM_STRUCT *pT, MATRIX& Mat );
			void		CheckOutOfRange( TARGET_STRUCT *pTS );
			void		OnParentOutOfRange( TARGET_ANIM_STRUCT *pT, TARGET_STRUCT *pTS );
			bool		IsInRange()const	{	return ( _nRangeStatus == IN_RANGE ); }
		
			static void		BuildQuatWithAnglesXYZ( QUATERNION& Q, float rX, float rY , float rZ );
			static void		GetAngleXYZFromQuat( QUATERNION& Q, float& rX, float& rY, float& rZ );
			


		protected:

			QUATERNION		_AnimQuat;
			QUATERNION		_Induction;
			float			_rAccX;
			float			_rAccY;
			float			_rAccZ;
			float			_rVx;
			float			_rVy;
			float			_rVz;
			float			_rMaxVx;
			float			_rMaxVy;
			float			_rMaxVz;
			float			_rMaxX;
			float			_rMaxY;
			float			_rMaxZ;
			float			_rX;
			float			_rY;
			float			_rZ;
			float			_rInductionWeight;
			int				_nBoneType;
			int				_nTrackIndex;
			int				_nParentIndex;
			int				_nIsTarget;
			RANGE_STATUS	_nRangeStatus;
			int				_nPad[2];

		};
	}
}

#endif // __M3D_TARGET_TRACK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
