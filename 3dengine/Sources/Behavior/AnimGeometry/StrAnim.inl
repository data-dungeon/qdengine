
//-----------------------------------------------------------------------------
//	Name:		MustNotifyCollideForTeleport
//	Object:		
//	04-12-07:	CVI - Created
//-----------------------------------------------------------------------------
bool	STR_ANIM::MustNotifyCollideForTeleport() const
{
	return ( _bNotifyCollideForTeleport );
}

//-----------------------------------------------------------------------------
//	Name:		AddNotifyCollideForTeleport
//	Object:		
//	04-12-07:	CVI - Created
//-----------------------------------------------------------------------------
void	STR_ANIM::AddNotifyCollideForTeleport(bool	bVal)
{
	_bNotifyCollideForTeleport |= bVal;
}

//-----------------------------------------------------------------------------
//	Name:		ResetCollideNotification
//	Object:		
//	04-12-07:	CVI - Created
//-----------------------------------------------------------------------------
void	STR_ANIM::ResetCollideNotification()
{
	_bNotifyCollideForTeleport = false;
}

