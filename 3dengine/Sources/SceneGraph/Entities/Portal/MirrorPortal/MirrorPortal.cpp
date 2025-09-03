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
// CLASS: MIRROR_PORTAL
//
//
//	01-03-29:	ELE - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__ ;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION (MIRROR_PORTAL)
//
//	The MIRROR_PORTAL class implements ...
//
//*****************************************************************************

#include	"..\include.h"

#include	INC_KRNCORE(Stream)
#include	INC_KRNCORE(Display\RenderPipeline)
#include	INC_KRNCORE(Display\GeometryManager)

#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)
#include	INC_3DENGINE(Display)

#ifdef _DEBUG
	#include	"MirrorPortal.inl"
#else
	#include	"../Inline.h"

	#include	INL_KRNCORE(Display/RenderPipeline)
	#include	INL_KRNCORE(Display/GeometryABC)
	#include	INL_KRNCORE(Display/RenderManager)

	#include	INL_3DENGINE(Display)
	#include	INL_3DENGINE(SceneGraph/Node)
	#include	INL_3DENGINE(SceneGraph/Entities/Mesh)

	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Ray)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/More)
	#include	INL_KRNCORE(Math/Color)
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

void*									MIRROR_PORTAL::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		MIRROR_PORTAL constructor
//	Object:		Default constructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
MIRROR_PORTAL::MIRROR_PORTAL()
:
PORTAL_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_MIRROR_PORTAL)
{

	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MIRROR_PORTAL destructor
//	Object:		Destructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
MIRROR_PORTAL::~MIRROR_PORTAL()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		Derivation from ENTITY_ABC : Display
//	01-08-06:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();

	if	( (pDisplay->IsMirrorsEnabled() == false) ||
		  (pDisplay->DisplayMirrors() && IsOnlyDynamicsReflexionsActivated()) )
	{
		int						i;
		bool					bOneBitAlpha;
		GEOMETRY_ABC *			pGeometry;
		COM_INSTANCE *			pMaterialInst;
		MATERIAL_ABC *			pMaterial;
		SHADER_ABC *			pShader;
		RENDER_CONTEXT *		pRC = RENDER_MANAGER::Instance()->GetRenderContext();
		RENDER_PIPELINE_ABC	*	pRP = pRC->GetRenderPipeline();

		for	(i = 0 ; i < _nNbrGeometries ; ++i)
		{
			pGeometry = _pGeometriesArray[i];
			pMaterialInst = &_pGeometriesMaterialInst[i];

			if	(pMaterialInst->HasHandle())
			{
				pMaterial = static_cast<MATERIAL_ABC *>(pMaterialInst->GetCommunicator());
				pShader	  = pMaterial->GetShader();

				#ifdef _DEBUG
					pShader->SetName(pMaterial->GetName());			
				#endif
			}
			else
			{
				pMaterial = NULL;
				pShader = NULL;
			}

			bOneBitAlpha = false;
			if (pMaterial && pMaterial->GetTransparency())
			{
				if	(pMaterial->HasOneBitAlpha())
				{
					bOneBitAlpha = true;
				}
				else
				{
					// Transparent geometry
					if (pMaterial->GetSort())
					{
						NODE * pCameraNode = DISPLAY_MANAGER::Instance()->GetCameraNode();				
						VECTOR CameraPos, GeometryPos = _pBoundingBox->GetCenter();

						if	(GetFlags(DYNAMIC_GEOMETRY))
						{
							DisplayParameters.GetNode()->GetGlobalMatrix().TransformPosition(GeometryPos);
						}

						pCameraNode->GetGlobalPosition(CameraPos);
						VECTOR Camera = GeometryPos - CameraPos;
						float rCameraDistance = Camera.SquareLength();
						
						DISPLAY_MANAGER::Instance()->GetCurrentPortalZone()->PushSortedTransparentObject(TRANSPARENT_OBJECT(rCameraDistance, DisplayParameters.GetNode(), i, 0));
					}
					else
					{
						DISPLAY_MANAGER::Instance()->GetCurrentPortalZone()->PushTransparentObject(TRANSPARENT_OBJECT(0.0f, DisplayParameters.GetNode(), i, 0));
					}

					continue;
				}
			}

			pRC->SetShader(pShader);

			// Cull mode
			RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
			pRC->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);

			// Lights
			pRC->EnableLighting(false);
			
			// Clipping
			pRC->EnableClipping(true);

			// Transform
			pRC->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());

			// Alpha test
			if	(bOneBitAlpha)
			{
				pRC->EnableAlphaTest(true);
				pRC->SetAlphaTestFunc(RENDER_CONTEXT::TESTFUNC_EQUAL);
				pRC->SetAlphaTestRef(pRC->GetMaxAlphaValue());
			}
			else
			{
				pRC->EnableAlphaTest(false);
			}

			// Render
			pRP->Display(pGeometry, true);
						
			// Reset lights
			pRC->ResetLights();

			// Reset cullmode
			pRC->SetCullMode(RmCullMode);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayTrans
//	Object:		
//	04-04-13:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::DisplayTrans(DISPLAY_PARAMETERS &	DisplayParameters,
									unsigned int			nGeometryIndex,
									unsigned int			nSkinClusterIndex)
{
	GEOMETRY_ABC *			pGeometry;
	COM_INSTANCE *			pMaterialInst;
	MATERIAL_ABC *			pMaterial;
	SHADER_ABC *			pShader;
	RENDER_CONTEXT *		pRC = RENDER_MANAGER::Instance()->GetRenderContext();
	RENDER_PIPELINE_ABC	*	pRP = pRC->GetRenderPipeline();

	pGeometry = _pGeometriesArray[nGeometryIndex];
	pMaterialInst = &_pGeometriesMaterialInst[nGeometryIndex];

	if	(pMaterialInst->HasHandle())
	{
		pMaterial = static_cast<MATERIAL_ABC *>(pMaterialInst->GetCommunicator());
		pShader	  = pMaterial->GetShader();

		#ifdef _DEBUG
			pShader->SetName(pMaterial->GetName());			
		#endif
	}
	else
	{
		pMaterial = NULL;
		pShader = NULL;
	}

	pRC->SetShader(pShader);

	// Cull mode
	RENDER_CONTEXT::CULL_MODE	RmCullMode = pRC->GetCullMode();
	pRC->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);

	// Lights
	pRC->EnableLighting(true);
	
	// Clipping
	pRC->EnableClipping(true);

	// Transform
	pRC->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());

	// Alpha test
	pRC->EnableAlphaTest(false);

	// Render
	pRP->Display(pGeometry, true);
				
	// Reset lights
	pRC->ResetLights();

	// Reset cullmode
	pRC->SetCullMode(RmCullMode);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayZ
//	Object:		
//	01-12-04:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::DisplayZ(const MATRIX &	WorldMatrix,
								const MATRIX &	ViewMatrix,
								int				nMaskValue)
{
	int					i;
	RENDER_CONTEXT		*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	bool				bWired = pContext->IsWireFrameEnabled();
	bool				bDepthTest = pContext->IsDepthTestEnabled();
	bool				bDepthWrite = pContext->IsDepthWriteEnabled();
	GEOMETRY_REF		Ref;

	pContext->EnableDepthWrite(true);
	pContext->EnableDepthTest(true);
	pContext->EnableColorWrite(false);
	pContext->EnableWireFrame(false);

	pContext->EnableDepthWrite(true, 1);
	pContext->EnableDepthTest(true, 1);
	pContext->EnableColorWrite(false, 1);

	MATRIX TempM;
	TempM.InverseAffine(ViewMatrix);
	pContext->SetViewTransform(&TempM);
	pContext->EnableVertexBlending(0, false);

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		Ref.SetGeometry(_pCopyGeometries[i]);
		Ref.SetGlobalMatrix(&WorldMatrix);
		Ref.SetPrelit(true);
		Ref.SetDoubleSided(true);
		Ref.SetDoClipping(true);

		#ifdef _DEBUG
			Ref.SetMeshName(GetName());
		#endif

		Ref.SetShader(NULL);

		pContext->GetRenderPipeline()->ClearZBuffer(*_pGeometriesArray[i], _pCopyGeometries[i], WorldMatrix, _rFarClippingDistance, &Ref, nMaskValue);
	}

	pContext->EnableColorWrite(true);
	pContext->EnableWireFrame(bWired);
	pContext->EnableDepthWrite(bDepthWrite);
	pContext->EnableDepthTest(bDepthTest);

	pContext->EnableColorWrite(true, 1);
	pContext->EnableDepthWrite(bDepthWrite, 1);
	pContext->EnableDepthTest(bDepthTest, 1);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayMask
//	Object:		
//	03-05-26:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::DisplayMask(const MATRIX &	WorldMatrix,
								   const MATRIX &	ViewMatrix,
								   int				nLevel,
								   int				nMaskValue)
{
	int					i;
	RENDER_CONTEXT		*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	bool				bWired = pContext->IsWireFrameEnabled();
	GEOMETRY_REF		Ref;

	pContext->EnableDepthWrite(false);
	pContext->EnableColorWrite(false);
	pContext->EnableWireFrame(false);

	MATRIX TempM;
	TempM.InverseAffine(ViewMatrix);
	pContext->SetViewTransform(&TempM);
	pContext->EnableVertexBlending(0, false);

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		Ref.SetGeometry(_pGeometriesArray[i]);
		Ref.SetGlobalMatrix(&WorldMatrix);
		Ref.SetPrelit(true);
		Ref.SetDoubleSided(true);
		Ref.SetDoClipping(true);

		#ifdef _DEBUG
			Ref.SetMeshName(GetName());
		#endif

		pContext->GetRenderPipeline()->DisplayMask(Ref, nLevel, nMaskValue);
	}

	pContext->EnableColorWrite(true);
	pContext->EnableWireFrame(bWired);
	pContext->EnableDepthWrite(true);
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	01-08-31:	ELE - Created
//-----------------------------------------------------------------------------
bool	MIRROR_PORTAL::RayCast(RAY_CAST_PARAMETERS &	RayCastParameters,
							   bool &					bContinue)
{
	bContinue	= true;
	bool bHit	= false;
	bool bCull	= false;

	RAY		LocalRay;
	MATRIX	Inverse; // No need to initialize

	// Compute inverse matrix
	Inverse.InverseAffine(RayCastParameters.GetGlobalMatrix());

	// Get local ray
	LocalRay = RayCastParameters.GetRay();
	LocalRay.Transform(Inverse);

	if (RayCastParameters.GetFlags(RAY_CAST_PARAMETERS::CULLING))
	{
		bCull = true;
	}

	for (int i = 0 ; i < _nNbrGeometries ; ++i)
	{
		if (_pGeometriesArray[i]->RayCast(RayCastParameters, LocalRay, bCull))
		{
			bHit = true;
		}
	}

	return (bHit);
}

//-----------------------------------------------------------------------------
//	Name:		UpdatePortalZone
//	Object:		
//	01-07-16:	ELE - Created
//-----------------------------------------------------------------------------
bool	MIRROR_PORTAL::UpdatePortalZone(const MATRIX &			ViewMatrix,
										const MATRIX &			NodeGlobalMatrix,
										SORTED_PORTAL_ZONE *	pPortalZone)
{
	MATRIX		InPortalMirroredGMatrix;
	MATRIX		InPortalFlippedGMatrix; 
	MATRIX		InPortalReferenceMatrix;
	MATRIX		ViewLMatrix;
	MATRIX		NewViewGMatrix;

	// Compute NewViewGMatrix, the global matrix of the new point of view
	InPortalMirroredGMatrix.MakeAffine();
	InPortalMirroredGMatrix.MulAffine(MATRIX::MIRROR_X, NodeGlobalMatrix);

	InPortalFlippedGMatrix.MakeAffine();
	InPortalFlippedGMatrix.MulAffine(MATRIX::ROTATION_Y_180, InPortalMirroredGMatrix);
	
	InPortalReferenceMatrix.MakeAffine();
	InPortalReferenceMatrix.InverseAffine(InPortalFlippedGMatrix);
	
	ViewLMatrix.MakeAffine();
	ViewLMatrix.MulAffine(ViewMatrix, InPortalReferenceMatrix); 				

	NewViewGMatrix.MakeAffine();
	NewViewGMatrix.MulAffine(ViewLMatrix, NodeGlobalMatrix);

	// Set the render parameters
	pPortalZone->SetViewMatrix(NewViewGMatrix);

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MIRROR_P", 7, CM_ReadChunk7);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MIRROR_P", 8, CM_ReadChunk8);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MIRROR_P", 9, CM_ReadChunk9);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MIRROR_P", 10, CM_ReadChunk10);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MIRROR_P", 11, CM_ReadChunk11);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MIRROR_P", 12, CM_ReadChunk12);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("MIRROR_P", 12, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MIRROR_PORTAL::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MIRROR_PORTAL) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MIRROR_PORTAL::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("MIRROR_PORTAL"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MIRROR_PORTAL,
											QDT_COMMENT("5A139CD0-C9B5-D3B3-B3B5-09B95589F887"),
											sizeof(MIRROR_PORTAL),
											ComInit,
											QDT_COMMENT("Class for mirror portals"),
											PORTAL_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::EM_Construct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pO);

	QDT_NEW_PLACED(pMirrorPortal, MIRROR_PORTAL)();
	QDT_ASSERT(pMirrorPortal);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-11-24:	VMA - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::EM_Construct_In_Memory(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	MIRROR_PORTAL	*pMirror = static_cast<MIRROR_PORTAL*> ( pO );

	if ( !MIRROR_PORTAL::_pVTable	)
	{
		GET_VTABLE( MIRROR_PORTAL );
	}
	pMirror->SetVirtualTable( MIRROR_PORTAL::_pVTable );
}


//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::EM_Destruct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{

	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pO);

	QDT_DELETE_PLACED(pMirrorPortal, MIRROR_PORTAL);
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::CM_ReadChunk7(IO_STREAM &		Stream,
									 COMMUNICATOR *	pCommunicator)
{
	int				i, nChunkVersion = 8;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	pMirrorPortal->ReadCAttributes7(Stream, nChunkVersion);

	pMirrorPortal->_pCopyGeometries = new GEOMETRY_ABC*[pMirrorPortal->_nNbrGeometries];

	if	(pMirrorPortal->_pGeometriesArray[0]->GetFormat() == GEOMETRY_ABC::FORMAT_TYPE_DIRECTX)
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();

			pMirrorPortal->_pCopyGeometries[i]->Copy(*pMirrorPortal->_pGeometriesArray[i], GEOMETRY_ABC::ALL, true, true);
			pMirrorPortal->_pCopyGeometries[i]->CreateContainer(pMirrorPortal->_pCopyGeometries[i]->GetVertexType(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrVertices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrIndices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrLayers(),
																true, false);
			pMirrorPortal->_pCopyGeometries[i]->SynchronizeIndices();
			pMirrorPortal->_pCopyGeometries[i]->SetNeedSynchronizeIndices(true);
		}
	}
	else
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::CM_ReadChunk8(IO_STREAM &		Stream,
									 COMMUNICATOR *	pCommunicator)
{
	int				i, nChunkVersion = 8;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	pMirrorPortal->ReadCAttributes8(Stream, nChunkVersion);

	pMirrorPortal->_pCopyGeometries = new GEOMETRY_ABC*[pMirrorPortal->_nNbrGeometries];

	if	(pMirrorPortal->_pGeometriesArray[0]->GetFormat() == GEOMETRY_ABC::FORMAT_TYPE_DIRECTX)
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();

			pMirrorPortal->_pCopyGeometries[i]->Copy(*pMirrorPortal->_pGeometriesArray[i], GEOMETRY_ABC::ALL, true, true);
			pMirrorPortal->_pCopyGeometries[i]->CreateContainer(pMirrorPortal->_pCopyGeometries[i]->GetVertexType(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrVertices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrIndices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrLayers(),
																true, false);
			pMirrorPortal->_pCopyGeometries[i]->SynchronizeIndices();
			pMirrorPortal->_pCopyGeometries[i]->SetNeedSynchronizeIndices(true);
		}
	}
	else
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk9
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::CM_ReadChunk9(IO_STREAM &		Stream,
									 COMMUNICATOR *	pCommunicator)
{
	int				i, nChunkVersion = 9;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	pMirrorPortal->ReadCAttributes8(Stream, nChunkVersion);

	pMirrorPortal->_pCopyGeometries = new GEOMETRY_ABC*[pMirrorPortal->_nNbrGeometries];

	if	(pMirrorPortal->_pGeometriesArray[0]->GetFormat() == GEOMETRY_ABC::FORMAT_TYPE_DIRECTX)
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();

			pMirrorPortal->_pCopyGeometries[i]->Copy(*pMirrorPortal->_pGeometriesArray[i], GEOMETRY_ABC::ALL, true, true);
			pMirrorPortal->_pCopyGeometries[i]->CreateContainer(pMirrorPortal->_pCopyGeometries[i]->GetVertexType(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrVertices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrIndices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrLayers(),
																true, false);
			pMirrorPortal->_pCopyGeometries[i]->SynchronizeIndices();
			pMirrorPortal->_pCopyGeometries[i]->SetNeedSynchronizeIndices(true);
		}
	}
	else
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk10
//	Object:		
//	03-10-16:	RMA - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::CM_ReadChunk10(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	int				i, nChunkVersion = 9;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	pMirrorPortal->ReadCAttributes9(Stream, nChunkVersion);

	pMirrorPortal->_pCopyGeometries = new GEOMETRY_ABC*[pMirrorPortal->_nNbrGeometries];

	if	(pMirrorPortal->_pGeometriesArray[0]->GetFormat() == GEOMETRY_ABC::FORMAT_TYPE_DIRECTX)
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();

			pMirrorPortal->_pCopyGeometries[i]->Copy(*pMirrorPortal->_pGeometriesArray[i], GEOMETRY_ABC::ALL, true, true);
			pMirrorPortal->_pCopyGeometries[i]->CreateContainer(pMirrorPortal->_pCopyGeometries[i]->GetVertexType(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrVertices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrIndices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrLayers(),
																true, false);
			pMirrorPortal->_pCopyGeometries[i]->SynchronizeIndices();
			pMirrorPortal->_pCopyGeometries[i]->SetNeedSynchronizeIndices(true);
		}
	}
	else
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk11
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::CM_ReadChunk11(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	int				i, nChunkVersion = 9;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	pMirrorPortal->ReadCAttributes10(Stream, nChunkVersion);

	pMirrorPortal->_pCopyGeometries = new GEOMETRY_ABC*[pMirrorPortal->_nNbrGeometries];

	if	(pMirrorPortal->_pGeometriesArray[0]->GetFormat() == GEOMETRY_ABC::FORMAT_TYPE_DIRECTX)
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();

			pMirrorPortal->_pCopyGeometries[i]->Copy(*pMirrorPortal->_pGeometriesArray[i], GEOMETRY_ABC::ALL, true, true);
			pMirrorPortal->_pCopyGeometries[i]->CreateContainer(pMirrorPortal->_pCopyGeometries[i]->GetVertexType(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrVertices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrIndices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrLayers(),
																true, false);
			pMirrorPortal->_pCopyGeometries[i]->SynchronizeIndices();
			pMirrorPortal->_pCopyGeometries[i]->SetNeedSynchronizeIndices(true);
		}
	}
	else
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk12
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::CM_ReadChunk12(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	int				i, nChunkVersion = 9;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	pMirrorPortal->ReadCAttributes11(Stream, nChunkVersion);

	pMirrorPortal->_pCopyGeometries = new GEOMETRY_ABC*[pMirrorPortal->_nNbrGeometries];

	if	(pMirrorPortal->_pGeometriesArray[0]->GetFormat() == GEOMETRY_ABC::FORMAT_TYPE_DIRECTX)
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();

			pMirrorPortal->_pCopyGeometries[i]->Copy(*pMirrorPortal->_pGeometriesArray[i], GEOMETRY_ABC::ALL, true, true);
			pMirrorPortal->_pCopyGeometries[i]->CreateContainer(pMirrorPortal->_pCopyGeometries[i]->GetVertexType(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrVertices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrIndices(),
																pMirrorPortal->_pCopyGeometries[i]->GetNbrLayers(),
																true, false);
			pMirrorPortal->_pCopyGeometries[i]->SynchronizeIndices();
			pMirrorPortal->_pCopyGeometries[i]->SetNeedSynchronizeIndices(true);
		}
	}
	else
	{
		for	(i = 0 ; i < pMirrorPortal->_nNbrGeometries ; ++i)
		{
			pMirrorPortal->_pCopyGeometries[i] = NULL;
		}
	}
}

#endif


//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	MIRROR_PORTAL::CM_WriteChunk(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	int				nChunkVersion = 9;
	MIRROR_PORTAL	*pMirrorPortal;
	pMirrorPortal = static_cast<MIRROR_PORTAL *>(pCommunicator);

	Stream.DebugOutput("Mirror portal :\t");
	Stream.DebugOutput(pMirrorPortal->GetName());
	Stream.DebugOutput("\r\n");

	pMirrorPortal->WriteCAttributes(Stream, nChunkVersion);

	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
