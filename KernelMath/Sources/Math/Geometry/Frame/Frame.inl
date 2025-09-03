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
//	CLASS:	FRAME
//
//	02-06-10:	JLT - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		GetTranslation
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR FRAME::GetTranslation() const
{
	return _Trans;
}

//-----------------------------------------------------------------------------
//	Name:		SetTranslation
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void FRAME::SetTranslation(const VECTOR &v)
{
	_Trans = v;
}

//-----------------------------------------------------------------------------
//	Name:		GetFrameToParent
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ MATRIX	FRAME::GetFrameToParent() const
{ 
	return _fTp; 
}

//-----------------------------------------------------------------------------
//	Name:		SetFrameToParent
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
/*_INLINE_ void FRAME::SetFrameToParent(const MATRIX &	M)
{ 
	QDT_ASSERT(M.HasNoTranslation() == true);

	_fTp = M;  
	_pTf = _fTp.GetInverseRotation(); 
}*/

//-----------------------------------------------------------------------------
//	Name:		GetT
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ MATRIX FRAME::GetParentToFrame() const
{
	return _pTf;
}

//-----------------------------------------------------------------------------
//	Name:		SetParentToFrame
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
/*_INLINE_ void FRAME::SetParentToFrame(const MATRIX &M)
{ 
	QDT_ASSERT(M.HasNoTranslation() == true);
	
	_pTf = M; 
	_fTp = _pTf.GetInverseRotation(); 
}*/

//-----------------------------------------------------------------------------
//	Name:		Apply
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void FRAME::Apply(VECTOR &v) const
{	
	VECTOR w = v;
	v = _fTp.ApplyRotation(w);
	v = v + GetTranslation();
}

//-----------------------------------------------------------------------------
//	Name:		Apply
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR FRAME::Apply(const VECTOR &src) const
{	
	VECTOR v = _fTp.ApplyRotation(src);
	v = v + GetTranslation();
	return v;
}

//-----------------------------------------------------------------------------
//	Name:		GetParent
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ FRAME * FRAME::GetParent()
{
	return _pParent;
}

//-----------------------------------------------------------------------------
//	Name:		SetParent
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void FRAME::SetParent(FRAME *pParent)
{
	_pParent = pParent;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
