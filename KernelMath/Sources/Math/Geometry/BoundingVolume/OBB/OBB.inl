#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

//-----------------------------------------------------------------------------
//	Name: OBB::GetCenter
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ POINT3D	OBB::GetCenter() const
{
	return ( _Center );
}

//-----------------------------------------------------------------------------
//	Name: OBB::GetDistance1
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ Float32 OBB::GetDistance1() const
{
	return ( _rDistance1 );
}

//-----------------------------------------------------------------------------
//	Name: OBB::GetDistance2
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ Float32 OBB::GetDistance2() const
{
	return ( _rDistance2 );
}

//-----------------------------------------------------------------------------
//	Name: OBB::GetDistance3
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ Float32 OBB::GetDistance3() const
{
	return ( _rDistance3 );
}

//-----------------------------------------------------------------------------
//	Name: OBB::GetDirection1
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR	OBB::GetDirection1() const
{
	return ( _Direction1 );
}

//-----------------------------------------------------------------------------
//	Name: OBB::GetDirection2
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR	OBB::GetDirection2() const
{
	return ( _Direction2 );
}

//-----------------------------------------------------------------------------
//	Name: OBB::GetDirection3
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR	OBB::GetDirection3() const
{
	return ( _Direction3 );
}

//-----------------------------------------------------------------------------
//	Name:		SetCenter
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetCenter(const POINT3D &	Center)
{
	_Center = Center;
}

//-----------------------------------------------------------------------------
//	Name:		SetDistance1
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetDistance1(Float32	rDistance1)
{
	_rDistance1 = rDistance1;
}

//-----------------------------------------------------------------------------
//	Name:		SetDistance2
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetDistance2(Float32	rDistance2)
{
	_rDistance2 = rDistance2;
}

//-----------------------------------------------------------------------------
//	Name:		SetDistance3
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetDistance3(Float32	rDistance3)
{
	_rDistance3 = rDistance3;
}

//-----------------------------------------------------------------------------
//	Name:		SetDirection1
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetDirection1(const VECTOR &	rDirection1)
{
	_Direction1 = rDirection1;
}

//-----------------------------------------------------------------------------
//	Name:		SetDirection2
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetDirection2(const VECTOR &	rDirection2)
{
	_Direction2 = rDirection2;
}

//-----------------------------------------------------------------------------
//	Name:		SetDirection3
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::SetDirection3(const VECTOR &	rDirection3)
{
	_Direction3 = rDirection3;
}

//-----------------------------------------------------------------------------
//	Name: OBB::Transform
//	Object: Transform an OBB, but I'm sure u've already guessed...
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
_INLINE_ void			OBB::Transform( const MATRIX & Matrix )
{
	QDT_ASSERT( Matrix.HasNoScale() == true );

	Matrix.Apply( _Direction1 );
	Matrix.Apply( _Direction2 );
	Matrix.Apply( _Direction3 );

	Matrix.Apply( _Center );
}

//-----------------------------------------------------------------------------
//	Name:		GetRandomPoint
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
_INLINE_ void	OBB::GetRandomPoint(POINT3D &	Point) const
{
	// We assume that _Direction1 is X, _Direction2 is Y and _Direction3 is Z

	Float32	rPosX = MATH::Rand(2.0f * _rDistance1) - _rDistance1;
	Float32	rPosY = MATH::Rand(2.0f * _rDistance2) - _rDistance2;
	Float32	rPosZ = MATH::Rand(2.0f * _rDistance3) - _rDistance3;

	Point.SetXYZ(rPosX, rPosY, rPosZ);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-04-16:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool OBB::Intersect(const POINT3D &PIn, const VECTOR &VIn, POINT3D &POut, VECTOR &VOut, const Float32 Inertia) const
{
	POut = PIn;
	VOut = VIn;
	return false;
}

