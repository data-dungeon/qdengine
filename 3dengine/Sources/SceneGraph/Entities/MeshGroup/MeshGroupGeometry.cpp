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
//	CLASS:	MESH_GROUP_GEOMETRY
//
//	03-09-01:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/MeshGroupGeometry)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KCORE(Profiler/Profiler)

#ifdef _DEBUG
	#include	"MeshGroupGeometry.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_GEOMETRY constructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP_GEOMETRY::MESH_GROUP_GEOMETRY()
:
_nSharedVertexBufferIndex(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_GEOMETRY destructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP_GEOMETRY::~MESH_GROUP_GEOMETRY()
{
//	delete _pGeometry;
//	_pGeometry = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::Display(unsigned int nIndex,
									 DISPLAY_PARAMETERS & DisplayParameters)
{
	BEGIN_PROFILE( PROFILE_MESHGROUP_DISPLAY );

	/*if (_ciMaterial.HasHandle())
	{
		MATERIAL_ABC * pMaterial = static_cast<MATERIAL_ABC *>(_ciMaterial.GetCommunicator());
	
		// The geometry don't receive a shadow, we must push it in its material
		if (pMaterial->GetNbrSortedObjects() == 0)
		{
			DISPLAY_MANAGER::Instance()->GetCurrentViewport()->PushMaterial(pMaterial);
		}
			
		pMaterial->PushObject(DisplayParameters.GetNode(), nIndex);
	}
	else
	{
		BEGIN_PROFILE( PROFILE_MESHGROUP_DISPLAYGEOMETRY );
		DisplayGeometryNoMaterial(DisplayParameters);
		END_PROFILE( PROFILE_MESHGROUP_DISPLAYGEOMETRY );
	}*/

	QDT_FAIL();

	END_PROFILE( PROFILE_MESHGROUP_DISPLAY );
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::DisplayHDR()
{
	QDT_NOT_IMPLEMENTED();
/*
	MATERIAL_ABC *				pMaterial = NULL;
	SHADER_ABC *				pShader = NULL;
	RENDER_CONTEXT *			pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
	RENDER_CONTEXT::CULL_MODE	CullMode = RmCullMode;
	RENDER_PIPELINE_ABC	*		pRP = pRC->GetRenderPipeline();

	if (IsDoubleSided())
	{
		CullMode = RENDER_CONTEXT::CULLMODE_NONE;
	}

	if	(_ciMaterial.HasHandle())
	{
		pMaterial = static_cast<MATERIAL_ABC *>(_ciMaterial.GetCommunicator());
		pShader = pMaterial->GetShader();

		if (pShader->IsDoubleSided())
		{
			CullMode = RENDER_CONTEXT::CULLMODE_NONE;
		}

		#ifdef _DEBUG
			pShader->SetName(pMaterial->GetName());			
		#endif
	}

	pRC->SetShader(pShader);
	pRC->SetCullMode(CullMode);
	
	// Clipping
	pRC->EnableClipping(DoClipping());

	// Bounding box
	pRC->SetBoundingVolume(&_BBox);
	pRC->SetBoundingVolumeForLightOptimize(&_BBox);
	
	// Alpha test
	pRC->EnableAlphaTest(false);

	// Render
	pRP->Display(_pGeometry, false);
	
	// Reset cullmode
	pRC->SetCullMode(RmCullMode);*/
}

//-----------------------------------------------------------------------------
//	Name:		SetLights
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetLights(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT * pRC = RENDER_MANAGER::Instance()->GetRenderContext();

	static QDT_VECTOR <NODE*>	vLightNodes;

	vLightNodes.Clear();

	BEGIN_PROFILE( PROFILE_MESHGROUP_FINDLIGHTS );
		FindLights(DisplayParameters, vLightNodes);
	END_PROFILE( PROFILE_MESHGROUP_FINDLIGHTS );

	// Lights
	if (vLightNodes.GetSize() != 0)
	{
		unsigned int i;

		for (i = 0 ; i < vLightNodes.GetSize() ; ++i)
		{
			LIGHT_ABC * pLight = static_cast<LIGHT_ABC *>(vLightNodes[i]->GetEntityInst().GetCommunicator());
			pRC->AddLightSource(pLight->GetLightSource());
		}
		pRC->EnableLighting(true);
	}
	else
	{
		pRC->EnableLighting(false);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		DisplaySorted
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::DisplaySorted(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT *			pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
	RENDER_CONTEXT::CULL_MODE	CullMode = RmCullMode;
	RENDER_PIPELINE_ABC	*		pRP = pRC->GetRenderPipeline();

	SetLights(DisplayParameters);

	if (IsDoubleSided() || DisplayParameters.IsDoubleSidedMaterial())
	{
		CullMode = RENDER_CONTEXT::CULLMODE_NONE;
	}

	pRC->SetCullMode(CullMode);
	pRC->EnableClipping(DoClipping());
	pRC->SetBoundingVolume(&_BBox);
	pRC->SetBoundingVolumeForLightOptimize(&_BBox);

	bool bFog = pRC->IsFogEnabled();
	pRC->EnableFog(GetEntityFog());

	pRP->Display(_pGeometry, false);

	pRC->EnableFog(bFog);

	pRC->ResetLights();
	pRC->SetCullMode(RmCullMode);*/
}

//-----------------------------------------------------------------------------
//	Name:		DisplayGeometry
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::DisplayGeometry(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	MATERIAL_ABC *				pMaterial = NULL;
	SHADER_ABC *				pShader = NULL;
	RENDER_CONTEXT *			pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
	RENDER_CONTEXT::CULL_MODE	CullMode = RmCullMode;
	RENDER_PIPELINE_ABC	*		pRP = pRC->GetRenderPipeline();
	bool						bPrelit = false;

	SetLights(DisplayParameters);

	if (IsDoubleSided())
	{
		CullMode = RENDER_CONTEXT::CULLMODE_NONE;
	}

	if	(_ciMaterial.HasHandle())
	{
		pMaterial = static_cast<MATERIAL_ABC *>(_ciMaterial.GetCommunicator());

		pShader = pMaterial->GetShader();

		if (pShader->IsDoubleSided())
		{
			CullMode = RENDER_CONTEXT::CULLMODE_NONE;
		}
		
		if (pMaterial->NeedLightsOnPrelit())
		{
			bPrelit = true;
		}

		#ifdef _DEBUG
			pShader->SetName(pMaterial->GetName());			
		#endif
	}

	pRC->SetShader(pShader);
	pRC->SetCullMode(CullMode);
	
	pRC->EnableClipping(DoClipping());
	pRC->SetBoundingVolume(&_BBox);
	pRC->SetBoundingVolumeForLightOptimize(&_BBox);
	pRC->EnableAlphaTest(false);

	pRP->Display(_pGeometry, bPrelit);
	
	pRC->ResetLights();
	pRC->SetCullMode(RmCullMode);*/
}

//-----------------------------------------------------------------------------
//	Name:		DisplayGeometryNoMaterial
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::DisplayGeometryNoMaterial(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT *			pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
	RENDER_CONTEXT::CULL_MODE	CullMode = RmCullMode;

	if (IsDoubleSided())
	{
		CullMode = RENDER_CONTEXT::CULLMODE_NONE;
	}

	pRC->SetShader(NULL);
	pRC->SetCullMode(CullMode);
	pRC->EnableLighting(false);
	
	// Clipping
	pRC->EnableClipping(DoClipping());

	// Bounding box
	pRC->SetBoundingVolume(&_BBox);
	pRC->SetBoundingVolumeForLightOptimize(&_BBox);
	pRC->EnableAlphaTest(false);

	// Texture
	pRC->SetTexture(0, 0);

	// Render
	pRC->GetRenderPipeline()->Display(_pGeometry, false);
	
	// Reset cullmode
	pRC->SetCullMode(RmCullMode);*/
}

//-----------------------------------------------------------------------------
//	Name:		FindLights
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::FindLights(DISPLAY_PARAMETERS &	DisplayParameters,
										QDT_VECTOR <NODE *> &	vLights)
{
	/*if	(NeedLightsOnPrelit() | NeedLightsOnDynamicLit())
	{
		SCENE_GRAPH *pSG = DisplayParameters.GetCurrentSG();
		COLOR_FLOAT	AmbientColor;

		QDT_ASSERT(pSG);

		const QDT_VECTOR <NODE *> * pvPotentialLights = &pSG->GetLightsOnPrelit();
		AmbientColor = pSG->GetStaticAmbientColor();
		
		if (NeedLightsOnDynamicLit())
		{
			QDT_ASSERT(NeedLightsOnPrelit() == false);
			pvPotentialLights = &pSG->GetLightsOnDynamicLit();
			//AmbientColor = pZone->GetDynamicAmbientColor();
		}

		// FPR: Temporary disble this(zone ambient gestion)
//		RENDER_MANAGER::Instance()->GetRenderContext()->SetGlobalAmbientLight(AmbientColor);

		unsigned int i;
		for (i = 0 ; i < pvPotentialLights->GetSize() ; ++i)
		{
			NODE * pLightNode = (*pvPotentialLights)[i];
			LIGHT_ABC * pLight = static_cast<LIGHT_ABC*>(pLightNode->GetEntity());
			if (pLight->GetSwitch() && pLight->TestLighting(_BSphere, pLightNode->GetGlobalMatrix()))
			{	
				vLights.PushTail(pLightNode);
			}
		}
	}*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		IsVisible
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::IsVisible(DISPLAY_PARAMETERS &	DisplayParameters)
{
	DISPLAY_MANAGER		*pDisplay		= DISPLAY_MANAGER::Instance();
	bool				 bCollide = true;
	bool				 bIntersectNear = false;

	QDT_ASSERT(pDisplay);

	bCollide = _BBox.Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
	DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
	
	_Flags.Set(MGF_DO_CLIPPING, bIntersectNear);
	
	if	(bCollide)
	{
		if	(pDisplay->IsTestNearFogPlaneNeeded())
		{
			// Test with near fog distance
			if	(_BBox.IsOutsideOf(pDisplay->GetNearFogPlane()))
			{
				SetEntityFog(false);
			}
			else
			{
				SetEntityFog(true);
			}
		}
		else
		{
//			SetEntityFog(RENDER_MANAGER::Instance()->GetRenderContext()->IsFogEnabled());
		}
	}

	return (bCollide);
}


//-----------------------------------------------------------------------------
//	Name:		TestShadowVisibility
//	Object:		
//	03-10-29:	SBE - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::TestShadowVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	DISPLAY_MANAGER		*pDisplay		= DISPLAY_MANAGER::Instance();
	bool				 bCollide = true;
	bool				 bIntersectNear = false;

	QDT_ASSERT(pDisplay);

	bCollide = _BBox.Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
	pDisplay->GetStats().AddHexahedronIntersections(1);
#endif	
	
	_Flags.Set(MGF_DO_SHADOW_CLIPPING, bIntersectNear);

	return (bCollide);
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterial
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetMaterial(const COM_INSTANCE &	ciMaterial)
{
	/*_ciMaterial = ciMaterial;
	
	if (ciMaterial.HasHandle())
	{
		MATERIAL_ABC *pMaterial = static_cast<MATERIAL_ABC*>(ciMaterial.GetCommunicator());
		
		if (pMaterial->NeedLightsOnPrelit())
		{
			SetNeedLightsOnPrelit(true);
		}
		
		if (pMaterial->NeedLightsOnDynamicLit())
		{
			SetNeedLightsOnDynamicLit(true);
		}
	}*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		GetDistanceFromCamera
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
float	MESH_GROUP_GEOMETRY::GetDistanceFromCamera()
{
	NODE * pCameraNode = DISPLAY_MANAGER::Instance()->GetCameraNode();							
	VECTOR CameraPos;

	const VECTOR & GeometryPos = _BBox.GetCenter();
	pCameraNode->GetGlobalPosition(CameraPos);
	
	VECTOR Camera = GeometryPos - CameraPos;
	return (Camera.SquareLength());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


