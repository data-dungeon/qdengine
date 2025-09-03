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
//	CLASS:	FRUSTRUM
//
//	01-07-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FRUSTRUM constructor
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
FRUSTRUM::FRUSTRUM()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		FRUSTRUM constructor
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
FRUSTRUM::FRUSTRUM(const FRUSTRUM &	Frustrum)
:
_Origin			(Frustrum._Origin),
_LeftDirection	(Frustrum._LeftDirection),
_TopDirection	(Frustrum._TopDirection),
_FrontDirection	(Frustrum._FrontDirection),
_NormalTop		(Frustrum._NormalTop),
_NormalBottom	(Frustrum._NormalBottom),
_NormalLeft		(Frustrum._NormalLeft),
_NormalRight	(Frustrum._NormalRight),
_rNear			(Frustrum._rNear),
_rFar			(Frustrum._rFar),
_rLeft			(Frustrum._rLeft),
_rTop			(Frustrum._rTop)
{
	ExtractClippingPlanes();
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM constructor
//	Object: Build a frustrum for a camera.
//  WARNING!!!! The construction assumes that the default camera position
//				is (0, 0, 0), looking down to (0, 0, -1).
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
FRUSTRUM::FRUSTRUM(float	rFocaleAngle,
				   float	rNearDistance,
				   float	rFarDistance,
				   float	rAspectRatio,
				   float	fScale)
{
	Set( rFocaleAngle, rNearDistance, rFarDistance, rAspectRatio, fScale);
}

//-----------------------------------------------------------------------------
//	Name:		~FRUSTRUM destructor
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
FRUSTRUM::~FRUSTRUM()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetOrigin
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR& FRUSTRUM::GetOrigin() const
{
	return ( _Origin );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetFrontDirection
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR& FRUSTRUM::GetFrontDirection() const
{
	return ( _FrontDirection );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetLeftDirection
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR& FRUSTRUM::GetLeftDirection() const
{
	return ( _LeftDirection );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetTopDirection
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR& FRUSTRUM::GetTopDirection() const
{
	return ( _TopDirection );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetNear
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
float FRUSTRUM::GetNear() const
{
	return ( _rNear );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetFar
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
float FRUSTRUM::GetFar() const
{
	return ( _rFar );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetLeft
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
float FRUSTRUM::GetLeft() const
{
	return ( _rLeft );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetTop
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
float FRUSTRUM::GetTop() const
{
	return ( _rTop );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetNormalTop
//	Object: 
//	01-06-15:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR & FRUSTRUM::GetNormalTop() const
{
	return ( _NormalTop );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetNormalBottom
//	Object: 
//	01-06-15:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR & FRUSTRUM::GetNormalBottom() const
{
	return ( _NormalBottom );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetNormalLeft
//	Object: 
//	01-06-15:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR & FRUSTRUM::GetNormalLeft() const
{
	return ( _NormalLeft );
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::GetNormalRight
//	Object: 
//	01-06-15:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR & FRUSTRUM::GetNormalRight() const
{
	return ( _NormalRight );
}

//-----------------------------------------------------------------------------
//	Name:		SetLeft
//	Object:		
//	02-11-07:	ELE - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetLeft(float rLeft)
{
	_rLeft = rLeft;
}

//-----------------------------------------------------------------------------
//	Name:		SetTop
//	Object:		
//	02-11-07:	ELE - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetTop(float rTop)
{
	_rTop = rTop;
}

//-----------------------------------------------------------------------------
//	Name:		GetPlane
//	Object:		
//	02-01-03:	JLT - Created
//-----------------------------------------------------------------------------
PLANE FRUSTRUM::GetClippingPlane(const CLIPPING_PLANES	index) const
{
	QDT_ASSERT(index < PLANES_COUNT);
	return (_ClippingPlanes[index]);
}

//-----------------------------------------------------------------------------
//	Name:		GetClippingPlanesCount
//	Object:		
//	02-01-03:	JLT - Created
//-----------------------------------------------------------------------------
int	FRUSTRUM::GetClippingPlanesCount() const
{
	return ((int) PLANES_COUNT);
}

//-----------------------------------------------------------------------------
//	Name:		GetHexahedron
//	Object:		
//	03-07-01:	ELE - Created
//-----------------------------------------------------------------------------
const HEXAHEDRON &	FRUSTRUM::GetHexahedron() const
{
	return	(_Hexahedron);
}

//-----------------------------------------------------------------------------
//	Name:		SetLeftDirection
//	Object:		
//	03-11-07:	FPR - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetLeftDirection(const VECTOR&	vLeftDir)
{
	_LeftDirection = vLeftDir;
}

//-----------------------------------------------------------------------------
//	Name:		SetTopDirection
//	Object:		
//	03-11-07:	FPR - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetTopDirection(const VECTOR&	vTopDir)
{
	_TopDirection = vTopDir;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrontDirection
//	Object:		
//	03-11-07:	FPR - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetFrontDirection(const VECTOR&	vFrontDir)
{
	_FrontDirection = vFrontDir;
}

//-----------------------------------------------------------------------------
//	Name:		SetFar
//	Object:		
//	03-11-18:	RMA - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetFar(float	rFar)
{
	if (rFar != _rFar)
	{
		_rFar = rFar;
		ExtractFarClippingPlane();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetNearFar
//	Object:		
//	03-11-18:	RMA - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::SetNear(float	rNear)
{
	if (_rNear != rNear)
	{
		_rNear = rNear;
		ExtractNearClippingPlane();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
