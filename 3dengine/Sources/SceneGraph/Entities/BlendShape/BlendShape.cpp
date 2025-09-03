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
//	CLASS:	BLEND_SHAPE
//
//	01-06-12:	ELE - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#define	BLEND_SHAPE_MEM_OPT

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	_DEBUG

#include	"..\Mesh\include.h"

#include	INC_KRNCOM(DataBank)

#include	INC_KRNCORE(Stream)

#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(SceneGraph\Entities\Light)
#include	INC_3DENGINE(Behavior\Controllers\Entity Controllers)
#include	INC_3DENGINE(Display)

#ifdef _DEBUG
	#include	"BlendShape.inl"
#else
	#include	"..\Mesh\Inline.h"

	#include	INL_KRNMATH(Math/Geometry/Shapes/Shapes1D/Vector)

	#include	INL_3DENGINE(SceneGraph\Node)
	#include	INL_3DENGINE(Display)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE constructor
//	Object:		
//	01-06-12:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE::BLEND_SHAPE()
:
MESH				(GetGenericCommunicatorDescriptor(), ENTITY_BLEND_SHAPE),
_bIValid			(false),
//_pBlendShapeDatas	(NULL),
_prWeights			(NULL),
_nNumId				( 0 )
{
	// Don onthing
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE destructor
//	Object:		
//	01-06-12:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE::~BLEND_SHAPE()
{
	if	(_prWeights)
	{
		delete [] _prWeights;
		_prWeights = NULL;
	}

//	delete	(_pBlendShapeDatas);
}

//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShape
//	Object:		
//	02-03-04:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::ApplyBlendShape(const DISPLAY_PARAMETERS & DisplayParameters)
{
	NODE					*pNode = DisplayParameters.GetNode();
	ENTITY_CONTROLLER_ABC	*pController = pNode->GetController(_nNumId);

	if (pController)
	{
		BLEND_SHAPE_CONTROLLER *pBSController = pController->GetBlendShapeController();
		QDT_ASSERT(pBSController);
		SetWeights(pBSController->GetWeights());

		Interpolate();
	}
}

//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShape
//	Object:		
//	03-07-23:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::ApplyBlendShape(const DISPLAY_PARAMETERS &	DisplayParameters,
									 int						nGeometryIndex)
{
	NODE					*pNode = DisplayParameters.GetNode();
	ENTITY_CONTROLLER_ABC	*pController = pNode->GetController(_nNumId);

	if (pController)
	{
		BLEND_SHAPE_CONTROLLER *pBSController = pController->GetBlendShapeController();
		QDT_ASSERT(pBSController);
		SetWeights(pBSController->GetWeights());

		Interpolate(nGeometryIndex);
	}
}
/*
//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShape
//	Object:		
//	03-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::ApplyBlendShape(const DISPLAY_PARAMETERS &	DisplayParameters,
									 GEOMETRY_REF *				pGeometryRef)
{
	NODE					*pNode = DisplayParameters.GetNode();
	ENTITY_CONTROLLER_ABC	*pController = pNode->GetController(_nNumId);

	if (pController)
	{
		BLEND_SHAPE_CONTROLLER *pBSController = pController->GetBlendShapeController();
		QDT_ASSERT(pBSController);

		pGeometryRef->SetWeights(pBSController->GetWeights());
		pGeometryRef->SetNbrBlendShapeTarget(GetNbrWeights());
		pGeometryRef->SetBlendShapeDatas(GetBlendShapeDatas());
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		Interpolate
//	Object:		
//	01-06-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::Interpolate()
{
	QDT_NOT_IMPLEMENTED();
/*
	int	nGeometryIndex;
	
	if	(_bIValid == false)
	{
		// Initialisation
		for	(nGeometryIndex = 0 ; nGeometryIndex < _nNbrGeometries ; ++nGeometryIndex)
		{
			_pGeometriesArray[nGeometryIndex]->SetTransformationFlags(GEOMETRY_ABC::TR_BLEND_SHAPE, GEOMETRY_ABC::TR_BLEND_SHAPE);

			_pGeometriesArray[nGeometryIndex]->PrepareForBlendShape();
		}

		NewInterpolate();

		_bIValid = true;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		Interpolate
//	Object:		
//	03-07-23:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::Interpolate(int	nGeometryIndex)
{
	QDT_NOT_IMPLEMENTED();
/*
	GEOMETRY_ABC	*pGeometry = _pGeometriesArray[nGeometryIndex];

	if	(pGeometry->IsBlendShapeValid() == false)
	{
		int	nTargetIndex, nNbrTargets = _pBlendShapeDatas->GetNbrTargets();

		pGeometry->SetTransformationFlags(GEOMETRY_ABC::TR_BLEND_SHAPE, GEOMETRY_ABC::TR_BLEND_SHAPE);
		pGeometry->PrepareForBlendShape();

		for	(nTargetIndex = 0 ; nTargetIndex < nNbrTargets ; ++nTargetIndex)
		{
			if	(_prWeights[nTargetIndex] != 0.0)
			{
				pGeometry->ApplyBlendShape(_prWeights[nTargetIndex],
										   _pBlendShapeDatas->GetNbrVertices(nTargetIndex, nGeometryIndex),
										   _pBlendShapeDatas->GetVertices(nTargetIndex, nGeometryIndex),
										   _pBlendShapeDatas->GetNormals(nTargetIndex, nGeometryIndex),
										   NULL,
										   _pBlendShapeDatas->GetNormalsRef(nTargetIndex, nGeometryIndex),
										   true);
			}
		}

		if	(pGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_STRIPS)
		{
#ifdef	BLEND_SHAPE_MEM_OPT
			pGeometry->UpdateStrippingDatas();
#else
			pGeometry->UpdateTrStrippingDatas();
#endif
		}

		pGeometry->SetBlendShapeValid(true);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		NewInterpolate
//	Object:		
//	01-12-03:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::NewInterpolate()
{
	QDT_NOT_IMPLEMENTED();
/*
	int	nNbrTargets = _pBlendShapeDatas->GetNbrTargets();
	int	nTargetIndex, nGeometryIndex;

	// Interpolation of the vertices and normals which move threw the blend shape deformer
	for	(nTargetIndex = 0 ; nTargetIndex < nNbrTargets ; ++nTargetIndex)
	{
		if	(_prWeights[nTargetIndex] != 0.0)
		{
			for	(nGeometryIndex = 0 ; nGeometryIndex < _nNbrGeometries ; ++nGeometryIndex)
			{
				_pGeometriesArray[nGeometryIndex]->ApplyBlendShape(_prWeights[nTargetIndex],
																   _pBlendShapeDatas->GetNbrVertices(nTargetIndex, nGeometryIndex),
																   _pBlendShapeDatas->GetVertices(nTargetIndex, nGeometryIndex),
																   _pBlendShapeDatas->GetNormals(nTargetIndex, nGeometryIndex),
																   NULL,
																   _pBlendShapeDatas->GetNormalsRef(nTargetIndex, nGeometryIndex),
																   true);
			}
		}
	}

	if	(_pGeometriesArray[0]->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_STRIPS)
	{
		for	(nGeometryIndex = 0 ; nGeometryIndex < _nNbrGeometries ; ++nGeometryIndex)
		{
#ifdef	BLEND_SHAPE_MEM_OPT
			_pGeometriesArray[nGeometryIndex]->UpdateStrippingDatas();
#else
			_pGeometriesArray[nGeometryIndex]->UpdateTrStrippingDatas();
#endif
			_pGeometriesArray[nGeometryIndex]->SetBlendShapeValid(true);
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		SetWeights
//	Object:		TEMP Set weights TEMP
//	01-07-06:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::SetWeights(float *prWeights)
{
	QDT_NOT_IMPLEMENTED();
/*
	int nEachWeight;
	int	nNbrTargets = _pBlendShapeDatas->GetNbrTargets();

	if (prWeights == NULL) return;

	for (nEachWeight = 0 ; nEachWeight < nNbrTargets ; ++nEachWeight)
	{
		_prWeights[nEachWeight] = prWeights[nEachWeight];
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		Update is called during the update behaviors process
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::Update()
{
	SetValid(false);
}

//-----------------------------------------------------------------------------
//	Name:		ToUpdate
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	BLEND_SHAPE::ToUpdate() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("BSHAPE", 2, CM_ReadChunk);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("BSHAPE", 3, CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("BSHAPE", 4, CM_ReadChunk4);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("BSHAPE", 5, CM_ReadChunk5);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("BSHAPE", 5, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	BLEND_SHAPE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	BLEND_SHAPE::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("BLEND_SHAPE"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE,
											QDT_COMMENT("762EEABD-F92D-F98A-256E-0AD04F8612D0"),
											sizeof(BLEND_SHAPE),
											ComInit,
											QDT_COMMENT("Class for blend shapes"),
											MESH::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	BLEND_SHAPE	*pBlendShape;
	pBlendShape = static_cast<BLEND_SHAPE *>(pO);

	QDT_NEW_PLACED(pBlendShape, BLEND_SHAPE)();
	QDT_ASSERT(pBlendShape);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	BLEND_SHAPE	*pBlendShape;
	pBlendShape = static_cast<BLEND_SHAPE *>(pO);

	QDT_DELETE_PLACED(pBlendShape, BLEND_SHAPE);
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::CM_ReadChunk(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	int			i, nNbrTargetShapes;
	BLEND_SHAPE	*pBlendShape;
	pBlendShape = static_cast<BLEND_SHAPE *>(pCommunicator);

//	pBlendShape->_pBlendShapeDatas = new BLEND_SHAPE_DATAS;
//	pBlendShape->_pBlendShapeDatas->SetVersion(2);
//	pBlendShape->_pBlendShapeDatas->SetNbrGeometries(pBlendShape->_nNbrGeometries);
//	pBlendShape->_pBlendShapeDatas->SetGeometries(pBlendShape->_pGeometriesArray);
//	pBlendShape->_pBlendShapeDatas->SetDataBankExport( DATA_BANK::IsExporting() );
//	Stream >> *pBlendShape->_pBlendShapeDatas;
//	pBlendShape->_pBlendShapeDatas->SetDataBankExport(false);

	nNbrTargetShapes = 0;//pBlendShape->_pBlendShapeDatas->GetNbrTargets();
//	pBlendShape->_prWeights = new float[nNbrTargetShapes];

	for	(i = 0 ; i < nNbrTargetShapes ; ++i)
	{
		pBlendShape->_prWeights[i] = 0.0f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	03-08-12:	SBE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::CM_ReadChunk3(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	int			i, nNbrTargetShapes;
	BLEND_SHAPE	*pBlendShape;
	pBlendShape = static_cast<BLEND_SHAPE *>(pCommunicator);

	//	optional line
	pBlendShape->_nNumId = 0;//	Default value for previous versions


//	pBlendShape->_pBlendShapeDatas = new BLEND_SHAPE_DATAS;
//	pBlendShape->_pBlendShapeDatas->SetVersion(3);
//	pBlendShape->_pBlendShapeDatas->SetNbrGeometries(pBlendShape->_nNbrGeometries);
//	pBlendShape->_pBlendShapeDatas->SetGeometries(pBlendShape->_pGeometriesArray);
//	pBlendShape->_pBlendShapeDatas->SetDataBankExport( DATA_BANK::IsExporting() );
//	Stream >> *pBlendShape->_pBlendShapeDatas;
//	pBlendShape->_pBlendShapeDatas->SetDataBankExport(false);

	nNbrTargetShapes = 0;//pBlendShape->_pBlendShapeDatas->GetNbrTargets();
//	pBlendShape->_prWeights = new float[nNbrTargetShapes];

	for	(i = 0 ; i < nNbrTargetShapes ; ++i)
	{
		pBlendShape->_prWeights[i] = 0.0f;
	}
	
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	04-11-23:	CVI - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::CM_ReadChunk4(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	QDT_Error("Blend shape version 4 obolete! Please rexport %s", pCommunicator->GetName());
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk5
//	Object:		
//	04-11-23:	CVI - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE::CM_ReadChunk5(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	int			i, nNbrTargetShapes;
	BLEND_SHAPE	*pBlendShape;

	pBlendShape = static_cast<BLEND_SHAPE *>(pCommunicator);
	Stream >> pBlendShape->_nNumId;		//	Identifiant for multiple blend shapes

//	pBlendShape->_pBlendShapeDatas = new BLEND_SHAPE_DATAS;
//	pBlendShape->_pBlendShapeDatas->SetVersion(3);
//	pBlendShape->_pBlendShapeDatas->SetNbrGeometries(pBlendShape->_nNbrGeometries);
//	pBlendShape->_pBlendShapeDatas->SetGeometries(pBlendShape->_pGeometriesArray);
//	pBlendShape->_pBlendShapeDatas->SetDataBankExport( DATA_BANK::IsExporting() );
//	Stream >> *pBlendShape->_pBlendShapeDatas;
//	pBlendShape->_pBlendShapeDatas->SetDataBankExport(false);

	nNbrTargetShapes = 0;//pBlendShape->_pBlendShapeDatas->GetNbrTargets();
//	pBlendShape->_prWeights = new float[nNbrTargetShapes];

	for	(i = 0 ; i < nNbrTargetShapes ; ++i)
	{
		pBlendShape->_prWeights[i] = 0.0f;
	}

}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	BLEND_SHAPE::CM_WriteChunk(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	BLEND_SHAPE	*pBlendShape;
	pBlendShape = static_cast<BLEND_SHAPE *>(pCommunicator);

	Stream.DebugOutput("Blenshape :\t");
	Stream.DebugOutput(pBlendShape->GetName());
	Stream.DebugOutput("\r\n");

	Stream << pBlendShape->_nNumId;
//	Stream << *pBlendShape->_pBlendShapeDatas;
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
