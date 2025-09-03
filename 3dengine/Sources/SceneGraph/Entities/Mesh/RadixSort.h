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
//	CLASS:	RADIX_SORT
//	The RADIX_SORT class implements ...
//
//	06-02-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#define MAX_SIZE 65536

class	RADIX_SORT
{
public:

	RADIX_SORT();
	~RADIX_SORT();

	void Sort(const Float32 * pInput, UInt32 uiSize);
	
	const UInt32 * GetIndices() const { return (_pIndices); }

private:

	void	CreateHistograms(const Float32 * pInput, UInt32 uiSize);
	Bool	CheckPassValidity(UInt32 * pInput, UInt32 uiSize, UInt32 uiPass);

	UInt32 *	_pIndices;
	UInt32 *	_pIndices2;
	UInt32 		_aIndices[MAX_SIZE];
	UInt32		_aIndices2[MAX_SIZE];
	UInt32		_aHistogram[256 * 4];
	UInt32		_aOffsets[256];
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
