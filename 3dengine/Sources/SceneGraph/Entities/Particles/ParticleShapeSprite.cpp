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
//	CLASS:	PARTICLE_SHAPE_SPRITE
//
//	01-11-20:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleShapeSprite)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(ComplexType/TypePoint2D)
#include	INCL_KCOM(ComplexType/TypeVector)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KMATH(Math/Color/Color)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)


#ifdef _DEBUG
	#include	"ParticleShapeSprite.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*					PARTICLE_SHAPE_SPRITE::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_SHAPE_SPRITE constructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE_SPRITE::PARTICLE_SHAPE_SPRITE()
:
PARTICLE_SHAPE	(CreateGenericCommunicatorDescriptor())
{
//	_pShapeSpriteData = static_cast<PARTICLE_SHAPE_SPRITE_DATA *>(_pShapeData);
}

//-----------------------------------------------------------------------------
//	Name:		~PARTICLE_SHAPE_SPRITE destructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE_SPRITE::~PARTICLE_SHAPE_SPRITE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::Update(float	rDeltaTime)
{
	PARTICLE_SHAPE::Update(rDeltaTime);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
/*MATERIAL *	PARTICLE_SHAPE_SPRITE::GetMaterial() const
{
	if	(_MaterialInst.HasHandle())
	{
		return	( static_cast<MATERIAL *>(_MaterialInst.GetCommunicator()) );
	}
	else
	{
		return	(NULL);
	}
}*/


//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::Copy(const PARTICLE_SHAPE_SPRITE &	ParticleShapeSprite)
{
	PARTICLE_SHAPE::Copy(ParticleShapeSprite);

//	_pShapeSpriteData = static_cast<PARTICLE_SHAPE_SPRITE_DATA *>(_pShapeData);
}


//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE_SPRITE &	PARTICLE_SHAPE_SPRITE::operator=(const PARTICLE_SHAPE_SPRITE &	ParticleShapeSprite)
{
	if (this != &ParticleShapeSprite)
	{
		Copy(ParticleShapeSprite);
	}

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR	*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PARTICLE SHAPE SPRITE", 0, CM_ReadChunk);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PARTICLE SHAPE SPRITE", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PARTICLE SHAPE SPRITE", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PARTICLE SHAPE SPRITE", 3, CM_ReadChunk3);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("PARTICLE SHAPE SPRITE", 3, CM_WriteChunk);
#endif

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetMaterial"), QDT_COMMENT("Set the material of the sprite."));
//	pMD->AddParameter(QDT_COMMENT("Material"), QDT_COMMENT(""), MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_MATERIAL, pMD, EM_SetMaterial);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetMaterial"), QDT_COMMENT("Get the material of the sprite."));
//	pMD->AddParameter(QDT_COMMENT("Material"), QDT_COMMENT(""), MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_GET_MATERIAL, pMD, EM_GetMaterial);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetUVs"), QDT_COMMENT("The position in the UVs track of the key to change."));
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The position in the UVs track of the key to change."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("UV1"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("UV2"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("UV3"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("UV4"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_UVS, pMD, EM_SetUVs);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetUVs"), QDT_COMMENT("The position in the UVs track of the key to change."));
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The position in the UVs track of the key to change."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV1X"), QDT_COMMENT("The X coordinate of the 1st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV1Y"), QDT_COMMENT("The Y coordinate of the 1st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV2X"), QDT_COMMENT("The X coordinate of the 2st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV2Y"), QDT_COMMENT("The Y coordinate of the 2st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV3X"), QDT_COMMENT("The X coordinate of the 3st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV3Y"), QDT_COMMENT("The Y coordinate of the 3st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV4X"), QDT_COMMENT("The X coordinate of the 4st UVs point."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUV4Y"), QDT_COMMENT("The Y coordinate of the 4st UVs point."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_UVS2, pMD, EM_SetUVs);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetUVsNbrKeys"), QDT_COMMENT("Set the number of keys in the UVs track of the sprite."));
	pMD->AddParameter(QDT_COMMENT("Material"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_UVS_NB_KEYS, pMD, EM_SetNbrUVsKeys);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetUVsTimeKey"), QDT_COMMENT("Set an UV time key."));
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rTime"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_UVS_TIME_KEY, pMD, EM_SetUVTimeKey);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDiffuseColor"), QDT_COMMENT("Set the color of a diffuse color key."));
	pMD->AddParameter(QDT_COMMENT("nTrackIndex"), QDT_COMMENT("The index of the track."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The key index in the track."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT("The color of the track."), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_DIFFUSE_COLOR, pMD, EM_SetDiffuseColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDiffuseColor"), QDT_COMMENT("Set the color of a diffuse color key."));
	pMD->AddParameter(QDT_COMMENT("nTrackIndex"), QDT_COMMENT("The index of the track."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The key index in the track."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nA"), QDT_COMMENT("The alpla componant of the key (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nR"), QDT_COMMENT("The red componant of the key (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nG"), QDT_COMMENT("The green componant of the key (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nB"), QDT_COMMENT("The blue componant of the key (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_DIFFUSE_COLOR2, pMD, EM_SetDiffuseColor2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDiffuseColorNbrKeys"), QDT_COMMENT("Set the number of keys of a diffuse color track."));
	pMD->AddParameter(QDT_COMMENT("nTrackIndex"), QDT_COMMENT("The index of a track."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nNbrKeys"), QDT_COMMENT("The number of keys of the track."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_DIFFUSE_COLOR_NB_KEYS, pMD, EM_SetNbrDiffuseColorKeys);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDiffuseColorTimeKey"), QDT_COMMENT("Set the time of a diffuse color key."));
	pMD->AddParameter(QDT_COMMENT("nTrackIndex"), QDT_COMMENT("The track index."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The index of the key in the track."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rTime"), QDT_COMMENT("The time of the key. Must be between 0.0 and 1.0."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_DIFFUSE_COLOR_TIME_KEY, pMD, EM_SetDiffuseColorTimeKey);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDiffuseColorTrack"), QDT_COMMENT("Choose the diffuse color track of the sprite."));
	pMD->AddParameter(QDT_COMMENT("nTrackIndex"), QDT_COMMENT("The index of the track."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_DIFFUSE_COLOR_TRACK, pMD, EM_SetDiffuseColorTrack);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddDiffuseColorTrack"), QDT_COMMENT("Add diffuse color tracks to the sprite."));
	pMD->AddParameter(QDT_COMMENT("nNbrTracks"), QDT_COMMENT("Number of tracks to add."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_ADD_DIFFUSE_COLOR_TRACK, pMD, EM_AddDiffuseColorTrack);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveDiffuseColorTrack"), QDT_COMMENT("Remove a diffuse color tracks to the sprite."));
	pMD->AddParameter(QDT_COMMENT("nTrackIndex"), QDT_COMMENT("If -1, the color track of radomly choosed."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_REMOVE_DIFFUSE_COLOR_TRACK, pMD, EM_RemoveDiffuseColorTrack);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetAxisAligned"), QDT_COMMENT("Set the sprite generated with alignment axis."));
	pMD->AddParameter(QDT_COMMENT("bAligned"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_AXIS_ALIGNED, pMD, EM_SetAxisAligned);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsAxisAligned"), QDT_COMMENT("Is the sprite generated with alignment axis ?"));
	pMD->SetReturn(QDT_COMMENT("bAligned"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_IS_AXIS_ALIGNED, pMD, EM_IsAxisAligned);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetAlignmentAxis"), QDT_COMMENT("Set the txo directions of the sprite."));
	pMD->AddParameter(QDT_COMMENT("vAlignmentAxis1"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("vAlignmentAxis2"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PSSPRITE_SET_ALIGNMENT_AXIS, pMD, EM_SetAlignmentAxis);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PARTICLE_SHAPE_SPRITE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_SHAPE_SPRITE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PARTICLE_SHAPE_SPRITE::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PARTICLE_SHAPE_SPRITE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_SHAPE_SPRITE,
										QDT_COMMENT("0DADD0E1-C0E1-B40F-4101-56B60FF2652B"), 
										sizeof(PARTICLE_SHAPE_SPRITE), 
										ComInit, 
										QDT_COMMENT("Shape sprite of a particle."),
										PARTICLE_SHAPE::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_Construct(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	PARTICLE_SHAPE_SPRITE *pPSS;
	pPSS = static_cast<PARTICLE_SHAPE_SPRITE *>(pO);

	QDT_NEW_PLACED(pPSS, PARTICLE_SHAPE_SPRITE)();
	QDT_ASSERT(pPSS);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-01-08:	IAM - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_Construct_In_Memory(void * const	pO,
													  void * const	pR,
													  void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_Destruct(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	PARTICLE_SHAPE_SPRITE *pPSS;
	pPSS = static_cast<PARTICLE_SHAPE_SPRITE *>(pO);

	QDT_DELETE_PLACED(pPSS, PARTICLE_SHAPE_SPRITE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMaterial
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetMaterial(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	
	/*COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	COM_INSTANCE*			pInst = static_cast<COM_INSTANCE*>(pP);

	pPSS->SetMaterialInst(*pInst);

	pPSS->UpdateShader();*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMaterial
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_GetMaterial(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	/*COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	COM_INSTANCE*			pInst		 = static_cast<COM_INSTANCE*>(pR);

	QDT_NEW_PLACED(pInst, COM_INSTANCE)(pPSS->GetMaterialInst());*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetUVs
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetUVs(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	struct PARAMETERS
	{
		int			_nKeyIndex;
		POINT2D		_UVs[4];
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS*				pParams		 = static_cast<PARAMETERS*>(pP);

	pPSS->SetUVs(pParams->_nKeyIndex, pParams->_UVs);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNbrUVsKeys
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetNbrUVsKeys(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	int*					pi			 = static_cast<int*>(pP);

	pPSS->SetUVsNbKeys(*pi);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetUVTimeKey
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetUVTimeKey(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	struct PARAMETERS
	{
		int			_nKeyIndex;
		float		_rTime;
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS*				pParams		 = static_cast<PARAMETERS*>(pP);

	pPSS->SetUVsTimeKey(pParams->_nKeyIndex, pParams->_rTime);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDiffuseColor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetDiffuseColor(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	struct PARAMETERS
	{
		int			_nTrackIndex;
		int			_nKeyIndex;
		COLOR		_Color;
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS*				pParams		 = static_cast<PARAMETERS*>(pP);

	pPSS->SetDiffuseColor(pParams->_nTrackIndex, pParams->_nKeyIndex, pParams->_Color);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDiffuseColor2
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetDiffuseColor2(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	struct PARAMETERS
	{
		int			_nTrackIndex;
		int			_nKeyIndex;
		int			_nA, _nR, _nG, _nB;
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS*				pParams		 = static_cast<PARAMETERS*>(pP);

	pPSS->SetDiffuseColor(pParams->_nTrackIndex, 
						  pParams->_nKeyIndex, 
						  COLOR(pParams->_nA, pParams->_nR, pParams->_nG, pParams->_nB));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNbrDiffuseColorKeys
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetNbrDiffuseColorKeys(void * const	pO,
														 void * const	pR,
														 void * const	pP)
{
	struct PARAMETERS
	{
		int			_nTrackIndex;
		int			_nNbrKeys;
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS*				pParams		 = static_cast<PARAMETERS*>(pP);

	pPSS->SetDiffuseColorNbKeys(pParams->_nTrackIndex, pParams->_nNbrKeys);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDiffuseColorTimeKey
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetDiffuseColorTimeKey(void * const	pO,
														 void * const	pR,
														 void * const	pP)
{
	struct PARAMETERS
	{
		int			_nTrackIndex;
		int			_nKeyIndex;
		float		_rTime;
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS*				pParams		 = static_cast<PARAMETERS*>(pP);

	pPSS->SetDiffuseColorTimeKey(pParams->_nTrackIndex, pParams->_nKeyIndex, pParams->_rTime);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDiffuseColorTrack
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetDiffuseColorTrack(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	int*					pi			 = static_cast<int*>(pP);

	pPSS->SetColorTrack(*pi);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddDiffuseColorTrack
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_AddDiffuseColorTrack(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	int*					pi			 = static_cast<int*>(pP);

	pPSS->AddColorTracks(*pi);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveDiffuseColorTrack
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_RemoveDiffuseColorTrack(void * const	pO,
														  void * const	pR,
														  void * const	pP)
{
	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	int*					pi			 = static_cast<int*>(pP);

	pPSS->RemoveColorTrack(*pi);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAxisAligned
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetAxisAligned(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	bool *					pb			 = static_cast<bool *>(pP);

	pPSS->SetAligned(*pb);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsAxisAligned
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_IsAxisAligned(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	bool *					pb			 = static_cast<bool *>(pR);

	*pb = pPSS->IsAligned();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAlignmentAxis
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::EM_SetAlignmentAxis(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	struct PARAMETERS
	{
		VECTOR	_V1;
		VECTOR	_V2;
	};

	COM_INSTANCE *			pComInstance = static_cast<COM_INSTANCE *>(pO);
	PARTICLE_SHAPE_SPRITE *	pPSS		 = static_cast<PARTICLE_SHAPE_SPRITE*>(pComInstance->GetCommunicator());
	PARAMETERS *			pParams		 = static_cast<PARAMETERS *>(pP);

	VECTOR	AlignedAxis1(pParams->_V1, VECTOR::VM_OBJECT);
	VECTOR	AlignedAxis2(pParams->_V2, VECTOR::VM_OBJECT);

	pPSS->SetAlignmentAxis(AlignedAxis1, AlignedAxis2);
}

//-----------------------------------------------------------------------------
//	Name:		COM_PostLoad
//	Object:		
//	04-04-28:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::COM_PostLoad()
{
#ifndef	_MASTER		//	MASTER mode : databank mode always true
	if ( GetHandle()->IsLoadedBinary() == false )
	{
		UpdateShader();
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		UpdateShader
//	Object:		
//	04-01-08:	VMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::UpdateShader()
{
	QDT_NOT_IMPLEMENTED();
/*
	if	(_MaterialInst.HasHandle())
	{
		_pShapeSpriteData->SetShader( static_cast<MATERIAL *>(_MaterialInst.GetCommunicator())->GetShader() );
	}
	else
	{
		_pShapeSpriteData->SetShader(NULL);
	}*/
}

	// Chunk
//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-09-11:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	PARTICLE_SHAPE_SPRITE::CM_ReadChunk(IO_STREAM &		Stream,
											COMMUNICATOR *	pCommunicator)
{

	PARTICLE_SHAPE_SPRITE	*pShape;
	pShape = static_cast<PARTICLE_SHAPE_SPRITE *>(pCommunicator);

	COM_INSTANCE MaterialInst;

	Stream	>> MaterialInst;
	pShape->SetMaterialInst(MaterialInst);

	float rLifeSpan, rMass;
	COLOR color;
	VECTOR scale;

	Stream	>> rLifeSpan;


	// Color track
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->	SetDiffuseColorNbKeys( 0, nNbrKeys);				
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		COLOR color;


		Stream >> rTimeKey;
		Stream >> color;


//		pShape->_pShapeSpriteData->SetDiffuseColorTimeKey(0, nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetDiffuseColor(0, nEachKey, color);

	}

	Stream	>> scale;
	Stream	>> rMass;

//	pShape->_pShapeSpriteData->SetLifeSpan(rLifeSpan);
//	pShape->_pShapeSpriteData->SetScale(0, scale);
//	pShape->_pShapeSpriteData->SetMass(rMass);

			
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	04-03-15:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::CM_ReadChunk1(IO_STREAM &	Stream,
											 COMMUNICATOR *	pCommunicator)
{
	PARTICLE_SHAPE_SPRITE	*pShape;
	pShape = static_cast<PARTICLE_SHAPE_SPRITE *>(pCommunicator);

	COM_INSTANCE MaterialInst;

	Stream	>> MaterialInst;
	pShape->SetMaterialInst(MaterialInst);

	float rLifeSpan, rMass;
	Stream	>> rLifeSpan;
//	pShape->_pShapeSpriteData->SetLifeSpan(rLifeSpan);

	// Color track
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->	SetDiffuseColorNbKeys( 0, nNbrKeys);				
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		COLOR color;


		Stream >> rTimeKey;
		Stream >> color;


//		pShape->_pShapeSpriteData->SetDiffuseColorTimeKey(0, nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetDiffuseColor(0, nEachKey, color);

	}

	// Scale track
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->SetScaleNbKeys(nNbrKeys);
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		VECTOR scale;


		Stream >> rTimeKey;
		Stream >> scale;


//		pShape->_pShapeSpriteData->SetScaleTimeKey(nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetScale(nEachKey, scale);

	}

	Stream	>> rMass;

//	pShape->_pShapeSpriteData->SetMass(rMass);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	04-06-02:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::CM_ReadChunk2(IO_STREAM &	Stream,
											 COMMUNICATOR *	pCommunicator)
{
	PARTICLE_SHAPE_SPRITE	*pShape;
	pShape = static_cast<PARTICLE_SHAPE_SPRITE *>(pCommunicator);

	COM_INSTANCE MaterialInst;

	Stream	>> MaterialInst;
	pShape->SetMaterialInst(MaterialInst);

	float rLifeSpan, rMass;
	Stream	>> rLifeSpan;
//	pShape->_pShapeSpriteData->SetLifeSpan(rLifeSpan);

	// Color track
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->	SetDiffuseColorNbKeys( 0, nNbrKeys);				
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		COLOR color;


		Stream >> rTimeKey;
		Stream >> color;


//		pShape->_pShapeSpriteData->SetDiffuseColorTimeKey(0, nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetDiffuseColor(0, nEachKey, color);

	}

	// Scale track
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->SetScaleNbKeys(nNbrKeys);
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		VECTOR scale;


		Stream >> rTimeKey;
		Stream >> scale;


//		pShape->_pShapeSpriteData->SetScaleTimeKey(nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetScale(nEachKey, scale);

	}

	Stream	>> rMass;
//	pShape->_pShapeSpriteData->SetMass(rMass);

	bool bAligned;
	VECTOR AxisX, AxisY;

	Stream	>> bAligned;
	Stream	>> AxisX;
	Stream	>> AxisY;

//	pShape->_pShapeSpriteData->SetAligned(bAligned);
//	pShape->_pShapeSpriteData->SetAlignmentAxis(AxisX, AxisY);

}


//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	04-12-03:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::CM_ReadChunk3(IO_STREAM &	Stream,
											 COMMUNICATOR *	pCommunicator)
{
		PARTICLE_SHAPE_SPRITE	*pShape;
	pShape = static_cast<PARTICLE_SHAPE_SPRITE *>(pCommunicator);

	COM_INSTANCE MaterialInst;

	Stream	>> MaterialInst;
	pShape->SetMaterialInst(MaterialInst);

	float rLifeSpan, rMass;
	Stream	>> rLifeSpan;
//	pShape->_pShapeSpriteData->SetLifeSpan(rLifeSpan);

	// Color track
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->	SetDiffuseColorNbKeys( 0, nNbrKeys);				
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		COLOR color;


		Stream >> rTimeKey;
		Stream >> color;


//		pShape->_pShapeSpriteData->SetDiffuseColorTimeKey(0, nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetDiffuseColor(0, nEachKey, color);

	}

	// Scale track
	Stream >> nNbrKeys;
//	pShape->_pShapeSpriteData->SetScaleNbKeys(nNbrKeys);
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		VECTOR scale;


		Stream >> rTimeKey;
		Stream >> scale;


//		pShape->_pShapeSpriteData->SetScaleTimeKey(nEachKey, rTimeKey);
//		pShape->_pShapeSpriteData->SetScale(nEachKey, scale);

	}

	Stream	>> rMass;
//	pShape->_pShapeSpriteData->SetMass(rMass);

	bool bAligned;
	VECTOR AxisX, AxisY;

	Stream	>> bAligned;
	Stream	>> AxisX;
	Stream	>> AxisY;

//	pShape->_pShapeSpriteData->SetAligned(bAligned);
//	pShape->_pShapeSpriteData->SetAlignmentAxis(AxisX, AxisY);


	//UV Track
	Stream >> nNbrKeys;
	pShape->SetUVsNbKeys(nNbrKeys);
	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey;
		POINT2D uv[4];

		Stream >> rTimeKey;
		Stream >> uv[0];
		Stream >> uv[1];
		Stream >> uv[2];
		Stream >> uv[3];

		pShape->SetUVsTimeKey(nEachKey, rTimeKey);
		pShape->SetUVs(nEachKey, uv);

	}
}



#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-09-11:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	PARTICLE_SHAPE_SPRITE::CM_WriteChunk(IO_STREAM &	Stream,
											 COMMUNICATOR *	pCommunicator)
{
	PARTICLE_SHAPE_SPRITE	*pShape;
	pShape = static_cast<PARTICLE_SHAPE_SPRITE *>(pCommunicator);

	Stream.DebugOutput("Particle Shape :\t");
	Stream.DebugOutput(pShape->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Material :\t");
	Stream << pShape->GetMaterialInst();
	Stream.DebugOutput("\r\n");

	float rLifeSpan = 0.0f;//pShape->_pShapeSpriteData->GetLifeSpan();

	Stream.DebugOutput("Life Span :\t");
	Stream << rLifeSpan;
	Stream.DebugOutput("\r\n");

	int nNbrKeys = 0;//pShape->_pShapeSpriteData->GetDiffuseColorNbKeys(0);
	int nEachKey;
	Stream.DebugOutput("Colors :\t");
	Stream << nNbrKeys;
	Stream.DebugOutput("\r\n");

	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey = 0.0f;//pShape->_pShapeSpriteData->GetDiffuseColorTimeKey( 0, nEachKey);
		COLOR color;
		//pShape->_pShapeSpriteData->GetDiffuseColor(0, nEachKey, color);

		Stream << rTimeKey;
		Stream.DebugOutput("\r\n");
		Stream << color;
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");

	nNbrKeys = 0;//pShape->_pShapeData->GetScaleNbKeys(), nEachKey;
	Stream.DebugOutput("Size Scale :\t");
	Stream << nNbrKeys;
	Stream.DebugOutput("\r\n");

	for(nEachKey=0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float rTimeKey = 0.0f;//pShape->_pShapeData->GetScaleTimeKey(nEachKey);
		//Strange but necessary
		float rStrangeTimeKey = (1.0 - rTimeKey) * rLifeSpan;
		VECTOR scale;
		//pShape->_pShapeData->GetScale(rStrangeTimeKey, scale);

		Stream << rTimeKey;
		Stream.DebugOutput("\r\n");
		Stream << scale;
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Mass :\t");
//	Stream << pShape->_pShapeSpriteData->GetMass();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Aligned :\t");
//	Stream << pShape->_pShapeSpriteData->IsAligned();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Axis X :\t");
//	Stream << pShape->_pShapeSpriteData->GetAlignmentAxis1();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Axis Y :\t");
//	Stream << pShape->_pShapeSpriteData->GetAlignmentAxis2();
	Stream.DebugOutput("\r\n");


	nNbrKeys = pShape->GetUVsNbKeys();
	Stream.DebugOutput("UVS Nbr Keys:\t");
	Stream << nNbrKeys;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("UVS Time Key and Value:\t");
	for(int nEachUv = 0; nEachUv<nNbrKeys; ++nEachUv)
	{
		POINT2D uv[4];

		float rTimeKey = pShape->GetUVsTimeKey(nEachUv);
		float rStrangeTimeKey = (1.0 - rTimeKey) * rLifeSpan;
		pShape->GetUVs(rStrangeTimeKey - MATH::EPSILON(), uv);

		Stream.DebugOutput("\r\n");
		Stream << rTimeKey;		
		Stream.DebugOutput("\r\n");
		Stream << uv[0];
		Stream.DebugOutput("\r\n");
		Stream << uv[1];
		Stream.DebugOutput("\r\n");
		Stream << uv[2];
		Stream.DebugOutput("\r\n");
		Stream << uv[3];
		Stream.DebugOutput("\r\n");

	}

	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

