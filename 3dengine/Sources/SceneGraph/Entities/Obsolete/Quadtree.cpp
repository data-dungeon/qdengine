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
//	CLASS:	QUADTREE
//
//	01-07-23:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	_DEBUG

#include	"include.h"

#include	INC_KRNCORE(Math/Geometry/BoundingVolume/Cylinder)
#include	INC_KRNCORE(Stream)

#include	INC_3DENGINE(SceneGraph/Node)
#include	INC_3DENGINE(Display)

#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes2D/Circle)
#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment2D)
#include	INC_KRNCORE(Math/Geometry/Intersection)
#include	INC_KRNCORE(Math/Geometry/Distance)

#ifndef _DEBUG
	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Math/Geometry/Distance)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Circle)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment2D)
	#include	INL_KRNCORE(Math/Geometry/Intersection)
	#include	INL_KRNCORE(Math/Geometry/BoundingVolume/Cylinder)
	#include	INL_KRNCORE(Math/Color)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/AABR2D)

	#include	INL_3DENGINE(SceneGraph/Node)
	#include	INL_3DENGINE(Display)

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

COMMUNICATOR_DESCRIPTOR	QUADTREE::_CD(QDT_COMMENT("QUADTREE"),							// Nom du Type 
									  MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_QUADTREE,		// Id du Type
									  "2F002168-B1CD-39CA-A64E-434FF4A8B788",			// GUID Unique (Ctrl+Alt+G pour le générer)
									  sizeof(QUADTREE),									// Taille de l'objet, toujours sizeof(CLASSE)
									  ComInit,											// La méthode d'initialisation de votre objet
									  QDT_COMMENT("Class for quadtrees"),				// Description de votre objet
									  ENTITY_ABC::GetGenericCommunicatorDescriptor()	// Paramètre Optionel, Classe Mère : COMMUNICATOR par défaut
									  );

void*					QUADTREE::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		QUADTREE constructor
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE::QUADTREE()
:
ENTITY_ABC	(&_CD, ENTITY_QUADTREE),
_pRootCell	(NULL),
_nNbrCells	(0),
_nDepth		(0),
_rHeight	(5.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~QUADTREE destructor
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE::~QUADTREE()
{
	delete _pRootCell;
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE::Display(DISPLAY_PARAMETERS &	DisplayParameters,
					   bool					bDoVisibility)
{
	const static COLOR_FLOAT Green(1.0f, 0.0f, 1.0f, 0.0f);

	if (DISPLAY_MANAGER::Instance()->DisplayQuadtrees())
	{
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());

		QDT_VECTOR <SEGMENT> vSegments;

		if	(_pRootCell)
		{
			_pRootCell->Display(_rHeight, vSegments);
		}

		RENDER_MANAGER::Instance()->GetRenderContext()->Render3DSegments(vSegments, Green);	
	}
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	QUADTREE::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	if (DISPLAY_MANAGER::Instance()->DisplayQuadtrees())
	{
		return	(true);
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	01-08-22:	JLT - Created
//-----------------------------------------------------------------------------
void QUADTREE::Clear()
{
	delete _pRootCell; _pRootCell = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Invert
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
void QUADTREE::Invert()
{
	if	(_pRootCell)
	{
		_pRootCell->Invert();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Contain
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
bool	QUADTREE::Contain(COM_HANDLE *pHandle) const
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR itEach = _dlUnits.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR itTail = _dlUnits.GetTail();

	while (itEach != itTail)
	{
		QDT_NOT_IMPLEMENTED();
		++itEach;
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::RegisterUnit(COM_HANDLE *pHandle)
{
	COM_INSTANCE Instance(pHandle);
	_dlUnits.PushTail(Instance);
}

//-----------------------------------------------------------------------------
//	Name:		UnRegister
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::UnregisterUnit(COM_HANDLE *pHandle)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR itEach = _dlUnits.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR itTail = _dlUnits.GetTail();

	while (itEach != itTail)
	{
		if ((*itEach).GetHandle() == pHandle)
		{
			_dlUnits.Remove(itEach);
			return;
		}

		++itEach;
	}

	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	QUADTREE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	QDT_ASSERT(pCD == &_CD);

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	// Chunk
	_CD.AddReadChunk("QUADTREE", 1, CM_ReadChunk);
	_CD.AddWriteChunk("QUADTREE", 1, CM_WriteChunk);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	QUADTREE::GetGenericCommunicatorDescriptor()
{
	return	(&_CD);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	QUADTREE::EM_Construct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	QUADTREE	*pQuadtree;
	pQuadtree = static_cast<QUADTREE *>(pO);

	QDT_NEW_PLACED(pQuadtree, QUADTREE)();
	QDT_ASSERT(pQuadtree);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-01-22:	VMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::EM_Construct_In_Memory(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	QUADTREE	*pObject = static_cast<QUADTREE*> ( pO );

	if ( !QUADTREE::_pVTable	)
	{
		GET_VTABLE( QUADTREE );
	}
	pObject->SetVirtualTable( QUADTREE::_pVTable );
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	QUADTREE::EM_Destruct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	QUADTREE	*pQuadtree;
	pQuadtree = static_cast<QUADTREE *>(pO);

	QDT_DELETE_PLACED(pQuadtree, QUADTREE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	QUADTREE::CM_ReadChunk(IO_STREAM &		Stream,
							   COMMUNICATOR *	pCommunicator)
{
	bool		bHasARootCell;
	QUADTREE	*pQuadtree;
	pQuadtree = static_cast<QUADTREE *>(pCommunicator);
		
	Stream >> pQuadtree->_nNbrCells;
	Stream >> pQuadtree->_nDepth;
	Stream >> bHasARootCell;

	if (bHasARootCell)
	{
		pQuadtree->_pRootCell = new QUADTREE_CELL;
		pQuadtree->_pRootCell->Read(Stream);
		pQuadtree->ComputeNeighbours();
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	QUADTREE::CM_WriteChunk(IO_STREAM &		Stream,
								COMMUNICATOR *	pCommunicator)
{
	bool		bHasARootCell = false;
	QUADTREE	*pQuadtree;
	pQuadtree = static_cast<QUADTREE *>(pCommunicator);

	if (pQuadtree->_pRootCell)
	{
		bHasARootCell = true;
	}
	
	Stream.DebugOutput("Quadtree :\t");
	Stream.DebugOutput(pQuadtree->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of cells :\t");
	Stream << pQuadtree->_nNbrCells;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Depth :\t");
	Stream << pQuadtree->_nDepth;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Root cell :\t");
	Stream << bHasARootCell;
	Stream.DebugOutput("\t");
	if (bHasARootCell)
	{
		Stream << *pQuadtree->_pRootCell;
	}
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("\r\n");
}

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	04-01-22:	VMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::WriteBinary(VERSION		Version,
							  IO_STREAM &	Stream)
{
	int nThisOffset = Stream.GetPos();
	Stream.PutBufferFast( this, sizeof(QUADTREE) );

	if ( _pRootCell )
	{
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pRootCell - (char*)this ), Stream.GetPos() );
		_pRootCell->WriteBinary(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitBinary
//	Object:		
//	04-01-22:	VMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::InitBinary(VERSION	Version)
{
	if ( _pRootCell )
	{
		_pRootCell->InitBinary();
		ComputeNeighbours();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrCells
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
int	QUADTREE::GetNbrCells() const
{
	return (_nNbrCells);
}

//-----------------------------------------------------------------------------
//	Name:		GetDepth
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
int	QUADTREE::GetDepth() const
{
	return (_nDepth);
}

//-----------------------------------------------------------------------------
//	Name:		SetRootCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetRootCell(const QUADTREE_CELL * const pRootCell)
{
	if (pRootCell)
	{
		_pRootCell = new QUADTREE_CELL;
		*_pRootCell = *pRootCell;
	}
	else
		_pRootCell = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrCells
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetNbrCells(int	nNbrCells)
{
	_nNbrCells = nNbrCells;
}

//-----------------------------------------------------------------------------
//	Name:		SetDepth
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetDepth(int	nDepth)
{
	_nDepth = nDepth;
}

//-----------------------------------------------------------------------------
//	Name:		SetHeight
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetHeight(float	rHeight)
{
	_rHeight = rHeight;
}

//-----------------------------------------------------------------------------
//	Name:		GetRootCell
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE::GetRootCell() const
{
	return	(_pRootCell);
}

//-----------------------------------------------------------------------------
//	Name:		FindByPos
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE::FindCellByPos(const POINT2D &	Pos)
{
	return (_pRootCell->FindByPos(Pos));
}

//-----------------------------------------------------------------------------
//	Name:		ComputeNeighbours
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::ComputeNeighbours()
{
	_pRootCell->UpdateNeightbour();
}

//-----------------------------------------------------------------------------
//	Name:		SetState
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetState(STATE	eState)
{
	_eState = eState;
}

//-----------------------------------------------------------------------------
//	Name:		SetParentCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetParentCell(QUADTREE_CELL *	pCell)
{
	_pParentCell = pCell;
}

//-----------------------------------------------------------------------------
//	Name:		AddToClosedList
//	Object:		
//	01-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::AddToClosedList()
{
	_bInClosedList = true;
}

//-----------------------------------------------------------------------------
//	Name:		BelongToClosedList
//	Object:		
//	01-09-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	QUADTREE_CELL::BelongToClosedList()
{
	return (_bInClosedList);
}

//-----------------------------------------------------------------------------
//	Name:		SetNorthWestCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetNorthWestCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_NORTH_WEST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_NORTH_WEST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetNorthEastCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetNorthEastCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_NORTH_EAST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_NORTH_EAST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSouthWestCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetSouthWestCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_SOUTH_WEST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_SOUTH_WEST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSouthEastCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetSouthEastCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_SOUTH_EAST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_SOUTH_EAST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_SOUTH_EAST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetChildCell
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE_CELL::GetChildCell(CARDINAL	eIndex) const
{
	return	(_pChildCell[eIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL* QUADTREE_CELL::operator[](unsigned int	nIndex) const
{
	QDT_ASSERT(nIndex < QCC_MAX);
	if	(nIndex >= QCC_MAX)
	{
		return	(NULL);
	}

	return	(GetChildCell(static_cast<CARDINAL>(nIndex)));
}

//-----------------------------------------------------------------------------
//	Name:		GetTriangle
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
const TRIANGLE2D& QUADTREE_CELL::GetTriangle(unsigned int	index) const
{
	QDT_ASSERT(index<2);
	return _Quad[index];
}

//-----------------------------------------------------------------------------
//	Name:		ComputeQuad
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::ComputeQuad()
{
	POINT2D P0(_rXMin, _rYMin);
	POINT2D P1(_rXMin, _rYMax);
	POINT2D P2(_rXMax, _rYMax);
	POINT2D P3(_rXMax, _rYMin);
	
	_Quad[0] = TRIANGLE2D(P0, P1, P2);
	_Quad[1] = TRIANGLE2D(P0, P2, P3);
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
float	QUADTREE_CELL::GetArea() const
{
	return (MATH::Abs(_Quad[0].GetArea()) + MATH::Abs(_Quad[1].GetArea()));
}

//-----------------------------------------------------------------------------
//	Name:		GetCardinal
//	Object:		
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::CARDINAL	QUADTREE_CELL::GetCardinal(const SIDE	eSide,
												   const bool	bFirst)
{
	switch(eSide)
	{
	default:
		QDT_FAIL();
	case	SIDE_SOUTH:
		if	(bFirst)
		{
			return	(QCC_SOUTH_EAST_CELL);
		}
		else
		{
			return	(QCC_SOUTH_WEST_CELL);
		}
		break;

	case	SIDE_WEST:
		if	(bFirst)
		{
			return	(QCC_SOUTH_WEST_CELL);
		}
		else
		{
			return	(QCC_NORTH_WEST_CELL);
		}
		break;

	case	SIDE_NORTH:
		if	(bFirst)
		{
			return	(QCC_NORTH_EAST_CELL);
		}
		else
		{
			return	(QCC_NORTH_WEST_CELL);
		}
		break;
	case	SIDE_EAST:
		if	(bFirst)
		{
			return	(QCC_SOUTH_EAST_CELL);
		}
		else
		{
			return	(QCC_NORTH_EAST_CELL);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetOppositeSide
//	Object:		
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::SIDE	QUADTREE_CELL::GetOppositeSide(const SIDE	eSide)
{
	switch(eSide)
	{
	default:
		QDT_FAIL();
	case	SIDE_NORTH:
		return	(SIDE_SOUTH);
	case	SIDE_EAST:
		return	(SIDE_WEST);
	case	SIDE_SOUTH:
		return	(SIDE_NORTH);
	case	SIDE_WEST:
		return	(SIDE_EAST);
	}
}

//-----------------------------------------------------------------------------
//	Name:		QUADTREE_CELL constructor
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::QUADTREE_CELL()
:AABR2D()
,_eState(QCS_EMPTY)
,_pParentCell(NULL)
,_bHasChild(false)
,_bInClosedList(false)
{
	_pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	_pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_EAST_CELL] = NULL;

	ComputeQuad();
}

//-----------------------------------------------------------------------------
//	Name:		QUADTREE_CELL constructor
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::QUADTREE_CELL(const POINT2D &	PMin,
							 const POINT2D &	PMax)
:AABR2D(PMin, PMax)
,_eState(QCS_EMPTY)
,_pParentCell(NULL)
,_bHasChild(false)
,_bInClosedList(false)
{
	_pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	_pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_EAST_CELL] = NULL;

	ComputeQuad();
}

//-----------------------------------------------------------------------------
//	Name:		QUADTREE_CELL constructor
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::QUADTREE_CELL(const AABR2D &	aabr2D)
:AABR2D(aabr2D)
,_eState(QCS_EMPTY)
,_pParentCell(NULL)
,_bHasChild(false)
,_bInClosedList(false)
{
	_pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	_pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_EAST_CELL] = NULL;

	ComputeQuad();
}

//-----------------------------------------------------------------------------
//	Name:		QUADTREE_CELL constructor
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::QUADTREE_CELL(const QUADTREE_CELL &	QC)
{
	_pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	_pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	_pChildCell[QCC_SOUTH_EAST_CELL] = NULL;

	Copy(QC);
}

//-----------------------------------------------------------------------------
//	Name:		ResetClosedList
//	Object:		
//	01-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::ResetClosedList()
{
	_bInClosedList = false;

	if (_bHasChild)
	{
		_pChildCell[QCC_NORTH_WEST_CELL]->ResetClosedList();
		_pChildCell[QCC_NORTH_EAST_CELL]->ResetClosedList();
		_pChildCell[QCC_SOUTH_WEST_CELL]->ResetClosedList();
		_pChildCell[QCC_SOUTH_EAST_CELL]->ResetClosedList();
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
const QUADTREE_CELL &	QUADTREE_CELL::operator=(const QUADTREE_CELL &	QC)
{
	if	(this != &QC)
	{
		Copy(QC);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::Copy(const QUADTREE_CELL &	QC)
{
	AABR2D::Copy(QC);

	SetState(QC.GetState());
	
	_pParentCell = QC.GetParentCell();
	
	SetNorthWestCell(QC.GetNorthWestCell());
	SetNorthEastCell(QC.GetNorthEastCell());
	SetSouthWestCell(QC.GetSouthWestCell());
	SetSouthEastCell(QC.GetSouthEastCell());

	ComputeQuad();
	
	_bHasChild = QC.HasChild();

	_avpNeightbours[SIDE_NORTH] = QC._avpNeightbours[SIDE_NORTH];
	_avpNeightbours[SIDE_EAST] = QC._avpNeightbours[SIDE_EAST];
	_avpNeightbours[SIDE_SOUTH] = QC._avpNeightbours[SIDE_SOUTH];
	_avpNeightbours[SIDE_WEST] = QC._avpNeightbours[SIDE_WEST];
}

//-----------------------------------------------------------------------------
//	Name:		~QUADTREE_CELL destructor
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::~QUADTREE_CELL()
{
	ClearChildCell();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::Display(float rHeight, 
							   QDT_VECTOR <SEGMENT> & vSegments)
{
	POINT3D p1(GetXMin(), rHeight, GetYMin());
	POINT3D p2(GetXMax(), rHeight, GetYMin());
	POINT3D p3(GetXMin(), rHeight, GetYMax());
	POINT3D p4(GetXMax(), rHeight, GetYMax());
	
	// Display a box around the cell
	SEGMENT	North(p1, p2);
	SEGMENT	South(p3, p4);
	SEGMENT	West(p1, p3);
	SEGMENT	East(p2, p4);

	vSegments.PushTail(North);
	vSegments.PushTail(South);
	vSegments.PushTail(West);
	vSegments.PushTail(East);
	
	switch(GetState())
	{
	case QCS_FILLED :
		{
			// Display a cross in the cell		
			SEGMENT Cross1(p1,p4);		
			SEGMENT Cross2(p2,p3);
			
			vSegments.PushTail(Cross1);
			vSegments.PushTail(Cross2);
		}
		break;
	case QCS_SUBDIVISED :
		{
			if (_bHasChild)	
			{
				_pChildCell[QCC_NORTH_WEST_CELL]->Display(rHeight, vSegments);
				_pChildCell[QCC_NORTH_EAST_CELL]->Display(rHeight, vSegments);
				_pChildCell[QCC_SOUTH_WEST_CELL]->Display(rHeight, vSegments);
				_pChildCell[QCC_SOUTH_EAST_CELL]->Display(rHeight, vSegments);
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::Read(IO_STREAM &	Stream)
{
	int	nState;
	
	Stream >> nState;
	SetState(static_cast<STATE>(nState));
	
	AABR2D::Read(Stream);

	Stream >> _bHasChild;

	if	(HasChild())
	{
		_pChildCell[QCC_NORTH_WEST_CELL] = new QUADTREE_CELL;
		_pChildCell[QCC_NORTH_WEST_CELL]->Read(Stream);
		_pChildCell[QCC_NORTH_WEST_CELL]->_pParentCell = this;

		_pChildCell[QCC_NORTH_EAST_CELL] = new QUADTREE_CELL;
		_pChildCell[QCC_NORTH_EAST_CELL]->Read(Stream);
		_pChildCell[QCC_NORTH_EAST_CELL]->_pParentCell = this;

		_pChildCell[QCC_SOUTH_WEST_CELL] = new QUADTREE_CELL;
		_pChildCell[QCC_SOUTH_WEST_CELL]->Read(Stream);
		_pChildCell[QCC_SOUTH_WEST_CELL]->_pParentCell = this;

		_pChildCell[QCC_SOUTH_EAST_CELL] = new QUADTREE_CELL;
		_pChildCell[QCC_SOUTH_EAST_CELL]->Read(Stream);
		_pChildCell[QCC_SOUTH_EAST_CELL]->_pParentCell = this;
	}

	ComputeQuad();
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::Write(IO_STREAM &	Stream) const
{
	Stream << GetState();

	AABR2D::Write(Stream);
	
	Stream << _bHasChild;
	if	(HasChild())
	{
		_pChildCell[QCC_NORTH_WEST_CELL]->Write(Stream);
		_pChildCell[QCC_NORTH_EAST_CELL]->Write(Stream);
		_pChildCell[QCC_SOUTH_WEST_CELL]->Write(Stream);
		_pChildCell[QCC_SOUTH_EAST_CELL]->Write(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	04-01-22:	VMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::WriteBinary(IO_STREAM &	Stream)
{
	int nThisOffset = Stream.GetPos();
	Stream.PutBufferFast( this, sizeof(QUADTREE_CELL) );

	if ( HasChild() )
	{
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pChildCell[QCC_NORTH_WEST_CELL] - (char*)this ), Stream.GetPos() );
		_pChildCell[QCC_NORTH_WEST_CELL]->WriteBinary(Stream);
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pChildCell[QCC_NORTH_EAST_CELL] - (char*)this ), Stream.GetPos() );
		_pChildCell[QCC_NORTH_EAST_CELL]->WriteBinary(Stream);
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pChildCell[QCC_SOUTH_WEST_CELL] - (char*)this ), Stream.GetPos() );
		_pChildCell[QCC_SOUTH_WEST_CELL]->WriteBinary(Stream);
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pChildCell[QCC_SOUTH_EAST_CELL] - (char*)this ), Stream.GetPos() );
		_pChildCell[QCC_SOUTH_EAST_CELL]->WriteBinary(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitBinary
//	Object:		
//	04-01-22:	VMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::InitBinary()
{
	if	(HasChild())
	{
		_pChildCell[QCC_NORTH_WEST_CELL]->InitBinary();
		_pChildCell[QCC_NORTH_WEST_CELL]->_pParentCell = this;
		_pChildCell[QCC_NORTH_EAST_CELL]->InitBinary();
		_pChildCell[QCC_NORTH_EAST_CELL]->_pParentCell = this;
		_pChildCell[QCC_SOUTH_WEST_CELL]->InitBinary();
		_pChildCell[QCC_SOUTH_WEST_CELL]->_pParentCell = this;
		_pChildCell[QCC_SOUTH_EAST_CELL]->InitBinary();
		_pChildCell[QCC_SOUTH_EAST_CELL]->_pParentCell = this;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Subdivise
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
bool	QUADTREE_CELL::Subdivise()
{
	if	(HasChild())
	{
		QDT_FAIL();
		return	(false);
	}

	POINT2D P0, P1;

	float XWidth2 = (_rXMax - _rXMin) * 0.5f;
	float YWidth2 = (_rYMax - _rYMin) * 0.5f;

	if ((XWidth2 < 10.f) || (XWidth2 < 10.f))
	{
		SetState(QCS_FILLED);
		_bHasChild = false;
		return	(false);
	}

	//=============================================================================
	// JLT : disposition of the Children cells
	//
	//		see the remark in the header to see the space orientation
	//=============================================================================

	P0.SetXY(_rXMin, _rYMin);
	P1.SetXY(_rXMin + XWidth2, _rYMin + YWidth2);
	_pChildCell[QCC_NORTH_WEST_CELL] = new QUADTREE_CELL(P0, P1);
	_pChildCell[QCC_NORTH_WEST_CELL] ->SetParentCell(this);
	_pChildCell[QCC_NORTH_WEST_CELL] ->SetState(GetState());

	P0.SetXY(_rXMin + XWidth2, _rYMin);
	P1.SetXY(_rXMax, _rYMin + YWidth2);
	_pChildCell[QCC_NORTH_EAST_CELL] = new QUADTREE_CELL(P0, P1);
	_pChildCell[QCC_NORTH_EAST_CELL] ->SetParentCell(this);
	_pChildCell[QCC_NORTH_EAST_CELL] ->SetState(GetState());
	
	P0.SetXY(_rXMin, _rYMin + YWidth2);
	P1.SetXY(_rXMin + XWidth2, _rYMax);
	_pChildCell[QCC_SOUTH_WEST_CELL] = new QUADTREE_CELL(P0, P1);
	_pChildCell[QCC_SOUTH_WEST_CELL] ->SetParentCell(this);
	_pChildCell[QCC_SOUTH_WEST_CELL] ->SetState(GetState());
	
	P0.SetXY(_rXMin + XWidth2, _rYMin + YWidth2);
	P1.SetXY(_rXMax, _rYMax);
	_pChildCell[QCC_SOUTH_EAST_CELL] = new QUADTREE_CELL(P0, P1);
	_pChildCell[QCC_SOUTH_EAST_CELL] ->SetParentCell(this);
	_pChildCell[QCC_SOUTH_EAST_CELL] ->SetState(GetState());

	_bHasChild = true;

	SetState(QCS_SUBDIVISED);

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		ClearChildCell
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::ClearChildCell()
{
	delete	_pChildCell[QCC_NORTH_WEST_CELL]; _pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	delete	_pChildCell[QCC_NORTH_EAST_CELL]; _pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	delete	_pChildCell[QCC_SOUTH_WEST_CELL]; _pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	delete	_pChildCell[QCC_SOUTH_EAST_CELL]; _pChildCell[QCC_SOUTH_EAST_CELL] = NULL;
	_bHasChild = false;
}

//-----------------------------------------------------------------------------
//	Name:		GetIntersectedArea
//	Object:		
//	01-08-30:	JLT - Created
//-----------------------------------------------------------------------------
float	QUADTREE_CELL::GetIntersectedArea(const AABR2D &	Bbox) const
{
	const int	nNbrPoints = 4;
	POINT2D		aPointsOut[nNbrPoints];
	POINT2D		aPointsIn[nNbrPoints];

	aPointsIn[0] = Bbox[0];
	aPointsIn[1] = Bbox[1];
	aPointsIn[2] = Bbox[2];
	aPointsIn[3] = Bbox[3];

	int		nNbrPointsOut = nNbrPoints;
	float	rArea = 0.f;

	if	(INTERSECTION2D::ClipPolygon((AABR2D)*this, aPointsIn, aPointsOut, 4, nNbrPointsOut))
	{
		int	nNbrTriangles = nNbrPointsOut - 2;
		int	i;

		//the intersected polygon is convex so fabsf is good
		for	(i = 0 ; i < nNbrTriangles ; ++i)
		{
			rArea += MATH::Abs(TRIANGLE2D(aPointsOut[0], aPointsOut[i+1], aPointsOut[i+2]).GetArea());
		}
	}

	return	(rArea);
}

//-----------------------------------------------------------------------------
//	Name:		Invert
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::Invert()
{
	if	(HasChild())
	{
		_pChildCell[0]->Invert();
		_pChildCell[1]->Invert();
		_pChildCell[2]->Invert();
		_pChildCell[3]->Invert();
	}
	else
	{
		if	(GetState() == QCS_FILLED)
		{
			SetState(QCS_EMPTY);
		}
		else
		{
			SetState(QCS_FILLED);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateNeightbour
//	Object:		Update the list of neightbour
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::UpdateNeightbour()
{
	if	(HasChild())
	{
		GetNorthWestCell()->UpdateNeightbour();
		GetNorthEastCell()->UpdateNeightbour();
		GetSouthWestCell()->UpdateNeightbour();
		GetSouthEastCell()->UpdateNeightbour();
	}
	else
	{
		//	Okay, this is a leaf...
		FillNeightbour(SIDE_EAST);		//	Fill the east to the west side
		FillNeightbour(SIDE_SOUTH);		//	Fill the north to the south side

		//	The west side and the south side is implicitly filled by symetry
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrNeightbours
//	Object:		Get the number of neightbours in a side
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
int	QUADTREE_CELL::GetNbrNeightbours(const SIDE	eSide) const
{
	return	(_avpNeightbours[eSide].GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetNeightbour
//	Object:		Get the indexed neightbour in the side
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE_CELL::GetNeightbour(const SIDE	eSide,
											 const int	nIndex) const
{
	return	(_avpNeightbours[eSide].At(nIndex));
}

//-----------------------------------------------------------------------------
//	Name:		FindByPos
//	Object:		Find a QUADTREE_CELL under the point P
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE_CELL::FindByPos(const POINT2D &	P) const
{
	if	(Contain(P) == false)
	{
		return	(false);
	}

	if	(HasChild())
	{
		//	Okay contain it, recurse for ecah child

		QUADTREE_CELL *	pQC;

		pQC = GetNorthWestCell()->FindByPos(P);
		if	(pQC)
		{
			return	(pQC);
		}

		pQC = GetNorthEastCell()->FindByPos(P);
		if	(pQC)
		{
			return	(pQC);
		}

		pQC = GetSouthWestCell()->FindByPos(P);
		if	(pQC)
		{
			return	(pQC);
		}

		pQC = GetSouthEastCell()->FindByPos(P);
		QDT_ASSERT(pQC);
		return	(pQC);
	}
	else
	{
		return	(const_cast<QUADTREE_CELL *>(this));
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetQuadtreeCellsList
//	Object:		Get a vector of pointer of QUADTREE_CELL from point PBegin to PEnd
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < QUADTREE_CELL * > *	QUADTREE_CELL::GetQuadtreeCellsList(const POINT2D &	PBegin,
																		const POINT2D &	PEnd) const
{
	QUADTREE_CELL *	pQCBegin;
	QUADTREE_CELL *	pQCEnd;
	QUADTREE_CELL *	pCurrent;

	pQCBegin = FindByPos(PBegin);
	if	(pQCBegin == NULL)
	{
		return	(NULL);
	}
	pQCEnd = FindByPos(PEnd);
	if	(pQCEnd == NULL)
	{
		return	(NULL);
	}

	QDT_VECTOR<QUADTREE_CELL *> *	pvpQC = new	QDT_VECTOR<QUADTREE_CELL *>;
	SIDE							eXSide, eYSide;
	SIDE							eXOppositeSide, eYOppositeSide;

	if	(PBegin.GetX() <= PEnd.GetX())
	{
		eXSide = SIDE_EAST;
	}
	else
	{
		eXSide = SIDE_WEST;
	}
	eXOppositeSide = GetOppositeSide(eXSide);

	if	(PBegin.GetY() <= PEnd.GetY())
	{
		eYSide = SIDE_NORTH;
	}
	else
	{
		eYSide = SIDE_SOUTH;
	}
	eYOppositeSide = GetOppositeSide(eYSide);

	SEGMENT2D	Ray(PBegin, PEnd);

	//	Now we know the two direction to take...
	pCurrent = pQCBegin;
	for(;;)
	{
		int				i, nNbrNeightbours;
		QUADTREE_CELL *	pQC;
		SEGMENT2D		SNeightbour;
		SEGMENT2D		SCurrent;

		SEGMENT2D		SIntersection;
		SEGMENT2D		SIntersection2;
		POINT2D			PIntersection;
		bool			bChangeCurrent = false;

		//	Add this cell to the vector
		pvpQC->PushTail(pCurrent);
		if	(pCurrent == pQCEnd)
		{
			break;
		}

		//	Parse each X neightbour

		//	Get the current Cell Segment
		SCurrent = pCurrent->GetSidedSegment(eXSide);

		nNbrNeightbours = pCurrent->GetNbrNeightbours(eXSide);
		for(i = 0 ; i < nNbrNeightbours ; ++i)
		{
			pQC = pCurrent->GetNeightbour(eXSide, i);

			//	Get the intersecting segment
			SNeightbour = pQC->GetSidedSegment(eXOppositeSide);

			//	First find the exact intersection of the two segments
			switch(INTERSECTION2D::SegmentSegment(SCurrent, SNeightbour, SIntersection, PIntersection))
			{
			default:
			case	INTERSECTION2D::I_NO_INTERSECTION_NO_COLINEAR_SEGMENT:
			case	INTERSECTION2D::I_NO_INTERSECTION_COLINEAR_SEGMENT:
			case	INTERSECTION2D::I_ONE_INTERSECTION:
				//	Ther must be one segment intersection because the neightbour is generated with by quadtree !
				QDT_FAIL();
			case	INTERSECTION2D::I_SEGMENT_INTERSECTION:
				//	Okay this is the correct case
				break;
			}

			//	Now we handle the intersection between the right frontier segment and the Segment of way
			switch(INTERSECTION2D::SegmentSegment(Ray, SIntersection, SIntersection2, PIntersection))
			{
			default:
				QDT_FAIL();
			case	INTERSECTION2D::I_NO_INTERSECTION_NO_COLINEAR_SEGMENT:
			case	INTERSECTION2D::I_NO_INTERSECTION_COLINEAR_SEGMENT:
				break;
			case	INTERSECTION2D::I_ONE_INTERSECTION:
			case	INTERSECTION2D::I_SEGMENT_INTERSECTION:
				pCurrent = pQC;
				bChangeCurrent = true;
				continue;
			}

			if	(bChangeCurrent)
			{
				break;
			}
		}

		if	(bChangeCurrent)
		{
			continue;
		}

		//	Parse each X neightbour

		//	Get the current Cell Segment
		SCurrent = pCurrent->GetSidedSegment(eXSide);

		nNbrNeightbours = pCurrent->GetNbrNeightbours(eXSide);
		for(i = 0 ; i < nNbrNeightbours ; ++i)
		{
			bool	bExit = false;

			pQC = GetNeightbour(eXSide, i);

			//	Get the intersecting segment
			SNeightbour = pQC->GetSidedSegment(eXOppositeSide);

			//	First find the exact intersection of the two segments
			switch(INTERSECTION2D::SegmentSegment(SCurrent, SNeightbour, SIntersection, PIntersection))
			{
			default:
			case	INTERSECTION2D::I_NO_INTERSECTION_NO_COLINEAR_SEGMENT:
			case	INTERSECTION2D::I_NO_INTERSECTION_COLINEAR_SEGMENT:
			case	INTERSECTION2D::I_ONE_INTERSECTION:
				//	Ther must be one segment intersection because the neightbour is generated with by quadtree !
				QDT_FAIL();
			case	INTERSECTION2D::I_SEGMENT_INTERSECTION:
				//	Okay this is the correct case
				break;
			}

			//	Now we handle the intersection between the right frontier segment and the Segment of way
			switch(INTERSECTION2D::SegmentSegment(Ray, SIntersection, SIntersection2, PIntersection))
			{
			default:
				QDT_FAIL();
			case	INTERSECTION2D::I_NO_INTERSECTION_NO_COLINEAR_SEGMENT:
			case	INTERSECTION2D::I_NO_INTERSECTION_COLINEAR_SEGMENT:
				break;
			case	INTERSECTION2D::I_ONE_INTERSECTION:
			case	INTERSECTION2D::I_SEGMENT_INTERSECTION:
				pCurrent = pQC;
				bChangeCurrent = true;
				continue;
			}

			if	(bExit)
			{
				break;
			}
		}

		if	(bChangeCurrent)
		{
			continue;
		}

		QDT_FAIL();
	}

	return	(pvpQC);
}

//-----------------------------------------------------------------------------
//	Name:		FillNeightbour
//	Object:		Fill the sided neightbours of a QUADTREE_CELL
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::FillNeightbour(const SIDE	eSide)
{
	QUADTREE_CELL *		pFatherQC;
	QUADTREE_CELL *		pNeightbourQC;
	int					nLevel = -1;
	QDT_VECTOR<char>	vbParity;
	SIDE				eOppositeSide = GetOppositeSide(eSide);

	ClearNeightbours(eOppositeSide);

	//	Search the parent in the side part
	pFatherQC = GetPartParent(eSide, nLevel, vbParity);

	if	(pFatherQC)
	{
		QUADTREE_CELL *	pBrotherQC;
		bool			bParity;

		QDT_ASSERT(nLevel > 0);
		QDT_ASSERT(nLevel == (int)vbParity.GetSize());
		QDT_ASSERT(pFatherQC->HasChild());

		//	Retrieve the last parity
		if (*vbParity.GetLast())
		{
			bParity = true;
		}
		else
		{
			bParity = false;
		}

		vbParity.PopTail();
		--nLevel;

		//	Search brother in the opposite side of the father but with the same parity
		pBrotherQC = pFatherQC->GetChildCell(GetCardinal(eOppositeSide, bParity));

		//	Now find the right neightbour (it can be also a father or it can be larger)
		pNeightbourQC = pBrotherQC->GetRecurse(eSide, nLevel, vbParity);

		//	Recurse each of its child on the eSide, make the link
		FillNeightbour(pNeightbourQC, eSide);
	}
	else
	{
		//	No father, it's an extrem eSide QUADTREE_CELL
		QDT_ASSERT(nLevel == -1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPartParent
//	Object:		Get the sided parent, return the level and a parity vector
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE_CELL::GetPartParent(const SIDE			eSide,
											 int &				nLevel,
											 QDT_VECTOR<char> &	vbParity)
{
	QUADTREE_CELL *	pFatherQC;
	QUADTREE_CELL *	pThisQC;

	pFatherQC = this;
	nLevel = 0;

	for(;;)
	{
		pThisQC = pFatherQC;
		pFatherQC = pThisQC->GetParentCell();
		if	(pFatherQC == NULL)
		{
			nLevel = -1;
			return	(NULL);
		}

		++nLevel;

		switch(eSide)
		{
		default:
			QDT_FAIL();
		case	SIDE_NORTH:
			if	(pFatherQC->GetNorthWestCell() == pThisQC)
			{
				vbParity.PushTail(false);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetNorthEastCell() == pThisQC)
			{
				vbParity.PushTail(true);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetSouthWestCell() == pThisQC)
			{
				vbParity.PushTail(false);
			}
			else if	(pFatherQC->GetSouthEastCell() == pThisQC)
			{
				vbParity.PushTail(true);
			}
			else
			{
				QDT_FAIL();
			}
			break;
		case	SIDE_EAST:
			if	(pFatherQC->GetNorthEastCell() == pThisQC)
			{
				vbParity.PushTail(false);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetSouthEastCell() == pThisQC)
			{
				vbParity.PushTail(true);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetNorthWestCell() == pThisQC)
			{
				vbParity.PushTail(false);
			}
			else if	(pFatherQC->GetSouthWestCell() == pThisQC)
			{
				vbParity.PushTail(true);
			}
			else
			{
				QDT_FAIL();
			}
			break;
		case	SIDE_SOUTH:
			if	(pFatherQC->GetSouthWestCell() == pThisQC)
			{
				vbParity.PushTail(false);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetSouthEastCell() == pThisQC)
			{
				vbParity.PushTail(true);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetNorthWestCell() == pThisQC)
			{
				vbParity.PushTail(false);
			}
			else if	(pFatherQC->GetNorthEastCell() == pThisQC)
			{
				vbParity.PushTail(true);
			}
			else
			{
				QDT_FAIL();
			}
			break;
		case	SIDE_WEST:
			if	(pFatherQC->GetNorthWestCell() == pThisQC)
			{
				vbParity.PushTail(false);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetSouthWestCell() == pThisQC)
			{
				vbParity.PushTail(true);
				return	(pFatherQC);
			}
			else if	(pFatherQC->GetNorthEastCell() == pThisQC)
			{
				vbParity.PushTail(false);
			}
			else if	(pFatherQC->GetSouthEastCell() == pThisQC)
			{
				vbParity.PushTail(true);
			}
			else
			{
				QDT_FAIL();
			}
			break;
		}

		//	Not the correct father, try next one...
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetRecurse
//	Object:		Get a QUADTREE_CELL by side, a max level and a max parity vector
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE_CELL::GetRecurse(const SIDE			eSide,
										  const int				nLevel,
										  QDT_VECTOR<char> &	vbParity)
{
	QUADTREE_CELL * pQC;
	bool			bParity;

	if	(nLevel == 0)
	{
		return	(this);
	}

	if	(HasChild() == false)
	{
		return	(this);
	}

	QDT_ASSERT(nLevel == (int)vbParity.GetSize());

	if (*vbParity.GetLast())
	{
		bParity = true;
	}
	else
	{
		bParity = false;
	}

	vbParity.PopTail();

	pQC = GetChildCell(GetCardinal(eSide, bParity));

	return	(pQC->GetRecurse(eSide, nLevel - 1, vbParity));
}

//-----------------------------------------------------------------------------
//	Name:		FillNeightbour
//	Object:		Update the neightbours list by side
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::FillNeightbour(QUADTREE_CELL *	pQC,
									  const SIDE		eSide)
{
	QDT_ASSERT(pQC);

	if	(pQC->HasChild())
	{
		FillNeightbour(pQC->GetChildCell(GetCardinal(eSide, false)), eSide);
		FillNeightbour(pQC->GetChildCell(GetCardinal(eSide, true)), eSide);
	}
	else
	{
		//	Fill with childs only...
		_avpNeightbours[GetOppositeSide(eSide)].PushTail(pQC);

		pQC->_avpNeightbours[eSide].PushTail(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ClearNeightbours
//	Object:		Clear the the neightbours list by side
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::ClearNeightbours(const SIDE	eSide)
{
	_avpNeightbours[eSide].Clear();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
