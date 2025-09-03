#include INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)

//-----------------------------------------------------------------------------
//	Name:		GetEffectiveRadius
//	Object:		
//	04-04-15:	RMA - Created
//-----------------------------------------------------------------------------
float	AABB::GetEffectiveRadius(const PLANE &Plane) const
{
	VECTOR AbsoluteNormal(Plane.GetNormal());
	AbsoluteNormal.Absolute();
	return (_Size.Dot(AbsoluteNormal) * 0.5f);
}

//-----------------------------------------------------------------------------
//	Name:		IsOutsideOf
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	AABB::IsOutsideOf(const PLANE &	Plane) const
{
	QDT_ASSERT(!Plane.GetNormal().IsNull());
	QDT_ASSERT(Plane.GetNormal().IsNormalized());

	float rProj = Plane.GetNormal().Dot(_Center) + Plane.GetD();
	float rEff  = GetEffectiveRadius(Plane);

	return (rProj - rEff >= 0.0f);
}

