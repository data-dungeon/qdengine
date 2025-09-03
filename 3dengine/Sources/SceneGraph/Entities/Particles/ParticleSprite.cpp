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
//	CLASS:	PARTICLE_SPRITE
//
//	01-11-20:	ELE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_KRNCOM(OldCom/Database)
#include	INC_KRNCORE(Display\RenderPipeline)

#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(Display)

#ifdef _DEBUG
	#include	"ParticleSprite.inl"
#else
	#include	"Inline.h"

	#include	INL_3DENGINE(SceneGraph\Material)
	#include	INL_3DENGINE(Display)

	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INL_KRNCORE(Math/Geometry/More)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_SPRITE constructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SPRITE::PARTICLE_SPRITE()
{
	_nType = PARTICLE::PARTICLE_TYPE_SPRITE;
}

//-----------------------------------------------------------------------------
//	Name:		~PARTICLE_SPRITE destructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SPRITE::~PARTICLE_SPRITE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SPRITE::Init()
{
	PARTICLE::Init();

	if	(_ShapeInst.HasHandle())
	{
		PARTICLE_SHAPE_SPRITE *pPS = static_cast<PARTICLE_SHAPE_SPRITE *>(_ShapeInst.GetCommunicator());

		_nDiffuseColorTrack = pPS->GetColorTrack();

		int	nNbrTracks = pPS->GetNbrColorTracks();
		if	((_nDiffuseColorTrack < 0) || (_nDiffuseColorTrack >= nNbrTracks))
		{
			_nDiffuseColorTrack = MATH::Rand(nNbrTracks);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SPRITE::Update(float	rDeltaTime)
{
	PARTICLE::Update(rDeltaTime);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SPRITE::Display()
{
	QDT_ASSERT(_ShapeInst.HasHandle());

	if	(_ShapeInst.HasHandle())
	{
		PARTICLE_SHAPE_SPRITE	*pPSS = static_cast<PARTICLE_SHAPE_SPRITE *>(_ShapeInst.GetCommunicator());
		RENDER_CONTEXT			*pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
		RENDER_PIPELINE_ABC		*pRenderPipeline = pRenderContext->GetRenderPipeline();
		COM_INSTANCE			MaterialInst = pPSS->GetMaterialInst();
		MATERIAL				*pMaterial = NULL;
		POINT3D					PositionInViewCS;
		VECTOR					vScale;

		if	(MaterialInst.HasHandle())
		{
			pMaterial = static_cast<MATERIAL *>(MaterialInst.GetCommunicator());
		}

		const MATRIX	*pMView = pRenderContext->GetTransform(RENDER_CONTEXT::MATRIXTYPE_VIEW);
		pMView->Apply(_Position, PositionInViewCS);

		pPSS->GetDiffuseColor(_nDiffuseColorTrack, _rLife, _DiffuseColor);
		pPSS->GetScale(_rLife, vScale);
		pPSS->GetUVs(_rLife, _pUVs);

		SCALE3D Scale(vScale.GetX(), vScale.GetY(), vScale.GetZ());
		
		pRenderPipeline->PushSprite(PositionInViewCS, Scale, &_pUVs[0], _rSpinAngle, _DiffuseColor, pMaterial->GetShader(), _bIsInfluencedByFog);

		DISPLAY_MANAGER::Instance()->GetStats().AddParticles(1);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
