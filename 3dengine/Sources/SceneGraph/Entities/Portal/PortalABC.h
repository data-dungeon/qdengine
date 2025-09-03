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
// CLASS: PORTAL_ABC
//
//
//01-03-29 ELE Created: 
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

class	NODE;
class	SORTED_PORTAL_ZONE;

class	PORTAL_ABC : public ENTITY_ABC
{

public:

	enum
	{
		CMD_PORTAL_LAST = CMD_ENTITY_LAST,
	};

	/** For flags usage */
	enum
	{
		STATIC_GEOMETRY	 = 1 << 0,		// Portal is static
		DYNAMIC_GEOMETRY = 1 << 1,		// Portal is dynamic
		STATIC_LIT		 = 1 << 2,		// Portal lighting is precalculed
		DYNAMIC_LIT		 = 1 << 3,		// Portal lighting is calculed in real-time
		INVISIBLE		 = 1 << 4,		// Portal is not displayed
		NO_COLLISION	 = 1 << 5,		// Portal is not used for collision detection
		NO_VISIBILITY	 = 1 << 16,		// No visibility test
		DO_CLIPPING		 = 1 << 14,		// Do hardware clipping
		DO_SHADOW_CULLING= 1 << 15,		// Do backface culling on dynamic shadowing
		ONLY_DYNAMIC	 = 1 << 16,		// Only for mirrors, reflect only dynamic objects
		ALL_FLAGS		 = 0xFFFFFFFF,
	};

	explicit						PORTAL_ABC(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType);
	virtual							~PORTAL_ABC();

	virtual void					COM_PostLoad();

	/** flags */
	_INLINE_ void					SetFlags(int nFlags);
	_INLINE_ int					GetFlags(int nFlagsMask) const ;
	_INLINE_ void					SetFlags(int nFlagsMask, int nFlagsSet) ;

	_INLINE_ void					SetOnlyDynamicsReflexions(bool b);
	_INLINE_ bool					IsOnlyDynamicsReflexionsActivated() const;

	/** Accessors */
	_INLINE_ void					SetNbrGeometries(int nNbrGeometries);
	_INLINE_ int					GetNbrGeometries() const;
	_INLINE_ void					SetGeometries(GEOMETRY_ABC ** pGeometries);
	_INLINE_ void					SetMaterials(COM_INSTANCE * pciMaterials);
	_INLINE_ GEOMETRY_ABC**			GetGeometries() const;
	_INLINE_ GEOMETRY_ABC*			GetGeometry(int i) const;
	_INLINE_ void					SetNbrVertices(int nNbrVertices);
	_INLINE_ void					SetSortedVertices(VECTOR * pSortedVertices);
	_INLINE_ void					SetBoundingBox(const BOUNDING_VOLUME *pBV);
	_INLINE_ float					GetFarClippingDistance() const ;
	_INLINE_ void					SetFarClippingDistance(float rValue) ;
	_INLINE_ int					GetNbrCrossingMax() const ;
	_INLINE_ void					SetNbrCrossingMax(int nNbrCrossingMax);
	_INLINE_ void					SetAABBTree(AABB_TREE * pTree);
	_INLINE_ SORTED_PORTAL_ZONE *	GetPortalZone() ;
	_INLINE_ virtual COM_INSTANCE	GetTwinPortalNodeInst() const;
	_INLINE_ BOUNDING_VOLUME *		GetBoundingBox() const;
	_INLINE_ OBR *					GetOBR() const;
	_INLINE_ const PLANE &			GetPlane() const;
	_INLINE_ int					GetPriority() const;
	_INLINE_ void					SetPriority(int n);
	_INLINE_ void					SetSharedVBIndices(unsigned int * pIndices);
	_INLINE_ void					SetSharedVBIndex(int nGeometry, unsigned int nIndex);
	_INLINE_ void					SetMeshGroup(const COM_INSTANCE & MeshGroup);
	_INLINE_ void					Reset();
	_INLINE_ void					AddCrossing();

	/** Derivation from ENTITY_ABC : Display */
	virtual void					Display(DISPLAY_PARAMETERS & DisplayParameters);
	bool							TestVisibility(const DISPLAY_PARAMETERS & DisplayParameters, bool bDoVisibility);
	virtual bool					TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

	/** Display the portal in z-buffer */
	virtual void					DisplayZ(const MATRIX & WorldMatrix, const MATRIX &	ViewMatrix, int nMaskValue);
	void							DisplayN(const MATRIX & WorldMatrix, const MATRIX &	ViewMatrix);
	virtual void					DisplayMask(const MATRIX & WorldMatrix, const MATRIX & ViewMatrix, int nLevel, int nMaskValue);

	/** Update the render parameters */
	virtual bool					UpdatePortalZone(const MATRIX & ViewMatrix, const MATRIX & NodeGlobalMatrix, SORTED_PORTAL_ZONE *pPortalOut) = 0;
	float							ComputeNearDistance(const VECTOR & ViewPos, const VECTOR & ViewDir);

	// Communication
	static void						ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

protected:

	/** Read from database the common attributes */
	void							ReadCAttributes7(class IO_STREAM & Stream, int nVersion);
	void							ReadCAttributes8(class IO_STREAM & Stream, int nVersion);
	void							ReadCAttributes9(class IO_STREAM & Stream, int nVersion);
	void							ReadCAttributes10(class IO_STREAM & Stream, int nVersion);
	void							ReadCAttributes11(class IO_STREAM & Stream, int nVersion);
	/** Write to database the common attributes */
#ifndef _MASTER
	void							WriteCAttributes(class IO_STREAM & Stream, int nVersion) const;
#endif

	/** Display bouding voulmes */
	void							DisplayBoundingBox(DISPLAY_PARAMETERS & DisplayParameters) const;

	void							BuildPlane();

	/** Base geometry */
	int								_nNbrGeometries;		// Number of geometries
	GEOMETRY_ABC **					_pGeometriesArray;		// Array of geometry (one geometry by material)
	COM_INSTANCE *					_pGeometriesMaterialInst;
	GEOMETRY_ABC **					_pCopyGeometries;		// Copy geometries for mirrors only

	// For shared vertex buffer (PC & XBox only)
	unsigned int *					_pSharedVBIndex;				
	COM_INSTANCE					_MeshGroup; 

	/** Far clipping distance */
	float							_rFarClippingDistance;

	/** Bounding volumes */
	BOUNDING_VOLUME	*				_pBoundingBox;
	OBR	*							_pOBR;

	OBB								_GlobalBoundingBox;
	AABB_TREE *						_pAABBTree;

private:

	void							UpdateSharedVBs();

	/** Flags */
	int			_nFlags;

	/** For the dummy representation */
	int			_nNbrVertices;
	VECTOR		*_pSortedVerticesArray;

	/** Plane of the portal */
	PLANE		_Plane;

	/** Number of times we allow to cross the portal in a frame */
	int			_nNbrCrossingMax;
	int			_nNbrCrossing;

	/** Priority of portal */
	int			_nPriority;

	/** For the scene seen threw the portal */
	SORTED_PORTAL_ZONE	*_pPortalZones;

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
