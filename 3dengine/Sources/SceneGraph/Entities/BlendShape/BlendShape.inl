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
//	CLASS:	BLEND_SHAPE
//
//	01-06-12:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
/*
//-----------------------------------------------------------------------------
//	Name:		SetBlendShapeDatas
//	Object:		
//	02-10-30:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::SetBlendShapeDatas(BLEND_SHAPE_DATAS *	pBlendShapeDatas)
{
	_pBlendShapeDatas = pBlendShapeDatas;
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetNbrWeights
//	Object:		Get the number of weights
//	01-06-27:	ELE - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE::GetNbrWeights() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return	(_pBlendShapeDatas->GetNbrTargets());
}

//-----------------------------------------------------------------------------
//	Name:		GetWeights
//	Object:		Get weights
//	01-06-27:	ELE - Created
//-----------------------------------------------------------------------------
float *	BLEND_SHAPE::GetWeights() const
{
	return	(_prWeights);
}

//-----------------------------------------------------------------------------
//	Name:		SetValid
//	Object:		
//	02-03-18:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::SetValid(bool	bValue)
{
	QDT_NOT_IMPLEMENTED();
/*
	_bIValid = bValue;

	for	(int i = 0 ; i < _nNbrGeometries ; ++i)
	{
		_pGeometriesArray[i]->SetBlendShapeValid(bValue);
	}*/
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeDatas
//	Object:		
//	03-01-16:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_DATAS *	BLEND_SHAPE::GetBlendShapeDatas() const
{
	return (_pBlendShapeDatas);
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetNumId
//	Object:		
//	04-11-23:	CVI - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::SetNumId(int	nNum)
{
	_nNumId = nNum;
}

//-----------------------------------------------------------------------------
//	Name:		GetNumId
//	Object:		
//	04-11-23:	CVI - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE::GetNumId() const
{
	return ( _nNumId );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
