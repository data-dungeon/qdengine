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
// CLASS: EDGE_STRIP_INFO
//
//
//00-12-22 CVI Created: 
//*****************************************************************************

class	EDGE;
class	DUAL_EDGE;
class	SURFACE_REC;

//=============================================================================
// Name: EDGE_STRIP_INFO
//00-12-22 CVI Created: 
//=============================================================================
/*class	EDGE_STRIP_INFO
{
public:
	_INLINE_	EDGE_STRIP_INFO();
	_INLINE_	~EDGE_STRIP_INFO();

	_INLINE_	void				SetSurface1( SURFACE_REC *pS );
	_INLINE_	void				SetSurface2( SURFACE_REC *pS );
	_INLINE_	void				SetEdge1( EDGE *pE );
	_INLINE_	void				SetEdge2( EDGE *pE );
	_INLINE_	void				SetDualEdge( DUAL_EDGE *pDE );

	_INLINE_	SURFACE_REC			*GetSurface1()const;
	_INLINE_	SURFACE_REC			*GetSurface2()const;
	_INLINE_	EDGE				*GetEdge1()const;
	_INLINE_	EDGE				*GetEdge2()const;
	_INLINE_	DUAL_EDGE			*GetDualEdge()const;
	_INLINE_	STRIP_EDGE_BUILDER	*GetStripEdgeBuilder();
	_INLINE_	const STRIP_EDGE_BUILDER	*GetStripEdgeBuilder()const;

				//Calcul des paramètres utilisés pour la construction du strip
				void			BuildParam();
				void			BuildEdgeStripBuilder();

private:
	SURFACE_REC			*_pSurface1;		//	Surfaces de l'edge
	SURFACE_REC			*_pSurface2;

	EDGE				*_pEdge1;		//	2 classes edge permettant le recoupement
	EDGE				*_pEdge2;
	DUAL_EDGE			*_pDualEdge;		//	Info du dualedge
	int					_nBord1;			//	Bord surface 1 (u0, u1, v0, v1)
	int					_nBord2;			//	Bord surface 2
	int					_nSens1;			//	Sens de parcours de la courbe
	int					_nSens2;
	VERTEX				*_pV1;			//	Buffers de vertex
	VERTEX				*_pV2;
	int					_nStep1;			//	Step de tesselation
	int					_nStep2;
	int					_nNbrVertex1;		//	Nombre de vertex tesselé sur la courbe
	int					_nNbrVertex2;
	float				*_prKnots1;		//	Vecteur de noeud de la courbe
	float				*_prKnots2;
	float				_rStart1;		//	Début tesselation
	float				_rStart2;
	float				_rEnd1;			//	Fin tesselation
	float				_rEnd2;
	int					_nOffset1;
	int					_nOffset2;

	STRIP_EDGE_BUILDER	_StripBuilder;
};*/

//=============================================================================
// CODE ENDS HERE
//=============================================================================
