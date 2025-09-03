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
//	CLASS:	SOFT_BODY
//
//	02-01-16:	ELE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(BaseType)
#include	INC_KRNCOM(ComplexType)

#include	INC_KRNCORE(Time)
#include	INC_KRNCORE(Stream)
#include	INC_KRNCORE(Math/Geometry/BoundingVolume/OBB)
#include	INC_KRNCORE(Display/ParticleExchanger)

#include	INC_KRNSERV(Physic/Lattice)
#include	INC_KRNSERV(Physic/Particle)
#include	INC_KRNSERV(Physic/PhysicalParticleSolver)

#include	INC_3DENGINE(Behavior/Datas)
#include	INC_3DENGINE(Behavior/HipsAnim)
#include	INC_3DENGINE(Behavior/Controllers/Container)
#include	INC_3DENGINE(SceneGraph/Node)
#include	INC_3DENGINE(SceneGraph/Material)
#include	INC_3DENGINE(SceneGraph/Entities/Light)
#include	INC_3DENGINE(Physic)
#include	INC_3DENGINE(Display)

#ifndef	_DEBUG
	#include	INL_KRNCORE(Display/Renderpipeline)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Plane)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes3D/Frustrum)
	#include	INL_KRNCORE(Display/GeometryABC)
	#include	INL_KRNCORE(Display/ParticleExchanger)
	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Math/Color)

	#include	INL_KRNSERV(Physic/Lattice)
	#include	INL_KRNSERV(Physic/Particle)
	#include	INL_KRNSERV(Physic/PhysicalEntity)
	#include	INL_KRNSERV(Physic/PhysicalParticleSolver)
				
	#include	INL_3DENGINE(SceneGraph/Entities/Mesh)
	#include	INL_3DENGINE(SceneGraph/Entities/Skeleton)
	#include	INL_3DENGINE(SceneGraph/Node)
	#include	INL_3DENGINE(Display)
	#include	INL_3DENGINE(Behavior/AnimNode)
	#include	INL_3DENGINE(Behavior/AnimTimer)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

int	SOFT_BODY::_nBVsIds = 0;

//-----------------------------------------------------------------------------
//	Name:		SOFT_BODY constructor
//	Object:		
//	02-01-16:	ELE - Created
//-----------------------------------------------------------------------------
SOFT_BODY::SOFT_BODY()
:
MESH			(GetGenericCommunicatorDescriptor(), ENTITY_SOFT_BODY),
_pLattice		(NULL),
_pSolver		(NULL),
_pExchanger		(NULL),
_AnimatedNodeInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_AnimInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	_pExchanger = new PARTICLE_EXCHANGER;
}

//-----------------------------------------------------------------------------
//	Name:		SOFT_BODY destructor
//	Object:		
//	02-01-16:	ELE - Created
//-----------------------------------------------------------------------------
SOFT_BODY::~SOFT_BODY()
{
	delete	_pLattice;
	delete	_pExchanger;
	
//	PHYSIC_MANAGER::Instance()->RemoveSolver(_pSolver);
	PHYSIC_MANAGER::Instance()->RemoveCloth(COM_INSTANCE(this));

	delete	_pSolver;
}

//-----------------------------------------------------------------------------
//	Name:		CreateLattice
//	Object:		
//	02-02-25:	ELE - Created
//-----------------------------------------------------------------------------
LATTICE * SOFT_BODY::CreateLattice()
{
	if	(_pLattice)
	{
		delete	(_pLattice);
	}

	_pLattice = new LATTICE;

	return	(_pLattice);
}

//-----------------------------------------------------------------------------
//	Name:		GetLattice
//	Object:		
//	02-02-27:	ELE - Created
//-----------------------------------------------------------------------------
LATTICE * SOFT_BODY::GetLattice()
{
	return	(_pLattice);
}

//-----------------------------------------------------------------------------
//	Name:		AttachSolver
//	Object:		
//	02-02-27:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::AttachSolver(PHYSICAL_PARTICLE_SOLVER *pSolver)
{
	_pSolver = pSolver;

	if	(_pSolver)
	{
		_pSolver->SetExchanger(_pExchanger);
		_pSolver->SetSoftBodyBox(&_BoundingBox);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSolver
//	Object:		
//	02-02-27:	ELE - Created
//-----------------------------------------------------------------------------
PHYSICAL_PARTICLE_SOLVER* SOFT_BODY::GetSolver()
{
	return _pSolver;	
}

//-----------------------------------------------------------------------------
//	Name:		GetParticleExchanger
//	Object:		
//	02-03-14:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_EXCHANGER* SOFT_BODY::GetParticleExchanger()
{
	return _pExchanger;
}

//-----------------------------------------------------------------------------
//	Name:		ResetPhysic
//	Object:		
//	02-03-22:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::ResetPhysic()
{
	if (_pSolver)
	{
		_pSolver->InitPhysic();
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitPhysic
//	Object:		
//	02-03-22:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::InitPhysic()
{
	GeometriesToParticles();

	if (_pSolver)
	{
		int	i;

		for	(i = 0 ; i < _nNbrGeometries; ++i)
		{
			MATERIAL_ABC	*pMaterial = static_cast<MATERIAL_ABC *>(_pGeometriesMaterialInst[i].GetCommunicator());

			if	(pMaterial->NeedLightsOnPrelit() || pMaterial->NeedLightsOnDynamicLit())
			{
				_pSolver->NeedComputeNormals(true);
				break;
			}
		}

		_pSolver->InitPhysic();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GeometriesToParticles
//	Object:		
//	02-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::GeometriesToParticles()
{
	if (_pExchanger == NULL) return;

	int	nGeometryIndex, nNbrPoints;

	for	(nGeometryIndex = 0 ; nGeometryIndex < _nNbrGeometries ; ++nGeometryIndex)
	{
		nNbrPoints = _pExchanger->GetNbrGeomPoints(nGeometryIndex);

		_pGeometriesArray[nGeometryIndex]->GeometryToParticles(nNbrPoints,
															   _pExchanger->GetVerticesRef(nGeometryIndex),
															   _pExchanger->GetVerticesLink(nGeometryIndex));
	}
}

//-----------------------------------------------------------------------------
//	Name:		ParticlesToGeometries
//	Object:		
//	02-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::ParticlesToGeometries()
{
	if (_pExchanger == NULL) return;

	int	nGeometryIndex, nNbrPoints;

	for	(nGeometryIndex = 0 ; nGeometryIndex < _nNbrGeometries ; ++nGeometryIndex)
	{
		nNbrPoints = _pExchanger->GetNbrGeomPoints(nGeometryIndex);

		_pGeometriesArray[nGeometryIndex]->ParticlesToGeometry(nNbrPoints,
															   _pExchanger->GetVerticesRef(nGeometryIndex),
															   _pExchanger->GetNormalsRef(nGeometryIndex),
															   _pExchanger->GetVerticesLink(nGeometryIndex),
															   _pExchanger->GetNormalsLink(nGeometryIndex),
															   _pExchanger->GetCoeffLink(nGeometryIndex),
															   false);

		if (_pGeometriesArray[nGeometryIndex]->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_STRIPS)
		{
			_pGeometriesArray[nGeometryIndex]->UpdateStrippingDatas();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		PlayAnim
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::PlayAnim(COM_INSTANCE &	AnimDataInst,
							float			rSpeed,
							bool			bCollide)
{
	if	(_AnimatedNodeInst.HasHandle())
	{
		NODE	*pNode = static_cast<NODE *>(_AnimatedNodeInst.GetCommunicator());

		if	(_pSolver)
		{
//			pNode->SetGlobalPosition(_pSolver->GetAnimatedPoint());
			_pSolver->SetAnimatedMatrix(&pNode->GetGlobalMatrix(), bCollide);
		}

		_AnimInst = BEHAVIOR_MANAGER::Instance()->CreateHipsAnim();
		QDT_ASSERT( _AnimInst.HasHandle() );

		HIPS_ANIM	*pAnim = static_cast<HIPS_ANIM*>(_AnimInst.GetCommunicator());
		QDT_ASSERT(pAnim);

		QDT_ASSERT( AnimDataInst.HasHandle() );
		ANIM_PC	*pAnimData = static_cast<ANIM_PC*>(AnimDataInst.GetCommunicator());
		QDT_ASSERT(pAnimData);

		static_cast<HIPS_ANIM_STRUCT*>(pAnim->GetAnimStruct())->InstanciateAnimPC(AnimDataInst);
		pAnim->GetAnimStruct()->SetAbsolute(true);
		pAnim->GetAnimStruct()->SetTeleport(false);
		pAnim->GetAnimStruct()->SetPreemptive(true);
		pAnim->GetAnimStruct()->SetBlending(false);
		pAnim->GetAnimStruct()->SetLoop(false);
		pAnim->GetAnimStruct()->SetNbrLoops(1);
		pAnim->GetAnimStruct()->SetSpeed(rSpeed);
		pAnim->GetAnimStruct()->SetStartTime(0.0f);
		pAnim->GetAnimStruct()->SetEndTime( static_cast<float>(pAnimData->GetNbFrames()) );
		pAnim->GetAnimStruct()->EnableDeletation(false);	

		pNode->PlayAnim(_AnimInst);
	}
}

//-----------------------------------------------------------------------------
//	Name:		StopAnim
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::StopAnim() const
{
	if	(_pSolver)
	{
		_pSolver->SetAnimatedMatrix(NULL, false);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDamping
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::SetDamping(int	nDamping)
{
	if	(_pSolver)
	{
		_pSolver->SetDamping(nDamping);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDamping
//	Object:		
//	03-11-27:	ELE - Created
//-----------------------------------------------------------------------------
int	SOFT_BODY::GetDamping() const
{
	if	(_pSolver)
	{
		return	(_pSolver->GetDamping());
	}

	return	(0);
}

//-----------------------------------------------------------------------------
//	Name:		SetStiffness
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::SetStiffness(float	rStiffness)
{
	if	(_pLattice)
	{
		_pLattice->ApplyStiffness(0, rStiffness);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetStiffness
//	Object:		
//	03-11-27:	ELE - Created
//-----------------------------------------------------------------------------
float	SOFT_BODY::GetStiffness() const
{
	if	(_pLattice)
	{
		return	(_pLattice->GetStiffness(0));
	}

	return	(0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		SetStretching
//	Object:		
//	03-11-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::SetStretching(float	rStretching)
{
	if	(_pLattice)
	{
		if	(rStretching < 100.0f)
		{
			rStretching = 100.0f;
		}

		_pLattice->ApplyStretching(0, rStretching);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetStretching
//	Object:		
//	03-11-27:	ELE - Created
//-----------------------------------------------------------------------------
float	SOFT_BODY::GetStretching() const
{
	if	(_pLattice)
	{
		return	(_pLattice->GetStretching(0));
	}

	return	(100.0f);
}

//-----------------------------------------------------------------------------
//	Name:		SetContraction
//	Object:		
//	03-11-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::SetContraction(float	rContraction)
{
	if	(_pLattice)
	{
		if	(rContraction > 100.0f)
		{
			rContraction = 100.0f;
		}

		if	(rContraction < 0.0f)
		{
			rContraction = 0.0f;
		}

		_pLattice->ApplyContraction(0, rContraction);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetContraction
//	Object:		
//	03-11-27:	ELE - Created
//-----------------------------------------------------------------------------
float	SOFT_BODY::GetContraction() const
{
	if	(_pLattice)
	{
		return	(_pLattice->GetContraction(0));
	}

	return	(100.0f);
}

//-----------------------------------------------------------------------------
//	Name:		SetAxe
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::SetAxe(const VECTOR &	Axe)
{
	if	(_pSolver)
	{
		_pSolver->SetAxe(Axe);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::Update()
{
	if	(_AnimatedNodeInst.HasHandle())
	{
		float	rSpeedFactor = TIME::Instance()->GetDefaultClock()->GetSpeedFactor();
		NODE	*pNode = static_cast<NODE *>(_AnimatedNodeInst.GetCommunicator());

		if	(pNode->GetContainer())
		{
			pNode->GetContainer()->Update(rSpeedFactor);
		}

/*		if	(_AnimInst.HasHandle())
		{
			HIPS_ANIM	*pAnim = static_cast<HIPS_ANIM*>(_AnimInst.GetCommunicator());
			if	(pAnim->GetAnimStruct()->IsFinished())
			{
				StopAnim();
				_AnimInst.Release();
				_AnimInst = COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
			}
		}*/
	}
}

//-----------------------------------------------------------------------------
//	Name:		ToUpdate
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
bool	SOFT_BODY::ToUpdate() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	ParticlesToGeometries();
	MESH::Display(DisplayParameters);
	
#ifndef _MASTER
	if (DISPLAY_MANAGER::Instance()->DisplayPhysic())
	{
		DisplayPhysicElements(DisplayParameters);
	}

	if (_pSolver)
	{
		_pSolver->DisplayBoundingVolumes();
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplayGeometries
//	Object:		
//	02-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::DisplayGeometries(DISPLAY_PARAMETERS &				DisplayParameters,
									 QDT_VECTOR <COM_INSTANCE> &		vLightsOnDynamicLit,
									 QDT_VECTOR <COM_INSTANCE> &		vLightsOnPrelit, 
									 BOUNDING_VOLUME *					pGlobalBoundingBox,
									 QDT_VECTOR <MATRIX> *				pvRefSkinningMatrices,
									 QDT_VECTOR <DYNAMIC_CLUSTER*> *	pvDCs,
									 const QDT_VECTOR<COM_INSTANCE> *	pvMats,
									 bool								bOptimizeRendererOnPS2,
									 QDT_VECTOR<LIGHT_SOURCE> *			pvLightSourcesOnDynamicLit,
									 QDT_VECTOR<LIGHT_SOURCE> *			pvLightSourcesOnPrelit)
{
	QDT_FAIL();
}


//-----------------------------------------------------------------------------
//	Name:		AddMeshEmitterInShadowMapRefs
//	Object:		
//	03-11-14:	SBE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::AddMeshEmitterInShadowMapRefs(DISPLAY_PARAMETERS & DisplayParameters, 
												 QDT_VECTOR <COM_INSTANCE> &vLightsOnDynamicLit, 
												 QDT_VECTOR <COM_INSTANCE> &vLightsOnPrelit, 
												 BOUNDING_VOLUME * pGlobalBoundingBox, 
												 QDT_VECTOR <MATRIX> *pvRefSkinningMatrices, 
												 QDT_VECTOR <DYNAMIC_CLUSTER*> *pvDCs, 
												 const QDT_VECTOR<COM_INSTANCE> *pvMats)
{
/*	GEOMETRY_REF *		pGeometryRef;
	int					i, nCount = 0;
	NODE *				pNode;
	unsigned int		nNbrShadowMapEmitters = pNode->GetNbrShadowMapEmitters();
	unsigned int		nNbrShadowLightMapEmitters = pNode->GetNbrShadowLightMapEmitters();
	COM_INSTANCE *		pMaterialInst;
	bool				bNeedLightsOnPrelit,
						bNeedLightsOnDynamicLit,
						bFog;
	MATERIAL_ABC *		pMaterial;
	SHADER_ABC *		pShader;
	GEOMETRY_ABC *		pGeometry;
	RENDER_CONTEXT *	pRC = RENDER_MANAGER::Instance()->GetRenderContext();

	while (nCount < _nNbrGeometries)
	{
		pGeometry = _pGeometriesArray[nCount];

		if (pvMats)
		{
			pMaterialInst = const_cast<COM_INSTANCE *>(&(*pvMats)[nCount]);
		}
		else
		{
			pMaterialInst = &_pGeometriesMaterialInst[nCount];
		}

		if	(pMaterialInst->HasHandle())
		{
			pMaterial = static_cast<MATERIAL_ABC *>(pMaterialInst->GetCommunicator());
			pShader	  = pMaterial->GetShader();
			bNeedLightsOnPrelit		= pMaterial->NeedLightsOnPrelit();
			bNeedLightsOnDynamicLit = pMaterial->NeedLightsOnDynamicLit(); 

			#ifdef _DEBUG
				pShader->SetName(pMaterial->GetName());			
			#endif
		}
		else
		{
			pMaterial = NULL;
			pShader = NULL;

			bNeedLightsOnPrelit = false;
			bNeedLightsOnDynamicLit = false;
		}

		pGeometryRef = pRC->GetGeometryRef();

		pGeometry->SetMaterialInst(*pMaterialInst, pShader);
		pGeometryRef->SetShader(pShader);

		#ifdef _DEBUG
			if	(pMaterial)
			{
				pGeometryRef->SetMaterialName(pMaterial->GetName());
			}
		#endif

		pGeometryRef->SetGeometry(pGeometry);
		if	(_pSolver)
		{
			pGeometryRef->SetGlobalMatrix(NULL);
		}
		else
		{
			pGeometryRef->SetGlobalMatrix(&DisplayParameters.GetNode()->GetGlobalMatrix());
		}
		pGeometryRef->SetGlobalMatrixIsIdentity(GetFlags(GLOBAL_MATRIX_IDENTITY) > 0);
		pGeometryRef->SetDoubleSided(GetFlags(DOUBLE_SIDED) > 0);
		pGeometryRef->SetCastShadows(GetFlags(CAST_SHADOWS) > 0);
		pGeometryRef->SetSelfShadowed(GetFlags(SIMPLE_SHADOWS) == 0 && DISPLAY_MANAGER::Instance()->IsSelfShadowingEnabled());
		pGeometryRef->SetDoClipping(GetFlags(DO_CLIPPING) > 0);
		pGeometryRef->SetDoShadowCulling(GetFlags(DO_SHADOW_CULLING) > 0);
		pGeometryRef->SetDoShadowClipping(GetFlags(DO_SHADOW_CLIPPING) > 0);
		pGeometryRef->SetStaticGeometry(GetFlags(STATIC_GEOMETRY) > 0);
		pGeometryRef->SetRefSkinningMatrix(pvRefSkinningMatrices);
		pGeometryRef->SetPrelit(GetFlags(STATIC_LIT) > 0);
		pGeometryRef->SetNumGeometry(nCount);
		pGeometryRef->SetDoDisplay(false);

		pGeometryRef->SetAmbientColor(pRC->GetGlobalAmbientLight());
		
		bFog = pRC->IsFogEnabled();
		pGeometryRef->EnableFog(bFog);

		if	(bFog)
		{
			pGeometryRef->SetFogNear(pRC->GetFogStart());
			pGeometryRef->SetFogFar(pRC->GetFogEnd());
			pGeometryRef->SetFogColor(pRC->GetFogColor());
		}

		pGeometryRef->SetBoundingBox(pGlobalBoundingBox);

		#ifdef _DEBUG
			pGeometryRef->SetMeshName(GetName());
		#endif
		
		pGeometry->ToSkin(false);

		if (bNeedLightsOnPrelit)
		{		
			for (i = 0 ; i < vLightsOnPrelit.GetSize() ; ++i)
			{
				NODE *		pLightNode	= static_cast<NODE *>(vLightsOnPrelit[i].GetCommunicator());
				LIGHT_ABC *	pLight		= static_cast<LIGHT_ABC *>(pLightNode->GetEntityInst().GetCommunicator());

				// no lighting if there's a light texture associated
				if (!( pLight->HasAProjectionTexture() && pLight->IsAdditiveBlending()))
				{
					pGeometryRef->AddLightSource(pLight->GetLightSource());
				}
			}

			pGeometryRef->SetAmbientColor(_StaticAmbientColor);
		}
		else if (bNeedLightsOnDynamicLit)
		{
			for (i = 0 ; i < vLightsOnDynamicLit.GetSize() ; ++i)
			{
				NODE *		pLightNode	= static_cast<NODE *>(vLightsOnDynamicLit[i].GetCommunicator());
				LIGHT_ABC *	pLight = static_cast<LIGHT_ABC *>(pLightNode->GetEntityInst().GetCommunicator());

				// no lighting if there's a light texture associated
				if (!( pLight->HasAProjectionTexture() && pLight->IsAdditiveBlending()))
				{
					pGeometryRef->AddLightSource(pLight->GetLightSource());
				}
			}

			pGeometryRef->SetAmbientColor(_DynamicAmbientColor);
		}

		for	(i = 0 ; i < nNbrShadowMapEmitters ; ++i)
		{
			pNode->GetShadowMapEmitter(i)->AddEmitter(pGeometryRef);
		}

		for	(i = 0 ; i < nNbrShadowLightMapEmitters ; ++i)
		{
			pNode->GetShadowLightMapEmitter(i)->AddLightMapEmitter(pGeometryRef);		
		}

		nCount++;
	}	*/

	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		DisplayBoundingBox
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::DisplayBoundingBox(DISPLAY_PARAMETERS &	DisplayParameters) const
{
#ifndef _MASTER
	const static COLOR_FLOAT	Red(1.0f, 1.0f, 0.0f, 0.0f);		// Case dynamic
	const static COLOR_FLOAT	White(1.0f, 1.0f, 1.0f, 1.0f);		// Case static
	const static COLOR_FLOAT	Cyan(1.0f, 0.0f, 1.0f, 1.0f);		// Case shadow emitter
	const static COLOR_FLOAT	Magenta(1.0f, 1.0f, 0.0f, 1.0f);	// Case shadow receiver
	RENDER_CONTEXT				*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	NODE						*pNode = DisplayParameters.GetNode();

	if	(_pSolver)
	{
		pContext->SetWorldTransform0(&MATRIX::IDENTITY);
		_BoundingBox.Display(Red);
	}
	else
	{
		if (GetFlags(STATIC_GEOMETRY))
		{
			pContext->SetWorldTransform0(&MATRIX::IDENTITY);

			if (pNode->GetNbrShadowMapEmitters() > 0)
			{
				_pBoundingBox->Display(Cyan);
			}
			else
			{
				if	(pNode->GetNbrShadowMapReceivers() > 0)
				{
					_pBoundingBox->Display(Magenta);
				}
				else
				{
					_pBoundingBox->Display(White);
				}
			}
		}
		else	// DYNAMIC_GEOMETRY
		{
			pContext->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
	
			if (pNode->GetNbrShadowMapEmitters() > 0)
			{
				_pBoundingBox->Display(Cyan);
			}
			else
			{
				if	(pNode->GetNbrShadowMapReceivers() > 0)
				{
					_pBoundingBox->Display(Magenta);
				}
				else
				{
					_pBoundingBox->Display(Red);
				}
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplayPhysicElements
//	Object:		
//	02-03-27:	ELE - Created
//-----------------------------------------------------------------------------
void SOFT_BODY::DisplayPhysicElements(DISPLAY_PARAMETERS &DisplayParameters)
{
#ifndef _MASTER
	const COLOR_FLOAT Red(1.0f, 1.0f, 0.0f, 0.0f);
	RENDER_CONTEXT	 *pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	
	if	(_pSolver)
	{
		pContext->SetWorldTransform0(&MATRIX::IDENTITY);
	}
	else
	{
		if	(GetFlags(STATIC_GEOMETRY))
		{
			pContext->SetWorldTransform0(&MATRIX::IDENTITY);
		}
		else
		{
			pContext->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
		}
	}

	if (_pSolver)
	{
		_pSolver->Display(Red);
	}
	else
	{
		if (_pLattice)
		{
			_pLattice->Display(Red);
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CreateForce
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
int	SOFT_BODY::CreateForce(int	nForceType)
{
	FORCE *pForce = NULL;

	if (!_pSolver)
		return -1;

	switch(nForceType)
	{
	case PHYSIC_MANAGER::GRAVITY_FORCE :
		pForce = _pSolver->CreateForce(PHYSICAL_ENTITY::GRAVITY_FORCE);
		break;
	case PHYSIC_MANAGER::WIND_FORCE :
		pForce = _pSolver->CreateForce(PHYSICAL_ENTITY::WIND_FORCE);
		break;
	}

	if (pForce)
		return (pForce->GetID());
	else
		return (-1);
}

//-----------------------------------------------------------------------------
//	Name:		DeleteForce
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::DeleteForce(int	nForceIdx)
{
	if	(_pSolver)
	{
		_pSolver->DeleteForce(nForceIdx);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateGravity
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::UpdateGravity(int	nForceIdx,
								 float	rI)
{
	if	(_pSolver == NULL)	return;

	_pSolver->UpdateGravity(nForceIdx, rI);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateForce
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::UpdateWind(int				nForceIdx,
							  const VECTOR &	Dir,
							  float				rMinI,
							  float				rMaxI)
{
	if	(_pSolver == NULL)	return;

	VECTOR	V = Dir;
	V.Normalize();

	_pSolver->UpdateWind(nForceIdx, V, rMinI, rMaxI);
}

//-----------------------------------------------------------------------------
//	Name:		ClearForces
//	Object:		
//	03-11-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::ClearForces()
{
	if	(_pSolver)
	{
		_pSolver->ClearForces();
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddBoundingVolume
//	Object:		
//	02-12-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::AddBoundingVolume(NODE *					pMeshNode,
									 BOUNDING_VOLUME_TYPE	BVType,
									 float					rE)
{
	if	(_pSolver == NULL) return;

	bool					bDynamic = false;
	ENTITY_ABC				*pEntity = pMeshNode->GetEntity();
	const BOUNDING_VOLUME	*pBV;
	
	switch	(BVType)
	{
	default:
		QDT_FAIL();
		pBV = NULL;
		break;

	case	BV_SPHERE:
		{
			if	(pEntity->GetEntityType() == ENTITY_SKELETON)
			{
				pBV = NULL;
			}
			else
			{
				pBV = static_cast<MESH *>(pEntity)->GetBoundingSphere();
				bDynamic = static_cast<MESH *>(pEntity)->GetFlags(MESH::DYNAMIC_GEOMETRY) > 0;
			}
		}
		break;

	case	BV_BOX:
		{
			if	(pEntity->GetEntityType() == ENTITY_SKELETON)
			{
				pBV = NULL;
			}
			else
			{
				pBV = static_cast<MESH *>(pEntity)->GetBoundingBox();
				bDynamic = static_cast<MESH *>(pEntity)->GetFlags(MESH::DYNAMIC_GEOMETRY) > 0;
			}
		}
		break;

	case	BV_CYLINDERS:
		{
			if	(pEntity->GetEntityType() == ENTITY_SKELETON)
			{
				SKELETON	*pSkeleton = static_cast<SKELETON *>(pEntity);

				pBV = pSkeleton->GetGlobalBoundingCylinder(pMeshNode);
				pSkeleton->NeedToUpdateCylinder(true);

				bDynamic = false;
			}
			else
			{
				pBV = NULL;
			}
		}
		break;
	}

	if (!pBV)
		return;

	_pSolver->AddBoundingVolume(pBV, &pMeshNode->GetGlobalMatrix(), bDynamic, 0.0f, rE);
}

//-----------------------------------------------------------------------------
//	Name:		AddBoundingVolume
//	Object:		
//	03-11-28:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::AddBoundingVolume(MATRIX *			pMatrix,
									 BOUNDING_VOLUME *	pBV)
{
	if	(_pSolver == NULL) return;
	if	(pBV == NULL) return;

	_pSolver->AddBoundingVolume(pBV, pMatrix, true, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		AddBoundingAABB
//	Object:		
//	03-11-05:	ELE - Created
//-----------------------------------------------------------------------------
int	SOFT_BODY::AddBoundingAABB(float	rMinX,
							   float	rMinY,
							   float	rMinZ,
							   float	rMaxX,
							   float	rMaxY,
							   float	rMaxZ)
{
	if	(_pSolver)
	{
		int									nBVId;
		QDT_PAIR <BOUNDING_VOLUME *, int>	BVPair;
		AABB								*pAABB = new AABB(rMinX, rMinY, rMinZ, rMaxX, rMaxY, rMaxZ);

		nBVId = _nBVsIds;
		_nBVsIds++;

		BVPair.SetFirst(pAABB);
		BVPair.SetSecond(nBVId);

		_IntersectionBVList.PushTail(BVPair);

		_pSolver->AddBoundingVolume(pAABB, NULL, false, 0.0f, 0.0f);

		return	(nBVId);
	}
	else
	{
		return	(-1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddBoundingSphere
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
int	SOFT_BODY::AddBoundingSphere(const VECTOR &	Center,
								 float			rRadius)
{
	if	(_pSolver)
	{
		int									nBVId;
		QDT_PAIR <BOUNDING_VOLUME *, int>	BVPair;
		SPHERE								*pSphere = new SPHERE(Center, rRadius);

		nBVId = _nBVsIds;
		_nBVsIds++;

		BVPair.SetFirst(pSphere);
		BVPair.SetSecond(nBVId);

		_IntersectionBVList.PushTail(BVPair);

		_pSolver->AddBoundingVolume(pSphere, NULL, false, 0.0f, 0.0f);

		return	(nBVId);
	}
	else
	{
		return	(-1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddBoundingCylinder
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
int	SOFT_BODY::AddBoundingCylinder(const VECTOR &	Dir,
								   const VECTOR &	Center,
								   float			rRadius,
								   float			rHeight)
{
	if	(_pSolver)
	{
		int									nBVId;
		QDT_PAIR <BOUNDING_VOLUME *, int>	BVPair;
		CYLINDER							*pCylinder = new CYLINDER(Center, Dir, rRadius, rHeight);

		nBVId = _nBVsIds;
		_nBVsIds++;

		BVPair.SetFirst(pCylinder);
		BVPair.SetSecond(nBVId);

		_IntersectionBVList.PushTail(BVPair);

		_pSolver->AddBoundingVolume(pCylinder, NULL, false, 0.0f, 0.0f);

		return	(nBVId);
	}
	else
	{
		return	(-1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddPlane
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
int	SOFT_BODY::AddPlane(float	ra,
						float	rb,
						float	rc,
						float	rd)
{
	if	(_pSolver)
	{
		PLANE	Plane(ra, rb, rc, rd);

		return	(_pSolver->AddPlane(Plane));
	}
	else
	{
		return	(-1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveBoundingVolume
//	Object:		
//	02-12-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::RemoveBoundingVolume(NODE *					pMeshNode,
										BOUNDING_VOLUME_TYPE	BVType)
{
	if	(_pSolver == NULL)	return;

	ENTITY_ABC				*pEntity = pMeshNode->GetEntity();
	const BOUNDING_VOLUME	*pBV;

	switch	(BVType)
	{
	default:
		QDT_FAIL();
		pBV = NULL;
		break;

	case	BV_SPHERE:
		{
			if	(pEntity->GetEntityType() == ENTITY_SKELETON)
			{
				pBV = NULL;
			}
			else
			{
				pBV = static_cast<MESH *>(pEntity)->GetBoundingSphere();
			}
		}
		break;

	case	BV_BOX:
		{
			if	(pEntity->GetEntityType() == ENTITY_SKELETON)
			{
				pBV = NULL;
			}
			else
			{
				pBV = static_cast<MESH *>(pEntity)->GetBoundingBox();
			}
		}
		break;

	case	BV_CYLINDERS:
		{
			if	(pEntity->GetEntityType() == ENTITY_SKELETON)
			{
				SKELETON	*pSkeleton = static_cast<SKELETON *>(pEntity);

				pBV = pSkeleton->GetGlobalBoundingCylinder(pMeshNode);
				pSkeleton->NeedToUpdateCylinder(false);
			}
			else
			{
				pBV = NULL;
			}
		}
		break;
	}

	if (!pBV)
		return;

	_pSolver->RemoveBoundingVolume(pBV);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveBoundingVolume
//	Object:		
//	03-11-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::RemoveBoundingVolume(int	nId)
{
	if	(_pSolver == NULL)	return;

	QDT_DLIST< QDT_PAIR <BOUNDING_VOLUME *, int> >::ITERATOR	It = _IntersectionBVList.GetHead();
	QDT_DLIST< QDT_PAIR <BOUNDING_VOLUME *, int> >::ITERATOR	ItEnd = _IntersectionBVList.GetTail();

	while	(It != ItEnd)
	{
		if	((*It).GetSecond() == nId)
		{
			_pSolver->RemoveBoundingVolume((*It).GetFirst());

			delete	((*It).GetFirst());
			_IntersectionBVList.Remove(It);

			return;
		}

		++It;
	}

	RemovePlane(nId);
}

//-----------------------------------------------------------------------------
//	Name:		RemovePlane
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::RemovePlane(int	nId)
{
	if	(_pSolver)
	{
		_pSolver->RemovePlane(nId);
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayBoundingVolumes
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::DisplayBoundingVolumes(bool	b)
{
#ifndef _MASTER
	if	(_pSolver)
	{
		_pSolver->DisplayBoundingVolumes(b);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		ClearBoundingVolumes
//	Object:		
//	03-11-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::ClearBoundingVolumes()
{
	if	(_pSolver == NULL)	return;

	QDT_DLIST< QDT_PAIR <BOUNDING_VOLUME *, int> >::ITERATOR	It;


	It = _IntersectionBVList.GetHead();
	while	(It != _IntersectionBVList.GetTail())
	{
		_pSolver->RemoveBoundingVolume((*It).GetFirst());

		delete	((*It).GetFirst());
		_IntersectionBVList.Remove(It);

		It = _IntersectionBVList.GetHead();
	}

	_pSolver->ClearPlanes();
}

//-----------------------------------------------------------------------------
//	Name:		SetPause
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::SetPause(bool	b)
{
	if	(_pSolver)
	{
		_pSolver->ForcePause(b);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsVisible
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	SOFT_BODY::IsVisible(DISPLAY_PARAMETERS &	DisplayParameters,
							 BOUNDING_VOLUME * *	pGlobalBV,
							 bool					bDoVisibility)
{
	DISPLAY_MANAGER		*pDisplay		= DISPLAY_MANAGER::Instance();
	SORTED_PORTAL_ZONE  *pCurrentZone	= pDisplay->GetCurrentPortalZone();
	bool				 bCollide = true;
	bool				 bIntersectNear = false;

	QDT_ASSERT(pDisplay);
	QDT_ASSERT(pCurrentZone);

	if	(_pSolver)
	{
		*pGlobalBV = &_BoundingBox;
	}
	else
	{
		if (GetFlags(MESH::STATIC_GEOMETRY))
		{
			QDT_ASSERT(DisplayParameters.GetNode()->GetFlags().IsSet(NODE::STATIC_MATRIX));
			*pGlobalBV = _pBoundingBox;
		}
		else
		{
			_GlobalBoundingBox = *_pBoundingBox;
			_GlobalBoundingBox.Transform(DisplayParameters.GetNode()->GetGlobalMatrix());
			*pGlobalBV = &_GlobalBoundingBox;
		}
	}

	// Must be done here because *pGlobalBV have to be initialized !!
	if (!bDoVisibility)
	{
		if	(_pSolver)
		{
			_pSolver->SetPaused(false);
		}

		return (true);
	}

	switch	(pCurrentZone->GetType())
	{
		default:
		case SORTED_PORTAL_ZONE::T_CAMERA:
		{
			bCollide = (*pGlobalBV)->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
			DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
		}
		break;

		case SORTED_PORTAL_ZONE::T_MIRROR:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;
	
			bCollide = false;

			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

			while	(ItL != ItLEnd)
			{
				bCollide = bCollide || (*pGlobalBV)->IntersectFront(*(*ItL), bIntersectNear);
#ifndef _MASTER
				DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
				++ItL;
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				QDT_ASSERT(pMF);

				bCollide = (*pGlobalBV)->IntersectFront(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
				DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
			}
		}
		break;
		
		case SORTED_PORTAL_ZONE::T_TWIN:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;
			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	ItEnd = pHexahedrons->GetTail();

			while	((It != ItEnd) && bCollide)
			{
				bCollide = false;

				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

				while	(ItL != ItLEnd)
				{
					bCollide = bCollide || (*pGlobalBV)->Intersect(*(*ItL), bIntersectNear);
#ifndef _MASTER
					DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
					++ItL;
				}

				++It;
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				if	(pMF)
				{
					bCollide = (*pGlobalBV)->Intersect(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
				}
				else
				{
					bCollide = (*pGlobalBV)->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
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

	if	(_pSolver)
	{
		if	(!bCollide)
		{
			BOUNDING_VOLUME				*pBV;
			PHYSICAL_PARTICLE_SOLVER	*pSolver = _pSolver;

			_pSolver = NULL;

			if	(IsVisible(DisplayParameters, &pBV, true))
			{
				_pSolver = pSolver;
				_pSolver->SetPaused(false);
			}
			else
			{
				_pSolver = pSolver;
				_pSolver->SetPaused(true);
			}
		}
		else
		{
			_pSolver->SetPaused(false);
		}
	}

	return (bCollide);
}


//-----------------------------------------------------------------------------
//	Name:		TestShadowVisibility
//	Object:		
//	03-10-09:	SBE - Created
//-----------------------------------------------------------------------------
bool	SOFT_BODY::TestShadowVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
/*	DISPLAY_MANAGER		*pDisplay		= DISPLAY_MANAGER::Instance();
	SORTED_PORTAL_ZONE  *pCurrentZone	= pDisplay->GetCurrentPortalZone();

	bool				 bCollide = true;
	bool				 bIntersectNear;
	BOUNDING_VOLUME *	 pGlobalBV;

	QDT_ASSERT(pDisplay);
	QDT_ASSERT(pCurrentZone);

	if	(_pSolver)
	{
		pGlobalBV = &_BoundingBox;
	}
	else
	{
		if (GetFlags(MESH::STATIC_GEOMETRY))
		{
			QDT_ASSERT(DisplayParameters.GetNode()->GetFlags().IsSet(NODE::STATIC_MATRIX));
			pGlobalBV = _pBoundingBox;
		}
		else
		{
			_GlobalBoundingBox = *_pBoundingBox;
			_GlobalBoundingBox.Transform(DisplayParameters.GetNode()->GetGlobalMatrix());
			pGlobalBV = &_GlobalBoundingBox;
		}
	}

	bCollide = pGlobalBV->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
	DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif

	if (bCollide)
	{
		if	(bIntersectNear)
		{
			SetFlags(DO_SHADOW_CLIPPING, DO_SHADOW_CLIPPING);
		}
		else
		{
			SetFlags(DO_SHADOW_CLIPPING, 0);
		}
	}

	return (bCollide);*/

	return (false);
}


//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR	*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Methodes	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDamping"), QDT_COMMENT("Change damping of soft body."));
	pMD->AddParameter(QDT_COMMENT("nDamping"), QDT_COMMENT("Value between 0 and 100000"), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_DAMPING, pMD, EM_SetDamping);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetStiffness"), QDT_COMMENT("Change stiffness of all springs."));
	pMD->AddParameter(QDT_COMMENT("rStiffness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_STIFFNESS, pMD, EM_SetStiffness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetStretchingFactor"), QDT_COMMENT("Change stretching factor of all springs."));
	pMD->AddParameter(QDT_COMMENT("rStretching"), QDT_COMMENT("In % of original length, value >= 100 only."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_STRETCHING, pMD, EM_SetStretching);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetContractionFactor"), QDT_COMMENT("Change contraction factor of all springs."));
	pMD->AddParameter(QDT_COMMENT("rContraction"), QDT_COMMENT("In % of original length, value between 0 & 100 included only."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_CONTRACTION, pMD, EM_SetContraction);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetAxe"), QDT_COMMENT("Change axe of axial particles."));
	pMD->AddParameter(QDT_COMMENT("Vector"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_SET_AXE, pMD, EM_SetAxe);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("PlayAnim"), QDT_COMMENT("Play animation on animated particles."));
	pMD->AddParameter(QDT_COMMENT("Animation"), QDT_COMMENT(""), ANIM_PC::GetGenericCommunicatorDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSpeed"), QDT_COMMENT("Animation speed"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("bCollide"), QDT_COMMENT("Animated particle collide with other ones"), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PLAY_ANIM, pMD, EM_PlayAnim);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("StopAnim"), QDT_COMMENT("Stop animation on animated particles."));
	pCD->AddMethod(CMD_STOP_ANIM, pMD, EM_StopAnim);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddCollisionPlane"), QDT_COMMENT("Add a collision plane on soft body."));
	pMD->AddParameter(QDT_COMMENT("a"), QDT_COMMENT("x of the plane normal"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("b"), QDT_COMMENT("y of the plane normal"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("c"), QDT_COMMENT("z of the plane normal"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("d"), QDT_COMMENT("d of plane"), TYPE_FLOAT::GetDescriptor());
	pMD->SetReturn(QDT_COMMENT("nPlaneId"), QDT_COMMENT("Id of the created plane (to remove it)."), TYPE_INT::GetDescriptor()); 
	pCD->AddMethod(CMD_ADD_COLLISION_PLANE, pMD, EM_AddCollisionPlane);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveCollisionPlane"), QDT_COMMENT("Remove a collision plane on soft body."));
	pMD->AddParameter(QDT_COMMENT("nPlaneId"), QDT_COMMENT("Id of the plane to remove."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_REMOVE_COLLISION_PLANE, pMD, EM_RemoveCollisionPlane);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddCollisionAABB"), QDT_COMMENT("Add a collision AABB on soft body."));
	pMD->AddParameter(QDT_COMMENT("MinX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("MinY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("MinZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("MaxX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("MaxY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("MaxZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->SetReturn(QDT_COMMENT("nAABBId"), QDT_COMMENT("Id of the created AABB (to remove it)."), TYPE_INT::GetDescriptor()); 
	pCD->AddMethod(CMD_ADD_COLLISION_AABB, pMD, EM_AddCollisionAABB);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveCollisionAABB"), QDT_COMMENT("Remove a collision AABB on soft body."));
	pMD->AddParameter(QDT_COMMENT("nAABBId"), QDT_COMMENT("Id of the AABB to remove."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_REMOVE_COLLISION_AABB, pMD, EM_RemoveCollisionAABB);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddCollisionSphere"), QDT_COMMENT("Add a collision sphere on soft body."));
	pMD->AddParameter(QDT_COMMENT("Center"), QDT_COMMENT("Center of the sphere"), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Radius"), QDT_COMMENT("Radius of the sphere"), TYPE_FLOAT::GetDescriptor());
	pMD->SetReturn(QDT_COMMENT("nSphereId"), QDT_COMMENT("Id of the created sphere (to remove it)."), TYPE_INT::GetDescriptor()); 
	pCD->AddMethod(CMD_ADD_COLLISION_SPHERE, pMD, EM_AddCollisionSphere);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveCollisionSphere"), QDT_COMMENT("Remove a collision sphere on soft body."));
	pMD->AddParameter(QDT_COMMENT("nSphereId"), QDT_COMMENT("Id of the sphere to remove."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_REMOVE_COLLISION_SPHERE, pMD, EM_RemoveCollisionSphere);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddCollisionCylinder"), QDT_COMMENT("Add a collision cylinder on soft body."));
	pMD->AddParameter(QDT_COMMENT("Direction"), QDT_COMMENT("Direction of the cylinder (base)"), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Center"), QDT_COMMENT("Center of the cylinder (base)"), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Radius"), QDT_COMMENT("Radius of the cylinder"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Height"), QDT_COMMENT("Height of the cylinder"), TYPE_FLOAT::GetDescriptor());
	pMD->SetReturn(QDT_COMMENT("nSphereId"), QDT_COMMENT("Id of the created cylinder (to remove it)."), TYPE_INT::GetDescriptor()); 
	pCD->AddMethod(CMD_ADD_COLLISION_CYLINDER, pMD, EM_AddCollisionCylinder);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveCollisionCylinder"), QDT_COMMENT("Remove a collision cylinder on soft body."));
	pMD->AddParameter(QDT_COMMENT("nCylinderId"), QDT_COMMENT("Id of the cylinder to remove."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_REMOVE_COLLISION_CYLINDER, pMD, EM_RemoveCollisionCylinder);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddBoundingVolume"), QDT_COMMENT("Add a bounding volume collision on soft body."));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT("Node containing bounding volume"), NODE::GetGenericCommunicatorDescriptor());
	pMD->AddParameter(QDT_COMMENT("nType"), QDT_COMMENT("Bounding volume type. 0 : Sphere, 1: Box, 2 : Cylinder."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rE"), QDT_COMMENT("Epsilon on point replace"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_ADD_BOUNDING_VOLUME, pMD, EM_AddBoundingVolume);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveBoundingVolume"), QDT_COMMENT("Remove a bounding volume collision on soft body."));
	pMD->AddParameter(QDT_COMMENT("Node"), QDT_COMMENT("Node containing bounding volume"), NODE::GetGenericCommunicatorDescriptor());
	pMD->AddParameter(QDT_COMMENT("nType"), QDT_COMMENT("Bounding volume type"), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_REMOVE_BOUNDING_VOLUME, pMD, EM_RemoveBoundingVolume);

#ifndef _MASTER
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("DisplayBoundingVolumes"), QDT_COMMENT("Display collision bounding volumes on soft body."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_DISPLAY_BOUNDING_VOLUMES, pMD, EM_DisplayBoundingVolume);
#else
	pCD->AddMethod(CMD_DISPLAY_BOUNDING_VOLUMES, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("ClearBoundingVolumes"), QDT_COMMENT("Remove all collision bounding volumes on soft body."));
	pCD->AddMethod(CMD_CLEAR_BOUNDING_VOLUMES, pMD, EM_ClearBoundingVolumes);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("CreateForce"), QDT_COMMENT("Create a force, return force created index."));
	pMD->AddParameter(QDT_COMMENT("nForceType"), QDT_COMMENT("Type of force. 0 : Gravity, 1 : Wind."), TYPE_INT::GetDescriptor()); 
	pMD->SetReturn(QDT_COMMENT("nForceIndex"), QDT_COMMENT("Index of the force created"), TYPE_INT::GetDescriptor()); 
	pCD->AddMethod(CMD_CREATE_FORCE, pMD, EM_CreateForce);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("DeleteForce"), QDT_COMMENT("Delete a force."));
	pMD->AddParameter(QDT_COMMENT("nForceIndex"), QDT_COMMENT("Index of the force to delete"), TYPE_INT::GetDescriptor()); 
	pCD->AddMethod(CMD_DELETE_FORCE, pMD, EM_DeleteForce);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UpdateGravity"), QDT_COMMENT("Update gravity force parameters."));
	pMD->AddParameter(QDT_COMMENT("nForceIndex"), QDT_COMMENT("Index of the force to update"), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nIntensity"), QDT_COMMENT("Intensity of the force"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_UPDATE_GRAVITY, pMD, EM_UpdateGravity);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UpdateWind"), QDT_COMMENT("Update wind force parameters."));
	pMD->AddParameter(QDT_COMMENT("Direction"), QDT_COMMENT("Direction of the force"), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nForceIndex"), QDT_COMMENT("Index of the force to update"), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nMinIntensity"), QDT_COMMENT("Intensity min of the force"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nMaxIntensity"), QDT_COMMENT("Intensity max of the force"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_UPDATE_WIND, pMD, EM_UpdateWind);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("ClearForces"), QDT_COMMENT("Remove all forces on soft body."));
	pCD->AddMethod(CMD_CLEAR_FORCES, pMD, EM_ClearForces);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetPause"), QDT_COMMENT("Pause physics on soft body."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PAUSE, pMD, EM_Pause);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("Reset"), QDT_COMMENT("Reset physics on soft body."));
	pCD->AddMethod(CMD_RESET, pMD, EM_Reset);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SOFTBODY", 1, CM_ReadChunk);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SOFTBODY", 2, CM_ReadChunk2);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("SOFTBODY", 2, CM_WriteChunk2);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	SOFT_BODY::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SOFT_BODY) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	SOFT_BODY::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("SOFT_BODY"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SOFT_BODY,
											QDT_COMMENT("31CDE481-0601-64AF-AF21-8C56EF1247CB"),
											sizeof(SOFT_BODY),
											ComInit,
											QDT_COMMENT("Class for soft bodies"),
											MESH::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	SOFT_BODY	*pSoftBody;
	pSoftBody = static_cast<SOFT_BODY *>(pO);

	QDT_NEW_PLACED(pSoftBody, SOFT_BODY)();
	QDT_ASSERT(pSoftBody);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	SOFT_BODY	*pSoftBody;
	pSoftBody = static_cast<SOFT_BODY *>(pO);

	QDT_DELETE_PLACED(pSoftBody, SOFT_BODY);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDamping
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_SetDamping(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	int				*pParamater;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParamater		= static_cast<int *>(pP);

	pSoftBody->SetDamping(*pParamater);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetStiffness
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_SetStiffness(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{	
	float			*pParameter;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<float *>(pP);

	pSoftBody->SetStiffness(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetStretching
//	Object:		
//	03-11-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_SetStretching(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	float			*pParamater;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParamater		= static_cast<float *>(pP);

	pSoftBody->SetStretching(*pParamater);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetContraction
//	Object:		
//	03-11-13:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_SetContraction(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	float			*pParamater;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParamater		= static_cast<float *>(pP);

	pSoftBody->SetContraction(*pParamater);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAxe
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_SetAxe(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	VECTOR			*pParamater;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParamater		= static_cast<VECTOR *>(pP);

	VECTOR	AlignedVector(*pParamater, VECTOR::VM_OBJECT);

	pSoftBody->SetAxe(AlignedVector);
}

//-----------------------------------------------------------------------------
//	Name:		EM_PlayAnim
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_PlayAnim(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{	
	struct	PARAMETERS
	{
		COM_INSTANCE	_AnimDataInst;
		float			_rSpeed;
		bool			_bCollide;
	};

	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	pSoftBody->PlayAnim(pParameters->_AnimDataInst, pParameters->_rSpeed, pParameters->_bCollide);
}

//-----------------------------------------------------------------------------
//	Name:		EM_StopAnim
//	Object:		
//	03-10-23:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_StopAnim(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());

	pSoftBody->StopAnim();
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddCollisionPlane
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_AddCollisionPlane(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	struct	PARAMETERS
	{
		float	_ra;
		float	_rb;
		float	_rc;
		float	_rd;
	};
	
	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;
	int				*pnPlaneIndex;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);
	pnPlaneIndex    = static_cast<int*>(pR);

	*pnPlaneIndex = pSoftBody->AddPlane(pParameters->_ra, pParameters->_rb, pParameters->_rc, pParameters->_rd);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveCollisionPlane
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_RemoveCollisionPlane(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	int				*pParameter;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<int *>(pP);

	pSoftBody->RemovePlane(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddCollisionAABB
//	Object:		
//	03-11-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_AddCollisionAABB(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	struct	PARAMETERS
	{
		float	_rMinX;
		float	_rMinY;
		float	_rMinZ;
		float	_rMaxX;
		float	_rMaxY;
		float	_rMaxZ;
	};
	
	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;
	int				*pnBVIndex;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);
	pnBVIndex		= static_cast<int*>(pR);

	*pnBVIndex = pSoftBody->AddBoundingAABB(pParameters->_rMinX, pParameters->_rMinY, pParameters->_rMinZ,
											pParameters->_rMaxX, pParameters->_rMaxY, pParameters->_rMaxZ);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveCollisionAABB
//	Object:		
//	03-11-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_RemoveCollisionAABB(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	int				*pParameter;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<int *>(pP);

	pSoftBody->RemoveBoundingVolume(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddCollisionSphere
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_AddCollisionSphere(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	struct	PARAMETERS
	{
		VECTOR	_Center;
		float	_rRadius;
	};
	
	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;
	int				*pnBVIndex;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);
	pnBVIndex		= static_cast<int*>(pR);

	VECTOR	AlignedPoint(pParameters->_Center, VECTOR::VM_OBJECT);

	*pnBVIndex = pSoftBody->AddBoundingSphere(AlignedPoint, pParameters->_rRadius);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveCollisionSphere
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_RemoveCollisionSphere(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	int				*pParameter;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<int *>(pP);

	pSoftBody->RemoveBoundingVolume(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddCollisionCylinder
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_AddCollisionCylinder(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	struct	PARAMETERS
	{
		VECTOR	_Dir;
		VECTOR	_Center;
		float	_rRadius;
		float	_rHeight;
	};
	
	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;
	int				*pnBVIndex;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);
	pnBVIndex		= static_cast<int*>(pR);

	VECTOR	AlignedVector(pParameters->_Dir, VECTOR::VM_OBJECT);
	VECTOR	AlignedPoint(pParameters->_Center, VECTOR::VM_OBJECT);

	*pnBVIndex = pSoftBody->AddBoundingCylinder(AlignedVector, AlignedPoint, pParameters->_rRadius, pParameters->_rHeight);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveCollisionCylinder
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_RemoveCollisionCylinder(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	int				*pParameter;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<int *>(pP);

	pSoftBody->RemoveBoundingVolume(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddBoundingVolume
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_AddBoundingVolume(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	struct	PARAMETERS
	{
		COM_INSTANCE	_NodeInst;
		int				_nBVType;
		float			_rE;
	};
	
	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	NODE	*pNode = static_cast<NODE *>(pParameters->_NodeInst.GetCommunicator());

	pSoftBody->AddBoundingVolume(pNode, (SOFT_BODY::BOUNDING_VOLUME_TYPE)(pParameters->_nBVType), pParameters->_rE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveBoundingVolume
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_RemoveBoundingVolume(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	struct	PARAMETERS
	{
		COM_INSTANCE	_NodeInst;
		int				_nBVType;
	};
	
	PARAMETERS		*pParameters;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	NODE	*pNode = static_cast<NODE *>(pParameters->_NodeInst.GetCommunicator());

	pSoftBody->RemoveBoundingVolume(pNode, (SOFT_BODY::BOUNDING_VOLUME_TYPE)(pParameters->_nBVType));
}

//-----------------------------------------------------------------------------
//	Name:		EM_DisplayBoundingVolume
//	Object:		
//	03-11-06:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_DisplayBoundingVolume(void * const	pO,
											void * const	pR,
											void * const	pP)
{
#ifndef _MASTER
	bool			*pParameter;
	SOFT_BODY		*pSoftBody;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<bool *>(pP);

	pSoftBody->DisplayBoundingVolumes(*pParameter);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearBoundingVolumes
//	Object:		
//	03-11-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_ClearBoundingVolumes(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());

	pSoftBody->ClearBoundingVolumes();
}

//-----------------------------------------------------------------------------
//	Name:		EM_CreateForce
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_CreateForce(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	int				*pnForceType	= static_cast<int *>(pP);
	int				*pnForceIndex	= static_cast<int *>(pR);

	*pnForceIndex = pSoftBody->CreateForce(*pnForceType);
}

//-----------------------------------------------------------------------------
//	Name:		EM_DeleteForce
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_DeleteForce(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	int				*pnForceIndex	= static_cast<int *>(pP);

	pSoftBody->DeleteForce(*pnForceIndex);
}

//-----------------------------------------------------------------------------
//	Name:		EM_UpdateGravity
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_UpdateGravity(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	struct PARAMETERS
	{
		int		_nForceIdx;
		float	_rIntensity;
	};

	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	PARAMETERS		*pParams		= static_cast<PARAMETERS *>(pP);

	pSoftBody->UpdateGravity(pParams->_nForceIdx, pParams->_rIntensity);
}

//-----------------------------------------------------------------------------
//	Name:		EM_UpdateWind
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_UpdateWind(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	struct PARAMETERS
	{
		VECTOR			_Direction;
		int				_nForceIdx;
		float			_rMinIntensity;
		float			_rMaxIntensity;
	};

	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	PARAMETERS		*pParams		= static_cast<PARAMETERS *>(pP);
	
	VECTOR	AlignedVector(pParams->_Direction, VECTOR::VM_OBJECT);

	pSoftBody->UpdateWind(pParams->_nForceIdx, AlignedVector, pParams->_rMinIntensity, pParams->_rMaxIntensity);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearForces
//	Object:		
//	03-11-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_ClearForces(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());

	pSoftBody->ClearForces();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Pause
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_Pause(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());
	bool			*pbPause		= static_cast<bool *>(pP);

	pSoftBody->SetPause(*pbPause);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Reset
//	Object:		
//	03-11-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::EM_Reset(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	SOFT_BODY		*pSoftBody		= static_cast<SOFT_BODY *>(pComInstance->GetCommunicator());

	pSoftBody->ResetPhysic();
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::CM_ReadChunk(IO_STREAM &		Stream,
								COMMUNICATOR *	pCommunicator)
{
	SOFT_BODY		*pSoftBody;

	pSoftBody = static_cast<SOFT_BODY *>(pCommunicator);
	
	pSoftBody->_pExchanger = new PARTICLE_EXCHANGER;
	pSoftBody->_pExchanger->SetGeometries(pSoftBody->_pGeometriesArray);
	Stream >> *pSoftBody->_pExchanger;

	pSoftBody->_pLattice = new LATTICE;
	pSoftBody->_pLattice->Read(Stream, pSoftBody->_pExchanger);

	pSoftBody->_AnimatedNodeInst.SetHandle(NODE::GetGenericCommunicatorDescriptor()->Create());
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	03-12-19:	FPR - Created
//-----------------------------------------------------------------------------
void	SOFT_BODY::CM_ReadChunk2(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	SOFT_BODY		*pSoftBody;

	pSoftBody = static_cast<SOFT_BODY *>(pCommunicator);
	
	pSoftBody->_pExchanger = new PARTICLE_EXCHANGER;
	pSoftBody->_pExchanger->SetGeometries(pSoftBody->_pGeometriesArray);
	Stream >> *pSoftBody->_pExchanger;

	pSoftBody->_pLattice = new LATTICE;
	pSoftBody->_pLattice->Read(Stream, pSoftBody->_pExchanger);

	pSoftBody->_AnimatedNodeInst.SetHandle(NODE::GetGenericCommunicatorDescriptor()->Create());

}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk2
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	SOFT_BODY::CM_WriteChunk2(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	SOFT_BODY	*pSoftBody;

	pSoftBody = static_cast<SOFT_BODY *>(pCommunicator);

	Stream.DebugOutput("Exchanger :\r\n");
	Stream << *pSoftBody->_pExchanger;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Lattice :\r\n");
	pSoftBody->_pLattice->Write(Stream);
	Stream.DebugOutput("\r\n");


}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


