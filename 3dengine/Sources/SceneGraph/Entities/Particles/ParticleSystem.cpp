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
//	CLASS:	PARTICLE_SYSTEM
//
//	01-11-20:	ELE - Created
//*****************************************************************************


#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleSystem)

#ifdef _DEBUG
	#include	"ParticleSystem.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_SYSTEM constructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM::PARTICLE_SYSTEM()
//:
//_lpInactiveParticlesSprite	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~PARTICLE_SYSTEM destructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM::~PARTICLE_SYSTEM()
{
	ResetForceFields();

	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		UpdateParticleSystem
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::UpdateParticleSystem(float	rDeltaFrameTime)
{
	QDT_NOT_IMPLEMENTED();
/*
	int					i;
	FORCE_FIELD_ABC		*pFF;
	NODE				*pFFNode;
	PARTICLE			*pParticle, *pNextParticle;
	float				rTime, rLastDeltaTime, rFixedDeltaFrameTime = 1.0f / TIME::Instance()->GetTargetFPS();

	// Update of the force fields
	int	nNbrForceFields = _vForceFieldsNodes.GetSize();
	for	(i = 0 ; i < nNbrForceFields ; ++i)
	{
		pFFNode = static_cast <NODE *> (_vForceFieldsNodes[i].GetCommunicator());
		pFF = static_cast<FORCE_FIELD_ABC *>(pFFNode->GetEntity());

		pFF->TransformedDatas(pFFNode->GetGlobalMatrix());
	}

	// Update of the particles
	if	(_dlActiveSpriteTables.GetSize() > 0)
	{
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	It = _dlActiveSpriteTables.GetHead();
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	ItNext, ItEnd = _dlActiveSpriteTables.GetTail();

		while	(It != ItEnd)
		{
			SPRITE_TABLE_ABC	*pSpriteTable = *It;
			ItNext = It + 1;
			
			pParticle = pSpriteTable->GetParticles();

			while	(pParticle != NULL)
			{
				// Update particle life
				pParticle->UpdateLife(rDeltaFrameTime);

				if	(pParticle->IsActive())
				{
					// Update particle physics 
					rLastDeltaTime = rDeltaFrameTime;

					if	(pParticle->OnlyUniformField() == false)
					{
						rTime = rFixedDeltaFrameTime;
						while	(rTime < rDeltaFrameTime)
						{
							// Update speed with forces calculations
							ComputeForces(pParticle, rFixedDeltaFrameTime);

							pParticle->UpdatePositionAndSpinAngle(rFixedDeltaFrameTime);

							rLastDeltaTime -= rFixedDeltaFrameTime; 

							rTime += rFixedDeltaFrameTime; 
						}
					}

					ComputeForces(pParticle, rLastDeltaTime);

					pParticle->UpdatePositionAndSpinAngle(rLastDeltaTime);

					// Next particle
					pParticle = pParticle->GetNext();
				}
				else
				{
					pNextParticle = pParticle->GetNext();

					// Remove from active particles list
					pSpriteTable->RemoveParticle(static_cast<PARTICLE_SPRITE *>(pParticle));
					if	(pSpriteTable->GetNbrParticles() == 0)
					{
						_dlActiveSpriteTables.Remove(It);
						_dlInactiveSpriteTables.PushTail(pSpriteTable);
					}

					AddInactiveParticleSprite(static_cast<PARTICLE_SPRITE *>(pParticle));

					// Next particle
					pParticle = pNextParticle;
				}
			}

			It = ItNext;
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::Display()
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT				*pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_CONTEXT::CULL_MODE	RMCullMode = pRenderContext->GetCullMode();
	SPRITE_TABLE_ABC			*pSpriteTable;

	if	(_dlGlowSpriteTables.GetSize() > 0)
	{
		pRenderContext->EnableLighting(false);
		pRenderContext->EnableDepthWrite(false);
		pRenderContext->EnableAlphaTest(false);
		pRenderContext->EnableClipping(false);
		pRenderContext->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);
		pRenderContext->SetGlobalAmbientLight(COLOR_FLOAT(1.0f, 0.0f, 0.0f, 0.0f));

		pRenderContext->SetViewTransform(NULL);
 		pRenderContext->SetWorldTransform0(NULL);
		pRenderContext->EnableVertexBlending(3, false);

		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	It = _dlGlowSpriteTables.GetHead();
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	ItNext, ItEnd = _dlGlowSpriteTables.GetTail();

		while	(It != ItEnd)
		{
			pSpriteTable = *It;
			ItNext = It + 1;

			if	(pSpriteTable->IsAlignedGlow())
			{
				pSpriteTable->DisplayAsAlignedGlow();
			}
			else
			{
				pSpriteTable->DisplayAsGlow();
			}

			PARTICLE	*pParticles = pSpriteTable->GetParticles();
			PARTICLE	*pNextParticles;
			while	(pParticles != NULL)
			{
				pNextParticles = pParticles->GetNext();
				AddInactiveParticleSprite(static_cast<PARTICLE_SPRITE *>(pParticles));

				pParticles = pNextParticles;
			}

			pSpriteTable->Reset();

			_dlGlowSpriteTables.Remove(It);
			_dlInactiveSpriteTables.PushTail(pSpriteTable);

			It = ItNext;
		}

		MATRIX	ViewMatrix;
		NODE	*pCameraNode = DISPLAY_MANAGER::Instance()->GetCameraNode();

		ViewMatrix.InverseAffine(pCameraNode->GetGlobalMatrix());
		pRenderContext->SetViewTransform(&ViewMatrix);

		pRenderContext->EnableDepthWrite(true);
		pRenderContext->SetCullMode(RMCullMode);		
	}*/	
}

//-----------------------------------------------------------------------------
//	Name:		RemoveForceField
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::RemoveForceField(const COM_INSTANCE &	ForceFieldInst)
{
	int	i;
	int	nNbrForceFields = _vForceFieldsNodes.GetSize();

	for	(i = 0 ; i < nNbrForceFields ; ++i)
	{
		if	(_vForceFieldsNodes[i] == ForceFieldInst)
		{
			QDT_VECTOR <COM_INSTANCE>::ITERATOR	It = _vForceFieldsNodes.GetHead();
			It += i;
			_vForceFieldsNodes.Remove(It);
		}
	}
}

/*
//-----------------------------------------------------------------------------
//	Name:		ComputeForces
//	Object:		
//	03-09-10:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::ComputeForces(PARTICLE *	pParticle,
									   float		rDeltaTime)
{
	float	rDegenerationLifeFactor = 1.0f;
	bool	bOnlyUniformField = false;
	VECTOR	FinalForce;

	if	(pParticle->NeedComputeForces())
	{
		int				i, nNbrParticleSystemForceFields;
		COM_INSTANCE	FFNodeInst;
		NODE			*pFFNode;
		FORCE_FIELD_ABC	*pFF;
		VECTOR			Force;

		FinalForce.SetXYZ(0.0f, 0.0f, 0.0f);
		rDegenerationLifeFactor = 1.0f;

		bOnlyUniformField = true;

		nNbrParticleSystemForceFields = _vForceFieldsNodes.GetSize();
		for	(i = 0 ; i < nNbrParticleSystemForceFields ; ++i)
		{
			FFNodeInst = _vForceFieldsNodes[i];
			pFFNode = static_cast<NODE *>(FFNodeInst.GetCommunicator());
			pFF = static_cast<FORCE_FIELD_ABC *>(pFFNode->GetEntity());

			pFF->TransformedDatas(pFFNode->GetGlobalMatrix());
			if	(pFF->TestInfluenceVolume(*pParticle))
			{
				if	(pFF->IsLifeField())
				{
					rDegenerationLifeFactor *= pFF->GetMagnitude();
				}
				else
				{
					pFF->GetForceToApply(*pParticle, Force);
					FinalForce += Force;

					if	((pFF->IsVortexField()) || (pFF->IsRadialField())  || (pFF->IsPerturbationField()))
					{
						bOnlyUniformField = false;
					}
				}
			}
		}

		VECTOR Acceleration = FinalForce * pParticle->GetOneOnMass();

		pParticle->UpdateSpeed(Acceleration);

		pParticle->SetDegenerationLifeFactor(rDegenerationLifeFactor);
		pParticle->SetOnlyUniformField(bOnlyUniformField);

		pParticle->ResetComputeForcesTime();
	}
	else
	{
		pParticle->UpdateComputeForcesTime(rDeltaTime);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetParticleSprite
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SPRITE *	PARTICLE_SYSTEM::GetParticleSprite()
{
	PARTICLE_SPRITE *	pParticleSprite;

	if	(_lpInactiveParticlesSprite != NULL)
	{
		pParticleSprite = _lpInactiveParticlesSprite;
		_lpInactiveParticlesSprite = static_cast<PARTICLE_SPRITE *>(_lpInactiveParticlesSprite->GetNext());
		if	(_lpInactiveParticlesSprite)
		{
			_lpInactiveParticlesSprite->SetPrevious(NULL);
		}
	}
	else
	{
		pParticleSprite = new PARTICLE_SPRITE;
		_vParticles.PushTail(pParticleSprite);
	}

	return	(pParticleSprite);
}
*/
//-----------------------------------------------------------------------------
//	Name:		RemoveLinks
//	Object:		
//	02-02-08:	DAC - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::RemoveLinks()
{
	_vForceFieldsNodes.Clear();
}
/*
//-----------------------------------------------------------------------------
//	Name:		AddActiveParticle
//	Object:		
//	02-01-31:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::AddActiveParticle(PARTICLE_SPRITE *	pParticle)
{
	if	(_dlActiveSpriteTables.GetSize() > 0)
	{
		bool										bFound = false;
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	It = _dlActiveSpriteTables.GetHead();
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	ItEnd = _dlActiveSpriteTables.GetTail();

		while	(It != ItEnd)
		{
			if	((*It)->GetShader() == pParticle->GetShader())
			{
				(*It)->AddParticle(pParticle);
				bFound = true;
				break;
			}

			++It;
		}

		if	(!bFound)
		{
			if	(_dlInactiveSpriteTables.GetSize() > 0)
			{
				SPRITE_TABLE_ABC	*pSpriteTable = *_dlInactiveSpriteTables.GetHead();

				pSpriteTable->SetShader(pParticle->GetShader());
				pSpriteTable->AddParticle(pParticle);

				_dlInactiveSpriteTables.RemoveHead();
				_dlActiveSpriteTables.PushTail(pSpriteTable);
			}
			else
			{
				SPRITE_TABLE_ABC	*pSpriteTable = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateSpriteTable();

				pSpriteTable->SetShader(pParticle->GetShader());
				pSpriteTable->AddParticle(pParticle);
		
				_dlActiveSpriteTables.PushTail(pSpriteTable);
			}
		}
	}
	else
	{
		if	(_dlInactiveSpriteTables.GetSize() > 0)
		{
			SPRITE_TABLE_ABC	*pSpriteTable = *_dlInactiveSpriteTables.GetHead();

			pSpriteTable->SetShader(pParticle->GetShader());
			pSpriteTable->AddParticle(pParticle);

			_dlInactiveSpriteTables.RemoveHead();
			_dlActiveSpriteTables.PushTail(pSpriteTable);
		}
		else
		{
			SPRITE_TABLE_ABC	*pSpriteTable = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateSpriteTable();

			pSpriteTable->SetShader(pParticle->GetShader());
			pSpriteTable->AddParticle(pParticle);
	
			_dlActiveSpriteTables.PushTail(pSpriteTable);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddSprite
//	Object:		
//	04-09-01:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::AddSprite(VECTOR*			pPositions,
								   POINT2D *		pUVs,
								   const COLOR &	Color,
								   SHADER_ABC *		pShader,
								   bool				bZTest)
{
	PARTICLE_SPRITE	*pParticle = GetParticleSprite();

	pParticle->SetSpritePosition(pPositions);
	pParticle->SetGlowUVs(pUVs);
	pParticle->SetDiffuseColor(Color);
	pParticle->SetZTest(bZTest);

	if	(_dlGlowSpriteTables.GetSize() > 0)
	{
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	It = _dlGlowSpriteTables.GetHead();
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	ItEnd = _dlGlowSpriteTables.GetTail();

		while	(It != ItEnd)
		{
			if	( ((*It)->GetShader() == pShader) && ((*It)->IsAlignedGlow() == true) )
			{
				(*It)->AddParticle(pParticle);
				return;
			}

			++It;
		}
	}

	if	(_dlInactiveSpriteTables.GetSize() > 0)
	{
		SPRITE_TABLE_ABC	*pSpriteTable = *_dlInactiveSpriteTables.GetHead();

		pSpriteTable->SetAlignedGlow(true);
		pSpriteTable->SetShader(pShader);
		pSpriteTable->AddParticle(pParticle);

		_dlInactiveSpriteTables.RemoveHead();
		_dlGlowSpriteTables.PushTail(pSpriteTable);
	}
	else
	{
		SPRITE_TABLE_ABC	*pSpriteTable = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateSpriteTable();

		pSpriteTable->SetAlignedGlow(true);
		pSpriteTable->SetShader(pShader);
		pSpriteTable->AddParticle(pParticle);

		_dlGlowSpriteTables.PushTail(pSpriteTable);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddGlow
//	Object:		
//	03-09-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::AddGlow(const VECTOR &		Position,
								 const VECTOR &		Size,
								 POINT2D *			pUVs,
								 float				rRotation,
								 const COLOR &		Color,
								 SHADER_ABC *		pShader,
								 bool				bZTest)
{
	PARTICLE_SPRITE	*pParticle = GetParticleSprite();
	COLOR C(Color);

#ifdef PLATFORM_PS2
	if (pShader->GetType() == SHT_PRELIT)
	{
		COLOR_FLOAT ColorFloat(C);
		ColorFloat *= ((PRELIT_SHADER *)pShader)->GetDiffuse();
		C = ColorFloat;
	}
#endif

	pParticle->ReSetSpritePosition();
	pParticle->SetPosition(Position);	
	pParticle->SetSizeX(Size.GetX());
	pParticle->SetSizeY(Size.GetY());
	pParticle->SetGlowUVs(pUVs);
	pParticle->SetSpinAngle(rRotation);
	pParticle->SetDiffuseColor(C);
	pParticle->SetZTest(bZTest);

	if	(_dlGlowSpriteTables.GetSize() > 0)
	{
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	It = _dlGlowSpriteTables.GetHead();
		QDT_DLIST <SPRITE_TABLE_ABC *>::ITERATOR	ItEnd = _dlGlowSpriteTables.GetTail();

		while	(It != ItEnd)
		{
			if	( ((*It)->GetShader() == pShader) && ((*It)->IsAlignedGlow() == false) )
			{
				(*It)->AddParticle(pParticle);
				return;
			}

			++It;
		}
	}

	if	(_dlInactiveSpriteTables.GetSize() > 0)
	{
		SPRITE_TABLE_ABC	*pSpriteTable = *_dlInactiveSpriteTables.GetHead();

		pSpriteTable->SetAlignedGlow(false);
        pSpriteTable->SetShader(pShader);
		pSpriteTable->AddParticle(pParticle);

		_dlInactiveSpriteTables.RemoveHead();
		_dlGlowSpriteTables.PushTail(pSpriteTable);
	}
	else
	{
		SPRITE_TABLE_ABC	*pSpriteTable = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateSpriteTable();

		pSpriteTable->SetAlignedGlow(false);
		pSpriteTable->SetShader(pShader);
		pSpriteTable->AddParticle(pParticle);

		_dlGlowSpriteTables.PushTail(pSpriteTable);
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetNbrForceFields
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
int	PARTICLE_SYSTEM::GetNbrForceFields() const
{
	return	(_vForceFieldsNodes.GetSize());
}
/*
//-----------------------------------------------------------------------------
//	Name:		AddInactiveParticleSprite
//	Object:		
//	02-01-31:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::AddInactiveParticleSprite(PARTICLE_SPRITE *	pParticle)
{
	pParticle->SetNext(_lpInactiveParticlesSprite);
	if	(_lpInactiveParticlesSprite)
	{
		_lpInactiveParticlesSprite->SetPrevious(pParticle);
	}

	_lpInactiveParticlesSprite = pParticle;
}
*/
//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-01-29:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::Clear()
{
//	_vParticles.ClearAll();
//	_vSpriteTables.ClearAll();
//	_dlActiveSpriteTables.ClearAll();
//	_dlGlowSpriteTables.ClearAll();
//	_dlInactiveSpriteTables.ClearAll();

//	_lpInactiveParticlesSprite = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		AddForceField
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::AddForceField(const COM_INSTANCE &	ForceFieldNodeInst)
{
	_vForceFieldsNodes.PushTail(ForceFieldNodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		ResetForceFields
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM::ResetForceFields()
{
	_vForceFieldsNodes.Clear();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
