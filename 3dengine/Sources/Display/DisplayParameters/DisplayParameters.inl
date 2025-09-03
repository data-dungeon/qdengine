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
// FILE: DisplayParameters.inl
//
//
//	01-02-13:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	01-07-05:	RMA - Created
//-----------------------------------------------------------------------------
NODE *	DISPLAY_PARAMETERS::GetNode() const
{
	return (_pNode);
}

//-----------------------------------------------------------------------------
//	Name:		SetNode
//	Object:		
//	01-07-05:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetNode(NODE *	pNode)
{
	_pNode = pNode;
}

//-----------------------------------------------------------------------------
//	Name:		GetFrustum
//	Object:		
//	01-07-09:	RMA - Created
//-----------------------------------------------------------------------------
const FRUSTRUM &	DISPLAY_PARAMETERS::GetFrustrum() const
{
	return (_CameraFrustrum);
}

//-----------------------------------------------------------------------------
//	Name:		SetFrustum
//	Object:		
//	01-07-09:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetFrustrum(const FRUSTRUM & Frustrum)
{
	_CameraFrustrum = Frustrum;
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingVolume
//	Object:		
//	03-06-12:	SBE - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME *	DISPLAY_PARAMETERS::GetBoundingVolume() const
{
	return _pBoundingVolume;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingVolume
//	Object:		
//	03-06-12:	SBE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetBoundingVolume(BOUNDING_VOLUME * pBoundingVolume)
{
	_pBoundingVolume = pBoundingVolume;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentSG
//	Object:		
//	04-07-21:	RMA - Created
//-----------------------------------------------------------------------------
SCENE_GRAPH *	DISPLAY_PARAMETERS::GetCurrentSG() const
{
	return (_pCurrentSG);
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentSG
//	Object:		
//	04-07-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetCurrentSG(SCENE_GRAPH *	pSG)
{
	_pCurrentSG = pSG;
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleSidedMaterial
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetDoubleSidedMaterial(bool	b)
{
	_bDoubleSidedMaterial = b;
}

//-----------------------------------------------------------------------------
//	Name:		IsDoubleSidedMaterial
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_PARAMETERS::IsDoubleSidedMaterial() const
{
	return (_bDoubleSidedMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		IsIgnoreGlobalMatrix
//	Object:		
//	04-08-25:	SBE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_PARAMETERS::IsIgnoreGlobalMatrix() const
{
	return (_bIgnoreGlobalMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		IsSkeletonInside
//	Object:		
//	04-08-30:	SBE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_PARAMETERS::IsSkeletonInside() const
{
	return (_bIsSkeletonInside);
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinningMatrices
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetSkinningMatrices(QDT_VECTOR < MATRIX > *	pSkinningMatrices)
{
	_pSkinningMatrices = pSkinningMatrices;
}

//-----------------------------------------------------------------------------
//	Name:		SetDynamicClusters
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetDynamicClusters(QDT_VECTOR < DYNAMIC_CLUSTER * > *	pDynamicClusters)
{
	_pDynamicClusters = pDynamicClusters;
}

//-----------------------------------------------------------------------------
//	Name:		GetSkinningMatrices
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < MATRIX > *	DISPLAY_PARAMETERS::GetSkinningMatrices() const
{
	return (_pSkinningMatrices);
}

//-----------------------------------------------------------------------------
//	Name:		GetDynamicClusters
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < DYNAMIC_CLUSTER * > *	DISPLAY_PARAMETERS::GetDynamicClusters() const
{
	return (_pDynamicClusters);
}

//-----------------------------------------------------------------------------
//	Name:		GetLightNodesOnDynamicLit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < COM_INSTANCE > *	DISPLAY_PARAMETERS::GetLightNodesOnDynamicLit() const
{
	return (_pLightNodesOnDynamicLit);
}

//-----------------------------------------------------------------------------
//	Name:		GetLightNodesOnPrelit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < COM_INSTANCE > *	DISPLAY_PARAMETERS::GetLightNodesOnPrelit() const
{
	return (_pLightNodesOnPrelit);
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetLightSourcesOnDynamicLit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < LIGHT_SOURCE > *	DISPLAY_PARAMETERS::GetLightSourcesOnDynamicLit() const
{
	return (_pLightSourcesOnDynamicLit);
}

//-----------------------------------------------------------------------------
//	Name:		GetLightSourcesOnPrelit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < LIGHT_SOURCE > *	DISPLAY_PARAMETERS::GetLightSourcesOnPrelit() const
{
	return (_pLightSourcesOnPrelit);
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetLightNodesOnDynamicLit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetLightNodesOnDynamicLit(QDT_VECTOR < COM_INSTANCE > *	pLightNodesOnDynamicLit)
{
	_pLightNodesOnDynamicLit = pLightNodesOnDynamicLit;
}

//-----------------------------------------------------------------------------
//	Name:		SetLightNodesOnPrelit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetLightNodesOnPrelit(QDT_VECTOR < COM_INSTANCE > *	pLightNodesOnPrelit)
{
	_pLightNodesOnPrelit = pLightNodesOnPrelit;
}
/*
//-----------------------------------------------------------------------------
//	Name:		SetLightSourcesOnDynamicLit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetLightSourcesOnDynamicLit(QDT_VECTOR < LIGHT_SOURCE > *	pLightSourcesOnDynamicLit)
{
	_pLightSourcesOnDynamicLit = pLightSourcesOnDynamicLit;
}

//-----------------------------------------------------------------------------
//	Name:		SetLightSourcesOnPrelit
//	Object:		
//	04-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetLightSourcesOnPrelit(QDT_VECTOR < LIGHT_SOURCE > *	pLightSourcesOnPrelit)
{
	_pLightSourcesOnPrelit = pLightSourcesOnPrelit;
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetIgnoreGlobalMatrix
//	Object:		
//	04-08-25:	SBE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetIgnoreGlobalMatrix(bool	b)
{
	_bIgnoreGlobalMatrix = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetSkeletonInside
//	Object:		
//	04-08-30:	SBE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_PARAMETERS::SetSkeletonInside(bool	b)
{
	_bIsSkeletonInside = b;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
