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

#include	"Root.h"
#include	"AnimLink.h"
#include	INCL_3DENGINE(Behavior/AnimNode/AnimNode)
#include	INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)

#ifdef _DEBUG
	#include "AnimLink.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ANIM_LINK constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_LINK::ANIM_LINK()
:
_PrevSeq(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_NextSeq(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	_pContainer	= NULL;
	_ChildList.Clear();
	_nChildIndexlist.Clear();



}

//-----------------------------------------------------------------------------
//	Name:		ANIM_LINK constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_LINK::ANIM_LINK(const ANIM_LINK & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_LINK destructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_LINK::~ANIM_LINK()
{
	if ( !_ChildList.IsEmpty() )
	{
		ResetChildList();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	03-06-06:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::Reset()
{
	_pContainer = NULL;

	_PrevSeq.Release();
	_NextSeq.Release();
	_ChildList.Clear();
}


//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_LINK &	ANIM_LINK::operator=(const ANIM_LINK & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::Copy(const ANIM_LINK & C)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		RemoveChild
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::RemoveChild(const COM_INSTANCE& CI)
{
	int									n, i;
	COM_INSTANCE						CICurrent;
	QDT_VECTOR<COM_INSTANCE>::ITERATOR	It;
	QDT_VECTOR<int>::ITERATOR			ItChildIndex;

	/////////////////////////////////////////////
	n				= GetNbrChildren();
	It				= _ChildList.GetHead();
	ItChildIndex	= _nChildIndexlist.GetHead();
	///////////////////////////////////////////

	/////////////////////////
	for ( i = 0; i < n; ++i )
	{
		CICurrent = GetChild( i );
		//
		if ( CICurrent.GetCommunicator() == CI.GetCommunicator() )
		{
			_ChildList.Remove( It );
			_nChildIndexlist.Remove( ItChildIndex );
			break;
		}
		//
		It++;
		ItChildIndex++;
		//
	}
	/////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		ResetChildList
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::ResetChildList()
{
	_ChildList.Clear();
	_nChildIndexlist.Clear();
}


//-----------------------------------------------------------------------------
//	Name:		GetChildPos
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_LINK::GetChildPos(int	nChildIndex) const
{
	int		i, n;

	/////////////////////////////////
	n = GetNbrChildren();
	if ( n <= 0 )
	{
		QDT_FAIL();
		return(  -1 );
	}
	/////////////////////////////////

	/////////////////////////////////
	for ( i = 0; i < n; ++i )
	{
		if ( GetChildIndex( i ) == nChildIndex )
		{
			return ( i );
		}
	}
	/////////////////////////////////

	//	Child not found
	QDT_FAIL();
	return ( -1 );

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////      SEQUENCE      //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//	Name:		AddSequence
//	Object:		
//	01-10-16:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::AddSequence(const COM_INSTANCE &CI, ANIM_NODE *pHolder )
{
	ANIM_NODE		*pAN;

	///////////////////////////////////
	pAN				= static_cast<ANIM_NODE*> (CI.GetCommunicator() );
	SetNextSeq( CI );
	///////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	pAN->GetAnimStruct()->GetAnimLink()->SetPrevSeq( COM_INSTANCE(pHolder) );
	pHolder->GetAnimStruct()->OnSequenceAdded( pAN );
	//////////////////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		InsertSequence
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::InsertSequence(const COM_INSTANCE& CI, ANIM_NODE *pHolder )
{
	ANIM_NODE	*pAN1;
	COM_INSTANCE	OldCI;

	/////////////////////////
	if ( !CI.HasHandle() )	return;
	if ( _NextSeq.HasHandle() == false )
	{
		AddSequence( CI, pHolder );
		return;
	}
	/////////////////////////
	OldCI = GetNextSeq();
	OldCI.Release();
	SetNextSeq( OldCI );
	AddSequence( CI, pHolder );
	/////////////////////////
	pAN1 = static_cast<ANIM_NODE*> ( CI.GetCommunicator() );
	pAN1->GetAnimStruct()->GetAnimLink()->AddSequence( OldCI, pAN1 );
	/////////////////////////	
}


//-----------------------------------------------------------------------------
//	Name:		RemoveSequence
//	Object:		
//	01-11-08:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_LINK::RemoveSequence( const COM_INSTANCE &CI)
{
	ANIM_NODE	*pAN;

	//////////////////////////////	
	if ( !GetNextSeq().HasHandle() )	return;
	if ( GetNextSeq() == CI )
	{
		pAN = static_cast<ANIM_NODE*> ( CI.GetCommunicator() );
		pAN->GetAnimStruct()->OnRemovedAsNextSequence();
		_NextSeq.Release();
		return;
	}
	//////////////////////////////	
	pAN = static_cast<ANIM_NODE*> ( _NextSeq.GetCommunicator() );
	pAN->GetAnimStruct()->GetAnimLink()->RemoveSequence( CI );
	//////////////////////////////	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////      CHILDREN      //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//	Name:		IsPlayingAnim
//	Object:		
//	02-02-22:	RMA - Created
//-----------------------------------------------------------------------------
bool	ANIM_LINK::IsPlayingAnim(const COM_INSTANCE& CI)const
{
	if (_ChildList.IsEmpty()) return (false);

	int nNbrChildren = _ChildList.GetSize();
	int nEachChildren;
	ANIM_NODE *pChild;

	for (nEachChildren = 0; nEachChildren < nNbrChildren ; ++nEachChildren)
	{
		pChild = static_cast<ANIM_NODE *>( _ChildList[nEachChildren].GetCommunicator() );
		if ( pChild->GetAnimStruct()->IsPlayingAnimRec( CI) ) return ( true );
	}

	return (false);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
