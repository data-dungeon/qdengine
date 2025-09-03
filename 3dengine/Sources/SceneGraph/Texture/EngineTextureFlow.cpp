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
//	CLASS:	ENGINE_TEXTURE_FILE
//
//	05-11-10:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/ITexture)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Flow)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/FlowData)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFlow)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_FILE Constructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_FLOW::ENGINE_TEXTURE_FLOW()
:
ENGINE_TEXTURE_ABC(GetGenericCommunicatorDescriptor()),
_uvSet(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_FILE Destructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_FLOW::~ENGINE_TEXTURE_FLOW()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_FLOW::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_FLOW));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_FLOW::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("ENGINE_TEXTURE_FLOW"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_FLOW,
										QDT_COMMENT("49994444-B0B0-F943-8BE3-66608EE88888"), 
										sizeof(ENGINE_TEXTURE_FLOW), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FLOW::ComInit(CLASS_DESCRIPTOR * const pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FLOWTEXT", 0, CM_ReadChunk);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("FLOWTEXT", 0, CM_WriteChunk);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FLOW::EM_Construct(void * const pO, 
									  void * const pR, 
									  void * const pP)
{
	ENGINE_TEXTURE_FLOW	*pShader;
	pShader = static_cast<ENGINE_TEXTURE_FLOW *>(pO);

	QDT_NEW_PLACED(pShader, ENGINE_TEXTURE_FLOW)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FLOW::EM_Destruct(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	ENGINE_TEXTURE_FLOW	*pShader;
	pShader = static_cast<ENGINE_TEXTURE_FLOW *>(pO);

	QDT_DELETE_PLACED(pShader, ENGINE_TEXTURE_FLOW);
}






//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FLOW::CM_ReadChunk(IO_STREAM & Stream, 
									   COMMUNICATOR * pCommunicator)
{
	QDT_NOT_IMPLEMENTED();

	ENGINE_TEXTURE_FLOW	*pTexture;
	pTexture = static_cast<ENGINE_TEXTURE_FLOW *>(pCommunicator);

	Stream >> pTexture->_uvSet;
	Stream >> pTexture->_uiMapSize;
	Stream >> pTexture->_fUpdateFrequency;
	Stream >> pTexture->_fDropSize;
	Stream >> pTexture->_fEaseIn;
	Stream >> pTexture->_fEaseOut;
	Stream >> pTexture->_fInitialSpeed;
	Stream >> pTexture->_fAcceleration;
	Stream >> pTexture->_fFriction;
	Stream >> pTexture->_fMaxLife;
	Stream >> pTexture->_fHeightLoss;
	Stream >> pTexture->_fRadiusLoss;
	Stream >> pTexture->_fTrailLoss;
	Stream >> pTexture->_fSpawnFreqR;
	Stream >> pTexture->_fSpawnFreqG;
	Stream >> pTexture->_fSpawnFreqB;
}
	
//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FLOW::CM_WriteChunk(IO_STREAM & Stream, 
										COMMUNICATOR * pCommunicator)
{
	QDT_NOT_IMPLEMENTED();


	ENGINE_TEXTURE_FLOW	*pTexture;
	pTexture = static_cast<ENGINE_TEXTURE_FLOW *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("ENGINE_TEXTURE_FLOW :");
	Stream.DebugOutput(pTexture->GetName());
	Stream.DebugOutput("\r\n");

	Stream << pTexture->_uvSet;
	Stream << pTexture->_uiMapSize;
	Stream << pTexture->_fUpdateFrequency;
	Stream << pTexture->_fDropSize;
	Stream << pTexture->_fEaseIn;
	Stream << pTexture->_fEaseOut;
	Stream << pTexture->_fInitialSpeed;
	Stream << pTexture->_fAcceleration;
	Stream << pTexture->_fFriction;
	Stream << pTexture->_fMaxLife;
	Stream << pTexture->_fHeightLoss;
	Stream << pTexture->_fRadiusLoss;
	Stream << pTexture->_fTrailLoss;
	Stream << pTexture->_fSpawnFreqR;
	Stream << pTexture->_fSpawnFreqG;
	Stream << pTexture->_fSpawnFreqB;
}



//-----------------------------------------------------------------------------
//	Name:		GetTexture
//	Object:		
//	06-03-29:	PDE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	ENGINE_TEXTURE_FLOW::GetTexture()
{
	MESH* pMesh = DISPLAY_MANAGER::Instance()->GetCurrentMesh();
	
	COMMUNICATOR* pCommunicator = DISPLAY_MANAGER::Instance()->GetCurrentMeshData().GetCommunicator();
	MESH_DATA* pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	FLOW* pFlow = pMesh->GetFlow(this, pMeshData->GetFlowData(_uvSet));
	return pFlow->GetTexture();	
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
