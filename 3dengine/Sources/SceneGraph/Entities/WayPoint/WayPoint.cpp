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
//	CLASS:	WAY_POINT
//
//	03-08-08:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/WayPoint/WayPoint)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		WAY_POINT constructor
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
WAY_POINT::WAY_POINT()
:
ENTITY_ABC(GetGenericCommunicatorDescriptor(), ENTITY_WAY_POINT),
_rRadius(0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		WAY_POINT destructor
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
WAY_POINT::~WAY_POINT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	WAY_POINT::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
#ifndef _MASTER
	if (DISPLAY_MANAGER::Instance()->DisplayWayPoints())
	{
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
		
		const COLOR_FLOAT Green(1.0f, 0.0f, 1.0f, 0.0f);
		const float rHeight = 40.0f;

		SEGMENT Segment(VECTOR(0.0f, 0.0f, 0.0f), VECTOR(0.0f, rHeight * 2.0f, 0.0f));

		Segment.Display(Green);

		// Horizontal circle
		const unsigned int nStep = 20;
		unsigned int i;
		VECTOR	Pt1(_rRadius, rHeight, 0.0f);
		VECTOR Pt2;

		Pt2.SetY(rHeight);
		
		for (i = 0; i < nStep; i++)
		{
			Pt2.SetX(_rRadius * MATH::Cos(2.0f * MATH::PI() * static_cast<float>(i + 1) / static_cast<float>(nStep)));
			Pt2.SetZ(_rRadius * MATH::Sin(2.0f * MATH::PI() * static_cast<float>(i + 1) / static_cast<float>(nStep)));

			RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegment(SEGMENT(Pt1, Pt2), Green);

			Pt1 = Pt2;
		}

		// Display connections
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&MATRIX::IDENTITY);

		for (i = 0 ; i < _vConnections.GetSize() ; ++i)
		{
			WAY_POINT_LINK *pLink = _vConnections[i];

			NODE *pNode1 = static_cast<NODE*>(pLink->GetWayPointNode1().GetCommunicator());
			NODE *pNode2 = static_cast<NODE*>(pLink->GetWayPointNode2().GetCommunicator());

			VECTOR Point1;
			VECTOR Point2;

			pNode1->GetGlobalPosition(Point1);
			pNode2->GetGlobalPosition(Point2);
			Point1.SetY(Point1.GetY() + rHeight);
			Point2.SetY(Point2.GetY() + rHeight);
			Segment.SetPoint1(Point1);
			Segment.SetPoint2(Point2);
			Segment.Display(Green);

			VECTOR Dir;
			VECTOR ArrowCenter(Point1);
			Dir.Sub(Point2, Point1);
			Dir.Mul(2.0f / 3.0f);

			Dir.Normalize();
			Dir.Mul(10.0f);

			VECTOR Arrow1(Dir);
			VECTOR Arrow2(Dir);

			MATRIX RotY(MATRIX::IDENTITY);

			RotY.SetRotY(MATH::PI() / 4.0f);
			RotY.TransformDirection(Arrow1);
			Segment.SetPoint1(ArrowCenter);
			Segment.SetPoint2(ArrowCenter + Arrow1);
			Segment.Display(Green);

			RotY.SetRotY(-MATH::PI() / 4.0f);
			RotY.TransformDirection(Arrow2);
			Segment.SetPoint2(ArrowCenter + Arrow2);
			Segment.Display(Green);
		}
	}
#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	WAY_POINT::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	WAY_POINT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	WAY_POINT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_WAY_POINT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	WAY_POINT::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("WAY_POINT"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_WAY_POINT,
										QDT_COMMENT("082425FC-F411-82FE-9AB2-F6232C2C20FC"), 
										sizeof(WAY_POINT), 
										ComInit, 
										QDT_COMMENT("Way point used by path finding"),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	WAY_POINT::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	WAY_POINT *pWP;
	pWP = static_cast<WAY_POINT *>(pO);

	QDT_NEW_PLACED(pWP, WAY_POINT)();
	QDT_ASSERT(pWP);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	WAY_POINT::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	WAY_POINT *pWP;
	pWP = static_cast<WAY_POINT *>(pO);

	QDT_DELETE_PLACED(pWP, WAY_POINT);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
