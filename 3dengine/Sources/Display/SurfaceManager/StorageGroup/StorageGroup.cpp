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
// CLASS: STORAGE_GROUP
//
//
//00-12-22 CVI Created:
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

//*****************************************************************************
//
// CLASS DESCRIPTION ( STORAGE_GROUP )
//
//	The STORAGE_GROUP class implements ...
//
//*****************************************************************************

#include	"..\include.h"
#ifdef	_DEBUG
	#include	"StorageGroup.inl"
#else
	#include	"../Inline.h"

#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
/*void	STORAGE_GROUP::Build( const STORAGE_GROUP & StorageGroup )
{
	int i;


	_nHandle = StorageGroup._nHandle;

	_nNbrEdges = StorageGroup._nNbrEdges;
	if ( _nNbrEdges )
	{
		_pEdgeTab	   = new EDGE[ _nNbrEdges ];
		_pnIndexEdgeTab = new int [ _nNbrEdges ];
		for ( i = 0 ; i < _nNbrEdges ; ++i )
		{
			_pEdgeTab[ i ].Build( StorageGroup._pEdgeTab[ i ] );
			_pnIndexEdgeTab[ i ] = StorageGroup._pnIndexEdgeTab[ i ];
		}
	}

	_nNbrDualEdges = StorageGroup._nNbrDualEdges;
	if ( _nNbrDualEdges )
	{
		_pDualEdgeTab = new DUAL_EDGE[ _nNbrDualEdges ];
		for ( i = 0 ; i < _nNbrDualEdges ; ++i )
		{
			_pDualEdgeTab[ i ].Build( StorageGroup._pDualEdgeTab[ i ] );
		}
	}

	_nNbrSurfaces = StorageGroup._nNbrSurfaces;
	if ( _nNbrSurfaces )
	{
		_pSurfRecTab	= new SURFACE_REC[ _nNbrSurfaces ] ; 	

		for ( i = 0 ; i < _nNbrSurfaces ; ++i )
		{
			_pSurfRecTab[ i ].Build( StorageGroup._pSurfRecTab[ i ] );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Relocate
//	Object:		Rellocation des tableaux après le chargement
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
void		STORAGE_GROUP::Relocate( int nStart )
{
	//	Tableau des dual edges
	if ( _pDualEdgeTab )
	{
		_pDualEdgeTab = reinterpret_cast< DUAL_EDGE* >( reinterpret_cast< int >( _pDualEdgeTab ) + nStart );
	}

	//	Tableau des edges
	if ( _pEdgeTab )
	{
		_pEdgeTab = reinterpret_cast< EDGE* >( reinterpret_cast< int >( _pEdgeTab ) + nStart );
	}

	//	Tableau des index d'edge
	if ( _pnIndexEdgeTab )
	{
		_pnIndexEdgeTab = reinterpret_cast< int* >( reinterpret_cast< int >( _pnIndexEdgeTab ) + nStart );
	}

	//	Surface rec
	
	if ( _pSurfRecTab )
	{
		_pSurfRecTab = reinterpret_cast< SURFACE_REC* >( reinterpret_cast< int >( _pSurfRecTab ) + nStart );
	}
}

//-----------------------------------------------------------------------------
//	Name:		RegisterSurface
//	Object:		Enregistrement d'une surface
//	01-01-18:	CVI - Created
//-----------------------------------------------------------------------------
int		STORAGE_GROUP::RegisterSurface( SURFACE *pS )
{
	int h;

	//	Récupère handle
	h = pS -> GetHandle();
	h &= 0x0000ffff;

	_pSurfRecTab[ h ].SetSurface( pS );
	return ( 1 );
}*/

//=============================================================================
// CODE ENDS HERE
//=============================================================================
