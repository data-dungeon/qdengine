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
//	CLASS:	CONE
//
//	01-06-22:	RMA - Created
//*****************************************************************************

#include INCL_KMATH(Math/Math)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CONE constructor
//	Object:		Default constructor
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
CONE::CONE()
:
_Top(0.0f, 0.0f, 0.0f),
_Axe(0.0f, 0.0f, 0.0f),
_rRadius(-1.0f),
_rAngle(0.0f),
_rCosAngle(1.0f),
_rSinAngle(0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CONE constructor
//	Object:		Copy constructor
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
CONE::CONE(const CONE & Cone)
:
_Top		(Cone._Top),
_Axe		(Cone._Axe),
_rRadius	(Cone._rRadius),
_rAngle		(Cone._rAngle),
_rCosAngle	(Cone._rCosAngle),
_rSinAngle	(Cone._rSinAngle)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CONE constructor
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
CONE::CONE(const VECTOR &	Top,
		   const VECTOR &	Axe,
		   const float		rAngle,
		   const float		rRadius)
:
_Top		(Top),
_Axe		(Axe),
_rRadius	(rRadius),
_rAngle		(rAngle)
{
	QDT_ASSERT(Axe.IsNormalized());

	MATH::GetCosinusAndSinus(_rAngle, _rCosAngle, _rSinAngle);
}

//-----------------------------------------------------------------------------
//	Name:		~CONE destructor
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
CONE::~CONE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetTop
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR &	CONE::GetTop() const
{
	return (_Top);
}

//-----------------------------------------------------------------------------
//	Name:		GetAxe
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR &	CONE::GetAxe() const
{
	return (_Axe);
}

//-----------------------------------------------------------------------------
//	Name:		GetRadius
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const float	CONE::GetRadius() const
{
	return (_rRadius);
}


//-----------------------------------------------------------------------------
//	Name:		GetAngle
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const float	CONE::GetAngle() const
{
	return (_rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetSinAngle
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const float	CONE::GetSinAngle() const
{
	return (_rSinAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetCosAngle
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const float	CONE::GetCosAngle() const
{
	return (_rCosAngle);
}

//-----------------------------------------------------------------------------
//	Name:		SetTop
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::SetTop(const VECTOR	& Top)
{
	_Top = Top;
}

//-----------------------------------------------------------------------------
//	Name:		SetAxe
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::SetAxe(const VECTOR & Axe)
{
	QDT_ASSERT(Axe.IsNormalized());
	_Axe = Axe;
}

//-----------------------------------------------------------------------------
//	Name:		SetRadius
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::SetRadius(const float rRadius)
{
	_rRadius = rRadius;
}

//-----------------------------------------------------------------------------
//	Name:		SetAngle
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::SetAngle(const float rAngle)
{
	_rAngle = rAngle;
	MATH::GetCosinusAndSinus(_rAngle, _rCosAngle, _rSinAngle);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
