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
//	CLASS:	SORTED_PORTAL_ZONE
//
//	01-07-16:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#define	OPTI_STL

//-----------------------------------------------------------------------------
//	Name:		AddHexahedron
//	Object:		
//	03-05-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddHexahedron(HEXAHEDRON *	pHexahedron)
{
	QDT_ASSERT(pHexahedron);

#ifndef	OPTI_STL
	_vHexahedrons.PushTail(pHexahedron);
#else
	QDT_ENSURE( (_nNbrHexahedrons < MAX_HEXAHEDRONS) && "If this assert occurs, please increase MAX_HEXAHEDRONS");
	_Hexahedrons[_nNbrHexahedrons++] = pHexahedron;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrHexahedrons
//	Object:		
//	03-05-06:	ELE - Created
//-----------------------------------------------------------------------------
int	SORTED_PORTAL_ZONE::GetNbrHexahedrons() const
{
#ifndef	OPTI_STL
	return	(_vHexahedrons.GetSize());
#else
	return	(_nNbrHexahedrons);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetHexahedron
//	Object:		
//	03-05-06:	ELE - Created
//-----------------------------------------------------------------------------
HEXAHEDRON *	SORTED_PORTAL_ZONE::GetHexahedron(int n)
{
	if	(n < 0)
	{
		return	(_pHexahedron);
	}

#ifndef	OPTI_STL
	return	(_vHexahedrons[n]);
#else
	return	(_Hexahedrons[n]);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SetLevel
//	Object:		
//	01-07-17:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetLevel(int	nLevel)
{
	_nLevel = nLevel;
}

//-----------------------------------------------------------------------------
//	Name:		AddPortalNodeInst
//	Object:		
//	01-07-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddPortalNodeInst(const COM_INSTANCE & PortalNodeInst)
{
#ifndef	OPTI_STL
	_vPortalNodesInst.PushTail(PortalNodeInst);
#else
	QDT_ENSURE( (_nNbrPortals < MAX_PORTALS) && "If this assert occurs, please increase MAX_PORTALS");
	_PortalNodesInst[_nNbrPortals++] = PortalNodeInst;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AddPortalOutNodeInst
//	Object:		
//	01-07-17:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddPortalOutNodeInst(const COM_INSTANCE & PortalOutNodeInst)
{
#ifndef	OPTI_STL
	_vPortalOutNodesInst.PushTail(PortalOutNodeInst);
#else
	QDT_ENSURE( (_nNbrOutPortals < MAX_PORTALS) && "If this assert occurs, please increase MAX_PORTALS");
	_PortalOutNodesInst[_nNbrOutPortals++] = PortalOutNodeInst;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		ClearPortals
//	Object:		
//	04-04-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::ClearPortals()
{
#ifndef	OPTI_STL
	_vPortalNodesInst.Clear();
	_vPortalOutNodesInst.Clear();
#else
	_nNbrPortals = 0;
	_nNbrOutPortals = 0;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SetCullMode
//	Object:		
//	01-08-03:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetCullMode(RENDER_CONTEXT::CULL_MODE	CullMode)
{
	_CullMode = CullMode;
}

//-----------------------------------------------------------------------------
//	Name:		SetViewMatrix
//	Object:		Set the new view matrix
//	01-03-30:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetViewMatrix(const MATRIX & ViewMatrix)
{
	_NewViewMatrix = ViewMatrix;
	_InvNewViewMatrix.InverseAffine(_NewViewMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		SetFather
//	Object:		
//	01-07-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetFather(SORTED_PORTAL_ZONE *	pFather)
{
	_pFather = pFather;
}

#ifdef	OPTI_STL
//-----------------------------------------------------------------------------
//	Name:		SetNext
//	Object:		
//	05-01-04:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetNext(SORTED_PORTAL_ZONE *	pNext)
{
	_pNext = pNext;
}

//-----------------------------------------------------------------------------
//	Name:		SetNextMirrorToDisplay
//	Object:		
//	05-01-04:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetNextMirrorToDisplay(SORTED_PORTAL_ZONE *	pNextMirrorToDisplay)
{
	_pNextMirrorToDisplay = pNextMirrorToDisplay;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetLevel
//	Object:		
//	01-07-17:	ELE - Created
//-----------------------------------------------------------------------------
int	SORTED_PORTAL_ZONE::GetLevel() const
{
	return	(_nLevel);
}

//-----------------------------------------------------------------------------
//	Name:		GetPortalNodeIndex
//	Object:		
//	01-08-02:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	SORTED_PORTAL_ZONE::GetPortalNodeInst() const
{
#ifndef	OPTI_STL
	return	(_vPortalNodesInst[0]);
#else
	return	(_PortalNodesInst[0]);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetPortalOutNodeIndex
//	Object:		
//	01-08-02:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	SORTED_PORTAL_ZONE::GetPortalOutNodeInst() const
{
#ifndef	OPTI_STL
	return	(_vPortalOutNodesInst[0]);
#else
	return	(_PortalOutNodesInst[0]);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetPortalNodeInst
//	Object:		
//	04-07-13:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	SORTED_PORTAL_ZONE::GetPortalNodeInst(int	n) const
{
#ifndef	OPTI_STL
	return	(_vPortalNodesInst[n]);
#else
	return	(_PortalNodesInst[n]);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetPortalOutNodeInst
//	Object:		
//	04-07-13:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	SORTED_PORTAL_ZONE::GetPortalOutNodeInst(int	n) const
{
#ifndef	OPTI_STL
	return	(_vPortalOutNodesInst[n]);
#else
	return	(_PortalOutNodesInst[n]);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SetZoneNode
//	Object:		Set the zone which portal belongs to
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetZoneNode(NODE *pZoneNode)
{
	_pZoneNode = pZoneNode;
}

//-----------------------------------------------------------------------------
//	Name:		GetViewMatrix
//	Object:		Get the new view matrix
//	01-03-30:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	SORTED_PORTAL_ZONE::GetViewMatrix() const
{
	return	(_NewViewMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetInvViewMatrix
//	Object:		
//	03-07-17:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	SORTED_PORTAL_ZONE::GetInvViewMatrix() const
{
	return	(_InvNewViewMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayParameters
//	Object:		Get the display parameters
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
DISPLAY_PARAMETERS & SORTED_PORTAL_ZONE::GetDisplayParameters()
{
	return	(_DisplayParameters);
}

//-----------------------------------------------------------------------------
//	Name:		GetZoneNode
//	Object:		Get the zone which portal belongs to
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
NODE*	SORTED_PORTAL_ZONE::GetZoneNode() const
{
	return	(_pZoneNode);
}

//-----------------------------------------------------------------------------
//	Name:		GetFather
//	Object:		
//	01-07-24:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE *	SORTED_PORTAL_ZONE::GetFather() const
{
	return	(_pFather);
}

#ifdef	OPTI_STL
//-----------------------------------------------------------------------------
//	Name:		GetNext
//	Object:		
//	05-01-04:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE *	SORTED_PORTAL_ZONE::GetNext() const
{
	return	(_pNext);
}

//-----------------------------------------------------------------------------
//	Name:		GetNextMirrorToDisplay
//	Object:		
//	05-01-04:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE *	SORTED_PORTAL_ZONE::GetNextMirrorToDisplay() const
{
	return	(_pNextMirrorToDisplay);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetCullMode
//	Object:		
//	01-08-03:	ELE - Created
//-----------------------------------------------------------------------------
RENDER_CONTEXT::CULL_MODE	SORTED_PORTAL_ZONE::GetCullMode() const
{
	return	(_CullMode);
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	01-08-29:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE::TYPE	SORTED_PORTAL_ZONE::GetType() const
{
	return	(_Type);
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	01-08-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetType(TYPE	Type)
{
	_Type = Type;
}

//-----------------------------------------------------------------------------
//	Name:		GetPriority
//	Object:		
//	03-05-23:	RMA - Created
//-----------------------------------------------------------------------------
int	SORTED_PORTAL_ZONE::GetPriority() const
{
	return (_nPortalPriority);
}

//-----------------------------------------------------------------------------
//	Name:		SetPriority
//	Object:		
//	03-05-23:	RMA - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetPriority(int	nP)
{
	_nPortalPriority = nP;
}

//-----------------------------------------------------------------------------
//	Name:		AddFilterMirrorZone
//	Object:		
//	04-04-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddFilterMirrorZone(SORTED_PORTAL_ZONE *	pFilterMirrorZone)
{
#ifndef	OPTI_STL
	_FilterMirrorZones.PushTail(pFilterMirrorZone);
#else
	// Insertion order and display order MUST be the sames
	if	(_pFilterMirrorZonesList)
	{
		_pFilterMirrorZonesListLast->SetNext(pFilterMirrorZone);
	}
	else
	{
		_pFilterMirrorZonesList = pFilterMirrorZone;
	}

	pFilterMirrorZone->SetNext(NULL);
	_pFilterMirrorZonesListLast = pFilterMirrorZone;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AddTwinZone
//	Object:		
//	01-08-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddTwinZone(SORTED_PORTAL_ZONE *	pTwinZone)
{
#ifndef	OPTI_STL
	_TwinZones.PushTail(pTwinZone);
#else
	// Insertion order and display order MUST be the sames
	if	(_pTwinZonesList)
	{
		_pTwinZonesListLast->SetNext(pTwinZone);
	}
	else
	{
		_pTwinZonesList = pTwinZone;
	}

	pTwinZone->SetNext(NULL);
	_pTwinZonesListLast = pTwinZone;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AddMirrorToDisplay
//	Object:		
//	03-05-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddMirrorToDisplay(SORTED_PORTAL_ZONE *	pMirrorZone)
{
#ifndef	OPTI_STL
	_MirrorsToDisplay.PushTail(pMirrorZone);
#else
	// Insertion order and display order are inverted
	pMirrorZone->SetNextMirrorToDisplay(_pMirrorsToDisplayList);
	_pMirrorsToDisplayList = pMirrorZone;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		EnableVisibility
//	Object:		
//	01-09-11:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::EnableVisibility(bool	bValue)
{
	_bVisibilityTest = bValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetFrustrum
//	Object:		
//	01-09-13:	ELE - Created
//-----------------------------------------------------------------------------
const FRUSTRUM &	SORTED_PORTAL_ZONE::GetFrustrum() const
{
	return	(_Frustrum);
}

//-----------------------------------------------------------------------------
//	Name:		SetFrustrum
//	Object:		
//	01-09-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetFrustrum(const FRUSTRUM &	Frustrum)
{
	_Frustrum = Frustrum;
	_DisplayParameters.SetFrustrum(_Frustrum);
}

//-----------------------------------------------------------------------------
//	Name:		IsVisibilityEnable
//	Object:		
//	01-09-11:	ELE - Created
//-----------------------------------------------------------------------------
bool	SORTED_PORTAL_ZONE::IsVisibilityEnable() const
{
	return	(_bVisibilityTest);
}

//-----------------------------------------------------------------------------
//	Name:		SetDistance
//	Object:		
//	01-11-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetDistance(float	rDistance)
{
	_rDistance = rDistance;
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance
//	Object:		
//	01-11-15:	ELE - Created
//-----------------------------------------------------------------------------
float	SORTED_PORTAL_ZONE::GetDistance() const
{
	return	(_rDistance);
}

//-----------------------------------------------------------------------------
//	Name:		SetNearDistance
//	Object:		
//	03-03-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetNearDistance(float	rDistance)
{
	_rNearClippingDistance = rDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaskValue
//	Object:		
//	03-05-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::SetMaskValue(int	n)
{
	_nMaskValue = n;
}

//-----------------------------------------------------------------------------
//	Name:		HasHDRMesh
//	Object:		
//	03-12-04:	SBE - Created
//-----------------------------------------------------------------------------
bool	SORTED_PORTAL_ZONE::HasHDRMesh() const
{
	return (_vHDRObjects.GetSize() != 0);
}

//-----------------------------------------------------------------------------
//	Name:		PushTransparentObject
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::PushTransparentObject(const TRANSPARENT_OBJECT &	TransObj)
{
	_vTransparentObjects.PushTail(TransObj);
}

//-----------------------------------------------------------------------------
//	Name:		PushSortedTransparentObject
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::PushSortedTransparentObject(const TRANSPARENT_OBJECT &	TransObj)
{
	_dlSortedTransparentObjects.PushTail(TransObj);
}
//-----------------------------------------------------------------------------
//	Name:		ResetHDR
//	Object:		
//	04-05-11:	SBE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::ResetHDR()
{
	_vHDRObjects.Clear();
	RENDER_CONTEXT* pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_PIPELINE_ABC		*pRenderPipeline = pContext->GetRenderPipeline();
	pRenderPipeline->ResetHdrTriangles();
}

//-----------------------------------------------------------------------------
//	Name:		PushHdrObject
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::PushHdrObject(const HDR_OBJECT &	HdrObj)
{
	_vHDRObjects.PushTail(HdrObj);
}

//-----------------------------------------------------------------------------
//	Name:		PushMaterial
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::PushMaterial(MATERIAL_ABC *	pMaterial)
{
	_vSortedMaterials.PushTail(pMaterial);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
