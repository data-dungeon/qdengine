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
// CLASS: GROUP
//
//
//	00-10-27	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

//*****************************************************************************
//
// CLASS DESCRIPTION ( GROUP )
//
//	The GROUP class implements ...
//
//*****************************************************************************

#include	"..\include.h"

#include	INC_KRNCORE(Stream)
#include	INC_OLDRENDERER(Display/RenderPipeline)
#include	INC_KRNCORE(Profiler)
#include	INC_KRNCORE(Time)

#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(SceneGraph\Entities\Portal)
#include	INC_3DENGINE(SceneGraph\Entities\Light)
#include	INC_3DENGINE(Display)
#include	INC_KRNCORE(Profiler)

#ifdef _DEBUG
	#include "Group.inl"
#else
	#include	"..\Inline.h"
	
	#include	INL_OLDRENDERER(Display/GeometryABC)
	#include	INL_OLDRENDERER(Display/Renderpipeline)
	#include	INL_OLDRENDERER(Display/RenderManager)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Plane)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes3D/Frustrum)
	#include	INL_KRNCORE(Math/Color)

	#include	INL_3DENGINE(SceneGraph\Node)
	#include	INL_3DENGINE(SceneGraph\Entities\Mesh)
	#include	INL_3DENGINE(Display)
#endif

//////////////////// INCLUDE PS2 A GICLER !!!!!!!!!!!!!!!!!
//#include <libpc.h>
//////////////////// INCLUDE PS2 A GICLER !!!!!!!!!!!!!!!!!

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: GROUP constructor
// Object: Default constructor
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
GROUP::GROUP()
:
ENTITY_ABC			(GetGenericCommunicatorDescriptor(), ENTITY_GROUP),
_nFlags				(0),
_pBoundingBox		(NULL),
_pBoundingSphere	(NULL),
_MeshInst			(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
// Name: GROUP destructor
// Object: Destructor
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
GROUP::~GROUP()
{
	delete (_pBoundingBox);
	delete (_pBoundingSphere);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-08-09:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
#ifdef GROUP_NOT_OBSO
	DisplayGeometry(DisplayParameters);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	03-01-23:	ELE - Created
//-----------------------------------------------------------------------------
bool	GROUP::RayCast(RAY_CAST_PARAMETERS &	RayCastParameters,
					   bool &					bContinue)
{
#ifdef GROUP_NOT_OBSO
	if	(_nGroupType == GTYPE_DISPLAY)
	{
		bContinue = false;
		return	(false);
	}

	float	rDistance;
	RAY		LocalRay;	

	LocalRay = RayCastParameters.GetRay();

	if (!_pBoundingSphere->Intersect(LocalRay, rDistance))
	{
		bContinue = false;
		return	(false);
	}

	if (!_pBoundingSphere->Intersect(LocalRay.GetOrigin()) && 
		(rDistance > RayCastParameters.GetImpactDistance())) 
	{
		bContinue = false;
		return (false);
	}

	bContinue = true;
#endif
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	01-08-09:	ELE - Created
//-----------------------------------------------------------------------------
bool	GROUP::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters,
							  bool &				bInside)
{
#ifdef GROUP_NOT_OBSO
	if	(_nGroupType == GTYPE_COLLISION)
	{
		bInside = false;
		return	(false);
	}

	DISPLAY_MANAGER		*pDisplay		= DISPLAY_MANAGER::Instance();
#ifndef _MASTER
	STATISTICS			&Stats			= pDisplay->GetStats();
#endif
	SORTED_PORTAL_ZONE  *pCurrentZone	= pDisplay->GetCurrentPortalZone();
	bool				 bCollide		= true;
	bool				 bIntersectNear = true;
	bool				 bTmpInside		= true;

	BOUNDING_VOLUME		*pBoundingVol = _pBoundingBox;

	QDT_ASSERT(pDisplay);
	QDT_ASSERT(pCurrentZone);

	if (!pCurrentZone->IsVisibilityEnable())
	{
		bInside = false;
		return (true);
	}
	
	switch	(pCurrentZone->GetType())
	{
		default:
		case SORTED_PORTAL_ZONE::T_CAMERA:
		{
			if	(_MeshInst.HasHandle() == false)
			{
				bCollide = pBoundingVol->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bInside, bIntersectNear);
			}
			else
			{
				bInside = true;
				bCollide = pBoundingVol->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
			}

#ifndef _MASTER
			Stats.AddHexahedronIntersections(1);
#endif
		}
		break;

		case SORTED_PORTAL_ZONE::T_MIRROR:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			bInside = true;

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;

			bCollide = false;

			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

			if	(bInside)
			{
				bInside = false;

				while	(ItL != ItLEnd)
				{
					bCollide = bCollide || pBoundingVol->IntersectFront(*(*ItL), bTmpInside, bIntersectNear);

#ifndef _MASTER					
					Stats.AddHexahedronIntersections(1);
#endif
					bInside = bInside || bTmpInside;

					++ItL;
				}
			}
			else
			{
				while	(ItL != ItLEnd)
				{
					bCollide = bCollide || pBoundingVol->IntersectFront(*(*ItL), bIntersectNear);

#ifndef _MASTER					
					Stats.AddHexahedronIntersections(1);
#endif
					++ItL;
				}
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				QDT_ASSERT(pMF);
				if	(bInside)
				{
					bCollide = pBoundingVol->IntersectFront(pMF->GetHexahedron(), bInside, bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
				}
				else
				{
					bCollide = pBoundingVol->IntersectFront(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
				}
			}
		}
		break;
		
		case SORTED_PORTAL_ZONE::T_TWIN:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			bInside = true;

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;
			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	ItEnd = pHexahedrons->GetTail();

			while	((It != ItEnd) && bCollide)
			{
				bCollide = false;

				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

				if	(bInside)
				{
					bInside = false;

					while	(ItL != ItLEnd)
					{
						bCollide = bCollide || pBoundingVol->Intersect(*(*ItL), bTmpInside, bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
						bInside = bInside || bTmpInside;

						++ItL;
					}
				}
				else
				{
					while	(ItL != ItLEnd)
					{
						bCollide = bCollide || pBoundingVol->Intersect(*(*ItL), bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
						++ItL;
					}
				}

				++It;
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				if	(pMF)
				{
					if	(bInside)
					{
						bCollide = pBoundingVol->Intersect(pMF->GetHexahedron(), bInside, bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
					}
					else
					{
						bCollide = pBoundingVol->Intersect(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
					}
				}
				else
				{
					bCollide = pBoundingVol->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
				}
			}
		}
		break;
	}

	if (bCollide)
	{
		if	(bIntersectNear)
		{
			SetFlags(DO_CLIPPING, DO_CLIPPING);
		}
		else
		{
			SetFlags(DO_CLIPPING, 0);
		}
	}

	return (bCollide);
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		TestShadowVisibility
//	Object:		
//	03-10-09:	SBE - Created
//-----------------------------------------------------------------------------
bool	GROUP::TestShadowVisibility(DISPLAY_PARAMETERS &	DisplayParameters,
								    bool &					bInside)
{
#ifdef GROUP_NOT_OBSO
	if	(_nGroupType == GTYPE_COLLISION)
	{
		bInside = false;
		return	(false);
	}

	DISPLAY_MANAGER		*pDisplay		= DISPLAY_MANAGER::Instance();
#ifndef _MASTER
	STATISTICS			&Stats			= pDisplay->GetStats();
#endif
	SORTED_PORTAL_ZONE  *pCurrentZone	= pDisplay->GetCurrentPortalZone();
	bool				 bCollide		= true;
	bool				 bIntersectNear	= true;
	bool				 bTmpInside		= true;
	BOUNDING_VOLUME		*pBoundingVol = _pBoundingBox;

	QDT_ASSERT(pDisplay);
	QDT_ASSERT(pCurrentZone);

/*	if (!pCurrentZone->IsVisibilityEnable())
	{
		bInside = false;
		return (true);
	}*/
	
	switch	(pCurrentZone->GetType())
	{
		default:
		case SORTED_PORTAL_ZONE::T_CAMERA:
		{
			if	(_MeshInst.HasHandle() == false)
			{
				bCollide = pBoundingVol->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bInside, bIntersectNear);
			}
			else
			{
				bInside = true;
				bCollide = pBoundingVol->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
			}

#ifndef _MASTER
			Stats.AddHexahedronIntersections(1);
#endif
		}
		break;

		case SORTED_PORTAL_ZONE::T_MIRROR:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			bInside = true;

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;

			bCollide = false;

			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

			if	(bInside)
			{
				bInside = false;

				while	(ItL != ItLEnd)
				{
					bCollide = bCollide || pBoundingVol->IntersectFront(*(*ItL), bTmpInside, bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
					bInside = bInside || bTmpInside;

					++ItL;
				}
			}
			else
			{
				while	(ItL != ItLEnd)
				{
					bCollide = bCollide || pBoundingVol->IntersectFront(*(*ItL), bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
					++ItL;
				}
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				QDT_ASSERT(pMF);
				if	(bInside)
				{
					bCollide = pBoundingVol->IntersectFront(pMF->GetHexahedron(), bInside, bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
				}
				else
				{
					bCollide = pBoundingVol->IntersectFront(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
				}
			}
		}
		break;
		
		case SORTED_PORTAL_ZONE::T_TWIN:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			bInside = true;

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;
			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	ItEnd = pHexahedrons->GetTail();

			while	((It != ItEnd) && bCollide)
			{
				bCollide = false;

				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

				if	(bInside)
				{
					bInside = false;

					while	(ItL != ItLEnd)
					{
						bCollide = bCollide || pBoundingVol->Intersect(*(*ItL), bTmpInside, bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
						bInside = bInside || bTmpInside;

						++ItL;
					}
				}
				else
				{
					while	(ItL != ItLEnd)
					{
						bCollide = bCollide || pBoundingVol->Intersect(*(*ItL), bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
						++ItL;
					}
				}

				++It;
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				if	(pMF)
				{
					if	(bInside)
					{
						bCollide = pBoundingVol->Intersect(pMF->GetHexahedron(), bInside, bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
					}
					else
					{
						bCollide = pBoundingVol->Intersect(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
						Stats.AddHexahedronIntersections(1);
#endif
					}
				}
				else
				{
					bCollide = pBoundingVol->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					Stats.AddHexahedronIntersections(1);
#endif
				}
			}
		}
		break;
	}

	return (bCollide);
#else
	return false;
#endif
}


//unsigned int	total = 0, totalCall = 0;

//-----------------------------------------------------------------------------
//	Name:		DisplayGeometry
//	Object:		
//	03-01-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::DisplayGeometry(DISPLAY_PARAMETERS &	DisplayParameters)
{
#ifdef GROUP_NOT_OBSO
#if	0
	if	(_MeshInst.HasHandle() == false) return;

	int					i, nNbrShadowMapEmitters, nNbrShadowMapReceivers, nNbrLights;
	MATERIAL_ABC		*pMaterial;
	SHADER_ABC			*pShader;
	GEOMETRY_ABC		*pGeometry;
	MESH				*pMesh = static_cast<MESH *>(_MeshInst.GetCommunicator());
	NODE				*pNode = DisplayParameters.GetNode();
	RENDER_CONTEXT		*pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_PIPELINE_ABC	*pRP = pRC->GetRenderPipeline();

	const COM_INSTANCE & MaterialInst = pMesh->GetMaterialInst(0);

	if	(MaterialInst.HasHandle())
	{
		pMaterial = static_cast<MATERIAL_ABC *>(MaterialInst.GetCommunicator());
		pShader = pMaterial->GetShader();
	}
	else
	{
		pMaterial = NULL;
		pShader = NULL;
	}

	nNbrShadowMapEmitters = pNode->GetNbrShadowMapEmitters();
	nNbrShadowMapReceivers = pNode->GetNbrShadowMapReceivers();

	pMesh->FindLights(DisplayParameters);

	if	(nNbrShadowMapEmitters || nNbrShadowMapReceivers || (pMaterial && pMaterial->GetSort()))
	{
		bool			bFog;
		COLOR			Color(false);
		GEOMETRY_REF	*pGeometryRef;

		pGeometry = pMesh->GetGeometries()[0];
		pGeometryRef = pRC->GetGeometryRef();

		pGeometry->SetMaterialInst(MaterialInst, pShader);
		pGeometry->ToSkin(false);

		pGeometryRef->SetShader(pShader);

		#ifdef _DEBUG
			if	(pMaterial)
			{
				pGeometryRef->SetMaterialName(pMaterial->GetName());
			}
		#endif
			
		pGeometryRef->SetGeometry(pGeometry);
		pGeometryRef->SetGlobalMatrix(&DisplayParameters.GetNode()->GetGlobalMatrix());
		pGeometryRef->SetGlobalMatrixIsIdentity(true);
		pGeometryRef->SetDoubleSided(pMesh->GetFlags(MESH::DOUBLE_SIDED) > 0);
		pGeometryRef->SetCastShadows(false);
		pGeometryRef->SetSelfShadowed(false);
		pGeometryRef->SetDoClipping(GetFlags(DO_CLIPPING) > 0);
		pGeometryRef->SetStaticGeometry(true);
		pGeometryRef->SetBoundingBox(_pBoundingBox);
		pGeometryRef->SetRefSkinningMatrix(NULL);
		pGeometryRef->SetPrelit(!pMesh->GetFlags(MESH::DYNAMIC_LIT));

		pGeometryRef->SetAmbientColor(pRC->GetGlobalAmbientLight());
		
		bFog = pRC->IsFogEnabled();
		pGeometryRef->EnableFog(bFog);

		if	(bFog)
		{
			pGeometryRef->SetFogNear(pRC->GetFogStart());
			pGeometryRef->SetFogFar(pRC->GetFogEnd());
			pGeometryRef->SetFogColor(pRC->GetFogColor());
		}

		#ifdef _DEBUG
			pGeometryRef->SetMeshName(pMesh->GetName());
		#endif		
		
	/*	if (pMaterial && pMaterial->GetFlags().IsSet(TS_MICROSCOPIC_SHADER))
		{
			// Microscopic material
			pMaterial->GetLightSource()->SetPosition(DisplayParameters.GetFrustrum().GetOrigin());
			pGeometryRef->AddLightSource(pMaterial->GetLightSource());
			pGeometryRef->SetPrelit(false);
			pGeometryRef->SetAmbientColor(COLOR(255, 0, 0, 0));
		}
		else*/
		if (_vLightNodes.GetSize() > 0)
		{
			nNbrLights = _vLightNodes.GetSize();
			for (i = 0 ; i < nNbrLights ; ++i)
			{
				NODE *		pLightNode	= static_cast<NODE *>(_vLightNodes[i].GetCommunicator());
				LIGHT_ABC *	pLight		= static_cast<LIGHT_ABC *>(pLightNode->GetEntityInst().GetCommunicator());
				pGeometryRef->AddLightSource(pLight->GetLightSource());
			}
		}

		if	((nNbrShadowMapEmitters > 0) || (nNbrShadowMapReceivers > 0))
		{
			for	(i = 0 ; i < nNbrShadowMapEmitters ; ++i)
			{
				pNode->GetShadowMapEmitter(i)->AddEmitter(pGeometryRef);
			}

			for	(i = 0 ; i < nNbrShadowMapReceivers ; ++i)
			{
				pNode->GetShadowMapEmitter(i)->AddReceiver(pGeometryRef);
			}
		}
		else
		{
			pRC->Push(pGeometryRef);
		}

		_vLightNodes.Clear();
	}
	else
	{
		//----------------------------------------//
//		unsigned int	before, after;
		//----------------------------------------//

		//----------------------------------------//
//		before = TIME_PS2::GetCurrentCPUClockTimer();
		//----------------------------------------//

		// Cull mode
		RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
		RENDER_CONTEXT::CULL_MODE	CullMode = RmCullMode;
		if (pMesh->GetFlags(MESH::DOUBLE_SIDED) > 0)
		{
			CullMode = RENDER_CONTEXT::CULLMODE_NONE;
		}

		if (pShader)
		{
			if (pShader->IsDoubleSided())
			{
				CullMode = RENDER_CONTEXT::CULLMODE_NONE;
			}
			
			pRC->SetShader(pShader); // CA C'EST LA GROSSE RAMETTE SUR PS2 !!!!!!!!!!!!!!!!!!!!! 50000 cycles pour 62 appels
		}

		pRC->SetCullMode(CullMode);

		// Lights
		pRC->EnableLighting(true);
		nNbrLights = _vLightNodes.GetSize();
		if (nNbrLights)
		{
			for (i = 0 ; i < nNbrLights ; ++i)
			{
				NODE *		pLightNode = static_cast<NODE *>(_vLightNodes[i].GetCommunicator());
				LIGHT_ABC *	pLight = static_cast<LIGHT_ABC *>(pLightNode->GetEntityInst().GetCommunicator());
				pRC->AddLightSource(pLight->GetLightSource());
			}
		}
		
		// Clipping
		pRC->EnableClipping(GetFlags(DO_CLIPPING) > 0);

		// Bounding volume
		pRC->SetBoundingVolume(_pBoundingBox);

		// Transform
		pRC->EnableVertexBlending(3, false);
		pRC->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());

		//-----------PERF COUNTER BEGIN------------------------------------------------//
//		int control, nbcachemiss;
//		control = SCE_PC0_ICACHE_MISS | (SCE_PC_U0|SCE_PC_S0|SCE_PC_K0| SCE_PC_EXL0);
//		control = SCE_PC1_DCACHE_MISS | (SCE_PC_U1|SCE_PC_S1|SCE_PC_K1| SCE_PC_EXL1);
//		control |= SCE_PC0_NO_EVENT | (SCE_PC_U0|SCE_PC_S0|SCE_PC_K0| SCE_PC_EXL0);
//		control |= SCE_PC1_NO_EVENT | (SCE_PC_U1|SCE_PC_S1|SCE_PC_K1| SCE_PC_EXL1);
//		control |= SCE_PC_CTE;
//		scePcStart( control, 0, 0); // Start measurement
		//-----------PERF COUNTER BEGIN------------------------------------------------//

		//-----------PERF COUNTER END------------------------------------------------//
//		total = scePcGetCounter1(); // Get measured value
//		scePcStop();

//		printf("total = %d\n", total);
		//-----------PERF COUNTER END------------------------------------------------//

		//----------------------------------------//
//		after = TIME_PS2::GetCurrentCPUClockTimer();
//		total += after - before;
//		totalCall++;
		//----------------------------------------//

		// Render
		pGeometry = *(pMesh->GetGeometries());

		//----------------------------------------//
//		after = TIME_PS2::GetCurrentCPUClockTimer();
//		total += after - before;
		//----------------------------------------//

		pRP->Display(pGeometry, !pMesh->GetFlags(MESH::DYNAMIC_LIT));
		
		//----------------------------------------//
//		before = TIME_PS2::GetCurrentCPUClockTimer();
		//----------------------------------------//

		// Reset lights
		if	(nNbrLights)
		{
			pRC->ResetLights();
			_vLightNodes.Clear();
		}

		// Reset cullmode
		pRC->SetCullMode(RmCullMode);

		//----------------------------------------//
//		after = TIME_PS2::GetCurrentCPUClockTimer();
//		total += after - before;
//		totalCall++;
		//----------------------------------------//
	}
#endif
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetMesh
//	Object:		
//	03-03-21:	ELE - Created
//-----------------------------------------------------------------------------
MESH *	GROUP::GetMesh()
{
#ifdef GROUP_NOT_OBSO
	if	(_MeshInst.HasHandle())
	{
		MESH	*pMesh = static_cast<MESH *>(_MeshInst.GetCommunicator());

		return	(pMesh);
	}
#endif
	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		ClearShadowMapFlags
//	Object:		
//	03-03-21:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::ClearShadowMapFlags()
{
#ifdef GROUP_NOT_OBSO
	if	(_MeshInst.HasHandle())
	{
		MESH	*pMesh = static_cast<MESH *>(_MeshInst.GetCommunicator());

		pMesh->ClearShadowMapFlags();
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		FindLights
//	Object:		
//	03-03-21:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::FindLights(DISPLAY_PARAMETERS &	DisplayParameters)
{
#ifdef GROUP_NOT_OBSO
	if	(_MeshInst.HasHandle())
	{
		MESH	*pMesh = static_cast<MESH *>(_MeshInst.GetCommunicator());
		
		pMesh->FindLights(DisplayParameters);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("GROUP", 1, CM_ReadChunk);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("GROUP", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	GROUP::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_GROUP) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	GROUP::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("GROUP"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_GROUP,
											QDT_COMMENT("CD24DD7D-BA0D-6B88-3194-3FFE231BBEB0"),
											sizeof(GROUP),
											ComInit,
											QDT_COMMENT("Class for Groups"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));

}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::EM_Construct(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	GROUP	*pGroup;
	pGroup = static_cast<GROUP *>(pO);

	QDT_NEW_PLACED(pGroup, GROUP)();
	QDT_ASSERT(pGroup);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::EM_Destruct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	GROUP	*pGroup;
	pGroup = static_cast<GROUP *>(pO);

	QDT_DELETE_PLACED(pGroup, GROUP);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	GROUP::CM_ReadChunk(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	GROUP	*pGroup;
	pGroup = static_cast<GROUP *>(pCommunicator);

	int	nBVType;

	Stream	>> pGroup->_nFlags
			>> nBVType;

	pGroup->_pBoundingBox = BV_FACTORY::Instance()->Create((BOUNDING_VOLUME::TYPE)nBVType);
	Stream	>> *pGroup->_pBoundingBox;

	pGroup->_pBoundingSphere = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_SPHERE);
	Stream	>> *pGroup->_pBoundingSphere;

	Stream	>> pGroup->_nGroupType;

	if	(pGroup->_nGroupType == GTYPE_DISPLAY)
	{
		Stream >> pGroup->_MeshInst;
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	GROUP::CM_WriteChunk(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	GROUP	*pGroup;
	pGroup = static_cast<GROUP *>(pCommunicator);

	Stream.DebugOutput("Group :\t");
	Stream.DebugOutput(pGroup->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Flags :\t");
	Stream	<< pGroup->_nFlags;
	Stream.DebugOutput("\t");
	Stream.DebugOutputBinary(pGroup->_nFlags);
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Bounding box :\t");
	Stream << pGroup->_pBoundingBox->GetType();
	Stream << *pGroup->_pBoundingBox;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Bounding Sphere :\t");
	Stream << *pGroup->_pBoundingSphere;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Type :\t");
	Stream	<< pGroup->_nGroupType;
	Stream.DebugOutput("\t");
	if	(pGroup->_nGroupType == GTYPE_DISPLAY)
	{
		Stream.DebugOutput("DISPLAY");
	}
	else
	{
		Stream.DebugOutput("COLLISION");
	}
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Mesh :\t");
	Stream << pGroup->_MeshInst;
	if	(pGroup->_MeshInst.HasHandle())
	{
		MESH	*pMesh = static_cast<MESH *>(pGroup->_MeshInst.GetCommunicator());

		Stream.DebugOutput("\t");
		Stream.DebugOutput(pMesh->GetName());
	}
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================