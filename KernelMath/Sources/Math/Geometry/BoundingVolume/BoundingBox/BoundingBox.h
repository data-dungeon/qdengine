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
//	The BOUNDING_BOX class implements ...
//
//	02-05-29:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
/*
class BOUNDING_BOX : public BOUNDING_VOLUME
{
private : 
	int		_nPadding[2];

	VECTOR _Center;
	VECTOR _HalfDiag;

public :

	BOUNDING_BOX();
	BOUNDING_BOX(const POINT3D &Min, const POINT3D &Max);
	BOUNDING_BOX(const BOUNDING_BOX & C);
	virtual ~BOUNDING_BOX();

	static	void*		_pVTable;

	BOUNDING_BOX &	operator=(const BOUNDING_BOX &C);
	virtual	BOUNDING_VOLUME & operator = (const BOUNDING_VOLUME &BoundingVolume);
	void Copy(const BOUNDING_BOX &C);
	

	virtual void		Empty();
	virtual bool		Grow (const POINT3D& p); 
#ifndef _MASTER_ONLY_DATABANK
	virtual void		Read(IO_STREAM &s);
#endif
#ifndef _MASTER
	virtual void		Write(IO_STREAM &s)const;
	virtual void		WriteBinary(IO_STREAM &s);
#endif
	virtual void		Display(const COLOR_FLOAT &Color) const;
	virtual float		GetRadius() const;

	virtual void			Transform(const MATRIX &Transform);
	virtual const VECTOR &	GetCenter() const;
	virtual float			GetSize() const;

	virtual bool		Intersect(const PLANE &Plane, bool &bSide) const;
	virtual bool		Intersect(const RAY &Ray, POINT3D & HitPoint) const;
	virtual bool		Intersect(const RAY &Ray, float &rDistance) const;
	virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
	virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
	virtual	bool	IntersectFront(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
	virtual bool	IntersectFront(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
	virtual bool		Intersect(const FRUSTRUM &Frustrum) const;
	virtual bool		Intersect(const CONE &Cone) const;
	virtual bool		Intersect(const SPHERE &Sphere) const;
	virtual bool		Intersect(const AABB &Aabb) const;
	virtual bool		Intersect(const POINT3D &Point) const;
	virtual bool		Intersect(VERTICAL_RAY & VerticalRay) const;
	
	void SetCenter(const POINT3D& p);
	VECTOR GetHalfDiag() const;
	void SetHalfDiag(const VECTOR& V);
	void Compute(const POINT3D& Min, const POINT3D& Max);
	void Grow(const BOUNDING_BOX& a, const BOUNDING_BOX& b);
	void Clear();
	void Grow (const BOUNDING_BOX& b);
	float GetLower(int i) const;
	float GetUpper(int i) const;
	
	int GetLongestAxis() const;
	static bool Intersect(const BOUNDING_BOX& a, const BOUNDING_BOX& b);

	// For shadow maps
	virtual float		GetOptimalFocalAngle(const POINT3D & Pos, float rAngle, float & rNear, float & rIntersectNear, const VECTOR & FrontDir) const;
};*/

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
