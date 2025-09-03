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
// CLASS: ZONE
//
//
//	01-02-08:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__ ;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION (ZONE)
//
//	The ZONE class implements ...
//
//*****************************************************************************

#include	"include.h"

#include	INC_KRNCOM(BaseType)
#include	INC_KRNCOM(ComplexType)

#include	INC_OLDRENDERER(Display/RenderManager)
#include	INC_KRNCORE(Stream)
#include	INC_KRNCORE(Time)

#include	INC_3DENGINE(SceneGraph\Entities\Particles)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(Display)

#include	INC_KRNCORE(Profiler)

#ifdef _DEBUG
	#include	"Zone.inl"
#else
	#include	"Inline.h"

	#include	INL_OLDRENDERER(Display/RenderManager)
	#include	INL_KRNCORE(Math\Geometry\Shapes\Shapes1D\Vector)
	#include	INL_KRNCORE(Math/Color)

	#include	INL_3DENGINE(Display)
	#include	INL_3DENGINE(SceneGraph\Node)
	#include	INL_3DENGINE(SceneGraph\Entities\Particles)
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: ZONE constructor
//	Object: Default constructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
ZONE::ZONE()
:
ENTITY_ABC			(GetGenericCommunicatorDescriptor(), ENTITY_ZONE),
_DynamicAmbientColor(0.0f, 0.0f, 0.0f, 0.0f),
_StaticAmbientColor	(0.0f, 0.0f, 0.0f, 0.0f),
_bFogEnable			(false),
_rFogNear			(0.1f),
_rFogFar			(10000.0f),
_FogColor			(255, 127, 127, 127)
{	
	_pParticleSystem = new PARTICLE_SYSTEM;
}

//-----------------------------------------------------------------------------
//	Name:		ZONE constructor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
ZONE::ZONE(COMMUNICATOR_DESCRIPTOR *	pCD,
		   ENTITY_TYPE					eEntityType)
:
ENTITY_ABC			(pCD, eEntityType),
_DynamicAmbientColor(0.0f, 0.5f, 0.5f, 0.5f),
_StaticAmbientColor	(0.0f, 0.0f, 0.0f, 0.0f),
_bFogEnable			(false),
_rFogNear			(0.1f),
_rFogFar			(10000.0f),
_FogColor			(255, 127, 127, 127)
{	
	_pParticleSystem = new PARTICLE_SYSTEM;
}

//-----------------------------------------------------------------------------
//	Name: Destructor
//	Object: Destructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
ZONE::~ZONE()
{
	ResetLights();
	ResetParticleEmitters();
	ResetShadowEmitters();

	delete	(_pParticleSystem);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveLinks
//	Object:		
//	02-02-08:	DAC - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveLinks()
{
	if	(_pParticleSystem)
	{
		_pParticleSystem->RemoveLinks();
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayParticles
//	Object:		
//	03-09-16:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::DisplayParticles(DISPLAY_PARAMETERS & DisplayParameters)
{
	int					i, nNbrParticleEmitters = _vParticleEmitters.GetSize();
	NODE				*pNode;
	PARTICLE_EMITTER	*pEmitter;

#ifndef PLATFORM_PS2	
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	DISPLAY_MANAGER * pDisplay = DISPLAY_MANAGER::Instance();

	if (pDisplay->GetFilterType() > TFT_BILINEAR)
	{
		pContext->SetTextureFilterType(0, TFT_BILINEAR);
	}
#endif


	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Display particles", COLOR(255, 0, 255, 255));
	#endif
	#endif

	for	(i = 0 ; i < nNbrParticleEmitters ; ++i)
	{
		pNode = _vParticleEmitters[i];
		pEmitter = static_cast<PARTICLE_EMITTER *>(pNode->GetEntity());

		DisplayParameters.SetNode(pNode);
		pEmitter->DisplayParticles(DisplayParameters, true);
	}

	_pParticleSystem->Display();

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
	#endif
	#endif

	#ifndef PLATFORM_PS2		
		pContext->SetTextureFilterType(0, pDisplay->GetFilterType());
	#endif
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	ZONE::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::Update()
{
	_pParticleSystem->UpdateParticleSystem(TIME::Instance()->GetDefaultClock()->GetDeltaFrameTimeInS());
}

//-----------------------------------------------------------------------------
//	Name:		ToUpdate
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	ZONE::ToUpdate() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		InitParameters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::InitParameters(float rFogDec)
{
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	//	Set Ambient color
	_OldDynamicAmbientColor = pContext->GetGlobalAmbientLight();

	pContext->SetGlobalAmbientLight(_StaticAmbientColor);

	//	Fog
	_bOldFogEnable = pContext->IsFogEnabled();
	if (IsFogEnable() && DISPLAY_MANAGER::Instance()->IsFogEnabled())		
	{
		pContext->EnableFog(true);
	}
	else
	{
		pContext->EnableFog(false);
	}

	_rOldFogNear = pContext->GetFogStart();
	_rOldFogFar = pContext->GetFogEnd();
	_OldFogColor = pContext->GetFogColor();

	// J'ai retire le rFogDec, car de l'exterieur du diner j'ai mis
	// le même fog dans le diner pour eviter de voir super bien le diner
	pContext->SetFogStart(GetFogNear()/* + rFogDec*/);
	pContext->SetFogEnd(GetFogFar()/* + rFogDec*/);
	pContext->SetFogColor(GetFogColor());
}

//-----------------------------------------------------------------------------
//	Name:		ResetParameters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::ResetParameters()
{
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	//	Set Ambient color
	pContext->SetGlobalAmbientLight(_OldDynamicAmbientColor);

	//	Fog
	pContext->EnableFog(_bOldFogEnable);
	pContext->SetFogStart(_rOldFogNear);
	pContext->SetFogEnd(_rOldFogFar);
	pContext->SetFogColor(_OldFogColor);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *		pMD;
	ATTRIBUTE_DESCRIPTOR	*pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Attributes
	pAD = new	ATTRIBUTE_DESCRIPTOR("fogColor", QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_GetFogColor, EM_SetFogColor2);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("dynamicAmbientColor", QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_GetAmbientColor, EM_SetAmbientColor2);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("staticAmbientColor", QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_GetStaticAmbientColor, EM_SetStaticAmbientColor2);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("fogNear", QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_GetFogNear, EM_SetFogNear);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("fogFar", QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_GetFogFar, EM_SetFogFar);
	pCD->AddAttribute(pAD);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetAmbientColor"), QDT_COMMENT("Set ambient color of the zone"));
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("alpha component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("red component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("green component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),  QDT_COMMENT("blue component of the zone color"),	TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_AMBIENT_COLOR, pMD, EM_SetAmbientColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetAmbientColor"), QDT_COMMENT("Set ambient color of the zone"));
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_AMBIENT_COLOR2, pMD, EM_SetAmbientColor2);	

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetFogMode"), QDT_COMMENT("Enable or disable fog."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_FOG_MODE, pMD, EM_SetFogMode);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetFogNear"), QDT_COMMENT("Set fog near distance."));
	pMD->AddParameter(QDT_COMMENT("rDistance"), QDT_COMMENT("Distance in cm."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_FOG_NEAR, pMD, EM_SetFogNear);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetFogFar"), QDT_COMMENT("Set fog far distance."));
	pMD->AddParameter(QDT_COMMENT("rDistance"), QDT_COMMENT("Distance in cm."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_FOG_FAR, pMD, EM_SetFogFar);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetFogColor"), QDT_COMMENT("Set fog color of the zone"));
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("alpha component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("red component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("green component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),	 QDT_COMMENT("blue component of the zone color"),	TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_FOG_COLOR, pMD, EM_SetFogColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetFogColor"), QDT_COMMENT("Set fog color of the zone"));
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_FOG_COLOR2, pMD, EM_SetFogColor2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("ClearParticles"), QDT_COMMENT("Remove all particles generated in the zone"));
	pCD->AddMethod(CMD_ZONE_CLEAR_PARTICLES, pMD, EM_ClearParticles);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetFogColor"), QDT_COMMENT("Get the fog color."));
	pMD->SetReturn(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_GET_FOG_COLOR, pMD, EM_GetFogColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetStaticAmbientColor"), QDT_COMMENT("Get the static ambient color of the zone."));
	pMD->SetReturn(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_GET_STATIC_AMBIENT_COLOR, pMD, EM_GetStaticAmbientColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetDynamicAmbientColor"), QDT_COMMENT("Get the dynamic ambient color of the zone."));
	pMD->SetReturn(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_GET_DYN_AMBIENT_COLOR, pMD, EM_GetAmbientColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetStaticAmbientColor"), QDT_COMMENT("Set the static ambient color of the zone"));
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("alpha component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("red component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("green component of the zone color"),	TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),  QDT_COMMENT("blue component of the zone color"),	TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_STATIC_AMBIENT_COLOR, pMD, EM_SetStaticAmbientColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetStaticAmbientColor"), QDT_COMMENT("Set the static ambient color of the zone"));
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_STATIC_AMBIENT_COLOR2, pMD, EM_SetStaticAmbientColor2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetFogNear"), QDT_COMMENT("Get fog near distance."));
	pMD->AddParameter(QDT_COMMENT("rDistance"), QDT_COMMENT("Distance in cm."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_GET_FOG_NEAR, pMD, EM_GetFogNear);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetFogFar"), QDT_COMMENT("Get fog far distance."));
	pMD->AddParameter(QDT_COMMENT("rDistance"), QDT_COMMENT("Distance in cm."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_GET_FOG_FAR, pMD, EM_GetFogFar);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDynamicAmbientColor"), QDT_COMMENT("Set the dynamic ambient color of the zone"));
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_ZONE_SET_DYN_AMBIENT_COLOR, pMD, EM_SetDynamicAmbientColor);
	
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("ZONE    ", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("ZONE    ", 3, CM_ReadChunk3);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("ZONE    ", 3 ,CM_WriteChunk3);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ZONE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ZONE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ZONE::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("ZONE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ZONE,
										QDT_COMMENT("F99C28B6-E682-3BCA-3FF0-178A149801D8"), 
										sizeof(ZONE), 
										ComInit, 
										QDT_COMMENT("Zone of the world. Zone are connected by portals."),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));

}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_Construct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	ZONE *pZ;
	pZ = static_cast<ZONE *>(pO);

	QDT_NEW_PLACED(pZ, ZONE)();
	QDT_ASSERT(pZ);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_Destruct(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	ZONE *pZ;
	pZ = static_cast<ZONE *>(pO);

	QDT_DELETE_PLACED(pZ, ZONE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAmbientColor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetAmbientColor(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	struct PARAMETERS
	{
		int		_nA;
		int		_nR;
		int		_nG;
		int		_nB;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pZ->SetDynamicAmbientColor(COLOR(pParam->_nA, pParam->_nR, pParam->_nG, pParam->_nB));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAmbientColor2
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetAmbientColor2(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	COLOR*			pC = static_cast<COLOR*>(pP);

	pZ->SetDynamicAmbientColor(*pC);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFogMode
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetFogMode(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	bool*			pb = static_cast<bool*>(pP);

	pZ->SetFogEnable(*pb);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFogNear
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetFogNear(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	float*			pr = static_cast<float*>(pP);

	pZ->SetFogNear(*pr);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFogFar
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetFogFar(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	float*			pr = static_cast<float*>(pP);

	pZ->SetFogFar(*pr);
}


//-----------------------------------------------------------------------------
//	Name:		EM_GetFogNear
//	Object:		
//	04-01-13:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_GetFogNear(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE			*pInst			= static_cast<COM_INSTANCE*>(pO);
	ZONE					*pZone			= static_cast<ZONE*>(pInst->GetCommunicator());
	float					*prNear			= static_cast<float*>(pP);

	*prNear = pZone->GetFogNear();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetFogFar
//	Object:		
//	04-01-13:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_GetFogFar(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	COM_INSTANCE			*pInst			= static_cast<COM_INSTANCE*>(pO);
	ZONE					*pZone			= static_cast<ZONE*>(pInst->GetCommunicator());
	float					*prNear			= static_cast<float*>(pP);

	*prNear = pZone->GetFogFar();
}



//-----------------------------------------------------------------------------
//	Name:		EM_SetFogColor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetFogColor(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	struct PARAMETERS
	{
		int		_nA;
		int		_nR;
		int		_nG;
		int		_nB;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pZ->SetFogColor(COLOR(pParam->_nA, pParam->_nR, pParam->_nG, pParam->_nB));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFogColor2
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetFogColor2(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	COLOR*			pC = static_cast<COLOR*>(pP);

	pZ->SetFogColor(*pC);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearParticles
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_ClearParticles(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());

	pZ->GetParticleSystem()->Clear();
}


//-----------------------------------------------------------------------------
//	Name:		EM_GetFogColor
//	Object:		
//	03-12-10:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_GetFogColor(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE			*pFogInst	= static_cast<COM_INSTANCE*>(pO);
	ZONE					*pZone		= static_cast<ZONE*>(pFogInst->GetCommunicator());
	COLOR					*pColor		= static_cast<COLOR*>(pR);

	*pColor = pZone->GetFogColor();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetAmbientColor
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_GetAmbientColor(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE			*pFogInst	= static_cast<COM_INSTANCE*>(pO);
	ZONE					*pZone		= static_cast<ZONE*>(pFogInst->GetCommunicator());
	COLOR					*pColor		= static_cast<COLOR*>(pR);

	*pColor = pZone->GetDynamicAmbientColor();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetStaticAmbientColor
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_GetStaticAmbientColor(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE			*pFogInst	= static_cast<COM_INSTANCE*>(pO);
	ZONE					*pZone		= static_cast<ZONE*>(pFogInst->GetCommunicator());
	COLOR					*pColor		= static_cast<COLOR*>(pR);

	*pColor = pZone->GetStaticAmbientColor();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetStaticAmbientColor
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetStaticAmbientColor(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	struct PARAMETERS
	{
		int		_nA;
		int		_nR;
		int		_nG;
		int		_nB;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pZ->SetStaticAmbientColor(COLOR(pParam->_nA, pParam->_nR, pParam->_nG, pParam->_nB));
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetDynamicAmbientColor
//	Object:		
//	04-10-07:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetDynamicAmbientColor(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	COLOR*			pC = static_cast<COLOR*>(pP);

	pZ->SetDynamicAmbientColor(*pC);
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetStaticAmbientColor2
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	ZONE::EM_SetStaticAmbientColor2(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	ZONE *			pZ = static_cast<ZONE*>(pComInstance->GetCommunicator());
	COLOR*			pC = static_cast<COLOR*>(pP);

	pZ->SetStaticAmbientColor(*pC);
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::CM_ReadChunk2(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	ZONE *pZ;
	pZ = static_cast<ZONE *>(pCommunicator);

	int nDummy;
	COLOR_FLOAT Conversion;

	Stream >> pZ->_DynamicAmbientColor;
	Stream >> pZ->_BBox;
	Stream >> pZ->_BSphere;
	Stream >> pZ->_bFogEnable;
	Stream >> pZ->_rFogNear;
	Stream >> pZ->_rFogFar;

	Stream >> nDummy;
	Stream >> nDummy;
	Stream >> Conversion;

	pZ->_DynamicAmbientColor *= 0.5f;
	
	pZ->_FogColor = Conversion;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::CM_ReadChunk3(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	ZONE *pZ;
	pZ = static_cast<ZONE *>(pCommunicator);

	int nDummy;
	COLOR_FLOAT Conversion;

	Stream >> pZ->_DynamicAmbientColor;
	Stream >> pZ->_StaticAmbientColor;
	Stream >> pZ->_BBox;
	Stream >> pZ->_BSphere;
	Stream >> pZ->_bFogEnable;
	Stream >> pZ->_rFogNear;
	Stream >> pZ->_rFogFar;

	Stream >> nDummy;
	Stream >> nDummy;
	Stream >> Conversion;
	
	pZ->_FogColor = Conversion;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk2
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	ZONE::CM_WriteChunk3(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	ZONE *pZ;
	pZ = static_cast<ZONE *>(pCommunicator);
	int nDummy = 0;
	COLOR_FLOAT Conversion;

	Stream.DebugOutput("Zone :\t");
	Stream.DebugOutput(pZ->GetName());
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Dynamic Ambient color :\t");
	Stream << pZ->_DynamicAmbientColor;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Static Ambient color :\t");
	Stream << pZ->_StaticAmbientColor;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Bounding box :\t");
	Stream << pZ->_BBox;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Bounding sphere :\t");
	Stream << pZ->_BSphere;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Fog enable :\t");
	Stream << pZ->_bFogEnable;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Fog near :\t");
	Stream << pZ->_rFogNear;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Fog far :\t");
	Stream << pZ->_rFogFar;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Fog type :\t");
	Stream << nDummy;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Fog density :\t");
	Stream << nDummy;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Fog color :\t");
	Stream << (COLOR_FLOAT)(pZ->_FogColor);
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		AddParticleEmitter
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::AddParticleEmitter(NODE *	pParticleEmitterNode)
{
	if (NODE::Find(_vParticleEmitters, pParticleEmitterNode) == _vParticleEmitters.GetTail())
	{
		_vParticleEmitters.PushTail(pParticleEmitterNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveParticleEmitter
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveParticleEmitter(NODE *	pParticleEmitterNode)
{
	QDT_VECTOR <NODE *>::ITERATOR	It = NODE::Find(_vParticleEmitters, pParticleEmitterNode);

	if (It != _vParticleEmitters.GetTail())
	{
		_vParticleEmitters.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddShadowEmitter
//	Object:		
//	04-06-01:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::AddShadowEmitter(NODE *	pShadowEmitterNode)
{
	if (NODE::Find(_vShadowEmitters, pShadowEmitterNode) == _vShadowEmitters.GetTail())
	{
		_vShadowEmitters.PushTail(pShadowEmitterNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveShadowEmitter
//	Object:		
//	04-06-01:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveShadowEmitter(NODE *	pShadowEmitterNode)
{
	QDT_VECTOR <NODE *>::ITERATOR	It = NODE::Find(_vShadowEmitters, pShadowEmitterNode);

	if (It != _vShadowEmitters.GetTail())
	{
		_vShadowEmitters.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddLightOnDynamicLit
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::AddLightOnDynamicLit(NODE *	pLightNode)
{
	if (NODE::Find(_vLightsOnDynamicLit, pLightNode) == _vLightsOnDynamicLit.GetTail())
	{
		_vLightsOnDynamicLit.PushTail(pLightNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddLightOnPrelit
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::AddLightOnPrelit(NODE *	pLightNode)
{
	if (NODE::Find(_vLightsOnPrelit, pLightNode) == _vLightsOnPrelit.GetTail())
	{
		_vLightsOnPrelit.PushTail(pLightNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveLightOnDynamicLit
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveLightOnDynamicLit(NODE *	pLightNode)
{
	QDT_VECTOR <NODE *>::ITERATOR	It = NODE::Find(_vLightsOnDynamicLit, pLightNode);

	if (It != _vLightsOnDynamicLit.GetTail())
	{
		_vLightsOnDynamicLit.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveLightOnPrelit
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveLightOnPrelit(NODE *	pLightNode)
{
	QDT_VECTOR <NODE *>::ITERATOR	It = NODE::Find(_vLightsOnPrelit, pLightNode);

	if (It != _vLightsOnPrelit.GetTail())
	{
		_vLightsOnPrelit.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddMeshNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddMeshNode(NODE *	pNode)
{
	if (NODE::FindInList(_lMeshNodes, pNode) == _lMeshNodes.GetTail())
	{
		_lMeshNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddMeshGroupNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddMeshGroupNode(NODE *	pNode)
{
	if (NODE::FindInList(_lMeshGroupNodes, pNode) == _lMeshGroupNodes.GetTail())
	{
		_lMeshGroupNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddSkeletonNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddSkeletonNode(NODE *	pNode)
{
	if (NODE::FindInList(_lSkeletonNodes, pNode) == _lSkeletonNodes.GetTail())
	{
		_lSkeletonNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddParticleEmitterNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddCollisionMeshNode(NODE *	pNode)
{
	if (NODE::FindInList(_lCollisionMeshNodes, pNode) == _lCollisionMeshNodes.GetTail())
	{
		_lCollisionMeshNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddGlowNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddGlowNode(NODE *	pNode)
{
	if (NODE::FindInList(_lGlowNodes, pNode) == _lGlowNodes.GetTail())
	{
		_lGlowNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddPrimitiveNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddPrimitiveNode(NODE *	pNode)
{
	if (NODE::FindInList(_lPrimitiveNodes, pNode) == _lPrimitiveNodes.GetTail())
	{
		_lPrimitiveNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddTriggerNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddTriggerNode(NODE *	pNode)
{
	if (NODE::FindInList(_lTriggerNodes, pNode) == _lTriggerNodes.GetTail())
	{
		_lTriggerNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddWaypointNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddWaypointNode(NODE *	pNode)
{
	if (NODE::FindInList(_lWayPointNodes, pNode) == _lWayPointNodes.GetTail())
	{
		_lWayPointNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddForceFieldNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::AddForceFieldNode(NODE *	pNode)
{
	if (NODE::FindInList(_lForceFieldNodes, pNode) == _lForceFieldNodes.GetTail())
	{
		_lForceFieldNodes.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveMeshNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveMeshNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lMeshNodes, pNode);

	if (It != _lMeshNodes.GetTail())
	{
		_lMeshNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveMeshGroupNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveMeshGroupNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lMeshGroupNodes, pNode);

	if (It != _lMeshGroupNodes.GetTail())
	{
		_lMeshGroupNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSkeletonNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveSkeletonNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lSkeletonNodes, pNode);

	if (It != _lSkeletonNodes.GetTail())
	{
		_lSkeletonNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveParticleEmitterNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveCollisionMeshNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lCollisionMeshNodes, pNode);

	if (It != _lCollisionMeshNodes.GetTail())
	{
		_lCollisionMeshNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveGlowNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveGlowNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lGlowNodes, pNode);

	if (It != _lGlowNodes.GetTail())
	{
		_lGlowNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemovePrimitiveNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemovePrimitiveNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lPrimitiveNodes, pNode);

	if (It != _lPrimitiveNodes.GetTail())
	{
		_lPrimitiveNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveTriggerNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveTriggerNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lTriggerNodes, pNode);

	if (It != _lTriggerNodes.GetTail())
	{
		_lTriggerNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveWaypointNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveWaypointNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lWayPointNodes, pNode);

	if (It != _lWayPointNodes.GetTail())
	{
		_lWayPointNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveForceFieldNode
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::RemoveForceFieldNode(NODE *	pNode)
{
	QDT_DLIST <NODE *>::ITERATOR	It = NODE::FindInList(_lForceFieldNodes, pNode);

	if (It != _lForceFieldNodes.GetTail())
	{
		_lForceFieldNodes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetVisibility
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE::ResetVisibility(DISPLAY_PARAMETERS & DisplayParameters)
{
	QDT_DLIST<NODE *>::ITERATOR itEach;
	QDT_DLIST<NODE *>::ITERATOR itTail;

	itEach = _lMeshNodes.GetHead();
	itTail = _lMeshNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->ResetVisibilityMesh(DisplayParameters);
		itEach++;
	}

	itEach = _lMeshGroupNodes.GetHead();
	itTail = _lMeshGroupNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->ResetVisibilityMeshGroup(DisplayParameters);
		itEach++;
	}

	itEach = _lSkeletonNodes.GetHead();
	itTail = _lSkeletonNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->ResetVisibilitySkeleton(DisplayParameters);
		itEach++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayNoShadowMapTree
//	Object:		
//	04-07-21:	RMA - Created
//  05-04-27:   RMA - Modified : For optimization purpose, filtering is set
//				to bilinear for filters
//-----------------------------------------------------------------------------
void	ZONE::DisplayNoShadowMapTree(DISPLAY_PARAMETERS &	DisplayParameters)
{
#ifndef PLATFORM_PS2
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();		
	DISPLAY_MANAGER * pDisplay = DISPLAY_MANAGER::Instance();
#endif

	QDT_DLIST<NODE *>::ITERATOR itEach;
	QDT_DLIST<NODE *>::ITERATOR itTail;

	// First displaying entities which have resetVisibility
	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Display meshes group", COLOR(255, 255, 255, 0));
	#endif
	#endif

	BEGIN_PROFILE(  PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_MESH_GROUP  );
	itEach = _lMeshGroupNodes.GetHead();
	itTail = _lMeshGroupNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
		itEach++;
	}
	END_PROFILE(  PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_MESH_GROUP  );

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
	#endif
	#endif

	// First displaying entities which have resetVisibility
	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Display meshes", COLOR(255, 255, 255, 255));
	#endif
	#endif
	
	BEGIN_PROFILE(  PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_MESH  );
	itEach = _lMeshNodes.GetHead();
	itTail = _lMeshNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->DisplayNoShadowMapTreeMesh(DisplayParameters);
		itEach++;
	}
	END_PROFILE(  PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_MESH  );

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
	#endif
	#endif

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Display skeletons", COLOR(255, 255, 128, 128));
	#endif
	#endif

	BEGIN_PROFILE( PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_MESH_SKELETON );
	itEach = _lSkeletonNodes.GetHead();
	itTail = _lSkeletonNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->DisplayNoShadowMapTreeSkeleton(DisplayParameters);
		itEach++;
	}
	END_PROFILE(  PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_MESH_SKELETON  );

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
	#endif
	#endif

	BEGIN_PROFILE( PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_GLOW );

#ifndef PLATFORM_PS2		
	if (pDisplay->GetFilterType() > TFT_BILINEAR)
	{
		pContext->SetTextureFilterType(0, TFT_BILINEAR);
	}
#endif

	itEach = _lGlowNodes.GetHead();
	itTail = _lGlowNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
		itEach++;
	}

#ifndef PLATFORM_PS2		
	pContext->SetTextureFilterType(0, pDisplay->GetFilterType());
#endif

	END_PROFILE(  PROFILE_DISPLAY_NO_SHADOW_MAP_TREE_GLOW  );

	// Optimizable dans la mesure ou on appelle le UpdateLightSource sur les lights 
	// pour mettre à jour les lights sources, ce qui pourrait être fait une seule 
	// fois par frame (Après toutes les animations)
	BEGIN_PROFILE( PROFILE_UPDATE_LIGHT_SOURCE );
	unsigned int i, nCount;
	nCount = _vLightsOnDynamicLit.GetSize();
	for (i = 0 ; i < nCount ; ++i)
	{
		_vLightsOnDynamicLit[i]->UpdateLightSource(DisplayParameters);
	}

	nCount = _vLightsOnPrelit.GetSize();
	for (i = 0 ; i < nCount ; ++i)
	{
		_vLightsOnPrelit[i]->UpdateLightSource(DisplayParameters);
	}
	END_PROFILE( PROFILE_UPDATE_LIGHT_SOURCE );
	//

#ifndef _MASTER
	itEach = _lPrimitiveNodes.GetHead();
	itTail = _lPrimitiveNodes.GetTail();
	while (itEach != itTail)
	{
		(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
		itEach++;
	}

	if (DISPLAY_MANAGER::Instance()->DisplayNavigationMesh())
	{
		itEach = _lCollisionMeshNodes.GetHead();
		itTail = _lCollisionMeshNodes.GetTail();
		while (itEach != itTail)
		{
			(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
			itEach++;
		}
	}

	if (DISPLAY_MANAGER::Instance()->DisplayTriggerVolumes() == true)
	{
		itEach = _lTriggerNodes.GetHead();
		itTail = _lTriggerNodes.GetTail();
		while (itEach != itTail)
		{
			(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
			itEach++;
		}
	}

	if (DISPLAY_MANAGER::Instance()->DisplayWayPoints() == true)
	{
		itEach = _lWayPointNodes.GetHead();
		itTail = _lWayPointNodes.GetTail();
		while (itEach != itTail)
		{
			(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
			itEach++;
		}
	}

	if (DISPLAY_MANAGER::Instance()->DisplayForceFields() == true)
	{
		itEach = _lForceFieldNodes.GetHead();
		itTail = _lForceFieldNodes.GetTail();
		while (itEach != itTail)
		{
			(*itEach)->DisplayNoShadowMapTreeEntity(DisplayParameters);
			itEach++;
		}
	}

	if (DISPLAY_MANAGER::Instance()->DisplayLightBoundingVolumes() == true ||
		DISPLAY_MANAGER::Instance()->DisplayLights() == true)
	{
		unsigned int i, nCount;

		nCount = _vLightsOnDynamicLit.GetSize();
		for (i = 0 ; i < nCount ; ++i)
		{
			_vLightsOnDynamicLit[i]->DisplayNoShadowMapTreeEntity(DisplayParameters);
		}

		nCount = _vLightsOnPrelit.GetSize();
		for (i = 0 ; i < nCount ; ++i)
		{
			_vLightsOnPrelit[i]->DisplayNoShadowMapTreeEntity(DisplayParameters);
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplayPhysics
//	Object:		
//	05-01-21:	RMA - Created
//-----------------------------------------------------------------------------
void SCENE_GRAPH::DisplayPhysics() const
{
	if (GetPhysicScene())
	{
		GetPhysicScene()->DebugDisplay();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Grab
//	Object:		
//	05-09-01:	ELE - Created
//-----------------------------------------------------------------------------
bool	SCENE_GRAPH::Grab(NODE *	pNode)
{
	if	(_pPhysicScene)
	{
		RAY			Ray;
		QUATERNION	Q;
		MATRIX		M;
		VECTOR		Position;
		VECTOR		Direction(0.0f, 0.0f, -1.0f);

		pNode->GetGlobalPosition(Position);
		Q = pNode->GetQuaternion();

		M.SetQuaternion(Q);
		M.TransformDirection(Direction);
		Direction.Normalize();
		
		Ray.SetOrigin(Position);
		Ray.SetDirection(Direction);

		I_PHYSIC_RAYCAST_HIT	*pRaycastHit = PHYSIC_CONTEXT::Instance()->CreateRaycastHit();
		I_PHYSIC_ACTOR			*pRayCastedActor = _pPhysicScene->RaycastClosestShape(Ray, PSST_DYNAMIC_SHAPES, pRaycastHit);

		if	(pRayCastedActor)
		{
			// Actor
			I_PHYSIC_DYNAMIC_ACTOR	*pActor = pNode->GetDynamicActor();

			if	(pActor == NULL)
			{
				pActor = pNode->CreatePhysicDynamicActor();
			}

			float	rRadius;
			VECTOR	LocalPos(0.0f, 0.0f, 0.0f);
			VECTOR	FirstChildPos, Pos, V;

			I_PHYSIC_SHAPE_SPHERE * pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();
			pSphere->SetRadius(1.0f);
			pSphere->SetLocalPosition(VECTOR(0.0f, 0.0f, 0.0f));

			pActor->PushShape(pSphere);
			pActor->SetMass(2000.0f);
			pActor->SetGlobalMatrix(pNode->GetGlobalMatrix());
			pActor->SetLinearDamping(1.0f);
			pActor->SetAngularDamping(1.0f);
			
			_pPhysicScene->RegisterDynamicActor(pActor);

			pActor->EnableKinematicMode(true);

			// Joint
			I_PHYSIC_JOINT	*pJoint = pNode->GetAttachJoint();

			if	(pJoint == NULL)
			{
				pJoint = PHYSIC_CONTEXT::Instance()->CreateJointFixed();
				pNode->SetAttachJoint(pJoint);
			}

			VECTOR	JointGlobalAnchor;
			pNode->GetGlobalPosition(JointGlobalAnchor);

			pJoint->SetActor1(pActor);
			pJoint->SetActor2(pRayCastedActor);
			pJoint->SetGlobalAnchor(JointGlobalAnchor);
			pJoint->SetGlobalAxis(Direction);
			pJoint->EnableCollision(false);

			_pPhysicScene->RegisterJoint(pJoint);

			BEHAVIOR_MANAGER::Instance()->AddNodeWithPhysics(pNode);

			return	(true);
		}
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Ungrab
//	Object:		
//	05-09-02:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::Ungrab(NODE *	pNode)
{
	if	(_pPhysicScene && pNode)
	{
		// Actor
		I_PHYSIC_DYNAMIC_ACTOR	*pActor = pNode->GetDynamicActor();

		if	(pActor)
		{
			_pPhysicScene->UnregisterDynamicActor(pActor);
		}

		// Joint
		I_PHYSIC_JOINT	*pJoint = pNode->GetAttachJoint();

		if	(pJoint)
		{
			_pPhysicScene->UnregisterJoint(pJoint);
		}

		BEHAVIOR_MANAGER::Instance()->RemoveNodeWithPhysics(pNode);
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
