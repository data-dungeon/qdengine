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
// CLASS: DUAL_EDGE
//
//
//00 - 12 - 22 CVI Created:
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( DUAL_EDGE )
//
//	The DUAL_EDGE class implements ...
//
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DUAL_EDGE constructor
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
DUAL_EDGE::DUAL_EDGE()
:
_nEdgeId1		( 0 ),
_nEdgeId2		( 0 ),
_nHandleSurf1	( 0 ),
_nHandleSurf2	( 0 ),
_nStatus		( 0 )	
{
}


//-----------------------------------------------------------------------------
//	Name:		DUAL_EDGE destructor
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
DUAL_EDGE::~DUAL_EDGE()
{
}


//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::Build( const DUAL_EDGE & DualEdge )
{
	if( this != &DualEdge)
	{
		_nEdgeId1		 = DualEdge._nEdgeId1;
		_nEdgeId2		 = DualEdge._nEdgeId2;
		_nHandleSurf1	 = DualEdge._nHandleSurf1;
		_nHandleSurf2	 = DualEdge._nHandleSurf2;
		_nStatus		 = DualEdge._nStatus;
	}
}


/*************** Accesseurs ****************/

//-----------------------------------------------------------------------------
//	Name:		GetEdgeId1
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
int		DUAL_EDGE::GetEdgeId1()const
{
	return ( _nEdgeId1 );
}


//-----------------------------------------------------------------------------
//	Name:		GetEdgeId2
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
int		DUAL_EDGE::GetEdgeId2()const
{
	return ( _nEdgeId2 );
}


//-----------------------------------------------------------------------------
//	Name:		GetHandleSurf1
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
int		DUAL_EDGE::GetHandleSurf1()const
{
	return ( _nHandleSurf1 );
}


//-----------------------------------------------------------------------------
//	Name:		GetHandleSurf2
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
int		DUAL_EDGE::GetHandleSurf2()const
{
	return ( _nHandleSurf2 );
}


//-----------------------------------------------------------------------------
//	Name:		GetStatus
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
int		DUAL_EDGE::GetStatus()const
{
	return ( _nStatus );
}


//-----------------------------------------------------------------------------
//	Name:		SetEdgeId1
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::SetEdgeId1( int n )
{
	_nEdgeId1 = n;
}


//-----------------------------------------------------------------------------
//	Name:		SetEdgeId2
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::SetEdgeId2( int n )
{
	_nEdgeId2 = n;
}


//-----------------------------------------------------------------------------
//	Name:		SetHandleSurf1
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::SetHandleSurf1( int n )
{
	_nHandleSurf1 = n;
}


//-----------------------------------------------------------------------------
//	Name:		SetHandleSurf2
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::SetHandleSurf2( int n )
{
	_nHandleSurf2 = n;
}


//-----------------------------------------------------------------------------
//	Name:		SetStatus
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::SetStatus( int n )
{
	_nStatus = n;
}

//-----------------------------------------------------------------------------
//	Name:		InvertEdges
//	Object:		invert the two edges for culling
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	DUAL_EDGE::InvertEdges()
{
	int	nId;

	//	First invert the two edges
	nId = GetEdgeId1();
	SetEdgeId1( GetEdgeId2() );
	SetEdgeId2( nId );

	//	Now handle of the surfaces
	nId = GetHandleSurf1();
	SetHandleSurf1( GetHandleSurf2() );
	SetHandleSurf2( nId );

}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
