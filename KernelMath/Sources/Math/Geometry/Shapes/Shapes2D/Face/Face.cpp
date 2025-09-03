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

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Face/Face)
#include 	INCL_KCORE(stream/iostream)

#ifdef _DEBUG 
#include "Face.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FACE constructor
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
FACE::FACE()
{
	_Index[0] = -1;
	_Index[1] = -1;
	_Index[2] = -1;
}

//-----------------------------------------------------------------------------
//	Name:		FACE constructor
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
FACE::FACE(const int i0, const int i1, const int i2)
{
	_Index[0] = i0;
	_Index[1] = i1;
	_Index[2] = i2;
}


//-----------------------------------------------------------------------------
//	Name:		FACE constructor
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
FACE::FACE(const FACE &C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		FACE destructor
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
FACE::~FACE()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
FACE& FACE::operator=(const FACE &C)
{
	Copy(C);
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-12-13:	JLT - Created
//-----------------------------------------------------------------------------
void FACE::Copy(const FACE & C)
{
	_Index[0] = C._Index[0];
	_Index[1] = C._Index[1];
	_Index[2] = C._Index[2];
	_Normal	  = C._Normal;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-05-23:	ELE - Created
//-----------------------------------------------------------------------------
void	FACE::Read(IO_STREAM &	Stream)
{
	Stream >> _Index[0] >> _Index[1] >> _Index[2];
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-05-23:	ELE - Created
//-----------------------------------------------------------------------------
void	FACE::Write(IO_STREAM &	Stream) const
{
	Stream << _Index[0];
	Stream.DebugOutput("\t");
	Stream << _Index[1];
	Stream.DebugOutput("\t");
	Stream << _Index[2];
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
