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
//	CLASS:	RAY_CASTER
//
//	01-07-26:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		HasHitSomething
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_CASTER::HasHitSomething() const
{
	QDT_NOT_IMPLEMENTED();
	return	(false);

	if	(_pRayCastParameters->GetImpactNodeID() != 0)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetImpactPoint
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR	RAY_CASTER::GetImpactPoint() const
{
	return (_pRayCastParameters->GetImpactPoint());
}

//-----------------------------------------------------------------------------
//	Name:		GetImpactNode
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE RAY_CASTER::GetImpactNodeInst() const
{
	return (COM_INSTANCE(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE,_pRayCastParameters->GetImpactNodeID()));
}

//-----------------------------------------------------------------------------
//	Name:		GetImpactDistance
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
float	RAY_CASTER::GetImpactDistance() const
{
	return (_pRayCastParameters->GetImpactDistance());
}


//-----------------------------------------------------------------------------
//	Name:		GetImpactNormal
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR &	RAY_CASTER::GetImpactNormal() const
{
	return (_pRayCastParameters->GetImpactNormal());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
