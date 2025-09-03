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
//	CLASS:	WAY_POINT_LINK
//
//	03-08-12:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/WayPoint/WayPointLink)
#include	INCL_3DENGINE(SceneGraph/Entities/WayPoint/WayPoint)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		WAY_POINT_LINK constructor
//	Object:		
//	03-08-12:	RMA - Created
//-----------------------------------------------------------------------------
WAY_POINT_LINK::WAY_POINT_LINK()
:
_bOneWay(false)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		WAY_POINT_LINK destructor
//	Object:		
//	03-08-12:	RMA - Created
//-----------------------------------------------------------------------------
WAY_POINT_LINK::~WAY_POINT_LINK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetWayPointNode1
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	WAY_POINT_LINK::GetWayPointNode1() const
{
	return	(GetWayPoint1()->GetNodeInst());
}

//-----------------------------------------------------------------------------
//	Name:		GetWayPointNode2
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	WAY_POINT_LINK::GetWayPointNode2() const
{
	return	(GetWayPoint2()->GetNodeInst());
}

//-----------------------------------------------------------------------------
//	Name:		GetWayPoint1
//	Object:		
//	03-08-12:	RMA - Created
//-----------------------------------------------------------------------------
WAY_POINT *	WAY_POINT_LINK::GetWayPoint1() const
{
	return (static_cast<WAY_POINT *>(_WP1.GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		GetWayPoint2
//	Object:		
//	03-08-12:	RMA - Created
//-----------------------------------------------------------------------------
WAY_POINT *	WAY_POINT_LINK::GetWayPoint2() const
{
	return (static_cast<WAY_POINT *>(_WP2.GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		Distance
//	Object:		
//	03-08-13:	RMA - Created
//-----------------------------------------------------------------------------
float	WAY_POINT_LINK::SqrDistance(const VECTOR &	Pos)
{
	NODE *pNode1 = static_cast<NODE*>(GetWayPointNode1().GetCommunicator());
	NODE *pNode2 = static_cast<NODE*>(GetWayPointNode2().GetCommunicator());

	VECTOR Node1Pos;
	VECTOR Node2Pos;
	pNode1->GetGlobalPosition(Node1Pos);
	pNode2->GetGlobalPosition(Node2Pos);

	SEGMENT LinkSegment(Node1Pos, Node2Pos);

	return (LinkSegment.SqrDistance(Pos));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
