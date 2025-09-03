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
//	CLASS:	DISPLAY_DUMMIES
//	The DISPLAY_DUMMIES class implements ...
//
//	02-06-04:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	DISPLAY_DUMMIES : public COM_OBJECT
{
public:

	enum DIS_DUM_FLAGS
	{
		DIS_DUM_DISPLAY_BOUNDING_BOXES				= 1 << 0,
		DIS_DUM_DISPLAY_BOUNDING_SPHERES			= 1 << 1,
		DIS_DUM_DISPLAY_LIGHT_BOUNDING_VOLUMES		= 1 << 2,
		DIS_DUM_DISPLAY_SKELETON_BOUNDING_VOLUMES	= 1 << 3,
		DIS_DUM_DISPLAY_LIGHTS						= 1 << 4,
		DIS_DUM_DISPLAY_JOINTS						= 1 << 5,
		DIS_DUM_DISPLAY_SKELETONS					= 1 << 6,
		DIS_DUM_DISPLAY_SHADOW_VOLUMES				= 1 << 7,
		DIS_DUM_DISPLAY_QUADTREES					= 1 << 8,
		DIS_DUM_DISPLAY_PARTICLE_EMITTERS			= 1 << 9,
		DIS_DUM_DISPLAY_FORCE_FIELDS				= 1 << 10,
		DIS_DUM_DISPLAY_BOUNDING_CYLINDERS			= 1 << 11,
		DIS_DUM_DISPLAY_PHYSIC						= 1 << 12,
		DIS_DUM_DISPLAY_PORTALS						= 1 << 13,
		DIS_DUM_DISPLAY_COLLISION_MAP				= 1 << 14,
	};

	/** For singleton */
	static	DISPLAY_DUMMIES *	Instance();
	static	void				Suicide();

	/** Modifiors */
	_INLINE_ void			DisplayBoundingBoxes(bool bValue)			{ _Flags.Set(DIS_DUM_DISPLAY_BOUNDING_BOXES, bValue); }
	_INLINE_ void			DisplayBoundingSpheres(bool bValue)			{ _Flags.Set(DIS_DUM_DISPLAY_BOUNDING_SPHERES, bValue); }
	_INLINE_ void			DisplayLightBoundingVolumes(bool bValue)	{ _Flags.Set(DIS_DUM_DISPLAY_LIGHT_BOUNDING_VOLUMES, bValue); }
	_INLINE_ void			DisplaySkeletonBoundingVolumes(bool bValue)	{ _Flags.Set(DIS_DUM_DISPLAY_SKELETON_BOUNDING_VOLUMES, bValue); }
	_INLINE_ void			DisplayLights(bool bValue)					{ _Flags.Set(DIS_DUM_DISPLAY_LIGHTS, bValue); }
	_INLINE_ void			DisplayJoints(bool bValue)					{ _Flags.Set(DIS_DUM_DISPLAY_JOINTS, bValue); }
	_INLINE_ void			DisplaySkeletons(bool bValue)				{ _Flags.Set(DIS_DUM_DISPLAY_SKELETONS, bValue); }
	_INLINE_ void			DisplayShadowVolumes(bool bValue)			{ _Flags.Set(DIS_DUM_DISPLAY_SHADOW_VOLUMES, bValue); }
	_INLINE_ void			DisplayQuadtrees(bool bValue)				{ _Flags.Set(DIS_DUM_DISPLAY_QUADTREES, bValue); }
	_INLINE_ void			DisplayParticleEmitters(bool bValue)		{ _Flags.Set(DIS_DUM_DISPLAY_PARTICLE_EMITTERS, bValue); }
	_INLINE_ void			DisplayForceFields(bool bValue)				{ _Flags.Set(DIS_DUM_DISPLAY_FORCE_FIELDS, bValue); }
	_INLINE_ void			DisplayBoundingCylinders(bool bValue)		{ _Flags.Set(DIS_DUM_DISPLAY_BOUNDING_CYLINDERS, bValue); }
	_INLINE_ void			DisplayPhysic(bool bValue)					{ _Flags.Set(DIS_DUM_DISPLAY_PHYSIC, bValue); }
	_INLINE_ void			DisplayPortals(bool bValue)					{ _Flags.Set(DIS_DUM_DISPLAY_PORTALS, bValue); }
	_INLINE_ void			DisplayCollisionMap(bool bValue)			{ _Flags.Set(DIS_DUM_DISPLAY_COLLISION_MAP, bValue); }

	/** Accessors */
	_INLINE_ bool			GetDisplayBoundingBoxes() const				{ return (_Flags.IsSet(DIS_DUM_DISPLAY_BOUNDING_BOXES)); }
	_INLINE_ bool			GetDisplayBoundingSpheres() const			{ return (_Flags.IsSet(DIS_DUM_DISPLAY_BOUNDING_SPHERES)); }
	_INLINE_ bool			GetDisplayLightBoundingVolumes() const		{ return (_Flags.IsSet(DIS_DUM_DISPLAY_LIGHT_BOUNDING_VOLUMES)); }
	_INLINE_ bool			GetDisplaySkeletonBoundingVolumes() const	{ return (_Flags.IsSet(DIS_DUM_DISPLAY_SKELETON_BOUNDING_VOLUMES)); }
	_INLINE_ bool			GetDisplayLights() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_LIGHTS)); }
	_INLINE_ bool			GetDisplayJoints() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_JOINTS)); }
	_INLINE_ bool			GetDisplaySkeletons() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_SKELETONS)); }
	_INLINE_ bool			GetDisplayShadowVolumes() const				{ return (_Flags.IsSet(DIS_DUM_DISPLAY_SHADOW_VOLUMES)); }
	_INLINE_ bool			GetDisplayQuadtrees() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_QUADTREES)); }
	_INLINE_ bool			GetDisplayParticleEmitters() const			{ return (_Flags.IsSet(DIS_DUM_DISPLAY_PARTICLE_EMITTERS)); }
	_INLINE_ bool			GetDisplayForceFields() const				{ return (_Flags.IsSet(DIS_DUM_DISPLAY_FORCE_FIELDS)); }
	_INLINE_ bool			GetDisplayBoundingCylinders() const			{ return (_Flags.IsSet(DIS_DUM_DISPLAY_BOUNDING_CYLINDERS)); }
	_INLINE_ bool			GetDisplayPhysic() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_PHYSIC)); }
	_INLINE_ bool			GetDisplayPortals() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_PORTALS)); }
	_INLINE_ bool			GetDisplayCollisionMap() const					{ return (_Flags.IsSet(DIS_DUM_DISPLAY_COLLISION_MAP)); }
	
	/** Derivation from COM_OBJECT */
	DESC*					GetDesc() const;
	int						GetNbChunk() const;
	const OLD_CHUNK &	GetChunk(int nIndex) const;
	bool					Read(const class OLD_CHUNK & Chunk, class IO_STREAM & Stream);
	bool					Write(const class OLD_CHUNK & Chunk, class IO_STREAM & Stream) const;
	void*					GetObject();
	bool					ReceiveCommandProc(COMMAND_ID CommandID, PARAMETER_STACK & Stack);


private:

	friend class			DISPLAY_DUMMIES_DESC;

	/** Contructor & destructor */
							DISPLAY_DUMMIES();
						   ~DISPLAY_DUMMIES();

	/** Chunks */
	enum
	{
		NBR_CHUNKS = 1,
	};
	static int				_nNbrChunks;
	static OLD_CHUNK	_ChunksList[NBR_CHUNKS]; 

	/** For singleton */
	static DISPLAY_DUMMIES *_pInstance;

	/** Flags */
	QDT_FLAGS				_Flags;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
