//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: JOINT
//
//
//	00-11-13	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( JOINT )
//
//	The JOINT class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Joint/Joint)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/BvFactory)

#ifdef _DEBUG
	#include "Joint.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: JOINT constructor
//	Object: Default constructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
JOINT::JOINT()
:
ENTITY_ABC			(GetGenericCommunicatorDescriptor(), ENTITY_JOINT),
_Type				(JT_NONE),
_nCollisionVType	(NONE),
_bJointIsFoot		(false),
_rDistanceFromFloor	(0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: JOINT destructor
//	Object: Destructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
JOINT::~JOINT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: JOINT::operator 
//	Object: Operator =
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
JOINT & JOINT::operator = (const JOINT & Joint)
{
	if (this != &Joint)
	{
		Copy(Joint) ;
	}

	return (*this) ;
}

//-----------------------------------------------------------------------------
//	Name: JOINT::Display
//	Object: Display
//			This method display a small yellow sphere at the joint position if
//			need
//	01-02-14:	RMA - Created
//-----------------------------------------------------------------------------
void JOINT::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
#ifndef _MASTER
	static const float			SPHERE_SIZE = 0.3f ;
	static const COLOR_FLOAT	BLUE(0.0f, 0.0f, 0.0f, 1.0f) ;
	static const int			NBR_STEPS = 2 ;						// Nbr of steps for the sphere tesselation
	static const SPHERE			JOINT_SPHERE(VECTOR::ZERO, SPHERE_SIZE) ;

	if (DISPLAY_MANAGER::Instance()->DisplayJoints())
	{
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
		RENDER_MANAGER::Instance()->GetRenderContext()->RenderFrame(VECTOR::ZERO, 4.0f) ;				
		RENDER_MANAGER::Instance()->GetRenderContext()->RenderWireSphere(JOINT_SPHERE, NBR_STEPS, BLUE) ;
	}	
#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	JOINT::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	JOINT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("JOINT", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("JOINT", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("JOINT", 3, CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("JOINT", 4, CM_ReadChunk4);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("JOINT", 5, CM_ReadChunk5);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("JOINT", 5, CM_WriteChunk5);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	JOINT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_JOINT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	JOINT::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("JOINT"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_JOINT,
											QDT_COMMENT("A63072FA-17B6-782E-1AC4-D70EE60C5D7C"),
											sizeof(JOINT),
											ComInit,
											QDT_COMMENT("Class for joints"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	JOINT::EM_Construct(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	JOINT	*pJoint;
	pJoint = static_cast<JOINT *>(pO);

	QDT_NEW_PLACED(pJoint, JOINT)();
	QDT_ASSERT(pJoint);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	JOINT::EM_Destruct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	JOINT	*pJoint;
	pJoint = static_cast<JOINT *>(pO);

	QDT_DELETE_PLACED(pJoint, JOINT);
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	JOINT::CM_ReadChunk1(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	JOINT	*pJoint;
	pJoint = static_cast<JOINT *>(pCommunicator);

	Stream	>> pJoint->_nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	JOINT::CM_ReadChunk2(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	JOINT	*pJoint;
	int		 nType;
	pJoint = static_cast<JOINT *>(pCommunicator);

	Stream	>> pJoint->_nIndex;
	Stream  >> nType;
	pJoint->_Type = static_cast<JOINT_TYPE>(nType);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::CM_ReadChunk3(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	JOINT	*pJoint;
	int		 nType;
	pJoint = static_cast<JOINT *>(pCommunicator);

	Stream	>> pJoint->_nIndex;
	Stream  >> nType;
	pJoint->_Type = static_cast<JOINT_TYPE>(nType);

	Stream >> pJoint->_nCollisionVType;

	switch(pJoint->_nCollisionVType)
	{

	default:
		QDT_FAIL();
		break;

	case NONE:
		break;

	case COLLISION_OBB:
		{
			Stream >> pJoint->_CollisionOBB;
		}
		break;

	case COLLISION_CYLINDER:
		{
			Stream >> pJoint->_CollisionCylinder;
		}
		break;
		
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	04-01-05:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::CM_ReadChunk4(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	JOINT	*pJoint;
	int		 nType;
	pJoint = static_cast<JOINT *>(pCommunicator);

	Stream	>> pJoint->_nIndex;
	Stream  >> nType;
	pJoint->_Type = static_cast<JOINT_TYPE>(nType);

	Stream >> pJoint->_nCollisionVType;

	switch(pJoint->_nCollisionVType)
	{

	default:
		QDT_FAIL();
		break;

	case NONE:
		break;

	case COLLISION_OBB:
		{
			Stream >> pJoint->_CollisionOBB;
		}
		break;

	case COLLISION_CYLINDER:
		{
			Stream >> pJoint->_CollisionCylinder;
		}
		break;
		
	}


	int nNbrVol, nEachVol;
	Stream >> nNbrVol;
	for(nEachVol=0; nEachVol<nNbrVol ; ++nEachVol)
	{
		int nType;
		Stream >> nType;
		switch(nType)
		{
		default:
			QDT_FAIL();
			break;

		case BOUNDING_VOLUME::TYPE_SPHERE:
			{
				BOUNDING_VOLUME* pBv = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_SPHERE);
				Stream >> *pBv;
				pJoint->AddSoftBodyCollisionVolume(pBv);
				
			}
			break;

		case BOUNDING_VOLUME::TYPE_AABB:
			{
				BOUNDING_VOLUME* pBv = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_AABB);
				Stream >> *pBv;
				pJoint->AddSoftBodyCollisionVolume(pBv);
			}
			break;

		case BOUNDING_VOLUME::TYPE_CYLINDER:
			{
				BOUNDING_VOLUME* pBv = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_CYLINDER);
				Stream >> *pBv;
				pJoint->AddSoftBodyCollisionVolume(pBv);
			}
			break;
		}			
		
	}
}


//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk5
//	Object:		
//	04-01-07:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::CM_ReadChunk5(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	JOINT	*pJoint;
	int		 nType;
	pJoint = static_cast<JOINT *>(pCommunicator);

	Stream	>> pJoint->_nIndex;
	Stream  >> nType;
	pJoint->_Type = static_cast<JOINT_TYPE>(nType);

	Stream >> pJoint->_nCollisionVType;

	switch(pJoint->_nCollisionVType)
	{

	default:
		QDT_FAIL();
		break;

	case NONE:
		break;

	case COLLISION_OBB:
		{
			Stream >> pJoint->_CollisionOBB;
		}
		break;

	case COLLISION_CYLINDER:
		{
			Stream >> pJoint->_CollisionCylinder;
		}
		break;
		
	}


	int nNbrVol, nEachVol;
	Stream >> nNbrVol;
	for(nEachVol=0; nEachVol<nNbrVol ; ++nEachVol)
	{
		int nType;
		Stream >> nType;
		switch(nType)
		{
		default:
			QDT_FAIL();
			break;

		case BOUNDING_VOLUME::TYPE_SPHERE:
			{
				BOUNDING_VOLUME* pBv = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_SPHERE);
				Stream >> *pBv;
				pJoint->AddSoftBodyCollisionVolume(pBv);
				
			}
			break;

		case BOUNDING_VOLUME::TYPE_AABB:
			{
				BOUNDING_VOLUME* pBv = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_AABB);
				Stream >> *pBv;
				pJoint->AddSoftBodyCollisionVolume(pBv);
			}
			break;

		case BOUNDING_VOLUME::TYPE_CYLINDER:
			{
				BOUNDING_VOLUME* pBv = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_CYLINDER);
				Stream >> *pBv;
				pJoint->AddSoftBodyCollisionVolume(pBv);
			}
			break;
		}			
		
	}


	Stream >> pJoint->_bJointIsFoot;
	Stream >> pJoint->_rDistanceFromFloor;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk5
//	Object:		
//	04-01-07:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	JOINT::CM_WriteChunk5(IO_STREAM &		Stream,
							  COMMUNICATOR *	pCommunicator)
{
	JOINT	*pJoint;
	int		 nType;
	pJoint = static_cast<JOINT *>(pCommunicator);

	Stream.DebugOutput("Joint :\t");
	Stream.DebugOutput(pJoint->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Index :\t");
	Stream	<< pJoint->_nIndex;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Type : \t");
	nType = pJoint->_Type;
	Stream << nType;
	Stream.DebugOutput("\r\n");

	Stream << pJoint->_nCollisionVType;		
	switch(pJoint->_nCollisionVType)
	{
		default:
			QDT_FAIL();
		break;

		case NONE:
			break;

		case COLLISION_OBB:
			{
				Stream.DebugOutput("Collision OBB :\t");
				Stream << pJoint->_CollisionOBB;
				Stream.DebugOutput("\r\n");	
			}
			break;

		case COLLISION_CYLINDER:
			{
				Stream.DebugOutput("Collision Cylinder :\t");
				Stream << pJoint->_CollisionCylinder;
				Stream.DebugOutput("\r\n");	
			}
			break;	
	}

	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Intersection with soft body Volumes :\r\n");
	int nNbrVol = pJoint->_vCollisionVolumes.GetSize(), nEachVol;
	Stream << nNbrVol;
	Stream.DebugOutput("\r\n");
	for(nEachVol=0; nEachVol<nNbrVol ; ++nEachVol)
	{
		Stream << pJoint->_vCollisionVolumes[nEachVol]->GetType();
		Stream << *pJoint->_vCollisionVolumes[nEachVol];
		Stream.DebugOutput("\r\n");
	}		
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Joint is a foot :\r\n");
	Stream << pJoint->_bJointIsFoot;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Distance from floor :\r\n");
	Stream << pJoint->_rDistanceFromFloor;
	Stream.DebugOutput("\r\n");

}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
