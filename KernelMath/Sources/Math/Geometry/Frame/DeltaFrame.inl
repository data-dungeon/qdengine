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
//	CLASS:	DELTA_FRAME
//
//	02-06-10:	JLT - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR DELTA_FRAME::operator[](const unsigned int i) const
{
	QDT_ASSERT(i<2);
	return (_v[i]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR& DELTA_FRAME::operator[](const unsigned int i)
{
	QDT_ASSERT(i<2);
	return (_v[i]);	
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
