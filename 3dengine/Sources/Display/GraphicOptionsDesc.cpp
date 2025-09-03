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
//	CLASS:	GRAPHIC_OPTIONS_DESC
//
//	02-03-27:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_3DENGINE(SceneGraph\Material)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

GRAPHIC_OPTIONS_DESC*	GRAPHIC_OPTIONS_DESC::_pInstance = NULL;
bool					GRAPHIC_OPTIONS_DESC::_bDestroyedOnce = false;

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
DESC *	GRAPHIC_OPTIONS_DESC::Instance()
{
	QDT_ASSERT(_bDestroyedOnce == false);

	if ( _pInstance == NULL )
	{
		_pInstance = (new GRAPHIC_OPTIONS_DESC());
	}
	
	return ( _pInstance );
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	GRAPHIC_OPTIONS_DESC::Suicide()
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
// Name:		GRAPHIC_OPTIONS_DESC constructor
// Object:
// 02-03-27		RMA Created:
//-----------------------------------------------------------------------------
GRAPHIC_OPTIONS_DESC::GRAPHIC_OPTIONS_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_GRAPHIC_OPTIONS, "GRAPHIC_OPTIONS", true, NBR_CMDS, 
	 "This class allows to access to all graphic options. You can configure them to privilegy render speed or visual quality.")
{
	// Lightmapping
	_pCommandDescsTab[0].Init(CMD_ENABLE_LIGHTMAPPING, 1, "EnableLightmapping", "Lightmapping is looking better than vertex color but is slower on some configurations wich don't support multi-texturing in one pass. Lightmapping consumes video memory.");
	_pCommandDescsTab[0].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[1].Init(CMD_IS_LIGHTMAPPING_ENABLED, 1, "IsLightmappingEnabled", "Check if lightmapping is enabled");
	_pCommandDescsTab[1].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
						  
	// Detail texturing
	_pCommandDescsTab[2].Init(CMD_ENABLE_DETAIL_TEXTURE, 1, "EnableDetailTexture", "Detail Texture are nice but is slower on some configurations wich don't support multi-texturing in one pass. Detail Texture consumes video memory.");
	_pCommandDescsTab[2].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[3].Init(CMD_IS_DETAIL_TEXTURE_ENABLED, 1, "IsDetailTextureEnabled", "Check if detail texture is enabled");
	_pCommandDescsTab[3].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
						  
	// Environnement mapping
	_pCommandDescsTab[4].Init(CMD_ENABLE_ENVIRONMENT_MAPPING, 1, "EnableEnvironmentMapping", "It adds a chrome effect on some materials. It consumes GPU power & video memory. It's slower on some configuration which don't support multi-texturing.");
	_pCommandDescsTab[4].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[5].Init(CMD_IS_ENVIRONMENT_MAPPING_ENABLED, 1, "IsEnvironmentEnabled", "Check if environment mapping is enabled");
	_pCommandDescsTab[5].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
		
	// Dynamic shadows
	_pCommandDescsTab[6].Init(CMD_ENABLE_DYNAMIC_SHADOWS, 1, "EnableDynamicShadows", "Dynamic shadows consume CPU power and video memory bandwith.");
	_pCommandDescsTab[6].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[7].Init(CMD_IS_DYNAMIC_SHADOWS_ENABLED, 1, "IsDynamicShadowsEnabled", "Check if dynamic shadows is enabled");
	_pCommandDescsTab[7].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
		
	// Self shadowing
	_pCommandDescsTab[8].Init(CMD_ENABLE_SELF_SHADOWING, 1, "EnableSelfShadowing", "Objects cast shadows on themselves.");
	_pCommandDescsTab[8].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[9].Init(CMD_IS_SELF_SHADOWING_ENABLED, 1, "IsSelfShadowingEnabled", "Check if self shadowing is enabled");
	_pCommandDescsTab[9].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");
		
	// Dynamic lighting
	_pCommandDescsTab[10].Init(CMD_ENABLE_DYNAMIC_LIGHTING, 1, "EnableDynamicLighting", "Dynamic lighting consumes GPU power");
	_pCommandDescsTab[10].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[11].Init(CMD_IS_DYNAMIC_LIGHTING_ENABLED, 1, "IsDynamiclightingEnabled", "Check if dynamic lighting is enabled");
	_pCommandDescsTab[11].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");

	// LOD
	_pCommandDescsTab[12].Init(CMD_ENABLE_LOD, 1, "EnableLOD", "Dynamic LOD reduces polygones when objects are far.");
	_pCommandDescsTab[12].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[13].Init(CMD_IS_LOD_ENABLED, 1, "IsLODEnabled", "Check if dynamic LOD is enabled");
	_pCommandDescsTab[13].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");

	// Shadows LOD
	_pCommandDescsTab[14].Init(CMD_ENABLE_SHADOWS_LOD, 1, "EnableShadowsLOD", "Dynamic shadows LOD reduces resolution of the shadow. It disables self shadowing.");
	_pCommandDescsTab[14].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	_pCommandDescsTab[15].Init(CMD_IS_SHADOWS_LOD_ENABLED, 1, "IsShadowsLODEnabled", "Check if dynamic self shadowing is enabled");
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bValue");

	// Texture detail
	_pCommandDescsTab[16].Init(CMD_SET_TEXTURES_DETAIL, 1, "SetTexturesDetail", "This parameter can decrease or increase the resolution of textures");
	_pCommandDescsTab[16].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "eLevel");
	_pCommandDescsTab[17].Init(CMD_GET_TEXTURES_DETAIL, 1, "GetTexturesDetail", "Get textures level of detail");
	_pCommandDescsTab[17].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::OUT_PARAMETER, "eLevel");

	// Polygones detail
	_pCommandDescsTab[18].Init(CMD_SET_POLYGONES_DETAIL, 1, "SetPolygonesDetail", "This parameter can decrease or increase the resolution of polygones");
	_pCommandDescsTab[18].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "eLevel");
	_pCommandDescsTab[19].Init(CMD_GET_POLYGONES_DETAIL, 1, "GetPolygonesDetail", "Get polygones level of detail");
	_pCommandDescsTab[19].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::OUT_PARAMETER, "eLevel");

	CreateEnumTab(3);
	SetEnum(0, "HIGH_DETAIL",	0);
	SetEnum(1, "MEDIUM_DETAIL", 1);
	SetEnum(2, "LOW_DETAIL",	2);
}

//-----------------------------------------------------------------------------
// Name:		GRAPHIC_OPTIONS_DESC destructor
// Object:
// 02-03-27:	RMA Created:
//-----------------------------------------------------------------------------
GRAPHIC_OPTIONS_DESC::~GRAPHIC_OPTIONS_DESC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CAMERA_DESC::New
//	Object: 
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
COM_OBJECT*	GRAPHIC_OPTIONS_DESC::New()
{
	if (_nNbObjects == 0)
	{
		_nNbObjects++;
		return new GRAPHIC_OPTIONS;
	}
	else
	{
		_nNbObjects++;
		return GRAPHIC_OPTIONS::Instance();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
void	GRAPHIC_OPTIONS_DESC::Delete(class COM_OBJECT *pC)
{
	_nNbObjects--;
	if (_nNbObjects == 0)
	{
		delete pC;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GRAPHIC_OPTIONS_DESC::ReceiveCommandProc
//	Object: 
//	02-03-27:	RMA - Created
//-----------------------------------------------------------------------------
bool GRAPHIC_OPTIONS_DESC::ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK & Stack) const
{
	QDT_ASSERT(CommandID > CMD_UNKNOWN);
	QDT_ASSERT(CommandID <= NBR_CMDS);

	switch(CommandID)
	{
	default:
		QDT_FAIL();

	case CMD_ENABLE_LIGHTMAPPING :
		MATERIAL_MANAGER::Instance()->SetLightMapping(Stack[0]);
		break;

	case CMD_IS_LIGHTMAPPING_ENABLED :
		Stack.SetBool(0, MATERIAL_MANAGER::Instance()->GetLightMapping());
		break;

	case CMD_ENABLE_DETAIL_TEXTURE :
		MATERIAL_MANAGER::Instance()->SetDetailTexture(Stack[0]);
		break;

	case CMD_IS_DETAIL_TEXTURE_ENABLED :
		Stack.SetBool(0, MATERIAL_MANAGER::Instance()->GetDetailTexture());
		break;

	case CMD_ENABLE_ENVIRONMENT_MAPPING :
		MATERIAL_MANAGER::Instance()->SetEnvironnementMapping(Stack[0]);
		break;

	case CMD_IS_ENVIRONMENT_MAPPING_ENABLED:
		Stack.SetBool(0, MATERIAL_MANAGER::Instance()->GetEnvironnementMapping());
		break;

	case CMD_ENABLE_DYNAMIC_SHADOWS :
		GRAPHIC_OPTIONS::Instance()->EnableDynamicShadows(Stack[0]);
		break;

	case CMD_IS_DYNAMIC_SHADOWS_ENABLED :
		Stack.SetBool(0, GRAPHIC_OPTIONS::Instance()->IsDynamicShadowsEnabled());
		break;

	case CMD_ENABLE_SELF_SHADOWING :
		GRAPHIC_OPTIONS::Instance()->EnableSelfShadowing(Stack[0]);
		break;

	case CMD_IS_SELF_SHADOWING_ENABLED :
		Stack.SetBool(0, GRAPHIC_OPTIONS::Instance()->IsSelfShadowingEnabled());
		break;

	case CMD_ENABLE_DYNAMIC_LIGHTING :
		GRAPHIC_OPTIONS::Instance()->EnableDynamicLighting(Stack[0]);
		break;

	case CMD_IS_DYNAMIC_LIGHTING_ENABLED :
		Stack.SetBool(0, GRAPHIC_OPTIONS::Instance()->IsDynamicLightingEnabled());
		break;

	case CMD_ENABLE_LOD :
		GRAPHIC_OPTIONS::Instance()->EnableLOD(Stack[0]);
		break;

	case CMD_IS_LOD_ENABLED :
		Stack.SetBool(0, GRAPHIC_OPTIONS::Instance()->IsLODEnabled());
		break;

	case CMD_ENABLE_SHADOWS_LOD :
		GRAPHIC_OPTIONS::Instance()->EnableShadowsLOD(Stack[0]);
		break;

	case CMD_IS_SHADOWS_LOD_ENABLED :
		Stack.SetBool(0, GRAPHIC_OPTIONS::Instance()->IsShadowsLODEnabled());
		break;

	case CMD_SET_TEXTURES_DETAIL :
		{
			int nLOD = Stack[0];
			GRAPHIC_OPTIONS::Instance()->SetTexturesDetail(static_cast<GRAPHIC_OPTIONS::GR_OP_LOD>(nLOD));
		}
		break;

	case CMD_GET_TEXTURES_DETAIL :
		Stack.SetInteger(0, GRAPHIC_OPTIONS::Instance()->GetTexturesDetail());
		break;

	case CMD_SET_POLYGONES_DETAIL :
		{
			int nLOD = Stack[0];
			GRAPHIC_OPTIONS::Instance()->SetPolygonesDetail(static_cast<GRAPHIC_OPTIONS::GR_OP_LOD>(nLOD));
		}
		break;

	case CMD_GET_POLYGONES_DETAIL :
		Stack.SetInteger(0, GRAPHIC_OPTIONS::Instance()->GetPolygonesDetail());
		break;
	}

	return (true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
