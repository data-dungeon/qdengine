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
// CLASS: BONE
//
//
//	00-11-09	GHP - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (BONE)
//
//	The BONE class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Bone/Bone)
#include	INCL_3DENGINE(SceneGraph/Node/Node)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: BONE constructor
// Object: 
//00-11-09 GHP Created: 
//-----------------------------------------------------------------------------
BONE::BONE(NODE* pFather, NODE* pSon)
:
_pFather	( pFather ) ,
_pSon		( pSon ) ,
_rS_BASE	( 0.0f ) ,
_refDir		( 0.0f, 0.0f, -1.0f) ,
_normTip	( 0.0f, .0f, 0.0f ) ,
_col		( (unsigned int)0xfff000f0 ) ,
_colFloat	( _col ) , 
_org		( 0.0f, 0.0f, 0.0f ) ,
_tipPoint	( 0.0f, 0.0f, 0.0f ) ,
_p1			(  0.000f ,	_rS_BASE,   1.000f ) ,
_p2			(  0.866f ,	_rS_BASE,  -0.500f ) ,
_p3			( -0.866f ,	_rS_BASE,  -0.500f ) ,
_seg		( _org, _tipPoint) , 
_s01		( _org, _p1) , 
_s02		( _org, _p2) , 
_s03		( _org, _p3) , 
_s41		( _tipPoint, _p1) , 
_s42		( _tipPoint, _p2) , 
_s43		( _tipPoint, _p3) , 
_s12		( _p1, _p2) , 
_s23		( _p2, _p3) , 
_s31		( _p3, _p1)
{
	_rotMatrix.Identity();

	_sonMat = _pSon->GetLocalMatrix();
	_fatherMat = _pFather->GetGlobalMatrix();
}
//-----------------------------------------------------------------------------
// Name: BONE constructor
// Object: 
//00-11-13 GHP Created: 
//-----------------------------------------------------------------------------
BONE::BONE(const BONE& Bone)
{
	if ( this != &Bone ) 
	{
		_pFather	= Bone._pFather ;
		_pSon		= Bone._pSon ; 
		_rS_BASE	= Bone._rS_BASE ; 
		_refDir		= Bone._refDir ;
		_normTip	= Bone._normTip ; 
		_col		= Bone._col ;
		_colFloat	= Bone._colFloat ;
		_org		= Bone._org ;
		_tipPoint	= Bone._tipPoint  ;
		_p1			= Bone._p1 ; 
		_p2			= Bone._p2 ;
		_p3			= Bone._p3 ;
		_seg		= Bone._seg ;
		_s01		= Bone._s01 ;
		_s02		= Bone._s02 ;
		_s03		= Bone._s03 ;
		_s41		= Bone._s41 ; 
		_s42		= Bone._s42 ; 
		_s43		= Bone._s43 ; 
		_s12		= Bone._s12 ; 
		_s23		= Bone._s23 ;
		_s31		= Bone._s31 ; 
		_sonMat		= Bone._sonMat ;
		_fatherMat	= Bone._fatherMat ;
		
		int i = 0 ; 
		
		for ( i ; i < BONE_NB_VERT ; ++ i )
		{	
			_pPointsTab[i]	= Bone._pPointsTab[i] ;	
			_pNormalsTab[i] = Bone._pNormalsTab[i] ;
			_pColorTab[i]	= Bone._pColorTab[i] ;	
		}
	}
}
//-----------------------------------------------------------------------------
// Name: BONE destructor
// Object: 
//00-11-09 GHP Created: 
//-----------------------------------------------------------------------------
BONE::~BONE()
{
	_pFather	= NULL ; 
	_pSon		= NULL ;
}

//-----------------------------------------------------------------------------
// Name: BONE::DisplayDummy
// Object: 
//00-11-09 GHP Created: 
//-----------------------------------------------------------------------------
void BONE::Display(const MATRIX & Matrix)
{
	QDT_NOT_IMPLEMENTED();
/*
	_sonMat = _pSon->GetLocalMatrix() ;
	_tipPoint = _sonMat.GetTranslation() ;	
	_normTip = _tipPoint;
	_normTip.Normalize() ;
	// $$$$ joints at the same position happends only at root 
	if ( _normTip.IsNull() )  
	{
		return ;
	}
	
	_fatherMat = _pFather->GetGlobalMatrix() ; 
	// statndard for any display function

	RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&_fatherMat);

	// get the matrix ready 
	_rotMatrix.Identity();
	_rotMatrix.SetRotation( _refDir,_normTip, 0 ) ; 

	// compute a 1/3 bone length ratio
	_rS_BASE	= - MATH::Abs( _tipPoint.GetY() / 3.0f ) ;
	if ( _rS_BASE < -1.0f )
	{
		_rS_BASE = -1.0f ;
	}
	else if ( _rS_BASE > -0.2f )
	{
		_rS_BASE = -0.2f ;
	}
	// the 5 point composing the polyhedron
	_org.SetXYZ(  0.000f,    0.000f, 0.000f ) ;
	_p1.SetXYZ (  0.000f *	_rS_BASE ,   1.000f *	_rS_BASE ,	_rS_BASE ) ;
	_p2.SetXYZ (  0.866f *	_rS_BASE ,  -0.500f *	_rS_BASE ,	_rS_BASE ) ;
	_p3.SetXYZ ( -0.866f *	_rS_BASE ,  -0.500f *	_rS_BASE ,	_rS_BASE ) ;
	// rotates the 3 ones in the middle so the plane of that 
	// is orthogonal to the bone axis
//	_p1.Transform( _rotMatrix ) ;
//	_p2.Transform( _rotMatrix ) ;
//	_p3.Transform( _rotMatrix ) ;
	// creates the various segment to draw the outline
	// the bone "core"
	_seg.SetPoints( _org, _tipPoint ) ; 
	// the starting terahedron
	_s01.SetPoints ( _org, _p1) ; 
	_s02.SetPoints ( _org, _p2) ; 
	_s03.SetPoints ( _org, _p3) ; 
	// the ending tetrahedron
	_s41.SetPoints ( _tipPoint, _p1) ; 
	_s42.SetPoints ( _tipPoint, _p2) ; 
	_s43.SetPoints ( _tipPoint, _p3) ; 
	// the triangle edges
	_s12.SetPoints ( _p1, _p2) ; 
	_s23.SetPoints ( _p2, _p3) ; 
	_s31.SetPoints ( _p3, _p1) ; 
	// fill the vextex tab
	_pPointsTab[0] = _org ;
	_pPointsTab[1] = _p1  ;
	_pPointsTab[2] = _p2  ;
	_pPointsTab[3] = _p3 ;
	_pPointsTab[4] = _tipPoint ;
	// same for color
	_pColorTab[0].SetIntegerValue( 0xffff0000 ) ;
	_pColorTab[1].SetIntegerValue( 0xff00ff00 ) ;
	_pColorTab[2].SetIntegerValue( 0xff00ff00 ) ;
	_pColorTab[3].SetIntegerValue( 0xff00ff00 ) ;
	_pColorTab[4].SetIntegerValue( 0xff0000ff ) ;
	// and normal
	_pNormalsTab[0].SetXYZ(  0.000f,  1.0f, 0.000f) ;
	_pNormalsTab[1].SetXYZ(  1.000f,  0.0f, 0.000f) ;
	_pNormalsTab[2].SetXYZ(  0.866f,  0.0f,-0.500f) ;
	_pNormalsTab[3].SetXYZ( -0.866f,  0.0f,-0.500f) ;
	_pNormalsTab[4].SetXYZ(  0.000f, -1.0f, 0.000f) ;
	// send everything

#ifndef __RENDERMANAGER__

	DISPLAY::Instance()->SetVertexTab( _pPointsTab, _pColorTab, _pNormalsTab);
	// command the display of the strips
	int		pStrips1[] = { 0, 1, 2, 4 } ;
	DISPLAY::Instance()->DisplayStrip( 4, 0, pStrips1,static_cast<POINT2D *> ( NULL ) ) ;
	
	int		pStrips2[] = { 0, 2, 3, 4 } ;
	DISPLAY::Instance()->DisplayStrip( 4, 0, pStrips2,static_cast<POINT2D *> ( NULL ) ) ;
	
	int		pStrips3[] = { 0, 3, 1, 4 } ;
	DISPLAY::Instance()->DisplayStrip( 4, 0, pStrips3,static_cast<POINT2D *> ( NULL ) ) ;

#else

	//TODO:Portage RENDERMANAGER

#endif

#ifndef __RENDERMANAGER__

	// a single line in the middle of the bone linking the joints
	DISPLAY::Instance()->DisplayLine( _seg, _colFloat) ; 
	// display the outline of the bone
	// top tetrahedron
	DISPLAY::Instance()->DisplayLine( _s01, _colFloat) ;
	DISPLAY::Instance()->DisplayLine( _s02, _colFloat) ;
	DISPLAY::Instance()->DisplayLine( _s03, _colFloat) ;
	// mid triangle
	DISPLAY::Instance()->DisplayLine( _s41, _colFloat) ;
	DISPLAY::Instance()->DisplayLine( _s42, _colFloat) ;
	DISPLAY::Instance()->DisplayLine( _s43, _colFloat) ;
	// bottom tetrahedron
	DISPLAY::Instance()->DisplayLine( _s12, _colFloat) ;
	DISPLAY::Instance()->DisplayLine( _s23, _colFloat) ;
	DISPLAY::Instance()->DisplayLine( _s31, _colFloat) ;

#else

	// a single line in the middle of the bone linking the joints
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _seg, _colFloat) ; 
	// display the outline of the bone
	// top tetrahedron
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s01, _colFloat) ;
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s02, _colFloat) ;
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s03, _colFloat) ;
	// mid triangle
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s41, _colFloat) ;
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s42, _colFloat) ;
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s43, _colFloat) ;
	// bottom tetrahedron
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s12, _colFloat) ;
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s23, _colFloat) ;
	RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment( _s31, _colFloat) ;

#endif*/
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================4

