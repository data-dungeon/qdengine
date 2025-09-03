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
//	CLASS:	INPUT_DEVICE_INFO
//
//	03-01-09:	JJA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/InputDeviceInfo)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE_INFO constructor
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_INFO::INPUT_DEVICE_INFO()
:
_nNbrXAbsAxes	( 0 ),
_nNbrYAbsAxes	( 0 ),
_nNbrZAbsAxes	( 0 ),
_nNbrXRelAxes	( 0 ),
_nNbrYRelAxes	( 0 ),
_nNbrZRelAxes	( 0 ),
_nNbrRXAxes		( 0 ),
_nNbrRYAxes		( 0 ),
_nNbrRZAxes		( 0 ),
_nNbrSliderAxes	( 0 ),
_nNbrButtons	( 0 ),
_nNbrKeys		( 0 ),
_nNbrPOVs		( 0 )
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE_INFO constructor
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_INFO::INPUT_DEVICE_INFO(const INPUT_DEVICE_INFO & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE_INFO destructor
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_INFO::~INPUT_DEVICE_INFO()
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_INFO &	INPUT_DEVICE_INFO::operator=(const INPUT_DEVICE_INFO & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::Copy(const INPUT_DEVICE_INFO & C)
{
	SetNbrXAbsAxes( C.GetNbrXAbsAxes() );
	SetNbrYAbsAxes( C.GetNbrYAbsAxes() );
	SetNbrZAbsAxes( C.GetNbrZAbsAxes() );
	SetNbrXRelAxes( C.GetNbrXRelAxes() );
	SetNbrYRelAxes( C.GetNbrYRelAxes() );
	SetNbrZRelAxes( C.GetNbrZRelAxes() );
	SetNbrRXAxes( C.GetNbrRXAxes() );
	SetNbrRYAxes( C.GetNbrRYAxes() );
	SetNbrRZAxes( C.GetNbrRZAxes() );
	SetNbrSliderAxes( C.GetNbrSliderAxes() );
	SetNbrButtons( C.GetNbrButtons() );
	SetNbrKeys( C.GetNbrKeys() );
	SetNbrPOVs( C.GetNbrPOVs() );
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrXAbsAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrXAbsAxes(const int	nNbrAxes)
{
	_nNbrXAbsAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrYAbsAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrYAbsAxes(const int	nNbrAxes)
{
	_nNbrYAbsAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrZAbsAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrZAbsAxes(const int	nNbrAxes)
{
	_nNbrZAbsAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrXRelAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrXRelAxes(const int	nNbrAxes)
{
	_nNbrXRelAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrYRelAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrYRelAxes(const int	nNbrAxes)
{
	_nNbrYRelAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrZRelAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrZRelAxes(const int	nNbrAxes)
{
	_nNbrZRelAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrRXAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrRXAxes(const int	nNbrAxes)
{
	_nNbrRXAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrRYAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrRYAxes(const int	nNbrAxes)
{
	_nNbrRYAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrRZAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrRZAxes(const int	nNbrAxes)
{
	_nNbrRZAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrSliderAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrSliderAxes(const int	nNbrAxes)
{
	_nNbrSliderAxes = nNbrAxes;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrButtons
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrButtons(const int	nNbrButtons)
{
	_nNbrButtons = nNbrButtons;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrKeys
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrKeys(const int	nNbrKeys)
{
	_nNbrKeys = nNbrKeys;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrPOVs
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::SetNbrPOVs(const int	nNbrPOVs)
{
	_nNbrPOVs = nNbrPOVs;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrXAbsAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrXAbsAxes() const
{
	return	( _nNbrXAbsAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrYAbsAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrYAbsAxes() const
{
	return	( _nNbrYAbsAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrZAbsAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrZAbsAxes() const
{
	return	( _nNbrZAbsAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrXRelAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrXRelAxes() const
{
	return	( _nNbrXRelAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrYRelAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrYRelAxes() const
{
	return	( _nNbrYRelAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrZRelAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrZRelAxes() const
{
	return	( _nNbrZRelAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRXAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrRXAxes() const
{
	return	( _nNbrRXAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRYAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrRYAxes() const
{
	return	( _nNbrRYAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRZAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrRZAxes() const
{
	return	( _nNbrRZAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrSliderAxes
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrSliderAxes() const
{
	return	( _nNbrSliderAxes );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrButtons
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrButtons() const
{
	return	( _nNbrButtons );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrKeys
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrKeys() const
{
	return	( _nNbrKeys );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPOVs
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_INFO::GetNbrPOVs() const
{
	return	( _nNbrPOVs );
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_INFO::Reset()
{
	SetNbrXAbsAxes( 0 );
	SetNbrYAbsAxes( 0 );
	SetNbrZAbsAxes( 0 );
	SetNbrXRelAxes( 0 );
	SetNbrYRelAxes( 0 );
	SetNbrZRelAxes( 0 );
	SetNbrRXAxes( 0 );
	SetNbrRYAxes( 0 );
	SetNbrRZAxes( 0 );
	SetNbrSliderAxes( 0 );
	SetNbrButtons( 0 );
	SetNbrKeys( 0 );
	SetNbrPOVs( 0 );
}

//=============================================================================
//	03-01-10:	JJA - Modified
//	CODE ENDS HERE
//=============================================================================
