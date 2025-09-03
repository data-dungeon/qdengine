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
//	CLASS:	MATERIAL_LIGHT_TRACK
//
//	02-10-24:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_KRNCOM(OldCom\Database\DatabaseManager)
#include	INC_KRNSERV(GenericDisplay)
#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(M3D)

#ifndef _DEBUG
	#include	INL_KRNCORE(Display/RenderManager)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MATERIAL_LIGHT_TRACK constructor
//	Object:		
//	02-10-24:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_LIGHT_TRACK::MATERIAL_LIGHT_TRACK()
:
_MaterialInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_TextureInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_OldTextureInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_nNumLayer			(-1)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MATERIAL_LIGHT_TRACK constructor
//	Object:		
//	02-10-24:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_LIGHT_TRACK::MATERIAL_LIGHT_TRACK(const MATERIAL_LIGHT_TRACK & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		MATERIAL_LIGHT_TRACK destructor
//	Object:		
//	02-10-24:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_LIGHT_TRACK::~MATERIAL_LIGHT_TRACK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-10-24:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_LIGHT_TRACK &	MATERIAL_LIGHT_TRACK::operator=(const MATERIAL_LIGHT_TRACK & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-10-24:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_LIGHT_TRACK::Copy(const MATERIAL_LIGHT_TRACK & C)
{
	_MaterialInst	= C._MaterialInst;
	_TextureInst	= C._TextureInst;
	_OldTextureInst	= C._OldTextureInst;
	_nNumLayer		= C._nNumLayer;
}

//-----------------------------------------------------------------------------
//	Name:		Preload
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_LIGHT_TRACK::Preload(IO_STREAM &	Stream)
{
	Stream >> _MaterialInst >> _TextureInst >> _nNumLayer;

	// Load also old texture for interpolation
	MATERIAL *pMat = static_cast<MATERIAL*>(_MaterialInst.GetCommunicator());
	_OldTextureInst = pMat->GetLightmapTexture();

	TEXTURE_NEW	*pTexture = static_cast<TEXTURE_NEW *>(_OldTextureInst.GetCommunicator());
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTextureIndex
//	Object:		
//	02-11-05:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_LIGHT_TRACK::UpdateTextureIndex()
{
	// Load also old texture for interpolation
	MATERIAL *pMat = static_cast<MATERIAL*>(_MaterialInst.GetCommunicator());
	_OldTextureInst = pMat->GetLightmapTexture();
}

//-----------------------------------------------------------------------------
//	Name:		LoadAndApply
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_LIGHT_TRACK::LoadAndApply(IO_STREAM &	Stream)
{
	Stream >> _MaterialInst >> _TextureInst >> _nNumLayer;

	MATERIAL *pMat = static_cast<MATERIAL*>(_MaterialInst.GetCommunicator());
	pMat->SetLightmapTexture(_TextureInst);
	pMat->UpdateShader(true);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-10-24:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_LIGHT_TRACK::Write(IO_STREAM &	Stream) const
{
	Stream << _MaterialInst << _TextureInst << _nNumLayer;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-11-05:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_LIGHT_TRACK::Update(float	rRatio,
									 bool	bEnd)
{
	QDT_ASSERT(_TextureInst.HasHandle());
	QDT_ASSERT(_OldTextureInst.HasHandle());
	QDT_ASSERT(_MaterialInst.HasHandle());

	MATERIAL *pMat = static_cast<MATERIAL*>(_MaterialInst.GetCommunicator());

	if (bEnd)
	{
		pMat->SetLightmapTexture(_TextureInst);
		pMat->UpdateShader(true);

		return;
	}

	TEXTURE_ABC *pTexture = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateTexture();
	QDT_ASSERT(pTexture);

	TEXTURE_NEW *pTextureNew	= static_cast<TEXTURE_NEW*>(_TextureInst.GetCommunicator());
	TEXTURE_NEW *pOldTextureNew = static_cast<TEXTURE_NEW*>(_OldTextureInst.GetCommunicator());

	QDT_ASSERT(pTextureNew->GetFormat()		== TEXTURE_ABC::ARGB8888);
	QDT_ASSERT(pOldTextureNew->GetFormat()	== TEXTURE_ABC::ARGB8888);
	QDT_ASSERT(pTextureNew->GetSizeX() == pOldTextureNew->GetSizeX());
	QDT_ASSERT(pTextureNew->GetSizeY() == pOldTextureNew->GetSizeY());

	if (!pTexture->CreateTexture(pTextureNew->GetSizeX(),
								 pTextureNew->GetSizeY(),
								 TEXTURE_ABC::TEXTUREFORMAT_A8R8G8B8,
								 pOldTextureNew->GetRawData(),
								 pTextureNew->GetRawData(),
								 rRatio,
								 0))
	{
		delete (pTexture);
		return;
	}

	pMat->SetRMLightmapTexture(pTexture);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
