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
//	CLASS:	ANIM_STATUS
//
//	01-10-11:	cvi - Created
//*****************************************************************************


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IsAbsolute
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsAbsolute() const
{
	return ( _nFlags.IsSet( ANIM_ABSOLUTE ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsRelative
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsRelative() const
{
	return ( !IsAbsolute() );
}

//-----------------------------------------------------------------------------
//	Name:		IsTeleport
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsTeleport() const
{
	return (_nFlags.IsSet(ANIM_TELEPORT));
}

//-----------------------------------------------------------------------------
//	Name:		IsLoop
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsLoop() const
{
	return (_nFlags.IsSet( ANIM_LOOP ));
}

//-----------------------------------------------------------------------------
//	Name:		IsPingPong
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsPingPong() const
{
	return(_nFlags.IsSet( ANIM_PINGPONG ));
}

//-----------------------------------------------------------------------------
//	Name:		IsPreemptive
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsPreemptive() const
{
	return ( _nFlags.IsSet( ANIM_PREEMPTIVE ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsMix
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsMix() const
{
	return ( _nFlags.IsSet( ANIM_MIX ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsCompose
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsCompose() const
{
	return (!IsMix());
}

//-----------------------------------------------------------------------------
//	Name:		IsBlended
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsBlended() const
{
	return (_nFlags.IsSet( ANIM_BLENDED ));
}

//-----------------------------------------------------------------------------
//	Name:		IsDelegateBlended
//	Object:		
//	02-01-17:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsDelegateBlended() const
{
	return (_nFlags.IsSet( ANIM_DELEGATE_BLENDING ));
}

//-----------------------------------------------------------------------------
//	Name:		SetAbsolute
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetAbsolute(bool	bVal)
{
	_nFlags.Set(ANIM_ABSOLUTE, bVal);
}

//-----------------------------------------------------------------------------
//	Name:		SetRelative
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetRelative(bool	bVal)
{
	_nFlags.Set(ANIM_ABSOLUTE, !bVal);
}

//-----------------------------------------------------------------------------
//	Name:		SetTeleport
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetTeleport(bool	bVal)
{
	_nFlags.Set(ANIM_TELEPORT, bVal);
}


//-----------------------------------------------------------------------------
//	Name:		SetLoop
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetLoop(bool	b)
{
	_nFlags.Set(ANIM_LOOP, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetPingPong
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetPingPong(bool	b)
{
	_nFlags.Set(ANIM_PINGPONG, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetPreemptive
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetPreemptive(bool	b)
{
	_nFlags.Set(ANIM_PREEMPTIVE, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetMix
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetMix(bool	b)
{
	_nFlags.Set(ANIM_MIX, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetCompose
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetCompose(bool	b)
{
	_nFlags.Set(ANIM_MIX, b);
}

//-----------------------------------------------------------------------------
//	Name:		IsRelCor
//	Object:		
//	01-10-30:	cvi - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsRelCor() const
{
	return ( _nFlags.IsSet( ANIM_RELCOR ) );	
}

//-----------------------------------------------------------------------------
//	Name:		SetRelCor
//	Object:		
//	01-10-30:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetRelCor(bool	b)
{
	_nFlags.Set(ANIM_RELCOR, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetBlended
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetBlended(bool	b)
{
	_nFlags.Set(ANIM_BLENDED, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetDelegateBlended
//	Object:		
//	02-01-17:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetDelegateBlended(bool	b)
{
	_nFlags.Set(ANIM_DELEGATE_BLENDING, b);
}

//-----------------------------------------------------------------------------
//	Name:		CanBeDelete
//	Object:		
//	02-06-03:	CVI - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::CanBeDelete() const
{
	return ( _nFlags.IsSet( ANIM_ENABLE_DELETE ) );
}

//-----------------------------------------------------------------------------
//	Name:		EnableDeletation
//	Object:		
//	02-06-03:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::EnableDeletation(bool	b )
{
	_nFlags.Set(ANIM_ENABLE_DELETE, b);
}

//-----------------------------------------------------------------------------
//	Name:		IsPersistant
//	Object:		
//	02-10-01:	CVI - Created
//-----------------------------------------------------------------------------
bool	ANIM_STATUS::IsPersistant() const
{
	return ( _nFlags.IsSet( ANIM_PERSISTANT ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetPersistant
//	Object:		
//	02-10-01:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::SetPersistant(bool	b)
{
	_nFlags.Set(ANIM_PERSISTANT, b);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
