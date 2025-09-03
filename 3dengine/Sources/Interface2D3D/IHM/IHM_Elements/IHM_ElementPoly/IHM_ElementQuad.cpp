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
//	CLASS:	IHM_ELEMENT_QUAD
//
//	01-09-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementQuad)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(ComplexType/TypePoint2D)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KMATH(Math/Color/Color)

#include	INCL_KRENDERER(Renderer\SVertexElement)
#include	INCL_KRENDERER(Renderer\IRenderTarget)
#include	INCL_KRENDERER(Renderer\IVertexBuffer)
#include	INCL_KRENDERER(Renderer\RenderContext)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_QUAD constructor
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_QUAD::IHM_ELEMENT_QUAD()
:
IHM_ELEMENT_POLY_ABC(GetGenericCommunicatorDescriptor())
{
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_QUAD destructor
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_QUAD::~IHM_ELEMENT_QUAD()
{
/*	if ( _pGeometry )
	{
		if (DISPLAY_MANAGER::IsInstancied())
		{
			DISPLAY_MANAGER::Instance()->PushIHMGeometry(_pGeometry);
			_pGeometry = NULL;
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		InitGeometry
//	Object:		
//	03-09-29:	VMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::InitGeometry()
{
/*	if ( _pGeometry == NULL )
	{
		RENDER_CONTEXT	*pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();

		// Allocations
		_pGeometry = pRenderContext->AllocateGeometry();
		_pGeometry->Create(GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST, 
						   GEOMETRY_ABC::VERTEX_TYPE_TRANSFORMED_LIT);
		_pGeometry->InitQuad();
		_pGeometry->SetNeedSynchronizeVertices(true);
		_pGeometry->SetNeedSynchronizeIndices(true);
	} */
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::Open(IHM_INTERFACE *		pInterface,
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
void	IHM_ELEMENT_QUAD::Close(IHM_INTERFACE *		pInterface,
							    IHM_PAGE *			pPage,
							    IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...	
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticesRelativeToScreen
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::SetVerticesRelativeToScreen(VIEWPORT * pViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
#ifdef PLATFORM_PS2
	if (_PolyFlags.IsSet(CLIPPING_IHM_QUAD))
	{
		SetVerticesRelativeToScreenClipping(pViewport);
		return;
	}
#endif

	unsigned int	i;
	VECTOR			P;

	InitGeometry();

	if	(_pGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
	{
		for	(i = 0 ; i < 4 ; ++i)
		{
			P = RelativeToScreen(pViewport, _pPoints[i], _rZDepth);
			_pGeometry->SetVertex(i, P);
		}
	}
	else
	{
		for	(i = 0 ; i < 3 ; ++i)
		{
			P = RelativeToScreen(pViewport, _pPoints[i], _rZDepth);
			_pGeometry->SetVertex(i, P);
		}

		P = RelativeToScreen(pViewport, _pPoints[1], _rZDepth);
		_pGeometry->SetVertex(3, P);

		P = RelativeToScreen(pViewport, _pPoints[3], _rZDepth);
		_pGeometry->SetVertex(4, P);

		P = RelativeToScreen(pViewport, _pPoints[2], _rZDepth);
		_pGeometry->SetVertex(5, P);
	}

	_PolyFlags.Set( TO_SYNC );*/
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticesRelativeToScreen
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::SetVerticesRelativeToScreenClipping(VIEWPORT * pViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	VECTOR			P0, P1, P2, P3;
	VECTOR			C0, C1, C2, C3;
	VECTOR			UV0,UV1,UV2;
	float			rX0, rX1;
	float			rY0, rY2;
	float			rU, rV;
	int				W, H;
	VECTOR			LT, WH;

	InitGeometry();

	INTERFACE_MANAGER::Instance()->GetWindowSize(W, H);
	if (pViewport)
	{
		LT.SetXYZ(pViewport->GetLeft() / 100.0f * W, pViewport->GetTop() / 100.0f * H, 0);
		WH.SetXYZ((pViewport->GetLeft() + pViewport->GetWidth()) / 100.0f * W, (pViewport->GetTop() + pViewport->GetHeight()) / 100.0f * H, 0);
	}
	else
	{
		LT.SetXYZ(0, 0, 0);
		WH.SetXYZ(W, H, 0);
	}
		
	C0 = P0 = RelativeToScreen(pViewport, _pPoints[0], _rZDepth);
	C1 = P1 = RelativeToScreen(pViewport, _pPoints[1], _rZDepth);
	C2 = P2 = RelativeToScreen(pViewport, _pPoints[2], _rZDepth);
	C3 = P3 = RelativeToScreen(pViewport, _pPoints[3], _rZDepth);

	rX0 = rX1 = ( P1.GetX() - P0.GetX() );
	rY0 = rY2 = ( P2.GetY() - P0.GetY() );

	if ( C0.GetX() < LT.GetX() )
	{
		 C0.SetX(LT.GetX());
		 C2.SetX(LT.GetX());
	}

	if ( C1.GetX() > WH.GetX() )
	{
		 C1.SetX(WH.GetX());
		 C3.SetX(WH.GetX());
	}

	if ( C0.GetY() < LT.GetY() )
	{
		 C0.SetY(LT.GetY());
		 C1.SetY(LT.GetY());
	}
	
	if ( C2.GetY() > WH.GetY() )
	{
		 C2.SetY(WH.GetY());
		 C3.SetY(WH.GetY());
	}

	rX0 = ( C0.GetX() - P0.GetX() ) / rX0;
	rX1 = ( C1.GetX() - P0.GetX() ) / rX1;
	rY0 = ( C0.GetY() - P0.GetY() ) / rY0;
	rY2 = ( C2.GetY() - P0.GetY() ) / rY2;

	UV0 = _pGeometry->GetNormalCopy(0);
	UV1 = _pGeometry->GetNormalCopy(1);
	UV2 = _pGeometry->GetNormalCopy(2);

	rU = UV1.GetX() - UV0.GetX();
	rV = UV2.GetY() - UV0.GetY();

	if	(_pGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
	{		
		_pGeometry->SetVertex(0, C0);
		_pGeometry->SetTextureCoordinate(0, 0, (POINT2D&)( UV0 + VECTOR(   rX0 * rU,   rY0 * rV, 0.f ) ) );
		_pGeometry->SetVertex(1, C1);
		_pGeometry->SetTextureCoordinate(0, 1, (POINT2D&)( UV0 + VECTOR(   rX1 * rU,   rY0 * rV, 0.f ) ) );
		_pGeometry->SetVertex(2, C2);
		_pGeometry->SetTextureCoordinate(0, 2, (POINT2D&)( UV0 + VECTOR(   rX0 * rU,   rY2 * rV, 0.f ) ) );
		_pGeometry->SetVertex(3, C3);
		_pGeometry->SetTextureCoordinate(0, 4, (POINT2D&)( UV0 + VECTOR(   rX1 * rU,   rY2 * rV, 0.f ) ) );

	}
	else
	{
		_pGeometry->SetVertex(0, C0);
		_pGeometry->SetTextureCoordinate(0, 0, (POINT2D&)( UV0 + VECTOR(   rX0 * rU,   rY0 * rV, 0.f ) ) );

		_pGeometry->SetVertex(1, C1);
		_pGeometry->SetTextureCoordinate(0, 1, (POINT2D&)( UV0 + VECTOR(   rX1 * rU,   rY0 * rV, 0.f ) ) );

		_pGeometry->SetVertex(2, C2);
		_pGeometry->SetTextureCoordinate(0, 2, (POINT2D&)( UV0 + VECTOR(   rX0 * rU,   rY2 * rV, 0.f ) ) );

		_pGeometry->SetVertex(3, C1);
		_pGeometry->SetTextureCoordinate(0, 3, (POINT2D&)( UV0 + VECTOR(   rX1 * rU,   rY0 * rV, 0.f ) ) );

		_pGeometry->SetVertex(4, C3);
		_pGeometry->SetTextureCoordinate(0, 4, (POINT2D&)( UV0 + VECTOR(   rX1 * rU,   rY2 * rV, 0.f ) ) );

		_pGeometry->SetVertex(5, C2);
		_pGeometry->SetTextureCoordinate(0, 5, (POINT2D&)( UV0 + VECTOR(   rX0 * rU,   rY2 * rV, 0.f ) ) );
	}

	_PolyFlags.Set( TO_SYNC );*/
}

//-----------------------------------------------------------------------------
//	Name:		SetVertices
//	Object:		
//	04-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::SetVertices(POINT2D *	pPoints)
{
	unsigned int	i;

	for	(i = 0 ; i < 4 ; ++i)
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
void	IHM_ELEMENT_QUAD::SetVerticesRelativeToWorld(POINT2D *pPoints, float rSizeX, float rSizeY)
{
	QDT_NOT_IMPLEMENTED();
	/*unsigned int	i;
	VECTOR			P;

	if	(_pGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
	{
		for	(i = 0 ; i < 4 ; ++i)
		{
			_pPoints[i] = pPoints[i];
			P = RelativeToWorld(_pPoints[i], rSizeX, rSizeY, _rZDepth);
			_pUntransformedGeometry->SetVertex(i, P);
		}
	}
	else
	{
		for	(i = 0 ; i < 3 ; ++i)
		{
			_pPoints[i] = pPoints[i];
			P = RelativeToWorld(_pPoints[i], rSizeX, rSizeY, _rZDepth);
			_pUntransformedGeometry->SetVertex(i, P);
		}

		P = RelativeToWorld(_pPoints[1], rSizeX, rSizeY, _rZDepth);
		_pUntransformedGeometry->SetVertex(3, P);

		_pPoints[3] = pPoints[3];
		P = RelativeToWorld(_pPoints[3], rSizeX, rSizeY, _rZDepth);
		_pUntransformedGeometry->SetVertex(4, P);

		P = RelativeToWorld(_pPoints[2], rSizeX, rSizeY, _rZDepth);
		_pUntransformedGeometry->SetVertex(5, P);
	}

	_bToSync = true;*/
}

//-----------------------------------------------------------------------------
//	Name:		SetColors
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::SetColors(COLOR	*pColors)
{
	_Colors[0] = pColors[0];
	_Colors[1] = pColors[1];
	_Colors[2] = pColors[2];
	_Colors[3] = pColors[3];
}

//-----------------------------------------------------------------------------
//	Name:		SetUVs
//	Object:		
//	01-10-02:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::SetUVs(POINT2D *	pUVs)
{
	_pUVs[0] = pUVs[0];
	_pUVs[1] = pUVs[1];
	_pUVs[2] = pUVs[2];
	_pUVs[3] = pUVs[3];
}

//-----------------------------------------------------------------------------
//	Name:		SetZDepth
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::SetZDepth(float	rZ)
{
	_rZDepth = rZ;
	_bVerticesToReset = true;
}

//-----------------------------------------------------------------------------
//	Name:		IsPointInElement
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_QUAD::IsPointInElement(VIEWPORT *	pViewport,
										   int			nPointX,
										   int			nPointY)
{
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		GetPointsCount
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_QUAD::GetPointsCount() const
{
	return ( 4 );
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_QUAD::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_QUAD) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_QUAD::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_QUAD"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_QUAD,
										QDT_COMMENT("26C18B27-818F-A384-DE22-3CD2251ECF26"),
										sizeof(IHM_ELEMENT_QUAD), 
										Init, 
										QDT_COMMENT("Ask 3D Engine to document this class."),
										IHM_ELEMENT_POLY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	pM = METHOD_DESCRIPTOR::Allocate("SetPoints", "Set relative coordinates of the points.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "Point1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Point2", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Point3", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Point4", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddCommand(CMD_SET_POINTS, pM, CM_SetPoints, EM_SetPoints);
	
	pM = METHOD_DESCRIPTOR::Allocate("SetColors", "Set colors of each point.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "Color1", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Color2", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Color3", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Color4", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	pCD->AddCommand(CMD_SET_COLORS, pM, CM_SetColors, EM_SetColors);

	pM = METHOD_DESCRIPTOR::Allocate("SetMappingCoordinates", "Set mapping coordinates of each point.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "UV2", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddCommand(CMD_SET_UVS, pM, CM_SetMappingCoordinates, EM_SetMappingCoordinates);

	pM = METHOD_DESCRIPTOR__Allocate("GetPoints", "Get relative coordinates of the points.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point2", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point3", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Point4", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_GET_POINTS, pM, EM_GetPoints);
	
	pM = METHOD_DESCRIPTOR__Allocate("GetColors", "Get colors of each point.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color1", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color2", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color3", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Color4", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_GET_COLORS, pM, EM_GetColors);

	pM = METHOD_DESCRIPTOR__Allocate("GetMappingCoordinates", "Get mapping coordinates of each point.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV1", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "UV2", "Ask 3D Engine to document this parameter.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_GET_UVS, pM, EM_GetMappingCoordinates);

	pM = METHOD_DESCRIPTOR__Allocate("EnableClipping", "Enable Clipping (use for quads greater than screen size).");
	METHOD_DESCRIPTOR__AddParameter(pM, "bEnable", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_CLIPPING, pM, EM_EnableClipping);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_Construct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	IHM_ELEMENT_QUAD *	pObject;
	pObject = static_cast<IHM_ELEMENT_QUAD *>(pO);
	QDT_NEW_PLACED(pObject, IHM_ELEMENT_QUAD)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-12-02:	VMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_Construct_In_Memory(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_Destruct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	IHM_ELEMENT_QUAD *	pObject;
	pObject = static_cast<IHM_ELEMENT_QUAD*> (pO);
	pObject->IHM_ELEMENT_QUAD::~IHM_ELEMENT_QUAD();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetPoints
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_SetPoints(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	POINT2D Points[4];
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);

	struct	PARAMETERS
	{
		POINT2D 	_Point1;
		POINT2D 	_Point2;
		POINT2D 	_Point3;
		POINT2D 	_Point4;
	};

	PARAMETERS *	pParameters;

	pParameters = (PARAMETERS *) pP;
	
	Points[0] = pParameters->_Point1;
	Points[1] = pParameters->_Point2;
	Points[2] = pParameters->_Point3;
	Points[3] = pParameters->_Point4;
	
	pIhmElementQuad->SetVertices(Points);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetPoints
//	Object:		
//	06-03-20:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_QUAD::CM_SetPoints(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_POINTS);
	COM_INSTANCE pObjet;

	Stack.GetThis(pObjet);
	
	IHM_ELEMENT_QUAD	*pQuad = static_cast<IHM_ELEMENT_QUAD *>(pObjet.GetCommunicator());

	POINT2D pointTable[4];

	Stack.GetParameter(0, pointTable[0]);
	Stack.GetParameter(1, pointTable[1]);
	Stack.GetParameter(2, pointTable[2]);
	Stack.GetParameter(3, pointTable[3]);

//	pointTable[0] = *(POINT2D*) Stack.GetUserDataPtr(-4);
//	pointTable[1] = *(POINT2D*) Stack.GetUserDataPtr(-3);
//	pointTable[2] = *(POINT2D*) Stack.GetUserDataPtr(-2);
//	pointTable[3] = *(POINT2D*) Stack.GetUserDataPtr(-1);

	pQuad->SetVertices(pointTable);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetColors
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_SetColors(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COLOR Colors[4];
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);

	struct	PARAMETERS
	{
		COLOR 	_Color1;
		COLOR 	_Color2;
		COLOR 	_Color3;
		COLOR 	_Color4;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	Colors[0] = pParameters->_Color1;
	Colors[1] = pParameters->_Color2;
	Colors[2] = pParameters->_Color3;
	Colors[3] = pParameters->_Color4;
	
	pIhmElementQuad->SetColors(Colors);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetColors
//	Object:		
//	06-03-21:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_QUAD::CM_SetColors(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_COLORS);
	COM_INSTANCE pObjet;

	Stack.GetThis(pObjet);
	
	IHM_ELEMENT_QUAD	*pQuad = static_cast<IHM_ELEMENT_QUAD *>(pObjet.GetCommunicator());

	COLOR colorTable[4];

	Stack.GetParameter(0, colorTable[0]);
	Stack.GetParameter(1, colorTable[1]);
	Stack.GetParameter(2, colorTable[2]);
	Stack.GetParameter(3, colorTable[3]);

//	colorTable[0] = *(COLOR*) Stack.GetUserDataPtr(-4);
//	colorTable[1] = *(COLOR*) Stack.GetUserDataPtr(-3);
//	colorTable[2] = *(COLOR*) Stack.GetUserDataPtr(-2);
//	colorTable[3] = *(COLOR*) Stack.GetUserDataPtr(-1);

	pQuad->SetColors(colorTable);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetMappingCoordinates
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_SetMappingCoordinates(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	POINT2D Points[4];
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);

	struct	PARAMETERS
	{
		POINT2D 	_Point1;
		POINT2D 	_Point2;
		POINT2D 	_Point3;
		POINT2D 	_Point4;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	Points[0] = pParameters->_Point1;
	Points[1] = pParameters->_Point2;
	Points[2] = pParameters->_Point3;
	Points[3] = pParameters->_Point4;
	
	pIhmElementQuad->SetUVs(Points);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetMappingCoordinates
//	Object:		
//	06-03-21:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_QUAD::CM_SetMappingCoordinates(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_UVS);
	COM_INSTANCE pObjet;

	Stack.GetThis(pObjet);
	
	IHM_ELEMENT_QUAD	*pQuad = static_cast<IHM_ELEMENT_QUAD *>(pObjet.GetCommunicator());

	POINT2D pointTable[4];

	Stack.GetParameter(0, pointTable[0]);
	Stack.GetParameter(1, pointTable[1]);
	Stack.GetParameter(2, pointTable[2]);
	Stack.GetParameter(3, pointTable[3]);

//	pointTable[0] = *(POINT2D*) Stack.GetUserDataPtr(-4);
//	pointTable[1] = *(POINT2D*) Stack.GetUserDataPtr(-3);
//	pointTable[2] = *(POINT2D*) Stack.GetUserDataPtr(-2);
//	pointTable[3] = *(POINT2D*) Stack.GetUserDataPtr(-1);

	pQuad->SetUVs(pointTable);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);}


//-----------------------------------------------------------------------------
//	Name:		EM_GetPoints
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_GetPoints(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);

	struct	PARAMETERS
	{
		POINT2D * 	_pPoint1;
		POINT2D *	_pPoint2;
		POINT2D *	_pPoint3;
		POINT2D *	_pPoint4;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	*pParameters->_pPoint1 = pIhmElementQuad->GetPoint(0);
	*pParameters->_pPoint2 = pIhmElementQuad->GetPoint(1);
	*pParameters->_pPoint3 = pIhmElementQuad->GetPoint(2);
	*pParameters->_pPoint4 = pIhmElementQuad->GetPoint(3);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetColors
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_GetColors(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);
	
	struct	PARAMETERS
	{
		COLOR * _pColor1;
		COLOR *	_pColor2;
		COLOR *	_pColor3;
		COLOR *	_pColor4;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	*pParameters->_pColor1 = pIhmElementQuad->GetColor(0);
	*pParameters->_pColor2 = pIhmElementQuad->GetColor(1);
	*pParameters->_pColor3 = pIhmElementQuad->GetColor(2);
	*pParameters->_pColor4 = pIhmElementQuad->GetColor(3);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMappingCoordinates
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_GetMappingCoordinates(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);

	struct	PARAMETERS
	{
		POINT2D * 	_pPoint1;
		POINT2D *	_pPoint2;
		POINT2D *	_pPoint3;
		POINT2D *	_pPoint4;
	};

	PARAMETERS *	pParameters;
	
	pParameters = (PARAMETERS *) pP;

	*pParameters->_pPoint1 = pIhmElementQuad->GetUV(0);
	*pParameters->_pPoint2 = pIhmElementQuad->GetUV(1);
	*pParameters->_pPoint3 = pIhmElementQuad->GetUV(2);
	*pParameters->_pPoint4 = pIhmElementQuad->GetUV(3);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableClipping
//	Object:		
//	04-08-31:	VMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_QUAD::EM_EnableClipping(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_QUAD *		pIhmElementQuad;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementQuad	= (IHM_ELEMENT_QUAD *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementQuad);

	pIhmElementQuad->_PolyFlags.Set(CLIPPING_IHM_QUAD, *(bool*)pP);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
