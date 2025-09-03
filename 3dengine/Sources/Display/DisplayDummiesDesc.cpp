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
//	CLASS:	DISPLAY_DUMMIES_DESC
//
//	02-06-04:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

DISPLAY_DUMMIES_DESC*	DISPLAY_DUMMIES_DESC::_pInstance = NULL;
bool					DISPLAY_DUMMIES_DESC::_bDestroyedOnce = false;

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
DESC *	DISPLAY_DUMMIES_DESC::Instance()
{
	QDT_ASSERT(_bDestroyedOnce == false);

	if ( _pInstance == NULL )
	{
		_pInstance = (new DISPLAY_DUMMIES_DESC());
	}
	
	return ( _pInstance );
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_DUMMIES_DESC::Suicide()
{
	QDT_ASSERT(_bDestroyedOnce == false);
	QDT_ASSERT(_pInstance != NULL);
	
	if (_pInstance != NULL)
	{
		delete _pInstance;
		_pInstance		 = NULL;
		_bDestroyedOnce	 = false;
	}
}
//-----------------------------------------------------------------------------
// Name:		DISPLAY_DUMMIES_DESC constructor
// Object:
// 02-03-27		RMA Created:
//-----------------------------------------------------------------------------
DISPLAY_DUMMIES_DESC::DISPLAY_DUMMIES_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_DISPLAY_DUMMIES, "DISPLAY_DUMMIES", true, NBR_CMDS, 
	 "This class allows you to display various dummies & informations for debuging.")
{
	// Bounding boxes
	_pCommandDescsTab[0].Init(CMD_SET_DISPLAY_BOUNDING_BOXES, 1, "DisplayBoundingBoxes", "Display bounding boxes of meshes in the window.");
	_pCommandDescsTab[0].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[1].Init(CMD_GET_DISPLAY_BOUNDING_BOXES, 1, "GetDisplayBoundingBoxes", "Check if bounding boxes of meshes are displaying.");
	_pCommandDescsTab[1].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Bounding spheres
	_pCommandDescsTab[2].Init(CMD_SET_DISPLAY_BOUNDING_SPHERES, 1, "DisplayBoundingSpheres", "Display bounding spheres of meshes in the window.");
	_pCommandDescsTab[2].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[3].Init(CMD_GET_DISPLAY_BOUNDING_SPHERES, 1, "GetDisplayBoundingSpheres", "Check if bounding spheres of meshes are displaying.");
	_pCommandDescsTab[3].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Light bounding volumes
	_pCommandDescsTab[4].Init(CMD_SET_DISPLAY_LIGHT_BOUNDING_VOLUMES, 1, "DisplayLightBV", "Display bounding volumes of lights in the window.");
	_pCommandDescsTab[4].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[5].Init(CMD_GET_DISPLAY_LIGHT_BOUNDING_VOLUMES, 1, "GetDisplayLightBV", "Check if bounding volumes of lights are displaying.");
	_pCommandDescsTab[5].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Light bounding volumes
	_pCommandDescsTab[6].Init(CMD_SET_DISPLAY_SKELETON_BOUNDING_VOLUMES, 1, "DisplaySkeletonBV", "Display bounding volumes of skeletons in the window.");
	_pCommandDescsTab[6].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[7].Init(CMD_GET_DISPLAY_SKELETON_BOUNDING_VOLUMES, 1, "GetDisplaySkeletonBV", "Check if bounding volumes of skeletons are displaying.");
	_pCommandDescsTab[7].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display lights
	_pCommandDescsTab[8].Init(CMD_SET_DISPLAY_LIGHTS, 1, "DisplayLights", "Display light dummies in the window.");
	_pCommandDescsTab[8].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[9].Init(CMD_GET_DISPLAY_LIGHTS, 1, "GetDisplayLights", "Check if light dummies are displaying.");
	_pCommandDescsTab[9].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display joints
	_pCommandDescsTab[10].Init(CMD_SET_DISPLAY_JOINTS, 1, "DisplayJoints", "Display joint dummies in the window.");
	_pCommandDescsTab[10].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[11].Init(CMD_GET_DISPLAY_JOINTS, 1, "GetDisplayJoints", "Check if joint dummies are displaying.");
	_pCommandDescsTab[11].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display skeletons
	_pCommandDescsTab[12].Init(CMD_SET_DISPLAY_SKELETONS, 1, "DisplaySkeletons", "Display skeleton dummies in the window.");
	_pCommandDescsTab[12].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[13].Init(CMD_GET_DISPLAY_SKELETONS, 1, "GetDisplaySkeletons", "Check if skeleton dummies are displaying.");
	_pCommandDescsTab[13].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display shadow volumes
	_pCommandDescsTab[14].Init(CMD_SET_DISPLAY_SHADOW_VOLUMES, 1, "DisplayShadowVolumes", "Display shadow volumes in the window.");
	_pCommandDescsTab[14].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[15].Init(CMD_GET_DISPLAY_SHADOW_VOLUMES, 1, "GetDisplayShadowVolumes", "Check if shadow volumes are displaying.");
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display quadtrees
	_pCommandDescsTab[16].Init(CMD_SET_DISPLAY_QUADTREES, 1, "DisplayQuadtrees", "Display quadtrees in the window.");
	_pCommandDescsTab[16].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[17].Init(CMD_GET_DISPLAY_QUADTREES, 1, "GetDisplayQuadtrees", "Check if quadtrees are displaying.");
	_pCommandDescsTab[17].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display particle emitters
	_pCommandDescsTab[18].Init(CMD_SET_DISPLAY_PARTICLE_EMITTERS, 1, "DisplayParticleEmitters", "Display particle emitters in the window.");
	_pCommandDescsTab[18].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[19].Init(CMD_GET_DISPLAY_PARTICLE_EMITTERS, 1, "GetDisplayParticleEmitters", "Check if particle emitters are displaying.");
	_pCommandDescsTab[19].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display force fields
	_pCommandDescsTab[20].Init(CMD_SET_DISPLAY_FORCE_FIELDS, 1, "DisplayForceFields", "Display force fields in the window.");
	_pCommandDescsTab[20].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[21].Init(CMD_GET_DISPLAY_FORCE_FIELDS, 1, "GetDisplayForceFields", "Check if force fields are displaying.");
	_pCommandDescsTab[21].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display bounding cylinders
	_pCommandDescsTab[22].Init(CMD_SET_DISPLAY_BOUNDING_CYLINDERS, 1, "DisplayBoundingCylinders", "Display bounding cylinders in the window.");
	_pCommandDescsTab[22].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[23].Init(CMD_GET_DISPLAY_BOUNDING_CYLINDERS, 1, "GetDisplayBoundingCylinders", "Check if bounding cylinders are displaying.");
	_pCommandDescsTab[23].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
	// Display physic
	_pCommandDescsTab[24].Init(CMD_SET_DISPLAY_PHYSIC, 1, "DisplayPhysic", "Display physic in the window.");
	_pCommandDescsTab[24].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[25].Init(CMD_GET_DISPLAY_PHYSIC, 1, "GetDisplayPhysic", "Check if physic is displaying.");
	_pCommandDescsTab[25].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");

	// Display physic
	_pCommandDescsTab[26].Init(CMD_SET_DISPLAY_COLLISION_MAP, 1, "DisplayCollisionMap", "Display collision map in the window.");
	_pCommandDescsTab[26].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[27].Init(CMD_GET_DISPLAY_COLLISION_MAP, 1, "GetDisplayCollisionMap", "Check if collision map is displaying.");
	_pCommandDescsTab[27].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
	
}

//-----------------------------------------------------------------------------
// Name:		DISPLAY_DUMMIES_DESC destructor
// Object:
// 02-03-27:	RMA Created:
//-----------------------------------------------------------------------------
DISPLAY_DUMMIES_DESC::~DISPLAY_DUMMIES_DESC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		New
//	Object: 
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
COM_OBJECT*	DISPLAY_DUMMIES_DESC::New()
{
	if (_nNbObjects == 0)
	{
		_nNbObjects++;
		return new DISPLAY_DUMMIES;
	}
	else
	{
		_nNbObjects++;
		return DISPLAY_DUMMIES::Instance();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_DUMMIES_DESC::Delete(class COM_OBJECT *pC)
{
	_nNbObjects--;
	if (_nNbObjects == 0)
	{
		delete pC;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_DUMMIES_DESC::ReceiveCommandProc
//	Object: 
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
bool DISPLAY_DUMMIES_DESC::ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK & Stack) const
{
	QDT_ASSERT(CommandID > CMD_UNKNOWN);
	QDT_ASSERT(CommandID <= NBR_CMDS);

	switch(CommandID)
	{
	default:
		QDT_FAIL();

	case CMD_SET_DISPLAY_BOUNDING_BOXES :
		DISPLAY_DUMMIES::Instance()->DisplayBoundingBoxes(Stack[0]);
		break;

	case CMD_GET_DISPLAY_BOUNDING_BOXES :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayBoundingBoxes());
		break;

	case CMD_SET_DISPLAY_BOUNDING_SPHERES :
		DISPLAY_DUMMIES::Instance()->DisplayBoundingSpheres(Stack[0]);
		break;

	case CMD_GET_DISPLAY_BOUNDING_SPHERES :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayBoundingSpheres());
		break;

	case CMD_SET_DISPLAY_LIGHT_BOUNDING_VOLUMES :
		DISPLAY_DUMMIES::Instance()->DisplayLightBoundingVolumes(Stack[0]);
		break;

	case CMD_GET_DISPLAY_LIGHT_BOUNDING_VOLUMES :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayLightBoundingVolumes());
		break;

	case CMD_SET_DISPLAY_SKELETON_BOUNDING_VOLUMES :
		DISPLAY_DUMMIES::Instance()->DisplaySkeletonBoundingVolumes(Stack[0]);
		break;

	case CMD_GET_DISPLAY_SKELETON_BOUNDING_VOLUMES :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplaySkeletonBoundingVolumes());
		break;

	case CMD_SET_DISPLAY_LIGHTS :
		DISPLAY_DUMMIES::Instance()->DisplayLights(Stack[0]);
		break;

	case CMD_GET_DISPLAY_LIGHTS :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayLights());
		break;

	case CMD_SET_DISPLAY_JOINTS :
		DISPLAY_DUMMIES::Instance()->DisplayJoints(Stack[0]);
		break;

	case CMD_GET_DISPLAY_JOINTS :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayJoints());
		break;

	case CMD_SET_DISPLAY_SKELETONS :
		DISPLAY_DUMMIES::Instance()->DisplaySkeletons(Stack[0]);
		break;

	case CMD_GET_DISPLAY_SKELETONS :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplaySkeletons());
		break;

	case CMD_SET_DISPLAY_SHADOW_VOLUMES :
		DISPLAY_DUMMIES::Instance()->DisplayShadowVolumes(Stack[0]);
		break;
		
	case CMD_GET_DISPLAY_SHADOW_VOLUMES :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayShadowVolumes());
		break;

	case CMD_SET_DISPLAY_QUADTREES :
		DISPLAY_DUMMIES::Instance()->DisplayQuadtrees(Stack[0]);
		break;

	case CMD_GET_DISPLAY_QUADTREES:
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayQuadtrees());
		break;

	case CMD_SET_DISPLAY_PARTICLE_EMITTERS :
		DISPLAY_DUMMIES::Instance()->DisplayParticleEmitters(Stack[0]);
		break;

	case CMD_GET_DISPLAY_PARTICLE_EMITTERS :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayParticleEmitters());
		break;

	case CMD_SET_DISPLAY_FORCE_FIELDS :
		DISPLAY_DUMMIES::Instance()->DisplayForceFields(Stack[0]);
		break;

	case CMD_GET_DISPLAY_FORCE_FIELDS :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayForceFields());
		break;		

	case CMD_SET_DISPLAY_BOUNDING_CYLINDERS :
		DISPLAY_DUMMIES::Instance()->DisplayBoundingCylinders(Stack[0]);
		break;

	case CMD_GET_DISPLAY_BOUNDING_CYLINDERS :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayBoundingCylinders());
		break;		

	case CMD_SET_DISPLAY_PHYSIC :
		DISPLAY_DUMMIES::Instance()->DisplayPhysic(Stack[0]);
		break;

	case CMD_GET_DISPLAY_PHYSIC :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayPhysic());
		break;

	case CMD_SET_DISPLAY_COLLISION_MAP :
		DISPLAY_DUMMIES::Instance()->DisplayCollisionMap(Stack[0]);
		break;
		
	case CMD_GET_DISPLAY_COLLISION_MAP :
		Stack.SetBool(0, DISPLAY_DUMMIES::Instance()->GetDisplayCollisionMap());
		break;
	}

	return (true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
