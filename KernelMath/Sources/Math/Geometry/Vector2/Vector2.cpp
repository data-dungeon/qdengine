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
//	CLASS:	VECTOR_2
//
//	03-03-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)

#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#	include	"Vector2.inl"
#endif

const VECTOR_2	VECTOR_2::ZERO(0.0f, 0.0f);

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-03-10:	RMA - Created
//-----------------------------------------------------------------------------
void VECTOR_2::Read(IO_STREAM& Stream)
{
	Stream >> _rX >> _rY;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-03-10:	RMA - Created
//-----------------------------------------------------------------------------
void VECTOR_2::Write(IO_STREAM&	Stream) const
{
	Stream << _rX << _rY;
}

//-----------------------------------------------------------------------------
//	Name:		IsInsideTriangle
//	Object:		
//	03-03-18:	RMA - Created
//-----------------------------------------------------------------------------
bool VECTOR_2::IsInsideTriangle(const VECTOR_2 Triangle[3]) const
{		
	VECTOR_2 V1, V2;

	V1.Sub(Triangle[1], Triangle[0]);
	V2.Sub(Triangle[0], *this);
	if (V1.Cross(V2) < 0.0f)
	{
		return false;
	}
	else
	{
		V1.Sub(Triangle[2], Triangle[1]);
		V2.Sub(Triangle[1], *this);
		if (V1.Cross(V2) < 0.0f)
		{
			return false;
		}
		else
		{
			V1.Sub(Triangle[0], Triangle[2]);
			V2.Sub(Triangle[2], *this);
			if (V1.Cross(V2) < 0.0f)
			{
				return false;
			}
		}
	}

	return true;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
