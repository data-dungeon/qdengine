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
//	CLASS:	PATH_FINDER
//
//	01-09-19:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include	"Include.h"

#include	INC_KRNCOM(BaseType)
#include	INC_KRNCOM(ComplexType)

#include	INC_KRNCORE(Time)

#include	INC_3DENGINE(SceneGraph/Entities/Path)
#include	INC_3DENGINE(M3D)

#ifndef _DEBUG
	#include	"Inline.h"
	#include	INL_3DENGINE(SceneGraph/Entities/Quadtree)

	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Circle)

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

// Communication
SINGLETON_DESCRIPTOR	 PATH_FINDER::_SD("PATH_FINDER",
										  MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PATH_FINDER, 
										  "7F5DC593-058C-7DCE-1C83-19EFC50849A9",
										  sizeof(PATH_FINDER), 
										  COM_Init,
										  "The path finder singleton");

// For singleton
PATH_FINDER*	PATH_FINDER::_pInstance; 

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	01-10-03:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FINDER *	PATH_FINDER::Instance()
{
	return (static_cast<PATH_FINDER *>(_SD.Instance()));
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	01-10-03:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::Suicide()
{
	_SD.Release();
}

//-----------------------------------------------------------------------------
//	Name:		PATH_FINDER constructor
//	Object:		
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FINDER::PATH_FINDER()
{
	_pInstance = this;
}

//-----------------------------------------------------------------------------
//	Name:		~PATH_FINDER destructor
//	Object:		
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FINDER::~PATH_FINDER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetQuadtree
//	Object:		
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::PushQuadtree(const COM_INSTANCE & QuadtreeInst)
{
	_vQuadtreeInst.PushTail(QuadtreeInst);
}

//-----------------------------------------------------------------------------
//	Name:		ClearQuadtrees
//	Object:		
//	03-02-25:	FDE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::ClearQuadtrees()
{
	_vQuadtreeInst.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		FindPath
//	Object:		"A Star" algorithm	
//	01-09-19:	RMA - Created
//-----------------------------------------------------------------------------
bool	PATH_FINDER::FindPath(const POINT2D	&	Src,
							  const POINT2D	&	Dst,
							  float				rRadius,
							  PATH &			Path)
{
	bool bRes;

	if (_vQuadtreeInst.IsEmpty()) return (false);
	if (Src == Dst)		return (false);

	// Reset path
	Path.ClearWayPoints();
	Path.SetRadiusUnit(rRadius);

	QUADTREE *pQ = static_cast<QUADTREE *>(_vQuadtreeInst[0].GetCommunicator());
	
	ASTAR	FindPath;

	FindPath.SetQuadtree(pQ);
	FindPath.SetSource(Src);
	FindPath.SetDestination(Dst);
	FindPath.SetRadius(rRadius);
	FindPath.SetPath(&Path);

	bRes = FindPath.Search();	

	Path.Smooth(pQ);

	return (bRes);
}

//-----------------------------------------------------------------------------
//	Name:		CheckUnitPos
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	PATH_FINDER::CheckUnitPos(const POINT2D &	Pos,
								  float				rRadiusUnit) const
{
	unsigned int i, nCount = _vQuadtreeInst.GetSize();
	for (i = 0; i < nCount ; ++i)
	{
		QUADTREE *pQ = static_cast<QUADTREE *>(_vQuadtreeInst[i].GetCommunicator());
		CIRCLE	  Circle(Pos, rRadiusUnit);
	
		if (!pQ->IntersectWithCircle(Circle))
		{
			return (true);
		}
	}

	return (false);
}		
/*
//-----------------------------------------------------------------------------
//	Name:		AddUnit
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::AddUnit(INDEX_INT *	pPUIndex)
{
	_vUnits.PushTail(pPUIndex);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveUnit
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	PATH_FINDER::RemoveUnit(INDEX_INT *	pPUIndex)
{
	QDT_DLIST <INDEX_INT*>::ITERATOR itEach = _vUnits.GetHead();
	QDT_DLIST <INDEX_INT*>::ITERATOR itTail = _vUnits.GetTail();

	while (itEach != itTail)
	{
		if (pPUIndex == *itEach)
		{
			_vUnits.Remove(itEach);
			return (true);
		}
	}

	return (false);
}
*/
//-----------------------------------------------------------------------------
//	Name:		COM_Init
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::COM_Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = new	METHOD_DESCRIPTOR(QDT_COMMENT("PushQuadtree"), QDT_COMMENT("Set the quadtree used for path finding."));
	pMD->AddParameter(QDT_COMMENT("Quadtree"), QDT_COMMENT(""), QUADTREE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PUSH_QUADTREE, pMD, EM_PushQuadtree);

	pMD = new	METHOD_DESCRIPTOR(QDT_COMMENT("FindPath"), QDT_COMMENT("Find a path from source to destination."));
	pMD->AddParameter(QDT_COMMENT("Source"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("Destination"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUnitRadius"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameterReference(QDT_COMMENT("Path"), QDT_COMMENT(""), PATH::GetGenericCommunicatorDescriptor());
	pMD->SetReturn(QDT_COMMENT("bSuccess"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_FIND_PATH, pMD, EM_FindPath);

	pMD = new	METHOD_DESCRIPTOR(QDT_COMMENT("CheckUnitPos"), QDT_COMMENT("Check if the given position is valid."));
	pMD->AddParameter(QDT_COMMENT("UnitPos"), QDT_COMMENT(""), TYPE_POINT_2D::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rUnitRadius"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->SetReturn(QDT_COMMENT("bRes"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_CHECK_UNIT_POS, pMD, EM_CheckUnitPos);

	//	Now register the class as a global variable
	GLOBAL_VARIABLE::AddVariable(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PATH_FINDER, "PATH_FINDER", pCD, _SD.GetSingletonPointer());
}

//-----------------------------------------------------------------------------
//	Name:		GetSingletonDescriptor
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	PATH_FINDER::GetSingletonDescriptor()
{
	return	(&_SD);
}
					
//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	PATH_FINDER *pPF = static_cast<PATH_FINDER *>(pO);

	QDT_NEW_PLACED(pPF, PATH_FINDER)();
	QDT_ASSERT(pPF);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	PATH_FINDER *pPF = static_cast<PATH_FINDER *>(pO);

	QDT_DELETE_PLACED(pPF, PATH_FINDER);
}
					
//-----------------------------------------------------------------------------
//	Name:		EM_PushQuadtree
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::EM_PushQuadtree(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE	*pQuadtreeInst = static_cast<COM_INSTANCE *>(pP);

	if (pQuadtreeInst->GetType() != MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_QUADTREE)
	{
		QDT_Message("Parameter 0 of m3d_path_finder:setQuadtree() is not of the right type.");
	}

	PATH_FINDER::Instance()->PushQuadtree(*pQuadtreeInst);
}

//-----------------------------------------------------------------------------
//	Name:		EM_FindPath
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::EM_FindPath(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	struct	PARAMETERS
	{
		POINT2D			_Souce;
		POINT2D			_Destination;
		float			_rUnitRadius;
		COM_INSTANCE	*_pPathInst;
	};

	PARAMETERS	*pParameters = static_cast<PARAMETERS *>(pP);
	bool		*pReturn = static_cast<bool *>(pR);
	
	if (pParameters->_pPathInst->GetType() != MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PATH)
	{
		QDT_Message("Parameter 4 of m3d_path_finder:FindPath() is not of the right type.");
	}

	PATH * pPath = static_cast<PATH *>(pParameters->_pPathInst->GetCommunicator());
	*pReturn = PATH_FINDER::Instance()->FindPath(pParameters->_Souce, pParameters->_Destination, pParameters->_rUnitRadius, *pPath);
}

//-----------------------------------------------------------------------------
//	Name:		EM_CheckUnitPos
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PATH_FINDER::EM_CheckUnitPos(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	struct	PARAMETERS
	{
		POINT2D	_UnitPos;
		float	_rUnitRadius;
	};

	PARAMETERS	*pParameters = static_cast<PARAMETERS *>(pP);
	bool		*pReturn = static_cast<bool *>(pR);

	*pReturn = PATH_FINDER::Instance()->CheckUnitPos(pParameters->_UnitPos, pParameters->_rUnitRadius);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================