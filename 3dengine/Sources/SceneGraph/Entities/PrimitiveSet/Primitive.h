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
//	CLASS:	PRIMITIVE
//	The PRIMITIVE class implements ...
//
//	02-12-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PRIMITIVE
{
public:

							PRIMITIVE();
							PRIMITIVE(const PRIMITIVE & C);
						   ~PRIMITIVE();

	PRIMITIVE &				operator=(const PRIMITIVE & C);

private:

	COLOR					_Color;

	QDT_VECTOR <POINT3D>	_vPoints;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
