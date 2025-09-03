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
//	CLASS:	VIEW_PORT_LIST
//
//	02-06-06:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/Viewport/ViewportList)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/RayCaster/RayCastParameters)
#include	INCL_KRENDERER(Renderer/RenderContext)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VIEW_PORT_LIST constructor
//	Object:		
//	02-06-06:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT_LIST::VIEWPORT_LIST()
:
COMMUNICATOR(GetGenericCommunicatorDescriptor())
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VIEWPORT_LIST constructor
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT_LIST::VIEWPORT_LIST(const VIEWPORT_LIST &	C)
:
COMMUNICATOR(C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		~VIEW_PORT_LIST destructor
//	Object:		
//	02-06-06:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT_LIST::~VIEWPORT_LIST()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT_LIST &	VIEWPORT_LIST::operator=(const VIEWPORT_LIST &C)
{
	if (this != &C)
	{
		Copy(C);
	}

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::Copy(const VIEWPORT_LIST &	C)
{
	COMMUNICATOR::Copy(C);
	_dlViewports = C._dlViewports;
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::Display() const
{
	if	(_dlViewports.GetSize() == 0)
	{
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER | CF_CLEAR_STENCIL, 0, 0, 0, 0, 1.0f);
	}
	else
	{
		QDT_DLIST <COM_INSTANCE>::ITERATOR It		= _dlViewports.GetHead();
		QDT_DLIST <COM_INSTANCE>::ITERATOR ItEnd	= _dlViewports.GetTail();
				
		while (It != ItEnd)
		{
			VIEWPORT *pViewport = static_cast<VIEWPORT *>((*It).GetCommunicator());
			if (pViewport->IsVisible())
			{
				DISPLAY_MANAGER::Instance()->SetCurrentViewportInst(*It);
				pViewport->Display();
			}
			++It;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::RayCast(int						nScreenX,
							   int						nScreenY,
							   RAY_CAST_PARAMETERS &	RayCastParameters)
{
	if (_dlViewports.IsEmpty()) return;	// Nothing to do

	QDT_DLIST <COM_INSTANCE>::ITERATOR It	= _dlViewports.GetHead();

	VIEWPORT*		pViewport = NULL;
	RAY				Ray;
	int				nViewportX;
	int				nViewportY;
	bool			bReverse = true;

	while (It != _dlViewports.GetTail())
	{
		pViewport = static_cast<VIEWPORT *>((*It).GetCommunicator());

		if (pViewport->GetRelativeCoordinates(nScreenX, nScreenY, nViewportX, nViewportY))
		{
			if (pViewport->GetActiveCameraInst().HasHandle() == false)
			{
				++It;
				continue;
			}

			CAMERA * pCamera = static_cast<CAMERA *>(pViewport->GetActiveCameraInst().GetCommunicator());

			if (pCamera == NULL)
			{
				++It;
				continue;
			}

			SCENE_GRAPH * pSG = pCamera->GetSceneGraph();

			if (pSG == NULL)
			{
				++It;
				continue;
			}

			// Point inside viewport
			Ray = pViewport->GetRay(nViewportX, nViewportY);
			RayCastParameters.SetRay(Ray);

//			pSG->RayCast(RayCastParameters);
			return;
		}

		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushOnTopViewport
//	Object:		
//	02-06-06:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::PushOnTop(const COM_INSTANCE &ViewportInst)
{
	_dlViewports.InsertTail(ViewportInst);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveViewport
//	Object:		
//	02-06-06:	RMA - Created
//-----------------------------------------------------------------------------
bool	VIEWPORT_LIST::Remove(const COM_INSTANCE &ViewportInst)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR	It;
	
	It = _dlViewports.Find(_dlViewports.GetHead(), ViewportInst);

	if (It != _dlViewports.GetTail())
	{
		_dlViewports.Remove(It);
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		InsertBefore
//	Object:		
//	02-06-06:	RMA - Created
//-----------------------------------------------------------------------------
bool	VIEWPORT_LIST::InsertBefore(const COM_INSTANCE &ViewportToInsert,
									const COM_INSTANCE &ViewportInList)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR	It;
	
	It = _dlViewports.Find(_dlViewports.GetHead(), ViewportInList);

	if (It != _dlViewports.GetTail())
	{
		_dlViewports.Insert(It, ViewportToInsert);
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrViewports
//	Object:		
//	02-06-06:	RMA - Created
//-----------------------------------------------------------------------------
int	VIEWPORT_LIST::GetNbrViewports() const
{
	return (_dlViewports.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	03-09-18:	JJA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::Clear()
{
	_dlViewports.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	05-09-14:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT * VIEWPORT_LIST::GetViewport(int iViewport)
{ 
	QDT_DLIST<COM_INSTANCE>::ITERATOR it = _dlViewports.GetHead();

	int i = 0;

	while (i < iViewport)
	{
		++i;
		++it;
	}

	return static_cast<VIEWPORT *>((*it).GetCommunicator()); 
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::Init(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	VIEWPORT_LIST::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_VIEWPORT_LIST) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	VIEWPORT_LIST::CreateGenericCommunicatorDescriptor()
{
	return ( QDT_NEW COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("VIEWPORT_LIST"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_VIEWPORT_LIST, 
											QDT_COMMENT("F8D413EC-EA41-606E-C262-3F13115C316C"),
											sizeof(VIEWPORT_LIST), 
											Init,
											QDT_COMMENT("The viewport list class.")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::EM_Construct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	VIEWPORT_LIST *	pObject;
	pObject = static_cast<VIEWPORT_LIST *>(pO);
	QDT_NEW_PLACED(pObject, VIEWPORT_LIST)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-12:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_LIST::EM_Destruct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	VIEWPORT_LIST *	pObject;
	pObject = static_cast<VIEWPORT_LIST*> (pO);
	pObject->VIEWPORT_LIST::~VIEWPORT_LIST();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
