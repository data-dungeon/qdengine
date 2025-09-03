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
//	CLASS:	FILTER_SCRATCH
//
//	04-01-15:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetMaterialInst
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	FILTER_SCRATCH::GetMaterialInst() const
{
	return (_ciMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
/*MATERIAL_ABC *	FILTER_SCRATCH::GetMaterial() const
{
	return (static_cast<MATERIAL_ABC*>(_ciMaterial.GetCommunicator()));
}*/

/*
//-----------------------------------------------------------------------------
//	Name:		GetGeometry
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	FILTER_SCRATCH::GetGeometry() const
{
	return (_pGeometry[_nCurrGeometry]);
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetColor
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
const COLOR &	FILTER_SCRATCH::GetColor() const
{
	return (_Color);
}

//-----------------------------------------------------------------------------
//	Name:		GetVisible
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
bool	FILTER_SCRATCH::GetVisible() const
{
	return (_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		GetDuration
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILTER_SCRATCH::GetDuration() const
{
	return (_nDuration);
}

//-----------------------------------------------------------------------------
//	Name:		GetFrequency
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILTER_SCRATCH::GetFrequency() const
{
	return (_nFrequency);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxSizeX
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
float	FILTER_SCRATCH::GetMaxSizeX() const
{
	return (_rMaxSizeX);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxSizeY
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
float	FILTER_SCRATCH::GetMaxSizeY() const
{
	return (_rMaxSizeY);
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterial
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetMaterial(const COM_INSTANCE &	ciMaterial)
{
	_ciMaterial = ciMaterial;
}

//-----------------------------------------------------------------------------
//	Name:		SetColor
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetColor(const COLOR &	C)
{
	_Color = C;
}

//-----------------------------------------------------------------------------
//	Name:		SetVisible
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetVisible(bool	b)
{
	_bVisible = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetDuration
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetDuration(unsigned int	n)
{
	_nDuration = n;
	// Assume Maya works in NTSC, the user sets a duration in frames...
	_rDurationInSec = 1000.0 * n / 30.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrequency
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetFrequency(unsigned int	n)
{
	_nFrequency = n;
	// Assume Maya works in NTSC, the user sets a duration in frames...
	// and we want ms
	_rFrequencyInSec = 1000.0 * n / 30.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxSizeX
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetMaxSizeX(float	r)
{
	_rMaxSizeX = r;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxSizeY
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetMaxSizeY(float	r)
{
	_rMaxSizeY = r;
}

//-----------------------------------------------------------------------------
//	Name:		GetRandomSize
//	Object:		
//	04-01-21:	RMA - Created
//-----------------------------------------------------------------------------
bool	FILTER_SCRATCH::GetRandomSize() const
{
	return (_bRandomSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetRandomSize
//	Object:		
//	04-01-21:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::SetRandomSize(bool	b)
{
	_bRandomSize = b;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
