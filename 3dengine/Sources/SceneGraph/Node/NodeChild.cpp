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
//	CLASS:	NODE_CHILD
//
//	06-01-13:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)

#include	INCL_KCORE(Stream/FileStream)
#include	INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)
#include	INCL_KPHYSICS(Physics/PhysicContext)

#ifdef _DEBUG
#include "NodeChild.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		NODE_CHILD constructor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_CHILD::NODE_CHILD()
:
_nType				(JT_NONE),
_LMatrix			(MATRIX::IDENTITY),
_AnimMatrix			(MATRIX::IDENTITY),
_GMatrix			(MATRIX::IDENTITY),
_iIndex				(-1),
_iFatherIndex		(-1),
_uiNbrChildren		(0),
_pChildren			(NULL),
_pNodeArray			(NULL),
_pDynamicActor		(NULL),
_pPhysicMaterial	(NULL),
_bKinematic			(true),
_pHairCell			(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		NODE_CHILD destructor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_CHILD::~NODE_CHILD()
{
	if(_pChildren)
	{
		delete [] _pChildren;
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_CHILD &	NODE_CHILD::operator=(const NODE_CHILD &	N)
{
	if	(this != &N)
	{
		Copy(N);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::Copy(const NODE_CHILD &	N)
{
	_nType = N._nType;
	_LMatrix = N._LMatrix;
	_AnimMatrix = N._AnimMatrix;
	_GMatrix = N._GMatrix;
	_iIndex = N._iIndex;
	_iFatherIndex = N._iFatherIndex;
	_uiNbrChildren = N._uiNbrChildren;

	if	(_uiNbrChildren)
	{
		UInt32	i;

		_pChildren = new UInt32[_uiNbrChildren];
		for	(i = 0 ; i < _uiNbrChildren ; ++i)
		{
			_pChildren[i] = N._pChildren[i];
		}
	}
	
	_pNodeArray = N._pNodeArray;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAnimMatrix
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::UpdateAnimMatrix(QUATERNION_VECTOR *	pQV)
{
	_AnimMatrix.SetQuaternion(pQV->_Quat);
	_AnimMatrix.SetTranslation(pQV->_Vect);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateGlobalMatrix
//	Object:		
//	06-01-13:	ELE - Created
//  06-02-07:	RMA - Modified : Father matrix can be the same as _GMatrix
//				This occurs an assert in the multiplication that I've
//				removed with TempMat2
//-----------------------------------------------------------------------------
void	NODE_CHILD::UpdateGlobalMatrix(const MATRIX &	FatherMatrix)
{
	if	(_pDynamicActor)
	{
		if	(_pDynamicActor->IsKinematicModeEnabled())
		{
			MATRIX	TempMat;

			TempMat.Mul(_AnimMatrix, _LMatrix);
			_GMatrix.Mul(TempMat, FatherMatrix);
		}
	}
	else
	{
		if	(_bKinematic == false)
		{
			_GMatrix.Mul(_LMatrix, FatherMatrix);
		}
		else
		{
			MATRIX	TempMat;

			TempMat.Mul(_AnimMatrix, _LMatrix);
			_GMatrix.Mul(TempMat, FatherMatrix);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreatePhysicDynamicActor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_DYNAMIC_ACTOR *	NODE_CHILD::CreatePhysicDynamicActor()
{
	_pDynamicActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();
	_pDynamicActor->SetUserData(I_PHYSIC_ACTOR::DATA_TYPE_NODECHILD, this);

	return (_pDynamicActor);
}

//-----------------------------------------------------------------------------
//	Name:		DeletePhysicDynamicActor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::DeletePhysicDynamicActor()
{
	PHYSIC_CONTEXT::Instance()->ReleaseActor(_pDynamicActor);
}

//-----------------------------------------------------------------------------
//	Name:		CreatePhysicMaterial
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_MATERIAL *	NODE_CHILD::CreatePhysicMaterial()
{
	_pPhysicMaterial = PHYSIC_CONTEXT::Instance()->CreateMaterial();

	return	(_pPhysicMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		DeletePhysicMaterial
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::DeletePhysicMaterial()
{
	PHYSIC_CONTEXT::Instance()->ReleaseMaterial(_pPhysicMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		EnableKinematic
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::EnableKinematic(Bool	b)
{
	if	(_pDynamicActor)
	{
		_pDynamicActor->EnableKinematicMode(b);
	}
	_bKinematic = b;
}

//-----------------------------------------------------------------------------
//	Name:		EnablePhysic
//	Object:		
//	06-03-21:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::EnablePhysic(Bool	b)
{
	if	(_pDynamicActor)
	{
		_pDynamicActor->EnableKinematicMode(!b);
	}
	_bKinematic = !b;

	UInt32	i;
	for	(i = 0 ; i < _uiNbrChildren ; ++i)
	{
		_pNodeArray->GetChild(_pChildren[i])->EnablePhysic(b);
	}
}

//-----------------------------------------------------------------------------
//	Name:		MoveActor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::MoveActor()
{
	if	(_pDynamicActor && _pDynamicActor->IsKinematicModeEnabled())
	{
		_pDynamicActor->MoveGlobalPosition(_GMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RetrieveActorPos
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::RetrieveActorPos(const MATRIX & FatherMatrix)
{
	if	(_pDynamicActor && (_pDynamicActor->IsKinematicModeEnabled() == false))
	{
		_pDynamicActor->GetGlobalMatrix(_GMatrix);
	}
	else
	{
		if	(_bKinematic == false)
		{
			_GMatrix.Mul(_LMatrix, FatherMatrix);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::Read(IO_STREAM &	Stream)
{
	int	nType;
	Stream >> nType;
	_nType = (JOINT_TYPE)nType;
	
	Stream >> _LMatrix;

	Stream >> _iIndex;
	Stream >> _iFatherIndex;
	Stream >> _uiNbrChildren;

	if	(_uiNbrChildren)
	{
		UInt32	i;

		_pChildren = new UInt32[_uiNbrChildren];
		for	(i = 0 ; i < _uiNbrChildren ; ++i)
		{
			Stream >> _pChildren[i];
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::Write(IO_STREAM &	Stream)
{
	Stream << _nType;
	
	Stream << _LMatrix;

	Stream << _iIndex;
	Stream << _iFatherIndex;
	Stream << _uiNbrChildren;

	UInt32	i;
	for	(i = 0 ; i < _uiNbrChildren ; ++i)
	{
		Stream << _pChildren[i];
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
