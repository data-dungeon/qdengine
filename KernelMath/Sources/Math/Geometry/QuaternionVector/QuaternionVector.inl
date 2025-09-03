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

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR &	QUATERNION_VECTOR::operator=(const QUATERNION_VECTOR & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::Copy(const QUATERNION_VECTOR & C)
{
	_Quat = C._Quat;
	_Vect = C._Vect;
}

//-----------------------------------------------------------------------------
//	Name:		SetQuat
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::SetQuat(const QUATERNION &	Quat)
{
	_Quat = Quat;

}

//-----------------------------------------------------------------------------
//	Name:		GetQuat
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
const QUATERNION &	QUATERNION_VECTOR::GetQuat() const
{
	return ( _Quat );
}

//-----------------------------------------------------------------------------
//	Name:		SetVector
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::SetVector(const VECTOR &	V)
{
	_Vect = V;
}

//-----------------------------------------------------------------------------
//	Name:		GetVector
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
const VECTOR &	QUATERNION_VECTOR::GetVector() const
{
	return ( _Vect );
}

//-----------------------------------------------------------------------------
//	Name:		Id
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::Id()
{
	_Quat.Id();
	_Vect = VECTOR::ZERO;
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR	QUATERNION_VECTOR::operator*(const QUATERNION_VECTOR &	QV) const
{
	QUATERNION_VECTOR	QVR;

	//////////////////////////////
	QVR._Quat	=	_Quat * QV._Quat;
	QVR._Vect	=	_Quat * QV._Vect;
	QVR._Vect	+=	_Vect;
	//////////////////////////////
	return ( QVR );
	//////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
VECTOR	QUATERNION_VECTOR::operator*(const VECTOR &	V) const
{
	VECTOR		VR;

	/////////////////////
	VR =	_Quat * V;
	VR +=	_Vect;
	return ( VR );
	/////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION_VECTOR::operator*(const QUATERNION &	Q) const
{
	QUATERNION		QR;

	///////////////
	QR = _Quat * Q;
	return ( QR );
	///////////////
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	02-02-13:	cvi - Created
//-----------------------------------------------------------------------------
QUATERNION_VECTOR	QUATERNION_VECTOR::operator*(float	rVal) const
{
	QUATERNION_VECTOR		QV;
	VECTOR					V;

	/////////////////////////////
	V = _Vect;
	QV._Quat.InterSLERPId( rVal, _Quat );
	QV._Vect.InterLinearId( rVal, V );
	return ( QV );
	/////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		Normalize
//	Object:		
//	02-09-23:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION_VECTOR::Normalize()
{
	_Quat.Normalize();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
