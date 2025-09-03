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
// CLASS: GROUP
//
//
//00-10-27 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: GROUP
//00-10-27 JVA Created: 
//=============================================================================
class	MESH;

class	GROUP : public ENTITY_ABC
{
public:

	enum
	{
		CMD_GROUP_LAST = CMD_ENTITY_LAST,
	};

	enum	GROUP_TYPE
	{
		GTYPE_DISPLAY	= 0,
		GTYPE_COLLISION	= 1,
	};

	GROUP();
	~GROUP();

	/** For flags usage */
	enum
	{
		DO_CLIPPING		= 1 << 14,		// Do hardware clipping
		ALL_FLAGS		= 0xFFFFFFFF,
	};

	/** Flags */
	_INLINE_ int				GetFlags(int nFlagsMask) const;
	_INLINE_ void				SetFlags(int nFlagsMask, int nFlagsSet);
	_INLINE_ bool				CastShadows() const;
			 MESH *				GetMesh();

	/** Derivation from ENTITY_ABC */
	virtual void				Display(DISPLAY_PARAMETERS & DisplayParameters);
	virtual bool				RayCast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue);

	/** Accessors */
	_INLINE_ const BOUNDING_VOLUME*	GetBoundingBox() const;
	int								GetGroupType() const;

	/* Modifiers */
	_INLINE_ void				SetBoundingBox(const BOUNDING_VOLUME *pBB);
	_INLINE_ void				SetBoundingSphere(const BOUNDING_VOLUME *pBS);
	_INLINE_ void				SetGroupType(int nType);
	_INLINE_ void				SetMeshInst(const COM_INSTANCE & MeshInst);

	/** Test the visibility of the group */
	bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters, bool & bInside);
	bool						TestShadowVisibility(DISPLAY_PARAMETERS & DisplayParameters, bool & bInside);

	/** Shadow maps */
	virtual void				ClearShadowMapFlags();

	void									FindLights(DISPLAY_PARAMETERS &	DisplayParameters);
	_INLINE_ const QDT_VECTOR <COM_INSTANCE> &	GetLightVector() const { return (_vLightNodes); }
	_INLINE_ QDT_VECTOR <COM_INSTANCE> &		GetLightVector() { return (_vLightNodes); }

	// Communication
	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

private:

	/** Display geometries */
	void						DisplayGeometry(DISPLAY_PARAMETERS & DisplayParameters);

	/** Flags */
	int					_nFlags;

	/** Bounding volumes */
	BOUNDING_VOLUME		*_pBoundingBox;
	BOUNDING_VOLUME		*_pBoundingSphere;

	int					_nGroupType;

	/** For GTYPE_DISPLAY */
	COM_INSTANCE		_MeshInst;

	QDT_VECTOR <COM_INSTANCE>	_vLightNodes;

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
