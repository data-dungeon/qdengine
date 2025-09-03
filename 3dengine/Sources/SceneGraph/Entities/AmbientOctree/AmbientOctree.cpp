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
//	CLASS:	AMBIENT_OCTREE
//
//	06-03-13:	PDE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"
#include	"AmbientOctree.h"
#include	INCL_KMATH(Math/SphericalHarmonics/IrradianceVolume)
#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonics)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		AMBIENT_OCTREE constructor
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
AMBIENT_OCTREE::AMBIENT_OCTREE()
:
ENTITY_ABC(GetGenericCommunicatorDescriptor(), ENTITY_AMBIENT_OCTREE),
_pIrradianceVolume(NULL)
{
		// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AMBIENT_OCTREE destructor
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
AMBIENT_OCTREE::~AMBIENT_OCTREE()
{
	if (_pIrradianceVolume)
	{
		delete _pIrradianceVolume;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::Register()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->AddAmbientOctree(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::Unregister()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->RemoveAmbientOctree(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "AMBITREE", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("AMBITREE", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	AMBIENT_OCTREE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_AMBIENT_OCTREE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	AMBIENT_OCTREE::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("AMBIENT_OCTREE"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_AMBIENT_OCTREE,
											QDT_COMMENT("A3CAEFGH-F069-C8B7-1947-8B233AEB8B80"), 
											sizeof(AMBIENT_OCTREE), 
											ComInit, 
											QDT_COMMENT("AmbientOctree"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::EM_Construct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	AMBIENT_OCTREE *pAO;
	pAO = static_cast<AMBIENT_OCTREE *>(pO);

	QDT_NEW_PLACED(pAO, AMBIENT_OCTREE)();
	QDT_ASSERT(pAO);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::EM_Destruct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	AMBIENT_OCTREE *pAO;
	pAO = static_cast<AMBIENT_OCTREE *>(pO);

	QDT_DELETE_PLACED(pAO, AMBIENT_OCTREE);
}

//-----------------------------------------------------------------------------
//	Name:		SetIrradianceVolume
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::SetIrradianceVolume(IRRADIANCE_VOLUME *	pIrrVol)
{  // for now on the deletion is handled within Maya
	_pIrradianceVolume = pIrrVol;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::CM_ReadChunk1(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	AMBIENT_OCTREE * pOctree = static_cast<AMBIENT_OCTREE *>(pCommunicator);
	pOctree->_pIrradianceVolume = new IRRADIANCE_VOLUME(Stream);
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::CM_WriteChunk1(IO_STREAM &		Stream,
									   COMMUNICATOR *	pCommunicator)
{
	AMBIENT_OCTREE * pOctree = static_cast<AMBIENT_OCTREE *>(pCommunicator);
	Stream << *(pOctree->_pIrradianceVolume);
}

//-----------------------------------------------------------------------------
//	Name:		SampleIrradiance
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	AMBIENT_OCTREE::SampleIrradiance(const VECTOR &			Pos,
										 SPHERICAL_HARMONICS &	sh)
{
	if (_pIrradianceVolume)
	{
		_pIrradianceVolume->SampleIrradiance(&sh, Pos.GetX(), Pos.GetY(), Pos.GetZ());
	}
	else
	{
		// Set to black
		sh.Init();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
