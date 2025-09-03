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
//	CLASS:	PATH
//
//	01-09-19:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include	"Include.h"

#include	INC_3DENGINE(SceneGraph/Entities/Quadtree)
#include	INC_3DENGINE(PathFinding)
#include	INC_3DENGINE(Display)

#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment2D)

#include	INC_KRNCOM(BaseType)
#include	INC_KRNCOM(ComplexType)

#ifndef _DEBUG

	#include	INL_3DENGINE(Display)

	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment2D)
	#include	INL_KRNCORE(Math/Geometry/More)
	#include	INL_KRNCORE(Math/Color)
	
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

COMMUNICATOR_DESCRIPTOR			 PATH::_CD("PATH", 
										   MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PATH,
										   "932E74FF-B6A7-AC5D-4B36-DEB70410646F", 
										   sizeof(PATH), 
										   ComInit, 
										   QDT_COMMENT("Path"),
										   ENTITY_ABC::GetGenericCommunicatorDescriptor());

//-----------------------------------------------------------------------------
//	Name:		PATH constructor
//	Object:		
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
PATH::PATH()
:
ENTITY_ABC	(&_CD, ENTITY_PATH),
_rHeight	(5.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~PATH destructor
//	Object:		
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
PATH::~PATH()
{
	ClearWayPoints();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::Display(DISPLAY_PARAMETERS &	DisplayParameters,
					  bool					bDoVisibility)
{
	static COLOR_FLOAT Red(1.0f, 1.0f, 0.0f, 0.0f);
	static COLOR_FLOAT Blue(1.0f, 0.0f, 0.0, 1.0f);

	if (DISPLAY_MANAGER::Instance()->DisplayQuadtrees())
	{
		MATRIX	ID(true);
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&ID);

		if (!_dlDrawnWayPoints.IsEmpty())
		{
			QDT_DLIST <POINT2D>::ITERATOR EachWayPoint = _dlDrawnWayPoints.GetHead();
			QDT_DLIST <POINT2D>::ITERATOR PrevWayPoint = _dlDrawnWayPoints.GetHead();
			QDT_DLIST <POINT2D>::ITERATOR LastWayPoint = _dlDrawnWayPoints.GetTail();

			SPHERE Sphere(POINT3D((*EachWayPoint).GetX(), _rHeight, (*EachWayPoint).GetY()), _rRadius);
			Sphere.Display(Red, 24); 

			++EachWayPoint;

			while (EachWayPoint != LastWayPoint)
			{
				POINT3D Pt0((*EachWayPoint).GetX(), _rHeight, (*EachWayPoint).GetY());
				POINT3D Pt1((*PrevWayPoint).GetX(), _rHeight, (*PrevWayPoint).GetY());

				SEGMENT Sg(Pt0, Pt1);

				Sg.Display(Red);		// Red

				Sphere.SetCenter(Pt0);
				Sphere.Display(Red, 24); 

				++EachWayPoint;
				++PrevWayPoint;
			}
		}

		if (!_dlDebug.IsEmpty())
		{
/*			QDT_DLIST <POINT2D>::ITERATOR EachWayPoint = _dlDebug.GetHead();
			QDT_DLIST <POINT2D>::ITERATOR PrevWayPoint = _dlDebug.GetHead();
			QDT_DLIST <POINT2D>::ITERATOR LastWayPoint = _dlDebug.GetTail();

			SPHERE Sphere(POINT3D((*EachWayPoint).GetX(), _rHeight, (*EachWayPoint).GetY()), _rRadius);
			Sphere.Display(Blue, 24); 

			++EachWayPoint;

			while (EachWayPoint != LastWayPoint)
			{
				POINT3D Pt0((*EachWayPoint).GetX(), _rHeight, (*EachWayPoint).GetY());
				POINT3D Pt1((*PrevWayPoint).GetX(), _rHeight, (*PrevWayPoint).GetY());

				SEGMENT Sg(Pt0, Pt1);

				Sg.Display(Blue);		// Red

				Sphere.SetCenter(Pt0);
				Sphere.Display(Blue, 24); 

				++EachWayPoint;
				++PrevWayPoint;
			}*/
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	PATH::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	if (DISPLAY_MANAGER::Instance()->DisplayQuadtrees())
	{
		return	(true);
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Smooth
//	Object:		For each point, we check if we can remove it.
//	01-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::Smooth(QUADTREE *pQuadtree)
{
	_dlDrawnWayPoints.Clear();
	_dlDebug.Clear();

	_dlDebug = _dlWayPoints;

	/////////////////////////////////////////////
	// FIRST PASS
	/////////////////////////////////////////////
	if (_dlWayPoints.GetSize() >=  3)
	{
		QDT_DLIST <POINT2D>::ITERATOR CheckPoint	= _dlWayPoints.GetHead();
		QDT_DLIST <POINT2D>::ITERATOR CurrentPoint	= _dlWayPoints.GetHead();
		QDT_DLIST <POINT2D>::ITERATOR LastPoint		= _dlWayPoints.GetTail();
		QDT_DLIST <POINT2D>::ITERATOR Temp;
		QDT_DLIST <POINT2D>::ITERATOR Next;
		
		++CurrentPoint;
		Next = CurrentPoint;
		++Next;
		
		while (Next != LastPoint)
		{
			SEGMENT2D Sg(*CheckPoint, *Next);

			if (!pQuadtree->IntersectWithSegment(Sg, _rRadius))
			{
				// Make a straight path between those points
				Temp = CurrentPoint;
				++CurrentPoint;
				++Next;
				_dlWayPoints.Remove(Temp);
			}
			else
			{
				CheckPoint = CurrentPoint;
				++CurrentPoint;
				++Next;
			}
		}
	}

	/////////////////////////////////////////////
	// SECOND PASS
	/////////////////////////////////////////////
	if (_dlWayPoints.GetSize() >=  3)
	{
		QDT_DLIST <POINT2D>::ITERATOR CurrentPoint	= _dlWayPoints.GetHead();
		QDT_DLIST <POINT2D>::ITERATOR LastPoint		= _dlWayPoints.GetTail();
		QDT_DLIST <POINT2D>::ITERATOR Next;
		
		Next = CurrentPoint;
		++Next;

		POINT2D Mid;
		
		while (Next != LastPoint)
		{
			Mid = ((*CurrentPoint) + (*Next)) / 2.0f;

			_dlWayPoints.Insert(Next, Mid);

			++CurrentPoint;
			++Next;
		}
	}

	/////////////////////////////////////////////
	// THIRD PASS
	/////////////////////////////////////////////
	if (_dlWayPoints.GetSize() >=  3)
	{
		QDT_DLIST <POINT2D>::ITERATOR CheckPoint	= _dlWayPoints.GetHead();
		QDT_DLIST <POINT2D>::ITERATOR CurrentPoint	= _dlWayPoints.GetHead();
		QDT_DLIST <POINT2D>::ITERATOR LastPoint		= _dlWayPoints.GetTail();
		QDT_DLIST <POINT2D>::ITERATOR Temp;
		QDT_DLIST <POINT2D>::ITERATOR Next;
		
		++CurrentPoint;
		Next = CurrentPoint;
		++Next;
		
		while (Next != LastPoint)
		{
			SEGMENT2D Sg(*CheckPoint, *Next);

			if (!pQuadtree->IntersectWithSegment(Sg, _rRadius))
			{
				// Make a straight path between those points
				Temp = CurrentPoint;
				++CurrentPoint;
				++Next;
				_dlWayPoints.Remove(Temp);
			}
			else
			{
				CheckPoint = CurrentPoint;
				++CurrentPoint;
				++Next;
			}
		}
	}

	_dlDrawnWayPoints = _dlWayPoints;
}

//-----------------------------------------------------------------------------
//	Name:		GetNextWayPoint
//	Object:		
//	01-10-03:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D	PATH::GetNextWayPoint()
{
	POINT2D Res = *(_dlWayPoints.GetHead());

	_dlWayPoints.RemoveHead();

	return (Res);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *			pMD;

	QDT_ASSERT(pCD == &_CD);

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = new	METHOD_DESCRIPTOR(QDT_COMMENT("IsEmpty"), QDT_COMMENT("Is empty?"));
	pMD->SetReturn(QDT_COMMENT("bRes"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_PATH_IS_EMPTY, pMD, EM_IsEmpty);

	pMD = new	METHOD_DESCRIPTOR(QDT_COMMENT("GetNextWayPoint"), QDT_COMMENT("Get next way point."));
	pMD->SetReturn(QDT_COMMENT("Pos"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_PATH_GET_NEXT_WAY_POINT, pMD, EM_GetNextWayPoint);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PATH::GetGenericCommunicatorDescriptor()
{
	return(&_CD);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::EM_Construct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	PATH *pPath;
	pPath = static_cast<PATH *>(pO);

	QDT_NEW_PLACED(pPath, PATH)();
	QDT_ASSERT(pPath);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::EM_Destruct(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	PATH *pPath;
	pPath = static_cast<PATH *>(pO);

	QDT_DELETE_PLACED(pPath, PATH);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsEmpty
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::EM_IsEmpty(void * const	pO,
						 void * const	pR,
						 void * const	pP)
{
	
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	PATH *			pPS			 = static_cast<PATH*>(pComInstance->GetCommunicator());
	bool*			bValue		 = static_cast<bool*>(pR);

	*bValue = pPS->IsEmpty();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetNextWayPoint
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH::EM_GetNextWayPoint(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	PATH *			pPS			 = static_cast<PATH*>(pComInstance->GetCommunicator());
	POINT2D*		pPos		 = static_cast<POINT2D*>(pR);

	QDT_NEW_PLACED(pPos, POINT2D)(pPS->GetNextWayPoint());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
