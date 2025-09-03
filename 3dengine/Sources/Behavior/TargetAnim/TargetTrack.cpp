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
//
//	03-11-13:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"TargetTrack.h"
#include	INCL_3DENGINE(Behavior/TargetAnim/TargetAnimStruct)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TARGET_TRACK constructor
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
TARGET_TRACK::TARGET_TRACK()
:
_rAccX( 0.0f ),
_rAccY( 0.0f ),
_rAccZ( 0.0f ),
_rVx( 0.0f ),
_rVy( 0.0f ),
_rVz( 0.0f ),
_rMaxVx( 0.0f ),
_rMaxVy( 0.0f ),
_rMaxVz( 0.0f ),
_rMaxX( 0.0f ),
_rMaxY( 0.0f ),
_rMaxZ( 0.0f ),
_rX( 0.0f ),
_rY( 0.0f ),
_rZ( 0.0f ),
_rInductionWeight( 1.0f),
_nBoneType( -1 ),
_nTrackIndex( -1 ),
_nParentIndex( -1 ),
_nIsTarget( 0 ),
_nRangeStatus( IN_RANGE )
{
	_AnimQuat.Id();
	_Induction.Id();
	
	float rRet = 90.0f / 30.0f * MATH::PI() / 180.0f;
	_rMaxVx = rRet;
	_rMaxVy = rRet;
}

//-----------------------------------------------------------------------------
//	Name:		TARGET_TRACK destructor
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
TARGET_TRACK::~TARGET_TRACK()
{
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::Update(const VECTOR &			TargetPoint,
							 float					rDelay,
							 TARGET_ANIM_STRUCT *	pT)
{
	TARGET_STRUCT	TargetStr;
	
	//////////////////////////////////////////////////////
	TargetStr._rDeltaTime = rDelay ;
//	QDT_ASSERT( rDelay > 0.0f );
	ComputeTargetRotation( pT, TargetPoint, &TargetStr );
	//////////////////////////////////////////////////////
	ComputeBaseRotation( pT, &TargetStr );
	//////////////////////////////////////////////////////
	IncludePropWeight( pT, &TargetStr );
	//////////////////////////////////////////////////////
	//	New algo
//	ComputeSpeed( &TargetStr );
	ComputeSpeed2( &TargetStr );
	//////////////////////////////////////////////////////
	BuildResultQuat( &TargetStr, pT );
	//////////////////////////////////////////////////////
	if ( _nParentIndex >= 0 )
	{
		ProcessInductionQuat( pT, &TargetStr );
	}
	//////////////////////////////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		ComputeTargetRotation
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ComputeTargetRotation(TARGET_ANIM_STRUCT *	pT,
											const VECTOR &			Pt,
											TARGET_TRACK::TARGET_STRUCT *	pTS)
{
	if ( IsInduction() == false )
	{
		MATRIX		Mat;
		MATRIX		InvMat;
		VECTOR		BonePos;
		VECTOR		V;
		VECTOR		LocV;
		MATRIX		Mat2;
		QUATERNION	Q;
		float		rX, rY, rZ;

		//	Global Mat;
		Mat = pT->GetGlobalMatrix( _nTrackIndex );
		//	Bone Pos
		BonePos = Mat.GetTranslation();
		//	Unit Vector fromp Bone to Target
		V.SetFromPoints(BonePos, Pt);
		//	Set vector in root coordinates
		GetRootGlobalMat( pT, Mat2);
		InvMat.InverseAffine( Mat2 );
		LocV = InvMat.ApplyRotation( V );
		LocV.Normalize();

		//	Get this mat in root coord
		GetMatInRootCoord( pT, Mat );
		Mat.GetQuaternion( Q );
		GetAngleXYZFromQuat( Q, rX, rY, rZ );

		//	Resulting Quat
		pTS->_TargetDeltaQuat.SetRotation( VECTOR::UNIT_Z, LocV );
		//	Angle X, Y, Z
		GetAngleXYZFromQuat( pTS->_TargetDeltaQuat, pTS->_rDeltaX, pTS->_rDeltaY, pTS->_rDeltaZ );
//		CheckDeltaWay( pTS );
		pTS->_rDeltaX -= rX;
		pTS->_rDeltaY -= rY;
		pTS->_rDeltaZ -= rZ;

	}
	else
	{
		_Induction.Normalize();
		pTS->_TargetDeltaQuat = _Induction;
		//	Angle X, Y, Z
		GetAngleXYZFromQuat( pTS->_TargetDeltaQuat, pTS->_rDeltaX, pTS->_rDeltaY, pTS->_rDeltaZ );
		if ( _nParentIndex == -1 )
		{
			ComputeAbsoluteDelta( pT, pTS, Pt );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeBaseRotation
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ComputeBaseRotation(TARGET_ANIM_STRUCT *	pT,
										  TARGET_TRACK::TARGET_STRUCT *		pTS)
{
	VECTOR			V, Vz;

	pTS->_rX = _rX;
	pTS->_rY = _rY;
	pTS->_rZ = _rZ;
}
/*
//-----------------------------------------------------------------------------
//	Name:		ComputeSpeed
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ComputeSpeed(TARGET_TRACK::TARGET_STRUCT *	pTS)
{
	float		rAX, rAY, rAZ;

	VECTOR		V;
	VECTOR		Acc;
	VECTOR		Vcc;
	VECTOR		dV;
	float		rA, rV;
	float		Pi2;
	float		l;
	float		rRatioX;
	float		rRatioY;

	//
	rAX = _rAccX;
	rAY = _rAccY;
	rAZ = _rAccZ;
	Pi2 = MATH::PI() * 0.5f;

	//	Get vector to go to best angle
	V.SetXYZ( pTS->_rBestAngleX, pTS->_rBestAngleY, 0.0f );
	l = V.Length();
	if ( l > MATH::EPSILON() )
	{
		V.Normalize();
	}

	//	Get speed vector
	Vcc.SetXYZ( _rMaxVx, _rMaxVy, 0.0f );
	rV = Vcc.Length();

	//	Get speed vector to go to best angle
	V.Mul( rV );

	//	Current speed vector
	Vcc.SetXYZ( _rVx, _rVy, 0.0f );

	//	Compute Normalized acceleration to go from
	//	current speed vector to best speed vector
	dV = V - Vcc;
	dV.Normalize();

	//	Compute acceleration vector
	Acc.SetXYZ( _rAccX, _rAccY, 0.0f );
	rA = Acc.Length() ;
	rAX = dV.GetX() * rA;
	rAY = dV.GetY() * rA;

	//	Speed;
	_rVx += rAX * pTS->_rDeltaTime;
	_rVy += rAY * pTS->_rDeltaTime;
	_rVz += rAZ * pTS->_rDeltaTime;

	//	Modify speed to avoid extrem behavior
	float	rAffX, rAffY;

	rAffX	= 1.0f;
	rAffY	= 1.0f;
	if ( pTS->_rDeltaTime > MATH::EPSILON() )
	{
		rAffY = rAffX = pTS->_rDeltaTime;
	}

	if ( MATH::Abs( _rVx ) > MATH::EPSILON() )
	{
		rRatioX = pTS->_rBestAngleX / (_rVx * rAffX );
		if ( rRatioX >= 0.0f && rRatioX <= 4.0f  )
		{
			_rVx = 1.0f / ( 4.0f * rAffX ) * pTS->_rBestAngleX;
		}
	}

	if ( MATH::Abs( _rVy ) > MATH::EPSILON() )
	{
		rRatioY = pTS->_rBestAngleY / ( _rVy * rAffY );
		if ( rRatioY >= 0.0f && rRatioY <= 4.0f  )
		{
			_rVy = 1.0f / ( 4.0f * rAffY ) * pTS->_rBestAngleY;
		}
	}
	//

	rRatioX = 2.0f * MATH::Abs( pTS->_rBestAngleX ) / Pi2;
	rRatioY = 2.0f * MATH::Abs( pTS->_rBestAngleY ) / Pi2;
	if ( rRatioX > 0.2f )
	{
		rRatioX = 0.95f;
	}

	if ( rRatioY > 0.2f )
	{
		rRatioY = 0.95f;
	}

	_rVx *= rRatioX;
	_rVy *= rRatioY;


	//	Clamp speed
	ClampSpeed();

}*/

//-----------------------------------------------------------------------------
//	Name:		ComputeSpeed2
//	Object:		
//	04-11-04:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ComputeSpeed2(TARGET_STRUCT *	pTS)
{
	VECTOR	V;
	VECTOR	VTemp;
	float	rLength;
	float	rRatio;
	float	rRelDist;
	float	rCoeff;

	V.SetXYZ( pTS->_rBestAngleX, pTS->_rBestAngleY, 0.0f );
	VTemp = V;
	rLength = V.Length();
	if ( rLength > MATH::EPSILON() )
	{
		V.Normalize();
	}

	//	Compute speed following max speed
	_rVx = _rMaxVx * V.GetX();
	_rVy = _rMaxVy * V.GetY();
	_rVz = 0.0f;

	//	Compute ratio : this is to smooth speed reduction when reaching the target.
	//	We use for this a coefficient representing 18 degrees ( 0.1 ).
	rCoeff = 0.1f;
	rRelDist = rLength / MATH::PI();
	if ( rRelDist < rCoeff )
	{
		rRatio = 1.0f - ( rCoeff - rRelDist ) / rCoeff;
		_rVx *= rRatio;
		_rVy *= rRatio;
	}
	//	Now avoid mico-movements
	rCoeff = 0.03f;
	if ( rRelDist < rCoeff )
	{
		rRatio = 1.0f - ( rCoeff - rRelDist ) / rCoeff;
		_rVx *= rRatio;
		_rVy *= rRatio;
	}



}

/*
//-----------------------------------------------------------------------------
//	Name:		ClampSpeed
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ClampSpeed()
{
	if ( MATH::Abs( _rVx ) > _rMaxVx )
	{
		if ( _rVx < 0.0f )
		{
			_rVx = -_rMaxVx;
		}
		else
		{
			_rVx = _rMaxVx;
		}
	}
	if ( MATH::Abs( _rVy ) > _rMaxVy )
	{
		if ( _rVy < 0.0f )
		{
			_rVy = -_rMaxVy;
		}
		else
		{
			_rVy = _rMaxVy;
		}
	}
	if ( MATH::Abs( _rVz ) > _rMaxVz )
	{
		if ( _rVz < 0.0f )
		{
			_rVz = -_rMaxVz;
		}
		else
		{
			_rVz = _rMaxVz;
		}
	}
}*/

//-----------------------------------------------------------------------------
//	Name:		IncludePropWeight
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::IncludePropWeight(TARGET_ANIM_STRUCT *	pT,
										TARGET_STRUCT *			pTS)
{
	if ( _nParentIndex < 0 )
	{
		CheckOutOfRange( pTS );
		pTS->_rBestAngleX = pTS->_rDeltaX;
		pTS->_rBestAngleY = pTS->_rDeltaY;

		return;
	}

	float			rPX, rPY;
	TARGET_TRACK	*pPT;
	float			rDX, rDY;
	float			rCoeff;

	pPT = pT->GetTargetTrack( _nParentIndex );


	rPX = pPT->_rX;
	rPY = pPT->_rY;

	rDX = rPX + _rX + pTS->_rDeltaX;
	rDY = rPY + _rY + pTS->_rDeltaY;

	rCoeff = _rMaxX / ( _rMaxX + pPT->_rMaxX );
	pTS->_rBestAngleX = 0.5f * ( rDX * rCoeff - _rX ) + 0.5f * pTS->_rDeltaX;

	rCoeff = _rMaxY / ( _rMaxY + pPT->_rMaxY );
	pTS->_rBestAngleY = 0.5f * ( rDY * rCoeff - _rY ) + 0.5f * pTS->_rDeltaY;

}

//-----------------------------------------------------------------------------
//	Name:		BuildResultQuat
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::BuildResultQuat(TARGET_TRACK::TARGET_STRUCT *	pTS, TARGET_ANIM_STRUCT *pT)
{
	float	rHalfPI;
	float	rVx, rVy, rVz;

	pTS->_rSaveX = _rX;
	pTS->_rSaveY = _rY;
	rVx = _rVx * pTS->_rDeltaTime;
	rVy = _rVy * pTS->_rDeltaTime;
	rVz = _rVz * pTS->_rDeltaTime;

	pTS->_rZ		+= rVz;

	if ( MATH::Abs( pTS->_rDeltaX ) <= MATH::Abs( rVx ) )
	{
		pTS->_rX		+= pTS->_rDeltaX;
		pTS->_rDeltaX	= 0.0f;
		_rVx			= 0.0f;
	}
	else
	{
		pTS->_rX		+= rVx;
		pTS->_rDeltaX	-= rVx;
	}

	if ( MATH::Abs( pTS->_rDeltaY ) <= MATH::Abs( rVy ) )
	{
		pTS->_rY		+= pTS->_rDeltaY;
		pTS->_rDeltaY	= 0.0f;
		_rVy			= 0.0f;
	}
	else
	{
		pTS->_rY		+= rVy;
		pTS->_rDeltaY	-= rVy;
	}

	pTS->_rDeltaZ	-= rVz;


	
	rHalfPI = 0.5f * MATH::PI();
	if ( MATH::Abs( pTS->_rDeltaX ) > rHalfPI )
	{
		pTS->_rDeltaX *= 0.5f;
	}
	if ( MATH::Abs( pTS->_rDeltaY ) > rHalfPI )
	{
		pTS->_rDeltaY *= 0.5f;
	}

	BuildQuatWithAnglesXYZ( pTS->_TargetDeltaQuat, pTS->_rDeltaX, pTS->_rDeltaY, pTS->_rDeltaZ );

	//	Clamp Angle
	ClampAngle( pTS );
	


	//////////////////	
	//	Manage when parent out of range
	OnParentOutOfRange( pT, pTS );
	//////////////////	
	
	BuildQuatWithAnglesXYZ( _AnimQuat, pTS->_rX, pTS->_rY, pTS->_rZ );

	_rX = pTS->_rX;
	_rY = pTS->_rY;
	_rZ = pTS->_rZ;
}

//-----------------------------------------------------------------------------
//	Name:		OnParentOutOfRange
//	Object:		
//	04-11-23:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::OnParentOutOfRange(TARGET_ANIM_STRUCT *	pT,
										 TARGET_TRACK::TARGET_STRUCT *		pTS)
{
	TARGET_TRACK	*pP;


	if ( _nParentIndex < 0 )	return;

	pP = pT->GetTargetTrack(_nParentIndex );

	if ( pP->IsInRange() )	return;


	VECTOR	V;
	float	rVx;
	float	rVy;
	float	rVz;
	VECTOR	VTemp;
	float	rLength;
	float	rRatio;
	float	rRelDist;
	float	rCoeff;

	V.SetXYZ( -_rX, -_rY, 0.0f );
	VTemp = V;
	rLength = V.Length();
	if ( rLength > MATH::EPSILON() )
	{
		V.Normalize();
	}

	//	Compute speed following max speed
	rVx = _rMaxVx * V.GetX();
	rVy = _rMaxVy * V.GetY();
	rVz = 0.0f;

	//	Compute ratio : this is to smooth speed reduction when reaching the target.
	//	We use for this a coefficient representing 18 degrees ( 0.1 ).
	rCoeff = 0.1f;
	rRelDist = rLength / MATH::PI();
	if ( rRelDist < rCoeff )
	{
		rRatio = 1.0f - ( rCoeff - rRelDist ) / rCoeff;
		rVx *= rRatio;
		rVy *= rRatio;
	}
	//	Now avoid mico-movements
	rCoeff = 0.03f;
	if ( rRelDist < rCoeff )
	{
		rRatio = 1.0f - ( rCoeff - rRelDist ) / rCoeff;
		rVx *= rRatio;
		rVy *= rRatio;
	}

	rVx *=pTS->_rDeltaTime;
	rVy *=pTS->_rDeltaTime;
	pTS->_rX = pTS->_rSaveX + rVx;
	pTS->_rY = pTS->_rSaveY + rVy;

}


//-----------------------------------------------------------------------------
//	Name:		ClampAngle
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ClampAngle(TARGET_TRACK::TARGET_STRUCT *	pTS)
{
	if ( MATH::Abs( pTS->_rX ) > _rMaxX )
	{
		if ( pTS->_rX < 0.0f )
		{
			pTS->_rX	= -_rMaxX;
		}
		else
		{
			pTS->_rX = _rMaxX;
		}
		_rVx		= 0.0f;
	}
	if ( MATH::Abs( pTS->_rY ) > _rMaxY )
	{
		if ( pTS->_rY < 0.0f )
		{
			pTS->_rY = -_rMaxY;
		}
		else
		{
			pTS->_rY = _rMaxY;
		}
		_rVy		= 0.0f;
	}
	if ( MATH::Abs( pTS->_rZ ) > _rMaxZ )
	{
		if ( pTS->_rZ < 0.0f )
		{
			pTS->_rZ = -_rMaxZ;
		}
		else
		{
			pTS->_rZ = _rMaxZ;
		}
		_rVz		= 0.0f;
	}
}


//-----------------------------------------------------------------------------
//	Name:		ProcessInductionQuat
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ProcessInductionQuat(TARGET_ANIM_STRUCT *pT, TARGET_TRACK::TARGET_STRUCT *	pTS)
{
	QUATERNION		Q;
	QUATERNION		InductionResult;


	Q	= pTS->_TargetDeltaQuat;
	InductionResult.BasicInterSLERPId( _rInductionWeight, Q );
	pT->AddInductionToParent( _nParentIndex, InductionResult );	

}
/*
//-----------------------------------------------------------------------------
//	Name:		BuildQuatWithAnglesXY
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::BuildQuatWithAnglesXY(QUATERNION &	Q,
											float			rX,
											float			rY)
{
	QUATERNION	QP, QH;
	VECTOR		V;

	/////////////////////////////
	V.SetXYZ( 1.0f, 0.0f, 0.0f );
	QP.SetVectRoll( V, rX );
	/////////////////////////////
	V.SetXYZ( 0.0f, 1.0f, 0.0f );
	QH.SetVectRoll( V, rY );
	/////////////////////////////
	Q = QH * QP;
	/////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetAngleXYFromQuat
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::GetAngleXYFromQuat(QUATERNION &	Q,
										 float &		rX,
										 float &		rY)
{
	MATRIX				Mat;
	VECTOR				V;
	QUATERNION			QP, QH, QR;

	Mat.Identity();
	Mat.SetQuaternion( Q );

	///////////////////////////////////
	Mat.GetPitchHeadRoll( QP, QH, QR );
	///////////////////////////////////
	QP.GetVectRoll( V, rX );
	if ( V.GetX() < 0.0f )	rX = -rX;
	///////////////////////////////////
	QH.GetVectRoll( V, rY );
	if ( V.GetY() < 0.0f )	rY = -rY;
	///////////////////////////////////
}*/

//-----------------------------------------------------------------------------
//	Name:		BuildQuatWithAnglesXYZ
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::BuildQuatWithAnglesXYZ(QUATERNION &	Q,
											 float			rX,
											 float			rY,
											 float			rZ)
{
	QUATERNION	QP, QH, QR;
	VECTOR		V;

	/////////////////////////////
	V.SetXYZ( 1.0f, 0.0f, 0.0f );
	QP.SetVectRoll( V, rX );
	/////////////////////////////
	V.SetXYZ( 0.0f, 1.0f, 0.0f );
	QH.SetVectRoll( V, rY );
	/////////////////////////////
	V.SetXYZ( 0.0f, 0.0f, 1.0f );
	QR.SetVectRoll( V, rZ );
	/////////////////////////////
	Q = QR * QH * QP;
	/////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		GetAngleXYZFromQuat
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::GetAngleXYZFromQuat(QUATERNION &	Q,
										  float &		rX,
										  float &		rY,
										  float &		rZ)
{
	VECTOR				V;
	QUATERNION			QP, QH;
	QUATERNION			InvQ;
	VECTOR				Vy;
	VECTOR				Vz( 0.0f, 0.0f, 1.0f );
	VECTOR				Vx;

	///////////////////////////////////
	Vy = Q * Vz;
	Vx = Vy;
	Vy.SetY( 0.0f );
	if ( Vy.Length() <= MATH::EPSILON() )
	{
		QH.Id();
	}
	else
	{
		Vy.Normalize();
		QH.SetRotation( Vz, Vy );
	}
	///////////////////////////////////

	///////////////////////////////////
	QH.GetVectRoll( V, rY );
	if ( V.GetY() < 0.0f )
	{
		rY = -rY;
	}
	///////////////////////////////////
	InvQ.Inverse( QH );
	V = InvQ * Vx;
	
	QP.SetRotation( Vz, V );
	///////////////////////////////////

	///////////////////////////////////
	QP.GetVectRoll( V, rX );
	if ( V.GetX() < 0.0f )	rX = -rX;
	///////////////////////////////////
	rZ = 0.0f;
	///////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		AddInduction
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::AddInduction(const QUATERNION &	Q)
{
	QUATERNION QSave;

	QSave = _Induction;
	_Induction = QSave * Q;
}

//-----------------------------------------------------------------------------
//	Name:		ResetParameters
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ResetParameters()
{
	_rVx	= 0.0f;
	_rVy	= 0.0f;
	_rVz	= 0.0f;
	_rX		= 0.0f;
	_rY		= 0.0f;
	_rZ		= 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		GetRootGlobalMat
//	Object:		
//	04-01-20:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::GetRootGlobalMat(TARGET_ANIM_STRUCT * pT, 
									   MATRIX&				Mat)
{
	TARGET_TRACK	*pTT;
	int				nTrackIndex;
	MATRIX			M1;
	MATRIX			M2;
	MATRIX			InvMat;

	pTT = pT->GetTargetTrack( 0 );
	nTrackIndex = pTT->GetTrackIndex();

	M1 = pT->GetGlobalMatrix( nTrackIndex );
	M2 = pT->GetContainerMatrix( nTrackIndex );
	InvMat.InverseAffine(M2);
	Mat.MakeAffine();
	Mat.MulAffine(InvMat, M1);
}

//-----------------------------------------------------------------------------
//	Name:		GetMatInRootCoord
//	Object:		
//	04-01-20:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::GetMatInRootCoord( TARGET_ANIM_STRUCT *	pT, 
										MATRIX&					Mat)
{
	MATRIX		M1;
	MATRIX		M2;
	MATRIX		M3;

	GetRootGlobalMat(pT, M1);
	M2 = pT->GetGlobalMatrix(_nTrackIndex);

	M3.InverseAffine(M1);
	Mat.MulAffine(M2, M3);
}
/*
//-----------------------------------------------------------------------------
//	Name:		CheckDeltaWay
//	Object:		
//	04-01-20:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::CheckDeltaWay(TARGET_STRUCT *	pTS)
{
	float	rPI;

	rPI = MATH::PI();

	if ( MATH::Abs( pTS->_rDeltaX ) > rPI )
	{
		if ( pTS->_rDeltaX < 0.0f )
		{
			pTS->_rDeltaX = 2.0f * rPI - pTS->_rDeltaX;
		}
		else
		{
			pTS->_rDeltaX = pTS->_rDeltaX - 2.0f * rPI;
		}
	}

	if ( MATH::Abs( pTS->_rDeltaY ) > rPI )
	{
		if ( pTS->_rDeltaY < 0.0f )
		{
			pTS->_rDeltaY = 2.0f * rPI - pTS->_rDeltaY;
		}
		else
		{
			pTS->_rDeltaY = pTS->_rDeltaY - 2.0f * rPI;
		}
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		CheckOutOfRange
//	Object:		
//	04-03-16:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::CheckOutOfRange( TARGET_STRUCT *		pTS )
{
	float		rSumX;
	float		rAbsSumX;
	float		rOldSumX;
	float		rLimitX;
	float		rSumY;
	float		rAbsSumY;
	float		rOldSumY;
	float		rLimitY;
	bool		bXIn;
	bool		bYIn;


	rOldSumY = rSumY	= pTS->_rAbsY;
	rAbsSumY = MATH::Abs( rSumY );
	rOldSumX = rSumX	= pTS->_rAbsX;
	rAbsSumX = MATH::Abs( rSumX );


	//	We make a state machine to manage rLimit value [0.65PI;0.70PI]
	if ( IsInRange() )
	{
		rLimitY	= MATH::PI() *0.7f;
		rLimitX	= MATH::PI() *0.35;
	}
	else
	{
		rLimitY = MATH::PI() *0.65f;
		rLimitX = MATH::PI() *0.325f;
	}

	bXIn = false;
	bYIn = false;

	_nRangeStatus = IN_RANGE;
	if (  rAbsSumY <= _rMaxY )
	{
		bYIn = true;
	}
	if (  rAbsSumX <= _rMaxX )
	{
		bXIn = true;
	}


	//bXIn = true;				//	We don't 
	if ( bXIn && bYIn )
	{
		return;
	}


	_nRangeStatus = OUT_RANGE;
	if ( (rAbsSumX > rLimitX) || (rAbsSumY > rLimitY) )
	{
		pTS->_rDeltaY = 0.25 * ( -_rY );
		pTS->_rDeltaX = 0.25 * ( -_rX );
		return;
	}
	else
	{
		_nRangeStatus = IN_RANGE;
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitFromPrevious
//	Object:		
//	04-07-27:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::InitFromPrevious(TARGET_TRACK *	pPrevTK)
{
	_rAccX	= pPrevTK->_rAccX;
	_rAccY	= pPrevTK->_rAccY;
	_rAccZ	= pPrevTK->_rAccZ;
	_rVx	= pPrevTK->_rVx;
	_rVy	= pPrevTK->_rVy;
	_rVz 	= pPrevTK->_rVz ;
	_rX		= pPrevTK->_rX;
	_rY		= pPrevTK->_rY;
	_rZ		= pPrevTK->_rZ;
}


//-----------------------------------------------------------------------------
//	Name:		ComputeAbsoluteDelta
//	Object:		
//	04-11-04:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_TRACK::ComputeAbsoluteDelta(TARGET_ANIM_STRUCT *pT,
										   TARGET_STRUCT *	pTS,
										   const VECTOR &	TargetPoint)
{
	MATRIX		Mat;
	MATRIX		InvMat;
	VECTOR		BonePos;
	VECTOR		V;
	VECTOR		LocV;
	MATRIX		Mat2;
	QUATERNION	Q;
	QUATERNION	Q2;
	float		rX, rY, rZ;


	//	Global Mat;
	Mat = pT->GetGlobalMatrix( _nTrackIndex );
	//	Bone Pos
	BonePos = Mat.GetTranslation();
	//	Unit Vector fromp Bone to Target
	V.SetFromPoints(BonePos, TargetPoint);
	//	Set vector in root coordinates
	GetRootGlobalMat( pT, Mat2);
	InvMat.InverseAffine( Mat2 );
	LocV = InvMat.ApplyRotation( V );
	LocV.Normalize();

	//	Get this mat in root coord
	GetMatInRootCoord( pT, Mat );
	Mat.GetQuaternion( Q );
	GetAngleXYZFromQuat( Q, rX, rY, rZ );

	//	Resulting Quat
	Q2.SetRotation( VECTOR::UNIT_Z, LocV );
	//	Angle X, Y, Z
	GetAngleXYZFromQuat( Q2, pTS->_rAbsX, pTS->_rAbsY, pTS->_rAbsZ );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
