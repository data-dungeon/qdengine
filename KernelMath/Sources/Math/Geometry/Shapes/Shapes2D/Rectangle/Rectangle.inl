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
//	CLASS:	RECTANGLE
//
//	01-10-22:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RECTANGLE constructor
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
RECTANGLE::RECTANGLE()
:
_nLeft	( 0 ),
_nRight	( 0 ),
_nTop	( 0 ),
_nBottom( 0 )
{
}


//-----------------------------------------------------------------------------
//	Name:		RECTANGLE constructor
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
RECTANGLE::RECTANGLE(int	nLeft,
					 int	nRight,
					 int	nTop,
					 int	nBottom)
:
_nLeft	( nLeft ),
_nRight	( nRight ),
_nTop	( nTop ),
_nBottom( nBottom )
{
}



//-----------------------------------------------------------------------------
//	Name:		RECTANGLE constructor
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
RECTANGLE::RECTANGLE(const RECTANGLE & C)
{
	Copy(C);
}


//-----------------------------------------------------------------------------
//	Name:		RECTANGLE destructor
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
RECTANGLE::~RECTANGLE()
{
	// Do nothing.
}


//-----------------------------------------------------------------------------
//	Name:		GetLeft
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
int	RECTANGLE::GetLeft() const
{
	return ( _nLeft );
}


//-----------------------------------------------------------------------------
//	Name:		GetRight
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
int	RECTANGLE::GetRight() const
{
	return ( _nRight );
}


//-----------------------------------------------------------------------------
//	Name:		GetTop
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
int	RECTANGLE::GetTop() const
{
	return ( _nTop );	
}


//-----------------------------------------------------------------------------
//	Name:		GetBottom
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
int	RECTANGLE::GetBottom() const
{
	return ( _nBottom );
}


//-----------------------------------------------------------------------------
//	Name:		SetLeft
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
void	RECTANGLE::SetLeft(const int	nLeft)
{
	_nLeft = nLeft;
}


//-----------------------------------------------------------------------------
//	Name:		SetRight
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
void	RECTANGLE::SetRight(const int	nRight)
{
	_nRight = nRight;
}


//-----------------------------------------------------------------------------
//	Name:		SetTop
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
void	RECTANGLE::SetTop(const int	nTop)
{
	_nTop = nTop;
}


//-----------------------------------------------------------------------------
//	Name:		SetBottom
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
void	RECTANGLE::SetBottom(const int	nBottom)
{
	_nBottom = nBottom;
}




//=============================================================================
//	CODE ENDS HERE
//=============================================================================
