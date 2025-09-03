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
//	CLASS:	PARTICLE
//
//	01-11-19:	ELE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(OldCom/Database)

#include	INC_3DENGINE(SceneGraph\Entities\ForceFields)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(Display)

#ifdef _DEBUG
	#include	"Particle.inl"
#else
	#include	"Inline.h"

	#include	INL_3DENGINE(SceneGraph\Node)
	#include	INL_3DENGINE(SceneGraph\Entities\ForceFields)
	#include	INL_3DENGINE(Display)
	
	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)
	#include	INL_KRNCORE(Math\Color)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PARTICLE constructor
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE::PARTICLE()
:
_rMass				(1.0f),
_OneOnMass			(1.0f),
_rMomentOfInertia	(1.0f),
_Position			(0.0f, 0.0f, 0.0f),
_rSpinAngle			(0.0f),
_SpinAxis			(0.0f, 0.0f, 1.0f),
_Speed				(0.0f, 0.0f, 0.0f),
_rSpinAngleSpeed	(0.0f),
_SpinAxisSpeed		(0.0f, 0.0f, 0.0f),
_rLife				(0.0f),
_bOnlyUniformField	(true),
_ShapeInst			(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_pNext				(NULL),
_rNbrSLeft			(0.0f)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		~PARTICLE destructor
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE::~PARTICLE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::Init()
{
	QDT_ASSERT(_ShapeInst.HasHandle());

	if (_ShapeInst.HasHandle())
	{
		PARTICLE_SHAPE *pPS = static_cast<PARTICLE_SHAPE *>(_ShapeInst.GetCommunicator());

		_rLife = pPS->GetLifeSpan();
		_rMass = pPS->GetMass();
		_rMomentOfInertia = pPS->GetMomentOfInertia();
	}

	_bOnlyUniformField = true;
	_pNext = NULL;

	_rNbrSLeft = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::Update(float	rDeltaTime)
{	
	QDT_ASSERT(_ShapeInst.HasHandle());

	if	(_ShapeInst.HasHandle())
	{
		PARTICLE_SHAPE *pPS = static_cast<PARTICLE_SHAPE *>(_ShapeInst.GetCommunicator());
		if (pPS->IsLifeSpanActive() && _bActive)
		{
			if	(_rDegenerationLifeFactor == 0.0f)
			{
				Desactivate();
			}
			else
			{
				_rLife -= _rDegenerationLifeFactor * rDeltaTime;
				if	(_rLife <= 0.0f)
				{
					Desactivate();
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::Display()
{
	RENDER_CONTEXT		*pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
	COLOR_FLOAT			Color(1.0f, 0.0f, 1.0f, 0.68f);
	MATRIX				Id(true);

	pRenderContext->SetWorldTransform0(&Id);
	_Position.Display(Color, 1.0f);

	DISPLAY_MANAGER::Instance()->GetStats().AddParticles(1);
}

//-----------------------------------------------------------------------------
//	Name:		ApplyPhysics
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::ApplyPhysics(float rDeltaTime)
{
/*	if	(_bActive)
	{
		int				i, nNbrForceFields;
		float			rDeltaSpinAngle;
		VECTOR			Force(false);
		VECTOR			NetForce(0.0f, 0.0f, 0.0f);	
		VECTOR			Acceleration(false);
		VECTOR			DeltaSpeed(false);
		VECTOR			DeltaPosition(false);
		FORCE_FIELD_ABC	*pFF;
		NODE			*pFFNode;

		//	Second law of Newton
		nNbrForceFields = _vForceFieldsNodes.GetSize();
		for	(i = 0 ; i < nNbrForceFields ; ++i)
		{
			pFFNode = static_cast<NODE *>(_vForceFieldsNodes[i]->GetObject());
			pFF = static_cast<FORCE_FIELD_ABC *>(pFFNode->GetEntity());
			pFF->GetForceToApply(*this, Force, rDeltaTime);
			NetForce += Force;
		}
		Acceleration = NetForce * _OneOnMass;
		
		// Update speed
		DeltaSpeed = Acceleration * rDeltaTime ; 	
		_Speed += DeltaSpeed;
		
		// Update position
		DeltaPosition = _Speed * rDeltaTime;
		_Position += DeltaPosition;

		// Update spin angle	
		rDeltaSpinAngle = _rSpinAngleSpeed * rDeltaTime;
		_rSpinAngle += rDeltaSpinAngle;
	}*/

	if	(_bActive)
	{
		float			rDeltaSpinAngle;
		VECTOR			DeltaSpeed(false);
		VECTOR			DeltaPosition(false);

		// Update speed
		DeltaSpeed = _Acceleration * rDeltaTime; 	
		_Speed += DeltaSpeed;
		
		// Update position
		DeltaPosition = _Speed * rDeltaTime;
		_Position += DeltaPosition;

		// Update spin angle	
		rDeltaSpinAngle = _rSpinAngleSpeed * rDeltaTime;
		_rSpinAngle += rDeltaSpinAngle;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeForces
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::ComputeForces(const QDT_VECTOR <COM_INSTANCE> & vParticleSystemForceFieldsNodes,
								float							  rDeltaTime)
{
	if	(_rNbrSLeft <= 0.0f)
	{
		int				i, nNbrParticleSystemForceFields;
		COM_INSTANCE	FFNodeInst;
		NODE			*pFFNode;
		FORCE_FIELD_ABC	*pFF;
		VECTOR			Force(false);

		_Force.SetXYZ(0.0f, 0.0f, 0.0f);
		_rDegenerationLifeFactor = 1.0f;

		_bOnlyUniformField = true;

		nNbrParticleSystemForceFields = vParticleSystemForceFieldsNodes.GetSize();
		for	(i = 0 ; i < nNbrParticleSystemForceFields ; ++i)
		{
			FFNodeInst = vParticleSystemForceFieldsNodes[i];
			pFFNode = static_cast<NODE *>(FFNodeInst.GetCommunicator());
			pFF = static_cast<FORCE_FIELD_ABC *>(pFFNode->GetEntity());

			pFF->TransformedDatas(pFFNode->GetGlobalMatrix());
			if	(pFF->TestInfluenceVolume(*this))
			{
				if	(pFF->IsLifeField())
				{
					_rDegenerationLifeFactor *= pFF->GetMagnitude();
				}
				else
				{
					pFF->GetForceToApply(*this, Force);
					_Force += Force;

					if	((pFF->IsVortexField()) || (pFF->IsRadialField()))
					{
						_bOnlyUniformField = false;
					}
				}
			}
		}

		_Acceleration = _Force * _OneOnMass;

		_rNbrSLeft += 0.4f;
	}
	else
	{
		_rNbrSLeft -= rDeltaTime;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetLife
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::ResetLife()
{
	QDT_ASSERT(_ShapeInst.HasHandle());

	if	(_ShapeInst.HasHandle())
	{
		PARTICLE_SHAPE *pPS = static_cast<PARTICLE_SHAPE *>(_ShapeInst.GetCommunicator());
		_rLife = pPS->GetLifeSpan();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
