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
//	CLASS:	ANIM_LINK
//
//	01-10-11:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetPrevSeq
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------

const COM_INSTANCE&	ANIM_LINK::GetPrevSeq()const 
{
	return ( _PrevSeq );
}

//-----------------------------------------------------------------------------
//	Name:		GetNextSeq
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE&	ANIM_LINK::GetNextSeq()const 
{
	return ( _NextSeq );
}

//-----------------------------------------------------------------------------
//	Name:		GetChild
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE&	ANIM_LINK::GetChild(int	nNumChild)const 
{
	//////////////////////////////////////
	//		BAD CASE
	if ( GetNbrChildren() <= nNumChild )
	{
		QDT_FAIL();
	}

	if ( nNumChild < 0 )
	{
		QDT_FAIL();
	}
	//////////////////////////////////////
	//	OK
	//////////////////////////////////////
	
	return ( _ChildList[nNumChild] );
	//////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetContainer
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
CONTAINER*	ANIM_LINK::GetContainer() const
{
	return( _pContainer );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrChildren
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_LINK::GetNbrChildren() const
{
	int		n;

	if ( _ChildList.IsEmpty() )
	{
		return ( 0 );
	}

	n = _ChildList.GetSize();
	return ( n );
}


//-----------------------------------------------------------------------------
//	Name:		SetPrevSeq
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::SetPrevSeq(const COM_INSTANCE&	CI)
{
	_PrevSeq = CI;
}

//-----------------------------------------------------------------------------
//	Name:		SetNextSeq
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::SetNextSeq(const COM_INSTANCE&	CI)
{
	_NextSeq = CI;
}

//---------------------------------------------------------------------------
//	Name:		SetContainer
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::SetContainer(CONTAINER *	pC)
{
	_pContainer = pC;
}

//-----------------------------------------------------------------------------
//	Name:		GetChildIndex
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_LINK::GetChildIndex(int	nPos) const
{
	return ( _nChildIndexlist[nPos] );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
