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
//	CLASS:	IHM_ELEMENT_BITMAP
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementBitmap)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementQuad)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*						IHM_ELEMENT_BITMAP::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_BITMAP constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_BITMAP::IHM_ELEMENT_BITMAP()
:
IHM_ELEMENT_RECT(GetGenericCommunicatorDescriptor()),
_IhmElementQuad( IHM_ELEMENT_QUAD::GetGenericCommunicatorDescriptor()->Create())
{
	_pCroppingUVs[ 0 ].SetXY( 0.0f, 0.0f );
	_pCroppingUVs[ 1 ].SetXY( 0.0f, 1.0f );
	_pCroppingUVs[ 2 ].SetXY( 1.0f, 0.0f );
	_pCroppingUVs[ 3 ].SetXY( 1.0f, 1.0f );
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_BITMAP destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_BITMAP::~IHM_ELEMENT_BITMAP()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::Open(IHM_INTERFACE *		pInterface,
								 IHM_PAGE *				pPage,
								 IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...	
}
	
//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::Close(IHM_INTERFACE *		pInterface,
								  IHM_PAGE *			pPage,
								  IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...	
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-10-01:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::DisplayElement(VIEWPORT * pViewport)
{
	POINT2D	pPos[ 4 ];
	IHM_ELEMENT_QUAD * pIhmElementQuad;

	pPos[ 0 ].SetXY( GetRelativePosLeft(),  GetRelativePosTop() );
	pPos[ 1 ].SetXY( GetRelativePosLeft(),  GetRelativePosBottom() );
	pPos[ 2 ].SetXY( GetRelativePosRight(), GetRelativePosTop() );
	pPos[ 3 ].SetXY( GetRelativePosRight(), GetRelativePosBottom() );

	pIhmElementQuad = static_cast<IHM_ELEMENT_QUAD*>(_IhmElementQuad.GetCommunicator());
	pIhmElementQuad->SetUVs( _pCroppingUVs );
	pIhmElementQuad->SetVertices( pPos );
	if	(_Flags.IsSet(RESIZE_ON_169))
	{
		pIhmElementQuad->Set169Resizing(true);
	}
	pIhmElementQuad->DisplayElement( pViewport );
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::Display(DISPLAY_PARAMETERS &	DisplayParameters,
									float					rSizeX,
									float					rSizeY)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetDatabaseType
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_BITMAP::GetDatabaseType()
{
	return ( MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP );
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterialInst
//	Object:		
//	02-10-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetMaterialInst(const COM_INSTANCE &	MaterialInst)
{
	IHM_ELEMENT_QUAD * pIhmElementQuad;
	pIhmElementQuad = static_cast<IHM_ELEMENT_QUAD*>(_IhmElementQuad.GetCommunicator());

	pIhmElementQuad->SetMaterialInst( MaterialInst );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterialInst
//	Object:		
//	03-04-25:	DAC - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	IHM_ELEMENT_BITMAP::GetMaterialInst() const
{
	IHM_ELEMENT_QUAD * pIhmElementQuad;
	pIhmElementQuad = static_cast<IHM_ELEMENT_QUAD*>(_IhmElementQuad.GetCommunicator());

	return	(pIhmElementQuad->GetMaterialInst());
}


//-----------------------------------------------------------------------------
//	Name:		SetColor
//	Object:		
//	03-03-11:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetColor(const COLOR &	Color)
{
	int		nColorIndex;
	COLOR	Colors[ 4 ];

	for ( nColorIndex = 0; nColorIndex < 4; ++nColorIndex )
	{
		Colors[ nColorIndex ] = Color;
	}

	GetIhmQuad()->SetColors( Colors );
}

//-----------------------------------------------------------------------------
//	Name:		GetColor
//	Object:		
//	04-02-24:	GGO - Created
//-----------------------------------------------------------------------------
COLOR	IHM_ELEMENT_BITMAP::GetColor() const
{
	// Since all colors are supposed to be the same on each vertex, 
	// we return the color of the first vertex.
	return ( GetIhmQuad()->GetColor( 0 ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetCroppingCoordinates
//	Object:		
//	01-11-15:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetCroppingCoordinates(int	nLeft,
												   int	nRight,
												   int	nTop,
												   int	nBottom)
{
	QDT_Warning(QDT_COMMENT("The method 'IHM_ELEMENT_BITMAP::SetCroppingCoordinates(int, int, int, int)' is obsolete. Use the method wuth float instead."));
}

//-----------------------------------------------------------------------------
//	Name:		SetCroppingCoordinates
//	Object:		
//	02-10-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetCroppingCoordinates(float	rLeft,
												   float	rRight,
												   float	rTop,
												   float	rBottom)
{
	_pCroppingUVs[ 0 ].SetXY( rLeft,  rTop );
	_pCroppingUVs[ 1 ].SetXY( rLeft,  rBottom );
	_pCroppingUVs[ 2 ].SetXY( rRight, rTop );
	_pCroppingUVs[ 3 ].SetXY( rRight, rBottom );
}

//-----------------------------------------------------------------------------
//	Name:		SetZDepth
//	Object:		
//	03-09-02:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetZDepth(const float	rZ)
{
	IHM_ELEMENT_ABC::SetZDepth( rZ );

	GetIhmQuad()->SetZDepth( rZ );
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	pM = METHOD_DESCRIPTOR__Allocate("SetCroppingCoordinates", "Set the cropping coordinates of the bitmap in case you want to display only a part of the bitmap. If the function is not called, all the bitmap is displayed. The coordinates are pixels numbers.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rLeft",   "Left cropping coordinate. Must be between 0 and 100.",   TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "rRight",  "Right cropping coordinate. Must be between 0 and 100.",  TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "rTop",    "Top cropping coordinate. Must be between 0 and 100.",    TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "rBottom", "Bottom cropping coordinate. Must be between 0 and 100.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_CROPPING_COORDINATES, pM, EM_SetCroppingCoordinates);

	pM = METHOD_DESCRIPTOR__Allocate("Copy", "Copy the IHM_ELEMENT.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "IhmElement", "The IhmElement to copy.", IHM_ELEMENT_BITMAP::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_COPY, pM, EM_Copy);

	pM = METHOD_DESCRIPTOR__Allocate("SetMaterial", "Set the material to be displayed when the element is visible.");
	//METHOD_DESCRIPTOR__AddParameter(pM, "Material", "The material to set.", MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_MATERIAL, pM, EM_SetMaterial);
}
	
//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::EM_Construct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	IHM_ELEMENT_BITMAP *	pObject;
	pObject = static_cast<IHM_ELEMENT_BITMAP *>(pO);
	QDT_NEW_PLACED(pObject, IHM_ELEMENT_BITMAP)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-12-02:	VMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::EM_Construct_In_Memory(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::EM_Destruct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	IHM_ELEMENT_BITMAP *	pObject;
	pObject = static_cast<IHM_ELEMENT_BITMAP*> (pO);
	pObject->IHM_ELEMENT_BITMAP::~IHM_ELEMENT_BITMAP();
}
	
//-----------------------------------------------------------------------------
//	Name:		EM_SetMaterial
//	Object:		
//	02-10-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::EM_SetMaterial(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	/*COM_INSTANCE *			pIhmBitmapInst;
	IHM_ELEMENT_BITMAP *	pIhmBitmap;
	COM_INSTANCE *			pParam;

	pIhmBitmapInst	= static_cast<COM_INSTANCE*>(pO);
	pIhmBitmap		= static_cast<IHM_ELEMENT_BITMAP*>(pIhmBitmapInst->GetCommunicator());
	pParam			= static_cast<COM_INSTANCE*>(pP);

	pIhmBitmap->SetMaterialInst( *pParam );	*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetCroppingCoordinates
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::EM_SetCroppingCoordinates(void * const	pO,
													  void * const	pR,
													  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_BITMAP *		pIhmElementBitmap;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementBitmap	= (IHM_ELEMENT_BITMAP *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementBitmap);

	struct	PARAMETERS
	{
		float 	_rLeft;
		float 	_rRight;
		float 	_rTop;
		float 	_rBottom;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	pIhmElementBitmap->SetCroppingCoordinates( 0.01f * pParameters->_rLeft, 0.01f * pParameters->_rRight, 0.01f * pParameters->_rTop, 0.01f * pParameters->_rBottom );
}

//-----------------------------------------------------------------------------
//	Name:		EM_Copy
//	Object:		
//	02-08-22:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::EM_Copy(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	COM_INSTANCE *	pObject;
	COM_INSTANCE **	pParameter;

	pObject = (COM_INSTANCE*)pO;
	pParameter = (COM_INSTANCE**)pP;

	*pObject = **pParameter;
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_BITMAP::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_BITMAP::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_BITMAP"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP, 
										QDT_COMMENT("E1F0BFBA-3176-2AEE-0284-1FCE0BCCEC3C"),
										sizeof(IHM_ELEMENT_BITMAP), 
										Init, 
										QDT_COMMENT("This IHM_ELEMENT displays a bitmap into the display window."),
										IHM_ELEMENT_RECT::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		GetIhmQuad
//	Object:		
//	03-03-11:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_QUAD *	IHM_ELEMENT_BITMAP::GetIhmQuad() const
{
	IHM_ELEMENT_QUAD *	pIhmQuad;

	pIhmQuad = static_cast<IHM_ELEMENT_QUAD*>( _IhmElementQuad.GetCommunicator() );

	return ( pIhmQuad );
}

//-----------------------------------------------------------------------------
//	Name:		COM_PostLoad
//	Object:		
//	03-09-22:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::COM_PostLoad()
{
	// TEMPORAIRE : Automatisation du Preload après un Load de l'objet....
	// A virer quand cet objet sera un COMMUNICATOR_CONTAINER

	GetMaterialInst().GetCommunicator( (LOADING_STACK::FM_PRIORITY_LEVEL) LOADING_STACK::Instance()->GetCurrentPriority() /* heu...*/ );
}

//-----------------------------------------------------------------------------
//	Name:		SetAddGeometryInDatabank
//	Object:		
//	05-01-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetGeometryInDatabank(const bool	bAdd)
{
	GetIhmQuad()->SetGeometryInDatabank( bAdd );
}

//-----------------------------------------------------------------------------
//	Name:		SetAddMaterialInDatabank
//	Object:		
//	05-01-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP::SetMaterialInDatabank(const bool	bAdd)
{
	GetIhmQuad()->SetMaterialInDatabank( bAdd );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

