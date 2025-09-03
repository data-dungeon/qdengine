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
// CLASS: LIGHT
//
//
//00-10-27 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: LIGHT
//00-10-27 JVA Created: 
//=============================================================================

class LIGHT : public ENTITY_ABC
{
	friend class LIGHT_DESC;
	/** Default constructor */
	explicit						LIGHT();
	/** Destructor */
								   ~LIGHT() ;

public:

	enum TYPE
	{
		POINT_LIGHT,
		SPOT_LIGHT,
		DIRECTIONAL_LIGHT,
	};

	enum LIGHT_FLAG
	{
		NO_LIGHTING_STATIC_NODE	  = 1 << 0,		
		NO_LIGHTING_DYNAMIC_NODE  = 1 << 1,
		SHADOW_PROJECTION		  = 1 << 2,
		ALL_FLAGS				  = 0xFFFFFFFF,
	};

	enum PRIMITIVE_TYPE
	{
		LIGHT_SPHERE,
		LIGHT_CONE,
		LIGHT_NB_PRIMITIVE_TYPES,
	};


	/** Operator = */
	LIGHT &							operator = (const LIGHT & Light) ;
	
	/** Get a flag mask */
	_INLINE_ unsigned long			GetFlags(unsigned long nFlagsMask) const ;
	/** Set a flag mask */
	_INLINE_ void					SetFlags(unsigned long nFlagsMask, unsigned long nFlagsSet) ;

	/** Get the type of light */
	_INLINE_ int					GetLightType() const;
	/** Set the type of light */
	_INLINE_ void					SetLightType(int nType) ;

	/** Get the color of the light */
	_INLINE_ const COLOR_FLOAT &	GetColor() const ;
	/** Set the color of the light */
	_INLINE_ void					SetColor(const COLOR_FLOAT & Color) ;
	
	/** Get the hot spot angle of the light */
	_INLINE_ float					GetHotSpotAngle() const ;
	/** Set the hot spot angle of the light */
	_INLINE_ void					SetHotSpotAngle(float rNewAngle) ;
	
	/** Get the fall off angle of the light*/
	_INLINE_ float					GetFallOffAngle() const ;
	/** Set the fall off angle of the light */ 
	_INLINE_ void					SetFallOffAngle(float rNewAngle) ;

	/** Get the hot spot distance of the light */ 
	_INLINE_ float					GetHotSpotDistance() const ;
	/** Set the hot spot distance of the light */ 
	_INLINE_ void					SetHotSpotDistance(float rDistance) ;

	/** Get the fall off distance of the light */
	_INLINE_ float					GetFallOffDistance() const ;
	/** Set the fall off distance of the light */
	_INLINE_ void					SetFallOffDistance(float rDistance) ;

	/** Get the mulitiplier of the light */
	_INLINE_ float					GetMultiplier() const ;
	/** Set the multiplier of the light */
	_INLINE_ void					SetMultiplier(float rNewMultiplier) ;

	/** Get the color of the shadow */
	_INLINE_ const COLOR_FLOAT &	GetShadowColor() const ;
	/** Set the color of the shadow */
	_INLINE_ void					SetShadowColor(const COLOR_FLOAT & Color) ;

	/** Get the bounding cone of the light */
	_INLINE_ CONE					GetBoundingCone() const;

	/** Get the bounding sphere of the light */
	_INLINE_ SPHERE					GetBoundingSphere() const;

	/** Send a light to the display of the kernel (before to display a mesh for exemple) */
	void							SendToDisplay(const MATRIX & GlobalMatrix);

	/** Derivation from ENTITY_ABC : Display */
	virtual void					Display(DISPLAY_PARAMETERS & DisplayParameters);
	/** Derivation from ENTITY_ABC : Sort */
	virtual void					Sort(NODE *pNode, DISPLAY_PARAMETERS & DisplayParameters);
	/** Derivation from ENTITY_ABC : Is a light? */
	_INLINE_ virtual bool			IsLight() const;
	
	/** Derivation from COM_OBJECT : Get descriptor */
	DESC*							GetDesc() const;
	/** Derivation from COM_OBJECT : Get the number of chunks */
	int								GetNbChunk() const;
	/** Derivation from COM_OBJECT : Get a chunk */
	const CHUNK &					GetChunk(int nIndex) const;
	/** Derivation from COM_OBJECT : Read a chunk from a stream */
	bool							Read(const class CHUNK & Chunk, class IO_STREAM & Stream);
	/** Derivation from COM_OBJECT : Write a chunk in a stream */
	bool							Write(const class CHUNK & Chunk, class IO_STREAM & Stream) const;
	void *							GetObject();
	bool							ReceiveCommandProc(COMMAND_ID, PARAMETER_STACK &);
	
private:

	/** Convert the light to the format of the RENDER_MANAGER*/
	void							BuildRMLight();

	/** Copy method to avoid code duplication in operator = & copy constructor */
	_INLINE_ void Copy(const LIGHT & Light);


	/** Number of chunks */
	enum
	{
		NB_CHUNK = 1,
	};
	/** Set to 0 if no object of this type has been loaded */
	static int		_nNbChunk;
	/** The array of the chunks which belongs to this COM_OBJECT */ 
	static CHUNK	_ChunkList[NB_CHUNK];

	/** Flags */
	unsigned long	_nFlags;
	
	/** Type of the light */
	int				_nType;

	/** Color of the light */
	COLOR_FLOAT		_Color;

	/** Hot Spot angle of the light*/
	float			_rHotSpotAngle;

	/** Fall off angle of the light */
	float			_rFallOffAngle;

	/** Hot spot distance of the light */
	float			_rHotSpotDistance;

	/** Fall off distance of the light */
	float			_rFallOffDistance;

	/** Multiplier of the light */
	float			_rMultiplier;

	/** Parameters for shadows */
	COLOR_FLOAT		_ShadowColor;

	/** RENDER_MANAGER format of the light */
	LIGHT_SOURCE	_RMLight;

	/** Infos to sort the light */
	SORTED_NODE_INFOS *	_pInfos;
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
