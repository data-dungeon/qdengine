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
// CLASS: CONTAINER
//
//
//	01-04-17:	GHP - Created
//*****************************************************************************
//=============================================================================
// CODE STARTS HERE
//=============================================================================
//-----------------------------------------------------------------------------
//	Name:		HasAJoint
//	Object:		
//	01-04-17:	GHP - Created
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Name:		GetMatrix
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
MATRIX &	CONTAINER::GetMatrix()
{
	return (_LMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetMatrix
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX &	CONTAINER::GetMatrix() const
{
	return (_LMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetHolderNode
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
NODE *	CONTAINER::GetHolderNode() const
{
	return ( _pHolderNode );
}


//-----------------------------------------------------------------------------
//	Name:		SetMatrix
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetMatrix(MATRIX &	Mat)
{
	_LMatrix = Mat;
}

//-----------------------------------------------------------------------------
//	Name:		SetHolderNode
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetHolderNode( NODE *pN )
{
	_pHolderNode = pN;
}

//-----------------------------------------------------------------------------
//	Name:		SetAnimNode
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetAnimNode( const COM_INSTANCE& Anim )
{
	_FirstAnimNode = Anim;
}

//-----------------------------------------------------------------------------
//	Name:		ResetTeleport
//	Object:		
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ResetTeleport()
{
	_Teleport.Id();
}

//-----------------------------------------------------------------------------
//	Name:		SetIndex
//	Object:		
//	03-07-30:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetIndex(int	nIndex)
{
	_nIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetIndex
//	Object:		
//	03-07-30:	CVI - Created
//-----------------------------------------------------------------------------
int	CONTAINER::GetIndex() const
{
	return ( _nIndex );
}

//-----------------------------------------------------------------------------
//	Name:		SetMainContainer
//	Object:		
//	03-07-30:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetMainContainer(CONTAINER *	pContainer)
{
	_pMainContainer = pContainer;
}

//-----------------------------------------------------------------------------
//	Name:		IsPlayingAnimation
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	CONTAINER::IsPlayingAnimation() const
{
	if ( _FirstAnimNode.HasHandle() )
	{
		return ( true);
	}
	if ( _PartAnim._PartAnimInst.HasHandle() )
	{
		return ( true );
	}
	return (false);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
