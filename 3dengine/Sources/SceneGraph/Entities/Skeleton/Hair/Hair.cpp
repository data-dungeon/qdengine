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
//	CLASS:	HAIR
//
//	05-12-02:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Hair/Hair)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Hair/HairCell)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)

#ifdef _DEBUG
#include	"Hair.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		HAIR constructor
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
HAIR::HAIR()
:
_pFirstRoot				(NULL),
_pHeadNodeChild			(NULL),
_uiLockedLevelsLimit	(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		HAIR destructor
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
HAIR::~HAIR()
{
	delete	(_pFirstRoot);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-12-05:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::Init(I_PHYSIC_SCENE *	pScene)
{
	if	(_pFirstRoot)
	{
		_pFirstRoot->Init(_pHeadNodeChild, pScene, _uiLockedLevelsLimit);
	}

//	InitSideConstraints();
}

//-----------------------------------------------------------------------------
//	Name:		EnableKinematic
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::EnableKinematic(Bool	b)
{
	if	(_pFirstRoot)
	{
		_pFirstRoot->EnableKinematic(b, _uiLockedLevelsLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetLastLockedLevel
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetLastLockedLevel(UInt32	nLevel)
{
	_uiLockedLevelsLimit = nLevel;

	if	(_pFirstRoot)
	{
//		_pFirstRoot->SetLastLockedLevel(nLevel);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetMass(Float32	rMaxMass,
					  Float32	rMinMass,
					  Int32		uiLevelLimit)
{
	Float32	rStep = 0.0f;
	
	if	(uiLevelLimit < 0)
	{
		rStep = (rMaxMass - rMinMass) / _HairCellsByLevel.GetSize();
	}

	if	(_pFirstRoot)
	{
		_pFirstRoot->SetMass(rMaxMass, rMinMass, rStep, uiLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDamping
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetDamping(Float32	rMaxLinearDamping,
						 Float32	rMinLinearDamping,
						 Float32	rMaxAngularDamping,
						 Float32	rMinAngularDamping,
						 Int32		uiLevelLimit)
{
	Float32	rLinearDampingStep = 0.0f;
	Float32	rAngularDampingStep = 0.0f;

	if	(uiLevelLimit < 0)
	{
		UInt32	nMaxLevel = _HairCellsByLevel.GetSize();

		rLinearDampingStep = (rMaxLinearDamping - rMinLinearDamping) / nMaxLevel;
		rAngularDampingStep = (rMaxAngularDamping - rMinAngularDamping) / nMaxLevel;
	}

	if	(_pFirstRoot)
	{
		_pFirstRoot->SetDamping(rMaxLinearDamping, rMinLinearDamping, rLinearDampingStep,
								rMaxAngularDamping, rMinAngularDamping, rAngularDampingStep,
								uiLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingLimit
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetSwingLimit(Float32	rMaxSwingLimit,
							Float32	rMinSwingLimit,
							Int32	uiLevelLimit)
{
	Float32	rSwingLimitStep = 0.0f;

	if	(uiLevelLimit < 0)
	{
		rSwingLimitStep = (rMaxSwingLimit - rMinSwingLimit) / _HairCellsByLevel.GetSize();
	}

	if	(_pFirstRoot)
	{
		_pFirstRoot->SetSwingLimit(rMaxSwingLimit, rMinSwingLimit, rSwingLimitStep, uiLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSpring
//	Object:		
//	05-12-14:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetSpring(Float32	rMaxSpring,
						Float32	rMinSpring,
						Float32	rMaxDamping,
						Float32	rMinDamping,
						Int32	uiLevelLimit)
{
	Float32	rSpringStep = 0.0f;
	Float32	rDampingStep = 0.0f;

	if	(uiLevelLimit < 0)
	{
		UInt32	nMaxLevel = _HairCellsByLevel.GetSize();

		rSpringStep = (rMaxSpring - rMinSpring) / nMaxLevel;
		rDampingStep = (rMaxDamping - rMinDamping) / nMaxLevel;
	}

	if	(_pFirstRoot)
	{
		_pFirstRoot->SetSpring(rMaxSpring, rMinSpring, rSpringStep,
							   rMaxDamping, rMinDamping, rDampingStep,
							   uiLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBounciness
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetBounciness(Float32	rMaxBounciness,
							Float32	rMinBounciness,
							Int32	uiLevelLimit)
{
	Float32	rBouncinessStep = 0.0f;

	if	(uiLevelLimit < 0)
	{
		rBouncinessStep = (rMaxBounciness - rMinBounciness) / _HairCellsByLevel.GetSize();
	}

	if	(_pFirstRoot)
	{
		_pFirstRoot->SetBounciness(rMaxBounciness, rMinBounciness, rBouncinessStep, uiLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFriction
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetFriction(Float32	rMaxFriction,
						  Float32	rMinFriction,
						  Int32		uiLevelLimit)
{
	Float32	rFrictionStep = 0.0f;

	if	(uiLevelLimit < 0)
	{
		rFrictionStep = (rMaxFriction - rMinFriction) / _HairCellsByLevel.GetSize();
	}

	if	(_pFirstRoot)
	{
		_pFirstRoot->SetFriction(rMaxFriction, rMinFriction, rFrictionStep, uiLevelLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetMassByLevel
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetMassByLevel(UInt32		nLevel,
							 Float32	rMass)
{
	UInt32	nMaxLevel = _HairCellsByLevel.GetSize();
	if	(nLevel < nMaxLevel)
	{
		HAIR_CELL * pCell = _HairCellsByLevel[nLevel];

		while	(pCell)
		{
			pCell->SetMass(rMass);
			pCell = pCell->GetNextSameLevel();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDampingByLevel
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetDampingByLevel(UInt32	nLevel,
								Float32	rLinearDamping,
								Float32	rAngularDamping)
{
	UInt32	nMaxLevel = _HairCellsByLevel.GetSize();
	if	(nLevel < nMaxLevel)
	{
		HAIR_CELL * pCell = _HairCellsByLevel[nLevel];

		while	(pCell)
		{
			pCell->SetDamping(rLinearDamping, rAngularDamping);
			pCell = pCell->GetNextSameLevel();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingLimitByLevel
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetSwingLimitByLevel(UInt32	nLevel,
								   Float32	rSwingLimit)
{
	UInt32	nMaxLevel = _HairCellsByLevel.GetSize();
	if	(nLevel < nMaxLevel)
	{
		HAIR_CELL * pCell = _HairCellsByLevel[nLevel];

		while	(pCell)
		{
			pCell->SetSwingLimit(rSwingLimit);
			pCell = pCell->GetNextSameLevel();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSpringByLevel
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetSpringByLevel(UInt32	nLevel,
							   Float32	rSpring,
							   Float32	rDamping)
{
	UInt32	nMaxLevel = _HairCellsByLevel.GetSize();
	if	(nLevel < nMaxLevel)
	{
		HAIR_CELL * pCell = _HairCellsByLevel[nLevel];

		while	(pCell)
		{
			pCell->SetSpring(rSpring, rDamping);
			pCell = pCell->GetNextSameLevel();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBouncinessByLevel
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetBouncinessByLevel(UInt32	nLevel,
								   Float32	rBounciness)
{
	UInt32	nMaxLevel = _HairCellsByLevel.GetSize();
	if	(nLevel < nMaxLevel)
	{
		HAIR_CELL * pCell = _HairCellsByLevel[nLevel];

		while	(pCell)
		{
			pCell->SetBounciness(rBounciness);
			pCell = pCell->GetNextSameLevel();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFrictionByLevel
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::SetFrictionByLevel(UInt32		nLevel,
								 Float32	rFriction)
{
	UInt32	nMaxLevel = _HairCellsByLevel.GetSize();
	if	(nLevel < nMaxLevel)
	{
		HAIR_CELL * pCell = _HairCellsByLevel[nLevel];

		while	(pCell)
		{
			pCell->SetFriction(rFriction);
			pCell = pCell->GetNextSameLevel();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddCell
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::AddCell(NODE_CHILD *	pNodeChild)
{
	QDT_ASSERT(pNodeChild);
	QDT_ASSERT(pNodeChild->GetType() == JT_HAIR);

	HAIR_CELL *	pNewCell = new HAIR_CELL(pNodeChild);
	NODE_CHILD * pFather = pNodeChild->GetNodeArray()->GetChild(pNodeChild->GetFatherIndex());

	pNodeChild->SetHairCell(pNewCell);

	if	(pFather->GetType() == JT_HEAD)
	{
		pNewCell->SetLevel(0);

		if	(_pFirstRoot)
		{
			HAIR_CELL *	pCell = _pFirstRoot;
			while	(pCell->GetBrother() != NULL)
			{
				pCell = pCell->GetBrother();
			}

			pCell->SetBrother(pNewCell);
		}
		else
		{
			_pFirstRoot = pNewCell;
		}
	}
	else
	{
		pFather->GetHairCell()->AddChild(pNewCell);
	}

	Int32	nNbrLevels = _HairCellsByLevel.GetSize();
	Int32	nLevel = pNewCell->GetLevel();

	if	(nLevel > (nNbrLevels - 1))
	{
		_HairCellsByLevel.PushTail(pNewCell);
	}
	else
	{
		HAIR_CELL *	pCell = _HairCellsByLevel[nLevel];
		while	(pCell->GetNextSameLevel() != NULL)
		{
			pCell = pCell->GetNextSameLevel();
		}

		pCell->SetNextSameLevel(pNewCell);
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitSideConstraints
//	Object:		
//	06-03-06:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR::InitSideConstraints()
{
	UInt32 i, nNbrLevels = _HairCellsByLevel.GetSize();

	for	(i = 0 ; i < nNbrLevels ; ++i)
	{
		HAIR_CELL *	pPrevLevel = _HairCellsByLevel[i];
		HAIR_CELL * pLevel = pPrevLevel->GetNextSameLevel();

		while	(pLevel != NULL)
		{
			if	((pLevel->GetFirstChild() == NULL) || (pPrevLevel->GetFirstChild() == NULL))
			{
				HAIR_CELL * pPrevFather = pPrevLevel->GetFather();
				HAIR_CELL * pFather = pLevel->GetFather();

				while	(pPrevFather != pFather)
				{
					pPrevFather = pPrevFather->GetFather();
					pFather = pFather->GetFather();
				}

				if	(pFather != NULL)
				{
					pLevel->CreateSideConstraint(pPrevLevel);
				}
			}

			pPrevLevel = pLevel;
			pLevel = pLevel->GetNextSameLevel();
		}

		pLevel = _HairCellsByLevel[i];
		if	(pPrevLevel != pLevel)
		{
			if	((pLevel->GetFirstChild() == NULL) || (pPrevLevel->GetFirstChild() == NULL))
			{
				HAIR_CELL * pPrevFather = pPrevLevel->GetFather();
				HAIR_CELL * pFather = pLevel->GetFather();

				while	(pPrevFather != pFather)
				{
					pPrevFather = pPrevFather->GetFather();
					pFather = pFather->GetFather();
				}

				if	(pFather != NULL)
				{
					pLevel->CreateSideConstraint(pPrevLevel);
				}
			}
		}
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
