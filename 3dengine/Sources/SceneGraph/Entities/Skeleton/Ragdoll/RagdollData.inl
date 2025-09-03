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
//	CLASS:	RAGDOLL_DATA
//
//	06-03-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetNbrActors
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RAGDOLL_DATA::GetNbrActors() const
{
	return	(_uiNbrActors);
}

//-----------------------------------------------------------------------------
//	Name:		GetActor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
const RAGDOLL_ACTOR *	RAGDOLL_DATA::GetActor(UInt32	uiActorIndex) const
{
	QDT_ASSERT(uiActorIndex < _uiNbrActors);
	return	(_pActors[uiActorIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrConstraints
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RAGDOLL_DATA::GetNbrConstraints() const
{
	return	(_uiNbrConstraints);
}

//-----------------------------------------------------------------------------
//	Name:		GetConstraint
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
const RAGDOLL_CONSTRAINT &	RAGDOLL_DATA::GetConstraint(UInt32	uiConstraintIndex) const
{
	QDT_ASSERT(uiConstraintIndex < _uiNbrConstraints);
	return	(_pConstraints[uiConstraintIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		SetActors
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::SetActors(UInt32				nNbrActors,
								RAGDOLL_ACTOR **	pActors)
{
	_uiNbrActors = nNbrActors;
	_pActors = pActors;
}

//-----------------------------------------------------------------------------
//	Name:		SetConstraints
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::SetConstraints(UInt32					nNbrConstraints,
									 RAGDOLL_CONSTRAINT *	pConstraints)
{
	_uiNbrConstraints = nNbrConstraints;
	_pConstraints = pConstraints;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
