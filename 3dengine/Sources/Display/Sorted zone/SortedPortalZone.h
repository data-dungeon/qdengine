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
//	The SORTED_PORTAL_ZONE class implements ...
//
//	01-07-16:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#define	OPTI_STL

class	VIEWPORT;
class	PORTAL_ABC;

class	SORTED_PORTAL_ZONE
{
public:
	SORTED_PORTAL_ZONE();
	~SORTED_PORTAL_ZONE();

	enum	TYPE
	{
		T_TWIN		= 0,
		T_MIRROR	= 1,
		T_CAMERA	= 2,

		T_MAX		= 0xFFFFFFFF
	};

#ifdef	OPTI_STL
	enum
	{
		MAX_PORTALS = 5,
		MAX_HEXAHEDRONS = MAX_PORTALS - 1,
	};
#endif

	/** Display the scene seen threw the portal */
	void									DisplayAsCamera(const VIEWPORT *pViewport);
	void									DisplayHDR(const VIEWPORT *pViewport);
	_INLINE_ void							ResetHDR();

	void									DisplayAsMirror(const VIEWPORT *pViewport);
	void									DisplayAsFilterMirror(const VIEWPORT *pViewport);
	void									DisplayAsTwin(const VIEWPORT *pViewport);

	void									DisplayMirrorZones(const VIEWPORT *pViewport);
	void									DisplayTwinZones(const VIEWPORT *pViewport);

	void									DisplayChildrenMirrors();
//	void									DisplayMirrors();

	/** Display the geometry of the portal in z-buffer */
	void									DisplayZ(const MATRIX &ViewMatrix);
	void									DisplayN(const MATRIX &ViewMatrix);
	void									DisplayMask(const MATRIX &ViewMatrix);

	_INLINE_ void							AddHexahedron(HEXAHEDRON * pHexahedron);
//	_INLINE_ void							AddMeshGroupHDR_Ref(const MESH_GROUP_HDR_REF &R);
	_INLINE_ int							GetNbrHexahedrons() const;
	_INLINE_ HEXAHEDRON *					GetHexahedron(int n);
	HEXAHEDRON *							BuildHexahedron();
	FRUSTRUM *								BuildMirroredFrustrum();

	void									Reset();
	bool									Has(PORTAL_ABC * pPortal);
	_INLINE_ bool							HasHDRMesh() const;

	/** Accessors */
	_INLINE_ void							SetLevel(int nLevel);
	_INLINE_ void							AddPortalNodeInst(const COM_INSTANCE & PortalNodeInst);
	_INLINE_ void							AddPortalOutNodeInst(const COM_INSTANCE & PortalOutNodeInst);
	_INLINE_ void							ClearPortals();
	_INLINE_ void							SetCullMode(RENDER_CONTEXT::CULL_MODE CullMode);
	_INLINE_ void							SetViewMatrix(const MATRIX & ViewMatrix);
	_INLINE_ void							SetZoneNode(NODE *pZoneNode);
	_INLINE_ void							SetFather(SORTED_PORTAL_ZONE *pFather);
#ifdef	OPTI_STL
	_INLINE_ void							SetNext(SORTED_PORTAL_ZONE *pNext);
	_INLINE_ void							SetNextMirrorToDisplay(SORTED_PORTAL_ZONE *pNextMirrorToDisplay);
#endif
	_INLINE_ int							GetLevel() const;
	_INLINE_ const COM_INSTANCE &			GetPortalNodeInst() const;
	_INLINE_ const COM_INSTANCE &			GetPortalOutNodeInst() const;
	_INLINE_ const COM_INSTANCE &			GetPortalNodeInst(int n) const;
	_INLINE_ const COM_INSTANCE &			GetPortalOutNodeInst(int n) const;
	_INLINE_ RENDER_CONTEXT::CULL_MODE		GetCullMode() const;
	_INLINE_ const MATRIX &					GetViewMatrix() const;
	_INLINE_ const MATRIX &					GetInvViewMatrix() const;
	_INLINE_ DISPLAY_PARAMETERS	&			GetDisplayParameters();
	_INLINE_ NODE *							GetZoneNode() const;
	_INLINE_ SORTED_PORTAL_ZONE	*			GetFather() const;
#ifdef	OPTI_STL
	_INLINE_ SORTED_PORTAL_ZONE	*			GetNext() const;
	_INLINE_ SORTED_PORTAL_ZONE	*			GetNextMirrorToDisplay() const;
#endif
	_INLINE_ TYPE							GetType() const;
	_INLINE_ void							SetType(TYPE Type);
	_INLINE_ void							EnableVisibility(bool bValue = true);
	_INLINE_ const FRUSTRUM					&GetFrustrum() const;
	_INLINE_ void							SetFrustrum(const FRUSTRUM & Frustrum);
	_INLINE_ bool							IsVisibilityEnable() const;
	_INLINE_ void							SetDistance(float rDistance);
	_INLINE_ float							GetDistance() const;
	_INLINE_ void							SetNearDistance(float rDistance);
	_INLINE_ int							GetPriority() const;
	_INLINE_ void							SetPriority(int nP);

	void									AddMirrorZone(SORTED_PORTAL_ZONE *pMirrorZone);
	_INLINE_ void							AddFilterMirrorZone(SORTED_PORTAL_ZONE *pMirrorZone);
	_INLINE_ void							AddTwinZone(SORTED_PORTAL_ZONE *pTwinZone);

	_INLINE_ void							AddMirrorToDisplay(SORTED_PORTAL_ZONE *pMirrorZone);

	_INLINE_ void							SetMaskValue(int n);

	_INLINE_ void							PushTransparentObject(const TRANSPARENT_OBJECT & TransObj);
	_INLINE_ void							PushSortedTransparentObject(const TRANSPARENT_OBJECT & TransObj);

	_INLINE_ void							PushHdrObject(const HDR_OBJECT & HdrObj);
	_INLINE_ void							PushMaterial(MATERIAL_ABC * pMaterial);

private:

	void									DisplayScene(const VIEWPORT * pViewport);
	void									DisplayTransparentObjects();
	void									DisplayHdrObjects(bool bIsNormalMeshDisplay = false);
	void									DisplaySortedObjects();

	// Recursivity level
	int										_nLevel;
	int										_nMaskValue;

	// zone which the portal out belongs to
	NODE									*_pZoneNode;

	// So that we can sort mirrors
	int										_nPortalPriority;
	float									_rDistance;

	// Near clipping distance
	float									_rNearClippingDistance;

	SORTED_PORTAL_ZONE						*_pFather;

	TYPE									_Type;

	// Culling mode for this zone
	RENDER_CONTEXT::CULL_MODE				_CullMode;

	FRUSTRUM								*_pMirroredFrustrum;
	HEXAHEDRON *							_pHexahedron;

	bool									_bVisibilityTest;
									
	// Portals
#ifndef	OPTI_STL
	QDT_VECTOR<COM_INSTANCE>				_vPortalNodesInst;
	QDT_VECTOR<COM_INSTANCE>				_vPortalOutNodesInst;
#else
	unsigned int							_nNbrPortals;
	COM_INSTANCE							_PortalNodesInst[MAX_PORTALS];
	unsigned int							_nNbrOutPortals;
	COM_INSTANCE							_PortalOutNodesInst[MAX_PORTALS];
#endif

#ifndef	OPTI_STL
	QDT_VECTOR<SORTED_PORTAL_ZONE *>		_MirrorZones;
	QDT_VECTOR<SORTED_PORTAL_ZONE *>		_FilterMirrorZones;
	QDT_VECTOR<SORTED_PORTAL_ZONE *>		_TwinZones;
	QDT_VECTOR<SORTED_PORTAL_ZONE *>		_MirrorsToDisplay;
#else
	SORTED_PORTAL_ZONE *					_pMirrorZonesList;
	SORTED_PORTAL_ZONE *					_pFilterMirrorZonesList;
	SORTED_PORTAL_ZONE *					_pFilterMirrorZonesListLast;
	SORTED_PORTAL_ZONE *					_pTwinZonesList;
	SORTED_PORTAL_ZONE *					_pTwinZonesListLast;
	SORTED_PORTAL_ZONE *					_pNext;

	SORTED_PORTAL_ZONE *					_pMirrorsToDisplayList;
	SORTED_PORTAL_ZONE *					_pNextMirrorToDisplay;
#endif

#ifndef	OPTI_STL
	QDT_VECTOR<HEXAHEDRON *>				_vHexahedrons;
#else
	unsigned int							_nNbrHexahedrons;
	HEXAHEDRON *							_Hexahedrons[MAX_HEXAHEDRONS];
#endif

	// New view matrix generated from the portal out
	MATRIX									_NewViewMatrix;
	MATRIX									_InvNewViewMatrix;

	// Display parameters of the portal out
	DISPLAY_PARAMETERS						_DisplayParameters;

	// Visibility test
	FRUSTRUM								_Frustrum;

	// Sorted materials
	QDT_VECTOR<MATERIAL_ABC *>				_vSortedMaterials;

	// Transparent Objects
	QDT_VECTOR<TRANSPARENT_OBJECT>			_vTransparentObjects;
	QDT_DLIST<TRANSPARENT_OBJECT>			_dlSortedTransparentObjects;

	// High dynamic range stuffs
	QDT_VECTOR<HDR_OBJECT>					_vHDRObjects;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

