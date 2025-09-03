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
// FILE: Node.inl
//
//
//	01-02-02:	RMA - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name: NODE::GetFlag
//	Object: Get the flags mask of the node
//	01-02-05:	RMA - Created
//-----------------------------------------------------------------------------
const QDT_FLAGS & NODE::GetFlags() const
{
	return (_Flags);
}

//-----------------------------------------------------------------------------
//	Name: NODE::GetFlag
//	Object: Get the flags mask of the node
//	01-02-05:	RMA - Created
//-----------------------------------------------------------------------------
QDT_FLAGS & NODE::GetFlags()
{
	return (_Flags);
}

//-----------------------------------------------------------------------------
//	Name: NODE::GetFlag
//	Object: 
//	05-01-27:	SBE - Created
//-----------------------------------------------------------------------------
int		NODE::GetFlags(int nFlagsMask) const
{
	return (_Flags.AsInt() & nFlagsMask);
}

//-----------------------------------------------------------------------------
//	Name: NODE::SetFlags
//	Object: 
//	05-01-27:	SBE - Created
//-----------------------------------------------------------------------------
void	NODE::SetFlags(int nFlagsMask, int nFlagsSet)
{
	_Flags.SetFlags(nFlagsMask, nFlagsSet);
}

//-----------------------------------------------------------------------------
// Name:		NODE::SetNodeContainer
// Object:		Set the node container
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
void NODE::SetContainer(CONTAINER* const pContainer)
{
	_pContainer = pContainer;

	if	(_pContainer)
	{
		if	(GetParentNode())
		{
			GetParentNode()->SetContainerOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
// Name:		NODE::GetNodeContainer
// Object:		Get the node container
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
CONTAINER* NODE::GetContainer() const
{
	return (_pContainer);
}

//-----------------------------------------------------------------------------
// Name:		NODE::SetLocalPosition
// Object:		Set the local position of the node 
// 01-02-02		RMA Created:
//-----------------------------------------------------------------------------	
void NODE::SetLocalPosition(const VECTOR & vPoint)
{
	_LMatrix.SetPosition(vPoint) ;
	QDT_ASSERT(_LMatrix.IsAffine());
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0) ;
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
// Name:		NODE::GetLocalPosition
// Object:		Get the local position of the node
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
void NODE::GetLocalPosition(VECTOR &outPos) const
{
	outPos = _LMatrix.GetTranslation();
}

//-----------------------------------------------------------------------------
// Name:		NODE::SetLocalRotation
// Object:		Set the local rotation
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
void NODE::SetLocalRotXYZ(const VECTOR & vAngle3D)
{		
	_LMatrix.SetRotXYZ(vAngle3D) ;
	QDT_ASSERT(_LMatrix.IsAffine());
	SetFlagsTree(GLOBAL_MATRIX_VALID , 0);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
// Name:		NODE::SetLocalMatrix
// Object:		Set the local matrix
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
void NODE::SetLocalMatrix(const MATRIX & Mat)
{
	_LMatrix = Mat ;
	QDT_ASSERT(_LMatrix.IsAffine());
	SetFlagsTree(GLOBAL_MATRIX_VALID , 0);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
// Name:		NODE::GetLocalMatrix
// Object:		Get the local matrix
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
const MATRIX & NODE::GetLocalMatrix() const
{
	return (_LMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalMatrix
//	Object:		
//	02-07-26:	RMA - Created
//-----------------------------------------------------------------------------
MATRIX &	NODE::GetLocalMatrix()
{
	return (_LMatrix);
}

//-----------------------------------------------------------------------------
// Name: NODE::GetGlobalPosition
// Object: Get the global position
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	NODE::GetGlobalPosition(VECTOR &outPos)
{
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID))
	{
		UpdateGlobalMatrix();
	}

	outPos = _GMatrix.GetTranslation();
}

//-----------------------------------------------------------------------------
// Name: NODE::SetGlobalRotation
// Object: Set the global rotation
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void NODE::SetGlobalRotXYZ(const VECTOR & vAngle3D)
{
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID))
	{
		UpdateGlobalMatrix() ;
	}

	_GMatrix.SetRotXYZ(vAngle3D);
	UpdateLocalMatrix();
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
// Name: NODE::SetGlobalMatrix
// Object: Set the global matrix
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void NODE::SetGlobalMatrix(const MATRIX & Mat)
{
	_GMatrix = Mat ;

	UpdateLocalMatrix() ;

	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);

	_Flags.Set(GLOBAL_MATRIX_VALID);

	SetGlobalMatrixInvalidOnChild();

	if ((_pDynamicActor) && !GetFlags().IsSet(PHYSIC_CONTROLLER))
	{
		bool b = _pDynamicActor->IsKinematicModeEnabled();
		_pDynamicActor->EnableKinematicMode(true);
		_pDynamicActor->SetGlobalMatrix(GetGlobalMatrix());
		_pDynamicActor->EnableKinematicMode(b);
	}
}

//-----------------------------------------------------------------------------
// Name: NODE::GetGlobalMatrix
// Object: Get the global matrix
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
MATRIX & NODE::GetGlobalMatrix()
{
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID))
	{
		UpdateGlobalMatrix();
	}
	
	return (_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalMatrix
//	Object:		
//	06-01-06:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE::GetGlobalMatrix() const
{
	return (_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		IsRootNode
//	Object:		
//	01-08-24:	GHP - Created
//-----------------------------------------------------------------------------
bool	NODE::IsRootNode() const
{
	return (_ParentInst.HasHandle() == false); 
}

//-----------------------------------------------------------------------------
//	Name:		ActivateBoundingBoxDisplay
//	Object:		
//	02-03-25:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::ActivateBoundingBoxDisplay(bool	b)
{
	_Flags.Set(DISPLAY_BOUNDING_BOX, b);
}

//-----------------------------------------------------------------------------
//	Name:		BoundingBoxDisplayIsActive
//	Object:		
//	02-03-25:	CVI - Created
//-----------------------------------------------------------------------------
bool	NODE::BoundingBoxDisplayIsActive() const
{
	return (_Flags.IsSet(DISPLAY_BOUNDING_BOX));
}

//-----------------------------------------------------------------------------
//	Name:		IncBlocReference
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::IncBlocReference()
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		DecBlocReference
//	Object:		
//	02-06-06:	CVI - Created
//-----------------------------------------------------------------------------
int	NODE::DecBlocReference()
{
	QDT_FAIL();
	return ( 0 );
}

//-----------------------------------------------------------------------------
//	Name:		ResetBlocReference
//	Object:		
//	02-06-10:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::ResetBlocReference()
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		GetBlocReference
//	Object:		
//	02-06-10:	CVI - Created
//-----------------------------------------------------------------------------
int	NODE::GetBlocReference() const
{
	QDT_FAIL();
	return ( 0 );
}

//-----------------------------------------------------------------------------
//	Name:		Replace
//	Object:		
//	02-12-02:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void NODE::Replace(const MATRIX &M)
{
	_GMatrix = M;
	UpdateLocalMatrix();
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetNbrChilds
//	Object:		
//	02-11-29:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	NODE::GetNbrChildren() const
{
	return (_vChildInst.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetChild
//	Object:		
//	02-11-29:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	NODE::GetChildInst(unsigned int i) const
{
	return (_vChildInst[i]);
}
*/

//-----------------------------------------------------------------------------
//	Name:		SetParent
//	Object:		
//	02-11-29:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::SetParentNodeInst(const COM_INSTANCE & ParentInst)
{
	_ParentInst = ParentInst;
}

//-----------------------------------------------------------------------------
//	Name:		NODE::GetParentIndex
//	Object:		Get the parent index
//	01-03-15:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE & NODE::GetParentNodeInst() const
{
	return (_ParentInst);
}

//-----------------------------------------------------------------------------
// Name:		NODE::GetParent
// Object:		Get the parent of the node THIS POINTER SHOULD NOT BE SAVED
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
NODE *	NODE::GetParentNode() const
{
	if (_ParentInst.HasHandle())
	{
		return	(static_cast<NODE *>(_ParentInst.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		SetFirstChildInst
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetFirstChildInst(const COM_INSTANCE &	FirstChildInst)
{
	_FirstChildInst = FirstChildInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstChildInst
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	NODE::GetFirstChildInst() const
{
	return	(_FirstChildInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstChild
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
NODE *	NODE::GetFirstChild() const
{
	if (_FirstChildInst.HasHandle())
	{
		return	(static_cast<NODE *>(_FirstChildInst.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		SetBrotherInst
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetBrotherInst(const COM_INSTANCE &	BrotherInst)
{
	_BrotherInst = BrotherInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetBrotherInst
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	NODE::GetBrotherInst() const
{
	return	(_BrotherInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetBrother
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
NODE *	NODE::GetBrother() const
{
	if (_BrotherInst.HasHandle())
	{
		return	(static_cast<NODE *>(_BrotherInst.GetCommunicator()));
	}

	return	(NULL);
}

/*
//-----------------------------------------------------------------------------
//	Name:		GetChild
//	Object:		
//	02-11-29:	RMA - Created
//-----------------------------------------------------------------------------
NODE *	NODE::GetChild(unsigned int	i) const
{
	return (static_cast<NODE*>(_vChildInst[i].GetCommunicator()));
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetChildrenEntityTypes
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
const QDT_FLAGS &	NODE::GetChildrenEntityTypes() const
{
	return (_ChildrenEntityTypes);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildrenEntityTypes
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
QDT_FLAGS &	NODE::GetChildrenEntityTypes()
{
	return (_ChildrenEntityTypes);
}

//-----------------------------------------------------------------------------
//	Name:		IsSetContainerOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::IsSetContainerOnChild() const
{
	return	(_Flags.IsSet(CONTAINER_ON_CHILD));
}

//-----------------------------------------------------------------------------
//	Name:		IsSetControllerOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::IsSetControllerOnChild() const
{
	return	(_Flags.IsSet(CONTROLLER_ON_CHILD));
}

//-----------------------------------------------------------------------------
//	Name:		IsSetEntityToUpdateOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::IsSetEntityToUpdateOnChild() const
{
	return	(_Flags.IsSet(ENTITY_TO_UPDATE_ON_CHILD));
}

//-----------------------------------------------------------------------------
//	Name:		IsSetGlobalMatrixInvalidOnChild
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::IsSetGlobalMatrixInvalidOnChild() const
{
	return	(_Flags.IsSet(GLOBAL_MATRIX_INVALID_ON_CHILD));
}

//-----------------------------------------------------------------------------
//	Name:		GetParentedIndex
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
int	NODE::GetParentedIndex() const
{
	return ( _nParentedIndex );
}

//-----------------------------------------------------------------------------
//	Name:		SetParentedIndex
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::SetParentedIndex(int	nIndex)
{
	_nParentedIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetParentGlobalMatrix
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE::GetParentGlobalMatrix() const
{
	if ( _nParentedIndex < 0 )
	{
		return ( GetParentNode()->GetGlobalMatrix() );
	}
	else
	{
		return ( GetParentNode()->GetIndexedGlobalMatrix( _nParentedIndex ));
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAsEntityNode
//	Object:		
//	04-03-12:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::SetAsEntityNode(bool	bVal)
{
	_Flags.Set( ENTITY_NODE, bVal );
}

//-----------------------------------------------------------------------------
//	Name:		IsEntityNode
//	Object:		
//	04-03-12:	CVI - Created
//-----------------------------------------------------------------------------
bool	NODE::IsEntityNode() const
{
	if ( _Flags.IsSet( ENTITY_NODE ) )
	{
		return ( true );
	}
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetStaticCollide
//	Object:		
//	04-03-26:	FDE - Created
//-----------------------------------------------------------------------------
void	NODE::SetStaticCollide(const bool	bStatus)
{
	_Flags.Set(STATIC_COLLISION, bStatus);
}

//-----------------------------------------------------------------------------
//	Name:		IsStaticCollide
//	Object:		
//	04-03-26:	FDE - Created
//-----------------------------------------------------------------------------
const bool	NODE::IsStaticCollide() const
{
	return (_Flags.IsSet(STATIC_COLLISION));
}

//-----------------------------------------------------------------------------
//	Name:		SetEntityFog
//	Object:		
//	04-08-12:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetEntityFog(bool	b)
{
	if	(b)
	{
		_Flags.SetFlags(ENTITY_FOG, ENTITY_FOG);
	}
	else
	{
		_Flags.SetFlags(ENTITY_FOG, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetEntityFog
//	Object:		
//	04-08-12:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::GetEntityFog() const
{
	return	(_Flags.IsSet(ENTITY_FOG));
}

//-----------------------------------------------------------------------------
//	Name:		GetDynamicActor
//	Object:		
//	04-08-12:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_DYNAMIC_ACTOR *	NODE::GetDynamicActor() const
{
	return _pDynamicActor;
}

//-----------------------------------------------------------------------------
//	Name:		GetPhysicController
//	Object:		
//	05-06-30:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_CONTROLLER *	NODE::GetPhysicController() const
{
	return	(_pPhysicController);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateOldPosition
//	Object:		
//	05-06-30:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UpdateOldPosition()
{
	_OldPos = _GMatrix.GetTranslation();
}

//-----------------------------------------------------------------------------
//	Name:		GetOldPosition
//	Object:		
//	05-06-30:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::GetOldPosition(VECTOR &	Position)
{
	Position = _OldPos;
}

//-----------------------------------------------------------------------------
//	Name:		CheckAndUpdateInsideTreeMatrices
//	Object:		
//	05-05-03:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::CheckAndUpdateInsideTreeMatrices()
{
	if ( (_Flags.IsSet(INSIDE_TREE_VALID)) == false )
	{
		UpdateInsideTree();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalBoundingBox
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
const OBB &	NODE::GetLocalBoundingBox() const
{
	return _LocalBoundingBox;
}

//-----------------------------------------------------------------------------
//	Name:		SetLocalBoundingBox
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
void	NODE::SetLocalBoundingBox(const OBB	& Obb)
{
	_LocalBoundingBox = Obb;
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalBoundingSphere
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
const SPHERE &	NODE::GetLocalBoundingSphere() const
{
	return _LocalBoundingSphere;
}

//-----------------------------------------------------------------------------
//	Name:		SetLocalBoundingSphere
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
void	NODE::SetLocalBoundingSphere(const SPHERE	& Sphere) 
{
	_LocalBoundingSphere = Sphere;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
