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
//	CLASS:	IHM_ELEMENT_TRIANGLE
//
//	01-09-28:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementTriangle)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComplexType/TypePoint2D)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KMATH(Math/Color/Color)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_TRIANGLE constructor
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TRIANGLE::IHM_ELEMENT_TRIANGLE()
:
IHM_ELEMENT_POLY_ABC(GetGenericCommunicatorDescriptor())
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT	*pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();

	// Allocations
	_pGeometry = pRenderContext->AllocateGeometry();
	_pGeometry->Create(GEOMETRY_ABC::GEOMETRY_TYPE_TRIANGLE_LIST, GEOMETRY_ABC::VERTEX_TYPE_TRANSFORMED_LIT);
	_pGeometry->InitTriangle();

	_pUntransformedGeometry = pRenderContext->AllocateGeometry();
	_pUntransformedGeometry->Create(GEOMETRY_ABC::GEOMETRY_TYPE_TRIANGLE_LIST, GEOMETRY_ABC::VERTEX_TYPE_UNTRANSFORMED_LIT);
	_pUntransformedGeometry->InitTriangle();

	_pGeometry->SetNeedSynchronizeVertices(true);
	_pUntransformedGeometry->SetNeedSynchronizeVertices(true);*/
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_TRIANGLE destructor
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TRIANGLE::~IHM_ELEMENT_TRIANGLE()
{
	/*if ( _pGeometry )
	{
		if (DISPLAY_MANAGER::IsInstancied())
		{
			DISPLAY_MANAGER::Instance()->PushIHMGeometry(_pGeometry);
			_pGeometry = NULL;
		}
	}

	if	(_pUntransformedGeometry)
	{
		delete	(_pUntransformedGeometry);
		_pUntransformedGeometry = NULL;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::Open(IHM_INTERFACE *		pInterface,
								   IHM_PAGE *				pPage,
							       IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...	
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::Close(IHM_INTERFACE *		pInterface,
									IHM_PAGE *			pPage,
									IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		SetVertices
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::SetVerticesRelativeToScreen(VIEWPORT * pViewport)
{
	unsigned int	i;
	VECTOR			P;

	for	(i = 0 ; i < 3 ; ++i)
	{
		P = RelativeToScreen(pViewport, _pPoints[i], _rZDepth);
//		_pGeometry->SetVertex(i, P);
	}

	_PolyFlags.Set( TO_SYNC );
}

//-----------------------------------------------------------------------------
//	Name:		SetVertices
//	Object:		
//	04-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::SetVertices(POINT2D *	pPoints)
{
	unsigned int	i;

	for	(i = 0 ; i < 3 ; ++i)
	{
		_pPoints[i] = pPoints[i];
	}

	_bVerticesToReset = true;
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticesRelativeToWorld
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::SetVerticesRelativeToWorld(POINT2D *	pPoints,
														 float		rSizeX,
														 float		rSizeY)
{
	unsigned int	i;
	VECTOR			P;

	for	(i = 0 ; i < 3 ; ++i)
	{
		_pPoints[i] = pPoints[i];
		P = RelativeToWorld(_pPoints[i], rSizeX, rSizeY, _rZDepth);
//		_pUntransformedGeometry->SetVertex(i, P);
	}

	_PolyFlags.Set( TO_SYNC );
}

//-----------------------------------------------------------------------------
//	Name:		SetColors
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::SetColors(COLOR *	pColors)
{
	unsigned int	i;

	for	(i = 0 ; i < 3 ; ++i)
	{
//		_pGeometry->SetDiffuseColor(i, pColors[i]);
//		_pUntransformedGeometry->SetDiffuseColor(i, pColors[i]);
	}

	_PolyFlags.Set( TO_SYNC );
}

//-----------------------------------------------------------------------------
//	Name:		SetUVs
//	Object:		
//	01-10-02:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::SetUVs(POINT2D *	pUVs)
{
	unsigned int	i;

	for	(i = 0 ; i < 3 ; ++i)
	{
//		_pGeometry->SetTextureCoordinate(0, i, pUVs[i]);
//		_pUntransformedGeometry->SetTextureCoordinate(0, i, pUVs[i]);
	}

	_PolyFlags.Set( TO_SYNC );
}

//-----------------------------------------------------------------------------
//	Name:		SetZDepth
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::SetZDepth(float	rZ)
{
	_rZDepth = rZ;
	_bVerticesToReset = true;
}

//-----------------------------------------------------------------------------
//	Name:		IsPointInElement
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TRIANGLE::IsPointInElement(VIEWPORT *	pViewport,
											   int			nPointX,
											   int			nPointY)
{
	QDT_NOT_IMPLEMENTED();
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		GetPointsCount
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_TRIANGLE::GetPointsCount() const
{
	return ( 3 );
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_TRIANGLE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TRIANGLE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_TRIANGLE::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_TRIANGLE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TRIANGLE,
										QDT_COMMENT("50D2C592-286E-2C87-4A38-6330F22627AF"),
										sizeof(IHM_ELEMENT_TRIANGLE), 
										Init, 
										QDT_COMMENT("Ask 3D Engine to document this class."),
										IHM_ELEMENT_POLY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pM = METHOD_DESCRIPTOR__Allocate("SetPoints", "Set relative coordinates of the points.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Point1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Point2", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Point3", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_SET_POINTS, pM, EM_SetPoints);
	
	pM = METHOD_DESCRIPTOR__Allocate("SetColors", "Set colors of each point.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Color1", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Color2", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Color3", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_SET_COLORS, pM, EM_SetColors);

	pM = METHOD_DESCRIPTOR__Allocate("SetMappingCoordinates", "Set mapping coordinates of each point.");
	METHOD_DESCRIPTOR__AddParameter(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_SET_UVS, pM, EM_SetMappingCoordinates);

	pM = METHOD_DESCRIPTOR__Allocate("GetPoints", "Get relative coordinates of the points.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point2", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point3", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_GET_POINTS, pM, EM_GetPoints);
	
	pM = METHOD_DESCRIPTOR__Allocate("GetColors", "Get colors of each point.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color1", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color2", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color3", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_GET_COLORS, pM, EM_GetColors);

	pM = METHOD_DESCRIPTOR__Allocate("GetMappingCoordinates", "Get mapping coordinates of each point.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_GET_UVS, pM, EM_GetMappingCoordinates);}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_Construct(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	IHM_ELEMENT_TRIANGLE *	pObject;
	pObject = static_cast<IHM_ELEMENT_TRIANGLE *>(pO);
	QDT_NEW_PLACED(pObject, IHM_ELEMENT_TRIANGLE)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_Destruct(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	IHM_ELEMENT_TRIANGLE *	pObject;
	pObject = static_cast<IHM_ELEMENT_TRIANGLE*> (pO);
	pObject->IHM_ELEMENT_TRIANGLE::~IHM_ELEMENT_TRIANGLE();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetPoints
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_SetPoints(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	POINT2D Points[3];
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TRIANGLE *		pIhmElementTriangle;

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementTriangle	= (IHM_ELEMENT_TRIANGLE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementTriangle);

	struct	PARAMETERS
	{
		POINT2D 	_Point1;
		POINT2D 	_Point2;
		POINT2D 	_Point3;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	Points[0] = pParameters->_Point1;
	Points[1] = pParameters->_Point2;
	Points[2] = pParameters->_Point3;
	
	pIhmElementTriangle->SetVertices(Points);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetColors
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_SetColors(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COLOR Colors[3];
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TRIANGLE *		pIhmElementTriangle;

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementTriangle	= (IHM_ELEMENT_TRIANGLE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementTriangle);

	struct	PARAMETERS
	{
		COLOR 	_Color1;
		COLOR 	_Color2;
		COLOR 	_Color3;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	Colors[0] = pParameters->_Color1;
	Colors[1] = pParameters->_Color2;
	Colors[2] = pParameters->_Color3;
	
	pIhmElementTriangle->SetColors(Colors);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMappingCoordinates
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_SetMappingCoordinates(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	POINT2D Points[3];
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TRIANGLE *		pIhmElementTriangle;

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementTriangle	= (IHM_ELEMENT_TRIANGLE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementTriangle);

	struct	PARAMETERS
	{
		POINT2D 	_Point1;
		POINT2D 	_Point2;
		POINT2D 	_Point3;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	Points[0] = pParameters->_Point1;
	Points[1] = pParameters->_Point2;
	Points[2] = pParameters->_Point3;
	
	pIhmElementTriangle->SetUVs(Points);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetPoints
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_GetPoints(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TRIANGLE *		pIhmElementTriangle;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementTriangle	= (IHM_ELEMENT_TRIANGLE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementTriangle);

	struct	PARAMETERS
	{
		POINT2D * 	_pPoint1;
		POINT2D *	_pPoint2;
		POINT2D *	_pPoint3;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	*pParameters->_pPoint1 = pIhmElementTriangle->GetPoint(0);
	*pParameters->_pPoint1 = pIhmElementTriangle->GetPoint(1);
	*pParameters->_pPoint1 = pIhmElementTriangle->GetPoint(2);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetColors
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_GetColors(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TRIANGLE *		pIhmElementTriangle;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementTriangle	= (IHM_ELEMENT_TRIANGLE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementTriangle);
	
	struct	PARAMETERS
	{
		COLOR * _pColor1;
		COLOR *	_pColor2;
		COLOR *	_pColor3;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	*pParameters->_pColor1 = pIhmElementTriangle->GetColor(0);
	*pParameters->_pColor1 = pIhmElementTriangle->GetColor(1);
	*pParameters->_pColor1 = pIhmElementTriangle->GetColor(2);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMappingCoordinates
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE::EM_GetMappingCoordinates(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TRIANGLE *		pIhmElementTriangle;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementTriangle	= (IHM_ELEMENT_TRIANGLE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementTriangle);

	struct	PARAMETERS
	{
		POINT2D * 	_pPoint1;
		POINT2D *	_pPoint2;
		POINT2D *	_pPoint3;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	*pParameters->_pPoint1 = pIhmElementTriangle->GetUV(0);
	*pParameters->_pPoint1 = pIhmElementTriangle->GetUV(1);
	*pParameters->_pPoint1 = pIhmElementTriangle->GetUV(2);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
