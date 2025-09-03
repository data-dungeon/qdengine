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
// CLASS: FORCE_FIELD_ABC
//
//
//	01-03-12:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (FORCE_FIELD_ABC)
//
//	The FORCE_FIELD_ABC class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/ForceFieldABC)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(ComplexType/TypeVector)

#ifdef _DEBUG
	#include	"ForceFieldABC.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC constructor
//	Object:		Defualt constructor
//	01-03-13:	RMA - Created
//-----------------------------------------------------------------------------
FORCE_FIELD_ABC::FORCE_FIELD_ABC(COMMUNICATOR_DESCRIPTOR *	pCD,
								 ENTITY_TYPE				eEntityType)
:
ENTITY_ABC		(pCD, eEntityType),
_rMagnitude		(1.0f),
_Direction		(1.0f, 0.0f, 0.0f),
_nShapeType		(FF_SHAPETYPE_NONE),
_pSphere		(NULL),
_pTransSphere	(NULL),
_eType			(TFF_UNKNOWN),
_bTransformed	(false),
_bActive		(true)
{
}

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC destructor
//	Object:		Destructor
//	01-03-13:	RMA - Created
//-----------------------------------------------------------------------------
FORCE_FIELD_ABC::~FORCE_FIELD_ABC()
{
	DeleteInfluenceShape();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
#ifndef _MASTER
	if	(DISPLAY_MANAGER::Instance()->DisplayForceFields())
	{
		COLOR_FLOAT	ColorFloat(1.0f, 0.0f, 1.0f, 1.0f);
		VECTOR		Point(VECTOR::ZERO);

		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&(DisplayParameters.GetNode()->GetGlobalMatrix()));

		switch	(_nShapeType)
		{
			default:
			case FF_SHAPETYPE_NONE:

				Point.Display(ColorFloat, 5.0f);
				break;

			case FF_SHAPETYPE_SPHERE:

				_pSphere->Display(ColorFloat);
				break;

			case FF_SHAPETYPE_OBB:

				_pObb->Display(ColorFloat);
				break;
		}
	}
#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::Update()
{
	SetTransformed(false);
}

//-----------------------------------------------------------------------------
//	Name:		ToUpdate
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::ToUpdate() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		SetInfluenceShape
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::SetInfluenceShape(const SPHERE &	Sphere)
{
	DeleteInfluenceShape();

	_nShapeType = FF_SHAPETYPE_SPHERE;

	_pSphere = new SPHERE;
	_pTransSphere = new SPHERE;

	*_pSphere = Sphere;
}

//-----------------------------------------------------------------------------
//	Name:		SetInfluenceShape
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::SetInfluenceShape(const OBB &	Obb)
{
	DeleteInfluenceShape();

	_nShapeType = FF_SHAPETYPE_OBB;
	
	_pObb = new OBB;
	_pTransObb = new OBB;

	*_pObb = Obb;
}
/*
//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::TestInfluenceVolume
//	Object:		Test the intersection with the influence volume of the force
//				force field & a physic body.
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
bool FORCE_FIELD_ABC::TestInfluenceVolume(const PARTICLE & Particle) const
{
	switch	(_nShapeType)
	{
		default:
		case FF_SHAPETYPE_NONE:

			return	(false);
			break;

		case FF_SHAPETYPE_SPHERE:

			return (INTERSECTION::TestIntersectionPointSphere(Particle.GetPosition(), *_pTransSphere));
			break;

		case FF_SHAPETYPE_OBB:

			//return (INTERSECTION::TestIntersectionPointOBB(*_pTransObb, Particle.GetPosition()));
			return ( (*_pTransObb).Intersect(Particle.GetPosition()) );
			break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::TestInfluenceVolume
//	Object:		Test the intersection with the influence volume of the force
//				force field & a physic body.
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::TestInfluenceVolume(PARTICLE_SPRITE_ARRAY * pParticleArray, 
											 unsigned int i) const
{
	switch	(_nShapeType)
	{
		default:
		case FF_SHAPETYPE_NONE:
			return	(false);

		case FF_SHAPETYPE_SPHERE:
			{
				VECTOR Position;
				pParticleArray->GetPosition(i, Position);
				return (INTERSECTION::TestIntersectionPointSphere(Position, *_pTransSphere));
			}

		case FF_SHAPETYPE_OBB:
			{
				VECTOR Position;
				pParticleArray->GetPosition(i, Position);
				return _pTransObb->Intersect(Position);
			}
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::UpdateGlobalPosition
//	Object:		Update the global position, orientation and bounding volume
//				in the global coordinate system
//				The parameter is :
//					* the global matrix of the force field
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void FORCE_FIELD_ABC::TransformedDatas(const MATRIX & GlobalForceFieldMatrix)
{
//	if	(!_bTransformed)
	{
		// In the global coordinate system
		GlobalForceFieldMatrix.TransformPosition(VECTOR::ZERO, _TransCenter);
		GlobalForceFieldMatrix.TransformDirection(_Direction, _TransDirection);

		switch	(_nShapeType)
		{
			default:
			case FF_SHAPETYPE_NONE:
				break;

			case FF_SHAPETYPE_SPHERE:

				*_pTransSphere	= *_pSphere;
				_pTransSphere->Transform(GlobalForceFieldMatrix);
				break;

			case FF_SHAPETYPE_OBB:

				*_pTransObb	= *_pObb;
				_pTransObb->Transform(GlobalForceFieldMatrix);
				break;
		}

		_bTransformed = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DeleteInfluenceShape
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::DeleteInfluenceShape()
{
	switch	(_nShapeType)
	{
		default:
		case FF_SHAPETYPE_NONE:
			break;

		case FF_SHAPETYPE_SPHERE:

			if	(_pSphere)
			{
				delete	_pSphere;
			}
			break;

		case FF_SHAPETYPE_OBB:

			if	(_pObb)
			{
				delete	_pObb;
			}
			break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReadCommon
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	FORCE_FIELD_ABC::ReadCommon(IO_STREAM &	Stream)
{
	Stream	>> _rMagnitude
			>> _Direction;

	Stream	>> _nShapeType;
	switch	(_nShapeType)
	{
		default:
		case FF_SHAPETYPE_NONE:
			break;

		case FF_SHAPETYPE_SPHERE:
			{
				SPHERE	Sphere;
				Stream	>> Sphere;
				
				SetInfluenceShape(Sphere);
			}
			break;

		case FF_SHAPETYPE_OBB:
			{
				OBB	Obb;
				Stream	>> Obb;

				SetInfluenceShape(Obb);
			}
			break;
	}

	int type;
	Stream	>> type;

	SetType( (TYPE_FORCE_FIELD)type );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteCommon
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	FORCE_FIELD_ABC::WriteCommon(IO_STREAM &	Stream)
{
	Stream.DebugOutput("Magnitude : ");
	Stream	<< _rMagnitude;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Magnitude : ");
	Stream	<< _Direction;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Shape type : ");
	Stream << _nShapeType;
	Stream.DebugOutput("\r\n");

	switch	(_nShapeType)
	{
		default:
			
		case FF_SHAPETYPE_NONE:	
			break;

		case FF_SHAPETYPE_SPHERE:
			Stream	<< *_pSphere;
			Stream.DebugOutput("\r\n");
			break;

		case FF_SHAPETYPE_OBB:
			Stream	<< *_pObb;
			Stream.DebugOutput("\r\n");
			break;
	}

	Stream.DebugOutput("Type : ");
	Stream	<< _eType;
	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::SetMagnitude
//	Object:		Set the magnitude
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void FORCE_FIELD_ABC::SetMagnitude(float rValue)
{
	_rMagnitude = rValue;
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR	*pMD;
	ATTRIBUTE_DESCRIPTOR	*pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Attributes
	pAD = new	ATTRIBUTE_DESCRIPTOR("magnitude", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetMagnitude, EM_SetMagnitude);
	pCD->AddAttribute(pAD);

	// Methodes	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetMagnitude"), QDT_COMMENT("Set the magnitude of the force field."));
	pMD->AddParameter(QDT_COMMENT("rMagnitude"), QDT_COMMENT("Magnitude of the force field"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_MAGNITUDE, pMD, EM_SetMagnitude);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetMagnitude"), QDT_COMMENT("Get the magnitude of the force field."));
	pMD->SetReturn(QDT_COMMENT("rMagnitude"), QDT_COMMENT("Magnitude of the force field"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_MAGNITUDE, pMD, EM_GetMagnitude);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDirection"), QDT_COMMENT("Set the vector direction of the force field."));
	pMD->AddParameter(QDT_COMMENT("Vector"), QDT_COMMENT("Vector direction of the force field"), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_SET_DIRECTION_1, pMD, EM_SetDirection1);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetDirection"), QDT_COMMENT("Get the vector direction of the force field."));
	pMD->SetReturn(QDT_COMMENT("Vector"), QDT_COMMENT("Vector direction of the force field"), TYPE_VECTOR::GetDescriptor());
	pCD->AddMethod(CMD_GET_DIRECTION_1, pMD, EM_GetDirection1);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDirection"), QDT_COMMENT("Set the vector direction of the force field."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("Vector direction x component"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("Vector direction y component"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rZ"), QDT_COMMENT("Vector direction z component"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_DIRECTION_2, pMD, EM_SetDirection2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetDirection"), QDT_COMMENT("Get the vector direction of the force field."));
	pMD->AddParameterReference(QDT_COMMENT("rX"), QDT_COMMENT("Vector direction x component"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameterReference(QDT_COMMENT("rY"), QDT_COMMENT("Vector direction y component"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameterReference(QDT_COMMENT("rZ"), QDT_COMMENT("Vector direction z component"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_DIRECTION_2, pMD, EM_GetDirection2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetInfluenceSphere"), QDT_COMMENT("Set influence sphere of the force field."));
	pMD->AddParameter(QDT_COMMENT("rRadius"), QDT_COMMENT("Influence sphere radius"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_INFLUENCE_SPHERE, pMD, EM_SetInfluenceSphere);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetInfluenceBox"), QDT_COMMENT("Set influence box of the force field."));
	pMD->AddParameter(QDT_COMMENT("rSizeX"), QDT_COMMENT("X size of the box"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSizeY"), QDT_COMMENT("Y size of the box"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSizeZ"), QDT_COMMENT("Z size of the box"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_INFLUENCE_BOX, pMD, EM_SetInfluenceBox);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetActive"), QDT_COMMENT("Set the activity of the force field."));
	pMD->AddParameter(QDT_COMMENT("bStatus"), QDT_COMMENT("Activity of the force field"), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE, pMD, EM_SetActive);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetActive"), QDT_COMMENT("return the activity of the force field."));
	pMD->SetReturn(QDT_COMMENT("bStatus"), QDT_COMMENT("Activity of the force field"), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_ACTIVE, pMD, EM_IsActive);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	FORCE_FIELD_ABC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FORCE_FIELD_ABC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	FORCE_FIELD_ABC::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("FORCE_FIELD_ABC"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FORCE_FIELD_ABC,
											QDT_COMMENT("CD5A2E5A-D2F7-FE4C-E0C8-FBE192F284AA"),
											sizeof(FORCE_FIELD_ABC),
											ComInit,
											QDT_COMMENT("Abstract class for force fields"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_Construct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_Destruct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMagnitude
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_SetMagnitude(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	float			*pParameter;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<float *>(pP);

	pForceField->SetMagnitude(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMagnitude
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_GetMagnitude(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	float			*pReturn;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pReturn			= static_cast<float *>(pR);

	*pReturn = pForceField->GetMagnitude();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDirection1
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_SetDirection1(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	VECTOR			*pParameter;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<VECTOR *>(pP);

	pForceField->SetDirection(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetDirection1
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_GetDirection1(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	VECTOR			*pReturn;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pReturn			= static_cast<VECTOR *>(pR);

	QDT_NEW_PLACED(pReturn, VECTOR)(pForceField->GetDirection());
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDirection2
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_SetDirection2(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	struct PARAMETERS
	{
		float	_rX;
		float	_rY;
		float	_rZ;
	};

	PARAMETERS		*pParameters;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	VECTOR	V(pParameters->_rX, pParameters->_rY, pParameters->_rZ);
	pForceField->SetDirection(V);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetDirection2
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_GetDirection2(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	struct PARAMETERS
	{
		float	*_prX;
		float	*_prY;
		float	*_prZ;
	};

	PARAMETERS		*pParameters;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	const VECTOR	&V = pForceField->GetDirection();
	
	*pParameters->_prX = V.GetX();
	*pParameters->_prY = V.GetY();
	*pParameters->_prZ = V.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetInfluenceSphere
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_SetInfluenceSphere(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	float			*pParameter;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<float *>(pP);

	SPHERE	Sphere(VECTOR::ZERO, *pParameter);

	pForceField->SetInfluenceShape(Sphere);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetInfluenceBox
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_SetInfluenceBox(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	struct PARAMETERS
	{
		float	_rSizeX;
		float	_rSizeY;
		float	_rSizeZ;
	};

	PARAMETERS		*pParameters;
	FORCE_FIELD_ABC	*pForceField;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pForceField		= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	pParameters		= static_cast<PARAMETERS *>(pP);

	OBB		Obb;

	Obb.SetCenter(VECTOR::ZERO);
	Obb.SetDirection1(VECTOR::UNIT_X);
	Obb.SetDirection2(VECTOR::UNIT_Y);
	Obb.SetDirection3(VECTOR::UNIT_Z);
	Obb.SetDistance1(pParameters->_rSizeX * 0.5f);
	Obb.SetDistance2(pParameters->_rSizeY * 0.5f);
	Obb.SetDistance3(pParameters->_rSizeZ * 0.5f);

	pForceField->SetInfluenceShape(Obb);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActive
//	Object:		
//	04-11-08:	RMA - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_SetActive(void * const pO, 
									  void * const pR, 
									  void * const pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	FORCE_FIELD_ABC	*pForceField	= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	bool			*pParameter		= static_cast<bool *>(pP);
	
	pForceField->SetActive(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsActive
//	Object:		
//	04-11-08:	RMA - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::EM_IsActive(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	FORCE_FIELD_ABC	*pForceField	= static_cast<FORCE_FIELD_ABC *>(pComInstance->GetCommunicator());
	bool			*pParameter		= static_cast<bool *>(pR);
	
	*pParameter = pForceField->IsActive();
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
