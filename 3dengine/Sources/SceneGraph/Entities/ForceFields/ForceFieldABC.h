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
// CLASS: FORCE_FIELD_ABC
//
//
//	01-03-12:	RMA - Created
//*****************************************************************************

//=============================================================================
// Name: FORCE_FIELD_ABC
//	01-03-12:	RMA - Created
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_FORCEFIELDS_FORCEFIELDABC_H__
#define __M3D_SCENEGRAPH_ENTITIES_FORCEFIELDS_FORCEFIELDABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(SPHERE)
DECL_KMATH(OBB)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class FORCE_FIELD_ABC : public ENTITY_ABC
		{
		public:

			enum	TYPE_FORCE_FIELD
			{
				TFF_UNKNOWN					= -1,
				TFF_UNIFORM_FORCE_FIELD		=  0,
				TFF_VORTEX_FORCE_FIELD,
				TFF_RADIAL_FORCE_FIELD,
				TFF_LIFE_FORCE_FIELD,
				TFF_PERTURBATION_FORCE_FIELD,
				TFF_FORCE_FIELD_NBR,
			};

			enum
			{
				CMD_SET_MAGNITUDE = CMD_ENTITY_LAST,
				CMD_GET_MAGNITUDE,
				CMD_SET_DIRECTION_1,
				CMD_GET_DIRECTION_1,
				CMD_SET_DIRECTION_2,
				CMD_GET_DIRECTION_2,
				CMD_SET_INFLUENCE_SPHERE,
				CMD_SET_INFLUENCE_BOX,
				CMD_SET_ACTIVE,
				CMD_IS_ACTIVE,
				CMD_FORCE_FIELD_LAST,
			};

			enum FF_SHAPE_TYPE
			{
				FF_SHAPETYPE_NONE = 0,
				FF_SHAPETYPE_OBB,
				FF_SHAPETYPE_SPHERE,
			};

										FORCE_FIELD_ABC(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType);
			virtual					   ~FORCE_FIELD_ABC();

			/** Derivating from ENTITY_ABC */
			void						Display(DISPLAY_PARAMETERS & DisplayParameters);
			virtual void				Update();
			virtual bool				ToUpdate() const;	

			/** Get the force to apply to a physic body */
		//	virtual void				GetForceToApply(const PARTICLE & Particle, VECTOR & Force) const = 0;
		//	virtual void				GetForceToApply(PARTICLE_SPRITE_ARRAY * pArray, unsigned int i, VECTOR & Force) const = 0;

			/** Update the datas in the repere of a particle emitter */
					void				TransformedDatas(const MATRIX & GlobalForceFieldMatrix);

			// Accessors
			_INLINE_ TYPE_FORCE_FIELD	GetType() const;
			_INLINE_ float				GetMagnitude() const;
			_INLINE_ const VECTOR &		GetDirection() const;
			_INLINE_ bool				IsUniformField() const;
			_INLINE_ bool				IsRadialField() const;
			_INLINE_ bool				IsVortexField() const;
			_INLINE_ bool				IsLifeField() const;
			_INLINE_ bool				IsPerturbationField() const;
			_INLINE_ bool				IsActive() const;

			// Modifiors
			virtual  void				SetMagnitude(float rValue);
			_INLINE_ void				SetType(TYPE_FORCE_FIELD eType);
			_INLINE_ void				SetDirection(const VECTOR & Vector);
					void				SetInfluenceShape(const SPHERE & Sphere);
					void				SetInfluenceShape(const OBB & Obb);
			_INLINE_ void				SetActive(bool b);

			/** Test the influence volume with physic body */
		//	bool						TestInfluenceVolume(const PARTICLE & Particle) const;
		//	bool						TestInfluenceVolume(PARTICLE_SPRITE_ARRAY * pParticleArray, unsigned int i) const;

			_INLINE_ void				SetTransformed(bool bValue);

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();

		protected:

			void			DeleteInfluenceShape();

		#ifndef _MASTER_ONLY_DATABANK
			void			ReadCommon(IO_STREAM & Stream);
		#endif
		#ifndef _MASTER
			void			WriteCommon(IO_STREAM & Stream);
		#endif

			/** Force field type */
			TYPE_FORCE_FIELD	_eType;

			/** Magnitude : Intensity of the force field */
			float			_rMagnitude;

			/** Direction of the force field */
			VECTOR			_Direction;

			/** Influence volume */
			int				_nShapeType;
			union
			{
				SPHERE		*_pSphere;
				OBB			*_pObb;
			};

			VECTOR			_TransCenter;
			/** Direction of the force field transformed in the global coordinate system */
			VECTOR			_TransDirection;

			/** Influence volume transformed in the global coordinate system */
			union
			{
				SPHERE		*_pTransSphere;
				OBB			*_pTransObb;
			};

			bool			_bTransformed;
			bool			_bActive;
			
			// Commands
			static void		EM_SetMagnitude(void * const pO, void * const pR, void * const pP);
			static void		EM_GetMagnitude(void * const pO, void * const pR, void * const pP);
			static void		EM_SetDirection1(void * const pO, void * const pR, void * const pP);
			static void		EM_GetDirection1(void * const pO, void * const pR, void * const pP);
			static void		EM_SetDirection2(void * const pO, void * const pR, void * const pP);
			static void		EM_GetDirection2(void * const pO, void * const pR, void * const pP);
			static void		EM_SetInfluenceSphere(void * const pO, void * const pR, void * const pP);
			static void		EM_SetInfluenceBox(void * const pO, void * const pR, void * const pP);
			static void		EM_SetActive(void * const pO, void * const pR, void * const pP);
			static void		EM_IsActive(void * const pO, void * const pR, void * const pP);

		private :

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);
		};
	}
}

#ifndef _DEBUG
	#include	"ForceFieldABC.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_FORCEFIELDS_FORCEFIELDABC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
