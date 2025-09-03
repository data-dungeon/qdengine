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
// CLASS: SURFACE_MANAGER
//
//
//00-12-22 CVI Created: 
//*****************************************************************************


//=============================================================================
// Name: SURFACE_MANAGER
//00-12-22 CVI Created: 
//=============================================================================
/*class	SURFACE_MANAGER
{

	enum
	{
		MAX_CVS		= 1000,					//	Maximum de CVs dans le patch
	};

	enum
	{
		MAX_VALUES	= 400,					//	Maximum de vertex sur une courbe
	};

	enum
	{
		MAX_STORAGE_GROUP = 200,				//	Nombre de groupe maximum
	};

	enum
	{
		DEFAULT_TESSELATION_PARAM = 1,	//	Paramètre de tesselation
	};

	enum
	{
		MAX_VERTEX = 2000,
	};

public:
	
	explicit	SURFACE_MANAGER();
				~SURFACE_MANAGER();

	// For singleton : Instance 
	_INLINE_ static SURFACE_MANAGER*	Instance();
	// For singleton : Suicide 
	_INLINE_ static void				Suicide();

	// Traitement éventuel des CVs pour projection ou tranformation
	bool				PreProcessSurface(SURFACE *pS, const MATRIX *matrice, SURFACE_DISPLAY_PARAM& SDP);

	// Rendu d'une surface mappée implicitement 
	void				RenderSurfUVInfoType(int NbUV, SURFACE *pS, SURFACE_UV_INFO *pSurfaceUVInfo);

	// Rendu d'une surface mapée par projectioin du mapping 
	void				RenderSurfNormal(int NbUV, SURFACE *pS, POINT2D *pUVs);

	// Enregistrement d'un groupe de surfaces
	int					RegisterGroup( STORAGE_GROUP *pSG);

	// Enregistrement d'une surface dans le groupe courant 
	int					RegisterSurface( SURFACE *pS );

	// Gestion des edges éventuels de la surface 
	int					ManageSurfaceEdges( SURFACE_DISPLAY_PARAM&		SDP);

	// Génération d'un strip pour jonction de patchs 
	int					GenerateStripEdge( STORAGE_GROUP *pSG, DUAL_EDGE *pDE );

	// Parameters for constant framerate computation 
	_INLINE_ void		SetCurrentFrameRate( float rCurrentFrameRate );
	_INLINE_ float		GetCurrentFrameRate()const;
	_INLINE_ float		GetLastTesselParam()const;
	_INLINE_ void		SetLastTesselParam( float rLastTP );
	_INLINE_ float		GetTesselParam()const;
	_INLINE_ void		SetTesselParam( float rTP );
	void				ComputeTesselParam();
	_INLINE_ bool		IsConstantTesselParam()const;
	_INLINE_ void		SetConstantTesselParam( bool bConstant );
	_INLINE_ void		SetRateToKeep( float rToKeep );
	_INLINE_ float		GetRateToKeep()const;

	//	Accesseurs divers
	_INLINE_ bool				IsTransformed() const;
	_INLINE_ bool				IsProjected() const;
	_INLINE_ STORAGE_GROUP**	GetStorageGroupTab() const;
	_INLINE_ int				GetNbStorageGroup() const;

	int					BeginFrame( float rFrameRate );
	int					EndFrame();
	int					ChangeGroup();


protected:

	bool				_bTransformed;		//	Etat des CVs
	bool				_bProjected;
	bool				_bConstantTesselParam;	//	Use constant tesselation parameter if true

	SURFACE_TESSELATION	_SurfTessel;		//	Classe de paramètre de tesselation, envoyée au rendu
	POINT3D				*_pCVsBuffer;		//	Buffer des CVs
	VECTOR				*_pNormals;
	float				*_pValuesBuffer;		//	Buffer des valeurs des fonctions de base
	STORAGE_GROUP		**_StorageGroupTab;	//	Tableau des groupes enregistrés
	int					_nNbrStorageGroup;		
	int					_nCurrentHandle;		//	Handle du dernier groupe
	EDGE_STRIP_INFO		_EdgeBuilder;		//	Tesselation d'un edge
	VERTEX				*_pVertexBuffer;		//	Buffer de vertex
	int					_nStartOffset;		//	Position de départ du buffer de vertex
											//	pour stockage de bords de la surface
	float				_rLastTesselParam;
	float				_rCurrentTesselParam;
	float				_rCurrentFrameRate;
	float				_rRateToKeep;

	// For singleton : has the MOTEUR3D been destroyed once?
	static bool					_bDestroyedOnce;
	// For singleton : instance
	static SURFACE_MANAGER	   *_pInstance;
};*/

