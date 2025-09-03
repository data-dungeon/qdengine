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
//	CLASS:	TARGET_ANIM_STRUCT
//
//	01-12-18:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"TargetAnimStruct.h"
#include	INCL_3DENGINE(Behavior/Controllers/Container/Container)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)

#ifdef _DEBUG
	#include "TargetAnimStruct.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TARGET_ANIM_STRUCT constructor
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
TARGET_ANIM_STRUCT::TARGET_ANIM_STRUCT()
:
_TargetPoint( 0.0f, 0.0f, 0.0f ),
_nNbrTargetTracks( 0 ),
_rCurrentDelay( 0.0f ),
_nTargetIndex(-1),
_bBlendTarget( false ),
_bTargetIsActive( false ),
_bGoingToTarget( false )
{

}

//-----------------------------------------------------------------------------
//	Name:		~TARGET_ANIM_STRUCT destructor
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
TARGET_ANIM_STRUCT::~TARGET_ANIM_STRUCT()
{
	_TargetNodeInst.Release();
}


int	_nValueUpdate = 0;
//-----------------------------------------------------------------------------
//	Name:		StoreRotTrans
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::StoreRotTrans(float	rTime)
{
	int						i;
	float					rDelay;
	QUATERNION_VECTOR		QV;
	

	_rCurrentDelay = _TimeAttr.GetCurrentDeltaTime();

	rDelay = _rCurrentDelay;
	if ( _bBlendTarget )
	{
		_BlendingTimer.UpdateTime( _rCurrentDelay );
	}
	ResetInduction();
	UpdateTargetNodePosition();
	for ( i = _nNbrTargetTracks - 1; i >= 0; i-- )
	{
		_TargetTrack[i].Update( _TargetPoint, rDelay, this );
		_pChildrenOwn[i]._Quat = _TargetTrack[i].GetResultQuat();
//		_pChildrenOwn[i]._Vect = VECTOR( 0.0f, 0.0f, 0.0f );
	}

}

//-----------------------------------------------------------------------------
//	Name:		ApplyTarget
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::ApplyTarget(QUATERNION_VECTOR *	pQV)
{
	int			i;
	int			nIndex;

	if ( _bBlendTarget )
	{
		//	BLENDING ACTIVED

		QUATERNION_VECTOR	QV;
		float				rCoeff;

		//	Compute coefficient of interpolation
		rCoeff = _BlendingTimer.GetCurTime() / _BlendingTimer.GetTimeLong();
		if ( !_bGoingToTarget )	rCoeff = 1.0f - rCoeff;

		// Parse all tracks and compute interpolation
		for ( i = 0; i < _nNbrTargetTracks; ++i )
		{
			nIndex	= _TargetTrack[i].GetTrackIndex();
			QV		= pQV[nIndex];
			pQV[nIndex].BasicInterSLERP( rCoeff, QV, _pChildrenOutput[i] );
		}

		//	Check when blending is finished
		if ( _BlendingTimer.IsFinished() )
		{
			_bBlendTarget = false;
			if ( _bGoingToTarget == false )
			{
				_AnimLink.GetContainer()->ActivateTargetAnim( false );
				_bTargetIsActive = false;
				ResetTrackParameters();
			}
		}
		//
	}
	else
	{
		//	NO BLENDING 

		//	Parse all tracks and copy transformation
		for ( i = 0; i < _nNbrTargetTracks; ++i )
		{
			nIndex		= _TargetTrack[i].GetTrackIndex();
			pQV[nIndex] = _pChildrenOutput[i];
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		InitTargetTracks
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::InitTargetTracks( const COM_INSTANCE& SklInst )
{
	int				i, j;
	SKELETON		*pSK;
	int				nIndex;
	int				nType;
	NODE_ARRAY		*pNodeArray;
	int				nParentIndex;

	pSK = static_cast<SKELETON *>(SklInst.GetCommunicator());
	InitChildren( _nNbrTargetTracks );

	pNodeArray = static_cast<NODE_ARRAY *>(pSK->GetNode());

	_BlendingTimer.SetTimeLong( 30.0f );

	for ( i = 0; i < _nNbrTargetTracks; ++i )
	{
		nType	= _TargetTrack[i].GetBoneType();
		nIndex	= pSK->GetSpecialBoneIndex( nType );
		if ( nIndex < 0 )
		{
			//QDT_FAIL();
			continue;
		}
		_TargetTrack[i].SetTrackIndex( nIndex );
		//	Search parent num target
		nParentIndex = pNodeArray->GetChild(nIndex)->GetFatherIndex();		//	Get parent index
		for ( j = 0; j < i; ++j )
		{
			//	check track index of previous target tracks
			if ( _TargetTrack[j].GetTrackIndex() == nParentIndex )
			{
				_TargetTrack[i].SetParentIndex( j );
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		ResetInduction
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::ResetInduction()
{
	int		i;

	for ( i = 0; i < _nNbrTargetTracks; ++i )
	{
		if ( _TargetTrack[i].IsInduction() )
		{
			_TargetTrack[i].ResetInduction();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalMatrix
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
const MATRIX &	TARGET_ANIM_STRUCT::GetGlobalMatrix(int	nNumObj)
{
	NODE_ARRAY *pN;

	pN = static_cast<NODE_ARRAY*> ( _AnimLink.GetContainer()->GetHolderNode() );
	return ( pN->GetIndexedGlobalMatrix( nNumObj ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetContainerMatrix
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
const MATRIX &	TARGET_ANIM_STRUCT::GetContainerMatrix(int	nNumObj)
{
	NODE_ARRAY *pN;

	pN = static_cast<NODE_ARRAY *> ( _AnimLink.GetContainer()->GetHolderNode() );
	return ( pN->GetIndexedContainerMatrix( nNumObj ) );
}

//-----------------------------------------------------------------------------
//	Name:		AddInductionToParent
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::AddInductionToParent(int				nParentIndex,
												 const QUATERNION &	Q)
{
	_TargetTrack[nParentIndex].AddInduction( Q );
}

//-----------------------------------------------------------------------------
//	Name:		SetTrackBoneType
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetTrackBoneType(int	nNum,
											 int	nType)
{
	_TargetTrack[nNum].SetBoneType( nType );
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxSpeed
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetMaxSpeed(int				nNum,
										const VECTOR &	V)
{
	_TargetTrack[nNum].SetMaxSpeed( V );
}

//-----------------------------------------------------------------------------
//	Name:		SetRange
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetRange(int			nNum,
									 const VECTOR &	V)
{
	_TargetTrack[nNum].SetRange( V );
}

//-----------------------------------------------------------------------------
//	Name:		SetAcceleration
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetAcceleration(int				nNum,
											const VECTOR &	V)
{
	_TargetTrack[nNum].SetAcceleration( V );
}

//-----------------------------------------------------------------------------
//	Name:		SetInductionWeight
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetInductionWeight(int		nNum,
											   float	rW)
{
	_TargetTrack[nNum].SetInductionWeight( rW );
}

//-----------------------------------------------------------------------------
//	Name:		SetInducted
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetInducted(int		nNum,
										bool	bVal)
{
	if ( bVal )
	{
		_TargetTrack[nNum].SetInducted( 1 );
	}
	else
	{
		_TargetTrack[nNum].SetInducted( 0 );
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetPoint
//	Object:		
//	03-11-13:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetTargetPoint(const VECTOR &	P)
{
	_TargetPoint = P;
}

#define _FIX_BUG_WATCH_WHEN_ACTIVATE_DURING_BLENDING_FROM
//-----------------------------------------------------------------------------
//	Name:		ActivateTarget
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::ActivateTarget(bool	bVal, bool bBlend )
{
#ifdef _FIX_BUG_WATCH_WHEN_ACTIVATE_DURING_BLENDING_FROM
	if ( bVal == false && _bTargetIsActive == false )
	{
		//	Already not active so nothing to do
		return;
	}

	if ( bVal && _bTargetIsActive == true )
	{
		if ( _bBlendTarget == false )
		{
			//	Already active ( blending off ) : nothing to do
			return;
		}
		//	Reactivation while blending from target
		if ( _bGoingToTarget == false )
		{
			ActiveBlendingTimer(true);
		}
		return;
	}
#endif
	//	Activation when not active
	if ( bVal && _bTargetIsActive == false )
	{
		if ( _AnimLink.GetContainer() )
		{
			_AnimLink.GetContainer()->ActivateTargetAnim( bVal );
		}
		_bTargetIsActive = true;
		if ( bBlend )
		{
			ActiveBlendingTimer( true );
		}
		return;
	}

	if ( !bVal && _bTargetIsActive == true )
	{
		if ( !bBlend )
		{
			if ( _AnimLink.GetContainer() )
			{
				_AnimLink.GetContainer()->ActivateTargetAnim( bVal );
			}
			_bTargetIsActive = false;
			ResetTrackParameters();
			return;
		}
		ActiveBlendingTimer( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetTargetTrack
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
TARGET_TRACK *	TARGET_ANIM_STRUCT::GetTargetTrack(int	nNum)
{
	return ( _TargetTrack + nNum );
}

//-----------------------------------------------------------------------------
//	Name:		ActiveBlendingTimer
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::ActiveBlendingTimer(bool	bToGo)
{
	_bGoingToTarget = bToGo;
	if ( _bBlendTarget )
	{
		_BlendingTimer.Mirror();
	}
	else
	{
		_BlendingTimer.Reset();
	}
	_bBlendTarget	= true;
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentDelay
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::SetCurrentDelay(float	rDelay)
{
	_rCurrentDelay = rDelay;
}

//-----------------------------------------------------------------------------
//	Name:		ResetTrackParameters
//	Object:		
//	03-11-25:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::ResetTrackParameters()
{
	int		i;

	for ( i = 0; i < _nNbrTargetTracks; ++i )
	{
		_TargetTrack[i].ResetParameters();
	}
}

//-----------------------------------------------------------------------------
//	Name:		LockTargetNode
//	Object:		
//	03-11-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::LockTargetNode(const COM_INSTANCE &	NodeInst, int nIndex )
{
	_TargetNodeInst = NodeInst;
	_nTargetIndex	= nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		UnlockTargetNode
//	Object:		
//	03-11-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::UnlockTargetNode()
{
	_TargetNodeInst.Release();
	_nTargetIndex = -1;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTargetNodePosition
//	Object:		
//	03-11-26:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::UpdateTargetNodePosition()
{
	if ( !_TargetNodeInst.HasHandle() )	return;

	
	if ( _nTargetIndex >= 0 )
	{
		UpdateTargetSklNodePosition();
		return;
	}
	NODE	*pN;

	pN = static_cast<NODE*> ( _TargetNodeInst.GetCommunicator() );
	pN->GetGlobalPosition( _TargetPoint );
}

//-----------------------------------------------------------------------------
//	Name:		UpdateTargetSklNodePosition
//	Object:		
//	04-01-08:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::UpdateTargetSklNodePosition()
{
	NODE_ARRAY	*pN;

	pN	= static_cast<NODE_ARRAY *> ( _TargetNodeInst.GetCommunicator() );
	_TargetPoint = pN->GetIndexedGlobalMatrix(_nTargetIndex).GetTranslation();
}

//-----------------------------------------------------------------------------
//	Name:		InitFromPrevious
//	Object:		
//	04-07-27:	CVI - Created
//-----------------------------------------------------------------------------
void	TARGET_ANIM_STRUCT::InitFromPrevious(TARGET_ANIM_STRUCT *	pPrevTS)
{
	int		i;

	if ( !pPrevTS )	return;
	for ( i = 0; i < _nNbrTargetTracks; ++i )
	{
		_TargetTrack[i].InitFromPrevious( pPrevTS->GetTargetTrack( i ) );
	}
	_BlendingTimer = pPrevTS->_BlendingTimer;
	
}
