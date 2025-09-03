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
//	CLASS:	COLLISION_3D_STRUCT
//
//	05-04-29:	ELE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"include.h"

#include	INC_KRNCORE(Display/GeometryABC)

#include	INC_3DENGINE(SceneGraph/Entities/MeshGroup)

#ifdef	_DEBUG
	#include	"Collision3DStruct.inl"
#else
	#include	INL_KRNCORE(Display/GeometryABC)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Plane)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Triangle)

	#include	"Inline.h"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Collide
//	Object:		
//	05-05-02:	ELE - Created
//-----------------------------------------------------------------------------
void	COLLISION_3D_STRUCT::Collide()
{
	NODE	*pNode = static_cast<NODE *>(_NodeInst.GetCommunicator());
	VECTOR	NodePosition, NewNodePosition;

	pNode->UpdateGlobalMatrix();
	pNode->GetGlobalPosition(NodePosition);

	for	(int i = 0 ; i < 3 ; ++i)
	{
		if	(Collide(NodePosition, NewNodePosition))
		{
			NodePosition = NewNodePosition;
		}
		else
		{
			break;
		}
	}

	pNode->SetGlobalPosition(NewNodePosition);
	_OldPos = NewNodePosition;
}

//-----------------------------------------------------------------------------
//	Name:		Collide
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	COLLISION_3D_STRUCT::Collide(const VECTOR &	NodePosition,
									 VECTOR	&		NewNodePosition)
{
	NODE				*pNode = static_cast<NODE *>(_NodeInst.GetCommunicator());
	const COM_INSTANCE	&SGNodeInst = pNode->GetSceneGraphNodeInst();

	NewNodePosition = NodePosition;

	if	(SGNodeInst.HasHandle())
	{
		NODE				*pChild, *pSGChild, *pSGNode = static_cast<NODE *>(SGNodeInst.GetCommunicator());
		int					i, nNbrChildren = pSGNode->GetNbrChildren();
		ENTITY_ABC			*pEntity;
		MESH_GROUP			*pMeshGroup;
		RAY					Ray;
		VECTOR				Direction;
		float				rDistance;
		bool				bContinue;

		Direction.Sub(NodePosition, _OldPos);

		if	(Direction.IsNull() == false)
		{
			rDistance = Direction.Normalize();
			Ray.SetOrigin(_OldPos);
			Ray.SetDirection(Direction);

			RAY_CAST_PARAMETERS	RayCastParam;
			RayCastParam.SetRay(Ray);
			RayCastParam.SetFlags(RAY_CAST_PARAMETERS::CULLING, RAY_CAST_PARAMETERS::CULLING);
			RayCastParam.SetImpactDistance(rDistance * 2.0f);

			for	(i = 0 ; i < nNbrChildren ; ++i)
			{
				pSGChild = pSGNode->GetChild(i);
				pEntity = pSGChild->GetEntity();

				if	(pEntity->GetEntityType() == ENTITY_ZONE)
				{
					int	j, nNbrZoneChildren = pSGChild->GetNbrChildren();

					for	(j = 0 ; j < nNbrZoneChildren ; ++j)
					{
						pChild = pSGChild->GetChild(j);
						pEntity = pChild->GetEntity();

						if	(pEntity->GetEntityType() == ENTITY_MESH_GROUP)
						{
							pMeshGroup = static_cast<MESH_GROUP *>(pEntity);

							if	(pMeshGroup->RayCast(RayCastParam, bContinue))
							{
								const	TRIANGLE	&ImpactTriangle = RayCastParam.GetImpactTriangle();
								VECTOR				ProjectPos, ExtrudeDirection;
								PLANE				ProjectionPlane;

								ProjectionPlane.SetFromTriangle(ImpactTriangle);
								ProjectionPlane.Project(NodePosition, ProjectPos);
								
								ExtrudeDirection.Sub(NodePosition, ProjectPos);
								rDistance = ExtrudeDirection.Normalize();

								if	((rDistance < _rRadius) || (ProjectionPlane.GetSide(NodePosition) != ProjectionPlane.GetSide(_OldPos)))
								{
									ProjectionPlane.SetD(ProjectionPlane.GetD() - _rRadius);
									ProjectionPlane.Project(NodePosition, NewNodePosition);
									return	(true);
								}
							}

							VECTOR	VSide(1.0f, 0.0f, 0.0f);
							pNode->GetGlobalMatrix().TransformDirection(VSide);
							VSide.Normalize();

							for	(int k = 0 ; k < 2 ; ++k)
							{
								Ray.SetDirection(VSide);

								RAY_CAST_PARAMETERS	RayCastParam;
								RayCastParam.SetRay(Ray);
								RayCastParam.SetFlags(RAY_CAST_PARAMETERS::CULLING, RAY_CAST_PARAMETERS::CULLING);
								RayCastParam.SetImpactDistance(_rRadius * 2.0f);

								if	(pMeshGroup->RayCast(RayCastParam, bContinue))
								{
									const	TRIANGLE	&ImpactTriangle = RayCastParam.GetImpactTriangle();
									VECTOR				ProjectPos, ExtrudeDirection;
									PLANE				ProjectionPlane;

									ProjectionPlane.SetFromTriangle(ImpactTriangle);
									ProjectionPlane.Project(NodePosition, ProjectPos);
									
									ExtrudeDirection.Sub(NodePosition, ProjectPos);
									rDistance = ExtrudeDirection.Normalize();

									if	((rDistance < _rRadius) || (ProjectionPlane.GetSide(NodePosition) != ProjectionPlane.GetSide(_OldPos)))
									{
										ProjectionPlane.SetD(ProjectionPlane.GetD() - _rRadius);
										ProjectionPlane.Project(NodePosition, NewNodePosition);
										return	(true);
									}
								}

								VSide = -VSide;
							}

							VECTOR	VTop(0.0f, 1.0f, 0.0f);
							pNode->GetGlobalMatrix().TransformDirection(VTop);
							VTop.Normalize();

							for	(int k = 0 ; k < 2 ; ++k)
							{
								Ray.SetDirection(VTop);

								RAY_CAST_PARAMETERS	RayCastParam;
								RayCastParam.SetRay(Ray);
								RayCastParam.SetFlags(RAY_CAST_PARAMETERS::CULLING, RAY_CAST_PARAMETERS::CULLING);
								RayCastParam.SetImpactDistance(_rRadius * 2.0f);

								if	(pMeshGroup->RayCast(RayCastParam, bContinue))
								{
									const	TRIANGLE	&ImpactTriangle = RayCastParam.GetImpactTriangle();
									VECTOR				ProjectPos, ExtrudeDirection;
									PLANE				ProjectionPlane;

									ProjectionPlane.SetFromTriangle(ImpactTriangle);
									ProjectionPlane.Project(NodePosition, ProjectPos);
									
									ExtrudeDirection.Sub(NodePosition, ProjectPos);
									rDistance = ExtrudeDirection.Normalize();

									if	((rDistance < _rRadius) || (ProjectionPlane.GetSide(NodePosition) != ProjectionPlane.GetSide(_OldPos)))
									{
										ProjectionPlane.SetD(ProjectionPlane.GetD() - _rRadius);
										ProjectionPlane.Project(NodePosition, NewNodePosition);
										return	(true);
									}
								}

								VTop = -VTop;
							}
						}
					}
				}
			}
		}
	}

	return	(false);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
