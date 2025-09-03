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
//	CLASS:	PARTICLE_ABC
//	The PARTICLE_ABC class implements ...
//
//	01-11-19:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class PARTICLE
{
public:
								PARTICLE();
	virtual						~PARTICLE();

	enum	PARTICLE_TYPE
	{
		PARTICLE_TYPE_NONE,
		PARTICLE_TYPE_SPRITE,
	};

	virtual void				Init();
	virtual void				Display();

	virtual void				Update(float rDeltaTime);

	void						ComputeForces(const QDT_VECTOR <COM_INSTANCE> & vParticleSystemForceFieldsNodes, float rDeltaTime = 0.0f);
	void						ApplyPhysics(float rDeltaTime);

	_INLINE_ float				GetMass() const;
	_INLINE_ void				SetMass(float rMass);
	_INLINE_ float				GetMomentOfInertia() const;
	_INLINE_ void				SetMomentOfInertia(float rMomentOfInertia);

	_INLINE_ const POINT3D &	GetPosition() const;
	_INLINE_ void				SetPosition(const POINT3D &Position);
	_INLINE_ void				UpdatePosition(const VECTOR &dPosition);

	_INLINE_ float				GetSpinAngle() const;
	_INLINE_ void				SetSpinAngle(float rSpinAngle);
	_INLINE_ void				UpdateSpinAngle(float rSpinAngle);

	_INLINE_ const VECTOR &		GetSpinAxis() const;
	_INLINE_ void				SetSpinAxis(const VECTOR &SpinAxis);
	_INLINE_ void				UpdateSpinAxis(const VECTOR &dSpinAxis);
	
	_INLINE_ const VECTOR &		GetSpeed() const;
	_INLINE_ void				SetSpeed(const VECTOR &Speed);
	_INLINE_ void				UpdateSpeed(const VECTOR &dSpeed);

	_INLINE_ float				GetSpinAngleSpeed() const;
	_INLINE_ void				SetSpinAngleSpeed(float rSpinAngleSpeed);
	_INLINE_ void				UpdateSpinAngleSpeed(float rSpinAngleSpeed);

	_INLINE_ const VECTOR &		GetSpinAxisSpeed() const;
	_INLINE_ void				SetSpinAxisSpeed(const VECTOR &SpinAxisSpeed);
	_INLINE_ void				UpdateSpinAxisSpeed(const VECTOR &dSpinAxisSpeed);

	_INLINE_ float				GetLife() const;
			 void				ResetLife();

	_INLINE_ bool				OnlyUniformField() const;

	_INLINE_ const COM_INSTANCE &	GetShapeInst() const;
	_INLINE_ void					SetShapeInst(const COM_INSTANCE & ShapeInst);

	_INLINE_ PARTICLE_TYPE		GetType() const;

	_INLINE_ bool				IsActive() const;
	_INLINE_ void				Activate();
	_INLINE_ void				Desactivate();

	_INLINE_ void				SetInfluencedByFog(bool bValue);

	_INLINE_ virtual bool		IsParticleSprite() const;

	// List
	_INLINE_ PARTICLE *			GetNext() const;
	_INLINE_ void				SetNext(PARTICLE * pParticle);

protected:

	// Physique
	float						_rMass;
	float						_OneOnMass;
	float						_rMomentOfInertia;

	// Position & Orientation
	POINT3D						_Position;
	float						_rSpinAngle;		//rotation des sprites
	VECTOR						_SpinAxis;			//inutile pour les sprites

	// Dynamic
	VECTOR						_Speed;
	float						_rSpinAngleSpeed;
	VECTOR						_SpinAxisSpeed;
	float						_rLife;

	// Force fields flags
	bool						_bOnlyUniformField;

	// Particle shape
	COM_INSTANCE				_ShapeInst;

	// Type
	PARTICLE_TYPE				_nType;

	// Particle is active (must be updated)
	bool						_bActive;
	bool						_bIsInfluencedByFog;

	// List
	PARTICLE					*_pNext;

	// Nbr of seconds left before updating force fields list
	float						_rNbrSLeft;

	// Physics
	VECTOR						_Force;
	VECTOR						_Acceleration;
	float						_rDegenerationLifeFactor;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
