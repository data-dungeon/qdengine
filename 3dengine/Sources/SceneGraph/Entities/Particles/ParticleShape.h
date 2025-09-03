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
//	CLASS:	PARTICLE_SHAPE
//	The PARTICLE_SHAPE class implements ...
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESHAPE_H__
#define __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESHAPE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)

#include	INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	PARTICLE_SHAPE : public COMMUNICATOR
		{
		public:

			enum
			{
				CMD_PSHAPE_ACTIVATE_LIFE_SPAN = CMD_COM_USER_START,
				CMD_PSHAPE_IS_LIFE_SPAN_ACTIVATED,
				CMD_PSHAPE_SET_LIFE_SPAN,
				CMD_PSHAPE_GET_LIFE_SPAN,
				CMD_PSHAPE_SET_MASS,
				CMD_PSHAPE_GET_MASS,
				CMD_PSHAPE_SET_MOMENT_OF_INERTIA,
				CMD_PSHAPE_GET_MOMENT_OF_INERTIA,
				CMD_PSHAPE_SET_SCALE,
				CMD_PSHAPE_SET_SCALE2,
				CMD_PSHAPE_SET_SCALE_NBR_KEYS,
				CMD_PSHAPE_GET_SCALE_NBR_KEYS,
				CMD_PSHAPE_SET_SCALE_TIME_KEY,
				CMD_PSHAPE_LAST,
			};

			virtual void						Update(float rDeltaTime);			
		//	virtual	SHADER_ABC *				GetMaterial() const;

		//	_INLINE_ PARTICLE_SHAPE_DATA *		GetShapeData() const;

		//	_INLINE_ PARTICLE::PARTICLE_TYPE	GetParticleType() const;

			_INLINE_ bool						IsLifeSpanActive() const;
			_INLINE_ void						ActivateLifeSpan();
			_INLINE_ void						DesactivateLifeSpan();

			_INLINE_ float						GetLifeSpan() const;
			_INLINE_ void						SetLifeSpan(float rLifeSpan);

			_INLINE_ float						GetMass() const;
			_INLINE_ void						SetMass(float rMass);

			_INLINE_ float						GetMomentOfInertia() const;
			_INLINE_ void						SetMomentOfInertia(float rMomentOfInertia);

			_INLINE_ void						SetScale(const VECTOR &vScale);
			_INLINE_ int						GetScaleNbKeys() const;				
			_INLINE_ void						SetScaleNbKeys(int nNbrKeys);				
			_INLINE_ void						SetScale(int nKey, const VECTOR &vScale);
			_INLINE_ void						GetScale(float rLife, VECTOR &vScale) const;
			_INLINE_ float						GetScaleTimeKey(int nKey) const;
			_INLINE_ void						SetScaleTimeKey(int nKey, float rTimeKey);
		//	_INLINE_ void						SetScaleTrack(const DATA_TRACK < VECTOR >&	track);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();

		protected:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void						EM_ActivateLifeSpan(void * const pO, void * const pR, void * const pP);
			static	void						EM_IsLifeSpanActivated(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetLifeSpan(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetLifeSpan(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetMass(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetMass(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetMomentOfInertia(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetMomentOfInertia(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetScale(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetScale2(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetScaleNbrKeys(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetScaleNbrKeys(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetScaleTimeKey(void * const pO, void * const pR, void * const pP);

												PARTICLE_SHAPE();
												PARTICLE_SHAPE(COMMUNICATOR_DESCRIPTOR *pCD);
			virtual							   ~PARTICLE_SHAPE();

		//	PARTICLE_SHAPE_DATA					*_pShapeData;
		};
	}
}

#ifndef _DEBUG
	#include	"ParticleShape.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESHAPE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
