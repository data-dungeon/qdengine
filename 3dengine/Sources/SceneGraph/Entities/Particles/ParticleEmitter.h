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
//	The PARTICLE_EMITTER class implements ...
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLEEMITTER_H__
#define __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLEEMITTER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(PARTICLE_SHAPE)
DECL_3DENGINE(FORCE_FIELD_ABC)
DECL_KMATH(SEGMENT)
DECL_KMATH(TRIANGLE)
DECL_KMATH(CIRCLE)
DECL_KMATH(DISC)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KCOM(CommunicatorType/ComInstance)

#define MAX_FORCE_FIELDS 8

namespace QDT
{
	namespace M3D
	{
		class	PARTICLE_EMITTER : public ENTITY_ABC
		{
		public:

			enum
			{
				CMD_PE_SET_PARTICLE_SHAPE = CMD_COM_USER_START,
				CMD_PE_GET_PARTICLE_SHAPE,
				CMD_PE_SET_SPAWN_RATE,
				CMD_PE_GET_SPAWN_RATE,
				CMD_PE_SET_EMISSION_START_ANGLE,
				CMD_PE_GET_EMISSION_START_ANGLE,
				CMD_PE_SET_EMISSION_END_ANGLE,
				CMD_PE_GET_EMISSION_END_ANGLE,
				CMD_PE_SET_EMISSION_CONE,
				CMD_PE_SET_EMISSION_SPEED,
				CMD_PE_GET_EMISSION_SPEED,
				CMD_PE_SET_EMISSION_SPIN_ANGLE,
				CMD_PE_GET_EMISSION_SPIN_ANGLE,
				CMD_PE_SET_EMISSION_SPIN_AXIS,
				CMD_PE_SET_EMISSION_SPIN_AXIS2,
				CMD_PE_GET_EMISSION_SPIN_AXIS,
				CMD_PE_SET_EMISSION_SPIN_ANGLE_SPEED,
				CMD_PE_GET_EMISSION_SPIN_ANGLE_SPEED,
				CMD_PE_SET_EMISSION_SPIN_AXIS_SPEED,
				CMD_PE_SET_EMISSION_SPIN_AXIS_SPEED2,
				CMD_PE_GET_EMISSION_SPIN_AXIS_SPEED,
				CMD_PE_SET_EMISSION_POINT,
				CMD_PE_SET_EMISSION_SEGMENT,
				CMD_PE_SET_EMISSION_TRIANGLE,
				CMD_PE_SET_EMISSION_BOX,
				CMD_PE_SET_EMISSION_SPHERE,
				CMD_PE_SET_EMISSION_DISC,	
				CMD_PE_SET_EMISSION_CIRCLE,
				CMD_PE_SET_ACTIVE,
				CMD_PE_IS_ACTIVE,
				CMD_PE_COPY,
				CMD_PE_SET_SCALEX,
				CMD_PE_GET_SCALEX,
				CMD_PE_SET_SCALEY,
				CMD_PE_GET_SCALEY,
				CMD_PE_SET_SCALEZ,
				CMD_PE_GET_SCALEZ,
				CMD_SET_RECEIVE_ALL_LIFE_FIELDS,
				CMD_PE_LAST,
			};

			enum PM_SHAPE_TYPE
			{
				PM_SHAPETYPE_NONE = 0,
				PM_SHAPETYPE_POINT3D,
				PM_SHAPETYPE_SEGMENT,
				PM_SHAPETYPE_TRIANGLE,
				PM_SHAPETYPE_OBB,
				PM_SHAPETYPE_SPHERE,
				PM_SHAPETYPE_CIRCLE,
				PM_SHAPETYPE_DISC,
			};
		 
												PARTICLE_EMITTER();
			virtual							   ~PARTICLE_EMITTER();

			// Operators
			PARTICLE_EMITTER &	operator=(const PARTICLE_EMITTER & C);

			virtual void						Register();
			virtual void						Unregister();

			virtual void						Instanciate(COM_INSTANCE & Instance);

			void								Copy(const PARTICLE_EMITTER &ParticleEmitter, bool bCopyShapeInst = true);

			// Derivation from ENTITY_ABC
			void								DisplayParticles(DISPLAY_PARAMETERS & DisplayParameters, bool bDoVisibility);
			bool								TestVisibility(FRUSTRUM & Frustum);

												
			virtual	 void						UpdateParticleEmitter(float rDeltaFrameTime, NODE * pNode);
			
			// Accessors
			_INLINE_ const OBB &				GetBoundingBox() const;
			_INLINE_ float						GetSpawnRate() const;
			_INLINE_ float						GetEmissionSpeed() const;
			_INLINE_ const VECTOR & 			GetStartAngle() const;
			_INLINE_ const VECTOR & 			GetEndAngle() const;
			_INLINE_ float						GetEmissionSpinAngle() const;
			_INLINE_ const VECTOR &				GetEmissionSpinAxis();
			_INLINE_ float						GetEmissionSpinAngleSpeed() const;
			_INLINE_ const VECTOR &				GetEmissionSpinAxisSpeed() const;
			_INLINE_ const COM_INSTANCE &		GetForceFieldInst(int nNumForce) const;
			_INLINE_ QDT_VECTOR <COM_INSTANCE>	GetForceFields() const;
			_INLINE_ int						GetNbrForceFields();
		//	PARTICLE::PARTICLE_TYPE				GetParticleType() const;
			_INLINE_ const COM_INSTANCE &		GetParticleShapeInst() const;
			_INLINE_ PARTICLE_SHAPE*			GetParticleShape() const;
			_INLINE_ bool						NeedUpdate() const;
			_INLINE_ bool						IsActive() const;
					void						GetEmissionPosition(VECTOR & StartPosition) const;
					void						GetEmissionSpeed(VECTOR & StartSpeed) const;
					void						GetEmissionSpin(float & rStartSpinAngle, VECTOR & StartSpinAxis) const;
					void						GetEmissionSpinSpeed(float & rStartSpinAngleSpeed, VECTOR & StartSpinAxisSpeed) const;
			_INLINE_ float						GetVisibilityDistance() const;
			

			// Modifiors
			_INLINE_ void						SetSpawnRate(float SpawnRate);
			_INLINE_ void						SetEmissionAngle(float Angle);
			_INLINE_ void						SetEmissionSpeed(float Speed);
			_INLINE_ void						SetStartAngle(const VECTOR & Angle);
			_INLINE_ void						SetEndAngle(const VECTOR & Angle);
			_INLINE_ void						SetEmissionSpinAngle(float SpinAngle);
			_INLINE_ void						SetEmissionSpinAxis(const VECTOR & Vector);
			_INLINE_ void						SetEmissionSpinAngleSpeed(float SpinAngleSpeed);
			_INLINE_ void						SetEmissionSpinAxisSpeed(const VECTOR & Vector);
			_INLINE_ void						AddForceFieldInst(const COM_INSTANCE &Forceinst);
			_INLINE_ void						SetForceFields(QDT_VECTOR <COM_INSTANCE> pForcesInst);	
			_INLINE_ void						SetNbrForceFields(int nNbr);
			_INLINE_ void						SetBoundingBox(const OBB& pBB);
			_INLINE_ void						SetParticleShapeInst(const COM_INSTANCE &ParticleShapeInst);
			_INLINE_ void						SetActive(bool bActive = true);
			_INLINE_ void						SetAutoSpawn(bool bAutoSpawn = true);
			_INLINE_ void						SetVisibilityDistance(float rDistance);
			_INLINE_ void						SetNeedUpdate(bool b);
		 
					int							GetNewParticleSprite();

					void						SetEmissionShape(const VECTOR & Point);
					void						SetEmissionShape(const SEGMENT & Segment);
					void						SetEmissionShape(const TRIANGLE & Triangle);
					void						SetEmissionShape(const OBB & Obb);
					void						SetEmissionShape(const SPHERE & Sphere);
					void						SetEmissionShape(const CIRCLE & Circle);
					void						SetEmissionShape(const DISC & Disc);
					
					void						DeleteEmissionShape();	

					void						Clear();

			// For Display
			_INLINE_ void						SetViewMatrix(const MATRIX* pViewMat);	
					void						UpdateParticles(float rDeltaFrameTime, NODE * pNode);
					void						SpawnParticles(float rDeltaFrameTime, NODE * pNode);
					void						SpawnParticle(const MATRIX & GlobalMatrix);


			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();

					void						ComputeForces(unsigned int i, float rDeltaTime = 0.0f);	
					void						ComputeForcesNoForce(unsigned int i, float rDeltaTime = 0.0f);
					void						ComputeForcesOneLifeField(unsigned int i, float rDeltaTime = 0.0f);

					void						COM_PostLoad();

		private:

			void								AllocateParticles();
			void								UpdateParticleShapeSprite();
			void								CreateSpriteTable();

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetParticleShape(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetParticleShape(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetSpawnRate(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetSpawnRate(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionStartAngle(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetEmissionStartAngle(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionEndAngle(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetEmissionEndAngle(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionCone(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionSpeed(void * const pO, void * const pR, void * const pP);	
			static	void						EM_GetEmissionSpeed(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionSpinAngle(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetEmissionSpinAngle(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionSpinAxis(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetEmissionSpinAxis2(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetEmissionSpinAxis(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionSpinAngleSpeed(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetEmissionSpinAngleSpeed(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionSpinAxisSpeed(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionSpinAxisSpeed2(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetEmissionSpinAxisSpeed(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionPoint(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionSegment(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionTriangle(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionBox(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionSphere(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionDisc(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetEmissionCircle(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetActive(void * const pO, void * const pR, void * const pP);
			static  void						EM_IsActive(void * const pO, void * const pR, void * const pP);
			static  void						EM_Copy(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleX(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetScaleX(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleY(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetScaleY(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleZ(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetScaleZ(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetReceiveAllLifeFields(void * const pO, void * const pR, void * const pP);

			// Chunk
		#ifndef _MASTER_ONLY_DATABANK
			static void							CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void							CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static void							CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

			OBB									_BoundingBox;

			VECTOR								_StartAngle;
			VECTOR								_EndAngle;
			VECTOR								_EmissionSpinAxis;
			VECTOR								_EmissionSpinAxisSpeed;

			COM_INSTANCE						_ParticleShapeInst;
			QDT_VECTOR<COM_INSTANCE>			_vForceFields;

			float								_rEmissionSpeed;
			float								_rEmissionSpinAngle;
			float								_rEmissionSpinAngleSpeed;
			float								_rVisibilityDistance;
			float								_rNewParticlesCounter;
			float								_rSpawnRate;	

			int									_nNbrForceFields;
			unsigned int						_nNbrParticles;
		//	SPRITE_TABLE_ABC *					_pSpriteTable;

		//	PARTICLE_SPRITE_ARRAY *				_pParticleArray;

			int									_nShapeType;
			union
			{
				void *							_pShape;
				VECTOR *						_pPoint;
				SEGMENT	*						_pSegment;
				TRIANGLE *						_pTriangle;
				OBB	*							_pObb;
				SPHERE *						_pSphere;
				CIRCLE *						_pCircle;
				DISC *							_pDisc;
			};

			bool								_bActive;
			bool								_bAutoSpawn;
			bool								_bNeedUpdate;
			bool								_bDoClipping;

			bool								_bIsInfluencedByAllLifeFields;

			void								SetUpForces(NODE * pNode);
			void								SetUpForcesSkipPerturbation(NODE * pNode);
			static FORCE_FIELD_ABC *			_ForceFields[MAX_FORCE_FIELDS];
			static bool							_bOnlyUniformForceFields;
			static unsigned int					_nNbrUsedForceFields;
		};
	}
}

#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleShape)

#ifndef _DEBUG
	#include	"ParticleEmitter.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLEEMITTER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
