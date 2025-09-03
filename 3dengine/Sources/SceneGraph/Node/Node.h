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
// CLASS: NODE
//
//
//00-10-31 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: NODE
//00-10-31 JVA Created: 
//=============================================================================

#ifndef __SCENEGRAPH_NODE_NODE_H__
#define __SCENEGRAPH_NODE_NODE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCOM(COMMUNICATOR_CONTAINER)
DECL_KMATH(AABB)
DECL_3DENGINE(CONTAINER)
DECL_3DENGINE(ENTITY_CONTROLLER_ABC)
DECL_3DENGINE(SKELETON)
DECL_3DENGINE(NODE_CHILD)
DECL_KPHYSICS(I_PHYSIC_SCENE)
DECL_KPHYSICS(I_PHYSIC_DYNAMIC_ACTOR)
DECL_KPHYSICS(I_PHYSIC_CONTROLLER)
DECL_KPHYSICS(I_PHYSIC_JOINT)

#include INCL_KCORE(Template/QDT_PAIR)
#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include INCL_KPHYSICS(Physics/Enums)

namespace QDT
{
	namespace M3D
	{
		class	NODE : public COMMUNICATOR
		{
		public :

			enum FLAG
			{  
				LOCAL_MATRIX_IDENTITY			= 1 << 0,
				GLOBAL_MATRIX_VALID				= 1 << 1,
				STATIC_MATRIX					= 1 << 2,
				UNTRANSFORMED					= 1 << 3,
				NO_LIGHTING						= 1 << 4,
				NO_COLLISION					= 1 << 5,
				DISPLAY_BOUNDING_BOX			= 1 << 6,
				STORE_LAST_GLOBAL_MATRIX		= 1 << 7,
				ENTITY_NODE						= 1 << 9,
				ENTITY_FOG						= 1 << 10,
				ENTITY_VISIBLE					= 1 << 11,
				INSIDE							= 1 << 12,
				STATIC_COLLISION				= 1 << 13,
				JOINT_NODE						= 1 << 14,
				CONTAINER_ON_CHILD				= 1 << 15,
				CONTROLLER_ON_CHILD				= 1 << 16,
				ENTITY_TO_UPDATE_ON_CHILD		= 1 << 17,
				GLOBAL_MATRIX_INVALID_ON_CHILD	= 1 << 18,
				DO_CLIPPING						= 1 << 19,		// Do hardware clipping on mesh
				FORCE_CLIPPING					= 1 << 20,
				INSIDE_TREE_VALID				= 1 << 21,		//	Used by NODE_ARRAY to test inside call to UpdateAllMatrix
				PHYSIC_CONTROLLER				= 1 << 22,
				RAGDOLL							= 1 << 23,
				FLAG_MAX = 0xFFFFFFFF
			};


										NODE();
										NODE(CLASS_DESCRIPTOR * const pCD);
			virtual						~NODE();

			static QDT_VECTOR<NODE*>::ITERATOR	Find(const QDT_VECTOR <NODE *> & v, NODE * pN);
			static QDT_DLIST<NODE*>::ITERATOR	FindInList(const QDT_DLIST <NODE *> & v, NODE * pN);

			// COMMUNICATOR_CONTAINER
			void						AddContentToCommunicatorContainer(COMMUNICATOR_CONTAINER * pContainer) const;

			void						TransformVector(VECTOR & Vect);
			void						TransformVector(const VECTOR & vInput, VECTOR & vTransformed);

			void						UpdateBehavior(float rSpeedFactor, bool bForceUpdate);
			virtual void				UpdateContainer(float rSpeedFactor);
			virtual void				ForceUpdate( float rSpeedFactor = 0.0f );
			virtual	VECTOR*				GetGroundPoints( int & nOutNbPoints );
			virtual	int					GetPotentialNbrTracks()const;

			virtual void				UpdateController(float rSpeedFactor);

			void						SetTestFatherVisibilityOnChild(bool b);

			// Write node info in a stream
			void						DumpTree(IO_STREAM &Out);
			void						Dump(IO_STREAM &Out);
			void						Dump(const char *pFileName);

			// Modifiors
			_INLINE_ void				SetContainer(CONTAINER* const pContainer);
			void						SetFlagsTree(int nFlagsMask, int nFlagsSet);
			void						SetFlagsUp(int nFlagsMask, int nFlagsSet);
			_INLINE_ void				SetEntityFog(bool b);
			void						SetController(ENTITY_CONTROLLER_ABC* pController);
//			void						BreakConnections();
//			void						BreakConnectionsRec();
//			static void					BreakConnections(const COM_INSTANCE & Catalog);
//			void						BuildNodeArrayRec(QDT_VECTOR <COM_INSTANCE> & vNodeArray);
			_INLINE_ void				SetStaticCollide(const bool bStatus);

			// Accessors
			_INLINE_ const QDT_FLAGS &		GetFlags() const;
			_INLINE_ QDT_FLAGS &			GetFlags();
			_INLINE_ int					GetFlags(int nFlagsMask) const;
			_INLINE_ void					SetFlags(int nFlagsMask, int nFlagsSet);
			_INLINE_ const QDT_FLAGS &		GetChildrenEntityTypes() const;
			_INLINE_ QDT_FLAGS &			GetChildrenEntityTypes();
			_INLINE_ bool					GetEntityFog() const;
			_INLINE_ bool					IsSetContainerOnChild() const;
			_INLINE_ bool					IsSetControllerOnChild() const;
			_INLINE_ bool					IsSetEntityToUpdateOnChild() const;
			_INLINE_ bool					IsSetGlobalMatrixInvalidOnChild() const;
			void							SetContainerOnChild();
			void							UnsetContainerOnChild();
			void							SetControllerOnChild();
			void							UnsetControllerOnChild();
			void							SetGlobalMatrixInvalidOnChild();
			void							UnsetGlobalMatrixInvalidOnChild();
			
//			_INLINE_ unsigned int			GetNbrChildren() const;
//			_INLINE_ const COM_INSTANCE &	GetChildInst(unsigned int i) const;
//			_INLINE_ NODE*					GetChild(unsigned int i) const;

			_INLINE_ void					SetParentNodeInst(const COM_INSTANCE & ParentInst);
			_INLINE_ const COM_INSTANCE &	GetParentNodeInst() const;
			_INLINE_ NODE *					GetParentNode() const;
			_INLINE_ void					SetFirstChildInst(const COM_INSTANCE & FirstChildInst);
			_INLINE_ const COM_INSTANCE &	GetFirstChildInst() const;
			_INLINE_ NODE *					GetFirstChild() const;
			_INLINE_ void					SetBrotherInst(const COM_INSTANCE & BrotherInst);
			_INLINE_ const COM_INSTANCE &	GetBrotherInst() const;
			_INLINE_ NODE *					GetBrother() const;

			_INLINE_ CONTAINER*				GetContainer() const;
			virtual ENTITY_CONTROLLER_ABC*	GetController(int nBSIdx) const;
			virtual bool					HasController() const;
			_INLINE_ int					GetParentedIndex()const;
			_INLINE_ void					SetParentedIndex( int nIndex );	
			_INLINE_ void					SetAsEntityNode( bool bVal );
			_INLINE_ bool					IsEntityNode()const;
			_INLINE_ void					ActivateBoundingBoxDisplay( bool b = true );
			_INLINE_ bool					BoundingBoxDisplayIsActive()const;
			void							ActiveAnimMix( bool bVal , bool bBlend );
			_INLINE_ const bool				IsStaticCollide() const;

			
			// Container
			void					ResetAnimMatrix();
			void					ResetContainerPos();
			void					ResetContainerMatrix();
			static int				ResetAllChildMatrix(NODE *pNode, void *pData);
			bool					CheckEmptyContainer() ;
			void					RemoveContainer();
			bool					IsChildOf(const COM_INSTANCE & NodeInst) const;
			
			// Hierarchy operations
			void					AddChild(const COM_INSTANCE & NodeInst);
			void					AddChild(const COM_INSTANCE & NodeInst, int nIndex);
			void					AddChildKeepGlobalMatrix(const COM_INSTANCE & NodeInst, int nIndex);
//			void					AttachChildKeepGlobalMat(const COM_INSTANCE & ChildInst, int nIndex = -1); // change local matrix to keep global matrix
//			void					AttachCatalog(const COM_INSTANCE & Catalog);
//			void					DetachChild(const COM_INSTANCE &ChildInst) ;
//			void					DetachChildKeepGlobalMat(const COM_INSTANCE &ChildInst);  // change local matrix to keep global matrix
//			void					DetachCatalog(const COM_INSTANCE & Catalog);

			void					RemoveFromFather();
			void					RemoveFromFatherKeepGlobalMat();
			void					Instanciate(NODE * pNewNode);
			void					InstanciateRec(NODE * pNewNode);
			void					PostInstanciate(const QDT_VECTOR < QDT_PAIR< COM_INSTANCE, COM_INSTANCE > > & vInstanciatedNodes);

			void					BuildAABB(AABB &BoundingBox);

			/** Matrix operations */
			virtual const MATRIX&	GetIndexedGlobalMatrix( int nNum )const;
			virtual void			UpdateGlobalMatrix();
			void					UpdateChildrenGlobalMatrix();
			virtual	void			UpdateLocalMatrix();
			_INLINE_ const MATRIX&	GetParentGlobalMatrix()const;
			_INLINE_ void			SetLocalPosition(const VECTOR & vPoint);
			_INLINE_ void			GetLocalPosition(VECTOR &outPos) const;
			_INLINE_ void			SetLocalRotXYZ(const VECTOR & vAngle3D);
			_INLINE_ void			SetLocalMatrix(const MATRIX & Mat);
			_INLINE_ const MATRIX & GetLocalMatrix() const;
			_INLINE_ MATRIX &		GetLocalMatrix();
					void			SetGlobalPosition(const VECTOR & vPoint);
			_INLINE_ void			GetGlobalPosition(VECTOR &outPos);
			_INLINE_ void			SetGlobalRotXYZ(const VECTOR & vAngle3D);
			_INLINE_ void			SetGlobalMatrix(const MATRIX & Mat);
			_INLINE_ MATRIX &		GetGlobalMatrix();
			_INLINE_ const MATRIX &	GetGlobalMatrix() const;
					void			GetGlobalCenter(VECTOR &outCenter);
			_INLINE_ void			IncBlocReference();
			_INLINE_ int			DecBlocReference();
			_INLINE_ void			ResetBlocReference();
			_INLINE_ int			GetBlocReference()const;		

			/** Rotations */
			/** If you combine more than one command, please send them in the right order (XYZ) */	
			void					LookUp(float rAngle);		// X axe
			void					LookDown(float rAngle);
			void					TurnLeft(float rAngle);		// Y axe
			void					TurnRight(float rAngle);
			void					RollLeft(float rAngle);		// Z axe
			void					RollRight(float rAngle);
			
			/** Displacement */
			void					MoveForward(float rValue);
			void					MoveBackward(float rValue);
			void					StrafeLeft(float rValue);
			void					StrafeRight(float rValue);
			void					MoveUp(float rValue);
			void					MoveDown(float rValue);

			/** Target */
			void					LookAt(const VECTOR & TargetPos);
			void					MoveNearTo(const VECTOR & TargetPos, float rValue);
			void					MoveAwayFrom(const VECTOR & TargetPos, float rValue);
			void					MoveLeftAround(const VECTOR & TargetPos, float rValue, bool bHasRoll = true );
			void					MoveRightAround(const VECTOR & TargetPos, float rValue, bool bHasRoll = true );
			void					MoveUpAround(const VECTOR & TargetPos, float rValue, bool bHasRoll = true );
			void					MoveDownAround(const VECTOR & TargetPos, float rValue, bool bHasRoll = true );
			
			//	Reset pitch, heading and roll
			void					ResetPitch();
			void					ResetHeading();
			void					ResetRoll();
			void					GetPitchHeadRoll( QUATERNION& QPitch, QUATERNION& QHead, QUATERNION& QRoll );
			void					GetPitchHeadRoll( ANGLE3D& Angle );
			float					GetFrontDirAngle();
			QUATERNION				GetQuaternion() const;
			void					SetQuaternion(const QUATERNION& Quat);
			
			/** Node*/
			/** Get the scene graph of the node */
//			const COM_INSTANCE &	GetSceneGraphNodeInst() const;

			/** Ray cast */
//			bool					RayCast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue) ;
			/** Ray Cast */
//			void					RayCastTree(RAY_CAST_PARAMETERS & RayCastParameters); 

			//	Play animation
			void					PlayAnim( const COM_INSTANCE& Anim );
			void					PushAnim( const COM_INSTANCE& Anim );
			const COM_INSTANCE 		GetActiveAnim()const;
			void					PlayTargetAnim( const COM_INSTANCE& TargetAnim );
			void					PlayPartAnim( const COM_INSTANCE& PartAnim );
			
//			int						CountJointChildren();
			int						CountTotalJointChildren();
//			int						CountTotalHairJointChildren();
//			int						SearchForFeet(SKELETON		*pSK);

			/** Make a living copy of a skeleton */
			COM_INSTANCE			LiveSkeleton(I_PHYSIC_SCENE * pScene, UInt32 uiLastLockedLevel, UInt32 uiFirstFreeLevel);
			void					LiveSkeleton(const COM_INSTANCE &Inst, I_PHYSIC_SCENE * pScene, UInt32 uiLastLockedLevel, UInt32 uiFirstFreeLevel);

			/** */
			bool					IsPlaying() const ; 
			/** */

			_INLINE_ bool			IsRootNode() const ; 

					void			GetFrame( float& rFrame) ;
			
			//	Purge animations
			void					PurgeAllAnimConnections();
			void					PurgeMainAnimConnection();
			void					PurgeTargetAnimConnection();
			void					PurgeAnimMixConnection();

			// Physic
			_INLINE_ I_PHYSIC_DYNAMIC_ACTOR *	GetDynamicActor() const;
			_INLINE_ I_PHYSIC_CONTROLLER *		GetPhysicController() const;
			void								UnregisterDynamicActor(I_PHYSIC_SCENE * pPhysicScene);
			void								ApplyForce(const VECTOR & ForceDir, float rForceStrength);
			void								ApplyTorque(const VECTOR & ForceDir, float rForceStrength);
			virtual void						EnableKinematic(bool b);
			bool								IsKinematicEnabled() const;
			bool								IsSklPhysicJoint() const;
			void								SetLinearDamping(float rDamping);
			float								GetLinearDamping() const;
			void								SetAngularDamping(float rDamping);
			float								GetAngularDamping() const;
			void								InitPhysicJoint(NODE_CHILD *pSC, I_PHYSIC_SCENE * pPhysicScene);
			void								Move();
			void								SetAttachJoint(I_PHYSIC_JOINT * pJoint);
			I_PHYSIC_JOINT *					GetAttachJoint();
			
			///////////////////////////////////////////////////////////////////////////
			int						CheckSubGraphFatherLink( bool bCorrect );
			///////////////////////////////////////////////////////////////////////////

			bool					IsLastGlobalMatrixStorred() const;
			void					Replace(const MATRIX &M);

			// Foot steps
			virtual void			FootStep(int nGroundPointIdx, float rGroundHeight);

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

			virtual	void					OnCloseObject();

//			void SaveState(QDT_VECTOR<COM_INSTANCE> & vNodes, QDT_VECTOR<MATRIX> & vMatrices, QDT_VECTOR<COM_INSTANCE> & vParentNodes);

			I_PHYSIC_DYNAMIC_ACTOR *		CreatePhysicDynamicActor();
			I_PHYSIC_CONTROLLER *			CreatePhysicController(PHYSIC_CONTROLLER_TYPE Type);
			void							DeletePhysicController();

			_INLINE_ void					UpdateOldPosition();
			_INLINE_ void					GetOldPosition(VECTOR & Position);

			// Visibility
			_INLINE_ const OBB &			GetLocalBoundingBox() const;
			_INLINE_ void					SetLocalBoundingBox(const OBB &);
			_INLINE_ const SPHERE &			GetLocalBoundingSphere() const;
			_INLINE_ void					SetLocalBoundingSphere(const SPHERE &);

		protected:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);

			static	void						CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			#ifndef _MASTER
			static	void						CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			#endif
		
			void								RemoveChild(const COM_INSTANCE & NodeInst);
			void								RemoveChildKeepGlobalMat(const COM_INSTANCE & NodeInst);

			void								SetGlobalMatrixInvalidOnFathers();
			void								SetGlobalMatrixInvalidOnChildren();
			_INLINE_ void						CheckAndUpdateInsideTreeMatrices();
			virtual	void						UpdateInsideTree();

			QDT_FLAGS							_Flags;
			QDT_FLAGS							_ChildrenEntityTypes;

			/** Matrix */
			MATRIX								_LMatrix;	// Local
			MATRIX								_GMatrix;	// Global

			// Visibility
			OBB 								_LocalBoundingBox;
			SPHERE								_LocalBoundingSphere;

			// Hierarchy
			COM_INSTANCE						_ParentInst;
			COM_INSTANCE						_FirstChildInst;
			COM_INSTANCE						_BrotherInst;

			// Controllers
			CONTAINER *							_pContainer;	
			ENTITY_CONTROLLER_ABC *				_pController; 

			int									_nParentedIndex;

			static const float					LIGHT_INTERPOLATION_DELAY;

			// Physics
			I_PHYSIC_DYNAMIC_ACTOR *			_pDynamicActor;
			I_PHYSIC_CONTROLLER *				_pPhysicController;
			bool								_bSklPhysicJoint;
			VECTOR								_OldPos;
			I_PHYSIC_JOINT *					_pAttachJoint;
		};
	}
}

#include INCL_3DENGINE(M3D/Moteur3DModule)
#include INCL_KPHYSICS(Physics/IPhysicDynamicActor)

#ifndef _DEBUG
#include "Node.inl"
#endif

#endif // __SCENEGRAPH_NODE_NODE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================

