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
// CLASS: DISPLAY_PARAMETERS
//
//
//	01-02-13:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (DISPLAY_PARAMETERS)
//
//	The DISPLAY_PARAMETERS class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/DisplayParameters/DisplayParameters)

#ifdef _DEBUG
	#include	"DisplayParameters.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_PARAMETERS constructor
//	Object:		
//	01-03-30:	ELE - Created
//-----------------------------------------------------------------------------
DISPLAY_PARAMETERS::DISPLAY_PARAMETERS()
:
_pNode						(NULL),
_pBoundingVolume			(NULL),
_pCurrentSG					(NULL),
_pSkinningMatrices			(NULL),
_pDynamicClusters			(NULL),
_pLightNodesOnDynamicLit	(NULL),
_pLightNodesOnPrelit		(NULL),
//_pLightSourcesOnDynamicLit	(NULL),
//_pLightSourcesOnPrelit		(NULL),
_bDoubleSidedMaterial		(false),
_bIgnoreGlobalMatrix		(false),
_bIsSkeletonInside			(false)
{
}
 
//-----------------------------------------------------------------------------
//	Name: DISPLAY_PARAMETERS constructor
//	Object: Copy constructor
//	01-02-13:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_PARAMETERS::DISPLAY_PARAMETERS(const DISPLAY_PARAMETERS & DisplayParameters)
{
	Copy(DisplayParameters);
}

//-----------------------------------------------------------------------------
//	Name: DISPLAY_PARAMETERS destructor
//	Object: Destructor
//	01-02-13:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_PARAMETERS::~DISPLAY_PARAMETERS()
{
}

//-----------------------------------------------------------------------------
//	Name: DISPLAY_PARAMETERS::operator 
//	Object: 
//	01-02-13:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_PARAMETERS & DISPLAY_PARAMETERS::operator = (const DISPLAY_PARAMETERS & DisplayParameters)
{
	if (this != &DisplayParameters)
	{
		Copy(DisplayParameters) ;
	}

	return (*this) ;
}

//-----------------------------------------------------------------------------
//	Name: DISPLAY_PARAMETERS::Copy
//	Object: Shallow copy method to avoid code duplication in operator = and
//			copy constructor
//	01-02-13:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_PARAMETERS::Copy(const DISPLAY_PARAMETERS & DisplayParameters)
{
	_pBoundingVolume			= DisplayParameters._pBoundingVolume;			
	_pCurrentSG					= DisplayParameters._pCurrentSG;			
	_pSkinningMatrices			= DisplayParameters._pSkinningMatrices;			
	_pDynamicClusters			= DisplayParameters._pDynamicClusters;			
	_pLightNodesOnDynamicLit	= DisplayParameters._pLightNodesOnDynamicLit;	
	_pLightNodesOnPrelit		= DisplayParameters._pLightNodesOnPrelit;		
//	_pLightSourcesOnDynamicLit	= DisplayParameters._pLightSourcesOnDynamicLit;
//	_pLightSourcesOnPrelit		= DisplayParameters._pLightSourcesOnPrelit;		
	_bDoubleSidedMaterial		= DisplayParameters._bDoubleSidedMaterial;		
	_bIgnoreGlobalMatrix		= DisplayParameters._bIgnoreGlobalMatrix;		
	_bIsSkeletonInside			= DisplayParameters._bIsSkeletonInside;			
	_CameraFrustrum				= DisplayParameters._CameraFrustrum;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateFrustrumFar
//	Object:		
//	04-08-13:	ELE - Created
//-----------------------------------------------------------------------------
float	DISPLAY_PARAMETERS::UpdateFrustrumFar(float rNewFar)
{
	float	rOldFar = _CameraFrustrum.GetFar();

	_CameraFrustrum.SetFar(rNewFar);

	return	(rOldFar);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
