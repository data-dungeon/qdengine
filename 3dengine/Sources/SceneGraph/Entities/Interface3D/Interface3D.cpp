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
//	CLASS:	INTERFACE_3D
//
//	02-12-04:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Interface3D/Interface3D)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Display/DisplayParameters/DisplayParameters)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INTERFACE_3D constructor
//	Object:		
//	02-12-04:	RMA - Created
//-----------------------------------------------------------------------------
INTERFACE_3D::INTERFACE_3D()
:
ENTITY_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_INTERFACE_3D),
_bHide		(false),
_rSizeX		(100.0f),
_rSizeY		(100.0f)
{
	_BoundingBox.SetCenter(VECTOR::ZERO);
	_BoundingBox.SetDirection1(VECTOR::UNIT_X);
	_BoundingBox.SetDirection2(VECTOR::UNIT_Y);
	_BoundingBox.SetDirection3(VECTOR::UNIT_Z);
	_BoundingBox.SetDistance1(GetSizeX() * 0.5);
	_BoundingBox.SetDistance2(GetSizeY() * 0.5);
	_BoundingBox.SetDistance3(0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		INTERFACE_3D destructor
//	Object:		
//	02-12-04:	RMA - Created
//-----------------------------------------------------------------------------
INTERFACE_3D::~INTERFACE_3D()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PushIhmElement
//	Object:		
//	02-12-04:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::AddIhmElement(const COM_INSTANCE& Element)
{
	float	ZDepth;
	IHM_ELEMENT_ABC	*pElement;

	pElement = static_cast<IHM_ELEMENT_ABC *>(Element.GetCommunicator());
	ZDepth = pElement->GetZDepth();
	
	INTERFACE_3D	*pInterface = pElement->GetInterface3D();

	if (pInterface)
	{
		pInterface->RemoveIhmElement(Element);
	}
	
	pElement->SetInterface3D(this);
	
	QDT_DLIST<COM_INSTANCE>::ITERATOR It	= _IhmElements.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd	= _IhmElements.GetTail();
	
	while (It != ItEnd)
	{
		pElement = static_cast<IHM_ELEMENT_ABC *>((*It).GetCommunicator());
		
		if (pElement->GetZDepth() < ZDepth)
		{
			_IhmElements.Insert(It, Element);
			return;
		}
		++It ;
	}
	
	_IhmElements.PushTail(Element);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIhmElement
//	Object:		
//	02-12-04:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::RemoveIhmElement(const COM_INSTANCE&	Elt)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR	It		= _IhmElements.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR	ItTail	= _IhmElements.GetTail();

	if (_IhmElements.GetSize() > 0)
	{
		while	(It != ItTail)
		{	
			if (*It == Elt)
			{
				_IhmElements.Remove(It);
				return;
			}
			
			++It;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
// obsolete

/*	if	(TestVisibility(DisplayParameters))
	{
		QDT_DLIST<COM_INSTANCE>::ITERATOR it	 = _IhmElements.GetHead();
		QDT_DLIST<COM_INSTANCE>::ITERATOR itTail = _IhmElements.GetTail();

		IHM_ELEMENT_ABC		*pElt;

		while (it != itTail)
		{
			pElt = static_cast<IHM_ELEMENT_ABC*>((*it).GetCommunicator());
			pElt->Display(DisplayParameters, GetSizeX(), GetSizeY());
			++it;
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	INTERFACE_3D::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	if (_bHide || _IhmElements.IsEmpty()) return (false);

	OBB	GlobalBox = _BoundingBox;
	GlobalBox.Transform(DisplayParameters.GetNode()->GetGlobalMatrix());

	if (!GlobalBox.Intersect(DisplayParameters.GetFrustrum())) return (false);

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateBoundingBox
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::UpdateBoundingBox()
{
	_BoundingBox.SetDistance1(GetSizeX() * 0.5f);
	_BoundingBox.SetDistance2(GetSizeY() * 0.5f);
}

//-----------------------------------------------------------------------------
//	Name:		Show
//	Object:		
//	02-12-10:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::Show(bool	b)
{
	_bHide = !b;
}

//-----------------------------------------------------------------------------
//	Name:		SetSizeX
//	Object:		
//	02-12-10:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::SetSizeX(float	r)
{
	_rSizeX = r;
	UpdateBoundingBox();
}

//-----------------------------------------------------------------------------
//	Name:		SetSizeY
//	Object:		
//	02-12-10:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::SetSizeY(float	r)
{
	_rSizeY = r;
	UpdateBoundingBox();
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-12-20:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::Copy(const INTERFACE_3D &	Interface)
{
	SetSizeX(Interface.GetSizeX());
	SetSizeY(Interface.GetSizeY());

	_bHide		 = Interface._bHide;
	_IhmElements = Interface._IhmElements;
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *		pMD;
	ATTRIBUTE_DESCRIPTOR *	pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pAD = new	ATTRIBUTE_DESCRIPTOR("_nSizeX", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetSizeX, EM_SetSizeX);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("_nSizeY", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetSizeY, EM_SetSizeY);
	pCD->AddAttribute(pAD);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSizeX"), QDT_COMMENT("Get size X."));
	pMD->SetReturn(QDT_COMMENT("rSizeX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_INT3D_GET_SIZE_X, pMD, EM_GetSizeX);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSizeY"), QDT_COMMENT("Get size Y."));
	pMD->SetReturn(QDT_COMMENT("rSizeY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_INT3D_GET_SIZE_Y, pMD, EM_GetSizeY);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSizeX"), QDT_COMMENT("Set size X."));
	pMD->AddParameter(QDT_COMMENT("rSizeX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_INT3D_SET_SIZE_X, pMD, EM_SetSizeX);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSizeY"), QDT_COMMENT("Set size Y."));
	pMD->AddParameter(QDT_COMMENT("rSizeY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_INT3D_SET_SIZE_Y, pMD, EM_SetSizeX);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("AddIhmElement"), QDT_COMMENT("Add an ihm element."));
	pMD->AddParameter(QDT_COMMENT("Ihm element to add") ,QDT_COMMENT("") ,IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_INT3D_ADD_IHM_ELEMENT, pMD, EM_AddIhmElement);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("RemoveIhmElement"), QDT_COMMENT("Remove an ihm element."));
	pMD->AddParameter(QDT_COMMENT("Ihm element to remove") ,QDT_COMMENT("") ,IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_INT3D_REMOVE_IHM_ELEMENT, pMD, EM_RemoveIhmElement);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("Show"), QDT_COMMENT("Show."));
	pMD->AddParameter(QDT_COMMENT("bShow") ,QDT_COMMENT("") ,TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_INT3D_SHOW, pMD, EM_Show);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	INTERFACE_3D::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_INTERFACE_3D) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	INTERFACE_3D::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("INTERFACE_3D"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_INTERFACE_3D,
										QDT_COMMENT("38CD5943-0A7C-2EFE-C8F3-D49F54F859D9"), 
										sizeof(INTERFACE_3D), 
										ComInit, 
										QDT_COMMENT("Interface 3D allow to display ihm element in the 3D world."),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_Construct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	INTERFACE_3D *pI3D;
	pI3D = static_cast<INTERFACE_3D *>(pO);

	QDT_NEW_PLACED(pI3D, INTERFACE_3D)();
	QDT_ASSERT(pI3D);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_Destruct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	INTERFACE_3D *pI3D;
	pI3D = static_cast<INTERFACE_3D *>(pO);

	QDT_DELETE_PLACED(pI3D, INTERFACE_3D);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSizeX
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_GetSizeX(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE*>(pO);;
	INTERFACE_3D *	pI3D		 = static_cast<INTERFACE_3D*>(pComInstance->GetCommunicator());
	float *			prSizeX		 = static_cast<float*>(pR);

	*prSizeX = pI3D->GetSizeX();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSizeY
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_GetSizeY(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE*>(pO);;
	INTERFACE_3D *	pI3D		 = static_cast<INTERFACE_3D*>(pComInstance->GetCommunicator());
	float *			prSizeY		 = static_cast<float*>(pR);

	*prSizeY = pI3D->GetSizeY();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSizeX
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_SetSizeX(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE*>(pO);;
	INTERFACE_3D *	pI3D		 = static_cast<INTERFACE_3D*>(pComInstance->GetCommunicator());
	float *			prSizeX		 = static_cast<float*>(pP);

	pI3D->SetSizeX(*prSizeX);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSizeY
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_SetSizeY(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE*>(pO);;
	INTERFACE_3D *	pI3D		 = static_cast<INTERFACE_3D*>(pComInstance->GetCommunicator());
	float *			prSizeY		 = static_cast<float*>(pP);

	pI3D->SetSizeY(*prSizeY);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddIhmElement
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_AddIhmElement(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *	pCI		= static_cast<COM_INSTANCE*>(pO);
	INTERFACE_3D *	pI3D	= static_cast<INTERFACE_3D*>(pCI->GetCommunicator() );
	pCI						= static_cast<COM_INSTANCE*>(pP);

	pI3D->AddIhmElement(*pCI);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveIhmElement
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_RemoveIhmElement(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *	pCI		= static_cast<COM_INSTANCE*>(pO);
	INTERFACE_3D *	pI3D	= static_cast<INTERFACE_3D*>(pCI->GetCommunicator() );
	pCI						= static_cast<COM_INSTANCE*>(pP);

	pI3D->RemoveIhmElement(*pCI);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Show
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	INTERFACE_3D::EM_Show(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	COM_INSTANCE *	pCI		= static_cast<COM_INSTANCE*>(pO);
	INTERFACE_3D *	pI3D	= static_cast<INTERFACE_3D*>(pCI->GetCommunicator() );
	bool *			pbShow	= static_cast<bool*>(pP);

	pI3D->Show(*pbShow);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
