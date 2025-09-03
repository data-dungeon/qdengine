//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM_BASE STR_ANIM_BASE::operator *	(const STR_ANIM_BASE &	SAB) const
{
	STR_ANIM_BASE STR;

	///////////////////////////////////////////////
	STR._AbsRel		= SAB._AbsRel;
	STR._Teleport	= _Teleport * SAB._Teleport;
	///////////////////////////////////////////////

	return ( STR );
}

//-----------------------------------------------------------------------------
//	Name:		=
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM_BASE& STR_ANIM_BASE::operator *=(const STR_ANIM_BASE &	SAB) 
{
	_AbsRel = SAB._AbsRel;
	_Teleport = _Teleport * SAB._Teleport;

	return ( *this );
}

