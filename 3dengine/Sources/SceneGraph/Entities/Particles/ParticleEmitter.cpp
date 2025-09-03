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
//	CLASS:	PARTICLE_EMITTER
//
//	01-11-20:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleEmitter)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/ForceFieldABC)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/LifeField/LifeField)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/DisplayParameters/DisplayParameters)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Circle/Circle)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Disc/Disc)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(ComplexType/TypeVector)

#ifdef _DEBUG
	#include	"ParticleEmitter.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

FORCE_FIELD_ABC *		PARTICLE_EMITTER::_ForceFields[MAX_FORCE_FIELDS];
bool					PARTICLE_EMITTER::_bOnlyUniformForceFields(true);
unsigned int			PARTICLE_EMITTER::_nNbrUsedForceFields = 0;

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_EMITTER constructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_EMITTER::PARTICLE_EMITTER()
:
ENTITY_ABC						(GetGenericCommunicatorDescriptor(), ENTITY_PARTICLE_EMITTER),
_rSpawnRate						(10.0f),
_rNewParticlesCounter			(0.0f),
_nShapeType						(PM_SHAPETYPE_NONE),
_pShape							(NULL),
_StartAngle						(VECTOR::ZERO),
_EndAngle						(VECTOR::ZERO),
_rEmissionSpeed					(0.0f),
_rEmissionSpinAngle				(0.0f),
_EmissionSpinAxis				(VECTOR::UNIT_Z), 		
_rEmissionSpinAngleSpeed		(0.0f),
_EmissionSpinAxisSpeed			(VECTOR::ZERO),
_ParticleShapeInst				(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_bActive						(true),
_bAutoSpawn						(true),
_bNeedUpdate					(true),
_bDoClipping					(false),
_bIsInfluencedByAllLifeFields	(false),
_rVisibilityDistance			(-1.0f),
_nNbrForceFields				(0),
_nNbrParticles					(0)
//_pParticleArray					(NULL),
//_pSpriteTable					(NULL)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		~PARTICLE_EMITTER destructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_EMITTER::~PARTICLE_EMITTER()
{
//	delete _pParticleArray;
//	_pParticleArray = NULL;

	DeleteEmissionShape();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_EMITTER &	PARTICLE_EMITTER::operator=(const PARTICLE_EMITTER &	C)
{
	if	(this != &C)
	{
		Copy(C, true);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::Register()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::Unregister()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::Instanciate(COM_INSTANCE &	Instance)
{
	PARTICLE_EMITTER *	pNewEmitter;

	Instance.SetHandle(PARTICLE_EMITTER::GetGenericCommunicatorDescriptor()->Create());
	pNewEmitter = static_cast<PARTICLE_EMITTER *>(Instance.GetCommunicator());

	*pNewEmitter = *this;

	InstanciateNode(pNewEmitter);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	03-10-23:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::Clear()
{
//	delete _pParticleArray;
//	delete _pSpriteTable;
//	_pParticleArray = NULL;
//	_pSpriteTable = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		AllocateParticles
//	Object:		
//	04-04-27:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::AllocateParticles()
{
	PARTICLE_SHAPE * pPS = GetParticleShape();
	QDT_ASSERT(pPS);

	_nNbrParticles = (int)(_rSpawnRate * pPS->GetLifeSpan()) + 1;
//	_pParticleArray = new PARTICLE_SPRITE_ARRAY(_nNbrParticles, (PARTICLE_SHAPE_SPRITE_DATA*)pPS->GetShapeData());
}

//-----------------------------------------------------------------------------
//	Name:		DisplayParticles
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::DisplayParticles(DISPLAY_PARAMETERS &	DisplayParameters,
										   bool					bDoVisibility)
{
	QDT_NOT_IMPLEMENTED();
/*
	// Display the emitter itself
#ifndef _MASTER
	if	(DISPLAY_MANAGER::Instance()->DisplayParticleEmitters())
	{
		float		rSize = 5.0f;
		COLOR_FLOAT	ColorFloat(1.0f, 1.0f, 1.0f, 0.0f);
		VECTOR		Point(VECTOR::ZERO);

		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&(DisplayParameters.GetNode()->GetGlobalMatrix()));

		switch (_nShapeType)
		{
			default:
				break;

			case PM_SHAPETYPE_NONE:
				Point.Display(ColorFloat, rSize);
				break;

			case PM_SHAPETYPE_POINT3D:
				_pPoint->Display(ColorFloat, rSize);
				break;

			case PM_SHAPETYPE_SEGMENT:
				_pSegment->Display(ColorFloat);
				break;

			case PM_SHAPETYPE_TRIANGLE:
				_pTriangle->Display(ColorFloat);
				break;

			case PM_SHAPETYPE_OBB:
				_pObb->Display(ColorFloat);
				break;

			case PM_SHAPETYPE_SPHERE:
				_pSphere->Display(ColorFloat);
				break;

			case PM_SHAPETYPE_DISC:
				_pDisc->Display(ColorFloat);
				break;
		}
	}
#endif

	// Then display its particles...
    if(TestVisibility(DisplayParameters))
	{	
		if (NeedUpdate())
		{
			#ifndef _MASTER
			#ifdef PLATFORM_XBOX
			RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Update particles", COLOR(255, 255, 0, 255));
			#endif
			#endif

			float	rDeltaTime	= TIME::Instance()->GetDefaultClock()->GetDeltaFrameTimeInS();
			UpdateParticleEmitter(rDeltaTime, DisplayParameters.GetNode());
			SetNeedUpdate(false);

			#ifndef _MASTER
			#ifdef PLATFORM_XBOX
			RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
			#endif
			#endif
		}

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Display particles", COLOR(255, 255, 0, 0));
		#endif
		#endif

		if	(_pSpriteTable)
		{
			// Initialize context
			MATRIX						ViewMatrix;
			NODE						*pCameraNode = DISPLAY_MANAGER::Instance()->GetCameraNode();
			RENDER_CONTEXT				*pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
			RENDER_CONTEXT::CULL_MODE	RMCullMode = pRenderContext->GetCullMode();
			bool						bAligned = static_cast<PARTICLE_SHAPE_SPRITE *>(_ParticleShapeInst.GetCommunicator())->IsAligned();

			ViewMatrix.InverseAffine(pCameraNode->GetGlobalMatrix());

			pRenderContext->EnableLighting(false);
			pRenderContext->EnableDepthWrite(false);
			pRenderContext->EnableAlphaTest(false);
			pRenderContext->EnableClipping(false);
			pRenderContext->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);
			pRenderContext->SetGlobalAmbientLight(COLOR_FLOAT(1.0f, 0.0f, 0.0f, 0.0f));

 			pRenderContext->SetWorldTransform0(NULL);
			pRenderContext->EnableVertexBlending(3, false);

			// If Locator is parent of emitter... There we care...
			OBB transBoundingBox = _BoundingBox;
			transBoundingBox.Transform(DisplayParameters.GetNode()->GetGlobalMatrix());

			if	(bAligned)
			{
					pRenderContext->EnableClipping(_bDoClipping);
					pRenderContext->SetBoundingVolume(&transBoundingBox);
					_pSpriteTable->SetViewMatrix(&ViewMatrix);				
					_pSpriteTable->AlignedDisplay();						
			}
			else
			{
				bool bSpin = (_rEmissionSpinAngle != 0) || (_rEmissionSpinAngleSpeed != 0.0f);

				if (bSpin == true || _pSpriteTable->SupportPointSprite() == false)
				{
					pRenderContext->SetViewTransform(NULL);
				}

				// Change fog to make particles appear smoothly
				float	rNinetyPercentOfVisibilityDistance = 0.9*_rVisibilityDistance;
				COLOR cPreviousFogColor = pRenderContext->GetFogColor();
				float rPreviousFogEnd	= pRenderContext->GetFogEnd();
				float rPreviousFogStart	= pRenderContext->GetFogStart();
				bool bFogEnabled		= pRenderContext->IsFogEnabled();

				if(rPreviousFogEnd>_rVisibilityDistance)
				{
					COLOR cBlackColor(1,0,0,0);
					pRenderContext->SetFogColor(cBlackColor);
					pRenderContext->SetFogEnd(_rVisibilityDistance);
					pRenderContext->SetFogStart(rNinetyPercentOfVisibilityDistance);
					pRenderContext->EnableFog(true);
				}

				if	(DisplayParameters.GetNode()->GetFlags(NODE::FORCE_CLIPPING) > 0)
				{
					pRenderContext->EnableClipping(true);
				}
				else
				{
					pRenderContext->EnableClipping(_bDoClipping);
				}
				pRenderContext->SetBoundingVolume(&transBoundingBox);
				_pSpriteTable->SetViewMatrix(&ViewMatrix);				
				_pSpriteTable->Display();				

				if(rPreviousFogEnd>_rVisibilityDistance)
				{
					//Give the context back
					pRenderContext->SetFogColor(cPreviousFogColor);
					pRenderContext->SetFogEnd(rPreviousFogEnd);
					pRenderContext->SetFogStart(rPreviousFogStart);
					pRenderContext->EnableFog(bFogEnabled);
				}

				pRenderContext->SetViewTransform(&ViewMatrix);
			}

			pRenderContext->EnableDepthWrite(true);
			pRenderContext->SetCullMode(RMCullMode);
		}

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
		#endif
		#endif
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE_EMITTER::TestVisibility(FRUSTRUM & Frustum)
{
	if	(_bAutoSpawn == false)
	{
		return	(true);
	}

	NODE * pNode = GetNode();

	DISPLAY_MANAGER		*pDisplay = DISPLAY_MANAGER::Instance();
	bool				 bCollide = true;
	bool				 bIntersectNear = false;
	
	// If Locator is parent of emitter... There we care...
	OBB transBoundingBox = _BoundingBox;
	transBoundingBox.Transform(pNode->GetGlobalMatrix());

	if(_rVisibilityDistance < 0.0f)
	{
		bCollide = transBoundingBox.Intersect(Frustum.GetHexahedron(), bIntersectNear);
#ifndef _MASTER
		DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
	}
	else
	{
		Frustum.SetFar(_rVisibilityDistance);
		bCollide = transBoundingBox.Intersect(Frustum.GetHexahedron(), bIntersectNear);
#ifndef _MASTER
		DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
	}

	if (bCollide)
	{
		_bDoClipping = bIntersectNear;
	}

	return (bCollide);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeForces
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::ComputeForces(unsigned int	i,
										float			rDeltaTime)
{
	QDT_NOT_IMPLEMENTED();
/*
	if	(_pParticleArray->NeedComputeForces(i))
	{
		VECTOR				Force;
		VECTOR				FinalForce(VECTOR::ZERO);
		unsigned int		j;

		for	(j = 0 ; j < _nNbrUsedForceFields ; ++j)
		{			
			if	(_ForceFields[j]->TestInfluenceVolume(_pParticleArray, i))
			{
				if	(_ForceFields[j]->IsLifeField())
				{
					_pParticleArray->SetLife(i, 0.0f);
				}
				else
				{
					_ForceFields[j]->GetForceToApply(_pParticleArray, i, Force);
					FinalForce.Add(Force);
				}
			}
		}

		FinalForce.Mul(_pParticleArray->GetOneOnMass());
		_pParticleArray->SetAcceleration(i, FinalForce);
		_pParticleArray->ResetComputeForcesTime(i);
	}
	else
	{
		_pParticleArray->UpdateComputeForcesTime(i, rDeltaTime);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		ComputeForcesNoForce
//	Object:		
//	04-11-10:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::ComputeForcesNoForce(unsigned int i, 
											   float rDeltaTime)
{
	QDT_NOT_IMPLEMENTED();
/*
	if	(_pParticleArray->NeedComputeForces(i))
	{
		_pParticleArray->ResetComputeForcesTime(i);
	}
	else
	{
		_pParticleArray->UpdateComputeForcesTime(i, rDeltaTime);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		ComputeForcesOneLifeField
//	Object:		
//	04-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::ComputeForcesOneLifeField(unsigned int i, 
													float rDeltaTime)
{
	QDT_NOT_IMPLEMENTED();
/*
	if	(_pParticleArray->NeedComputeForces(i))
	{		
		if	(_ForceFields[0]->TestInfluenceVolume(_pParticleArray, i))
		{
			_pParticleArray->SetLife(i, 0.0f);
		}

		_pParticleArray->ResetComputeForcesTime(i);
	}
	else
	{
		_pParticleArray->UpdateComputeForcesTime(i, rDeltaTime);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		SetUpForces
//	Object:		
//	03-12-16:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetUpForces(NODE * pNode)
{
	_bOnlyUniformForceFields = true;
	_nNbrUsedForceFields = 0;
	FORCE_FIELD_ABC * pField;

	int i;
	for (i = 0 ; i < _nNbrForceFields ; ++i)
	{
		if (i < MAX_FORCE_FIELDS)
		{
			pField = static_cast<FORCE_FIELD_ABC *>(_vForceFields[i].GetCommunicator());
			if (pField->IsActive())
			{
				_ForceFields[i] = pField;
				_ForceFields[i]->TransformedDatas(pNode->GetGlobalMatrix());
				++_nNbrUsedForceFields;

				if(_ForceFields[i]->IsVortexField() || 
				   _ForceFields[i]->IsRadialField())
				{
					_bOnlyUniformForceFields = false;
				}
			}
		}
	}

	if (_bIsInfluencedByAllLifeFields)
	{
		SCENE_GRAPH * pSG = GetSceneGraph();

/*		const QDT_DLIST <NODE *> & lForceFields = pSG->GetForceFields();

		QDT_DLIST <NODE *>::ITERATOR it = lForceFields.GetHead();
		QDT_DLIST <NODE *>::ITERATOR itTail = lForceFields.GetTail();

		while (it != itTail)
		{
			NODE * pForceField = (*it);
			if (pForceField->GetEntityInst().GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIFE_FIELD)
			{
				LIFE_FIELD * pLifeField = (LIFE_FIELD *)pForceField->GetEntity();
				if (pLifeField->IsActive())
				{
					bool bFound = false;
					for (i = 0 ; i < _nNbrForceFields ; ++i)
					{
						if (_ForceFields[i] == pLifeField)
						{
							bFound = true;
						}
					}

					if (bFound == false && _nNbrForceFields < MAX_FORCE_FIELDS-1)
					{
						_ForceFields[_nNbrUsedForceFields] = pLifeField;
						_ForceFields[_nNbrUsedForceFields]->TransformedDatas(pForceField->GetGlobalMatrix());
						++_nNbrUsedForceFields;
					}
				}
			}
			++it;
		}*/
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetUpForces
//	Object:		
//	03-12-16:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetUpForcesSkipPerturbation(NODE * pNode)
{
	_bOnlyUniformForceFields = true;
	_nNbrUsedForceFields = 0;

	int i = 0;
	for (i = 0 ; i < _nNbrForceFields ; ++i)
	{
		if (_nNbrUsedForceFields < MAX_FORCE_FIELDS)
		{
			FORCE_FIELD_ABC * pFF = static_cast<FORCE_FIELD_ABC *>(_vForceFields[i].GetCommunicator());
			if (pFF->IsActive() && 
				pFF->IsPerturbationField() == false)
			{
				_ForceFields[_nNbrUsedForceFields] = pFF;
				_ForceFields[_nNbrUsedForceFields]->TransformedDatas(pNode->GetGlobalMatrix());

				if(_ForceFields[_nNbrUsedForceFields]->IsVortexField() || _ForceFields[_nNbrUsedForceFields]->IsRadialField())
				{
					_bOnlyUniformForceFields = false;
				}

				++_nNbrUsedForceFields;
			}
		}
	}

	if (_bIsInfluencedByAllLifeFields)
	{
		SCENE_GRAPH * pSG = GetSceneGraph();

/*		const QDT_DLIST <NODE *> & lForceFields = pSG->GetForceFields();

		QDT_DLIST <NODE *>::ITERATOR it = lForceFields.GetHead();
		QDT_DLIST <NODE *>::ITERATOR itTail = lForceFields.GetTail();

		while (it != itTail)
		{
			NODE * pForceField = (*it);
			if (pForceField->GetEntityInst().GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIFE_FIELD)
			{
				LIFE_FIELD * pLifeField = (LIFE_FIELD *)pForceField->GetEntity();
				if (pLifeField->IsActive())
				{
					bool bFound = false;
					for (i = 0 ; i < _nNbrForceFields ; ++i)
					{
						if (_ForceFields[i] == pLifeField)
						{
							bFound = true;
						}
					}

					if (bFound == false && _nNbrForceFields < MAX_FORCE_FIELDS-1)
					{
						_ForceFields[_nNbrUsedForceFields] = pLifeField;
						_ForceFields[_nNbrUsedForceFields]->TransformedDatas(pForceField->GetGlobalMatrix());
						++_nNbrUsedForceFields;
					}
				}
			}
			++it;
		}*/
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateParticles
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::UpdateParticles(float	rDeltaFrameTime,
										  NODE * pNode)
{
	QDT_NOT_IMPLEMENTED();
/*
	float			rTime = 0.0f;
	float			rLastDeltaTime = 0.0f;
	float			rFixedDeltaFrameTime = 1.0f / TIME::Instance()->GetTargetFPS();
	unsigned int	i;

	SetUpForces(pNode);

	if	(_bOnlyUniformForceFields == false)
	{
		i = 0;

		while (i < _pParticleArray->GetNbrOfActiveParticles())
		{
			if (_pParticleArray->UpdateLife(i, rDeltaFrameTime))
			{
				// Update particle physics 
				rLastDeltaTime = rDeltaFrameTime;

				rTime = rFixedDeltaFrameTime;
				while	(rTime < rDeltaFrameTime)
				{
					ComputeForces(i, rFixedDeltaFrameTime);
					_pParticleArray->UpdatePositionAndSpinAngle(i, rFixedDeltaFrameTime);
					rLastDeltaTime -= rFixedDeltaFrameTime;
					rTime += rFixedDeltaFrameTime;
				}

				ComputeForces(i, rLastDeltaTime);
				_pParticleArray->UpdatePositionAndSpinAngle(i, rLastDeltaTime);

				++i;
			}
			else
			{
				_pParticleArray->Desactivate(i);
			}
		}
	}
	else
	{
		bool bUpdateForces = false;

		for (i = 0 ; i < _nNbrUsedForceFields ; ++i)
		{
			if (_ForceFields[i]->GetType() != FORCE_FIELD_ABC::TFF_LIFE_FORCE_FIELD)
			{
				bUpdateForces = true;
				break;
			}
		}

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Compute forces", COLOR(255, 180, 88, 45));
		#endif
		#endif

		if (_nNbrUsedForceFields != 0)
		{
			if (_nNbrUsedForceFields == 1 && 
				_ForceFields[0]->GetType() == FORCE_FIELD_ABC::TFF_LIFE_FORCE_FIELD)
			{
				i = 0; 
	
				while (i < _pParticleArray->GetNbrOfActiveParticles())
				{
					if (_pParticleArray->UpdateLife(i, rDeltaFrameTime))
					{
						ComputeForcesOneLifeField(i, rDeltaFrameTime);
						++i;
					}
					else
					{
						_pParticleArray->Desactivate(i);
					}
				}
			}
			else
			{
				i = 0 ; 
	
				while (i < _pParticleArray->GetNbrOfActiveParticles())
				{
					if (_pParticleArray->UpdateLife(i, rDeltaFrameTime))
					{
						ComputeForces(i, rDeltaFrameTime);
						++i;
					}
					else
					{
						_pParticleArray->Desactivate(i);
					}
				}
			}
		}
		else
		{
			i = 0 ; 
	
			while (i < _pParticleArray->GetNbrOfActiveParticles())
			{
				if (_pParticleArray->UpdateLife(i, rDeltaFrameTime))
				{
					ComputeForcesNoForce(i, rDeltaFrameTime);
					++i;
				}
				else
				{
					_pParticleArray->Desactivate(i);
				}
			}
		}

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
		#endif
		#endif
		
		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Update positions and spin angles", COLOR(255, 64, 128, 12));
		#endif
		#endif
		
		_pParticleArray->UpdatePositions(rDeltaFrameTime, bUpdateForces);
		_pParticleArray->UpdateSpinAngles(rDeltaFrameTime);

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
		#endif
		#endif
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		SpawnParticles
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SpawnParticles(float			rDeltaFrameTime,
										 NODE *			pNode)
{
	QDT_NOT_IMPLEMENTED();
/*
	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Spawn particles", COLOR(255, 255, 255, 255));
	#endif
	#endif

	// Spawn the new particles
	VECTOR					StartPosition;
	VECTOR					StartSpeed;
	VECTOR					StartSpinAxis;
	VECTOR					StartSpinAxisSpeed;
	float					rStartSpinAngle;
	float					rStartSpinAngleSpeed;
	unsigned int			nNbrNewParticles;
	float					rDeltaFrame = 1.0f / TIME::Instance()->GetTargetFPS();
	float					rNewDeltaTime, rParticleDeltaTime;
	unsigned int			nEachNewParticle = 0;
	float					rLastDeltaTime, rTime;
	const MATRIX &			GlobalMatrix = pNode->GetGlobalMatrix();
	PARTICLE_SHAPE_SPRITE * pParticleShape = static_cast<PARTICLE_SHAPE_SPRITE *>(_ParticleShapeInst.GetCommunicator());
	float					rLifeSpan = pParticleShape->GetLifeSpan();

	if (rDeltaFrameTime > rLifeSpan)
	{
		rDeltaFrameTime = rLifeSpan;
	}
	
	float rDeltaTime = rDeltaFrameTime;
	_rNewParticlesCounter += _rSpawnRate * rDeltaTime;
	nNbrNewParticles = (int)MATH::Floor(_rNewParticlesCounter);
	_rNewParticlesCounter -= nNbrNewParticles;

	SetUpForcesSkipPerturbation(pNode);
	
	if (nNbrNewParticles > _nNbrParticles)
	{
		nNbrNewParticles = _nNbrParticles;
	}
	
	if	(nNbrNewParticles)
	{
		rNewDeltaTime = rDeltaTime / nNbrNewParticles;
		
		switch	(GetParticleType())
		{
		default:
			break;
			
		case	PARTICLE::PARTICLE_TYPE_SPRITE:
			{				
				for (nEachNewParticle = 0 ; nEachNewParticle < nNbrNewParticles ; ++nEachNewParticle )
				{			
					int nParticleSprite = GetNewParticleSprite();
					
					if(nParticleSprite >= 0)
					{						
						_pParticleArray->Init(nParticleSprite);
						
						GetEmissionPosition(StartPosition);
						GetEmissionSpeed(StartSpeed);
						GetEmissionSpin(rStartSpinAngle, StartSpinAxis);
						GetEmissionSpinSpeed(rStartSpinAngleSpeed, StartSpinAxisSpeed);
						
						GlobalMatrix.TransformPosition(StartPosition);
						GlobalMatrix.TransformDirection(StartSpinAxis);
						GlobalMatrix.TransformDirection(StartSpinAxisSpeed);
						StartSpeed = GlobalMatrix.ApplyRotation(StartSpeed);
						
						_pParticleArray->SetPosition(nParticleSprite, StartPosition);
						_pParticleArray->SetSpeed(nParticleSprite, StartSpeed);
						_pParticleArray->SetSpinAngle(nParticleSprite, rStartSpinAngle);
						_pParticleArray->SetSpinAngleSpeed(rStartSpinAngleSpeed);
						
						if	(pParticleShape->IsAligned())
						{	
							// Apply rotation to the alignement too...
							_pParticleArray->SetAlignmentAxis(nParticleSprite, GlobalMatrix.ApplyRotation(pParticleShape->GetAlignmentAxis1()), GlobalMatrix.ApplyRotation(pParticleShape->GetAlignmentAxis2()));
						}
						
						ComputeForces(nParticleSprite);
						
						rParticleDeltaTime = nEachNewParticle * rNewDeltaTime;
						
						if	(_pParticleArray->UpdateLife(nParticleSprite, rParticleDeltaTime))
						{							
							rLastDeltaTime = rParticleDeltaTime;
							
							if	(_bOnlyUniformForceFields == false)
							{
								rTime = rDeltaFrame;
								while	(rTime < rParticleDeltaTime)
								{
									ComputeForces(nParticleSprite, rDeltaFrame);
									_pParticleArray->UpdatePositionAndSpinAngle(nParticleSprite, rDeltaFrame);
									
									rLastDeltaTime -= rDeltaFrame;
									
									rTime += rDeltaFrame;
								}
							}
							
							ComputeForces(nParticleSprite, rLastDeltaTime);
							_pParticleArray->UpdatePositionAndSpinAngle(nParticleSprite, rLastDeltaTime);
						}
						else
						{
							_pParticleArray->Desactivate(nParticleSprite);
						}
					}
				}
				break;
			}
		}
	}

	#ifndef _MASTER
	#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
	#endif
	#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		SpawnParticle
//	Object:		
//	04-03-16:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SpawnParticle(const MATRIX &	GlobalMatrix)
{
	QDT_NOT_IMPLEMENTED();
/*
	switch	(GetParticleType())
	{
	default:
		break;

	case	PARTICLE::PARTICLE_TYPE_SPRITE:
		{
			
			VECTOR					StartPosition;
			VECTOR					StartSpeed;
			VECTOR					StartSpinAxis;
			VECTOR					StartSpinAxisSpeed;
			float					rStartSpinAngle;
			float					rStartSpinAngleSpeed;
			int						nParticleShape;
			
			nParticleShape = GetNewParticleSprite();
			
			if (nParticleShape >= 0)
			{	
				PARTICLE_SHAPE_SPRITE * pParticleShape = static_cast<PARTICLE_SHAPE_SPRITE *>(_ParticleShapeInst.GetCommunicator());
				
				_pParticleArray->Init(nParticleShape);
			
				GetEmissionPosition(StartPosition);
				GetEmissionSpeed(StartSpeed);
				GetEmissionSpin(rStartSpinAngle, StartSpinAxis);
				GetEmissionSpinSpeed(rStartSpinAngleSpeed, StartSpinAxisSpeed);
				
				GlobalMatrix.TransformPosition(StartPosition);
				GlobalMatrix.TransformDirection(StartSpinAxis);
				GlobalMatrix.TransformDirection(StartSpinAxisSpeed);
				
				_pParticleArray->SetPosition(nParticleShape, StartPosition);
				_pParticleArray->SetSpeed(nParticleShape, StartSpeed);
				_pParticleArray->SetSpinAngle(nParticleShape, rStartSpinAngle);
				_pParticleArray->SetSpinAngleSpeed(rStartSpinAngleSpeed);
				
				if	(pParticleShape->IsAligned())
				{
					// Don't do the GlobalMatrix.ApplyRotation in the case of SpawnParticle (FootSteps), it's precomputed
					_pParticleArray->SetAlignmentAxis(nParticleShape, pParticleShape->GetAlignmentAxis1(), pParticleShape->GetAlignmentAxis2());
				}
			}
			
			break;
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		UpdateParticleEmitter
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::UpdateParticleEmitter(float	rDeltaFrameTime,
												NODE *	pNode)
{
	if (_rSpawnRate <= 0.0f || rDeltaFrameTime == 0.0f)
	{		
		return;
	}

	UpdateParticles(rDeltaFrameTime, pNode);

	if	(_bActive && _bAutoSpawn)
	{
		SpawnParticles(rDeltaFrameTime, pNode);	
	}
}
//-----------------------------------------------------------------------------
//	Name:		GetNewParticleSprite
//	Object:		
//	04-10-27:	RMA - Created
//-----------------------------------------------------------------------------
int PARTICLE_EMITTER::GetNewParticleSprite()
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return (_pParticleArray->GetNewParticleSprite());
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const VECTOR &	Point)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_POINT3D;
	_pPoint = new VECTOR;
	*_pPoint = Point;
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const SEGMENT &	Segment)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_SEGMENT;
	_pSegment = new SEGMENT;
	*_pSegment = Segment;
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const TRIANGLE &	Triangle)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_TRIANGLE;
	_pTriangle = new TRIANGLE;
	*_pTriangle = Triangle;
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const OBB &	Obb)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_OBB;	
	_pObb = new OBB;
	*_pObb = Obb;
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const SPHERE &	Sphere)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_SPHERE;
	_pSphere = new SPHERE;
	*_pSphere = Sphere;
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const CIRCLE &	Circle)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_CIRCLE;
	_pCircle = new CIRCLE;
	*_pCircle = Circle;
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionShape(const DISC &	Disc)
{
	DeleteEmissionShape();
	_nShapeType = PM_SHAPETYPE_DISC;
	_pDisc = new DISC;
	*_pDisc = Disc;
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetParticleType
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE::PARTICLE_TYPE	PARTICLE_EMITTER::GetParticleType() const
{
	if	(_ParticleShapeInst.HasHandle())
	{
		return	(GetParticleShape()->GetParticleType());
	}
	else
	{
		return	(PARTICLE::PARTICLE_TYPE_NONE);
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetEmissionPosition
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::GetEmissionPosition(VECTOR &	StartPosition) const
{
	QDT_ASSERT((_nShapeType == PM_SHAPETYPE_NONE) || (_pShape != NULL));
	
	switch (_nShapeType)
	{
	case PM_SHAPETYPE_NONE:

		StartPosition = VECTOR::ZERO;
		break;

	case PM_SHAPETYPE_POINT3D:

		StartPosition = *(static_cast<VECTOR *>(_pShape));
		break;

	case PM_SHAPETYPE_SEGMENT:

		static_cast<SEGMENT *>(_pShape)->GetRandomPoint(StartPosition);
		break;

	case PM_SHAPETYPE_TRIANGLE:

		static_cast<TRIANGLE *>(_pShape)->GetRandomPoint(StartPosition);
		break;

	case PM_SHAPETYPE_OBB:

		static_cast<OBB *>(_pShape)->GetRandomPoint(StartPosition);
		break;

	case PM_SHAPETYPE_SPHERE:

		static_cast<SPHERE *>(_pShape)->GetRandomPoint(StartPosition);
		break;

	case PM_SHAPETYPE_CIRCLE:

		static_cast<CIRCLE *>(_pShape)->GetRandomPoint(StartPosition);
		break;

	case PM_SHAPETYPE_DISC:

		static_cast<DISC *>(_pShape)->GetRandomPoint(StartPosition);
		break;
		
	default:

		QDT_Error(QDT_COMMENT("ParticleEmitter : bad shape type"));
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpeed
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::GetEmissionSpeed(VECTOR &	StartSpeed) const
{
	if	(_rEmissionSpeed > 0.0f)
	{
		VECTOR	Angle;
		MATRIX	Matrix;

		Angle.SetX(MATH::Rand(_EndAngle.GetX() - _StartAngle.GetX()) + _StartAngle.GetX());
		Angle.SetY(MATH::Rand(_EndAngle.GetY() - _StartAngle.GetY()) + _StartAngle.GetY());
		Angle.SetZ(MATH::Rand(_EndAngle.GetZ() - _StartAngle.GetZ()) + _StartAngle.GetZ());

		Matrix.SetRotXYZ(Angle);
		Matrix.TransformDirection(VECTOR::UNIT_Z, StartSpeed);

		StartSpeed.SetLength(_rEmissionSpeed);
		return;
	}

	StartSpeed.SetXYZ(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpin
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::GetEmissionSpin(float &	rStartSpinAngle,
										  VECTOR &	StartSpinAxis) const
{
	rStartSpinAngle = _rEmissionSpinAngle;
	StartSpinAxis.Copy(_EmissionSpinAxis);
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpinSpeed
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::GetEmissionSpinSpeed(float &	rStartSpinAngleSpeed,
											   VECTOR &	StartSpinAxisSpeed) const
{
	rStartSpinAngleSpeed = _rEmissionSpinAngleSpeed;
	StartSpinAxisSpeed.Copy(_EmissionSpinAxisSpeed);
}

//-----------------------------------------------------------------------------
//	Name:		DeleteEmissionShape
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::DeleteEmissionShape()
{
	if ( ! _pShape )
	{
		return;
	}

	switch ( _nShapeType )
	{
	case PM_SHAPETYPE_POINT3D:

		delete _pPoint;
		break;

	case PM_SHAPETYPE_SEGMENT:

		delete _pSegment;
		break;

	case PM_SHAPETYPE_TRIANGLE:

		delete _pTriangle;
		break;

	case PM_SHAPETYPE_OBB:

		delete _pObb;
		break;

	case PM_SHAPETYPE_SPHERE:

		delete _pSphere;
		break;

	case PM_SHAPETYPE_CIRCLE:

		delete _pCircle;
		break;

	case PM_SHAPETYPE_DISC:

		delete _pDisc;
		break;
		
	default:

		QDT_ASSERT( "Particle Emitter : delete bad shape type" );
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::Copy(const PARTICLE_EMITTER &	ParticleEmitter, bool bCopyShapeInst)
{
	_rNewParticlesCounter		= ParticleEmitter._rNewParticlesCounter;

	_rSpawnRate					= ParticleEmitter._rSpawnRate;

//	_nShapeType					= ParticleEmitter._nShapeType;
//	_pShape						= ParticleEmitter._pShape;

	_StartAngle					= ParticleEmitter._StartAngle;
	_EndAngle					= ParticleEmitter._EndAngle;
	_rEmissionSpeed				= ParticleEmitter._rEmissionSpeed;

	_rEmissionSpinAngle			= ParticleEmitter._rEmissionSpinAngle;
	_EmissionSpinAxis			= ParticleEmitter._EmissionSpinAxis;
	_rEmissionSpinAngleSpeed	= ParticleEmitter._rEmissionSpinAngleSpeed;
	_EmissionSpinAxisSpeed		= ParticleEmitter._EmissionSpinAxisSpeed;

	if (bCopyShapeInst)
	{
	_ParticleShapeInst			= ParticleEmitter._ParticleShapeInst;
	}

	_bActive					= ParticleEmitter._bActive;
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *			pMD;
	ATTRIBUTE_DESCRIPTOR *		pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PARTICLE SHAPE SPRITE", 0, CM_ReadChunk);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PARTICLE SHAPE SPRITE", 1, CM_ReadChunk1);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("PARTICLE SHAPE SPRITE", 1, CM_WriteChunk);	
#endif

	pAD = new	ATTRIBUTE_DESCRIPTOR("ParticleShape", QDT_COMMENT("Shape of the particles emitted."), PARTICLE_SHAPE::GetGenericCommunicatorDescriptor());
	pAD->SetAccessors(EM_GetParticleShape, EM_SetParticleShape);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("spawnRate", QDT_COMMENT("Spawn rate of the particles emitted."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetSpawnRate, EM_SetSpawnRate);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("speed", QDT_COMMENT("Emission speed of the particles."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetEmissionSpeed, EM_SetEmissionSpeed);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("spinAngleSpeed", QDT_COMMENT("Speed of the spin angle."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetEmissionSpinAngleSpeed, EM_SetEmissionSpinAngleSpeed);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("bActive", QDT_COMMENT("Speed of the spin angle."), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_IsActive, EM_SetActive);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("spinAngle", QDT_COMMENT("Spin angle."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetEmissionSpinAngle, EM_SetEmissionSpinAngle);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("emitterScaleX", QDT_COMMENT("Scale on X axis."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetScaleX, EM_SetScaleX);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("emitterScaleY", QDT_COMMENT("Scale on Y axis."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetScaleY, EM_SetScaleY);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("emitterScaleZ", QDT_COMMENT("Scale on Z axis."), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetScaleZ, EM_SetScaleZ);
	pCD->AddAttribute(pAD);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetParticleShape"), QDT_COMMENT("Set the shape of the particle."));
	pMD->AddParameter(QDT_COMMENT("Shape"), QDT_COMMENT("Shape of the emitter."), PARTICLE_SHAPE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PE_SET_PARTICLE_SHAPE, pMD, EM_SetParticleShape);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetParticleShape"), QDT_COMMENT("Get the shape of the particle."));
	pMD->SetReturn(QDT_COMMENT("Shape"), QDT_COMMENT("Shape of the emitter."), PARTICLE_SHAPE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PE_GET_PARTICLE_SHAPE, pMD, EM_GetParticleShape);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSpawnRate"), QDT_COMMENT("Set spawn rate of emitted particles."));
	pMD->AddParameter(QDT_COMMENT("rRate"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_SPAWN_RATE, pMD, EM_SetSpawnRate);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSpawnRate"), QDT_COMMENT("Get spawn rate of emitted particles."));
	pMD->SetReturn(QDT_COMMENT("rRate"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_SPAWN_RATE, pMD, EM_GetSpawnRate);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionStartAngle"), QDT_COMMENT("Set start angle of the emissive volume."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_START_ANGLE, pMD, EM_SetEmissionStartAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionStartAngle"), QDT_COMMENT("Get start angle of the emissive volume."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_START_ANGLE, pMD, EM_GetEmissionStartAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionEndAngle"), QDT_COMMENT("Set end angle of the emissive volume."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_END_ANGLE, pMD, EM_SetEmissionEndAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionEndAngle"), QDT_COMMENT("Get end angle of the emissive volume."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_END_ANGLE, pMD, EM_GetEmissionEndAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionCone"), QDT_COMMENT("Set cone of emissive volume."));
	pMD->AddParameter(QDT_COMMENT("rUserX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUserY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rConeAngle"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_CONE, pMD, EM_SetEmissionCone);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpeed"), QDT_COMMENT("Set emission speed of particles."));
	pMD->AddParameter(QDT_COMMENT("rSpeed"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPEED, pMD, EM_SetEmissionSpeed);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionSpeed"), QDT_COMMENT("Get emission speed of particles."));
	pMD->SetReturn(QDT_COMMENT("rSpeed"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_SPEED, pMD, EM_GetEmissionSpeed);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpinAngle"), QDT_COMMENT("Set emission spin angle of particles."));
	pMD->AddParameter(QDT_COMMENT("rSpinAngle"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPIN_ANGLE, pMD, EM_SetEmissionSpinAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionSpinAngle"), QDT_COMMENT("Get emission spin angle of particles."));
	pMD->SetReturn(QDT_COMMENT("rSpinAngle"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_SPIN_ANGLE, pMD, EM_GetEmissionSpinAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpinAxis"), QDT_COMMENT("Set emission spin angle of particles."));
	pMD->AddParameter(QDT_COMMENT("vAxis"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPIN_AXIS, pMD, EM_SetEmissionSpinAxis);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpinAxis"), QDT_COMMENT("Set emission spin axis of particles."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("X component of axis."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("Y component of axis."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT("Z component of axis."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPIN_AXIS2, pMD, EM_SetEmissionSpinAxis2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionSpinAngle"), QDT_COMMENT("Get emission spin angle of particles."));
	pMD->SetReturn(QDT_COMMENT("vAxis"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_SPIN_AXIS, pMD, EM_GetEmissionSpinAxis);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpinAngleSpeed"), QDT_COMMENT("Set emission spin angle of particles."));
	pMD->AddParameter(QDT_COMMENT("rSpinAngleSpeed"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPIN_ANGLE_SPEED, pMD, EM_SetEmissionSpinAngleSpeed);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionSpinAngleSpeed"), QDT_COMMENT("Get emission spin angle of particles."));
	pMD->SetReturn(QDT_COMMENT("rSpinAngleSpeed"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_SPIN_ANGLE_SPEED, pMD, EM_GetEmissionSpinAngleSpeed);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpinAxisSpeed"), QDT_COMMENT("Set emission spin axis of particles."));
	pMD->AddParameter(QDT_COMMENT("vAxis"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPIN_AXIS_SPEED, pMD, EM_SetEmissionSpinAxisSpeed);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSpinAxisSpeed"), QDT_COMMENT("Set emission spin axis speed of particles."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("X component of axis speed."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("Y component of axis speed."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT("Z component of axis speed."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPIN_AXIS_SPEED2, pMD, EM_SetEmissionSpinAxisSpeed2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetEmissionSpinAngleSpeed"), QDT_COMMENT("Get emission spin axis speed of particles."));
	pMD->SetReturn(QDT_COMMENT("vAxis"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_EMISSION_SPIN_AXIS_SPEED, pMD, EM_GetEmissionSpinAxisSpeed);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionPoint"), QDT_COMMENT("Set point as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("vPos"), QDT_COMMENT("Give the point relative to (0.0, 0.0, 0.0)"), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_POINT, pMD, EM_SetEmissionPoint);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSegment"), QDT_COMMENT("Set a segment as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("vPos1"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("vPos2"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SEGMENT, pMD, EM_SetEmissionSegment);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionTriangle"), QDT_COMMENT("Set a triangle as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("vPos1"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("vPos2"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("vPos3"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_TRIANGLE, pMD, EM_SetEmissionTriangle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionBox"), QDT_COMMENT("Set a box as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("rSizeX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSizeY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSizeZ"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_BOX, pMD, EM_SetEmissionBox);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionSphere"), QDT_COMMENT("Set a sphere as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("rRadius"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_SPHERE, pMD, EM_SetEmissionSphere);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionDisc"), QDT_COMMENT("Set a disc as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("rRadius"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_DISC, pMD, EM_SetEmissionDisc);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetEmissionCircle"), QDT_COMMENT("Set a circle as spawn zone."));
	pMD->AddParameter(QDT_COMMENT("rRadius"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_EMISSION_CIRCLE, pMD, EM_SetEmissionCircle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetActive"), QDT_COMMENT("Set active."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_ACTIVE, pMD, EM_SetActive);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsActive"), QDT_COMMENT("Is active?"));
	pMD->SetReturn(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PE_IS_ACTIVE, pMD, EM_IsActive);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("Copy"), QDT_COMMENT("Copy a particle emitter."));
	pMD->AddParameter(QDT_COMMENT("ParticleEmitter"), QDT_COMMENT("Object of type PARTICLE_EMITTER."), PARTICLE_EMITTER::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PE_COPY, pMD, EM_Copy);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleX"), QDT_COMMENT("Set Scale on X."));
	pMD->AddParameter(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_SCALEX, pMD, EM_SetScaleX);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetScaleX"), QDT_COMMENT("Get Scale on X."));
	pMD->SetReturn(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_SCALEX, pMD, EM_GetScaleX);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleY"), QDT_COMMENT("Set Scale on Y."));
	pMD->AddParameter(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_SCALEY, pMD, EM_SetScaleY);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetScaleY"), QDT_COMMENT("Get Scale on Y."));
	pMD->SetReturn(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_SCALEY, pMD, EM_GetScaleY);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleZ"), QDT_COMMENT("Set Scale on Z."));
	pMD->AddParameter(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_SET_SCALEZ, pMD, EM_SetScaleZ);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetScaleZ"), QDT_COMMENT("Get Scale on Z."));
	pMD->SetReturn(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PE_GET_SCALEZ, pMD, EM_GetScaleZ);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetReceiveAllLifeFields"), QDT_COMMENT("Force the emitter to be influenced by every life fields"));
	pMD->AddParameter(QDT_COMMENT("bStatus"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_RECEIVE_ALL_LIFE_FIELDS, pMD, EM_SetReceiveAllLifeFields);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PARTICLE_EMITTER::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PARTICLE_EMITTER::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PARTICLE_EMITTER"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER,
										QDT_COMMENT("3A7EB88E-3F7A-1763-F9A4-04ECFCF2374B"), 
										sizeof(PARTICLE_EMITTER), 
										ComInit, 
										QDT_COMMENT("Emitter of particles."),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_Construct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	PARTICLE_EMITTER *pPE;
	pPE = static_cast<PARTICLE_EMITTER *>(pO);

	QDT_NEW_PLACED(pPE, PARTICLE_EMITTER)();
	QDT_ASSERT(pPE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_Destruct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	PARTICLE_EMITTER *pPE;
	pPE = static_cast<PARTICLE_EMITTER *>(pO);

	QDT_DELETE_PLACED(pPE, PARTICLE_EMITTER);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetParticleShape
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetParticleShape(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE		 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());

	pComInstance = static_cast<COM_INSTANCE*>(pP);

	pPE->SetParticleShapeInst(*pComInstance);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetParticleShape
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetParticleShape(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE		 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());

	pComInstance = static_cast<COM_INSTANCE*>(pR);

	QDT_NEW_PLACED(pComInstance, COM_INSTANCE)(pPE->GetParticleShapeInst());
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSpawnRate
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetSpawnRate(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pP);

	pPE->SetSpawnRate(*pr);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSpawnRate
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetSpawnRate(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pR);

	*pr = pPE->GetSpawnRate();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionStartAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionStartAngle(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	struct PARAMETERS
	{
		float _rX, _rY, _rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	pPE->SetStartAngle(VECTOR(pParams->_rX, pParams->_rY, pParams->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionStartAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionStartAngle(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	struct PARAMETERS
	{
		float _rX, _rY, _rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	VECTOR vAngle = pPE->GetStartAngle();

	pParams->_rX = vAngle.GetX();
	pParams->_rY = vAngle.GetY();
	pParams->_rZ = vAngle.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionEndAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionEndAngle(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	struct PARAMETERS
	{
		float _rX, _rY, _rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	pPE->SetEndAngle(VECTOR(pParams->_rX, pParams->_rY, pParams->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionEndAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionEndAngle(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	struct PARAMETERS
	{
		float _rX, _rY, _rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	VECTOR vAngle = pPE->GetEndAngle();

	pParams->_rX = vAngle.GetX();
	pParams->_rY = vAngle.GetY();
	pParams->_rZ = vAngle.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionCone
//	Object:		
//	04-12-09:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionCone(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	struct PARAMETERS
	{
		float _rUX, _rUY, _rConeAngle;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);
	VECTOR				angle;

	angle.SetX(pParams->_rUX - pParams->_rConeAngle);
	angle.SetY(pParams->_rUY - pParams->_rConeAngle);
	angle.SetZ(0.0f);
	pPE->SetStartAngle(angle);

	angle.SetX(pParams->_rConeAngle + pParams->_rUX);
	angle.SetY(pParams->_rConeAngle + pParams->_rUY);
	angle.SetZ(0.0f);
	pPE->SetEndAngle(angle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpeed
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpeed(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pP);

	pPE->SetEmissionSpeed(*pr);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionSpeed
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionSpeed(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pR);

	*pr = pPE->GetEmissionSpeed();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpinAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpinAngle(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pP);

	pPE->SetEmissionSpinAngle(*pr);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionSpinAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionSpinAngle(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pR);

	*pr = pPE->GetEmissionSpinAngle();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpinAxis
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpinAxis(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	VECTOR *			pV			 = static_cast<VECTOR*>(pP);

	pPE->SetEmissionSpinAxis(*pV);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpinAxis2
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpinAxis2(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	struct PARAMETERS
	{
		float _rX, _rY, _rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	pPE->SetEmissionSpinAxis(VECTOR(pParams->_rX, pParams->_rY, pParams->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionSpinAxis
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionSpinAxis(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	VECTOR *			pV			 = static_cast<VECTOR*>(pR);

	QDT_NEW_PLACED(pV, VECTOR)(pPE->GetEmissionSpinAxis());
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpinAngleSpeed
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpinAngleSpeed(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pP);

	pPE->SetEmissionSpinAngleSpeed(*pr);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionSpinAngleSpeed
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionSpinAngleSpeed(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pr			 = static_cast<float*>(pR);

	*pr = pPE->GetEmissionSpinAngleSpeed();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpinAxisSpeed
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpinAxisSpeed(void * const	pO,
													  void * const	pR,
													  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	VECTOR *			pV			 = static_cast<VECTOR*>(pP);

	pPE->SetEmissionSpinAxisSpeed(*pV);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSpinAxisSpeed2
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSpinAxisSpeed2(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	struct PARAMETERS
	{
		float _rX, _rY, _rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	pPE->SetEmissionSpinAxisSpeed(VECTOR(pParams->_rX, pParams->_rY, pParams->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetEmissionSpinAxisSpeed
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetEmissionSpinAxisSpeed(void * const	pO,
													  void * const	pR,
													  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	VECTOR *			pV			 = static_cast<VECTOR*>(pR);

	QDT_NEW_PLACED(pV, VECTOR)(pPE->GetEmissionSpinAxisSpeed());
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionPoint
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionPoint(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	VECTOR *			pPoint		 = static_cast<VECTOR*>(pP);

	pPE->SetEmissionShape(VECTOR(*pPoint, VECTOR::VM_OBJECT));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSegment
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSegment(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	struct PARAMETERS
	{
		VECTOR	_vPos1, _vPos2;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	pPE->SetEmissionShape(SEGMENT(pParams->_vPos1, pParams->_vPos2));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionTriangle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionTriangle(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	struct PARAMETERS
	{
		VECTOR	_vPos1, _vPos2, _vPos3;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);

	pPE->SetEmissionShape(TRIANGLE(pParams->_vPos1, pParams->_vPos2, pParams->_vPos3));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionBox
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionBox(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	struct PARAMETERS
	{
		float	_rSizeX, _rSizeY, _rSizeZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParams		 = static_cast<PARAMETERS*>(pP);
	
	OBB		Obb;
	
	Obb.SetCenter(VECTOR::ZERO);
	Obb.SetDirection1(VECTOR::UNIT_X);
	Obb.SetDirection2(VECTOR::UNIT_Y);
	Obb.SetDirection3(VECTOR::UNIT_Z);
	Obb.SetDistance2(pParams->_rSizeX * 0.5f);
	Obb.SetDistance1(pParams->_rSizeY * 0.5f);
	Obb.SetDistance3(pParams->_rSizeZ * 0.5f);
		
	pPE->SetEmissionShape(Obb);	
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionSphere
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionSphere(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				prRadius	 = static_cast<float*>(pP);

	pPE->SetEmissionShape(SPHERE(VECTOR::ZERO, *prRadius));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionDisc
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionDisc(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				prRadius	 = static_cast<float*>(pP);

	DISC	Disc;
	VECTOR	Center(VECTOR::ZERO);
	VECTOR	Axis(VECTOR::UNIT_Y);

	Disc.Build(Center, *prRadius, Axis);
	
	pPE->SetEmissionShape(Disc);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetEmissionCircle
//	Object:		
//	03-09-11:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetEmissionCircle(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				prRadius	 = static_cast<float*>(pP);

	CIRCLE			Circle;
	const POINT2D	Center(0.0f, 0.0f);

	Circle.SetCenter(Center);
	Circle.SetRadius(*prRadius);
	
	pPE->SetEmissionShape(Circle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActive
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetActive(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	bool *				pbValue		 = static_cast<bool*>(pP);

	pPE->SetActive(*pbValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsActive
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_IsActive(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	bool *				pbValue		 = static_cast<bool*>(pP);

	*pbValue = pPE->IsActive();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Copy
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_Copy(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	pComInstance					 = static_cast<COM_INSTANCE*>(pP);

	pPE->Copy(*static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleX
//	Object:		
//	04-04-30:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetScaleX(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pbValue		 = static_cast<float*>(pP);

	switch ( pPE->_nShapeType )
	{
	case PM_SHAPETYPE_POINT3D:
		break;

	case PM_SHAPETYPE_SEGMENT:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_TRIANGLE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_OBB:
		pPE->_pObb->SetDistance1(*pbValue);
		break;

	case PM_SHAPETYPE_SPHERE:
		pPE->_pSphere->SetRadius(*pbValue);
		break;

	case PM_SHAPETYPE_CIRCLE:
		pPE->_pCircle->SetRadius(*pbValue);
		break;

	case PM_SHAPETYPE_DISC:
		pPE->_pDisc->SetRadius(*pbValue);
		break;
		
	default:
		QDT_FAIL();
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetScaleX
//	Object:		
//	04-04-30:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetScaleX(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleY
//	Object:		
//	04-04-30:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetScaleY(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pbValue		 = static_cast<float*>(pP);

	switch ( pPE->_nShapeType )
	{
	case PM_SHAPETYPE_POINT3D:
		break;

	case PM_SHAPETYPE_SEGMENT:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_TRIANGLE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_OBB:
		pPE->_pObb->SetDistance2(*pbValue);
		break;

	case PM_SHAPETYPE_SPHERE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_CIRCLE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_DISC:
		//Do Nothing, What to do?
		break;
		
	default:
		QDT_FAIL();
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetScaleY
//	Object:		
//	04-04-30:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetScaleY(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleZ
//	Object:		
//	04-04-30:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetScaleZ(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	float *				pbValue		 = static_cast<float*>(pP);

	switch ( pPE->_nShapeType )
	{
	case PM_SHAPETYPE_POINT3D:
		break;

	case PM_SHAPETYPE_SEGMENT:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_TRIANGLE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_OBB:
		pPE->_pObb->SetDistance3(*pbValue);
		break;

	case PM_SHAPETYPE_SPHERE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_CIRCLE:
		//Do Nothing, What to do?
		break;

	case PM_SHAPETYPE_DISC:
		//Do Nothing, What to do?
		break;
		
	default:
		QDT_FAIL();
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetScaleZ
//	Object:		
//	04-04-30:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_GetScaleZ(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetReceiveAllLifeFields
//	Object:		
//	04-10-26:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::EM_SetReceiveAllLifeFields(void * const pO, 
													 void * const pR, 
													 void * const pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_EMITTER *	pPE			 = static_cast<PARTICLE_EMITTER*>(pComInstance->GetCommunicator());
	bool *				pbValue		 = static_cast<bool*>(pP);

	pPE->_bIsInfluencedByAllLifeFields = *pbValue;
}

#ifndef _MASTER_ONLY_DATABANK

// Chunk
//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-09-11:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::CM_ReadChunk(IO_STREAM &		Stream,
									   COMMUNICATOR *	pCommunicator)
{
	PARTICLE_EMITTER	*pEmitter;
 	pEmitter = static_cast<PARTICLE_EMITTER *>(pCommunicator);

	Stream	>> pEmitter->_rNewParticlesCounter
			>> pEmitter->_rSpawnRate
			>> pEmitter->_nShapeType;

	VECTOR tmp;
	DISC tmpDisc;
	SEGMENT tmpSeg;
	TRIANGLE tmpTri;
	OBB tmpBox;
	SPHERE tmpSphere;
	CIRCLE tmpCircle;

	switch(pEmitter->_nShapeType)
		{
			default:
			case PM_SHAPETYPE_NONE:		
				QDT_FAIL();
				break;

			case PM_SHAPETYPE_POINT3D:
				Stream  >> tmp;	
				pEmitter->_pPoint = new VECTOR;
				*(pEmitter->_pPoint) = tmp;
				break;

			case PM_SHAPETYPE_SEGMENT:
				Stream  >> tmpSeg;	
				pEmitter->_pSegment = new SEGMENT;
				*(pEmitter->_pSegment) = tmpSeg;
				break;						

			case PM_SHAPETYPE_TRIANGLE:
				Stream  >> tmpTri;	
				pEmitter->_pTriangle = new TRIANGLE;
				*(pEmitter->_pTriangle) = tmpTri;
				break;

			case PM_SHAPETYPE_OBB:
				Stream  >> tmpBox;	
				pEmitter->_pObb = new OBB;
				*(pEmitter->_pObb) = tmpBox;
				break;

			case PM_SHAPETYPE_SPHERE:
				Stream  >> tmpSphere;	
				pEmitter->_pSphere = new SPHERE;
				*(pEmitter->_pSphere) = tmpSphere;
				break;

			case PM_SHAPETYPE_CIRCLE:
				Stream  >> tmpCircle;	
				pEmitter->_pCircle = new CIRCLE;
				*(pEmitter->_pCircle) = tmpCircle;
				break;

			case PM_SHAPETYPE_DISC:
				Stream  >> tmpDisc;	
				pEmitter->_pDisc = new DISC;
				*(pEmitter->_pDisc) = tmpDisc;
				break;
		}

	bool bDummy;

	Stream	>> pEmitter->_StartAngle
			>> pEmitter->_EndAngle
			>> pEmitter->_rEmissionSpeed
			>> pEmitter->_rEmissionSpinAngle
			>> pEmitter->_EmissionSpinAxis
			>> pEmitter->_rEmissionSpinAngleSpeed
			>> pEmitter->_EmissionSpinAxisSpeed
			>> pEmitter->_ParticleShapeInst
			>> pEmitter->_bActive
			>> bDummy
			>> pEmitter->_BoundingBox
			>> pEmitter->_nNbrForceFields;

	//pEmitter->_pForceFields = new COM_INSTANCE[pEmitter->_nNbrForceFields];
	pEmitter->_vForceFields.Clear();
	pEmitter->_vForceFields.Reserve(pEmitter->_nNbrForceFields);

	int i;
	COM_INSTANCE tmpInst;
	for(i = 0; i<pEmitter->_nNbrForceFields; ++i)
	{
		Stream >> tmpInst;
		pEmitter->_vForceFields.PushTail(tmpInst);
	}
	
	pEmitter->AllocateParticles();
	pEmitter->CreateSpriteTable();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	04-03-10:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::CM_ReadChunk1(IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	PARTICLE_EMITTER	*pEmitter;
 	pEmitter = static_cast<PARTICLE_EMITTER *>(pCommunicator);

	Stream	>> pEmitter->_rNewParticlesCounter
			>> pEmitter->_rSpawnRate
			>> pEmitter->_nShapeType;

	VECTOR tmp;
	DISC tmpDisc;
	SEGMENT tmpSeg;
	TRIANGLE tmpTri;
	OBB tmpBox;
	SPHERE tmpSphere;
	CIRCLE tmpCircle;

	switch(pEmitter->_nShapeType)
		{
			default:
			case PM_SHAPETYPE_NONE:		
				QDT_FAIL();
				break;

			case PM_SHAPETYPE_POINT3D:
				Stream  >> tmp;	
				pEmitter->_pPoint = new VECTOR;
				*(pEmitter->_pPoint) = tmp;
				break;

			case PM_SHAPETYPE_SEGMENT:
				Stream  >> tmpSeg;	
				pEmitter->_pSegment = new SEGMENT;
				*(pEmitter->_pSegment) = tmpSeg;
				break;						

			case PM_SHAPETYPE_TRIANGLE:
				Stream  >> tmpTri;	
				pEmitter->_pTriangle = new TRIANGLE;
				*(pEmitter->_pTriangle) = tmpTri;
				break;

			case PM_SHAPETYPE_OBB:
				Stream  >> tmpBox;	
				pEmitter->_pObb = new OBB;
				*(pEmitter->_pObb) = tmpBox;
				break;

			case PM_SHAPETYPE_SPHERE:
				Stream  >> tmpSphere;	
				pEmitter->_pSphere = new SPHERE;
				*(pEmitter->_pSphere) = tmpSphere;
				break;

			case PM_SHAPETYPE_CIRCLE:
				Stream  >> tmpCircle;	
				pEmitter->_pCircle = new CIRCLE;
				*(pEmitter->_pCircle) = tmpCircle;
				break;

			case PM_SHAPETYPE_DISC:
				Stream  >> tmpDisc;	
				pEmitter->_pDisc = new DISC;
				*(pEmitter->_pDisc) = tmpDisc;
				break;
		}

	bool bDummy;

	Stream	>> pEmitter->_StartAngle
			>> pEmitter->_EndAngle
			>> pEmitter->_rEmissionSpeed
			>> pEmitter->_rEmissionSpinAngle
			>> pEmitter->_EmissionSpinAxis
			>> pEmitter->_rEmissionSpinAngleSpeed
			>> pEmitter->_EmissionSpinAxisSpeed
			>> pEmitter->_ParticleShapeInst
			>> pEmitter->_bActive
			>> bDummy
			>> pEmitter->_BoundingBox
			>> pEmitter->_rVisibilityDistance
			>> pEmitter->_nNbrForceFields;

	//pEmitter->_pForceFields = new COM_INSTANCE[pEmitter->_nNbrForceFields];
	pEmitter->_vForceFields.Clear();
	pEmitter->_vForceFields.Reserve(pEmitter->_nNbrForceFields);

	int i;
	COM_INSTANCE tmpInst;
	for(i = 0; i<pEmitter->_nNbrForceFields; ++i)
	{
		Stream >> tmpInst;
		pEmitter->_vForceFields.PushTail(tmpInst);
	}
	
	pEmitter->AllocateParticles();
	pEmitter->CreateSpriteTable();
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-09-11:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	PARTICLE_EMITTER::CM_WriteChunk(IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	PARTICLE_EMITTER	*pEmitter;
	pEmitter = static_cast<PARTICLE_EMITTER *>(pCommunicator);

	Stream.DebugOutput("Particle Emitter :\t");
	Stream.DebugOutput(pEmitter->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Particles Counter :\t");
	Stream << pEmitter->_rNewParticlesCounter;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Spawn Rate :\t");
	Stream << pEmitter->_rSpawnRate;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emitter shape type :\t");
	Stream << pEmitter->_nShapeType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emitter shape :\t");
	switch(pEmitter->_nShapeType)
	{
		default:
		case PM_SHAPETYPE_NONE:		
			QDT_FAIL();
			break;

		case PM_SHAPETYPE_POINT3D:
			Stream <<  *(pEmitter->_pPoint);
			break;

		case PM_SHAPETYPE_SEGMENT:
			Stream <<  *(pEmitter->_pSegment);
			break;

		case PM_SHAPETYPE_TRIANGLE:
			Stream <<  *(pEmitter->_pTriangle);
			break;

		case PM_SHAPETYPE_OBB:
			Stream <<  *(pEmitter->_pObb);
			break;

		case PM_SHAPETYPE_SPHERE:
			Stream <<  *(pEmitter->_pSphere);
			break;

		case PM_SHAPETYPE_CIRCLE:
			Stream <<  *(pEmitter->_pCircle);
			break;

		case PM_SHAPETYPE_DISC:
			Stream <<  *(pEmitter->_pDisc);
			break;
	}

	Stream.DebugOutput("\r\n");


	Stream.DebugOutput("Start angle :\t");
	Stream << pEmitter->_StartAngle;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("End angle :\t");
	Stream << pEmitter->_EndAngle;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emission speed :\t");
	Stream << pEmitter->_rEmissionSpeed;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emission spin angle :\t");
	Stream << pEmitter->_rEmissionSpinAngle;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emission spin axis :\t");
	Stream << pEmitter->_EmissionSpinAxis;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emission spin angle speed :\t");
	Stream << pEmitter->_rEmissionSpinAngleSpeed;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Emission spin axis speed:\t");
	Stream << pEmitter->_EmissionSpinAxisSpeed;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Particle shape instance :\t");
	Stream << pEmitter->_ParticleShapeInst;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Activate :\t");
	Stream << pEmitter->_bActive;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Dummy :\t");
	Stream << true;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Bounding volume:\t");
	Stream << pEmitter->_BoundingBox;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Distance of visibility:\t");
	Stream << pEmitter->_rVisibilityDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Nb of force fields :\t");
	Stream << pEmitter->_nNbrForceFields;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Force fields instances :\t");
	
	int i;
	for( i=0; i<pEmitter->_nNbrForceFields; ++i)
	{
		Stream << pEmitter->_vForceFields[i];
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CreateSpriteTable
//	Object:		
//	04-04-28:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::CreateSpriteTable()
{
	QDT_NOT_IMPLEMENTED();
/*
	PARTICLE_SHAPE * pShape = static_cast<PARTICLE_SHAPE *>(_ParticleShapeInst.GetCommunicator());
	PARTICLE_SHAPE_SPRITE_DATA * pShapeData = static_cast<PARTICLE_SHAPE_SPRITE_DATA*>(pShape->GetShapeData());
	SHADER_ABC * pShader = pShapeData->GetShader();

	bool bSpin = (_rEmissionSpinAngle != 0.0f) || (_rEmissionSpinAngleSpeed != 0.0f);

	_pSpriteTable = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateSpriteTable();
	_pSpriteTable->SetParticleShapeSprite(pShapeData);

	_pSpriteTable->SetParticleArray(_pParticleArray);

	_pSpriteTable->SetNbrAllocatedParticles(_nNbrParticles);
	_pSpriteTable->AllocateBuffer(bSpin);
	_pSpriteTable->SetShader(pShader);*/
}

//-----------------------------------------------------------------------------
//	Name:		COM_PostLoad
//	Object:		
//	04-04-28:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::COM_PostLoad()
{
#ifndef	_MASTER		//	MASTER mode : databank mode always true
	if ( GetHandle()->IsLoadedBinary() == false )
	{
		UpdateParticleShapeSprite();
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		UpdateParticleShapeSprite
//	Object:		
//	04-04-30:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::UpdateParticleShapeSprite()
{
	QDT_NOT_IMPLEMENTED();
/*
	PARTICLE_SHAPE * pShape = static_cast<PARTICLE_SHAPE *>(_ParticleShapeInst.GetCommunicator());
	PARTICLE_SHAPE_SPRITE_DATA * pShapeData = static_cast<PARTICLE_SHAPE_SPRITE_DATA*>(pShape->GetShapeData());
	_pParticleArray->SetShapeData(pShapeData);*/
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
