//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: SURFACE_MANAGER
//
//
//00 - 12 - 22 CVI Created:
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

//*****************************************************************************
//
// CLASS DESCRIPTION ( SURFACE_MANAGER )
//
//	The SURFACE_MANAGER class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_KRNCORE(Time)

#ifdef	_DEBUG
	#include	"SurfaceManager.inl"
#else
	#include	"Inline.h"

#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

// For singleton
/*SURFACE_MANAGER*	SURFACE_MANAGER::_pInstance = 0; 
bool				SURFACE_MANAGER::_bDestroyedOnce = false;


SURFACE_MANAGER::SURFACE_MANAGER()
{
	int		i;

	_pValuesBuffer	 = new float[ MAX_VALUES ] ; 			//	Buffer de stockage des valeurs
														// des fonctions de base sur la courbe u

	//	Fixe buffer le buffer à utiliser pour le stockage de valeurs des fonctions de base
	_SurfTessel.SetBuffer( _pValuesBuffer );


	_pCVsBuffer		 = new POINT3D[ MAX_CVS ] ; 				//	Buffer pour calcul de projection des CVs

	_pNormals		 = new VECTOR[ MAX_CVS ] ; 				//	Buffer de normals

	_pVertexBuffer	 = new VERTEX[ 10 * MAX_VERTEX ] ; 			//	Buffer de vertex pour les edges


	
	 //	Init du storagegroup 

	_StorageGroupTab	 = ( STORAGE_GROUP** ) new int[ MAX_STORAGE_GROUP ];
	_nNbrStorageGroup	 = 0;
	for ( i = 0 ; i < MAX_STORAGE_GROUP ; ++i )
	{
		_StorageGroupTab[ i ] = NULL;
	}

	 // Init for edges 
	_nCurrentHandle	 = - 1;
	_nStartOffset	 = 0;

	 // Init for tesselation using framerate
	SetConstantTesselParam( true );
	SetCurrentFrameRate( 20.0f );
	SetLastTesselParam( 1.0f );
	SetTesselParam( 1.0f );
	SetRateToKeep( 25.0f );

}


//	Destructeur
SURFACE_MANAGER::~SURFACE_MANAGER()
{
	if ( _pValuesBuffer )		
	{
		delete [ ] _pValuesBuffer;
	}
	if ( _pCVsBuffer )			
	{
		delete [ ] _pCVsBuffer;
	}
	if ( _StorageGroupTab )		
	{
		delete [ ] _StorageGroupTab;
	}
	if ( _pVertexBuffer )		
	{
		delete [ ] _pVertexBuffer;
	}
	if ( _pNormals )			
	{
		delete [ ] _pNormals;
	}
}


// Methode pour le rendu d'une surface mappée implicitement 
void	SURFACE_MANAGER::RenderSurfUVInfoType( int nNbrUV , SURFACE *pS , SURFACE_UV_INFO *pSurfaceUVInfo )
{
	const MATRIX			*pM;
	SURFACE_DISPLAY_PARAM	SDP;
	int						nHGroup;
	bool					bRes;

#ifndef __RENDERMANAGER__

	pM = DISPLAY::Instance() -> GetResultMatrix() ; 					//	Récupère matrice de transformation

#else

	//TODO:Portage RENDERMANAGER
	pM = NULL;

#endif

	bRes = PreProcessSurface( ( SURFACE* )pS , pM , SDP ) ; 		//	Traitement éventuel des CVs

	if ( ! bRes )
	{
		return ; 				//	Nurbs not displayed
	}

	
	//	Fixe etat des CVs
	if ( _bProjected )
	{
		//	CVs projetés
		SDP.SetProjectionType( SURFACE_DISPLAY_PARAM::PROJECTED_CVS );
	}
	else if ( _bTransformed )
	{
		// CVs transformés
		SDP.SetProjectionType( SURFACE_DISPLAY_PARAM::TRANSFORMED_CVS );
	}
	else
	{
		//	Aucun traitement effectué
		SDP.SetProjectionType( SURFACE_DISPLAY_PARAM::UNTRANSFORMED_CVS );
	}

	//	Nombre de couches de mapping
	SDP.SetNbUV( nNbrUV );

	//	Type de mapping
	SDP.SetMappingType( SURFACE_DISPLAY_PARAM::SURFACEUV_INFOTYPE );

	//	Fixe surface
	SDP.SetSurface( pS );

	//	Fixe Buffer de CVs
	SDP.SetCVsBuffer( _pCVsBuffer );

	//	Fixe normales
	SDP.SetNormals( _pNormals );

	//	Init surfaceUVInfo
	SDP.SetUVInfo( pSurfaceUVInfo );

	//	Fixe classe de paramètre de tesselation
	SDP.SetSurfTesselation( & _SurfTessel );

	//	Init du champ computed ( calcul des valeurs de fonction de base de la courbe u
	_SurfTessel.SetComputed( FALSE );


	//	Test si changement de groupe ou pas de groupe présent
	if ( pS -> GetHandle() != - 1 )
	{
		nHGroup = 	pS -> GetHandle();
		nHGroup >>= 	16;

		//	Test si changement de groupe ou pas de groupe présent
		if ( ( _nCurrentHandle != nHGroup ) || ( _nCurrentHandle == - 1 ) )
		{
			//	Handle du groupe courant
			_nCurrentHandle	 = nHGroup;
			//	Opération de changement de groupe
			ChangeGroup();
		}
		 // Fixe le début du buffer de vertex
		SDP.SetVertexBuffer( & _pVertexBuffer[ _nStartOffset ] );
	}
	else
	{
		 // Fixe le début du buffer de vertex
		SDP.SetVertexBuffer( & _pVertexBuffer[ 0 ] );
	}


	//	Rendu de la surface

#ifndef __RENDERMANAGER__

	DISPLAY::Instance() -> DisplaySurface( SDP );

#else

	//TODO:Portage RENDERMANAGER

#endif


	//	Gestion des edges éventuels de la surface
	ManageSurfaceEdges( SDP );

}

// Rendu d'une surface mappée par projection du mapping
void	SURFACE_MANAGER::RenderSurfNormal( int nNbrUV , SURFACE *pS , POINT2D *pUVs )
{
	const MATRIX			*pM;
	SURFACE_DISPLAY_PARAM	SDP;
	int						nHGroup;
	bool					bRes;

	
	//	Transformation éventuelle des CVs

#ifndef __RENDERMANAGER__

	pM = DISPLAY::Instance() -> GetResultMatrix();

#else

	//TODO:Portage RENDERMANAGER
	pM = NULL;

#endif

	bRes = PreProcessSurface( ( SURFACE* )pS , pM , SDP );

	if ( ! bRes )
	{
		return;
	}


	//	Fixe etat des CVs
	if ( _bProjected )
	{
		//	CVs projetés
		SDP.SetProjectionType( SURFACE_DISPLAY_PARAM::PROJECTED_CVS );
	}
	else if ( _bTransformed )
	{
		// Cvs transformés
		SDP.SetProjectionType( SURFACE_DISPLAY_PARAM::TRANSFORMED_CVS );
	}
	else
	{
		//	Cvs non traités
		SDP.SetProjectionType( SURFACE_DISPLAY_PARAM::UNTRANSFORMED_CVS );
	}

	//	Nombre de couches de mapping
	SDP.SetNbUV( nNbrUV );

	//	Type de mapping
	SDP.SetMappingType( SURFACE_DISPLAY_PARAM::PROJECTION_MAPPING );

	//	Fixe surface
	SDP.SetSurface( pS );

	//	Fixe Buffer de CVs
	SDP.SetCVsBuffer( _pCVsBuffer );

	//	Fixe normales
	SDP.SetNormals( _pNormals );

	//	Init surfaceUVInfo
	SDP.SetUVs( pUVs );

	//	Init classe de parmaètre de tesselation
	SDP.SetSurfTesselation( & _SurfTessel );
	_SurfTessel.SetComputed( FALSE ) ; 			//	Calcul des valeurs de la courbe u non effectué

	//	Init du buffer de vertex pour tesselation des edges
	if ( pS -> GetHandle() != - 1 )
	{
		nHGroup = 	pS -> GetHandle();
		nHGroup >>= 	16;

		//	Test si changement de groupe ou pas de groupe présent
		if ( ( _nCurrentHandle != nHGroup ) || ( _nCurrentHandle == - 1 ) )
		{
			//	Handle du groupe courant
			_nCurrentHandle	 = nHGroup;
			//	Opération de changement de groupe
			ChangeGroup();
		}
		 // Fixe le début du buffer de vertex 
		SDP.SetVertexBuffer( & _pVertexBuffer[ _nStartOffset ] );
	}
	else
	{
		 // Fixe le début du buffer de vertex
		SDP.SetVertexBuffer( & _pVertexBuffer[ 0 ] );
	}
	

#ifndef __RENDERMANAGER__

	//	Affichage de la surface
	DISPLAY::Instance() -> DisplaySurface( SDP );

#else

	//TODO::Portage RENDERMANAGER

#endif

	//	Gestion des edges éventuels de la surface
	ManageSurfaceEdges( SDP );


}

// Gestion changement de groupe 
int	SURFACE_MANAGER::ChangeGroup()
{
	int			n;
	int			i;
	DUAL_EDGE	*pDE;

	_nStartOffset	 = 0 ; 			//	Début du buffer de vertex
	
	 // On remet les flag à 0 
	if ( _nCurrentHandle != - 1 )
	{
		n	 = _StorageGroupTab[ _nCurrentHandle ] -> GetNbDualEdges() ; 			//	Nombre de dual edges
		pDE = _StorageGroupTab[ _nCurrentHandle ] -> GetDualEdgeTab() ; 			//	Tableau des dualedges

		//	Parcours du tableau
		for ( i = 0 ; i < n ; ++i )
		{
			//	Met flag à 0
			pDE -> SetStatus( 0 );
			++pDE; 						//	Dualedge suivant
		}
	}
	return ( 1 );
}

// Calculs éventuels sur CVs 
bool	SURFACE_MANAGER::PreProcessSurface( SURFACE *pS , const MATRIX *matrice , SURFACE_DISPLAY_PARAM & SDP )
{
	int			nAmount , nAmount2;
	int			nNbrUCVs;
	int			nNbrVCVs;
	POINT3D		Tmp;
	POINT3D		*pCVs , *pP , *pLast;// , *pTemp;
	VECTOR		*pN;
	float		rMinx , rMiny , rMaxx , rMaxy , rMaxz;
	float		x , y , z , dx , dy;
	float		h , w ; 					//	Pour viewport
	float		wu , wv;
	float		rStepU , rStepV;
	float		rParamTesselation;
	float		rtu , rtv;
	float		rdiffu , rdiffv;
	POINT3D		pSrcPoints[ 8 ];
	POINT3D		pDstPoints[ 8 ];
	float		rRMinx , rRMiny , rRMinz , rRMaxx , rRMaxy , rRMaxz;
	int			nStep;



	//	Cvs transformés par défaut
	_bTransformed = TRUE;

	//	Projetés par defaut
	_bProjected	 = FALSE;

	pCVs = pS -> GetpCVs() ; 				//	Récupère Buffer de CVs
	pN	  = pS -> GetBufferNormals() ; 			//	Get normals
	nNbrUCVs = pS -> GetNbCVsInU() ; 			//	Nombre de CVS en U
	nNbrVCVs = pS -> GetNbCVsInV() ; 			//	Nombre de CVS en V
	nAmount = ( nNbrUCVs * nNbrVCVs ) ; 			//	Nombre total de CVs
	QDT_ASSERT( nAmount <= MAX_CVS );

	if ( nAmount > MAX_CVS )	
	{
		return	( false );
	}
	
	//	Init pour traitement taille de la surface
	rMinx = FLT_MAX;
	rMiny = FLT_MAX;

	rMaxx = FLT_MIN;
	rMaxy = FLT_MIN;
	rMaxz = - FLT_MAX;

	matrice -> Apply( nAmount , pN , _pNormals );
	matrice -> Apply( nAmount , pCVs , _pCVsBuffer );

	//	Get Min Max
	rRMinx = pS -> GetMinX();
	rRMiny = pS -> GetMinY();
	rRMinz = pS -> GetMinZ();
	rRMaxx = pS -> GetMaxX();
	rRMaxy = pS -> GetMaxY();
	rRMaxz = pS -> GetMaxZ();

	//	Build points
	pSrcPoints[ 0 ].SetXYZ( rRMinx , rRMiny , rRMinz );
	pSrcPoints[ 1 ].SetXYZ( rRMinx , rRMiny , rRMaxz );
	pSrcPoints[ 2 ].SetXYZ( rRMinx , rRMaxy , rRMinz );
	pSrcPoints[ 3 ].SetXYZ( rRMinx , rRMaxy , rRMaxz );
	pSrcPoints[ 4 ].SetXYZ( rRMaxx , rRMiny , rRMinz );
	pSrcPoints[ 5 ].SetXYZ( rRMaxx , rRMiny , rRMaxz );
	pSrcPoints[ 6 ].SetXYZ( rRMaxx , rRMaxy , rRMinz );
	pSrcPoints[ 7 ].SetXYZ( rRMaxx , rRMaxy , rRMaxz );

	matrice -> Apply( 8 , pSrcPoints , pDstPoints );


	//	Project points
	nAmount2 = 8;
	pP = & pDstPoints[ 0 ];

	while ( nAmount2-- )
	{

#ifndef __RENDERMANAGER__

		DISPLAY::Instance() -> VertexProjection( *pP );

#else

		//TODO:Portage RENDERMANAGER

#endif
		x = pP -> GetX();
		y = pP -> GetY();
		z = pP -> GetZ();

		if ( x < rMinx )
		{
			rMinx = x;
		}

		if ( x > rMaxx )
		{
			rMaxx = x;
		}

		if ( y < rMiny )
		{
			rMiny = y;
		}

		if ( y > rMaxy )
		{
			rMaxy = y;
		}

		if ( z > rMaxz )
		{
			rMaxz = z;
		}

		pP++;
	}


	pP		 = _pCVsBuffer;
	pLast	 = pP;

#ifndef __RENDERMANAGER__

	DISPLAY::Instance() -> GetViewport( w , h );

#else

	w = RENDER_MANAGER::Instance()->GetRenderContext()->GetViewportWidth();
	h = RENDER_MANAGER::Instance()->GetRenderContext()->GetViewportHeight();

#endif

	if ( this -> IsProjected() )
	{
		while ( nAmount-- )
		{
			//rotation + projection des vertex...voir pour projection cavalière

#ifndef __RENDERMANAGER__

			DISPLAY::Instance() -> VertexProjection( *pP );

#else

			//TODO:Portage RENDERMANAGER

#endif
			pP++;

		}
	}


	//	On va déterminer le step
	dx = rMaxx - rMinx;
	dy = rMaxy - rMiny;

#ifndef __RENDERMANAGER__

	//	Récupère dimensions de l'ecran
	DISPLAY::Instance() -> GetViewport( w , h );

#else

	//	Récupère dimensions de l'ecran
	w = RENDER_MANAGER::Instance()->GetRenderContext()->GetViewportWidth();
	h = RENDER_MANAGER::Instance()->GetRenderContext()->GetViewportHeight();

#endif


	//	Clipping de la surface
//	if ( rMaxx < 0 )
//	{
//		return ( false );
//	}
//	if ( rMinx > w )
//	{
//		return ( false );
//	}
//
//	if ( rMaxy < 0 )
//	{
//		return ( false );
//	}
//	if ( rMiny > h )
//	{
//		return ( false );
//	}
//	if ( rMaxz < 0 )
//	{
//		return ( false );
//	}
//
	w *= h;
	dx *= dy;
	
	if ( dx != 0.0f )
	{
		w /= dx ; 		//	Calcul rapport superficie ecran / patch
		rParamTesselation = GetTesselParam() ; 		//	Paramètre de tesselation

		w	 *= rParamTesselation;
		//	Get level of tesselation
		rtu	 = pS -> GetTesselationLevelInU();
		rtv = pS -> GetTesselationLevelInV();

		rtu *= rtu;
		rtv *= rtv;

		if ( rtu > 1.0f )
		{
			rdiffu = rtu / 200.0f;
		}
		else
		{
			rdiffu = 0.0f;
		}

		if ( rtv > 1.0f )
		{
			rdiffv = rtv / 200.0f;
		}
		else
		{
			rdiffv = 0.0f;
		}
		
		wu = ( ( 12.0f + rdiffu ) * w ) / ( rtu * 350.0f + w );
		wu = ( 0.75f - rdiffu ) + wu;

		wv = ( ( 12.0f + rdiffu ) * w ) / ( rtv * 350.0f + w );
		wv = ( 0.75f - rdiffv ) + wv;

	}
	else
	{
		rtu = 1.0f;
		rtv = 1.0f;
		wu = 8.0f;
		wv = 8.0f;
	}


	//	Calcul des steps
	rStepU = wu / static_cast< float >( nNbrUCVs );
	if ( rtu > 1.0f )
	{
		if ( rStepU > 0.24f )
		{
			rStepU = 0.24f;
		}
	}
	else if ( rStepU > 0.49f )
	{
		rStepU = 0.49f;
	}

	rStepV = wv / static_cast< float >( nNbrVCVs );
	if ( rtv > 1.0f )
	{
		if ( rStepV > 0.24f )
		{
			rStepV = 0.24f;
		}
	}
	else if ( rStepV > 0.49f )
	{
		rStepV = 0.49f;
	}
	nStep = static_cast< int >( 1.0f / rStepU );
	rStepU = 1 / ( static_cast< float >( nStep ) );

	nStep = static_cast< int >( 1.0f / rStepV );
	rStepV = 1 / ( static_cast< float >( nStep ) );

	
	//	Fixe résulat dans classe de parmètres de tesselation

	SDP.SetStepInU( rStepU );
	SDP.SetStepInV( rStepV );


	return ( true );
}

// Enregistrement des storagegroup
int		SURFACE_MANAGER::RegisterGroup( STORAGE_GROUP *pSG )
{
	int		i;
	bool	bFound;

	//	Recherche d'une place libre
	bFound = false;

	for ( i = 0 ; i < MAX_STORAGE_GROUP ; ++i )
	{
		if ( _StorageGroupTab[ i ] == NULL )
		{
			bFound = true;
			break;
		}
	}

	QDT_ASSERT( bFound );

	//	Enregistre storage group
	_StorageGroupTab[ i ] = pSG;
	++_nNbrStorageGroup;

	//	Fixe le handle
	pSG -> SetHandle( i );
	_nCurrentHandle = i;

	return ( i );
}

// Enregistrement des surfaces
int		SURFACE_MANAGER::RegisterSurface( SURFACE *pS )
{
	int				hG , hS;
	STORAGE_GROUP	*pSG;

	if ( _nNbrStorageGroup == 0 )
	{
		pS -> SetHandle( - 1 );
		return ( 1 );
	}
	//	Construction du handle de la surface
	hG = _nCurrentHandle << 16;

	//	Handle de la surface
	hS = pS -> GetHandle();

	//	Ajout handle du group courant
	hS |= hG;

	//	Fixe le nouveau handle de la surface
	pS -> SetHandle( hS );

	//	Fixe le pointeur de surface
	pSG = _StorageGroupTab[ _nCurrentHandle ];
	pSG -> RegisterSurface( pS );


	return ( 1 );
}

int					IndexToTest = 11 ; 				// Pour debug

// Gestion des edges eventuels de la surface 
int		SURFACE_MANAGER::ManageSurfaceEdges( SURFACE_DISPLAY_PARAM & 	SDP )
{
	DUAL_EDGE			*pDE0 , *pDE1;
	EDGE				*pE;
	SURFACE_REC			*pSR;
	int					nHGroup;
	int					nHSurf;
	SURFACE				*pS;
	STORAGE_GROUP		*pSG;
	int					i , j;
	int					nNbrEdges;
	int					nStat;
	int					nTypeAS;
	STRIP_EDGE_BUILDER	*pEdgeStripBuilder;
	int					i0 , i1;


//	return 1;
	
	pS		 = SDP.GetSurface() ; 					//	Récupère surface

	nHSurf	 = pS -> GetHandle() ; 					//	Récupère handle de la surface
	if ( nHSurf == - 1 )
	{
		return ( 1 );
	}

	nHGroup	 = ( nHSurf & 0xffff0000 ) >> 16 ; 		//	Handle du groupe

	if ( nHGroup == - 1 )
	{
		return 1 ; 								//	Pas de storage group pour la surface
	}

	nHSurf	 = nHSurf & 0xffff ; 					//	Handle surface

	pSG		 = _StorageGroupTab[ nHGroup ] ; 			//	Récupère adresse du groupe

	pSR		 = pSG -> GetSurfaceTab() ; 			//	Tableau de surface rec
	pSR		 = & pSR[ nHSurf ] ; 						//	SurfaceRec correspondante

	 // Fixe buffer pour l'edge
	pSR -> SetVertexBuffer( & _pVertexBuffer[ _nStartOffset ] );
	
	i0 = SDP.GetNbUVertex() ; 			//	Récupère nombre de vertex des bords
	i1 = SDP.GetNbVVertex();
	pSR -> SetNbUVertex( i0 ) ; 			//	Les sauve dans le SurfaceRec
	pSR -> SetNbVVertex( i1 );


	 // Met à jour offset du buffer de vertex pour les edges
	_nStartOffset += ( ( 2 * i0 ) + ( 2 * i1 ) );


	i		 = pSR -> GetIndexEdgeTab(); 			//	Index d'edge

	pE		 = pSG -> GetEdgeTab(); 				//	Tableau d'edge

	pE		 = & pE[ i ]; 							//	Récupère adresse du premier edge de la surface

	nNbrEdges	 = pSR -> GetNbEdges(); 				//	Récupère nombre d'edges


	pDE0	 = pSG -> GetDualEdgeTab(); 			//	Tableau des dual edges du group

	
	 //	Parcourt liste des edges de la surface 
	for ( i = 0 ; i < nNbrEdges ; ++i )
	{
		nTypeAS	 = pE -> GetTypeAs(); 				//	Type de l'edge ( edge 1 ou 2 )

		j		 = pE -> GetDualEdgeId(); 			//	Index du dual edge
		if ( j < 0 )
		{
			continue ; 							//	Si pas de dual edge on passe à l'edge suivant
		}
		pDE1	 = & pDE0[ j ]; 						//	Adresse du dual edge

		
		 //	Vérification de l'etat actuel du dual edge 
		nStat	 = 	pDE1 -> GetStatus(); 			//	Récupère status actuel
		nStat	 |= 	( nTypeAS + 1 ); 				//	Ajoute status pour edge courant

		 //	Calcul eventuel de l'edge 
		if ( nStat == 3 )
		{

			 // Test de DEBUG 
//			if ( j == IndexToTest )					//	Supprimer cette ligne si test terminé
			{
				 // Fin test de debug

				 //	Ok , les deux edges ont été téssélés , donc on génère le strip
				GenerateStripEdge( pSG , pDE1 );
				pEdgeStripBuilder = _EdgeBuilder.GetStripEdgeBuilder();

#ifndef __RENDERMANAGER__

				DISPLAY::Instance() -> DisplayStripEdgeSurface( *pEdgeStripBuilder , SDP );

#else

				//TODO:Portage RENDERMANAGER

#endif

				nStat = 0 ; 				//	Remet le status à 0 pour frame suivante
			}
		}


		//	Fixe le nouveau status
		pDE1 -> SetStatus( nStat );


		//	Edge suivant
		++pE;

	}

	
	return ( 1 );
}

// Méthode de génération d'un strip pour jonction d'un edge avec un autre
int		SURFACE_MANAGER::GenerateStripEdge( STORAGE_GROUP *pSG , DUAL_EDGE *pDE )
{
	EDGE		*pE1 , *pE2 , *pE;
	SURFACE_REC	*pSR , *pSR1 , *pSR2;
	int			nIndex1 , nIndex2;
	int			nHandleSurf1 , nHandleSurf2;
	
	//	Récupère tableau d'edge
	pE = pSG -> GetEdgeTab();

	//	Récupère index des edges
	nIndex1 = pDE -> GetEdgeId1();
	nIndex2 = pDE -> GetEdgeId2();

	//	Récupère adresse des edges
	pE1 = & pE[ nIndex1 ];
	pE2 = & pE[ nIndex2 ];

	//	Index de surface
	nHandleSurf1 = pDE -> GetHandleSurf1();
	nHandleSurf2 = pDE -> GetHandleSurf2();

	//	Adresses
	pSR = pSG -> GetSurfaceTab();

	pSR1 = & pSR[ nHandleSurf1 ];
	pSR2 = & pSR[ nHandleSurf2 ];

	
	 //	 		 Analyse des types de bord pour récupérer 
	 //		les adresses de début des vertex et le sens 
	 //		du parcours						
	
	 //Init
	_EdgeBuilder.SetDualEdge( pDE ) ; 			//	Dualedge
	_EdgeBuilder.SetEdge1( pE1 ) ; 				//	Edge1
	_EdgeBuilder.SetEdge2( pE2 ) ; 				//	Edge2
	_EdgeBuilder.SetSurface1( pSR1 ) ; 			//	Surface1
	_EdgeBuilder.SetSurface2( pSR2 ) ; 			//	Surface2
	
	 // Calcul des adresses de vertex et autres coeffs
	_EdgeBuilder.BuildParam();

	

	return ( 1 );
}

//Initialisation à effectuer à chaque frame
int	SURFACE_MANAGER::BeginFrame( float rFrameRate )
{
	_nStartOffset = 0 ; 				//	Réinitialisation dans début du tableau
	
	if ( _nNbrStorageGroup == 1 )
	{
		ChangeGroup() ; 				//	Si un seul groupe , force le changement
	}

	//	Tesselation paramterization
	rFrameRate = TIME::GetFrameRate();
	SetLastTesselParam( GetTesselParam() );
	SetCurrentFrameRate( rFrameRate );
	ComputeTesselParam();
	return ( 1 );
}

// Initialisation à effectuer à la fin de chaque frame
int	SURFACE_MANAGER::EndFrame()
{
	_nCurrentHandle = - 1;
	return ( 1 );
}

//-----------------------------------------------------------------------------
// Name: SURFACE_MANAGER::ComputeTesselParam
// Object:
//00 - 12 - 21 CVI Created:
//-----------------------------------------------------------------------------
void	SURFACE_MANAGER::ComputeTesselParam()
{
	float		rTemp , rTempMax , rTempMin;
	float		rCurFr;
	float		rParam;
	float		rNewParam;

	//	If constant tesselation param , we go out
	if ( IsConstantTesselParam() )
	{
		SetTesselParam( 1.0f );
		return;
	}

	//	Compute new parameter
	rCurFr	 = GetCurrentFrameRate();
	rTemp	 = GetRateToKeep();
	if ( rTemp < 0.001f )
	{
		return;
	}

	if ( fabs( rCurFr - rTemp ) / rTemp < 0.166 )
	{
		return;
	}

	if ( rCurFr < rTemp )
	{
		rTempMin	 = rTemp ; 		
		rParam		 = ( rTempMin - rCurFr ) / rTempMin;
//		rParam		 *= rParam;

		if ( rParam > 4.0f )
		{
			rParam = 4.0f;
		}
		rParam = rParam + 1.0f;
	}
	else
	{
		rTempMax	 = rTemp ; 		
		rParam		 = ( rTempMax - rCurFr ) / rTempMax;
//		rParam		 *= rParam;

		if ( rParam < - 0.999f )
		{
			rParam = - 0.999f;
		}
		rParam = rParam + 1.0f;
	}

	rNewParam		 = GetLastTesselParam();
	if ( rNewParam < 0.0001f )
	{
		rNewParam = 0.0001f;
	}

	rNewParam		 *= rParam;
	if ( rNewParam > 100.0f )
	{
		rNewParam = 100.0f;
	}
	
	SetTesselParam( rNewParam );
	
}*/

//=============================================================================
// CODE ENDS HERE
//=============================================================================
