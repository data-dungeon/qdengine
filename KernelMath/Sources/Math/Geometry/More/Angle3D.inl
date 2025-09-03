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
// CLASS: ANGLE3D
//
//
//	01-01-24:	ONA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ANGLE3D::ANGLE3D
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
ANGLE3D::ANGLE3D()
{

}


//-----------------------------------------------------------------------------
// Name: ANGLE3D::ANGLE3D
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
ANGLE3D::ANGLE3D( const ANGLE3D &Angle3D):
_rAlpha ( Angle3D._rAlpha ),
_rBeta ( Angle3D._rBeta ),
_rGamma ( Angle3D._rGamma )
{
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::ANGLE3D
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
ANGLE3D::ANGLE3D( const float rAngleAlpha, 
				  const float rAngleBeta, 
				  const float rAngleGamma):
_rAlpha ( rAngleAlpha ),
_rBeta ( rAngleBeta ),
_rGamma ( rAngleGamma )
{
}


//-----------------------------------------------------------------------------
// Name: ANGLE3D::operator
// Object: 
//01-01-15 RMA Created: 
//-----------------------------------------------------------------------------
ANGLE3D& ANGLE3D::operator = (const ANGLE3D& Angle3D)
{
	_rAlpha = Angle3D._rAlpha ;
	_rBeta	= Angle3D._rBeta ;
	_rGamma	= Angle3D._rGamma ;

	return (*this) ;
}


//-----------------------------------------------------------------------------
//	Name: ANGLE3D::operator 
//	Object: 
//	01-06-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	ANGLE3D::operator ==(const ANGLE3D& Angle3D) const
{
	return ( ( MATH::EqualsEpsilon( _rAlpha, Angle3D._rAlpha ) == true ) &&
			 ( MATH::EqualsEpsilon( _rBeta,  Angle3D._rBeta )  == true ) &&
			 ( MATH::EqualsEpsilon( _rGamma, Angle3D._rGamma ) == true ) );
}


//-----------------------------------------------------------------------------
//	Name: ANGLE3D::operator 
//	Object: 
//	01-06-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	ANGLE3D::operator !=(const ANGLE3D& Angle3D) const
{
	return ( ( MATH::EqualsEpsilon( _rAlpha, Angle3D._rAlpha ) == false ) ||
			 ( MATH::EqualsEpsilon( _rBeta,  Angle3D._rBeta )  == false ) ||
			 ( MATH::EqualsEpsilon( _rGamma, Angle3D._rGamma ) == false ) );
}


//-----------------------------------------------------------------------------
// Name: ANGLE3D::Set
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	ANGLE3D::Set( const ANGLE3D &Angle3D)
{
	_rAlpha = Angle3D._rAlpha;
	_rBeta  = Angle3D._rBeta;
	_rGamma = Angle3D._rGamma;
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::Set
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	ANGLE3D::Set( const float rAngleAlpha, 
					  const float rAngleBeta, 
					  const float rAngleGamma)
{
	_rAlpha = rAngleAlpha;
	_rBeta  = rAngleBeta;
	_rGamma = rAngleGamma;
}



//-----------------------------------------------------------------------------
// Name: ANGLE3D::GetAngles
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void ANGLE3D::GetAngles(float &a, float &b, float &g) const 
{
	a = _rAlpha;
	b = _rBeta;
	g = _rGamma;
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::GetAlpha
// Object: 
//01-01-02 RMA Created: 
//-----------------------------------------------------------------------------
float	ANGLE3D::GetAlpha() const
{
	return (_rAlpha) ;
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::GetBeta
// Object: 
//01-01-02 RMA Created: 
//-----------------------------------------------------------------------------
float	ANGLE3D::GetBeta() const
{
	return (_rBeta) ;
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::GetGamma
// Object: 
//01-01-02 RMA Created: 
//-----------------------------------------------------------------------------
float	ANGLE3D::GetGamma() const
{
	return (_rGamma) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetAlphaRef
//	Object:		
//	06-03-17:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	ANGLE3D::GetAlphaRef() const
{
	return (_rAlpha) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetBetaRef
//	Object:		
//	06-03-17:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	ANGLE3D::GetBetaRef() const
{
	return (_rBeta) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetGammaRef
//	Object:		
//	06-03-17:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	ANGLE3D::GetGammaRef() const
{
	return (_rGamma) ;
}


//-----------------------------------------------------------------------------
// Name: ANGLE3D::SetAlpha
// Object: 
//01-01-02 RMA Created: 
//-----------------------------------------------------------------------------
void	ANGLE3D::SetAlpha(const float &rAlpha)
{
	_rAlpha = rAlpha ;
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::SetBeta
// Object: 
//01-01-02 RMA Created: 
//-----------------------------------------------------------------------------
void	ANGLE3D::SetBeta(const float &rBeta)
{
	_rBeta = rBeta ;
}

//-----------------------------------------------------------------------------
// Name: ANGLE3D::SetGamma
// Object: 
//01-01-02 RMA Created:
//-----------------------------------------------------------------------------
void	ANGLE3D::SetGamma(const float &rGamma)
{
	_rGamma = rGamma ;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
