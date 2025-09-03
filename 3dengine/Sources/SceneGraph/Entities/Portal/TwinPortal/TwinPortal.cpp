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
// CLASS: TWIN_PORTAL
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
// CLASS DESCRIPTION (TWIN_PORTAL)
//
//	The TWIN_PORTAL class implements ...
//
//*****************************************************************************

#include	"..\Include.h"

#include	INC_KRNCOM(BaseType)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)

#include	INC_KRNCORE(Stream)

#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)
#include	INC_3DENGINE(Display)

#ifdef _DEBUG
	#include	"TwinPortal.inl"
#else
	#include	"../Inline.h"

	#include	INL_KRNCORE(Display\GeometryABC)

	#include	INL_3DENGINE(SceneGraph/Node)
	#include	INL_3DENGINE(Display)
	#include	INL_3DENGINE(SceneGraph/Entities/Mesh)
	
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Ray)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Triangle)
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

void*									TWIN_PORTAL::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		TWIN_PORTAL constructor
//	Object:		Default constructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
TWIN_PORTAL::TWIN_PORTAL()
:
PORTAL_ABC			(GetGenericCommunicatorDescriptor(), ENTITY_TWIN_PORTAL),
_TwinPortalNodeInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_bRayCasted			(false)
{

	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TWIN_PORTAL destructor
//	Object:		Destructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
TWIN_PORTAL::~TWIN_PORTAL()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AutoChooseZoneRaycast
//	Object:		
//	04-04-29:	RMA - Created
//-----------------------------------------------------------------------------
bool	TWIN_PORTAL::AutoChooseZoneRaycast(RAY_CAST_PARAMETERS & RayCastParameters, 
										   bool & bContinue)
{
	if	(_TwinPortalNodeInst.HasHandle())
	{
		RAY		LocalRay;
		MATRIX	Inverse;
		VECTOR	PortalDir(0.0f, 0.0f, 1.0f);

		// Compute inverse matrix
		Inverse.InverseAffine(RayCastParameters.GetGlobalMatrix());

		// Get local ray
		LocalRay = RayCastParameters.GetRay();
		LocalRay.Transform(Inverse);
		
		if	(PortalDir.Dot(LocalRay.GetDirection()) < 0.0f)
		{
			if (_pAABBTree)
			{
				if (_pAABBTree->RayCast(RayCastParameters, LocalRay))
				{
					return (true);
				}
			}
#ifdef PLATFORM_PC
			else
			{
				for (int i = 0 ; i < _nNbrGeometries ; ++i)
				{
					if (_pGeometriesArray[i]->RayCast(RayCastParameters, LocalRay, true))
					{
						return (true);
					}
				}
			}
#endif
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	01-09-10:	ELE - Created
//-----------------------------------------------------------------------------
bool	TWIN_PORTAL::RayCast(RAY_CAST_PARAMETERS &	RayCastParameters,
							 bool &					bContinue)
{
	bContinue = true;

	if	(_TwinPortalNodeInst.HasHandle())
	{
		RAY		LocalRay;
		MATRIX	Inverse;
		VECTOR	PortalDir(0.0f, 0.0f, 1.0f);

		// Compute inverse matrix
		Inverse.InverseAffine(RayCastParameters.GetGlobalMatrix());

		// Get local ray
		LocalRay = RayCastParameters.GetRay();
		LocalRay.Transform(Inverse);

		if	(PortalDir.Dot(LocalRay.GetDirection()) < 0.0f)
		{
			// Must be test here !!!!!
			if	(!_bRayCasted)
			{
				// Save old raycast parameters
				RAY_CAST_PARAMETERS OldRayCastParameters = RayCastParameters;
				RayCastParameters.SetRay(LocalRay);
				
				if (_pAABBTree)
				{
					if (_pAABBTree->RayCast(RayCastParameters))
					{
						RayCastParameters = OldRayCastParameters;
						
						NODE	*pTwinPortalNode = static_cast<NODE *>(_TwinPortalNodeInst.GetCommunicator());
						NODE	*pZoneNode = static_cast<NODE *>(pTwinPortalNode->GetZoneNodeInst().GetCommunicator());
						
						RayCastParameters.SetPortalInNodeID(_TwinPortalNodeInst.GetID());
						
						_bRayCasted = true;
						
						pZoneNode->RayCastTree(RayCastParameters);
						
						RayCastParameters.SetPortalInNodeID(OldRayCastParameters.GetPortalInNodeID());
						
						return (false);
					}
				}
				#ifdef PLATFORM_PC
				else
				{	
					for (int i = 0 ; i < _nNbrGeometries ; ++i)
					{
						if (_pGeometriesArray[i]->RayCast(RayCastParameters, LocalRay, true))
						{
							RayCastParameters = OldRayCastParameters;
							
							NODE	*pTwinPortalNode = static_cast<NODE *>(_TwinPortalNodeInst.GetCommunicator());
							NODE	*pZoneNode = static_cast<NODE *>(pTwinPortalNode->GetZoneNodeInst().GetCommunicator());
							
							RayCastParameters.SetPortalInNodeID(_TwinPortalNodeInst.GetID());
							
							_bRayCasted = true;
							
							pZoneNode->RayCastTree(RayCastParameters);
							
							RayCastParameters.SetPortalInNodeID(OldRayCastParameters.GetPortalInNodeID());
							
							return (false);
						}
					}
				}
				#endif

				RayCastParameters = OldRayCastParameters;

				_bRayCasted = true;
			}
			else
			{
				_bRayCasted = false;
			}
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		UpdatePortalZone
//	Object:		
//	01-07-16:	ELE - Created
//-----------------------------------------------------------------------------
bool	TWIN_PORTAL::UpdatePortalZone(const MATRIX &		ViewMatrix,
									  const MATRIX &		NodeGlobalMatrix,
									  SORTED_PORTAL_ZONE *	pPortalZone)
{
	if	(_TwinPortalNodeInst.HasHandle())
	{
		MATRIX		InPortalFlippedGMatrix;
		MATRIX		InPortalReferenceMatrix;
		MATRIX		ViewLMatrix;
		MATRIX		NewViewGMatrix;
		NODE		*pOutPortalNode;

		pOutPortalNode = static_cast<NODE *>(_TwinPortalNodeInst.GetCommunicator());

		// Compute NewViewGMatrix, the global matrix of the new point of view
		InPortalFlippedGMatrix.MakeAffine();
		InPortalFlippedGMatrix.MulAffine(MATRIX::ROTATION_Y_180 , NodeGlobalMatrix);

		InPortalReferenceMatrix.InverseAffine(InPortalFlippedGMatrix);
		ViewLMatrix.MakeAffine();
		ViewLMatrix.MulAffine(ViewMatrix, InPortalReferenceMatrix); 				

		NewViewGMatrix.MakeAffine();
		NewViewGMatrix.MulAffine(ViewLMatrix, pOutPortalNode->GetGlobalMatrix());

		// Set the render parameters
		pPortalZone->SetViewMatrix(NewViewGMatrix);

		return	(true);
	}
	else
	{
		return	(false);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR	*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);
	
	// Methodes	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwinPortal"), QDT_COMMENT("Set the output portal."));
	pMD->AddParameter(QDT_COMMENT("TwinPortalNode"), QDT_COMMENT("Node of the output portal"), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(SET_TWIN_PORTAL, pMD, EM_SetTwinPortal);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("UnSetTwinPortal"), QDT_COMMENT("No output portal."));
	pCD->AddMethod(UNSET_TWIN_PORTAL, pMD, EM_UnsetTwinPortal);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("NoVisibility"), QDT_COMMENT("No visibility test is done."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(NO_VISIBILITY, pMD, EM_NoVisibility);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TWIN_P", 7, CM_ReadChunk7);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TWIN_P", 8, CM_ReadChunk8);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TWIN_P", 9, CM_ReadChunk9);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TWIN_P", 10, CM_ReadChunk10);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TWIN_P", 11, CM_ReadChunk11);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TWIN_P", 12, CM_ReadChunk12);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("TWIN_P", 12, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TWIN_PORTAL::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TWIN_PORTAL) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TWIN_PORTAL::CreateGenericCommunicatorDescriptor()
{
	return (  new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("TWIN_PORTAL"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TWIN_PORTAL,
											QDT_COMMENT("4FA984AF-3838-95CA-C48F-51CB8D74A065"),
											sizeof(TWIN_PORTAL),
											ComInit,
											QDT_COMMENT("Class for twin portals"),
											PORTAL_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pO);

	QDT_NEW_PLACED(pTwinPortal, TWIN_PORTAL)();
	QDT_ASSERT(pTwinPortal);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-11-24:	VMA - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::EM_Construct_In_Memory(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	TWIN_PORTAL	*pTwin = static_cast<TWIN_PORTAL*> ( pO );

	if ( !TWIN_PORTAL::_pVTable	)
	{
		GET_VTABLE( TWIN_PORTAL );
	}
	pTwin->SetVirtualTable( TWIN_PORTAL::_pVTable );
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pO);

	QDT_DELETE_PLACED(pTwinPortal, TWIN_PORTAL);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwinPortal
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::EM_SetTwinPortal(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{

	TWIN_PORTAL		*pTwinPortal;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pTwinPortal		= static_cast<TWIN_PORTAL *>(pComInstance->GetCommunicator());
	pComInstance	= static_cast<COM_INSTANCE *>(pP);

	pTwinPortal->SetTwinPortalNodeInst(*pComInstance);
}

//-----------------------------------------------------------------------------
//	Name:		EM_UnsetTwinPortal
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::EM_UnsetTwinPortal(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	TWIN_PORTAL		*pTwinPortal;
	COM_INSTANCE	*pComInstance;
	
	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pTwinPortal		= static_cast<TWIN_PORTAL *>(pComInstance->GetCommunicator());

	pTwinPortal->SetTwinPortalNodeInst(COM_INSTANCE());
}

//-----------------------------------------------------------------------------
//	Name:		EM_NoVisibility
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::EM_NoVisibility(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	bool			*pParameter;
	TWIN_PORTAL		*pTwinPortal;
	COM_INSTANCE	*pComInstance;

	pComInstance	= static_cast<COM_INSTANCE *>(pO);
	pTwinPortal		= static_cast<TWIN_PORTAL *>(pComInstance->GetCommunicator());
	pParameter		= static_cast<bool *>(pP);

	if (*pParameter)
	{
		pTwinPortal->SetFlags(NO_VISIBILITY, NO_VISIBILITY);
	}
	else
	{
		pTwinPortal->SetFlags(NO_VISIBILITY, 0);
	}
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::CM_ReadChunk7(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 8;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->ReadCAttributes7(Stream, nChunkVersion);

	Stream >> pTwinPortal->_TwinPortalNodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::CM_ReadChunk8(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 8;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->ReadCAttributes8(Stream, nChunkVersion);

	Stream >> pTwinPortal->_TwinPortalNodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::CM_ReadChunk9(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 9;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->ReadCAttributes8(Stream, nChunkVersion);

	Stream >> pTwinPortal->_TwinPortalNodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::CM_ReadChunk10(IO_STREAM &		Stream,
								    COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 9;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->ReadCAttributes9(Stream, nChunkVersion);

	Stream >> pTwinPortal->_TwinPortalNodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk11
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::CM_ReadChunk11(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 9;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->ReadCAttributes10(Stream, nChunkVersion);

	Stream >> pTwinPortal->_TwinPortalNodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk12
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	TWIN_PORTAL::CM_ReadChunk12(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 9;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->ReadCAttributes11(Stream, nChunkVersion);

	Stream >> pTwinPortal->_TwinPortalNodeInst;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	TWIN_PORTAL::CM_WriteChunk(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	int			nChunkVersion = 9;
	TWIN_PORTAL	*pTwinPortal;
	pTwinPortal = static_cast<TWIN_PORTAL *>(pCommunicator);

	pTwinPortal->WriteCAttributes(Stream, nChunkVersion);

	Stream.DebugOutput("Twin portal :\t");
	Stream.DebugOutput(pTwinPortal->GetName());
	Stream.DebugOutput("\r\n");

	Stream << pTwinPortal->_TwinPortalNodeInst;

	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
