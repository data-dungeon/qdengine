//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: SURFACE_MANAGER
//
//
//00 - 12 - 22 CVI Created:
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( SURFACE_MANAGER )
//
//	The SURFACE_MANAGER class implements ...
//
//*****************************************************************************


//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name:		DISPLAY_MANAGER::Instance
// Object:		For singleton : Instance 
// 01-01-24		FKE Created: 
//-----------------------------------------------------------------------------
/*SURFACE_MANAGER* SURFACE_MANAGER::Instance()
{
	QDT_ASSERT(_bDestroyedOnce == false ); 

	if (_pInstance == NULL) 
	{
		_pInstance = new SURFACE_MANAGER();
	}
	
	return (_pInstance); 
}

//-----------------------------------------------------------------------------
// Name:		DISPLAY_MANAGER::Suicide
// Object:		For singleton : suicide
// 01-01-24		FKE Created: 
//-----------------------------------------------------------------------------
void SURFACE_MANAGER::Suicide()
{
	QDT_ASSERT(_bDestroyedOnce == false ); 
	QDT_ASSERT(_pInstance != NULL);
	
	if (_pInstance != NULL) 
	{
		delete (_pInstance);
		_pInstance		= NULL;
		_bDestroyedOnce	= true;
	}
}

bool	SURFACE_MANAGER::IsTransformed()const
{
	return ( _bTransformed );
}

bool	SURFACE_MANAGER::IsProjected()const
{
	return ( _bProjected );
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::SetCurrentFrameRate
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
void	SURFACE_MANAGER::SetCurrentFrameRate( float rCurrentFrameRate )
{
	_rCurrentFrameRate = rCurrentFrameRate;
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::GetCurrentFrameRate
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
float	SURFACE_MANAGER::GetCurrentFrameRate()const
{
	return ( _rCurrentFrameRate );
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::GetLastTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
float	SURFACE_MANAGER::GetLastTesselParam()const
{
	return ( _rLastTesselParam );
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::SetLastTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
void	SURFACE_MANAGER::SetLastTesselParam( float rLastTP )
{
	_rLastTesselParam = rLastTP;
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::GetTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
float	SURFACE_MANAGER::GetTesselParam()const
{
	return ( _rCurrentTesselParam );
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::SetTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
void	SURFACE_MANAGER::SetTesselParam( float rTP )
{
	_rCurrentTesselParam = rTP;
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::IsConstantTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
bool	SURFACE_MANAGER::IsConstantTesselParam()const
{
	return ( _bConstantTesselParam );
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::SetConstantTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
void	SURFACE_MANAGER::SetConstantTesselParam( bool bConstant )
{
	_bConstantTesselParam = bConstant;
	if ( bConstant )
	{
		SetTesselParam( 1.0f );
	}
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::SetRateToKeep
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
void	SURFACE_MANAGER::SetRateToKeep( float rToKeep )
{
	_rRateToKeep = rToKeep;
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::GetRateToKeep
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
float	SURFACE_MANAGER::GetRateToKeep()const
{
	return ( _rRateToKeep );
}*/

//=============================================================================
// CODE ENDS HERE
//=============================================================================
