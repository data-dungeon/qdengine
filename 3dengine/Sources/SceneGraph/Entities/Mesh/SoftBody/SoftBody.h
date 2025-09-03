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
//	CLASS:	SOFT_BODY
//	The SOFT_BODY class implements ...
//
//	02-01-16:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class SOFT_BODY : public MESH
{
public :
	typedef QDT_VECTOR<LATTICE*>::ITERATOR LIt;

	enum
	{
		CMD_SET_DAMPING = CMD_MESH_LAST,
		CMD_SET_STIFFNESS,
		CMD_SET_STRETCHING,
		CMD_SET_CONTRACTION,
		CMD_SET_AXE,
		CMD_PLAY_ANIM,
		CMD_STOP_ANIM,
		CMD_ADD_COLLISION_PLANE,
		CMD_REMOVE_COLLISION_PLANE,
		CMD_ADD_COLLISION_AABB,
		CMD_REMOVE_COLLISION_AABB,
		CMD_ADD_COLLISION_SPHERE,
		CMD_REMOVE_COLLISION_SPHERE,
		CMD_ADD_COLLISION_CYLINDER,
		CMD_REMOVE_COLLISION_CYLINDER,
		CMD_ADD_BOUNDING_VOLUME,
		CMD_REMOVE_BOUNDING_VOLUME,
		CMD_DISPLAY_BOUNDING_VOLUMES,
		CMD_CLEAR_BOUNDING_VOLUMES,
		CMD_CREATE_FORCE,
		CMD_DELETE_FORCE,
		CMD_UPDATE_GRAVITY,
		CMD_UPDATE_WIND,
		CMD_CLEAR_FORCES,
		CMD_PAUSE,
		CMD_RESET,
		CMD_SOFT_BODY_LAST,
	};

	enum	BOUNDING_VOLUME_TYPE
	{
		BV_SPHERE	= 0,
		BV_BOX,
		BV_CYLINDERS,
	};

								SOFT_BODY();
								~SOFT_BODY();

	/** From mesh */
	virtual void				Display(DISPLAY_PARAMETERS & DisplayParameters);
	virtual void				DisplayGeometries(DISPLAY_PARAMETERS & DisplayParameters, QDT_VECTOR <COM_INSTANCE> &vLightsOnDynamicLit, QDT_VECTOR<COM_INSTANCE> &vLightsOnPrelit, BOUNDING_VOLUME * pGlobalBoundingBox, QDT_VECTOR <MATRIX> *pvRefSkinningMatrices = NULL, QDT_VECTOR <DYNAMIC_CLUSTER*> *pvDCs = NULL, const QDT_VECTOR<COM_INSTANCE> *pvMats = NULL, bool bOptimizeRendererOnPS2 = false, QDT_VECTOR<LIGHT_SOURCE> * pvLightSourcesOnDynamicLit = NULL, QDT_VECTOR<LIGHT_SOURCE> * pvLightSourcesOnPrelit = NULL);
	virtual void				AddMeshEmitterInShadowMapRefs(DISPLAY_PARAMETERS & DisplayParameters, QDT_VECTOR <COM_INSTANCE> &vLightsOnDynamicLit, QDT_VECTOR<COM_INSTANCE> &vLightsOnPrelit, BOUNDING_VOLUME * pGlobalBoundingBox, QDT_VECTOR <MATRIX> *pvRefSkinningMatrices = NULL, QDT_VECTOR <DYNAMIC_CLUSTER*> *pvDCs = NULL, const QDT_VECTOR<COM_INSTANCE> *pvMats = NULL);
	virtual void				DisplayBoundingBox(DISPLAY_PARAMETERS & DisplayParameters) const;
	virtual bool				TestShadowVisibility(DISPLAY_PARAMETERS & DisplayParameters);
	virtual void				Update();
	virtual bool				ToUpdate() const;

	LATTICE *					CreateLattice();
	LATTICE *					GetLattice();
	void						AttachSolver(PHYSICAL_PARTICLE_SOLVER *pSolver);
	PHYSICAL_PARTICLE_SOLVER *	GetSolver();
	PARTICLE_EXCHANGER *		GetParticleExchanger();
	virtual void				ResetPhysic();
	virtual void				InitPhysic();
	void						GeometriesToParticles();
	void						ParticlesToGeometries();
	void						PlayAnim(COM_INSTANCE & AnimDataInst, float rSpeed, bool bCollide);
	void						StopAnim() const;
	void						SetDamping(int nDamping);
	int							GetDamping() const;
	void						SetStiffness(float rStiffness);
	float						GetStiffness() const;
	void						SetStretching(float rStretching);
	float						GetStretching() const;
	void						SetContraction(float rContraction);
	float						GetContraction() const;
	void						SetAxe(const VECTOR & Axe);

	_INLINE_ void				SetParticleExchanger(PARTICLE_EXCHANGER * pParticleExchanger) { _pExchanger = pParticleExchanger; }

	int							CreateForce(int nForceType);
	void						DeleteForce(int nForceIdx);
	void						UpdateGravity(int nForceIdx, float rI);
	void						UpdateWind(int nForceIdx, const VECTOR & Dir, float rMinI, float rMaxI);
	void						ClearForces();

	void						AddBoundingVolume(NODE *pMeshNode, BOUNDING_VOLUME_TYPE BVType, float rE);
	void						AddBoundingVolume(MATRIX * pMatrix, BOUNDING_VOLUME * pBV);
	int							AddBoundingAABB(float rMinX, float rMinY, float rMinZ, float rMaxX, float rMaxY, float rMaxZ);
	int							AddBoundingSphere(const VECTOR & Center, float rRadius);
	int							AddBoundingCylinder(const VECTOR & Dir, const VECTOR & Center, float rRadius, float rHeight);
	int							AddPlane(float ra, float rb, float rc, float rd);

	void						RemoveBoundingVolume(NODE *pMeshNode, BOUNDING_VOLUME_TYPE BVType);
	void						RemoveBoundingVolume(int nId);
	void						RemovePlane(int nId);

	void						DisplayBoundingVolumes(bool b);

	void						ClearBoundingVolumes();

	void						SetPause(bool b);

	// Communication
	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

protected :

	virtual bool				IsVisible(DISPLAY_PARAMETERS & DisplayParameters, BOUNDING_VOLUME * * pGlobalBV, bool bDoVisibility);			

private :

#ifndef _MASTER
	bool						WriteVersion1(IO_STREAM &s) const;
#endif
	bool						ReadVersion1(IO_STREAM &s);
	void						DisplayPhysicElements(DISPLAY_PARAMETERS &DisplayParameters);

	LATTICE						*_pLattice;

	PHYSICAL_PARTICLE_SOLVER	*_pSolver;
	PARTICLE_EXCHANGER			*_pExchanger;

	COM_INSTANCE				_AnimatedNodeInst;
	COM_INSTANCE				_AnimInst;

	AABB						_BoundingBox;

	QDT_DLIST< QDT_PAIR <BOUNDING_VOLUME *, int> >	_IntersectionBVList;
	static int										_nBVsIds;

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Commands
	static void		EM_SetDamping(void * const pO, void * const pR, void * const pP);
	static void		EM_SetStiffness(void * const pO, void * const pR, void * const pP);
	static void		EM_SetStretching(void * const pO, void * const pR, void * const pP);
	static void		EM_SetContraction(void * const pO, void * const pR, void * const pP);
	static void		EM_SetAxe(void * const pO, void * const pR, void * const pP);
	static void		EM_PlayAnim(void * const pO, void * const pR, void * const pP);
	static void		EM_StopAnim(void * const pO, void * const pR, void * const pP);
	static void		EM_AddCollisionPlane(void * const pO, void * const pR, void * const pP);
	static void		EM_RemoveCollisionPlane(void * const pO, void * const pR, void * const pP);
	static void		EM_AddCollisionAABB(void * const pO, void * const pR, void * const pP);
	static void		EM_RemoveCollisionAABB(void * const pO, void * const pR, void * const pP);
	static void		EM_AddCollisionSphere(void * const pO, void * const pR, void * const pP);
	static void		EM_RemoveCollisionSphere(void * const pO, void * const pR, void * const pP);
	static void		EM_AddCollisionCylinder(void * const pO, void * const pR, void * const pP);
	static void		EM_RemoveCollisionCylinder(void * const pO, void * const pR, void * const pP);
	static void		EM_AddBoundingVolume(void * const pO, void * const pR, void * const pP);
	static void		EM_RemoveBoundingVolume(void * const pO, void * const pR, void * const pP);
	static void		EM_DisplayBoundingVolume(void * const pO, void * const pR, void * const pP);
	static void		EM_ClearBoundingVolumes(void * const pO, void * const pR, void * const pP);
	static void		EM_CreateForce(void * const pO, void * const pR, void * const pP);
	static void		EM_DeleteForce(void * const pO, void * const pR, void * const pP);
	static void		EM_UpdateGravity(void * const pO, void * const pR, void * const pP);
	static void		EM_UpdateWind(void * const pO, void * const pR, void * const pP);
	static void		EM_ClearForces(void * const pO, void * const pR, void * const pP);
	static void		EM_Pause(void * const pO, void * const pR, void * const pP);
	static void		EM_Reset(void * const pO, void * const pR, void * const pP);

	// Chunks
#ifndef _MASTER_ONLY_DATABANK
	static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static void		CM_WriteChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

