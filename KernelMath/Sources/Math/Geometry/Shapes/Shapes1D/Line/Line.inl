//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

//-----------------------------------------------------------------------------
// Name: LINE constructor
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
LINE::LINE()
{
}


//-----------------------------------------------------------------------------
// Name: LINE constructor
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
LINE::LINE(const LINE &Line)
:
_Point ( Line._Point ),
_Direction ( Line._Direction )
{	
}

//-----------------------------------------------------------------------------
// Name: LINE constructor
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
LINE::LINE(const VECTOR &	LinePt, 
		   const VECTOR &		LineDir)
:
_Point ( LinePt ),
_Direction ( LineDir )
{
}

//-----------------------------------------------------------------------------
// Name: LINE constructor
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
LINE::LINE(const SEGMENT &Seg)
{
	SetFromSegment( Seg );	
}

//-----------------------------------------------------------------------------
// Name: LINE constructor
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
LINE::LINE(const RAY &Ray)
{
	SetFromRay( Ray );	
}

//-----------------------------------------------------------------------------
// Name: LINE::Build
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::Set(const LINE &Line)
{
	_Point = Line._Point;
	_Direction.Copy( Line._Direction );
}

//-----------------------------------------------------------------------------
// Name: LINE::Build
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::Set(const VECTOR &	LinePt, 
				  const VECTOR &	LineDir)
{
	_Point = LinePt;
	_Direction.Normalize( LineDir );
}

//-----------------------------------------------------------------------------
// Name: LINE::Build
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::SetFromSegment(const SEGMENT & Seg)
{
	_Point = Seg.GetPoint1();
	_Direction = Seg.GetPoint2() - _Point;
	_Direction.Normalize();
}

//-----------------------------------------------------------------------------
// Name: LINE::Build
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::SetFromRay(const RAY &Ray)
{
	_Point = Ray.GetOrigin();
	_Direction.Copy( Ray.GetDirection() );
}

//-----------------------------------------------------------------------------
// Name: LINE::Transform
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::Transform(const LINE &Line, const MATRIX &Matrix)
{
	Matrix.TransformPosition(Line._Point, _Point);
	Matrix.TransformDirection(Line._Direction, _Direction);
}

//-----------------------------------------------------------------------------
// Name: LINE::Transform
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::Transform(const MATRIX &Matrix)
{
	Matrix.TransformPosition(_Point);
	Matrix.TransformDirection(_Direction);
}

//-----------------------------------------------------------------------------
// Name: LINE::GetPoint
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::GetPoint(VECTOR &p)const
{
	p = _Point;
}

//-----------------------------------------------------------------------------
// Name: LINE::GetDirection
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	LINE::GetDirection(VECTOR &v)const
{
	v.Copy(_Direction);
}

//-----------------------------------------------------------------------------
//	Name: LINE::GetDirection
//	Object: Returns the direction of the line.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	LINE::GetDirection() const
{
	return ( _Direction );
}

//-----------------------------------------------------------------------------
//	Name: LINE::GetPoint
//	Object: Return the point of the line.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	LINE::GetPoint() const
{
	return ( _Point );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
