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
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\Include.h"

#include	INC_KRNCOM(OldCom/Database)
#include	INC_KRNCORE(Display\RenderPipeline)
#include	INC_KRNCORE(Time)
#include	INC_KRNCORE(Profiler)

#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes2D/Plane)
#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes2D/OBR)
#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes3D/Hexahedron)

#include	INC_3DENGINE(SceneGraph\Entities\Portal)
#include	INC_3DENGINE(SceneGraph\Entities\Particles)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)
#include	INC_3DENGINE(SceneGraph\Entities\Zone)
#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(SceneGraph\Node)

#include	INC_3DENGINE(Display\Viewport)

#ifdef _DEBUG
	#include "SortedPortalZone.inl"
#else
	#include	"../Inline.h"

	#include	INL_3DENGINE(SceneGraph/Entities/Portal)
	#include	INL_3DENGINE(SceneGraph/Node)
	#include	INL_3DENGINE(SceneGraph/Entities/Zone)
	#include	INL_3DENGINE(SceneGraph/Entities)

	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Display/RenderPipeline)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes3D/Frustrum)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Plane)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SORTED_PORTAL_ZONE constructor
//	Object:		Default constructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE::SORTED_PORTAL_ZONE()
:
_nMaskValue					(0),
_pZoneNode					(NULL),
_pFather					(NULL),
_Type						(T_CAMERA),
_pMirroredFrustrum			(NULL),
_pHexahedron				(NULL),
_bVisibilityTest			(true),
#ifdef	OPTI_STL
_nNbrPortals				(0),
_nNbrOutPortals				(0),
_pMirrorZonesList			(NULL),
_pFilterMirrorZonesList		(NULL),
_pFilterMirrorZonesListLast	(NULL),
_pTwinZonesList				(NULL),
_pTwinZonesListLast			(NULL),
_pNext						(NULL),
_pMirrorsToDisplayList		(NULL),
_pNextMirrorToDisplay		(NULL),
_nNbrHexahedrons			(0),
#endif
_rDistance					(0.0f),
_rNearClippingDistance		(0.0f),
_nPortalPriority			(0)
{
	_NewViewMatrix.Identity();
	_InvNewViewMatrix.Identity();
}

//-----------------------------------------------------------------------------
//	Name:		SORTED_PORTAL_ZONE destructor
//	Object:		Destructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE::~SORTED_PORTAL_ZONE()
{
	delete	_pMirroredFrustrum;
	delete	_pHexahedron;
	
	ClearPortals();

#ifndef	OPTI_STL
	_vHexahedrons.Clear();
#else
	_nNbrHexahedrons = 0;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	05-01-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::Reset()
{
	ClearPortals();

#ifndef	OPTI_STL
	_vHexahedrons.Clear();

	_TwinZones.Clear();
	_MirrorZones.Clear();
	_FilterMirrorZones.Clear();
#else
	_nNbrHexahedrons = 0;

	_pTwinZonesList = NULL;
	_pTwinZonesListLast = NULL;
	_pMirrorZonesList = NULL;
	_pFilterMirrorZonesList = NULL;
	_pFilterMirrorZonesListLast = NULL;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplayAsCamera
//	Object:		
//	01-08-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayAsCamera(const VIEWPORT *	pViewport)
{
	if	(_pZoneNode)
	{
		QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It, ItEnd;
		RENDER_CONTEXT								*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
		DISPLAY_MANAGER								*pDisplay = DISPLAY_MANAGER::Instance();
		COLOR										Color(0, 0, 0, 0);
		SORTED_PORTAL_ZONE							*pZone;

		pDisplay->PushMirrorZone(this);

		// Display all the mirror zones
#ifndef	OPTI_STL
		It = _TwinZones.GetHead();
		ItEnd = _TwinZones.GetTail();

		while	(It != ItEnd)
		{
			(*It)->DisplayMirrorZones(pViewport);

			++It;
		}
#else
		pZone = _pTwinZonesList;
		while	(pZone)
		{
			pZone->DisplayMirrorZones(pViewport);
			pZone = pZone->GetNext();
		}
#endif

#ifndef	OPTI_STL
		It = _MirrorZones.GetHead();
		ItEnd = _MirrorZones.GetTail();

		while	(It != ItEnd)
		{
			(*It)->DisplayAsMirror(pViewport);

			++It;
		}
#else
		pZone = _pMirrorZonesList;
		while	(pZone)
		{
			pZone->DisplayAsMirror(pViewport);
			pZone = pZone->GetNext();
		}
#endif

		pDisplay->PopMirrorZone();

#ifndef	OPTI_STL
		if	(_MirrorsToDisplay.GetSize())
#else
		if	(_pMirrorsToDisplayList)
#endif
		{
			pContext->ClearBackground(RENDER_CONTEXT::CLEARBACKGROUND_DEPTHBUFFER | RENDER_CONTEXT::CLEARBACKGROUND_STENCILBUFFER, Color, 1.0f, 0.0f);

			// Display mirrors so that no futher render could corupt reflexions
			DisplayChildrenMirrors();
		}

		pDisplay->SetOnlyDynamicsReflexions(false);
#ifndef	OPTI_STL
		pDisplay->SetDisplayMirrors(_FilterMirrorZones.GetSize() > 0);
#else
		pDisplay->SetDisplayMirrors(_pFilterMirrorZonesList != NULL);
#endif

		DisplayScene(pViewport);

		// Clear
		ClearPortals();

#ifndef	OPTI_STL
		_TwinZones.Clear();
		_MirrorZones.Clear();
		_FilterMirrorZones.Clear();
#else
		_pTwinZonesList = NULL;
		_pTwinZonesListLast = NULL;
		_pMirrorZonesList = NULL;
		_pFilterMirrorZonesList = NULL;
		_pFilterMirrorZonesListLast = NULL;
#endif

#ifndef	OPTI_STL
		_vHexahedrons.Clear();
#else
		_nNbrHexahedrons = 0;
#endif

		pDisplay->ResetHex();
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayAsCameraHDR
//	Object:		
//	03-10-08:	RMA - Created
//-----------------------------------------------------------------------------
void SORTED_PORTAL_ZONE::DisplayHDR(const VIEWPORT *pViewport)
{
//	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It;
//	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR  ItEnd;
	RENDER_CONTEXT* pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	unsigned int i;

	pContext->SetViewTransform(&_InvNewViewMatrix);
	
	for (i = 0 ; i < _vHDRObjects.GetSize() ; ++i)
	{
		_vHDRObjects[i].Display(_DisplayParameters);
	}

	RENDER_PIPELINE_ABC		*pRenderPipeline = pContext->GetRenderPipeline();
	pRenderPipeline->RenderHdrTriangles(true);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayAsMirror
//	Object:		
//	01-09-03:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayAsMirror(const VIEWPORT *	pViewport)
{
	if	(_pZoneNode)
	{
		NODE										*pPortalNode = NULL;
		PORTAL_ABC									*pPortal = NULL;
		QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It, ItEnd;
		SORTED_PORTAL_ZONE							*pFatherMirror;					
		RENDER_CONTEXT								*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
		RENDER_CONTEXT::TEST_FUNC					DepthTestFunc;
		DISPLAY_MANAGER								*pDisplay = DISPLAY_MANAGER::Instance();
		SORTED_PORTAL_ZONE							*pZone;

#ifdef	PLATFORM_PS2
		bool	bShadowsEnabled = pDisplay->AreShadowsEnabled();
		pDisplay->EnableShadows(false);
#endif

		pDisplay->PushMirrorZone(this);

		// Add hexahedrons for recursif visibility test
		pDisplay->AddHexahedronLevel(_Type);

		pDisplay->AddHexahedron(_pHexahedron);

		pDisplay->AddMirroredFrustrum(_pMirroredFrustrum);

		// Display mirror as mask
		DisplayMask(_pFather->GetViewMatrix());

		// Display all the mirror zones
#ifndef	OPTI_STL
		It = _TwinZones.GetHead();
		ItEnd = _TwinZones.GetTail();

		while	(It != ItEnd)
		{
			(*It)->DisplayMirrorZones(pViewport);

			++It;
		}
#else
		pZone = _pTwinZonesList;
		while	(pZone)
		{
			pZone->DisplayMirrorZones(pViewport);
			pZone = pZone->GetNext();
		}
#endif

#ifndef	OPTI_STL
		It = _MirrorZones.GetHead();
		ItEnd = _MirrorZones.GetTail();

		while	(It != ItEnd)
		{
			(*It)->DisplayAsMirror(pViewport);

			++It;
		}
#else
		pZone = _pMirrorZonesList;
		while	(pZone)
		{
			pZone->DisplayAsMirror(pViewport);
			pZone = pZone->GetNext();
		}
#endif

		pDisplay->PopMirrorZone();

		pFatherMirror = pDisplay->GetLastMirrorZone();
		pFatherMirror->AddMirrorToDisplay(this);

		pContext->ClearBackground(RENDER_CONTEXT::CLEARBACKGROUND_DEPTHBUFFER, pViewport->GetBackgroundColor(), 0.0f, 0.0f);

		DepthTestFunc = pContext->GetDepthTestFunc();
		pContext->SetDepthTestFunc(RENDER_CONTEXT::TESTFUNC_ALWAYS);
		DisplayZ(_pFather->GetViewMatrix());
		pContext->SetDepthTestFunc(DepthTestFunc);

		// Display mirrors so that no futher render could corupt reflexions
		DisplayChildrenMirrors();

		pDisplay->SetOnlyDynamicsReflexions(false);
		pDisplay->SetDisplayMirrors(false);
	
		// Near clip plane test
		pPortalNode = static_cast<NODE *>(GetPortalNodeInst().GetCommunicator());
		pPortal = static_cast<PORTAL_ABC *>(pPortalNode->GetEntity());
		pPortal->Reset();

//		MATRIX	Proj = pContext->ModifyProjectionMatrix(pPortal->GetPlane(), _InvNewViewMatrix);
		DisplayScene(pViewport);
//		pContext->SetPerspectiveTransform(&Proj);

		// Clear
		pDisplay->RemoveHexahedronLevel(_Type);
		pDisplay->RemoveMirroredFrustrum();

#ifndef	OPTI_STL
		_TwinZones.Clear();
		_MirrorZones.Clear();
#else
		_pTwinZonesList = NULL;
		_pTwinZonesListLast = NULL;
		_pMirrorZonesList = NULL;
#endif

#ifdef	PLATFORM_PS2
		pDisplay->EnableShadows(bShadowsEnabled);
#endif
		pDisplay->ResetSingleDisplayNodes();
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayAsFilterMirror
//	Object:		
//	04-04-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayAsFilterMirror(const VIEWPORT *	pViewport)
{
	if	(_pZoneNode)
	{
		NODE			*pPortalNode = NULL;
		PORTAL_ABC		*pPortal = NULL;				
		DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();

		// Add hexahedrons for recursif visibility test
		pDisplay->AddHexahedronLevel(_Type);

		pDisplay->AddHexahedron(_pHexahedron);

		pDisplay->AddMirroredFrustrum(_pMirroredFrustrum);

		pDisplay->SetOnlyDynamicsReflexions(true);
		pDisplay->SetDisplayMirrors(false);

		DisplayScene(pViewport);

		// Clear
		pDisplay->RemoveHexahedronLevel(_Type);
		pDisplay->RemoveMirroredFrustrum();

		pPortalNode = static_cast<NODE *>(GetPortalNodeInst().GetCommunicator());
		pPortal = static_cast<PORTAL_ABC *>(pPortalNode->GetEntity());
		pPortal->Reset();

		ClearPortals();
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayAsTwin
//	Object:		
//	01-09-03:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayAsTwin(const VIEWPORT *	pViewport)
{
	if	(_pZoneNode)
	{
		int											i, nNbrHexahedrons;
		NODE										*pPortalNode = NULL;
		PORTAL_ABC									*pPortal = NULL;
		DISPLAY_MANAGER								*pDisplay = DISPLAY_MANAGER::Instance();
		QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It, ItEnd;

		// Add hexahedrons for recursif visibility test
		pDisplay->AddHexahedronLevel(_Type);

		pDisplay->AddHexahedron(_pHexahedron);

		nNbrHexahedrons = GetNbrHexahedrons();
		for	(i = 0 ; i < nNbrHexahedrons ; ++i)
		{
			pDisplay->AddHexahedron(GetHexahedron(i));
		}

		pDisplay->SetOnlyDynamicsReflexions(false);
		pDisplay->SetDisplayMirrors(false);

		DisplayScene(pViewport);

		// Clear
		pDisplay->RemoveHexahedronLevel(_Type);

#ifndef	OPTI_STL
		int	j, nNbrPortals = _vPortalNodesInst.GetSize();
		for	(j = 0 ; j < nNbrPortals ; ++j)
#else
		unsigned int	j;
		for	(j = 0 ; j < _nNbrPortals ; ++j)
#endif
		{
			pPortalNode = static_cast<NODE *>(GetPortalNodeInst(j).GetCommunicator());
			pPortal = static_cast<PORTAL_ABC *>(pPortalNode->GetEntity());
			pPortal->Reset();
		}

		ClearPortals();

#ifndef	OPTI_STL
		_TwinZones.Clear();
		_MirrorZones.Clear();
#else
		_pTwinZonesList = NULL;
		_pTwinZonesListLast = NULL;
		_pMirrorZonesList = NULL;
#endif

#ifndef	OPTI_STL
		_vHexahedrons.Clear();
#else
		_nNbrHexahedrons = 0;
#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayMirrorZones
//	Object:		
//	01-09-03:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayMirrorZones(const VIEWPORT *	pViewport)
{
	int											i, nNbrHexahedrons;
	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It, ItEnd;
	DISPLAY_MANAGER								*pDisplay = DISPLAY_MANAGER::Instance();
	SORTED_PORTAL_ZONE							*pZone;

	// Add hexahedrons for recursif visibility test
	pDisplay->AddHexahedronLevel(_Type);

	pDisplay->AddHexahedron(_pHexahedron);

	nNbrHexahedrons = GetNbrHexahedrons();
	for	(i = 0 ; i < nNbrHexahedrons ; ++i)
	{
		pDisplay->AddHexahedron(GetHexahedron(i));
	}

	pDisplay->AddMirroredFrustrum(_pMirroredFrustrum);

	// Display all the mirror zones
#ifndef	OPTI_STL
	It = _TwinZones.GetHead();
	ItEnd = _TwinZones.GetTail();

	while	(It != ItEnd)
	{
		(*It)->DisplayMirrorZones(pViewport);

		++It;
	}
#else
	pZone = _pTwinZonesList;
	while	(pZone)
	{
		pZone->DisplayMirrorZones(pViewport);
		pZone = pZone->GetNext();
	}
#endif

#ifndef	OPTI_STL
	It = _MirrorZones.GetHead();
	ItEnd = _MirrorZones.GetTail();

	while	(It != ItEnd)
	{
		(*It)->DisplayAsMirror(pViewport);

		++It;
	}
#else
	pZone = _pMirrorZonesList;
	while	(pZone)
	{
		pZone->DisplayAsMirror(pViewport);
		pZone = pZone->GetNext();
	}
#endif

	pDisplay->RemoveHexahedronLevel(_Type);
	pDisplay->RemoveMirroredFrustrum();
}

//-----------------------------------------------------------------------------
//	Name:		DisplayTwinZones
//	Object:		
//	01-09-03:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayTwinZones(const VIEWPORT *	pViewport)
{
	int											i, nNbrHexahedrons;
	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It, ItEnd;
	DISPLAY_MANAGER								*pDisplay = DISPLAY_MANAGER::Instance();
	SORTED_PORTAL_ZONE							*pZone;

	// Add hexehedrons for recursif visibility test
	pDisplay->AddHexahedronLevel(_Type);

	pDisplay->AddHexahedron(_pHexahedron);

	nNbrHexahedrons = GetNbrHexahedrons();
	for	(i = 0 ; i < nNbrHexahedrons ; ++i)
	{
		pDisplay->AddHexahedron(GetHexahedron(i));
	}

	// Display all the twin zones
#ifndef	OPTI_STL
	It = _TwinZones.GetHead();
	ItEnd = _TwinZones.GetTail();

	while	(It != ItEnd)
	{
		(*It)->DisplayAsTwin(pViewport);

		++It;
	}
#else
	pZone = _pTwinZonesList;
	while	(pZone)
	{
		pZone->DisplayAsTwin(pViewport);
		pZone = pZone->GetNext();
	}
#endif

#ifndef	OPTI_STL
	It = _MirrorZones.GetHead();
	ItEnd = _MirrorZones.GetTail();

	while	(It != ItEnd)
	{
		(*It)->DisplayTwinZones(pViewport);

		++It;
	}
#else
	pZone = _pMirrorZonesList;
	while	(pZone)
	{
		pZone->DisplayTwinZones(pViewport);
		pZone = pZone->GetNext();
	}
#endif

	pDisplay->RemoveHexahedronLevel(_Type);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayZ
//	Object:		Display the geometry of the portal in z-buffer
//	01-07-17:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayZ(const MATRIX &		ViewMatrix)
{
	NODE		*pPortalNode;
	PORTAL_ABC	*pPortal;

#ifndef	OPTI_STL
	int	i, nNbrPortals = _vPortalNodesInst.GetSize();
	for	(i = 0 ; i < nNbrPortals ; ++i)
#else
	unsigned int	i;
	for	(i = 0 ; i < _nNbrPortals ; ++i)
#endif
	{
		pPortalNode = static_cast<NODE *>(GetPortalNodeInst(i).GetCommunicator());
		pPortal = static_cast<PORTAL_ABC *>(pPortalNode->GetEntity());
		pPortal->DisplayZ(pPortalNode->GetGlobalMatrix(), ViewMatrix, _nMaskValue);
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayN
//	Object:		
//	01-10-17:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayN(const MATRIX &	ViewMatrix)
{
	NODE		*pPortalNode;
	PORTAL_ABC	*pPortal;

#ifndef	OPTI_STL
	int	i, nNbrPortals = _vPortalNodesInst.GetSize();
	for	(i = 0 ; i < nNbrPortals ; ++i)
#else
	unsigned int	i;
	for	(i = 0 ; i < _nNbrPortals ; ++i)
#endif
	{
		pPortalNode = static_cast<NODE *>(GetPortalNodeInst(i).GetCommunicator());
		pPortal = static_cast<PORTAL_ABC *>(pPortalNode->GetEntity());
		pPortal->DisplayN(pPortalNode->GetGlobalMatrix(), ViewMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayMask
//	Object:		
//	03-05-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayMask(const MATRIX &	ViewMatrix)
{
	NODE		*pPortalNode;
	PORTAL_ABC	*pPortal;

#ifndef	OPTI_STL
	int	i, nNbrPortals = _vPortalNodesInst.GetSize();
	for	(i = 0 ; i < nNbrPortals ; ++i)
#else
	unsigned int	i;
	for	(i = 0 ; i < _nNbrPortals ; ++i)
#endif
	{
		pPortalNode = static_cast<NODE *>(GetPortalNodeInst(i).GetCommunicator());
		pPortal = static_cast<PORTAL_ABC *>(pPortalNode->GetEntity());
		pPortal->DisplayMask(pPortalNode->GetGlobalMatrix(), ViewMatrix, _nLevel, _nMaskValue);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BuildHexahedron
//	Object:		
//	01-10-09:	ELE - Created
//-----------------------------------------------------------------------------
HEXAHEDRON *	SORTED_PORTAL_ZONE::BuildHexahedron()
{
	NODE		*pPortalOutNode = NULL;
	PORTAL_ABC	*pPortalOut = NULL;
	PLANE		Plane[4];
	PLANE		NearPlane, FarPlane;
	OBR			GlobalOBR;

#ifndef	OPTI_STL
	if	(_vPortalOutNodesInst.GetSize() > 0)
#else
	if	(_nNbrOutPortals)
#endif
	{
		pPortalOutNode = static_cast<NODE *>(GetPortalOutNodeInst().GetCommunicator());
	}
	else
	{
		pPortalOutNode = static_cast<NODE *>(GetPortalNodeInst().GetCommunicator());
	}

	pPortalOut = static_cast<PORTAL_ABC *>(pPortalOutNode->GetEntity());

	GlobalOBR = *(pPortalOut->GetOBR());
	GlobalOBR.Transform(pPortalOutNode->GetGlobalMatrix());
	VECTOR	NewViewMatrixPosition;
	NewViewMatrixPosition = _NewViewMatrix.GetTranslation();
	GlobalOBR.GeneratePlanes(NewViewMatrixPosition, Plane);

	CAMERA	*pCamera = DISPLAY_MANAGER::Instance()->GetActiveCamera();

	NearPlane = pPortalOut->GetPlane();
	NearPlane.SetD(pCamera->GetNearClippingDistance());
	NearPlane.Transform(pPortalOutNode->GetGlobalMatrix());

	FarPlane = pPortalOut->GetPlane();
	FarPlane.SetCoeff(-FarPlane.GetNormal().GetX(), -FarPlane.GetNormal().GetY(), -FarPlane.GetNormal().GetZ(), -pCamera->GetFarClippingDistance());
	FarPlane.Transform(pPortalOutNode->GetGlobalMatrix());

	if (_pHexahedron)
		_pHexahedron->SetPlanes(NearPlane, FarPlane, Plane[0], Plane[1], Plane[2], Plane[3]);
	else
		_pHexahedron = new HEXAHEDRON(NearPlane, FarPlane, Plane[0], Plane[1], Plane[2], Plane[3]);

	return	(_pHexahedron);
}

//-----------------------------------------------------------------------------
//	Name:		BuildMirroredFrustrum
//	Object:		
//	03-02-21:	ELE - Created
//-----------------------------------------------------------------------------
FRUSTRUM *	SORTED_PORTAL_ZONE::BuildMirroredFrustrum()
{
	if	(_Type == T_MIRROR)
	{
		CAMERA	*pAC = DISPLAY_MANAGER::Instance()->GetActiveCamera();

		if	(_pMirroredFrustrum == NULL)
		{
			_pMirroredFrustrum = new FRUSTRUM(pAC->GetFocalAngle(), pAC->GetNearClippingDistance(), pAC->GetFarClippingDistance(), pAC->GetAspectRatio());
		}
		else
		{
			_pMirroredFrustrum->Set(pAC->GetFocalAngle(), pAC->GetNearClippingDistance(), pAC->GetFarClippingDistance(), pAC->GetAspectRatio());
		}

		_pMirroredFrustrum->Transform(_NewViewMatrix);

		return	(_pMirroredFrustrum);
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		Has
//	Object:		
//	03-05-06:	ELE - Created
//-----------------------------------------------------------------------------
bool	SORTED_PORTAL_ZONE::Has(PORTAL_ABC *	pPortal)
{
#ifndef	OPTI_STL
	int	nNbrOutPortals = _vPortalOutNodesInst.GetSize();

	if	(nNbrOutPortals > 0)
#else
	if	(_nNbrOutPortals > 0)
#endif
	{
		NODE		*pPortalOutNode;
		PORTAL_ABC	*pPortalOut;

#ifndef	OPTI_STL
		int	i;
		for	(i = 0 ; i < nNbrOutPortals ; ++i)
#else
		unsigned int	i;
		for	(i = 0 ; i < _nNbrOutPortals ; ++i)
#endif
		{
			pPortalOutNode = static_cast<NODE *>(GetPortalOutNodeInst(i).GetCommunicator());
			pPortalOut = static_cast<PORTAL_ABC *>(pPortalOutNode->GetEntity());

			if	(pPortalOut == pPortal)
			{
				return	(true);
			}
		}
	}

	return	(false);
}
/*
//-----------------------------------------------------------------------------
//	Name:		DisplayMirrors
//	Object:		
//	01-11-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayMirrors()
{
	int											i, nNbrMirrors;
	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It;

	// Display all the mirror zones
	It = _MirrorZones.GetTail() - 1;
	nNbrMirrors = _MirrorZones.GetSize();

	i = 0;
	while	(i < nNbrMirrors)
	{
		(*It)->DisplayN(_NewViewMatrix);
		(*It)->ClearPortals();

		if	((i + 1) != nNbrMirrors)	--It;

		++i;
	}

	_MirrorZones.Clear();
}
*/
//-----------------------------------------------------------------------------
//	Name:		DisplayChildrenMirrors
//	Object:		
//	03-05-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayChildrenMirrors()
{
#ifndef	OPTI_STL

	int	i, nNbrMirros;

	RENDER_MANAGER::Instance()->GetRenderContext()->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);

	nNbrMirros = _MirrorsToDisplay.GetSize();
	for	(i = nNbrMirros - 1 ; i >= 0 ; --i)
	{
		_MirrorsToDisplay[i]->DisplayN(_NewViewMatrix);
		_MirrorsToDisplay[i]->ClearPortals();
	}

	_MirrorsToDisplay.Clear();

#else

	SORTED_PORTAL_ZONE	*pZone;

	RENDER_MANAGER::Instance()->GetRenderContext()->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);
	
	pZone = _pMirrorsToDisplayList;
	while	(pZone)
	{
		pZone->DisplayN(_NewViewMatrix);
		pZone->ClearPortals();

		pZone = pZone->GetNextMirrorToDisplay();
	}

	_pMirrorsToDisplayList = NULL;

#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplayScene
//	Object:		
//	02-04-22:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayScene(const VIEWPORT *	pViewport)
{
	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It, ItEnd;
	DISPLAY_MANAGER								*pDisplay = DISPLAY_MANAGER::Instance();
	RENDER_CONTEXT								*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_PIPELINE_ABC							*pRenderPipeline = pContext->GetRenderPipeline();
	VECTOR										CameraPos;
	VECTOR										SkyPos(0.0f, 0.0f, 0.0f);
	float										rOldFar;
	bool										bFog;
	SORTED_PORTAL_ZONE							*pSortedZone;

	// Display current zone
	pContext->SetViewTransform(&_InvNewViewMatrix);
	DISPLAY_MANAGER::Instance()->SetCurrentPortalZone(this);
	pRenderPipeline->AddTransList();
	pRenderPipeline->AddHdrList();

	// If we are in fog, far clipping distance is set to fog far
	NODE *	 pZoneNode = GetZoneNode();
	ZONE *	 pZone = (ZONE *)(pZoneNode->GetEntity());

	_DisplayParameters.SetFrustrum(_Frustrum);
	_DisplayParameters.SetCurrentZone(pZone);

	pContext->SetCullMode(_CullMode);

	float	rDistance = 0.0f;
	if	(_Type == T_TWIN)
	{
		ZONE	*pFatherZone = static_cast<ZONE *>(_pFather->GetZoneNode()->GetEntity());

		if	(pZone->HasSameFog(pFatherZone) == false)
		{
			const COM_INSTANCE &	PortalNodeInst = GetPortalNodeInst();
			NODE					*pPortalNode = static_cast<NODE *>(PortalNodeInst.GetCommunicator());
			PLANE					PortalPlane;
			VECTOR					PortalPos, ViewPos;
			VECTOR					ViewDir(VECTOR::NEG_UNIT_Z);

			PortalPos = pPortalNode->GetGlobalMatrix().GetTranslation();
			ViewPos = _NewViewMatrix.GetTranslation();
			_NewViewMatrix.TransformDirection(ViewDir);

			PortalPlane.SetFromPointAndNormal(PortalPos, ViewDir);

			rDistance = MATH::Abs(PortalPlane.GetSignedDistance(ViewPos));
		}
	}

	pZone->InitParameters(rDistance);

	bFog = pContext->IsFogEnabled();
	if	(bFog)
	{
		DISPLAY_MANAGER::Instance()->ComputeNearFogPlane(_DisplayParameters);

		rOldFar = _DisplayParameters.UpdateFrustrumFar(pContext->GetFogEnd());
		pDisplay->GetActiveCamera()->SetFarClippingDistance(pContext->GetFogEnd());
	}

	// Send geometries to render pipeline
	if (DISPLAY_MANAGER::Instance()->IsDynamicShadowsEnabled())
	{	
		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			pContext->BeginPixEvent("Reset visibility", COLOR(255, 255, 0, 0));
		#endif
		#endif

		BEGIN_PROFILE( PROFILE_ZONE_RESET_VISIBILITY );
		pZone->ResetVisibility(_DisplayParameters);	
		END_PROFILE( PROFILE_ZONE_RESET_VISIBILITY );

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			pContext->EndPixEvent();
		#endif
		#endif

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			pContext->BeginPixEvent("Display no shadow map tree", COLOR(255, 255, 0, 255));
		#endif
		#endif
		
		BEGIN_PROFILE( PROFILE_ZONE_DISPLAY_NOSMTREE );
		pZone->DisplayNoShadowMapTree(_DisplayParameters);
		END_PROFILE( PROFILE_ZONE_DISPLAY_NOSMTREE );

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			pContext->EndPixEvent();
		#endif
		#endif

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			pContext->BeginPixEvent("Display sorted objects", COLOR(255, 0, 0, 255));
		#endif
		#endif
	
		BEGIN_PROFILE( PROFILE_DISPLAY_SORTED_OBJECT );
		DisplaySortedObjects();	
		END_PROFILE( PROFILE_DISPLAY_SORTED_OBJECT );

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			pContext->EndPixEvent();
		#endif
		#endif
	}
	else
	{
		BEGIN_PROFILE( PROFILE_ZONE_RESET_VISIBILITY );
		pZone->ResetVisibility(_DisplayParameters);	
		END_PROFILE( PROFILE_ZONE_RESET_VISIBILITY );

		BEGIN_PROFILE( PROFILE_ZONE_DISPLAY_NOSMTREE );
		pZone->DisplayNoShadowMapTree(_DisplayParameters);
		END_PROFILE( PROFILE_ZONE_DISPLAY_NOSMTREE );

		BEGIN_PROFILE( PROFILE_DISPLAY_SORTED_OBJECT );
		DisplaySortedObjects();	
		END_PROFILE( PROFILE_DISPLAY_SORTED_OBJECT );
	}

	// Render objects		
	BEGIN_PROFILE( PROFILE_RENDER_DIFF_GEOMETRIES );
		pRenderPipeline->RenderGeometries();
	END_PROFILE( PROFILE_RENDER_DIFF_GEOMETRIES );

#ifndef PLATFORM_PS2
	// Render objects that cast simple shadows	
	BEGIN_PROFILE( PROFILE_RENDER_SIMPLE_GEOMETRIES );
		pRenderPipeline->RenderSimpleGeometries();
	END_PROFILE( PROFILE_RENDER_SIMPLE_GEOMETRIES );
#else
	pContext->SendBeginFrameDmaList();
#endif	

	if	(bFog)
	{
		pDisplay->NeedTestNearFogPlane(false);

		_DisplayParameters.UpdateFrustrumFar(rOldFar);
		pDisplay->GetActiveCamera()->SetFarClippingDistance(rOldFar);
	}

	pZone->ResetParameters();
	
	BEGIN_PROFILE( PROFILE_DISPLAY_TWIN_ZONES );
#ifndef	OPTI_STL
		It = _FilterMirrorZones.GetHead();
		ItEnd = _FilterMirrorZones.GetTail();

		while	(It != ItEnd)
		{
			(*It)->DisplayAsFilterMirror(pViewport);

			++It;
		}
#else
		pSortedZone = _pFilterMirrorZonesList;
		while	(pSortedZone)
		{
			pSortedZone->DisplayAsFilterMirror(pViewport);
			pSortedZone = pSortedZone->GetNext();
		}
#endif

		// Display all the twin zones
#ifndef	OPTI_STL
		It = _TwinZones.GetHead();
		ItEnd = _TwinZones.GetTail();

		while	(It != ItEnd)
		{
			(*It)->DisplayAsTwin(pViewport);

			++It;
		}
#else
		pSortedZone = _pTwinZonesList;
		while	(pSortedZone)
		{
			pSortedZone->DisplayAsTwin(pViewport);
			pSortedZone = pSortedZone->GetNext();
		}
#endif
	END_PROFILE( PROFILE_DISPLAY_TWIN_ZONES );

	// Set all parameters again for transparent objects
	DISPLAY_MANAGER::Instance()->SetCurrentPortalZone(this);

	pContext->SetViewTransform(&_InvNewViewMatrix);
	pContext->SetCullMode(_CullMode);

	_DisplayParameters.SetFrustrum(_Frustrum);

	pZone->InitParameters(rDistance);
	if	(bFog)
	{
		DISPLAY_MANAGER::Instance()->ComputeNearFogPlane(_DisplayParameters);

		rOldFar = _DisplayParameters.UpdateFrustrumFar(pContext->GetFogEnd());
		pDisplay->GetActiveCamera()->SetFarClippingDistance(pContext->GetFogEnd());
	}

	BEGIN_PROFILE( PROFILE_RENDER_HDR_OBJECTS );
		// Render Hdr objects
			#ifndef _MASTER
			#ifdef PLATFORM_XBOX
			pContext->BeginPixEvent("Render hdr objects", COLOR(255, 255, 0, 255));
			#endif
			#endif

			#ifndef PLATFORM_PS2
			#ifndef HDR_SEPARATE_TARGET
			pContext->EnableColorChannelsWrite(CWC_ALL_CHANNELS);
			#endif
			#endif
			
			pRenderPipeline->RenderHdrTriangles(pViewport->IsHDRMeshNormalDisplayEnabled());
			DisplayHdrObjects(pViewport->IsHDRMeshNormalDisplayEnabled());
			
			#ifndef PLATFORM_PS2
			#ifndef HDR_SEPARATE_TARGET
			pContext->EnableColorChannelsWrite(CWC_RGB_ONLY);
			#endif
			#endif

#ifdef PLATFORM_PS2
			if (GetType() != T_CAMERA)
			{
				ResetHDR();
			}
#endif

			#ifndef _MASTER
			#ifdef PLATFORM_XBOX
				pContext->EndPixEvent();
			#endif
			#endif
	END_PROFILE( PROFILE_RENDER_HDR_OBJECTS );

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		pContext->BeginPixEvent("Render transparent objects", COLOR(255, 255, 255, 255));
	#endif
	#endif

	BEGIN_PROFILE( PROFILE_RENDER_TRANSPARENT_OBJECTS );
		// Render transparents objects
		pRenderPipeline->RenderTransparentTriangles();
		DisplayTransparentObjects();
	END_PROFILE( PROFILE_RENDER_TRANSPARENT_OBJECTS );

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		pContext->EndPixEvent();
	#endif
	#endif

	BEGIN_PROFILE( PROFILE_RENDER_PARTICLES );	
		// Render Particles
		pZone->DisplayParticles(_DisplayParameters);	
	END_PROFILE( PROFILE_RENDER_PARTICLES );

	_DisplayParameters.SetFrustrum(_Frustrum);

	if	(bFog)
	{
		pDisplay->NeedTestNearFogPlane(false);

		_DisplayParameters.UpdateFrustrumFar(rOldFar);
		pDisplay->GetActiveCamera()->SetFarClippingDistance(rOldFar);
	}

	pZone->ResetParameters();

	pRenderPipeline->RemoveTransList();
	pRenderPipeline->RemoveHdrList();
}

//-----------------------------------------------------------------------------
//	Name:		DisplayTransparentObjects
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayTransparentObjects()
{
	RENDER_CONTEXT * pContext	 = RENDER_MANAGER::Instance()->GetRenderContext();
	bool			 bDepthWrite = pContext->IsDepthWriteEnabled();

	pContext->EnableDepthWrite(false);

	// First we display unsorted objects
	for (unsigned int i = 0 ; i < _vTransparentObjects.GetSize() ; ++i)
	{
		_vTransparentObjects[i].Display(_DisplayParameters);
	}

	// Next we sort & display sorted transparent objects
	_dlSortedTransparentObjects.Sort();
	QDT_DLIST<TRANSPARENT_OBJECT>::ITERATOR it = _dlSortedTransparentObjects.GetHead();
	while (it != _dlSortedTransparentObjects.GetTail())
	{
		(*it).Display(_DisplayParameters);
		++it;
	}

	_dlSortedTransparentObjects.Clear();
	_vTransparentObjects.Clear();

	pContext->EnableDepthWrite(bDepthWrite);
}


//-----------------------------------------------------------------------------
//	Name:		DisplayHdrObjects
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplayHdrObjects(bool bIsNormalMeshDisplay)
{	
	for (unsigned int i = 0 ; i < _vHDRObjects.GetSize() ; ++i)
	{
		if(!bIsNormalMeshDisplay && !_vHDRObjects[i].IsNormalMeshDisplay())
		{
			continue;
		}
		
		_vHDRObjects[i].Display(_DisplayParameters);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddMirrorZone
//	Object:		
//	01-08-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::AddMirrorZone(SORTED_PORTAL_ZONE *	pMirrorZone)
{
#ifndef	OPTI_STL

	float	rDistance = pMirrorZone->GetDistance();
	int		nPriority = pMirrorZone->GetPriority();

	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	It =_MirrorZones.GetHead();
	QDT_VECTOR<SORTED_PORTAL_ZONE *>::ITERATOR	ItEnd =	_MirrorZones.GetTail();

	while (It != ItEnd)
	{
		if ((*It)->GetPriority() < nPriority || 
		   ((*It)->GetPriority() == nPriority && (*It)->GetDistance() < rDistance))
		{
			_MirrorZones.Insert(It, pMirrorZone);
			return;
		}
		++It ;
	}
	
	_MirrorZones.PushTail(pMirrorZone);

#else

	if	(_pMirrorZonesList)
	{
		float				rDistance = pMirrorZone->GetDistance();
		int					nPriority = pMirrorZone->GetPriority();
		SORTED_PORTAL_ZONE	*pZone = _pMirrorZonesList;
		SORTED_PORTAL_ZONE	*pPrevZone = NULL;

		while	(pZone)
		{
			if ( pZone->GetPriority() < nPriority || 
			     (pZone->GetPriority() == nPriority && pZone->GetDistance() < rDistance) )
			{
				pMirrorZone->SetNext(pZone);

				if	(pPrevZone)
				{
					pPrevZone->SetNext(pMirrorZone);
				}
				else
				{
					_pMirrorZonesList = pMirrorZone;
				}

				return;
			}

			pPrevZone = pZone;
			pZone = pZone->GetNext();
		}

		pMirrorZone->SetNext(NULL);
		pPrevZone->SetNext(pMirrorZone);
	}
	else
	{
		pMirrorZone->SetNext(NULL);
		_pMirrorZonesList = pMirrorZone;
	}

#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplaySortedObjects
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
void	SORTED_PORTAL_ZONE::DisplaySortedObjects()
{
#ifdef SORT_OBJECT_BY_MATERIALS
	unsigned int i, nCount = _vSortedMaterials.GetSize();
	for (i = 0 ; i < nCount ; ++i)
	{
		_vSortedMaterials[i]->DisplaySortedObjects(_DisplayParameters);
	}

	_vSortedMaterials.Clear();
#endif
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


