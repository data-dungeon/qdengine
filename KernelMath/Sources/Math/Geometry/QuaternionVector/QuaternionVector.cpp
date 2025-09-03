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
//	CLASS:	QUATERNION_VECTOR
//
//	02-02-13:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include 	INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)

#ifdef _DEBUG
#include	"QuaternionVector.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		QUATERNION_VECTOR constructor
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR::QUATERNION_VECTOR()
{
	Id();
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION_VECTOR constructor
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR::QUATERNION_VECTOR(const QUATERNION_VECTOR & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION_VECTOR destructor
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR::~QUATERNION_VECTOR()
{

}

//-----------------------------------------------------------------------------
//	Name:		Inverse
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::Inverse()
{
	QUATERNION	Q;
	VECTOR		V;

	////////////////////////
	Q = _Quat;
	Q.Inverse();
	////////////////////////
	V = Q * _Vect;
	V.Neg();
	////////////////////////
	_Quat = Q;
	_Vect = V;
	////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		operator*=
//	Object:		
//	02-10-24:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::operator*=(const QUATERNION_VECTOR &	QV)
{
	QUATERNION_VECTOR	QV1;

	QV1 = (*this) * QV;

	Copy( QV1 );
}

//-----------------------------------------------------------------------------
//	Name:		InterSLERPId
//	Object:		
//	02-09-17:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::InterSLERPId(float						rVal,
										const QUATERNION_VECTOR &	QV)
{
	VECTOR	V;

	////////////////////////////////////////////
	V = QV._Vect;
	////////////////////////////////////////////
	_Quat.InterSLERPId( rVal, QV._Quat );
	_Vect.InterLinearId( rVal, V );
	////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		operator/
//	Object:		Return Delta so that  *this = QV * Delta
//	02-02-14:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR	QUATERNION_VECTOR::operator/(const QUATERNION_VECTOR &	QV)
{
	QUATERNION_VECTOR	QVTemp;
	QUATERNION_VECTOR	QRes;

	/////////////////////
	QVTemp = QV;
	QVTemp.Inverse();
	/////////////////////
	QRes = QVTemp *  ( *this );
	/////////////////////	
	return ( QRes );
	/////////////////////	
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterSLERP
//	Object:		
//	02-09-23:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::BasicInterSLERP(float						rVal,
										   const QUATERNION_VECTOR &	QV1,
										   const QUATERNION_VECTOR &	QV2)
{
	VECTOR	V1, V2;

	////////////////////////////////////////////
	V1 = QV1._Vect;
	V2 = QV2._Vect;
	////////////////////////////////////////////
	_Quat.BasicInterSLERP( rVal, QV1._Quat, QV2._Quat );
	_Vect.InterLinear( rVal, V1, V2 );
	////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterSLERPId
//	Object:		
//	02-09-23:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::BasicInterSLERPId(float						rVal,
											 const QUATERNION_VECTOR &	QV)
{
	VECTOR	V;

	////////////////////////////////////////////
	V = QV._Vect;
	////////////////////////////////////////////
	_Quat.BasicInterSLERPId( rVal, QV._Quat );
	_Vect.InterLinearId( rVal, V );
	////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		InterSLERP
//	Object:		
//	02-02-14:	cvi - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::InterSLERP(float						rVal,
									  const QUATERNION_VECTOR &	QV1,
									  const QUATERNION_VECTOR &	QV2)
{
	VECTOR	V1, V2;

	////////////////////////////////////////////
	V1 = QV1._Vect;
	V2 = QV2._Vect;
	////////////////////////////////////////////
	_Quat.InterSLERP( rVal, QV1._Quat, QV2._Quat );
	_Vect.InterLinear( rVal, V1, V2 );
	////////////////////////////////////////////
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
