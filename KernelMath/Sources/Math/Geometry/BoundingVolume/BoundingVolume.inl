

//#include INCL_KCORE(Math/Geometry/Shapes/Shapes0D/Point3D/Point3D)
//#include INCL_KCORE(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	02-04-08:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ BOUNDING_VOLUME::TYPE BOUNDING_VOLUME::GetType() const
{
	return TYPE_BV;
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-04-15:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool BOUNDING_VOLUME::Intersect(const POINT3D &PIn, const VECTOR &VIn, POINT3D &POut, VECTOR &VOut, const float Inertia) const
{
	POut = PIn;
	VOut = VIn;
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		operator<
//	Object:		
//	02-05-24:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool BOUNDING_VOLUME::operator<(const BOUNDING_VOLUME &BV) const
{
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		Empty
//	Object:		
//	02-07-01:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_VOLUME::Empty()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		Empty
//	Object:		
//	02-07-01:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool BOUNDING_VOLUME::Grow(const POINT3D& p)
{
	return false;	
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxSize
//	Object:		
//	02-07-17:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ float BOUNDING_VOLUME::GetRadius() const
{
	return 0.f;
}


