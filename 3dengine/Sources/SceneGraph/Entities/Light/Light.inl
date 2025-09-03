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
// FILE: Light.inl
//
//
//	01-02-09:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: LIGHT::Copy
//	Object: Copy method to avoid a code duplication in copy constructor &
//			operator =
//	01-02-09:	RMA - Created
//-----------------------------------------------------------------------------
void LIGHT::Copy(const LIGHT & Light)
{
	_nType = Light._nType;

	_Color = Light._Color ;

	_rHotSpotAngle = Light._rHotSpotAngle;
	_rFallOffAngle = Light._rFallOffAngle;

	_rHotSpotDistance = Light._rHotSpotDistance;
	_rFallOffDistance = Light._rFallOffDistance;

	_rMultiplier = Light._rMultiplier;

	_nFlags = Light._nFlags;
}

//-----------------------------------------------------------------------------
//	Name: LIGHT::GetFlags
//	Object: 
//	01-02-21:	RMA - Created
//-----------------------------------------------------------------------------
unsigned long LIGHT::GetFlags(unsigned long nFlagsMask) const
{
	return (_nFlags & nFlagsMask) ;
}

//-----------------------------------------------------------------------------
//	Name: LIGHT::SetFlags
//	Object: 
//	01-02-21:	RMA - Created
//-----------------------------------------------------------------------------
void LIGHT::SetFlags(unsigned long nFlagsMask, unsigned long nFlagsSet)
{
	_nFlags &= ~nFlagsMask ;
	_nFlags |=  nFlagsSet ;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::TYPE	
// Object: Get the type of the light
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
int LIGHT::GetLightType() const
{
	return (_nType) ;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetLightType
// Object: Set the type of the light
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void LIGHT::SetLightType(int nNewType)
{
	_nType = nNewType ;
}

//-----------------------------------------------------------------------------
// Name: GetColor
// Object: Get the color of the light
//00 - 12 - 21 RPA Created:
//-----------------------------------------------------------------------------
const COLOR_FLOAT & LIGHT::GetColor()const
{
	return (_Color) ;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetColor
// Object: Set the color of the light
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void LIGHT::SetColor(const COLOR_FLOAT & NewColor)
{
	_Color = NewColor ;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetHotSpotAngle
// Object: Get the hot spot angle
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
float	LIGHT::GetHotSpotAngle()const
{
	return ( _rHotSpotAngle );
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetHotSpotAngle
// Object: Set the hot spot angle
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	LIGHT::SetHotSpotAngle( float rNewAngle )
{
	_rHotSpotAngle = rNewAngle;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetFallOffAngle
// Object: Get the fall off angle
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
float	LIGHT::GetFallOffAngle()const
{
	return ( _rFallOffAngle );
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetFallOffAngle
// Object: Set the fall off angle
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	LIGHT::SetFallOffAngle( float rNewAngle )
{
	_rFallOffAngle = rNewAngle;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetHotSpotDistance
// Object: Get the hot spot distance 
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
float	LIGHT::GetHotSpotDistance()const
{
	return ( _rHotSpotDistance );
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetHotSpotDistance
// Object: Set the hot spot distance
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	LIGHT::SetHotSpotDistance( float rDistance )
{
	_rHotSpotDistance = rDistance;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetFallOffDistance
// Object: Get the fall off distance
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
float	LIGHT::GetFallOffDistance()const
{
	return ( _rFallOffDistance );
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetFallOffDistance
// Object: Set the fall off distance 
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	LIGHT::SetFallOffDistance( float rDistance )
{
	_rFallOffDistance = rDistance;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetMultiplier
// Object: Get the multiplier of the light
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
float	LIGHT::GetMultiplier()const
{
	return ( _rMultiplier );
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SetMultiplier
// Object: Set the mulitiplier of the light
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	LIGHT::SetMultiplier( float rNewMultiplier )
{
	_rMultiplier = rNewMultiplier;
}

//-----------------------------------------------------------------------------
//	Name:		GetShadowColor
//	Object:		Get the color of the shadow
//	01-06-11:	ELE - Created
//-----------------------------------------------------------------------------
const COLOR_FLOAT &	LIGHT::GetShadowColor() const
{
	return ( _ShadowColor ) ;
}

//-----------------------------------------------------------------------------
//	Name:		SetShadowColor
//	Object:		Set the color of the shadow
//	01-06-11:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT::SetShadowColor(const COLOR_FLOAT &	Color)
{
	_ShadowColor = Color ;
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetBoundingCone
// Object: Get the bounding cone of the light
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
CONE LIGHT::GetBoundingCone() const
{
	CONE	Cone ;
	POINT3D	Position(0.0f, 0.0f, 0.0f) ;
	VECTOR	Direction(1.0f, 0.0f, 0.0f) ;

	QDT_FAIL();

/*	switch (_nType)
	{
	case LIGHT::POINT_LIGHT :

		Cone.Build(Position , Direction , MATH::PI() , _rFallOffDistance );
		break ;

	case LIGHT::SPOT_LIGHT :
		Cone.Build(Position , Direction , _rFallOffAngle , _rFallOffDistance );
		break ;
	}*/

	return (Cone) ;
}

//-----------------------------------------------------------------------------
//	Name: LIGHT::GetBoundingSphere
//	Object: Get the bounding sphere 
//	01-02-13:	RMA - Created
//-----------------------------------------------------------------------------
SPHERE LIGHT::GetBoundingSphere() const
{
	SPHERE		BSphere ;
	POINT3D		Pos(0.0f, 0.0f, 0.0f) ;

	BSphere.Set(Pos , _rFallOffDistance) ;

	return (BSphere) ;
}

//-----------------------------------------------------------------------------
//	Name: LIGHT::IsLight
//	Object: Is a light?
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
bool LIGHT::IsLight() const
{
	return (true);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================