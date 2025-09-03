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
//	CLASS:	ANIM_NODE_STRUCT
//
//	01-10-11:	cvi - Created
//*****************************************************************************


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetTimeAttr
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
const	TIME_ATTR_EX &	ANIM_NODE_STRUCT::GetTimeAttr()const
{
	return ( _TimeAttr );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimStatus
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
const	ANIM_STATUS &	ANIM_NODE_STRUCT::GetAnimStatus()const
{
	return ( _AnimStatus );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimLink
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
const	ANIM_LINK &	ANIM_NODE_STRUCT::GetAnimLink()const
{
	return ( _AnimLink );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrChildren
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_NODE_STRUCT::GetNbrChildren() const
{
	return ( _nNbrChildren );
}

//-----------------------------------------------------------------------------
//	Name:		IsTeleport
//	Object:		
//	01-10-15:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsTeleport() const
{
	return ( _AnimStatus.IsTeleport() );
}

//-----------------------------------------------------------------------------
//	Name:		SetChildIndex
//	Object:		
//	01-10-15:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetChildIndex(int	nIndex)
{
	_nChildIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetChildIndex
//	Object:		
//	01-10-15:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_NODE_STRUCT::GetChildIndex() const
{
	return ( _nChildIndex );
}

//-----------------------------------------------------------------------------
//	Name:		SetTimeAttr
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetTimeAttr(const TIME_ATTR_EX &	TimeAttr)
{
	_TimeAttr = TimeAttr;
}

//-----------------------------------------------------------------------------
//	Name:		SetContainer
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetContainer(CONTAINER *	pC)
{
	_AnimLink.SetContainer( pC );
}

//-----------------------------------------------------------------------------
//	Name:		SetEndTime
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetEndTime(float	rVal)
{
	_TimeAttr.SetEndTime( rVal );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimLink
//	Object:		
//	01-10-24:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_LINK*	ANIM_NODE_STRUCT::GetAnimLink()
{
	return ( &_AnimLink );
}

//-----------------------------------------------------------------------------
//	Name:		HasBlending
//	Object:		
//	01-11-09:	cvi - Created
//-----------------------------------------------------------------------------
bool		ANIM_NODE_STRUCT::HasBlending() const
{
	return ( _AnimStatus.IsBlended() );
}


//-----------------------------------------------------------------------------
//	Name:		IsManualTimer
//	Object:		
//	02-10-01:	CVI - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsManualTimer() const
{
	return ( _TimeAttr.IsManualIncrement() );
}

//-----------------------------------------------------------------------------
//	Name:		SetManualTimer
//	Object:		
//	02-10-01:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetManualTimer(bool	b)
{
	_TimeAttr.SetManualIncrement( b );
}

//-----------------------------------------------------------------------------
//	Name:		SetManualIncrement
//	Object:		
//	02-10-01:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetManualIncrement(float	rVal)
{
	_TimeAttr.SetFrameIncrement( rVal );
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentFrame
//	Object:		
//	02-10-22:	CVI - Created
//-----------------------------------------------------------------------------
float	ANIM_NODE_STRUCT::GetCurrentFrame() const
{
	return ( _TimeAttr.GetCurTime() );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimNode
//	Object:		
//	03-03-18:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_NODE *	ANIM_NODE_STRUCT::GetAnimNode()
{
	return ( _pAnimNode );
}

//-----------------------------------------------------------------------------
//	Name:		SetAnimNode
//	Object:		
//	03-03-18:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetAnimNode(ANIM_NODE *	pAN)
{
	_pAnimNode = pAN;
}


//-----------------------------------------------------------------------------
//	Name:		SetCurTime
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetCurTime(float	rVal)
{
	_TimeAttr.SetCurTime( rVal );
}

//-----------------------------------------------------------------------------
//	Name:		IsActive
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsActive() const
{
	return ( _bIsActive );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlendingFrames
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
float	ANIM_NODE_STRUCT::GetNbrBlendingFrames() const
{
	return ( _TimeAttr.GetBlendingTimeLong() );
}

//-----------------------------------------------------------------------------
//	Name:		SetAnimEndBlending
//	Object:		
//	03-10-16:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetAnimEndBlending(bool	bVal)
{
	_bBlendAtAnimEnd = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		SetPartStartIndex
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetPartStartIndex(int	nIndex)
{
	_nPartStartIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetPartStartIndex
//	Object:		
//	04-01-31:	JJA - Created
//-----------------------------------------------------------------------------
int	ANIM_NODE_STRUCT::GetPartStartIndex() const
{
	return	( _nPartStartIndex );
}

//-----------------------------------------------------------------------------
//	Name:		SetPartEndTrack
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetPartEndTrack(int	nIndex)
{
	_nPartEndIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetPartEndTrack
//	Object:		
//	04-01-31:	JJA - Created
//-----------------------------------------------------------------------------
int	ANIM_NODE_STRUCT::GetPartEndTrack() const
{
	return	( _nPartEndIndex );
}

//-----------------------------------------------------------------------------
//	Name:		SetPartAnim
//	Object:		
//	03-12-03:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetPartAnim(bool	bVal)
{
	_bIsPartAnim = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		IsFirstUpdate
//	Object:		
//	04-12-07:	CVI - Created
//-----------------------------------------------------------------------------
bool	ANIM_NODE_STRUCT::IsFirstUpdate() const
{
	return ( _bFirstUpdate );
}

//-----------------------------------------------------------------------------
//	Name:		SetFirstUpdateSatus
//	Object:		
//	04-12-07:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_NODE_STRUCT::SetFirstUpdateSatus(bool	bVal)
{
	_bFirstUpdate = bVal;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
