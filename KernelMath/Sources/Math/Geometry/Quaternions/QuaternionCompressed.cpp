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

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Quaternions/QuaternionCompressed)

#ifdef _DEBUG
#include "QuaternionCompressed.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		QUATERNION_COMPRESSED constructor
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
QUATERNION_COMPRESSED::QUATERNION_COMPRESSED()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION_COMPRESSED destructor
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
QUATERNION_COMPRESSED::~QUATERNION_COMPRESSED()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Compress
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION_COMPRESSED::Compress(const QUATERNION &	Q)
{
	QDT_ASSERT(Q.IsNormalized());

	QDT_ASSERT(-1.0f <= Q.GetX() && Q.GetX() <= 1.0f);
	QDT_ASSERT(-1.0f <= Q.GetY() && Q.GetY() <= 1.0f);
	QDT_ASSERT(-1.0f <= Q.GetZ() && Q.GetZ() <= 1.0f);
	QDT_ASSERT(-1.0f <= Q.GetW() && Q.GetW() <= 1.0f);

	float rX = (Q.GetX() + 1.0f) * 32767.0f;
	float rY = (Q.GetY() + 1.0f) * 32767.0f;
	float rZ = (Q.GetZ() + 1.0f) * 32767.0f;
	float rW = (Q.GetW() + 1.0f) * 32767.0f;

	_nX = (unsigned short)(rX);
	_nY = (unsigned short)(rY);
	_nZ = (unsigned short)(rZ);
	_nW = (unsigned short)(rW);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	04-01-23:	RMA - Created
//-----------------------------------------------------------------------------
bool	QUATERNION_COMPRESSED::operator == (const QUATERNION_COMPRESSED &	Q) const
{
	return ((_nX == Q._nX) && 
			(_nY == Q._nY) && 
			(_nZ == Q._nZ) && 
			(_nW == Q._nW));
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	04-01-23:	RMA - Created
//-----------------------------------------------------------------------------
bool	QUATERNION_COMPRESSED::operator!=(const QUATERNION_COMPRESSED &	Q) const
{
	return ((_nX != Q._nX) || 
			(_nY != Q._nY) || 
			(_nZ != Q._nZ) || 
			(_nW != Q._nW));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
