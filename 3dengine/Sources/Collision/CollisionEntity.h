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
//	CLASS:	COLLISION_ENTITY
//	The COLLISION_ENTITY class implements ...
//
//	03-09-01:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_COLLISION_COLLISIONENTITY_H__
#define __M3D_COLLISION_COLLISIONENTITY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(NODE)
DECL_3DENGINE(COLLISION_MESH)
DECL_3DENGINE(COLLISION_NOTIFIER)
DECL_3DENGINE(CM_SECTOR)
DECL_3DENGINE(CM_EDGE)
DECL_3DENGINE(CM_FACE)

#include INCL_KCOM(CommunicatorType/ComInstance)
#include INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)
#include INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_3DENGINE(SceneGraph/Node/Node)

#define MAX_GROUND_POINTS 10

namespace QDT
{
	namespace M3D
	{
		class COLLISION_ENTITY
		{
		protected:

			//-------------------------------------------------------------------------------------------
			class REPULSOR			// used to share repulsion data between the sectors
			{
			public:
				inline			REPULSOR(POINT2D *BasePos)		{ _pBasePos=BasePos; _nEdgeID=-1;}
				inline	int		GetEdge(void)					{ return _nEdgeID; }
				inline	void	AddIntersection(POINT2D &Point, int EdgeID, int Priority)
				{
					POINT2D Tmp = *_pBasePos - Point;
					float	SquareDist = Tmp.GetX() * Tmp.GetX() + Tmp.GetY() * Tmp.GetY();

					if(_nEdgeID != -1)
						if( (SquareDist >= _rSquareDist) || (Priority > _nPriority) )
							return;

					_nPriority		= Priority;
					_rSquareDist	= SquareDist;
					_nEdgeID		= EdgeID;
					Intersect		= Point;
				}

			public:
				POINT2D		Intersect;
			protected:
				int			_nEdgeID;
				int			_nPriority;
				float		_rSquareDist;
				POINT2D		*_pBasePos;
			};

			//-------------------------------------------------------------------------------------------
			class FAST_INTERSECT	// used to retain computations from the first coordinates set
			{
			public:
					enum INTERSECT_TYPE
					{
						IT_NO_INTERSECT		= 0x00000000,
						IT_ALMOST,
						IT_FULL,
						IT_FORCE_DWORD		= 0xffffffff
					};

								FAST_INTERSECT(POINT2D *v1, POINT2D *v2)		// Base line (typically the move vector)
								{
									_v1 = v1;
									_v2 = v2;
									_Diff = v1->GetX() - v2->GetX();
									_DiffY = v1->GetY() - v2->GetY();
									if(MATH::Abs(_Diff) < 0.0001f)
									{
										_IsVertical = true;
									}
									else
									{
										_a = (v1->GetY() - v2->GetY()) / _Diff;
										_b = v2->GetY() - _a * v2->GetX();
										_IsVertical = false;
									}
								}
						INTERSECT_TYPE	GetIntersection(POINT2D &v3, POINT2D &v4, POINT2D &Intersect, POINT2D &Normal);

			protected:		
				POINT2D	*_v1;
				POINT2D	*_v2;
				int		_IsVertical;
				float	_Diff;
				float	_DiffY;
				float	_a;
				float	_b;
			};

			//-------------------------------------------------------------------------------------------
		public:
			enum SEG_SIDE
			{
				SIDE_INSIDE			= 0,
				SIDE_P1,
				SIDE_P2,
				SIDE_PARALLEL,
				SIDE_FORCE_DWORD	= 0xffffffff
			};

			enum ENTITY_FLAGS
			{
				CET_NONE				= 0x00000000,
				CET_DYNAMIC				= 0x00000001,
				CET_STATIC				= 0x00000002,
				CET_DYNAMIC_REPULSION	= 0x00000004,
				CET_STATIC_REPULSION	= 0x00000008,

				CET_FORCE_DWORD			= 0xffffffff
			};

			enum ENTITY_WEIGHT
			{
				EW_LIGHT				= 0x00000000,
				EW_MEDIUM				= 0x00000001,
				EW_HEAVY				= 0x00000002,


				EW_ULTRA_HEAVY			= 0x00100000,	//  /!\ RESERVED TO THE COLISION INTERNAL SYSTEM /!\

				EW_FORCE_DWORD			= 0xffffffff
			};

		public:
										COLLISION_ENTITY();
										COLLISION_ENTITY(const COM_INSTANCE &NodeCI);
										~COLLISION_ENTITY();

			COLLISION_ENTITY &			operator=(const COLLISION_ENTITY & ci);

			inline	COLLISION_ENTITY	*GetNext(void)							{ return _pNext; }
			inline	void				SetNext(COLLISION_ENTITY *pNext)		{ _pNext = pNext; }
					void				SetNode(const COM_INSTANCE &NodeCI);
			inline	NODE				*GetNode(void)							{ return _pNode; }
			inline	const COM_INSTANCE	&GetNodeCI(void)						{ return _NodeInstance; }

					int					CollideAgainst(COLLISION_MESH *pColMesh);
					int					VerticalProcess(COLLISION_MESH *pColMesh, float &FoundAltitude, int iGPoint, const COM_INSTANCE &ColMeshNodeCI);
					int					GravityApplication(void);
					int					CollideAgainst(CM_SECTOR *pSector, COLLISION_MESH *pColMesh, FAST_INTERSECT &fastInter, REPULSOR &Repulsion);
					int					PostCollide(void);
					int					IsInBoundingBox(COLLISION_MESH *pColMesh);
					int					IsInBoundingBox(POINT2D &Origin, POINT2D &Max);

			inline	void				GetLastPos(POINT2D &Dest)		{ Dest = _LastPos; }
					void				ForcePosition(POINT2D &Pos);
			inline	void				ForceLastPos(POINT2D &Source)	{ _LastPos = Source; }

					int					IsPositionValid(COLLISION_MESH *pColMesh, POINT2D &outCloser, int &bFound);
			static	int					IsPositionValid(COLLISION_MESH *pColMesh, POINT2D &outCloser, int &bFound, VECTOR &NodePos);
			inline	int					GetFlags(void)							{ return _nFlags; }
			inline	void				SetFlags(int nNewFlags)					{ _nFlags = nNewFlags; }
			inline	void				RiseFlags(int nRisedFlags)				{ _nFlags |= nRisedFlags; }
			inline	void				LowerFlags(int nLoweredFlags)			{ _nFlags &= ~nLoweredFlags; }

			inline	void				ResetStaticTouchedState(void)			{ _bStaticTouched = false; if(_pSubEntity) _pSubEntity->ResetStaticTouchedState(); }
			inline	void				SetStaticTouched(void)					{ _bStaticTouched = true; }
			inline	int					IsStaticTouched(void)					{ return _bStaticTouched; }
			inline	int					GetNbStaticOccurences(void)				{ return _nbStaticOccurences; }
					void				GenerateStaticColEvent(void);
					void				GenerateDynamicColEvent(COLLISION_ENTITY *pAgainst);
			inline	void				ResetStaticColEvent(void)				{ _nbStaticOccurences = 0; }
			inline	int					IsActive(void)							{ return _IsActive; }
			inline	void				SetActivity(int bOnOff)					{ _IsActive = bOnOff; }
					COLLISION_MESH		*GetLastPosColMesh(void);
			inline	COM_INSTANCE		&GetLastPosColMeshCI(void)				{ return _LastPosColMeshCI; }
			inline	void				SetLastColMesh(const COM_INSTANCE &MeshCI)	{ _LastPosColMeshCI = MeshCI; }

			inline	void				ForceProcessOccured(void)				{ _nProcessOccured = true; }
					void				PrepareProcessing(void);

					void				PostVerticalProcessing(int bDoRepositioning, float YPos, float LowestAltFound);

			inline	void				FindSGCI(void)							{ _NodeSG = _pNode->GetSceneGraphNodeInst(); }
			inline	void				ReleaseZoneCI(void)						{ _NodeSG.Release(); }
					int					GetNbGroundPoints();
					VECTOR				*GetGroundPoints(int &nbPoints);
			inline	void				SetContactPoint(int iG, VECTOR & Contact)	{ _LastContacts[iG] = Contact; }
			inline	void				SetNodeAsGroundPoint(int bOnOff)		{ _bNodeProcessedAsGroundPoint = bOnOff; }
			inline	int					IsNodeActivatedAsGroundPoint(void)		{ return _bNodeProcessedAsGroundPoint; }
					int					ProcessSubEntity(COLLISION_MESH *pColMesh);

			inline	void				GetSecurePosition(VECTOR & Pos)		{ Pos.SetX(_LastPos.GetX()); Pos.SetZ(_LastPos.GetY()); }
					float				GetGroundAltitude(void);

					void				UpdateForPanicMode(void);

				// Dynamic Collision
					int					DynamicCollide(COLLISION_ENTITY *pAgainst);
					int					DynamicStepProcess(COLLISION_ENTITY *pAgainst);
					float				GetDynamicVolumeMaximalStep(int &Priority);
					int					GetRepulsionVector(CYLINDER &pCyl, CYLINDER &pCyl2, POINT2D &Repulsion);
					int					GetRepulsionVector(CYLINDER &pCyl, OBB &pObb, POINT2D &Repulsion);
			inline	ENTITY_WEIGHT		GetWeight(void)								{ return _nWeight; }
			inline	void				SetWeight(ENTITY_WEIGHT nWeight)			{ _nWeight = nWeight; }
			_INLINE_ void				SetDebugInfo(const QDT_STRING & sDebugInfo)	{ _sDebugInfo = sDebugInfo; }
			_INLINE_ const QDT_STRING & GetDebugInfo() const						{ return	(_sDebugInfo); }

			inline	void				ResetSliding(void)							{ _bSlideFreezed = false; if(_pSubEntity) _pSubEntity->ResetSliding(); }
			inline	int					IsGravityEnabled(void)						{ return _bGravityEnabled; }
			inline	void				SetGravityProcessing(int bOnOff)			{ _bGravityEnabled = bOnOff; }
			inline	void				SetDynamicVolumeNode(COM_INSTANCE &NodeCI)	{ _DynamicMeshNode = NodeCI; }
					int					IsAttachedToSceneGraph(void);
			inline	bool				IsLoaded(void)								{ return _NodeInstance.IsLoaded(); }

					// Agatha power patch de la mort qui tue la mer morte.
					void				SetSecondaryNode(COM_INSTANCE &SecondaryNode);
					void				ReleaseSecondaryNode(void);

					// Teleportation
					void				UpdateForTeleport(void);

					// Notification
					void				SetNotifier(const COLLISION_NOTIFIER *pNotifier);

					// Tests
					int					GetClosestPointTo(POINT2D &v1, POINT2D &v2, POINT2D &BasePos, POINT2D &Normal, POINT2D &Closest);
			static	int					GetIntersectionPoint(POINT2D &v1, POINT2D &v2, POINT2D &BasePos, POINT2D &Normal, POINT2D &Intersect);
			static	SEG_SIDE			IsPointOnSegment(POINT2D &v1, POINT2D &v2, POINT2D &Point);
			static	int					GetGroundAltitude(COLLISION_MESH *pColMesh, VECTOR &TestPt, float &fOutAlt, const COM_INSTANCE &ColMeshNodeCI);
					BOUNDING_VOLUME		*GetTransformedBoundingVolume(void);

					// Camera System
					_INLINE_ void				SetProcessingCollisionForCamSystem(const bool & bStatus) { _bEnableProcessCam = bStatus; };
					_INLINE_ const bool			IsProcessingCollisionForCamSystem() const { return _bEnableProcessCam; };

		protected:
					void				SetNewPosition( const VECTOR& NodePos, NODE *pNode=0 );
			static	int					GetClosestPointTo(COLLISION_MESH *pColMesh, const VECTOR &NodePos, CM_SECTOR *pSector, POINT2D &Intersect, int &iOutEdge, float &Score);
			static	int					GetClosestPointTo(COLLISION_MESH *pColMesh, const VECTOR &NodePos, int iSX, int iSY, int iScan, POINT2D &Point, int &iOutEdge);
					int					ProcessSaturation(CM_EDGE &Edge, POINT2D &Normal, POINT2D &vSat, int VtxID);
					void				UpdateBoundingBox(float fRadius);
					int					ProcessRepulsion(COLLISION_MESH *pColMesh, REPULSOR &Repulsion);
			static	int					VerticalRayCast(COLLISION_MESH *pColMesh, CM_FACE &Face, VECTOR &Ray, float &fAlt);
			inline	int					IsSlidingFreezed(void)			{ return _bSlideFreezed; }
			const	BOUNDING_VOLUME		*GetBoundingVolume(void);
					MATRIX				&GetBoundingVolumeGlobalMatrix(void);

		public:
			float					_YSpeed;
			float					_YAccel;

		protected:
			// Gravity emulation
			int						_bGravityEnabled;
			int						_bNodeProcessedAsGroundPoint;

			// Tops generator
			int						_GPointsState[MAX_GROUND_POINTS];
			//float					_GPointsAlt[MAX_GROUND_POINTS];
			VECTOR					_LastContacts[MAX_GROUND_POINTS];

			// Activity
			int						_IsActive;

			// Bounding Box
			POINT2D					_bbMin;
			POINT2D					_bbMax;

			// Repulsion
			int						_nLoops;
			POINT2D					_MoveVector;
			int						_nProcessOccured;

			// Agatha power patch de la mort qui tue la mer morte.
			COLLISION_ENTITY		*_pSubEntity;
			int						_nInternalType;

			// Instance Datas
			POINT2D					_RealLastPos;
			POINT2D					_LastPos;
			MATRIX					_LastMtx;
			POINT2D					_CurPos;
			float					_SquareMoveLen;
			COM_INSTANCE			_NodeSG;
			COM_INSTANCE			_NodeInstance;		// Here for referencements purposes
			COM_INSTANCE			_DynamicMeshNode;	// Will be set if it differs from _NodeInstance
			NODE					*_pNode;			// Will give us faster acces to target object
			COLLISION_ENTITY		*_pNext;			// Chained list management, simple list
			int						_nFlags;
			ENTITY_WEIGHT			_nWeight;
			int						_bSlideFreezed;
			int						_bStaticTouched;
			int						_nbStaticOccurences;
			COLLISION_NOTIFIER		*_pNotifier;
			COM_INSTANCE			_LastPosColMeshCI;
			VECTOR					_NodePos;
			QDT_STRING				_sDebugInfo;
			CYLINDER				_Cyl;
			OBB						_Obb;

			// Camera System
			bool					_bEnableProcessCam;
			bool					_Padding[15];
		};
	}
}

#endif // __M3D_COLLISION_COLLISIONENTITY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
