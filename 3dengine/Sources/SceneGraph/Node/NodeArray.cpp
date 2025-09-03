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
//	CLASS:	NODE_ARRAY
//
//	06-01-13:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)

#include	INCL_KCORE(Stream/FileStream)
#include	INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)
#include	INCL_3DENGINE(Behavior/Controllers/Container/Container)

#ifdef _DEBUG
#include "NodeArray.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		NODE_ARRAY constructor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_ARRAY::NODE_ARRAY()
:
NODE			(GetGenericCommunicatorDescriptor()),
_uiNbrChildren	(0),
_pChild			(NULL)
{}

//-----------------------------------------------------------------------------
//	Name:		NODE_ARRAY destructor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_ARRAY::~NODE_ARRAY()
{
	delete [] _pChild;
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::Instanciate(NODE_ARRAY *	pNewNodeArray)
{
	NODE::Instanciate(pNewNodeArray);

	pNewNodeArray->_uiNbrChildren = _uiNbrChildren;

	if	(_uiNbrChildren)
	{
		UInt32	i;

		pNewNodeArray->_pChild = new NODE_CHILD[_uiNbrChildren];
		for	(i = 0 ; i < _uiNbrChildren ; ++i)
		{
			pNewNodeArray->_pChild[i] = _pChild[i];
			pNewNodeArray->_pChild[i].SetNodeArray(pNewNodeArray);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::InstanciateRec(NODE_ARRAY *	pNewNodeArray)
{
	NODE::InstanciateRec(pNewNodeArray);

	pNewNodeArray->_uiNbrChildren = _uiNbrChildren;

	if	(_uiNbrChildren)
	{
		UInt32	i;

		pNewNodeArray->_pChild = new NODE_CHILD[_uiNbrChildren];
		for	(i = 0 ; i < _uiNbrChildren ; ++i)
		{
			pNewNodeArray->_pChild[i] = _pChild[i];
			pNewNodeArray->_pChild[i].SetNodeArray(pNewNodeArray);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPotentialNbrTracks
//	Object:		
//	06-01-17:	ELE - Created
//-----------------------------------------------------------------------------
int	NODE_ARRAY::GetPotentialNbrTracks() const
{
	//	number of children + this
	return	(_uiNbrChildren + 1);
}

//-----------------------------------------------------------------------------
//	Name:		GetIndexedGlobalMatrix
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE_ARRAY::GetIndexedGlobalMatrix(int	nNum) const
{
	if	(nNum < 0)
	{
		return	(_GMatrix);
	}

	return	(_pChild[nNum].GetGlobalMatrix());
}

//-----------------------------------------------------------------------------
//	Name:		GetIndexedContainerMatrix
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE_ARRAY::GetIndexedContainerMatrix(int	nNum) const
{
	if	(nNum < 0)
	{
		return	(_GMatrix);
	}

	return	(_pChild[nNum].GetAnimMatrix());
}

//-----------------------------------------------------------------------------
//	Name:		GetChildLocalPos
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::GetChildLocalPos(int		nNumChild,
									 VECTOR &	OutPos) const
{
	OutPos = _pChild[nNumChild].GetLocalMatrix().GetTranslation();
}

//-----------------------------------------------------------------------------
//	Name:		SetChildLocalPos
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::SetChildLocalPos(int			nNumChild,
									 const VECTOR &	Pos)
{
	_pChild[nNumChild].GetLocalMatrix().SetPosition(Pos);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildGlobalPos
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::GetChildGlobalPos(int		nNumChild,
									  VECTOR &	OutPos) const
{
	OutPos =_pChild[nNumChild].GetGlobalMatrix().GetTranslation();
}

//-----------------------------------------------------------------------------
//	Name:		GetChildAnimPos
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::GetChildAnimPos(int			nNumChild,
									VECTOR &	OutPos) const
{
	OutPos =_pChild[nNumChild].GetAnimMatrix().GetTranslation();
}

//-----------------------------------------------------------------------------
//	Name:		SetChildAnimPos
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::SetChildAnimPos(int				nNumChild,
									const VECTOR &	Pos)
{
	_pChild[nNumChild].GetAnimMatrix().SetPosition(Pos);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildLocalRot
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::GetChildLocalRot(int			nNumChild,
									 QUATERNION &	OutQuat) const
{
	_pChild[nNumChild].GetLocalMatrix().GetQuaternion(OutQuat);
}

//-----------------------------------------------------------------------------
//	Name:		SetChildLocalRot
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::SetChildLocalRot(int				nNumChild,
									 const QUATERNION &	Quat)
{
	_pChild[nNumChild].GetLocalMatrix().SetQuaternion(Quat);
	UpdateAllMatrices(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildGlobalRot
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::GetChildGlobalRot(int			nNumChild,
									  QUATERNION &	OutQuat) const
{
	_pChild[nNumChild].GetGlobalMatrix().GetQuaternion(OutQuat);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildAnimRot
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::GetChildAnimRot(int				nNumChild,
									QUATERNION &	OutQuat) const
{
	_pChild[nNumChild].GetAnimMatrix().GetQuaternion(OutQuat);
}

//-----------------------------------------------------------------------------
//	Name:		SetChildAnimRot
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::SetChildAnimRot(int					nNumChild,
									const QUATERNION &	Quat)
{
	_pChild[nNumChild].GetAnimMatrix().SetQuaternion(Quat);
	UpdateAllMatrices(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateContainer
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::UpdateContainer(float rSpeedFactor)
{
	NODE::UpdateContainer(rSpeedFactor);

	if (_pContainer)
	{
		UpdateAnimMatrices();
	}

	UpdateAllMatrices(GetGlobalMatrix());

	if	(_FirstChildInst.HasHandle())
	{
		NODE_ARRAY *	pNodeArray;
		NODE *			pChild = GetFirstChild();

		while	(pChild)
		{
			pChild->UpdateGlobalMatrix();

			if	(pChild->GetHandle()->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
			{
				pNodeArray = static_cast<NODE_ARRAY *>(pChild);
				pNodeArray->UpdateAllMatrices(pChild->GetGlobalMatrix());
			}

			pChild = pChild->GetBrother();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAnimMatrices
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::UpdateAnimMatrices(QUATERNION_VECTOR *	pQV)
{
	
	UInt32	i;

	for (i = 0 ; i < _uiNbrChildren ; ++i)
	{
		_pChild[i].UpdateAnimMatrix(pQV);
		pQV++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAnimMatrices
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::UpdateAnimMatrices()
{
	QUATERNION_VECTOR *	pQV;

	pQV = _pContainer->GetStrAnim()->_pChildrenOutput;
	if	(pQV)
	{
		UpdateAnimMatrices(pQV);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAllMatrices
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::UpdateAllMatrices(const MATRIX &	RootMatrix)
{
	UInt32			i;
	const MATRIX *	pFatherMatrix;

	for (i = 0 ; i < _uiNbrChildren ; ++i)
	{
		Int32	iFatherIndex = _pChild[i].GetFatherIndex();

		QDT_ASSERT(iFatherIndex < (Int32)_uiNbrChildren);
		QDT_ASSERT(iFatherIndex != i);

		if	(iFatherIndex == -1)
		{
			pFatherMatrix = &RootMatrix;
		}
		else
		{
			pFatherMatrix = &_pChild[iFatherIndex].GetGlobalMatrix();
		}

		_pChild[i].UpdateGlobalMatrix(*pFatherMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateGlobalMatrix
//	Object:		
//	06-03-21:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::UpdateGlobalMatrix()
{
	NODE::UpdateGlobalMatrix();
	UpdateAllMatrices(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateLocalMatrix
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::UpdateLocalMatrix()
{
	NODE::UpdateLocalMatrix();
	UpdateAllMatrices(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		ForceUpdate
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::ForceUpdate(float	rSpeedFactor)
{
	NODE::ForceUpdate(rSpeedFactor);
	UpdateAllMatrices(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		ResetChildren
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::ResetChildren()
{
	UInt32		i;
	QUATERNION	Q(false);

	Q.Id();
	for (i = 0 ; i < _uiNbrChildren ; ++i)
	{
		_pChild[i].GetAnimMatrix().SetQuaternion(Q);
	}
	UpdateAllMatrices(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		EnableKinematic
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::EnableKinematic(Bool	b)
{
	UInt32	i;

	for (i = 0 ; i < _uiNbrChildren ; ++i)
	{
		_pChild[i].EnableKinematic(b);
	}
}

//-----------------------------------------------------------------------------
//	Name:		MoveActors
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::MoveActors()
{
	UInt32	i;

	for (i = 0 ; i < _uiNbrChildren ; ++i)
	{
		_pChild[i].MoveActor();
	}
}

//-----------------------------------------------------------------------------
//	Name:		RetrieveActorsPos
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::RetrieveActorsPos()
{
	UInt32			i;
	const MATRIX *	pFatherMatrix;

	for (i = 0 ; i < _uiNbrChildren ; ++i)
	{
		Int32	iFatherIndex = _pChild[i].GetFatherIndex();

		QDT_ASSERT(iFatherIndex < (Int32)_uiNbrChildren);
		QDT_ASSERT(iFatherIndex != i);

		if	(iFatherIndex == -1)
		{
			pFatherMatrix = &_GMatrix;
		}
		else
		{
			pFatherMatrix = &_pChild[iFatherIndex].GetGlobalMatrix();
		}

		_pChild[i].RetrieveActorPos(*pFatherMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnablePhysic
//	Object:		
//	06-03-21:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::EnablePhysic(UInt32	nJointIndex,
								 bool	b)
{
	if	(nJointIndex < _uiNbrChildren)
	{
		_pChild[nJointIndex].EnablePhysic(b);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("NODEAR  ", 1 ,CM_ReadChunk);
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("NODEAR  ", 1 ,CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NODE_ARRAY::GetGenericCommunicatorDescriptor()
{
	return	(COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NODE_ARRAY::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("NODE_ARRAY"),								
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY,			
										QDT_COMMENT("FF8FABBB-1ED4-CEB5-538D-F792A3DF45E3"),			
										sizeof(NODE_ARRAY),										
										ComInit,											
										QDT_COMMENT("Class for node_array."),
										NODE::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	NODE_ARRAY *	pNodeArray;
	pNodeArray = static_cast<NODE_ARRAY *>(pO);

	QDT_NEW_PLACED(pNodeArray, NODE_ARRAY)();
	QDT_ASSERT(pNodeArray);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	NODE_ARRAY *	pNodeArray;
	pNodeArray = static_cast<NODE_ARRAY *>(pO);

	QDT_DELETE_PLACED(pNodeArray, NODE_ARRAY);
	QDT_ASSERT(pNodeArray);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_ARRAY::CM_ReadChunk(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	NODE_ARRAY *	pNodeArray;
	pNodeArray = static_cast<NODE_ARRAY *>(pCommunicator);

	Stream >> pNodeArray->_uiNbrChildren;

	if	(pNodeArray->_uiNbrChildren)
	{
		UInt32	i;

		pNodeArray->_pChild = new NODE_CHILD[pNodeArray->_uiNbrChildren];
		for	(i = 0 ; i < pNodeArray->_uiNbrChildren ; ++i)
		{
			pNodeArray->_pChild[i].Read(Stream);
			pNodeArray->_pChild[i].SetNodeArray(pNodeArray);

			// Additional verifications
			QDT_ASSERT(pNodeArray->_pChild[i].GetFatherIndex() != i);
			QDT_ASSERT(pNodeArray->_pChild[i].GetFatherIndex() < (Int32)pNodeArray->_uiNbrChildren);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE_ARRAY::CM_WriteChunk(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	NODE_ARRAY *	pNodeArray;
	pNodeArray = static_cast<NODE_ARRAY *>(pCommunicator);

	Stream.DebugOutput("Node array :\t");
	Stream.DebugOutput(pNodeArray->GetName());
	Stream.DebugOutput("\r\n");

	Stream << pNodeArray->_uiNbrChildren;

	UInt32	i;
	for	(i = 0 ; i < pNodeArray->_uiNbrChildren ; ++i)
	{
		pNodeArray->_pChild[i].Write(Stream);
	}

	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

