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
// CLASS: EDGE_STRIP_INFO
//
//
//00 - 12 - 22 CVI Created:
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

//*****************************************************************************
//
// CLASS DESCRIPTION ( EDGE_STRIP_INFO )
//
//	The EDGE_STRIP_INFO class implements ...
//
//*****************************************************************************

#include	"..\include.h"

#ifdef	_DEBUG
	#include	"EdgeStripInfo.inl"
#else
	#include	"../Inline.h"

#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BuildParam
//	Object:		Calcul des paramètres utilisés pour la construction du strip 
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
/*void	EDGE_STRIP_INFO::BuildParam()
{
	SURFACE		*pS;

	//	Récupère bords et sens
	_nBord1	 = 	_pEdge1 -> GetType();
	_nSens1	 = 	_nBord1 & 1;
//	_nBord1	> >= 1 ; 					//	Retire sens de parcours

	_nBord2	 = 	_pEdge2 -> GetType();
	_nSens2	 = 	_nBord2 & 1;
//	_nBord2	> >= 1 ; 					//	Retire sens de parcours

	
	 // On commence par la surface 1


	pS = _pSurface1 -> GetSurface() ; 		//	Récupère surface

	//	Déterminer s'il s'agit d'une courbe u ou v
	if ( _nBord1 > 3 )
	{
		//	bord de type V
		_prKnots1	 = pS -> GetBufferVKnots() ; 		//	Tableau de knots
		_rStart1	 = pS -> GetStartInV() ; 			//	valeur de début
		_rEnd1		= pS -> GetEndInV() ; 				//	valeur de fin

		//	Nombre de vertex sur la courbe
		_nNbrVertex1 = _pSurface1 -> GetNbVVertex() ; 		

		//	Décallage pour bord de type v
		_nOffset1 = _pSurface1 -> GetNbUVertex();

		if ( _nBord1 > 5 )
		{
			//	Courbe VEnd
			++_nOffset1;
		}
		
	}
	else
	{
		//	bord de type U
		_prKnots1	 = pS -> GetBufferUKnots() ; 		//	Tableau de knots
		_rStart1	 = pS -> GetStartInU() ; 			//	valeur de début
		_rEnd1	 = pS -> GetEndInU() ; 				//	valeur de fin
		//	Nombre de vertex sur la courbe
		_nNbrVertex1 = _pSurface1 -> GetNbUVertex() ; 		

		//	Courbe UEnd
		if ( _nBord1 > 1 )
		{
			//	Se place correctement sur le buffer
			_nOffset1 = _pSurface1 -> GetNbUVertex() + ( 2 * _pSurface1 -> GetNbVVertex() );
		}
		else
		{
			_nOffset1 = 0;
		}
	}

	 //Surface 2
	pS = _pSurface2 -> GetSurface() ; 		//	Récupère surface

	//	Déterminer s'il s'agit d'une courbe u ou v
	if ( _nBord2 > 3 )
	{
		//	bord de type V
		_prKnots2	 = pS -> GetBufferVKnots() ; 		//	Tableau de knots
		_rStart2	 = pS -> GetStartInV() ; 			//	valeur de début
		_rEnd2	 = pS -> GetEndInV() ; 				//	valeur de fin

		//	Nombre de vertex sur la courbe
		_nNbrVertex2 = _pSurface2 -> GetNbVVertex() ; 		

		//	Décallage pour bord de type v
		_nOffset2 = _pSurface2 -> GetNbUVertex();

		if ( _nBord2 > 5 )
		{
			//	Courbe VEnd
			++_nOffset2;
		}
	}
	else
	{
		//	bord de type U
		_prKnots2	 = pS -> GetBufferUKnots() ; 		//	Tableau de knots
		_rStart2	 = pS -> GetStartInU() ; 			//	valeur de début
		_rEnd2	 = pS -> GetEndInU() ; 				//	valeur de fin
		//	Nombre de vertex sur la courbe
		_nNbrVertex2 = _pSurface2 -> GetNbUVertex() ; 		

		//	Courbe UEnd
		if ( _nBord2 > 1 )
		{
			//	Se place correctement sur le buffer
			_nOffset2 = _pSurface2 -> GetNbUVertex() + ( 2 * _pSurface2 -> GetNbVVertex() );
		}
		else
		{
			_nOffset2 = 0;
		}


	}

	BuildEdgeStripBuilder();
}

//-----------------------------------------------------------------------------
//	Name:		BuildEdgeStripBuilder
//	Object:		Initialisation de la classe de construction du strip
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void	EDGE_STRIP_INFO::BuildEdgeStripBuilder()
{
	float			rIStart , rIEnd;
	int				nVertexOffsetStart;
	int				nVertexOffsetEnd;
	int				nNbrVertexForStrip;

	//	Index de début et de fin de l'edge
	rIStart	 = _pEdge1 -> GetIndexStart() ; 		//	Index de début
	rIEnd	 = _pEdge1 -> GetIndexEnd() ; 		//	Index de fin

	//	Offset du premier vertex
	nVertexOffsetStart = static_cast< int > ( rIStart * ( _nNbrVertex1 - 1 ) );
	//	Offset du dernier vertex
	nVertexOffsetEnd = static_cast< int > ( rIEnd * ( _nNbrVertex1 - 1 ) );

	//	We extend the intervalle
//	if ( _nSens1 )
//	{
//		//	Inverse case
//
//		//	End
//		nVertexOffsetEnd -= 1;
//		if ( nVertexOffsetEnd < 0 )
//		{
//			nVertexOffsetEnd  = 0;
//		}
//		//	Start
//		nVertexOffsetStart += 1;
//		if ( nVertexOffsetStart >= _nNbrVertex1 )
//		{
//			nVertexOffsetStart  = _nNbrVertex1 - 1;
//		}
//	}
//	else
//	{
		//	normal case

		//	Start
		nVertexOffsetStart -= 1;
		if ( nVertexOffsetStart < 0 )
		{
			nVertexOffsetStart  = 0;
		}
		//	End
		nVertexOffsetEnd += 1;
		if ( nVertexOffsetEnd >= _nNbrVertex1 )
		{
			nVertexOffsetEnd = _nNbrVertex1 - 1;
		}
//	}



	//	Nombre de vertex sur l'edge
	nNbrVertexForStrip = nVertexOffsetEnd - nVertexOffsetStart;
	if ( _nSens1 )
	{
		//	Si sens inverse , alors se place à la fin du buffer
		nVertexOffsetStart += nNbrVertexForStrip;
	}

	//	Actualise l'offset de départ suivant type de courbe
	if	( _nBord1 > 3 )
	{
		//	Sur courbes de type v , il y a alternance:
		//	VSTART , VEND - -> il faut donc multiplier par 2 l'offset
		nVertexOffsetStart *= 2;
	}
	//	Offset following curve type
	nVertexOffsetStart += _nOffset1;


	//	Vertex
	_pV1 = _pSurface1 -> GetVertexBuffer();
	_pV1 = & _pV1[ nVertexOffsetStart ];

	 // Fixe les paramètres 
	_StripBuilder.SetNbVertex1( nNbrVertexForStrip ) ; 			//	Nombre de vertex dans ce strip


	//	Surface 2

	//	Index de début et de fin de l'edge
	rIStart	 = _pEdge2 -> GetIndexStart() ; 		//	Index de début
	rIEnd	 = _pEdge2 -> GetIndexEnd() ; 		//	Index de fin

	//	Offset du premier vertex
	nVertexOffsetStart = static_cast< int > ( rIStart * ( _nNbrVertex2 - 1 ) );
	//	Offset du dernier vertex
	nVertexOffsetEnd = static_cast< int > ( rIEnd * ( _nNbrVertex2 - 1 ) );

	//	We extend the intervalle
//	if ( _nSens2 )
//	{
//		//	Inverse case
//
//		//	End
//		nVertexOffsetEnd -= 1;
//		if ( nVertexOffsetEnd < 0 )
//		{
//			nVertexOffsetEnd  = 0;
//		}
//		//	Start
//		nVertexOffsetStart += 1;
//		if ( nVertexOffsetStart >= _nNbrVertex2 )
//		{
//			nVertexOffsetStart  = _nNbrVertex2 - 1;
//		}
//	}
//	else
//	{
		//	normal case

		//	Start
		nVertexOffsetStart -= 1;
		if ( nVertexOffsetStart < 0 )
		{
			nVertexOffsetStart  = 0;
		}
		//	End
		nVertexOffsetEnd += 1;
		if ( nVertexOffsetEnd >= _nNbrVertex2 )
		{
			nVertexOffsetEnd = _nNbrVertex2 - 1;
		}
//	}


	//	Nombre de vertex sur l'edge
	nNbrVertexForStrip = nVertexOffsetEnd - nVertexOffsetStart;
	if ( _nSens2 )
	{
		//	Si sens inverse , alors se place à la fin du buffer
		nVertexOffsetStart += nNbrVertexForStrip;
	}

	//	Actualise l'offset de départ suivant type de courbe
	if	( _nBord2 > 3 )
	{
		//	Sur courbes de type v , il y a alternance:
		//	VSTART , VEND - -> il faut donc multiplier par 2 l'offset
		nVertexOffsetStart *= 2;
	}

	//	Offset following curve type
	nVertexOffsetStart += _nOffset2;


	//	Vertex
	_pV2 = _pSurface2 -> GetVertexBuffer();
	_pV2 = & _pV2[ nVertexOffsetStart ];

	 // Fixe les paramètres 
	_StripBuilder.SetNbVertex2( nNbrVertexForStrip ) ; 			//	Nombre de vertex dans ce strip



	 // Buffers de vertex 
	_StripBuilder.SetVertexBuffer( _pV1 , _pV2 );

	 // Fixe les bords
	_StripBuilder.SetBord1( _nBord1 );
	_StripBuilder.SetBord2( _nBord2 );
	_StripBuilder.SetSens1( _nSens1 );
	_StripBuilder.SetSens2( _nSens2 );
}*/

//=============================================================================
// CODE ENDS HERE
//=============================================================================
