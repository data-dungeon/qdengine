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
//	CLASS:	BOUNDING_BOX
//
//	02-05-29:	JLT - Created
//*****************************************************************************
/*
//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::Compute(const POINT3D& Min, const POINT3D& Max) 
{
	_HalfDiag = (Max - Min) * 0.5f;
	_Center = Min + _HalfDiag;
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::Grow(const BOUNDING_BOX &a, const BOUNDING_BOX &b)
{
	POINT3D L(	min(a.GetLower(_X_), b.GetLower(_X_)),
				min(a.GetLower(_Y_), b.GetLower(_Y_)), 
				min(a.GetLower(_Z_), b.GetLower(_Z_)));

	POINT3D U(	max(a.GetUpper(_X_), b.GetUpper(_X_)), 
				max(a.GetUpper(_Y_), b.GetUpper(_Y_)),	
				max(a.GetUpper(_Z_), b.GetUpper(_Z_)));
	Compute(L, U);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::Clear()
{ 
	_Center.SetXYZ(0, 0, 0); 
	_HalfDiag.SetXYZ(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool BOUNDING_BOX::Grow(const POINT3D &p)
{
	POINT3D L(min(GetLower(_X_), p[_X_]), min(GetLower(_Y_), p[_Y_]), min(GetLower(_Z_), p[_Z_]));
	POINT3D U(max(GetUpper(_X_), p[_X_]), max(GetUpper(_Y_), p[_Y_]), max(GetUpper(_Z_), p[_Z_]));
	Compute(L, U);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::Grow(const BOUNDING_BOX& b) 
{ 
	Grow(*this, b); 
}

//-----------------------------------------------------------------------------
//	Name:		GetLower
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ float BOUNDING_BOX::GetLower(int i) const 
{ 
	return _Center[i] - _HalfDiag[i]; 
}

//-----------------------------------------------------------------------------
//	Name:		GetUpper
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ float BOUNDING_BOX::GetUpper(int	i) const
{ 
	return _Center[i] + _HalfDiag[i]; 
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ float BOUNDING_BOX::GetRadius() const 
{ 
	return max(max(_HalfDiag[_X_], _HalfDiag[_Y_]), _HalfDiag[_Z_]); 
}

//-----------------------------------------------------------------------------
//	Name:		GetLongestAxis
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ int BOUNDING_BOX::GetLongestAxis() const 
{ 
	return _HalfDiag.GetClosestAxis(); 
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool BOUNDING_BOX::Intersect(const BOUNDING_BOX &a, const BOUNDING_BOX &b)
{
	bool Result;

	Result = MATH::Abs(a._Center[_X_] - b._Center[_X_]) <= (a._HalfDiag[_X_] + b._HalfDiag[_X_]) &&
			 MATH::Abs(a._Center[_Y_] - b._Center[_Y_]) <= (a._HalfDiag[_Y_] + b._HalfDiag[_Y_]) &&
			 MATH::Abs(a._Center[_Z_] - b._Center[_Z_]) <= (a._HalfDiag[_Z_] + b._HalfDiag[_Z_]);
	
	return Result;
}

//-----------------------------------------------------------------------------
//	Name:		SetCenter
//	Object:		
//	02-05-30:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::SetCenter(const POINT3D &p)
{
	_Center = p;
}

//-----------------------------------------------------------------------------
//	Name:		GetHalfDiag
//	Object:		
//	02-05-30:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR BOUNDING_BOX::GetHalfDiag() const
{
	return _HalfDiag;
}

//-----------------------------------------------------------------------------
//	Name:		SetHalfDiag
//	Object:		
//	02-05-30:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::SetHalfDiag(const VECTOR &V)
{
	_HalfDiag = V;
}

//-----------------------------------------------------------------------------
//	Name:		Empty
//	Object:		
//	02-07-01:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void BOUNDING_BOX::Empty()
{
	Clear();
}*/


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
