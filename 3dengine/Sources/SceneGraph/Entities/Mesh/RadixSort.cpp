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
//
//	06-02-02:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"
#include	"RadixSort.h"

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes\Includes)
#include	INCL_KMATH(Math\Math)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RADIX_SORT constructor
//	Object:		
//	06-02-06:	RMA - Created
//-----------------------------------------------------------------------------
RADIX_SORT::RADIX_SORT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~RADIX_SORT destructor
//	Object:		
//	06-02-06:	RMA - Created
//-----------------------------------------------------------------------------
RADIX_SORT::~RADIX_SORT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:		
//	06-02-06:	RMA - Created
//-----------------------------------------------------------------------------
void	RADIX_SORT::Sort(const Float32 * pInput2,
					  UInt32 uiSize)
{
	UInt32 i, j;
	UInt32 * pInput = (UInt32*)pInput2;

	if (uiSize > MAX_SIZE)
	{
		uiSize = MAX_SIZE;
	}

	// Reset indices
	for (i=0 ; i < uiSize ; i++)	
	{
		_aIndices[i] = i;
	}

	CreateHistograms(pInput2, uiSize);
	
	// Compute #negative values involved if needed
	UInt32 uiNbrNegativeValues = 0;

	// An efficient way to compute the number of negatives values we'll have to deal with is simply to sum the 128
	// last values of the last histogram. Last histogram because that's the one for the Most Significant Byte,
	// responsible for the sign. 128 last values because the 128 first ones are related to positive numbers.
	UInt32* h3= &_aHistogram[768];
	for (i = 128 ; i < 256 ; i++)	
	{
		uiNbrNegativeValues += h3[i];	// 768 for last histogram, 128 for negative part
	}

	_pIndices = &_aIndices[0];
	_pIndices2 = &_aIndices2[0];

	// Radix sort, j is the pass number (0=LSB, 3=MSB)
	for (i = 0; i < 4 ;i++)
	{
		// Should we care about negative values?
		if(i!=3)
		{
			if (CheckPassValidity(pInput, uiSize, i))
			{
				// Create offsets
				UInt32 * uiCurCount = &_aHistogram[i<<8];
				_aOffsets[0] = 0;
				for(j = 1 ; j < 256 ; j++)		
				{
					_aOffsets[j] = _aOffsets[j-1] + uiCurCount[j-1];
				}

				// Perform Radix Sort
				UInt8* pInputBytes	= (UInt8*)pInput;
				UInt32* pIndicesCur	= _pIndices;
				UInt32* pIndicesEnd	= &pIndicesCur[uiSize];
				pInputBytes += i;
				while(pIndicesCur != pIndicesEnd)
				{
					UInt32 id = *pIndicesCur++;
					_pIndices2[_aOffsets[pInputBytes[id<<2]]++] = id;
				}

				// Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
				UInt32* pTmp = _pIndices;	
				_pIndices = _pIndices2; 
				_pIndices2 = pTmp;
			}
		}
		else
		{
			if (CheckPassValidity(pInput, uiSize, i))
			{
				UInt32* puiCurCount = &_aHistogram[i<<8];
				// Create biased offsets, in order for negative numbers to be sorted as well
				_aOffsets[0] = uiNbrNegativeValues;												// First positive number takes place after the negative ones
				for(j = 1 ; j < 128 ; j++)		
				{
					// 1 to 128 for positive numbers
					_aOffsets[j] = _aOffsets[j-1] + puiCurCount[j-1];	
				}

				// We must reverse the sorting order for negative numbers!
				_aOffsets[255] = 0;

				for(j=0 ; j<127 ; j++)		
				{
					// Fixing the wrong order for negative values
					_aOffsets[254-j] = _aOffsets[255-j] + puiCurCount[255-j];	
				}
					
				for(j=128 ; j<256 ; j++)	
				{
					// Fixing the wrong place for negative values
					_aOffsets[j] += puiCurCount[j];							
				}

				// Perform Radix Sort
				for(j=0 ; j<uiSize ; j++)
				{
					// Radix byte, same as above. AND is useless here (UInt32).
					UInt32 uiRadix = pInput[_pIndices[j]]>>24;								
					// ### cmp to be killed. Not good. Later.
					if (uiRadix<128)		
					{
						// Number is positive, same as above
						_pIndices2[_aOffsets[uiRadix]++] = _pIndices[j];		
					}
					else				
					{
						// Number is negative, flip the sorting order
						_pIndices2[--_aOffsets[uiRadix]] = _pIndices[j];		
					}
				}
				// Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
				UInt32 * pTmp = _pIndices;	
				_pIndices = _pIndices2; 
				_pIndices2 = pTmp;
			}
			else
			{
				UInt8 uiUniqueVal = *(((UInt8*)pInput)+i);	

				// The pass is useless, yet we still have to reverse the order of current list if all values are negative.
				if(uiUniqueVal >= 128)
				{
					for(j=0; j < uiSize ;j++)	
					{
						_pIndices2[j] = _pIndices[uiSize-j-1];
					}

					// Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
					UInt32* pTmp = _pIndices;	
					_pIndices = _pIndices2; 
					_pIndices2 = pTmp;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CheckPassValidity
//	Object:		
//	06-02-06:	RMA - Created
//-----------------------------------------------------------------------------
Bool	RADIX_SORT::CheckPassValidity(UInt32 * pInput,
								      UInt32 uiSize,
								      UInt32 uiPass)
{
	UInt32 * pCurCount = &_aHistogram[uiPass<<8];													\
																								\
	/* Reset flag. The sorting pass is supposed to be performed. (default) */					\
	Bool bPerformPass = true;																	\
																								\
	/* Check pass validity */																	\
																								\
	/* If all values have the same byte, sorting is useless. */									\
	/* It may happen when sorting bytes or words instead of dwords. */							\
	/* This routine actually sorts words faster than dwords, and bytes */						\
	/* faster than words. Standard running time (O(4*n))is reduced to O(2*n) */					\
	/* for words and O(n) for bytes. Running time for floats depends on actual values... */		\
																								\
	/* Get first byte */																		\
	UInt8 UniqueVal = *(((UInt8*)pInput) + uiPass);													\
																								\
	/* Check that byte's counter */																\
	if(pCurCount[UniqueVal] == uiSize)	
	{
		bPerformPass = false;
	}

	return (bPerformPass);
}

//-----------------------------------------------------------------------------
//	Name:		CreateHistograms
//	Object:		
//	06-02-06:	RMA - Created
//-----------------------------------------------------------------------------
void	RADIX_SORT::CreateHistograms(const Float32 * pInput,
									 UInt32 uiSize)
{
	// Clear counters
	memset(_aHistogram, 0, 256 * 4 * sizeof(UInt32));
																																																													
	// Prepare to count																		
	UInt8* p = (UInt8 *)pInput;																	
	UInt8* pe = &p[uiSize * 4];																		
	UInt32 * h0= &_aHistogram[0];		// Histogram for first pass (LSB)						
	UInt32 * h1= &_aHistogram[256];		// Histogram for second pass							
	UInt32 * h2= &_aHistogram[512];		// Histogram for third pass								
	UInt32 * h3= &_aHistogram[768];		// Histogram for last pass (MSB)					
																																																						
	while (p!=pe)																				
	{		
		// Create histograms without the previous overhead									
		h0[*p++]++;	
		h1[*p++]++;	
		h2[*p++]++;	
		h3[*p++]++;											
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

