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
//	CLASS:	PARTICLE_SHAPE
//
//	01-11-20:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleShape)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(ComplexType/TypeVector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

#ifdef _DEBUG
	#include	"ParticleShape.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_SHAPE constructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE::PARTICLE_SHAPE()
:
COMMUNICATOR	(GetGenericCommunicatorDescriptor())
{
//	_pShapeData = new PARTICLE_SHAPE_DATA;
}

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_SHAPE constructor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE::PARTICLE_SHAPE(COMMUNICATOR_DESCRIPTOR *pCD)
:
COMMUNICATOR	(pCD)
{
//	_pShapeData = new PARTICLE_SHAPE_SPRITE_DATA;
}

//-----------------------------------------------------------------------------
//	Name:		~PARTICLE_SHAPE destructor
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE::~PARTICLE_SHAPE()
{
//	delete	(_pShapeData);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::Update(float	rDeltaTime)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
/*MATERIAL *	PARTICLE_SHAPE::GetMaterial() const
{
	return	(NULL);
}*/

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR		*pMD;
	ATTRIBUTE_DESCRIPTOR	*pAD;

	pAD = new	ATTRIBUTE_DESCRIPTOR("_bLifeSpan", QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pAD->SetAccessors(EM_IsLifeSpanActivated, EM_ActivateLifeSpan);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("_rLifeSpan", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetLifeSpan, EM_SetLifeSpan);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("_rMass", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetMass, EM_SetMass);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("_rMomentOfInertia", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetMomentOfInertia, EM_SetMomentOfInertia);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("_nNbrScaleKeys", QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pAD->SetAccessors(EM_GetScaleNbrKeys, EM_SetScaleNbrKeys);
	pCD->AddAttribute(pAD);

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("ActivateLifeSpan"), QDT_COMMENT("Activate life span."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_ACTIVATE_LIFE_SPAN, pMD, EM_ActivateLifeSpan);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsActivateLifeSpan"), QDT_COMMENT("Is life span activated?"));
	pMD->SetReturn(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_IS_LIFE_SPAN_ACTIVATED, pMD, EM_IsLifeSpanActivated);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetLifeSpan"), QDT_COMMENT("Set life span value."));
	pMD->AddParameter(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_LIFE_SPAN, pMD, EM_SetLifeSpan);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetLifeSpan"), QDT_COMMENT("Get life span value."));
	pMD->SetReturn(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_GET_LIFE_SPAN, pMD, EM_GetLifeSpan);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetMass"), QDT_COMMENT("Set mass."));
	pMD->AddParameter(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_MASS, pMD, EM_SetMass);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetMass"), QDT_COMMENT("Get mass."));
	pMD->SetReturn(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_GET_MASS, pMD, EM_GetMass);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetMomentOfInertia"), QDT_COMMENT("Set moment of inertia."));
	pMD->AddParameter(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_MOMENT_OF_INERTIA, pMD, EM_SetMomentOfInertia);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetMomentOfInertia"), QDT_COMMENT("Get moment of inertia."));
	pMD->SetReturn(QDT_COMMENT("rValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_GET_MOMENT_OF_INERTIA, pMD, EM_GetMomentOfInertia);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScale"), QDT_COMMENT("Set a scale key of the particle shape."));
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The position in the scale track of the key to change."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("vScale3d"), QDT_COMMENT(""), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_SCALE, pMD, EM_SetScale);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScale"), QDT_COMMENT("Set a scale key of the particle shape."));
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT("The position in the scale track of the key to change."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("The X coordiante of the scale of the key."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("The Y coordiante of the scale of the key."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT("The Z coordiante of the scale of the key."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_SCALE2, pMD, EM_SetScale2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleNbrKeys"), QDT_COMMENT("Set number of scale keys."));
	pMD->AddParameter(QDT_COMMENT("nValue"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_SCALE_NBR_KEYS, pMD, EM_SetScaleNbrKeys);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetScaleNbrKeys"), QDT_COMMENT("Get number of scale keys."));
	pMD->SetReturn(QDT_COMMENT("nValue"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_GET_SCALE_NBR_KEYS, pMD, EM_GetScaleNbrKeys);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleTimeKey"), QDT_COMMENT("Set scale time key."));
	pMD->AddParameter(QDT_COMMENT("nKeyIndex"), QDT_COMMENT(""), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rTime"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_PSHAPE_SET_SCALE_TIME_KEY, pMD, EM_SetScaleTimeKey);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PARTICLE_SHAPE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_SHAPE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PARTICLE_SHAPE::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PARTICLE_SHAPE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_SHAPE,
										QDT_COMMENT("CD44BD7B-F8F3-9238-9364-18EE1D2CC1DE"), 
										sizeof(PARTICLE_SHAPE), 
										ComInit, 
										QDT_COMMENT("Shape of a particle."),
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_Construct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	PARTICLE_SHAPE *pPS;
	pPS = static_cast<PARTICLE_SHAPE *>(pO);

	QDT_NEW_PLACED(pPS, PARTICLE_SHAPE)();
	QDT_ASSERT(pPS);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_Destruct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	PARTICLE_SHAPE *pPS;
	pPS = static_cast<PARTICLE_SHAPE *>(pO);

	QDT_DELETE_PLACED(pPS, PARTICLE_SHAPE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AcitvateLifeSpawn
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_ActivateLifeSpan(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	bool *				pbValue		 = static_cast<bool*>(pP);

	if (*pbValue)
	{
		pPS->ActivateLifeSpan();
	}
	else
	{
		pPS->DesactivateLifeSpan();
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsLifeSpawnActivated
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_IsLifeSpanActivated(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	bool *				pbValue		 = static_cast<bool*>(pR);

	*pbValue = pPS->IsLifeSpanActive();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLifeSpan
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetLifeSpan(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	float *				prValue		 = static_cast<float*>(pP);

	pPS->SetLifeSpan(*prValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetLifeSpan
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_GetLifeSpan(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	float *				prValue		 = static_cast<float*>(pR);

	*prValue = pPS->GetLifeSpan();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMass
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetMass(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	float *				prValue		 = static_cast<float*>(pP);

	pPS->SetMass(*prValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMass
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_GetMass(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	float *				prValue		 = static_cast<float*>(pR);

	*prValue = pPS->GetMass();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMomentOfInertia
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetMomentOfInertia(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	float *				prValue		 = static_cast<float*>(pP);

	pPS->SetMomentOfInertia(*prValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMomentOfInertia
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_GetMomentOfInertia(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	float *				prValue		 = static_cast<float*>(pR);

	*prValue = pPS->GetMomentOfInertia();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScale
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetScale(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	struct PARAMETERS
	{
		int		_nKeyIndex;
		VECTOR	_vScale;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParameters	 = static_cast<PARAMETERS*>(pP);

	pPS->SetScale(pParameters->_nKeyIndex, pParameters->_vScale);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScale2
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetScale2(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	struct PARAMETERS
	{
		int		_nKeyIndex;
		float	_rX;
		float	_rY;
		float	_rZ;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParameters	 = static_cast<PARAMETERS*>(pP);

	pPS->SetScale(pParameters->_nKeyIndex, VECTOR(pParameters->_rX, pParameters->_rY, pParameters->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleNbrKeys
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetScaleNbrKeys(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	int *				pnValue		 = static_cast<int*>(pP);

	pPS->SetScaleNbKeys(*pnValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetScaleNbrKeys
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_GetScaleNbrKeys(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);;
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	int *				pnValue		 = static_cast<int*>(pR);

	*pnValue = pPS->GetScaleNbKeys();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleTimeKey
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::EM_SetScaleTimeKey(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	struct PARAMETERS
	{
		int		_nKeyIndex;
		float	_rTime;
	};

	COM_INSTANCE *		pComInstance = static_cast<COM_INSTANCE*>(pO);
	PARTICLE_SHAPE *	pPS			 = static_cast<PARTICLE_SHAPE*>(pComInstance->GetCommunicator());
	PARAMETERS *		pParameters	 = static_cast<PARAMETERS*>(pP);

	pPS->SetScaleTimeKey(pParameters->_nKeyIndex, pParameters->_rTime);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
