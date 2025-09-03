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
//	CLASS:	COLLISION_MESH
//	The COLLISION_MESH class implements ...
//
//	03-08-07:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(CM_SECTOR)
DECL_3DENGINE(CM_EDGE)
DECL_3DENGINE(CM_FACE)
DECL_KMATH(VECTOR)
DECL_KMATH(AABB)
DECL_KMATH(COLOR_FLOAT)
DECL_KMATH(SEGMENT)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

#define MAX_SEG 50

namespace QDT
{
	namespace M3D
	{
		class	COLLISION_MESH : public ENTITY_ABC
		{
		public:
			enum Y_REPLUSION_MODE
			{
				YRM_NONE,
				YRM_YLOCKED,
				YRM_RAYCAST,

				YRM_FORCEDWORD	= 0xffffffff
			};

			enum CM_RETURN_CODES
			{
				CMRC_CORRECTION_DONE_RESULT_SURE,
				CMRC_CORRECTION_DONE_RESULT_UNSURE,
				CMRC_NO_CORRECTION_DONE,

				CMRC_FORCEWDWORD						= 0xffffffff
			};

		public:

			// Constructor & destructor
												COLLISION_MESH();
												~COLLISION_MESH();

		// Dibugeuh
			static inline void					SetDebugDisplayMode(int nDisplayMode)	{ _nDebugDisplayMode = nDisplayMode; }
			static inline int					GetDebugDisplayMode(void)				{ return _nDebugDisplayMode; }

			// Setup
					void						SetNbFaces(int nbFaces);
					void						SetNbVertices(int nbVertices);
					void						SetNbEdges(int nbEdges);
					void						SetNbFaceVtx(int nbFVtx);
					void						SetNbSteps(int StepsX, int StepsY);
			inline	void						SetRadius(float fRadius)		{ _fRadius	= fRadius;		}
			inline	void						SetOrigin(POINT2D Origin)		{ _Origin	= Origin;		}
			inline	void						SetMax(POINT2D Max)				{ _Max		= Max;			}
			inline	void						SetStepSize(float StepSize)		{ _StepSize = StepSize;		}
			inline	CM_SECTOR					**GetSectors(void)				{ return _pSectors;			}
			inline	POINT2D						*GetVertices(void)				{ return _pVertices;		}
			inline	VECTOR						*GetFaceVtx(void)				{ return _pFVtx;			}
			inline	CM_EDGE						*GetEdges(void)					{ return _pEdges;			}
			inline	CM_FACE						*GetFaces(void)					{ return _pFaces;			}
			inline	POINT2D						*GetEdgesNormals(void)			{ return _pEdgesNormals;	}
			inline	float						GetStepSize(void)				{ return _StepSize;			}
			inline	int							GetNbStepsX(void)				{ return _StepsX;			}
			inline	int							GetNbStepsY(void)				{ return _StepsY;			}
			inline	POINT2D						&GetOrigin(void)				{ return _Origin;			}
			inline	POINT2D						&GetMax(void)					{ return _Max;				}
			inline	float						GetRadius(void)					{ return _fRadius;			}
			inline	Y_REPLUSION_MODE			GetRepulsionMode(void)			{ return _nYRepMode;		}
			inline	void						SetRepulsionMode(Y_REPLUSION_MODE RepMode) { _nYRepMode = RepMode;}
			inline	float						GetYLock(void)					{ return _fYLock;			}
			inline	void						SetYLock(float fYLock)			{ _fYLock = fYLock;			}
			inline	int							GetVersion(void)				{ return _nVersion;			}
					int							IsPointInside(VECTOR &pt);
					int							IsPointInside(POINT2D &pt);
					int							VerticalRayCast(VECTOR &Source, float &fAlt);
					CM_RETURN_CODES				CorrectPosition(VECTOR &Source, VECTOR &Dest, float Dir, int &bMerged, float &rMinResizeAllowed);
					void						ClearUsedFlags(VECTOR &Center, float rRadius);
					void						ExcludeCameraSectors( AABB *pAABB);
					void						ResetCameraSectors(void);

			// Debug
		#ifndef _MASTER
					static void					AddLineToDebugDisplay(SEGMENT &Segment, COLOR_FLOAT &Color);
					void						DebugSetSectors(int s1x, int s1y, int s2x, int s2y);
					void						DebugSetEdges(int EdgeID1, int EdgeID2);
		#endif


			// ENTITY_ABC
			virtual void						Display(DISPLAY_PARAMETERS & DisplayParameters);
			virtual bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

			// Communication
			static	void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static	COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static	COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();
				
		protected:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);

		#ifndef _MASTER_ONLY_DATABANK
			static	void						CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void						CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void						CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void						CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static	void						CM_WriteChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

		protected:
			static	int							_nDebugDisplayMode;

					// Collision Datas
					VECTOR						*_pFVtx;
					POINT2D						*_pVertices;
					POINT2D						*_pEdgesNormals;
					CM_EDGE						*_pEdges;
					CM_FACE						*_pFaces;

					int							_nbVertices;
					int							_nbEdges;
					int							_nbFaces;
					int							_nbFVtx;

					Y_REPLUSION_MODE			_nYRepMode;
					float						_fYLock;

					float						_fRadius;

					// Sector Datas
					float						_StepSize;
					POINT2D						_Origin;
					POINT2D						_Max;
					CM_SECTOR					**_pSectors;
					int							_StepsX;
					int							_StepsY;

		#ifndef _MASTER
					// Debug
					static	SEGMENT				_SegmentListTbl[MAX_SEG];
					static	COLOR_FLOAT			_SegmentTblColors[MAX_SEG];
					static	int					_nUsedSegments;
		#endif
					int 						_s1x;
					int 						_s1y;
					int 						_s2x;
					int 						_s2y;
					int 						_EdgeID1;
					int 						_EdgeID2;

					int							_nVersion;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
