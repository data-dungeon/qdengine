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
//	CLASS:	FACE
//
//	02-12-13:	JLT - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
int FACE::operator[](const unsigned int i) const
{
	QDT_ASSERT(i<3);
	return _Index[i];
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
int& FACE::operator[](const unsigned int i)
{
	QDT_ASSERT(i<3);
	return _Index[i];
}

//-----------------------------------------------------------------------------
//	Name:		GetNormal
//	Object:		
//	02-12-19:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	FACE::GetNormal() const
{
	return	(_Normal);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeNormal
//	Object:		
//	02-12-19:	ELE - Created
//-----------------------------------------------------------------------------
void	FACE::ComputeNormal(const VECTOR &	V1,
							const VECTOR &	V2)
{
	_Normal.Cross(V1, V2);
	_Normal.Normalize();
}

//-----------------------------------------------------------------------------
//	Name:		GetIndex
//	Object:		
//	03-10-28:	ELE - Created
//-----------------------------------------------------------------------------
int	FACE::GetIndex(int	i) const
{
	return	(_Index[i]);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
