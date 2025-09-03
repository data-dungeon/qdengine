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
//	CLASS:	ENGINE_TEXTURE_BANK
//
//	06-03-28:	FBO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureBank)

#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFile)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetLinksNb
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
void	ENGINE_TEXTURE_BANK::SetLinksNb(UInt32	iLinksNb)
{
	if (_iLinksNb != iLinksNb)
	{
		if (_pLinksArray != NULL)
		{
			for (UInt32 iLinkP = 0; iLinkP < _iLinksNb; iLinkP++)
			{
				_pLinksArray[iLinkP]._rMeshDataCI.Release();

				ENGINE_TEXTURE_FILE* pETexture = static_cast<ENGINE_TEXTURE_FILE*>(_pLinksArray[iLinkP]._rTextureCI.GetCommunicator());
				RENDER_CONTEXT::ReleaseTexture(pETexture->GetTexture());
				_pLinksArray[iLinkP]._rTextureCI.Release();
			}
			delete _pLinksArray;
		}

		_iLinksNb = iLinksNb;
		_pLinksArray = new ENGINE_TEXTURE_BANK_LINK[_iLinksNb];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetLink
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
void	ENGINE_TEXTURE_BANK::SetLink(UInt32			iLinkIndex,
									 COM_INSTANCE&	rMeshDataCI,
									 COM_INSTANCE&	rTextureCI)
{
	QDT_ASSERT(iLinkIndex < _iLinksNb);
	QDT_ASSERT(rMeshDataCI.HasHandle() == true);
	QDT_ASSERT(rTextureCI.HasHandle() == true);
	
	_pLinksArray[iLinkIndex]._rMeshDataCI = rMeshDataCI;
	_pLinksArray[iLinkIndex]._rTextureCI = rTextureCI;
}

//-----------------------------------------------------------------------------
//	Name:		GetLinkTextureInstance
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE &	ENGINE_TEXTURE_BANK::GetLinkTextureInstance(UInt32	iLinkIndex)
{
	QDT_ASSERT(iLinkIndex < _iLinksNb);

	return _pLinksArray[iLinkIndex]._rTextureCI;
}

//-----------------------------------------------------------------------------
//	Name:		GetTexture
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	ENGINE_TEXTURE_BANK::GetTexture()
{
	return NULL;	
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_BANK Constructor
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_BANK::ENGINE_TEXTURE_BANK() : 
ENGINE_TEXTURE_ABC(GetGenericCommunicatorDescriptor()), 
_iLinksNb(0), 
_pLinksArray(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_BANK Destructor
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_BANK::~ENGINE_TEXTURE_BANK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_BANK::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_BANK));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_BANK::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("ENGINE_TEXTURE_BANK"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_BANK,
										QDT_COMMENT("45861452-HELL-7777-2569-56C86254F853"), 
										sizeof(ENGINE_TEXTURE_BANK), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_BANK::ComInit(CLASS_DESCRIPTOR * const pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_BANK::EM_Construct(void * const pO, 
									  void * const pR, 
									  void * const pP)
{
	ENGINE_TEXTURE_BANK	*pShader;
	pShader = static_cast<ENGINE_TEXTURE_BANK *>(pO);

	QDT_NEW_PLACED(pShader, ENGINE_TEXTURE_BANK)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-03-28:	FBO - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_BANK::EM_Destruct(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	ENGINE_TEXTURE_BANK	*pShader;
	pShader = static_cast<ENGINE_TEXTURE_BANK *>(pO);

	QDT_DELETE_PLACED(pShader, ENGINE_TEXTURE_BANK);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================