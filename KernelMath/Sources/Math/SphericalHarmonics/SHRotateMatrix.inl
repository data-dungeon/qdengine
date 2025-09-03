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

#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonics)

//-----------------------------------------------------------------------------
//	Name:		SHROTATE_MATRIX constructor
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
SHROTATE_MATRIX::SHROTATE_MATRIX()
{
	// Init each sub-matrix
	for (UInt32 i = 0, j = 0; i < N_BANDS; i++)
	{
		UInt32 w = i * 2 + 1;
		m_Matrices[i] = SUB_MATRIX(m_Elements + j, w);
		j += (w * w);
	}

	// The first 1x1 sub-matrix is always 1 and so doesn't need to be stored in the rotation matrix
	// It's stored simply to make the indexing logic as easy as possible
	m_Elements[0] = 1;
}

//-----------------------------------------------------------------------------
//	Name:		~SHROTATE_MATRIX destructor
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
SHROTATE_MATRIX::~SHROTATE_MATRIX()
{
}



//-----------------------------------------------------------------------------
//	Name:		operator
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32 &	SHROTATE_MATRIX::operator() (const int l, const int m, const int n)
{
		return (m_Matrices[l](m, n));
}

//-----------------------------------------------------------------------------
//	Name:		operator
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SHROTATE_MATRIX::operator() (const int l, const int m, const int n) const
{
		return (m_Matrices[l](m, n));
}


//-----------------------------------------------------------------------------
//	Name:		Kronecker_delta
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32 SHROTATE_MATRIX::Kronecker_delta(const Int32	m,
										 const Int32	n)
{
	// Kronecker Delta
	return (m == n ? 1.0f : 0.0f);
}






//=============================================================================
//	CODE ENDS HERE
//=============================================================================
