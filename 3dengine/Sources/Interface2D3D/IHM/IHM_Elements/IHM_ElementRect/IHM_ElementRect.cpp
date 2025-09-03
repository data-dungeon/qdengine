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
//	CLASS:	IHM_ELEMENT_RECT
//
//	01-11-30:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementRect)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Interface)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/typeFloat)


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_RECT constructor
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_RECT::IHM_ELEMENT_RECT()
:
IHM_ELEMENT_ABC(GetGenericCommunicatorDescriptor()),
_rRelativePosLeft	( 0.0f ),
_rRelativePosRight	( 0.0f ),
_rRelativePosTop	( 0.0f ),
_rRelativePosBottom	( 0.0f )
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_RECT constructor
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_RECT::IHM_ELEMENT_RECT(CLASS_DESCRIPTOR * const pCD)
:
IHM_ELEMENT_ABC(pCD),
_rRelativePosLeft	( 0.0f ),
_rRelativePosRight	( 0.0f ),
_rRelativePosTop	( 0.0f ),
_rRelativePosBottom	( 0.0f )
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_RECT destructor
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_RECT::~IHM_ELEMENT_RECT()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		IsPointInElement
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_RECT::IsPointInElement(VIEWPORT *	pViewport,
										   int			nPointX,
										   int			nPointY)
{
	return ( ( nPointX > GetAbsolutePosLeft(pViewport)   ) &&
			 ( nPointX < GetAbsolutePosRight(pViewport)  ) &&
			 ( nPointY > GetAbsolutePosTop(pViewport)    ) &&
			 ( nPointY < GetAbsolutePosBottom(pViewport) ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetRelativePosition
//	Object:		
//	01-09-28:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_RECT::GetRelativePosition(float &	rLeft,
											  float &	rRight,
											  float &	rTop,
											  float &	rBottom) const
{
	rLeft	= _rRelativePosLeft;
	rRight	= _rRelativePosRight;
	rTop	= _rRelativePosTop;
	rBottom	= _rRelativePosBottom;
}

//-----------------------------------------------------------------------------
//	Name:		DebugDisplay
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	IHM_ELEMENT_RECT::DebugDisplay(VIEWPORT * pViewport)
{
	VECTOR				Pt1, Pt2;
	QDT_VECTOR<SEGMENT>	vSegments;
	IHM_INTERFACE *		pInterface;
	float				rLeft, rRight, rTop, rBottom;

	pInterface = GetInterfaceContainer();

	if ( pViewport == NULL )
	{
		rLeft	= GetRelativePosLeft();
		rRight	= GetRelativePosRight();
		rTop	= GetRelativePosTop();
		rBottom	= GetRelativePosBottom();
	}
	else
	{
		rLeft	= pViewport->GetLeft() + pViewport->GetWidth()  * GetRelativePosLeft()   / 100.0f;
		rRight	= pViewport->GetLeft() + pViewport->GetWidth()  * GetRelativePosRight()  / 100.0f;
		rTop	= pViewport->GetTop()  + pViewport->GetHeight() * GetRelativePosTop()    / 100.0f;
		rBottom	= pViewport->GetTop()  + pViewport->GetHeight() * GetRelativePosBottom() / 100.0f;
	}

	Pt1.SetXYZ( rLeft,  rTop,    0.0f );
	Pt2.SetXYZ( rLeft,  rBottom, 0.0f );
	vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );

	Pt1.SetXYZ( rLeft,  rBottom, 0.0f );
	Pt2.SetXYZ( rRight, rBottom, 0.0f );
	vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );

	Pt1.SetXYZ( rRight, rBottom, 0.0f );
	Pt2.SetXYZ( rRight, rTop,    0.0f );
	vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );

	Pt1.SetXYZ( rRight, rTop,    0.0f );
	Pt2.SetXYZ( rLeft,  rTop,    0.0f );
	vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );

	QDT_NOT_IMPLEMENTED();
//	RENDER_MANAGER::Instance()->GetRenderContext()->Render2DSegments( vSegments, INTERFACE_MANAGER::Instance()->GetIHMDebugDisplayColor() );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetAbsolutePosLeft
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_RECT::GetAbsolutePosLeft(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rLeft;
	
	// In which viewport is the element?
	if ( ( pViewport == NULL ) && ( HasInterfaceContainer() == true ) )
	{
		pViewport = GetInterfaceContainer()->GetViewport();
	}

	// Get screen relative coordinate.
	if ( pViewport == NULL )
	{
		rScreenRelativePos = _rRelativePosLeft;
	}
	else
	{
		rScreenRelativePos = pViewport->GetLeft() + pViewport->GetWidth() * _rRelativePosLeft * 0.01f;
	}

	rLeft = INTERFACE_MANAGER::Instance()->GetWindowWidth() * 0.01f * rScreenRelativePos;

	return ( rLeft );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsolutePosRight
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_RECT::GetAbsolutePosRight(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rRight;
	
	// In which viewport is the element?
	if ( ( pViewport == NULL ) && ( HasInterfaceContainer() == true ) )
	{
		pViewport = GetInterfaceContainer()->GetViewport();
	}

	// Get screen relative coordinate.
	if ( pViewport == NULL )
	{
		rScreenRelativePos = _rRelativePosRight;
	}
	else
	{
		rScreenRelativePos = pViewport->GetLeft() + pViewport->GetWidth() * _rRelativePosRight * 0.01f;
	}

	rRight = INTERFACE_MANAGER::Instance()->GetWindowWidth() * 0.01f * rScreenRelativePos;

	return ( rRight );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsolutePosTop
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_RECT::GetAbsolutePosTop(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rTop;

	// In which viewport is the element?
	if ( ( pViewport == NULL ) && ( HasInterfaceContainer() == true ) )
	{
		pViewport = GetInterfaceContainer()->GetViewport();
	}

	// Get screen relative coordinate.
	if ( pViewport == NULL )
	{
		rScreenRelativePos = _rRelativePosTop;
	}
	else
	{
		rScreenRelativePos = pViewport->GetTop() + pViewport->GetHeight() * _rRelativePosTop * 0.01f;
	}

	rTop = INTERFACE_MANAGER::Instance()->GetWindowHeight() * 0.01f * rScreenRelativePos;

	return ( rTop );
}

//-----------------------------------------------------------------------------
//	Name:		GetAbsolutePosBottom
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_RECT::GetAbsolutePosBottom(VIEWPORT * pViewport) const
{
	float	rScreenRelativePos, rBottom;
	
	// In which viewport is the element?
	if ( ( pViewport == NULL ) && ( HasInterfaceContainer() == true ) )
	{
		pViewport = GetInterfaceContainer()->GetViewport();
	}

	// Get screen relative coordinate.
	if ( pViewport == NULL )
	{
		rScreenRelativePos = _rRelativePosBottom;
	}
	else
	{
		rScreenRelativePos = pViewport->GetTop() + pViewport->GetHeight() * _rRelativePosBottom * 0.01f;
	}

	rBottom = INTERFACE_MANAGER::Instance()->GetWindowHeight() * 0.01f * rScreenRelativePos;

	return ( rBottom );
}

float	IHM_ELEMENT_RECT::GetRelativePosLeft()   const						{return ( _rRelativePosLeft );}
float	IHM_ELEMENT_RECT::GetRelativePosRight()  const						{return ( _rRelativePosRight );}
float	IHM_ELEMENT_RECT::GetRelativePosTop()    const						{return ( _rRelativePosTop );}
float	IHM_ELEMENT_RECT::GetRelativePosBottom() const						{return ( _rRelativePosBottom );}
																			
void	IHM_ELEMENT_RECT::SetRelativePosLeft(  float rRelativePosLeft)		{_rRelativePosLeft	 = rRelativePosLeft;}
void	IHM_ELEMENT_RECT::SetRelativePosRight( float rRelativePosRight)		{_rRelativePosRight	 = rRelativePosRight;}
void	IHM_ELEMENT_RECT::SetRelativePosTop(   float rRelativePosTop)		{_rRelativePosTop 	 = rRelativePosTop;}
void	IHM_ELEMENT_RECT::SetRelativePosBottom(float rRelativePosBottom)	{_rRelativePosBottom = rRelativePosBottom;}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_RECT::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pM = METHOD_DESCRIPTOR::Allocate("SetRelativePosition", "Set the relative position (in percentage of the rendering window (or viewport if any) size) of an element.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "rLeftRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "rRightRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "rTopRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "rBottomRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(CMD_SET_RELATIVE_POSITION, pM, CM_SetRelativePosition, EM_SetRelativePosition);

	pM = METHOD_DESCRIPTOR__Allocate("GetRelativePosition", "Get the relative position of an element (in percentage of the rendering window (or viewport if any)).");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rLeftRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rRightRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rTopRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rBottomRelativePos", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_RELATIVE_POSITION, pM, EM_GetRelativePosition);
}

	
//-----------------------------------------------------------------------------
//	Name:		EM_SetRelativePosition
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_RECT::EM_SetRelativePosition(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_RECT *	pIhmElementRect;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementRect	= (IHM_ELEMENT_RECT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementRect);

	struct	PARAMETERS
	{
		float	_rLeftRelativePos;
		float	_rRightRelativePos;
		float	_rTopRelativePos;
		float	_rBottomRelativePos;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	pIhmElementRect->SetRelativePosLeft(   pParameters->_rLeftRelativePos );
	pIhmElementRect->SetRelativePosRight(  pParameters->_rRightRelativePos );
	pIhmElementRect->SetRelativePosTop(    pParameters->_rTopRelativePos );
	pIhmElementRect->SetRelativePosBottom( pParameters->_rBottomRelativePos );
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetFont
//	Object:		
//	06-03-16:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_RECT::CM_SetRelativePosition(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_RELATIVE_POSITION);
	COM_INSTANCE pObjet;
	Stack.GetThis(pObjet);
	
	Float32 rLeftRelativePos;
	Float32 rRightRelativePos;
	Float32 rTopRelativePos;
	Float32 rBottomRelativePos;

	IHM_ELEMENT_RECT	*pIhmRect = static_cast<IHM_ELEMENT_RECT *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, rLeftRelativePos);
	Stack.GetParameter(1, rRightRelativePos);
	Stack.GetParameter(2, rTopRelativePos);
	Stack.GetParameter(3, rBottomRelativePos);

	pIhmRect->SetRelativePosLeft(   rLeftRelativePos );
	pIhmRect->SetRelativePosRight(  rRightRelativePos );
	pIhmRect->SetRelativePosTop(    rTopRelativePos );
	pIhmRect->SetRelativePosBottom( rBottomRelativePos );

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetRelativePosition
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_RECT::EM_GetRelativePosition(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_RECT *	pIhmElementRect;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementRect	= (IHM_ELEMENT_RECT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementRect);

	struct	PARAMETERS
	{
		float *	_rLeftRelativePos;
		float *	_rRightRelativePos;
		float *	_rTopRelativePos;
		float *	_rBottomRelativePos;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	*pParameters->_rLeftRelativePos = pIhmElementRect->GetRelativePosLeft();
	*pParameters->_rRightRelativePos = pIhmElementRect->GetRelativePosRight();
	*pParameters->_rTopRelativePos = pIhmElementRect->GetRelativePosTop();
	*pParameters->_rBottomRelativePos = pIhmElementRect->GetRelativePosBottom();
}


//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_RECT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_RECT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_RECT::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_RECT"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_RECT, 
										QDT_COMMENT("376A36AB-E683-3E26-B1BA-D78C1471C2AE"),
										-1, 
										Init, 
										QDT_COMMENT("Abstract Class for all IHM_ELEMENT_* ."),
										IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor()));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
