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
//	CLASS:	MESH_GROUP
//
//	03-09-01:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/MeshGroup)
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/MeshGroupGeometry)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include	"MeshGroup.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP constructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP::MESH_GROUP()
:
ENTITY_ABC(GetGenericCommunicatorDescriptor(), ENTITY_MESH_GROUP),
_nNbrSharedVertexBuffers(0),
//_pSharedVertexBuffers(NULL),
_nNbrGeometries(0),
_pGeometries(NULL),
_pbVisibleGeometries(NULL),
_pStaticActor(NULL),
_pMeshData(NULL),
_pCollisionVertices(NULL),
_pCollisionIndices(NULL),
_nNbrCollisionVertices(0),
_nNbrCollisionIndices(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP destructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP::~MESH_GROUP()
{
	PHYSIC_CONTEXT::Instance()->ReleaseActor(_pStaticActor);
	_pStaticActor = NULL;
	_pMeshData = NULL;

//	delete [] _pSharedVertexBuffers;
	delete [] _pGeometries;
	delete [] _pbVisibleGeometries;

	_pGeometries = NULL;
	_pbVisibleGeometries = NULL;

	delete [] _pCollisionVertices;
	_pCollisionVertices = NULL;

	delete [] _pCollisionIndices;
	_pCollisionIndices = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		DisplayHdrObject
//	Object:		
//	04-05-19:	FPR - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::DisplayHdrObject(DISPLAY_PARAMETERS &	DisplayParameters,
									 unsigned int			nGeometryIndex,
									 unsigned int			nSkinClusterIndex)
{	
	QDT_NOT_IMPLEMENTED();
/*
	HDR_OBJECT HDR_object;
	RENDER_CONTEXT *  pRC = RENDER_MANAGER::Instance()->GetRenderContext();

	pRC->SetWorldTransform0(NULL);
	pRC->EnableVertexBlending(3, false);

	QDT_ASSERT(nGeometryIndex < _nNbrGeometries);

	_pGeometries[nGeometryIndex].DisplayGeometry(DisplayParameters);*/
}

//-----------------------------------------------------------------------------
//	Name:		DisplayTrans
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::DisplayTrans(DISPLAY_PARAMETERS &	DisplayParameters,
								 unsigned int			nGeometryIndex,
								 unsigned int			nSkinClusterIndex)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT *  pRC = RENDER_MANAGER::Instance()->GetRenderContext();

	pRC->SetWorldTransform0(NULL);
	pRC->EnableVertexBlending(3, false);

	QDT_ASSERT(nGeometryIndex < _nNbrGeometries);

	_pGeometries[nGeometryIndex].DisplayGeometry(DisplayParameters);*/
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	unsigned int i;
	static COLOR_FLOAT Green(1.0f, 0.0f, 1.0f, 0.0f);
	
	RENDER_CONTEXT *  pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	DISPLAY_MANAGER * pDM = DISPLAY_MANAGER::Instance();
	
//	pRC->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
	pRC->SetWorldTransform0(NULL);

	for (i = 0 ; i < _nNbrGeometries ; ++i)
	{
		if (_pbVisibleGeometries[i])
		{
			_pGeometries[i].Display(i, DisplayParameters);

#ifndef _MASTER
			if (pDM->DisplayBoundingBoxes())
			{
				_pGeometries[i].GetBoundingBox().Display(Green);
			}
#endif
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		DisplaySorted
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::DisplaySorted(DISPLAY_PARAMETERS &	DisplayParameters,
								  unsigned int			nGeometry)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT *  pRC = RENDER_MANAGER::Instance()->GetRenderContext();

	pRC->EnableVertexBlending(3, false);
//	pRC->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
	pRC->SetWorldTransform0(NULL);

	_pGeometries[nGeometry].DisplaySorted(DisplayParameters);*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-09-22:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	bool bRes = false;
	unsigned int i;

	for (i = 0 ; i < _nNbrGeometries ; ++i)
	{
		_pbVisibleGeometries[i] = _pGeometries[i].IsVisible(DisplayParameters);
		bRes |= _pbVisibleGeometries[i];
	}

	return (bRes);
}

//-----------------------------------------------------------------------------
//	Name:		TestGeometriesVisibility
//	Object:		
//	03-09-22:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP::TestGeometriesVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return (TestVisibility(DisplayParameters));
}
/*
//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	03-09-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP::RayCast(RAY_CAST_PARAMETERS &	RayCastParameters,
							bool &					bContinue)
{
	bContinue = true;

	if	(_pMeshData == false)
	{
		return	(false);
	}

	I_PHYSIC_RAYCAST_HIT *	pRaycastHit = PHYSIC_CONTEXT::Instance()->CreateRaycastHit();
	bool	bTouched = _pMeshData->Raycast(RayCastParameters.GetRay(), QDT_FLOAT32_MAX, 0xffffffff, pRaycastHit, false);

	if	(bTouched)
	{
		float	rImpactDistance = pRaycastHit->GetImpactDistance();
		if	(rImpactDistance > RayCastParameters.GetImpactDistance())
		{
			delete	(pRaycastHit);
			return	(false);
		}

		VECTOR	ImpactNormal;
		pRaycastHit->GetImpactNormal(ImpactNormal);

		RayCastParameters.SetImpactDistance(rImpactDistance);
		RayCastParameters.SetImpactNormal(ImpactNormal);

		delete	(pRaycastHit);
		return	(true);
	}

	delete	(pRaycastHit);
	return (false);
}
*/
//-----------------------------------------------------------------------------
//	Name:		AllocateMemory
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CreateGeometries()
{
	QDT_NOT_IMPLEMENTED();
/*
	unsigned int i;
	RENDER_CONTEXT * pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();

	QDT_ASSERT(_nNbrGeometries > 0);

	_pbVisibleGeometries = new bool [_nNbrGeometries];

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		_pGeometries[i].SetGeometry(pRenderContext->AllocateGeometry());
		_pbVisibleGeometries[i] = false;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrGeometries
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetNbrGeometries(unsigned int nCount)
{
	QDT_ASSERT(nCount > 0);

	delete [] _pGeometries;

	_pGeometries = new MESH_GROUP_GEOMETRY[nCount];
	_nNbrGeometries = nCount;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrSharedVertexBuffer
//	Object:		
//	04-06-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetNbrSharedVertexBuffers(unsigned int nCount)
{
	QDT_NOT_IMPLEMENTED();
/*
	if (nCount > 0)
	{
		delete [] _pGeometries;
		_pSharedVertexBuffers = new GEOMETRY_ABC * [nCount];
		_nNbrSharedVertexBuffers = nCount;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 0,CM_ReadChunk0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 1,CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 2,CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 3,CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 4,CM_ReadChunk4);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 5,CM_ReadChunk5);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MSHGROUP", 6,CM_ReadChunk6);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("MSHGROUP", 6, CM_WriteChunk6);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MESH_GROUP::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_GROUP) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MESH_GROUP::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("MESH_GROUP"),						
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_GROUP,	
										QDT_COMMENT("69F9851E-C8CB-AE71-9B7B-EA67EB7F8EA5"),			
										sizeof(MESH_GROUP),								
										ComInit,										
										QDT_COMMENT("Class which groups all static meshes"),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	MESH_GROUP *pMG;
	pMG = static_cast<MESH_GROUP *>(pO);

	QDT_NEW_PLACED(pMG, MESH_GROUP)();
	QDT_ASSERT(pMG);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	MESH_GROUP *pMG;
	pMG = static_cast<MESH_GROUP *>(pO);

	QDT_DELETE_PLACED(pMG, MESH_GROUP);
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk0
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk0(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk1(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk2(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	04-04-14:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk3(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	04-04-14:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk4(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	QDT_FAIL();
	}
	
//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk5
//	Object:		
//	04-04-14:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk5(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
	{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk6
//	Object:		
//	04-04-14:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::CM_ReadChunk6(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	unsigned int	i, nNbrGeometries, nChunkVersion = 9, nNbrSharedVertexBuffers, nSharedVBIndex;
	COM_INSTANCE ciMaterial;
	MESH_GROUP * pMeshGroup = static_cast<MESH_GROUP *>(pCommunicator);
	AABB BoundingBox;
	SPHERE BoundingSphere;
	bool bDoubleSided, bCullingOnShadows;

	Stream >> nNbrSharedVertexBuffers;
	pMeshGroup->SetNbrSharedVertexBuffers(nNbrSharedVertexBuffers);
	for (i = 0 ; i < nNbrSharedVertexBuffers ; ++i)
	{
//		RENDER_CONTEXT * pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
//		pMeshGroup->_pSharedVertexBuffers[i] = pRenderContext->AllocateGeometry();
//		pMeshGroup->_pSharedVertexBuffers[i]->SetVersion(nChunkVersion);
//		pMeshGroup->_pSharedVertexBuffers[i]->ComputeCopyVertices(false);
//		pMeshGroup->_pSharedVertexBuffers[i]->Read(Stream);
//		pMeshGroup->_pSharedVertexBuffers[i]->SetIsSharedVertexBuffer(true);
	}
	
	Stream >> nNbrGeometries;
	pMeshGroup->SetNbrGeometries(nNbrGeometries);
	pMeshGroup->CreateGeometries();
	
	for (i = 0 ; i < pMeshGroup->GetNbrGeometries() ; ++i)
	{
		MESH_GROUP_GEOMETRY & MGGeometry = pMeshGroup->_pGeometries[i];

		Stream >> ciMaterial;
		MGGeometry.SetMaterial(ciMaterial);

		Stream >> BoundingBox;
		MGGeometry.SetBoundingBox(BoundingBox);

		Stream >> BoundingSphere;
		MGGeometry.SetBoundingSphere(BoundingSphere);

		Stream >> bDoubleSided;
		MGGeometry.SetDoubleSided(bDoubleSided);

		Stream >> bCullingOnShadows;
		MGGeometry.SetCullingOnShadowing(bCullingOnShadows);

//		MGGeometry.GetGeometry()->SetVersion(nChunkVersion);
//		MGGeometry.GetGeometry()->ComputeCopyVertices(false);
//		MGGeometry.GetGeometry()->Read(Stream);

		Stream >> nSharedVBIndex;

		MGGeometry.SetSharedVBIndex(nSharedVBIndex);

//		if (pMeshGroup->_pSharedVertexBuffers)
//		{
//			MGGeometry.SetSharedVB(pMeshGroup->_pSharedVertexBuffers[nSharedVBIndex]);
//		}
	}
	Stream >> pMeshGroup->_BoundingBox;

	//pMeshGroup->_Tree.Read2(Stream);
	Stream >> pMeshGroup->_nNbrCollisionVertices;
	Stream >> pMeshGroup->_nNbrCollisionIndices;

	pMeshGroup->_pCollisionVertices = new float[pMeshGroup->_nNbrCollisionVertices*3];
	for (i = 0 ; i < pMeshGroup->_nNbrCollisionVertices ; ++i)
	{
		Stream >> pMeshGroup->_pCollisionVertices[i * 3 + 0];
		Stream >> pMeshGroup->_pCollisionVertices[i * 3 + 1];
		Stream >> pMeshGroup->_pCollisionVertices[i * 3 + 2];
	}

	pMeshGroup->_pCollisionIndices = new unsigned int[pMeshGroup->_nNbrCollisionIndices];
	for (i = 0 ; i < pMeshGroup->_nNbrCollisionIndices ; ++i)
	{
		Stream >> pMeshGroup->_pCollisionIndices[i];
	}

	pMeshGroup->_pStaticActor = PHYSIC_CONTEXT::Instance()->CreateStaticActor();
	pMeshGroup->_pMeshData = PHYSIC_CONTEXT::Instance()->CreateShapeMeshData();

	pMeshGroup->_pMeshData->SetNbrVertices(pMeshGroup->_nNbrCollisionVertices);
	pMeshGroup->_pMeshData->SetPoints(pMeshGroup->_pCollisionVertices);
	pMeshGroup->_pMeshData->SetNbrTriangles(pMeshGroup->_nNbrCollisionIndices / 3);
	pMeshGroup->_pMeshData->SetIndices(pMeshGroup->_pCollisionIndices);
	pMeshGroup->_pMeshData->GenerateDatas();

	delete [] pMeshGroup->_pCollisionVertices;
	pMeshGroup->_pCollisionVertices = NULL;

	delete [] pMeshGroup->_pCollisionIndices;
	pMeshGroup->_pCollisionIndices = NULL;

	pMeshGroup->_pStaticActor->PushShape(pMeshGroup->_pMeshData);
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk5
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	MESH_GROUP::CM_WriteChunk6(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	unsigned int	i, nChunkVersion = 9;
	MESH_GROUP * pMeshGroup;
	pMeshGroup = static_cast<MESH_GROUP *>(pCommunicator);

	Stream.DebugOutput("MESH_GROUP : ");
	Stream.DebugOutput(pMeshGroup->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Nbr Shared vertex buffer (PC & XBox only) :");
	Stream << pMeshGroup->GetNbrSharedVertexBuffers();
	Stream.DebugOutput("\r\n");

	for (i = 0 ; i < pMeshGroup->GetNbrSharedVertexBuffers() ; ++i)
	{
//		pMeshGroup->_pSharedVertexBuffers[i]->SetVersion(nChunkVersion);
//		pMeshGroup->_pSharedVertexBuffers[i]->Write(Stream);
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("Number of geometries : ");
	Stream << pMeshGroup->GetNbrGeometries();
	Stream.DebugOutput("\r\n");

	for (i = 0 ; i < pMeshGroup->GetNbrGeometries() ; ++i)
	{
		const MESH_GROUP_GEOMETRY & MGGeometry = pMeshGroup->_pGeometries[i];

		Stream.DebugOutput("Geometry ");
		Stream.DebugOutput((int)i);
		Stream.DebugOutput(" :\r\n");

		// Material
		Stream.DebugOutput("Material : ");
		Stream << MGGeometry.GetMaterial();
		if (MGGeometry.GetMaterial().HasHandle())
		{
			Stream.DebugOutput(MGGeometry.GetMaterial().GetName());
		}
		Stream.DebugOutput(" :\r\n");

		// Box
		Stream.DebugOutput("Bounding box : ");
		Stream << MGGeometry.GetBoundingBox();
		Stream.DebugOutput("\r\n");

		// Sphere
		Stream.DebugOutput("Bounding sphere : ");
		Stream << MGGeometry.GetBoundingSphere();
		Stream.DebugOutput("\r\n");

		// Double sided
		Stream.DebugOutput("Double sided : ");
		Stream << MGGeometry.IsDoubleSided();
		Stream.DebugOutput("\r\n");

		// Backface Culling On Dynamic shadows
		Stream.DebugOutput("Backface Culling On Dynamic Shadows : ");
		Stream << MGGeometry.DoShadowCulling();
		Stream.DebugOutput("\r\n");

		// Geometry
		Stream.DebugOutput("Geometry\r\n");
//		pMeshGroup->_pGeometries[i].GetGeometry()->SetVersion(nChunkVersion);
//		pMeshGroup->_pGeometries[i].GetGeometry()->Write(Stream);
		Stream.DebugOutput("\r\n");

		// Shared vertex buffer index
		Stream.DebugOutput("Shared vertex buffer index\r\n");
		Stream << MGGeometry.GetSharedVBIndex();
		Stream.DebugOutput("\r\n");
	}

	Stream << pMeshGroup->_BoundingBox;

	// Write collision datas
	Stream << pMeshGroup->_nNbrCollisionVertices;
	Stream << pMeshGroup->_nNbrCollisionIndices;

	for (i = 0 ; i <  pMeshGroup->_nNbrCollisionVertices * 3 ; ++i)
	{
		Stream << pMeshGroup->_pCollisionVertices[i];
	}

	for (i = 0 ; i < pMeshGroup->_nNbrCollisionIndices ; ++i)
	{
		Stream << pMeshGroup->_pCollisionIndices[i];
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetDistanceFromCamera
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
float	MESH_GROUP::GetDistanceFromCamera(unsigned int nGeometry,
										  NODE * pNode)
{
	return (_pGeometries[nGeometry].GetDistanceFromCamera());
}
//-----------------------------------------------------------------------------
//	Name:		SetNbrCollisionVertices
//	Object:		
//	05-02-16:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetNbrCollisionVertices(unsigned int nNbrCollisionVertices)
{
	QDT_ASSERT(_pCollisionVertices == NULL);
	_nNbrCollisionVertices = nNbrCollisionVertices;
	_pCollisionVertices = new float[_nNbrCollisionVertices * 3];
}	

//-----------------------------------------------------------------------------
//	Name:		SetNbrCollisionIndices
//	Object:		
//	05-02-16:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetNbrCollisionIndices(unsigned int nNbrCollisionIndices)
{
	QDT_ASSERT(_pCollisionIndices == NULL);
	_nNbrCollisionIndices = nNbrCollisionIndices;
	_pCollisionIndices = new unsigned int [_nNbrCollisionIndices];
}

//-----------------------------------------------------------------------------
//	Name:		SetCollisionVertices
//	Object:		
//	05-02-16:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetCollisionVertices(float * pVertices)
{
	for (unsigned int i = 0 ; i < _nNbrCollisionVertices * 3; ++i)
	{
		_pCollisionVertices[i] = pVertices[i];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCollisionIndices
//	Object:		
//	05-02-16:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetCollisionIndices(unsigned int * pIndices)
{
	for (unsigned int i = 0 ; i < _nNbrCollisionIndices ; ++i)
	{
		_pCollisionIndices[i] = pIndices[i];
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

