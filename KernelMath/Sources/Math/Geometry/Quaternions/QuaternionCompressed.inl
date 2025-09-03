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
//	CLASS:	QUATERNION_COMPRESSED
//
//	04-01-27:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

static float rConversion = 1.0f / 32767.0f;

//-----------------------------------------------------------------------------
//	Name:		Uncompress
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION_COMPRESSED::Uncompress(QUATERNION &	Q) const
{
	float rX = ((float)_nX) * rConversion;
	float rY = ((float)_nY) * rConversion;
	float rZ = ((float)_nZ) * rConversion;
	float rW = ((float)_nW) * rConversion;

	Q.SetX(rX - 1.0f);
	Q.SetY(rY - 1.0f);
	Q.SetZ(rZ - 1.0f);
	Q.SetW(rW - 1.0f);

	QDT_ASSERT(Q.IsNormalized());

	QDT_ASSERT(-1.0f <= Q.GetX() && Q.GetX() <= 1.0f);
	QDT_ASSERT(-1.0f <= Q.GetY() && Q.GetY() <= 1.0f);
	QDT_ASSERT(-1.0f <= Q.GetZ() && Q.GetZ() <= 1.0f);
	QDT_ASSERT(-1.0f <= Q.GetW() && Q.GetW() <= 1.0f);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
