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
// FILE: Plane.inl
//
//
//	01-04-02:	RMA - Created
//*****************************************************************************


//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	03-03-10:	RMA - Created
//-----------------------------------------------------------------------------
bool	PLANE::operator == (const PLANE &	P) const
{
	return (_vNormal == P._vNormal && MATH::EqualsEpsilon(GetD(), P.GetD(), 0.01f));
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	03-03-13:	RMA - Created
//-----------------------------------------------------------------------------
bool	PLANE::operator!=(const PLANE &	P) const
{
	return (_vNormal != P._vNormal || !MATH::EqualsEpsilon(GetD(), P.GetD(), 0.01f));
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance
//	Object:		
//	03-03-10:	RMA - Created
//-----------------------------------------------------------------------------
float	PLANE::GetSignedDistance(const VECTOR &	Pos) const
{
	QDT_ASSERT(IsNormalNormalized());
	return (Pos.Dot(_vNormal) + GetD());
}

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::Copy(const PLANE &	Plane)
{
	_vNormal.Copy(Plane._vNormal);
	SetD(Plane.GetD());
}

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::SetCoeff(float rA,
					    float rB,
					    float rC,
					    float rD)
{ 
	QDT_ASSERT((MATH::Abs(rA) + MATH::Abs(rB) + MATH::Abs(rC)) >= 0.0001f);
	_vNormal.SetXYZ( rA, rB, rC );
	SetD(rD);
}

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-16:	RPA - Created
//  01-02-14:	GGO - Removed Normal normalization, added QDT_ASSERT.
//-----------------------------------------------------------------------------
void	PLANE::SetFromPointAndNormal(const VECTOR &	Pt,
									 const VECTOR &		Normal)
{
	_vNormal.Copy(Normal);
	SetD( -(Normal.GetX() * Pt.GetX()
		   +Normal.GetY() * Pt.GetY()
		   +Normal.GetZ() * Pt.GetZ()));
}

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::SetFromTriangle(const TRIANGLE &	Triangle)
{
	SetFromPoints( Triangle.GetPoint0() , Triangle.GetPoint1() , Triangle.GetPoint2() );
}

//-----------------------------------------------------------------------------
//	Name:		Flip
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::Flip()
{
	_vNormal.Neg();
	SetD(-GetD());
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		Returns the closest point of the plane to the origin.
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
VECTOR	PLANE::GetPoint(bool	bNormalizedNormal) const
{
	VECTOR Point( -_vNormal.GetX() * GetD(), -_vNormal.GetY() * GetD(), -_vNormal.GetZ() * GetD());

	if ( bNormalizedNormal == false )
	{
		Point /= _vNormal.Length();
	}

	return (Point);
}

//-----------------------------------------------------------------------------
//	Name:		GetNormal
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::GetNormal( VECTOR & Normal ) const
{
	Normal.Copy( _vNormal );
}

//-----------------------------------------------------------------------------
//	Name:		GetNormal
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	PLANE::GetNormal() const
{
	return ( _vNormal );
}

//-----------------------------------------------------------------------------
//	Name:		GetCoeff
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::GetCoeff(float &	rA,
						float &	rB,
						float &	rC,
						float &	rD) const
{
	rA = _vNormal.GetX();
	rB = _vNormal.GetY();
	rC = _vNormal.GetZ();
	rD = GetD();
}

//-----------------------------------------------------------------------------
//	Name:		NormalizeNormal
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
void	PLANE::NormalizeNormal()
{
	if ( _vNormal.IsNull() == true )
	{
		return;
	}
	float rInvNorm = 1.0f / _vNormal.Length();

	_vNormal	*= rInvNorm;
	SetD(GetD() * rInvNorm);
}

//-----------------------------------------------------------------------------
//	Name:		NormalizePlane
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
void	PLANE::NormalizePlane()
{
	float rCoeff;

	rCoeff =  1.0f / _vNormal.Length();

	_vNormal	*= rCoeff;
	SetD(GetD() * rCoeff);	
}

//-----------------------------------------------------------------------------
//	Name:		IsNormalNormalized
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
bool	PLANE::IsNormalNormalized() const
{
	return ( _vNormal.IsNormalized() == true );
}

//-----------------------------------------------------------------------------
//	Name:		IsPlaneNormalized
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
bool	PLANE::IsPlaneNormalized() const
{
	return ( ( IsNormalNormalized() == true ) && ( GetD() >= 0.0f ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetConstant
//	Object:		
//	02-12-04:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ float PLANE::GetConstant() const
{
	return (_vNormal.GetPad());
}

//-----------------------------------------------------------------------------
//	Name:		SetD
//	Object:		
//	03-05-23:	RMA - Created
//-----------------------------------------------------------------------------
void	PLANE::SetD(float	r)
{
	_vNormal.SetPad(r);
}

//-----------------------------------------------------------------------------
//	Name:		SetD
//	Object:		
//	03-06-13:	SBE - Created
//-----------------------------------------------------------------------------
float	PLANE::GetD(void) const
{
	return (_vNormal.GetPad());
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-08-12:	RMA - Created
//-----------------------------------------------------------------------------
void	PLANE::operator=(const PLANE &	P)
{
	_vNormal = P.GetNormal();
	SetD(P.GetD());
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-11-28:	SBE - Created
//-----------------------------------------------------------------------------
void	PLANE::operator=(const POINT4D &	P)
{
	_vNormal.SetXYZ(P.GetX(), P.GetY(), P.GetZ());
	_vNormal.SetPad(P.GetW());
}

//-----------------------------------------------------------------------------
//	Name:		SetNormal
//	Object:		
//	05-11-28:	SBE - Created
//-----------------------------------------------------------------------------
void	PLANE::SetNormal(const VECTOR & V)
{
	_vNormal = V;
}

//-----------------------------------------------------------------------------
//	Name:		SetNormal
//	Object:		
//	06-02-24:	SBE - Created
//-----------------------------------------------------------------------------
void	PLANE::FlipNormal()
{
	_vNormal = -_vNormal;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
