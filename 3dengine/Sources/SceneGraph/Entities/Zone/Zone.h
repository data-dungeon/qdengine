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
// CLASS: ZONE
//
//
//	01-02-08:	RMA - Created
//*****************************************************************************


//=============================================================================
// Name: ZONE
//	01-02-08:	RMA - Created
//=============================================================================

class PARTICLE_SYSTEM;

class ZONE : public ENTITY_ABC
{
public:

	typedef enum PRIMITIVE_TYPE
	{
		BLOCK_BOUNDING_PLANES,
		BLOCK_BOUNDING_SPHERE,
		BLOCK_NB_PRIMITIVE_TYPES,
	};

	enum
	{
		CMD_ZONE_SET_AMBIENT_COLOR = CMD_ENTITY_LAST,
		CMD_ZONE_SET_AMBIENT_COLOR2,
		CMD_ZONE_SET_FOG_MODE,
		CMD_ZONE_SET_FOG_NEAR,
		CMD_ZONE_SET_FOG_FAR,
		CMD_ZONE_SET_FOG_COLOR,
		CMD_ZONE_SET_FOG_COLOR2,
		CMD_ZONE_CLEAR_PARTICLES,
		CMD_ZONE_GET_FOG_COLOR,
		CMD_ZONE_GET_STATIC_AMBIENT_COLOR,
		CMD_ZONE_GET_DYN_AMBIENT_COLOR,
		CMD_ZONE_SET_STATIC_AMBIENT_COLOR,
		CMD_ZONE_SET_STATIC_AMBIENT_COLOR2,
		CMD_ZONE_GET_FOG_NEAR,
		CMD_ZONE_GET_FOG_FAR,
		CMD_ZONE_SET_DYN_AMBIENT_COLOR,
		CMD_ZONE_LAST,
	};

	/** Accessors */
	_INLINE_ const COLOR_FLOAT &		GetDynamicAmbientColor() const;
	_INLINE_ const COLOR_FLOAT &		GetStaticAmbientColor() const;
	_INLINE_ const SPHERE &				GetBoundingSphere() const;
	_INLINE_ const AABB &				GetBoundingBox()const;
	_INLINE_ bool						IsFogEnable()const;
	_INLINE_ float						GetFogNear()const;
	_INLINE_ float						GetFogFar()const;

	_INLINE_ const COLOR &				GetFogColor()const;

	/** Modifiors */
	_INLINE_ void						SetDynamicAmbientColor(const COLOR_FLOAT & Color);
	_INLINE_ void						SetStaticAmbientColor(const COLOR_FLOAT & Color);
	_INLINE_ void						SetBoundingSphere(const SPHERE & Sphere);
	_INLINE_ void						SetBoundingBox(const AABB & Box);
	_INLINE_ void						SetFogEnable(bool b);
	_INLINE_ void						SetFogNear( float rval );
	_INLINE_ void						SetFogFar( float rval);

	_INLINE_ void						SetFogColor( const COLOR & C);

	/** Derivation from ENTITY_ABC: Is a zone? */
	virtual	void						DisplayParticles(DISPLAY_PARAMETERS & DisplayParameters);
	virtual bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);
	virtual void						Update();
	virtual bool						ToUpdate() const;
	virtual	void						InitParameters(float rFogDec);
	virtual	void						ResetParameters();

	_INLINE_	PARTICLE_SYSTEM *		GetParticleSystem();
	void								RemoveLinks();
	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

	// Light management
	_INLINE_ const QDT_VECTOR<NODE*> &	GetLightsOnDynamicLit() const;
	_INLINE_ const QDT_VECTOR<NODE*> &	GetLightsOnPrelit() const;
	void								AddLightOnDynamicLit(NODE *pLightNode);
	void								AddLightOnPrelit(NODE *pLightNode);
	void								RemoveLightOnDynamicLit(NODE *pLightNode);
	void								RemoveLightOnPrelit(NODE *pLightNode);
	_INLINE_ void						ResetLights();

	// Force fields
	_INLINE_ const QDT_DLIST <NODE *> &	GetForceFields();					

	// Particle emitters management
	void								AddParticleEmitter(NODE * pParticleEmitterNode);
	void								RemoveParticleEmitter(NODE * pParticleEmitterNode);
	_INLINE_ void						ResetParticleEmitters();

	// Shadow management
	void								AddShadowEmitter(NODE * pShadowEmitterNode);
	void								RemoveShadowEmitter(NODE * pShadowEmitterNode);
	_INLINE_ void						ResetShadowEmitters();
	_INLINE_ int						GetNbrShadowEmitters() const;
	_INLINE_ NODE *						GetShadowEmitter(unsigned int i) const;
	_INLINE_ const QDT_DLIST<NODE*> &	GetMeshGroupNodes() const;
	_INLINE_ const QDT_DLIST<NODE*> &	GetMeshNodes() const;

	void								AddMeshNode(NODE * pNode);
	void								AddMeshGroupNode(NODE * pNode);
	void								AddSkeletonNode(NODE * pNode);
	void								AddCollisionMeshNode(NODE * pNode);
	void								AddGlowNode(NODE * pNode);
	void								AddPrimitiveNode(NODE * pNode);
	void								AddTriggerNode(NODE * pNode);
	void								AddWaypointNode(NODE * pNode);
	void								AddForceFieldNode(NODE * pNode);
	void								AddPortalNode(NODE * pNode);
	void								RemoveMeshNode(NODE * pNode);
	void								RemoveMeshGroupNode(NODE * pNode);
	void								RemoveSkeletonNode(NODE * pNode);
	void								RemoveCollisionMeshNode(NODE * pNode);
	void								RemoveGlowNode(NODE * pNode);
	void								RemovePrimitiveNode(NODE * pNode);
	void								RemoveTriggerNode(NODE * pNode);
	void								RemoveWaypointNode(NODE * pNode);
	void								RemoveForceFieldNode(NODE * pNode);
	void								RemovePortalNode(NODE * pNode);

	virtual void						ResetVisibility(DISPLAY_PARAMETERS & DisplayParameters);
	virtual void						DisplayNoShadowMapTree(DISPLAY_PARAMETERS & DisplayParameters);

	// Physic
	_INLINE_ I_PHYSIC_SCENE *			GetPhysicScene() const;
	void								DisplayPhysics() const;

	bool								Grab(NODE * pNode);
	void								Ungrab(NODE * pNode);

private:

	// Constructor & destructor
    									ZONE();
										ZONE(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType);
	virtual							   ~ZONE();	

	/** Ambient color of the zone */
	COLOR_FLOAT							_DynamicAmbientColor;
	COLOR_FLOAT							_StaticAmbientColor;
	COLOR_FLOAT							_OldDynamicAmbientColor;
	COLOR_FLOAT							_OldStaticAmbientColor;
		
	//	Bounding box	
	AABB								_BBox;
	SPHERE								_BSphere;
	/*******/

	bool								_bFogEnable, _bOldFogEnable;	//	Enable fog
	float								_rFogNear, _rOldFogNear;		//	fog dist min 
	float								_rFogFar, _rOldFogFar;			//	fog dist max
	COLOR								_FogColor;						//	fog color
	COLOR								_OldFogColor;	

	QDT_VECTOR <NODE *>					_vParticleEmitters;
	QDT_VECTOR <NODE *>					_vShadowEmitters;
	QDT_VECTOR <NODE *>					_vLightsOnDynamicLit;
	QDT_VECTOR <NODE *>					_vLightsOnPrelit;

	PARTICLE_SYSTEM					   *_pParticleSystem;

	// To optimize display
	QDT_DLIST <NODE *>					_lMeshNodes;
	QDT_DLIST <NODE *>					_lMeshGroupNodes;
	QDT_DLIST <NODE *>					_lSkeletonNodes;
	QDT_DLIST <NODE *>					_lCollisionMeshNodes;
	QDT_DLIST <NODE *>					_lGlowNodes;
	QDT_DLIST <NODE *>					_lPrimitiveNodes;
	QDT_DLIST <NODE *>					_lTriggerNodes;
	QDT_DLIST <NODE *>					_lWayPointNodes;
	QDT_DLIST <NODE *>					_lForceFieldNodes;

	// Physic
	I_PHYSIC_SCENE *					_pPhysicScene;

	// COM
	static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
	static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetAmbientColor(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetAmbientColor2(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetFogMode(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetFogNear(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetFogFar(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetFogColor(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetFogColor2(void * const pO, void * const pR, void * const pP);
	static	void						EM_ClearParticles(void * const pO, void * const pR, void * const pP);
	static void							EM_GetFogColor(void * const pO, void * const pR, void * const pP);
	static void							EM_GetAmbientColor(void * const pO, void * const pR, void * const pP);
	static void							EM_GetStaticAmbientColor(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetStaticAmbientColor(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetStaticAmbientColor2(void * const pO, void * const pR, void * const pP);
	static	void						EM_GetFogNear(void * const pO, void * const pR, void * const pP);
	static	void						EM_GetFogFar(void * const pO, void * const pR, void * const pP);
	static	void						EM_SetDynamicAmbientColor(void * const pO, void * const pR, void * const pP);

#ifndef _MASTER_ONLY_DATABANK
	static	void						CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static	void						CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static	void						CM_WriteChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
