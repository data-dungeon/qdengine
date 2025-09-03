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
//	CLASS:	FILTER_LAYER
//
//	04-01-05:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetMaterialInst
//	Object:		
//	04-01-05:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	FILTER_LAYER::GetMaterialInst() const
{
	return (_ciMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	04-01-05:	RMA - Created
//-----------------------------------------------------------------------------
/*MATERIAL_ABC *	FILTER_LAYER::GetMaterial() const
{
	return (static_cast<MATERIAL_ABC*>(_ciMaterial.GetCommunicator()));
}*/

/*
//-----------------------------------------------------------------------------
//	Name:		GetGeometry
//	Object:		
//	04-01-05:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	FILTER_LAYER::GetGeometry() const
{
	return (_pGeometry[_nCurrGeometry]);
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetMaterial
//	Object:		
//	04-01-05:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::SetMaterial(const COM_INSTANCE &	ciMaterial)
{
	_ciMaterial = ciMaterial;
}

//-----------------------------------------------------------------------------
//	Name:		SetColor
//	Object:		
//	04-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::SetColor(const COLOR &	C)
{
	_Color = C;
}

//-----------------------------------------------------------------------------
//	Name:		GetColor
//	Object:		
//	04-01-13:	RMA - Created
//-----------------------------------------------------------------------------
const COLOR &	FILTER_LAYER::GetColor() const
{
	return (_Color);
}

//-----------------------------------------------------------------------------
//	Name:		GetVisible
//	Object:		
//	04-01-14:	RMA - Created
//-----------------------------------------------------------------------------
bool	FILTER_LAYER::GetVisible() const
{
	return (_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		GetMatchPixel
//	Object:		
//	04-01-14:	RMA - Created
//-----------------------------------------------------------------------------
bool	FILTER_LAYER::GetMatchPixel() const
{
	return (_bMatchPixel);
}

//-----------------------------------------------------------------------------
//	Name:		SetVisible
//	Object:		
//	04-01-14:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::SetVisible(bool	b)
{
	_bVisible = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetMatchPixel
//	Object:		
//	04-01-14:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::SetMatchPixel(bool	b)
{
	_bMatchPixel = b;
}

//-----------------------------------------------------------------------------
//	Name:		GetRandomUV
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
bool	FILTER_LAYER::GetRandomUV() const
{
	return (_bRandomUV);
}

//-----------------------------------------------------------------------------
//	Name:		SetRandomUV
//	Object:		
//	04-01-15:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::SetRandomUV(bool	b)
{
	_bRandomUV = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrequency
//	Object:		
//	04-01-21:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::SetFrequency(unsigned int	nFrequency)
{
	_nFrequency = nFrequency;
	_rFrequencyInSec = 1000.0 * nFrequency / 30.0f;
}

//-----------------------------------------------------------------------------
//	Name:		GetFrequency
//	Object:		
//	04-01-21:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILTER_LAYER::GetFrequency() const
{
	return (_nFrequency);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
