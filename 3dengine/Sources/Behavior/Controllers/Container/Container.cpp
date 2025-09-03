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
// CLASS: CONTAINER
//
//
//	01-01-19:	GHP - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (CONTAINER)
//
//	The CONTAINER class implements the behavior of a NODE
//
//*****************************************************************************

#include "Root.h"
#include "Container.h"

#include INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)
#include INCL_3DENGINE(Behavior/AnimNode/AnimNode)
#include INCL_3DENGINE(Behavior/TargetAnim/TargetAnimStruct)
#include INCL_3DENGINE(Behavior/BehaviorManager)
#include INCL_3DENGINE(SceneGraph/Node/Node)
//#include INCL_3DENGINE(Collision/CollisionManager)

#ifdef _DEBUG
	#include "Container.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CONTAINER constructor
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
CONTAINER::CONTAINER()
:
_bMustTeleport(false),
_bNewAnimAdded(false),
_ActiveAnim(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	_LMatrix.Identity();
	_Teleport.Id();
	_nIndex			= -1;
	_pMainContainer = NULL;
	_pSaveQVArray	= NULL;
	_nNbrChildren	= 0;
	_bSaveBlending	= false;
	_pHolderNode	= NULL;
}

//-----------------------------------------------------------------------------
//	Name:		CONTAINER constructor
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
CONTAINER::CONTAINER(const CONTAINER &	C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		~CONTAINER destructor
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
CONTAINER::~CONTAINER()
{
	DeleteAllAnimation();	
	_ActiveAnim.Release();
	if ( _pSaveQVArray )
	{
		delete [] _pSaveQVArray;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::Copy(const CONTAINER &	C)
{
	_FirstAnimNode = C._FirstAnimNode;

	_pHolderNode	= C._pHolderNode;

	_LMatrix		= C._LMatrix;
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
CONTAINER &	CONTAINER::operator=(const CONTAINER &	C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}


#ifdef _DEBUG
	//#define _DEBUG_ANIM
#endif

//-----------------------------------------------------------------------------
//	Name:		UpdateFromMainContainer
//	Object:		
//	03-07-30:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::UpdateFromMainContainer()
{
//	QDT_ASSERT(_LMatrix.HasNoScale());

	if (_pMainContainer && _pMainContainer->_StrAnim._pChildrenOutput )
	{
		if ( _nIndex < _pMainContainer->_StrAnim._nNbrChildren )
		{
			_pMainContainer->GetAnimMatrix( _nIndex, _LMatrix );
		}
		else
		{
			_LMatrix.Identity();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimMatrix
//	Object:		
//	03-07-30:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::GetAnimMatrix(int		nIndex,
								 MATRIX &	Mat)
{
	QUATERNION_VECTOR	*pQV;

	if ( _StrAnim._pChildrenOutput == NULL )
	{
		Mat.Identity();
		return;
	}

	pQV = _StrAnim._pChildrenOutput + nIndex;
//	QDT_ASSERT(pQV->GetQuat().IsNormalized());

	Mat.SetQuaternion( pQV->_Quat );
	Mat.SetPosition( pQV->_Vect );

//	QDT_ASSERT(Mat.HasNoScale());
}


//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::Update(float	rDelay)
{
	ANIM_NODE_STRUCT*		pAN;
	COM_INSTANCE			CI;
	float					rSaveDelay;

	//////////////////////////////////////
	//QDT_ASSERT( _FirstAnimNode.HasHandle() );
	CI = _FirstAnimNode;
	ShiftAnim();
	if ( CI.GetHandle() != _FirstAnimNode.GetHandle() )
	{
		_Teleport.Id();
	}
	//////////////////////////////////////

	//////////////////////////////////////
	pAN = GetAnimNodeStruct();
	if (!pAN)
	{
		_Teleport.Id();
		_StrAnim._pChildrenOutput = NULL;
		if ( GetPartAnimStruct() == NULL )
		{
			GetHolderNode()->UpdateGlobalMatrix();
			return;
		}
	}
	//////////////////////////////////////

	//////////////////////////////////////

#ifdef _DEBUG
	//rDelay = 0.2f;
#endif
	rSaveDelay = rDelay;
	PrepareStrBeforeUpdate( pAN );
	
	if (pAN)
	{
		QDT_ASSERT( pAN->IsActive() );
		pAN->GetAnimLink()->SetContainer( this );
		pAN->Process( rDelay, &_StrAnim );
	//	QDT_ASSERT( _ActiveAnim.HasHandle() );
	}
	//////////////////////////////////////

	//////////////////////////////////////
	UpdatePartAnim( rSaveDelay );
	AfterUpdate();
	UpdateTarget( rSaveDelay );
	GetHolderNode()->UpdateGlobalMatrix();
	//////////////////////////////////////

	_LastFirstAnimNode = _FirstAnimNode;
}

//-----------------------------------------------------------------------------
//	Name:		PrepareStrBeforeUpdate
//	Object:		
//	03-10-14:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::PrepareStrBeforeUpdate( ANIM_NODE_STRUCT *pAN )
{
	if ( pAN && pAN->IsBlending() == false )
	{
		_bSaveBlending = false;
	}

	_StrAnim._Own.Reset();
	_StrAnim._pChildrenOutput = NULL;
	_StrAnim.ResetCollideNotification();
	if ( pAN )
	{
		_StrAnim._nNbrChildren = pAN->GetNbrChildren();
	}

	if ( _bSaveBlending )
	{
		_StrAnim = _SaveStrAnim;
	}

	const MATRIX & Mat = GetHolderNode()->GetLocalMatrix();
	Mat.GetQuaternion( _StrAnim._Own._AbsRel._Absolute._Quat );
	_StrAnim._Own._AbsRel._Absolute._Vect = Mat.GetTranslation();
	_StrAnim._Own._Teleport.Id();
	if ( !_bSaveBlending )
	{
		return;
	}
	_StrAnim._pChildrenOutput = _SaveStrAnim._pChildrenOutput;
}


//-----------------------------------------------------------------------------
//	Name:		ShiftAnim
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ShiftAnim()
{
	STR_ANIM					StrAnim;
	COM_INSTANCE				CI;
	ANIM_NODE_STRUCT			*pANS;
	COM_INSTANCE				CINULL(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);


	////////////////////////////////////////////
	pANS = GetAnimNodeStruct();
	while( pANS && pANS->IsFinished() )
	{
		CI = pANS->GetAnimLink()->GetNextSeq();
		pANS->SendToDeleteList();
		SetAnimNode( CI );
//		QDT_ASSERT( CI.HasHandle() );
		pANS = GetAnimNodeStruct();
		if ( pANS )
		{
			pANS->GetAnimLink()->SetPrevSeq( CINULL );
		}
	}
	if ( _ActiveAnim.HasHandle() )
	{
		_ActiveAnim.Release();
	}
	if ( pANS && pANS->IsActive() == false )
	{
		pANS->InitBeforeStart( NULL );
		pANS->InitBeforeUpdateStart( NULL );
		pANS->Activate( true );
	}
}

//#define DEBUG_ANIM
//-----------------------------------------------------------------------------
//	Name:		AfterUpdate
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------

void	CONTAINER::AfterUpdate()
{
	MATRIX		Mat;
	MATRIX		NewLMat;
	MATRIX		LMat;

	Mat.Identity();
	NewLMat.Identity();
	LMat.Identity();
	
	/////////////////////////////////////////////////////////////////////
	if ( _StrAnim._Own._bAbsolute )
	{
		/////////////////////////////////////////////
		
		QDT_ASSERT(_StrAnim._Own._AbsRel._Absolute._Quat.IsNormalized());
		Mat.SetQuaternion( _StrAnim._Own._AbsRel._Absolute.GetQuat() );
		Mat.SetTranslation( _StrAnim._Own._AbsRel._Absolute.GetVector() );
		/////////////////////////////////////////////	
		
		/////////////////////////////////////////////
		GetHolderNode()->SetLocalMatrix( Mat );
		/////////////////////////////////////////////
	}
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	if ( _StrAnim._Own._bTeleport )
	{
		_Teleport	= _StrAnim._Own._Teleport;
		_StrAnim._Own._Teleport.Id();

		LMat = GetHolderNode()->GetLocalMatrix();
		QDT_ASSERT(_Teleport._Quat.IsNormalized());
		Mat.SetQuaternion( _Teleport.GetQuat() );
		Mat.SetTranslation( _Teleport.GetVector() );
	
		NewLMat.Mul( Mat, LMat );
		GetHolderNode()->SetLocalMatrix( NewLMat );
		QDT_ASSERT(NewLMat.HasNoScale() );
	}
	else
	{
		_Teleport.Id();
	}
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	QDT_ASSERT(_StrAnim._Own._AbsRel._Relative._Quat.IsNormalized());
	_LMatrix.SetQuaternion( _StrAnim._Own._AbsRel._Relative.GetQuat() );
	_LMatrix.SetTranslation( _StrAnim._Own._AbsRel._Relative.GetVector() );
	/////////////////////////////////////////////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		HasOrCreatesContainer
//	Object:		
//	01-03-07:	GHP - Created
//-----------------------------------------------------------------------------
bool	CONTAINER::HasOrCreatesContainer(NODE * const	pN) 
{
	CONTAINER		*pC;

	if	(pN->GetContainer() == NULL)		
	{	
		pC = new CONTAINER();
		pC->SetHolderNode( pN );
		pN->SetContainer( pC );	
	}

	return	(true);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================


//-----------------------------------------------------------------------------
//	Name:		DeleteAllAnimation
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::DeleteAllAnimation()
{
	DeleteMainAnimConnection();
	DeleteTargetConnection();
	DeleteAnimMixConnection();
}

//-----------------------------------------------------------------------------
//	Name:		DeleteTargetConnection
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::DeleteTargetConnection()
{
	ANIM_NODE_STRUCT	*pAN;
	COM_INSTANCE		CI;

	///////////////////////////////////////
	pAN = ANIM_NODE::GetStructFromInstance( _Target._TargetAnimInst );
	if ( pAN )
	{
		pAN->RecStop();
		_Target._TargetAnimInst.Release();
		_Target._StrAnim._pChildrenOutput = NULL;
		//ShiftAnim();
	}
	///////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		DeleteAnimMixConnection
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::DeleteAnimMixConnection()
{
	ANIM_NODE_STRUCT	*pAN;
	COM_INSTANCE		CI;

	///////////////////////////////////////
	pAN = GetPartAnimStruct();
	if ( pAN )
	{
		pAN->RecStop();
		ShiftPartAnim();
		_PartAnim._StrAnim._pChildrenOutput = NULL;
	}
	///////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		DeleteMainAnimConnection
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::DeleteMainAnimConnection()
{
	ANIM_NODE_STRUCT	*pAN;
	COM_INSTANCE		CI;

	///////////////////////////////////////
	pAN = GetAnimNodeStruct();
	if ( pAN )
	{
		pAN->RecStop();
		ShiftAnim();
		_StrAnim._pChildrenOutput = NULL;
	}
	///////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		SetSpeed
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetSpeed(COM_INSTANCE& Anim,
							float		rSpeed)
{
	ANIM_NODE_STRUCT	*pAN;
	TIME_ATTR_EX		TimeAttr;

	///////////////////////////////////////////////////////
	pAN			= GetAnimNodeStruct();
	TimeAttr	= pAN->GetTimeAttr();
	TimeAttr.SetSpeed( rSpeed );
	pAN->SetTimeAttr( TimeAttr );
	///////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
bool	CONTAINER::IsEmpty()
{
	if ( _FirstAnimNode.HasHandle() )	return ( false );		//	Not empty
	if ( _PartAnim._PartAnimInst.HasHandle() )	return ( false );
	return( true );		//	Empty
}

//-----------------------------------------------------------------------------
//	Name:		IsPlaying
//	Object:		
//	04-03-30:	CVI - Created
//-----------------------------------------------------------------------------
bool	CONTAINER::IsPlaying()
{
	ANIM_NODE_STRUCT	*pANS;

	//	First shift anim to be sure we would have next active anim
	ShiftAnim();
	//	Check if has a main animation
	if ( (_FirstAnimNode.HasHandle() ) == true  )
	{
		//	Yes, so check if it is still running
		pANS = ANIM_NODE::GetStructFromInstance( _FirstAnimNode );
		if ( (pANS->IsFinished()) == false )
		{
			//	Is playing
			_ActiveAnim = _FirstAnimNode;
			return ( true );
		}
	}

	//	Check if has a anim mix animation
	if ( (_PartAnim._PartAnimInst.HasHandle()) == true )
	{
		//	Yes, so check if it is still running
		pANS = ANIM_NODE::GetStructFromInstance( _PartAnim._PartAnimInst );
		if ( (pANS->IsFinished()) == false )
		{
			//	Now check if it is active
			if ( _PartAnim._bIsActive == true )
			{
				//	Is playing
				_ActiveAnim = _PartAnim._PartAnimInst;
				return ( true );
			}
		}
	}
	// not playing
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		OnAnimRemoved
//	Object:		
//	02-03-08:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::OnAnimRemoved(ANIM_NODE *	pAN)
{
	if ( COM_INSTANCE(pAN) == _FirstAnimNode )
	{
		QDT_ASSERT( pAN->GetAnimStruct()->IsFinished() );
		_FirstAnimNode.Release();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetActiveAnim
//	Object:		
//	03-06-12:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetActiveAnim(const COM_INSTANCE &	Anim)
{
	_ActiveAnim = Anim;
}

//-----------------------------------------------------------------------------
//	Name:		GetActiveAnim
//	Object:		
//	03-06-12:	CVI - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	CONTAINER::GetActiveAnim() const
{
	return ( _ActiveAnim );
}

//-----------------------------------------------------------------------------
//	Name:		ResetAnimMatrices
//	Object:		
//	03-09-29:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ResetAnimMatrices()
{
	_LMatrix.Identity();
	_LMatrix.SetTranslation(VECTOR::ZERO);
	_StrAnim._pChildrenOutput = NULL;
	_Teleport.Id();
}


//-----------------------------------------------------------------------------
//	Name:		SaveBlendingStart
//	Object:		
//	03-10-14:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SaveBlendingStart(ANIM_NODE_STRUCT *	pANS)
{
	int	i, n;

	//
	n = pANS->GetNbrChildren();
	if ( n > _nNbrChildren || !_pSaveQVArray )
	{
		if ( _pSaveQVArray )
		{
			delete[]_pSaveQVArray;
		}
		_pSaveQVArray	= new QUATERNION_VECTOR[n];
		_nNbrChildren	= n;
	}
	//
	_bSaveBlending					= true;
	_SaveStrAnim					= _StrAnim;
	//	SOLVE PB HERE AND STR_ANIM_BASE and see PREPARE method
	_SaveStrAnim._Own._Teleport.Id();
	_Teleport.Id();
	_SaveStrAnim._pChildrenOutput	= _pSaveQVArray;
	if ( !_StrAnim._pChildrenOutput )	return;
	//
	for ( i = 0; i < n; ++i )
	{
		_SaveStrAnim._pChildrenOutput[i] = _StrAnim._pChildrenOutput[i];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SaveBlendingStop
//	Object:		
//	03-10-14:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SaveBlendingStop()
{
	_bSaveBlending = false;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrChildren
//	Object:		
//	03-10-30:	GGO - Created
//-----------------------------------------------------------------------------
void	CONTAINER::SetNbrChildren(int	nNbrChildren)
{
	_nNbrChildren = nNbrChildren;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTarget
//	Object:		
//	03-11-17:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::UpdateTarget( float rDelay )
{
	if ( _Target._TargetAnimInst.HasHandle() == false )
	{
		return;
	}
	if ( _Target._bIsActive == false )
	{
		return;
	}
	TARGET_ANIM_STRUCT	*pTAS;
	COM_INSTANCE		SaveActiveAnim;

	SaveActiveAnim = _ActiveAnim;
	pTAS = static_cast<TARGET_ANIM_STRUCT*> ( ANIM_NODE::GetStructFromInstance( _Target._TargetAnimInst ) );
	pTAS->SetCurrentDelay( rDelay );
	pTAS->Process( rDelay , &_Target._StrAnim );
	if ( _StrAnim._pChildrenOutput )
	{
		pTAS->ApplyTarget( _StrAnim._pChildrenOutput );
	}
	_ActiveAnim = SaveActiveAnim;
}

//-----------------------------------------------------------------------------
//	Name:		ActivateTargetAnim
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ActivateTargetAnim(bool	bVal)
{
	_Target._bIsActive = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		PlayTargetAnim
//	Object:		
//	03-11-17:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::PlayTargetAnim(const COM_INSTANCE &	TargetAnim)
{
	COM_INSTANCE	CI;

	CI = _Target._TargetAnimInst;
	_Target._TargetAnimInst = TargetAnim;
	TARGET_ANIM_STRUCT	*pAN;

	pAN = static_cast<TARGET_ANIM_STRUCT*> ( ANIM_NODE::GetStructFromInstance( TargetAnim ) );
	pAN->SetContainer( this );

	if ( _Target._bIsActive )
	{
		//	Copy current values
		TARGET_ANIM_STRUCT	*pOld;
		pOld = static_cast<TARGET_ANIM_STRUCT*> ( ANIM_NODE::GetStructFromInstance( CI ) );
		pAN->InitFromPrevious( pOld );
	}
}


//-----------------------------------------------------------------------------
//	Name:		ActivatePartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ActivatePartAnim(bool	bVal)
{
	_PartAnim._bIsActive = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		PlayPartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::PlayPartAnim(const COM_INSTANCE &	PartAnim)
{
	ANIM_NODE_STRUCT	*pAN;

	ShiftPartAnim();
	
	if ( _PartAnim._PartAnimInst.HasHandle() == false )
	{
		_PartAnim._PartAnimInst = PartAnim;
		pAN = GetPartAnimStruct();
		pAN->SetContainer( this );
		pAN->InitBeforeStart( NULL );
		pAN->InitBeforeUpdateStart( NULL );
	}
	else
	{
		pAN = GetPartAnimStruct();
		pAN->AddSequence( PartAnim );
	}
	pAN = ANIM_NODE::GetStructFromInstance( PartAnim );
	pAN->SetContainer( this );
}

//-----------------------------------------------------------------------------
//	Name:		UpdatePartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::UpdatePartAnim(float	rDelay)
{
	if ( _PartAnim._PartAnimInst.HasHandle() == false )
	{
		return;
	}
	if ( _PartAnim._bIsActive == false )
	{
		return;
	}

	ANIM_NODE_STRUCT	*pAN;
	COM_INSTANCE		SaveActiveAnim;

	ShiftPartAnim();
	SaveActiveAnim = _ActiveAnim;
	pAN = GetPartAnimStruct();
	if ( !pAN )	return;

	pAN->GetAnimLink()->SetContainer( this );
	_PartAnim._StrAnim.ResetCollideNotification();
	pAN->Process( rDelay , &_PartAnim._StrAnim );
//	if ( _StrAnim._pChildrenOutput )
	{
		pAN->ApplyPartAnim( &_PartAnim._StrAnim, &_StrAnim );
	}
	_ActiveAnim = SaveActiveAnim;
}

//-----------------------------------------------------------------------------
//	Name:		ShiftPartAnim
//	Object:		
//	03-12-03:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ShiftPartAnim()
{
	COM_INSTANCE				CI;
	ANIM_NODE_STRUCT			*pANS;
	COM_INSTANCE				CINULL(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
	COM_INSTANCE				SaveActiveAnim;
	int							nBlendStatus;
	float						rBlendCoeff;

	////////////////////////////////////////////
	pANS = GetPartAnimStruct();
	if ( !pANS )	return;
	if ( pANS->IsFinished() == false )
	{
		return;
	}
	////////////////////////////////////////////

	////////////////////////////////////////////
	SaveActiveAnim = _ActiveAnim;
	nBlendStatus = pANS->GetPartBlendingStatus();
	rBlendCoeff  = pANS->GetPartBlendCoeff();
	////////////////////////////////////////////

	////////////////////////////////////////////
	while( pANS && pANS->IsFinished() )
	{
		CI = pANS->GetAnimLink()->GetNextSeq();
		pANS->SendToDeleteList();
		_PartAnim._PartAnimInst = CI;
//		QDT_ASSERT( CI.HasHandle() );
		pANS = GetPartAnimStruct();
		if ( pANS )
		{
			pANS->GetAnimLink()->SetPrevSeq( CINULL );
		}
	}
	////////////////////////////////////////////

	////////////////////////////////////////////
	if ( pANS )
	{
		pANS->SetPartBlendingStatus( nBlendStatus );
		pANS->SetPartBlendCoeff( rBlendCoeff );
	}
	////////////////////////////////////////////

	////////////////////////////////////////////
	if ( pANS && pANS->IsActive() == false )
	{
		pANS->InitBeforeStart( NULL );
		pANS->InitBeforeUpdateStart( NULL );
		pANS->Activate( true );
	}
	////////////////////////////////////////////

	////////////////////////////////////////////
	_ActiveAnim = SaveActiveAnim;
	////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimNode
//	Object:		
//	01-10-19:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_NODE *	CONTAINER::GetAnimNode() 
{
	ANIM_NODE*		pAN;
	COM_INSTANCE	CI;

	/////////////////////////////////////////////////////
	if ( !_FirstAnimNode.HasHandle() )		return ( NULL );
	pAN = static_cast<ANIM_NODE*> ( _FirstAnimNode.GetCommunicator() );
	return ( pAN );
	/////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimNodeStruct
//	Object:		
//	03-03-25:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT *	CONTAINER::GetAnimNodeStruct()
{
	ANIM_NODE_STRUCT	*pANS;

	pANS = ANIM_NODE::GetStructFromInstance( _FirstAnimNode );
	return ( pANS );
}

//-----------------------------------------------------------------------------
//	Name:		GetPartAnimStruct
//	Object:		
//	03-12-03:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_NODE_STRUCT *	CONTAINER::GetPartAnimStruct()
{
	ANIM_NODE_STRUCT	*pAN;

	pAN = ANIM_NODE::GetStructFromInstance( _PartAnim._PartAnimInst );
	return ( pAN );
}

//-----------------------------------------------------------------------------
//	Name:		ProcessPartAnimActivation
//	Object:		
//	03-12-03:	CVI - Created
//-----------------------------------------------------------------------------
void	CONTAINER::ProcessPartAnimActivation(bool	bVal,
											 bool	bBlend)
{
	ANIM_NODE_STRUCT	*pAN;

	pAN = GetPartAnimStruct();
	if ( !pAN )	return;
	pAN->ActivePartAnim( bVal, bBlend );
	if ( bVal )
	{
		///////////////////////////////////
		// Don't forget to register animation to update it!
		if ( _pHolderNode )
		{
			QDT_ASSERT(_pHolderNode );
			BEHAVIOR_MANAGER::Instance()->AddAnimatedNode(COM_INSTANCE(_pHolderNode));
		}
		///////////////////////////////////
	}
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================

