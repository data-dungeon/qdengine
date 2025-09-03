//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: BEHAVIOR_MANAGER
//	The BEHAVIOR_MANAGER class implements All the behavioral stuff
//	Aka Animation, if something is animated it should be updated through 
//	the behavior manager.
//	in the first version of the engine this class was named BEHAVIOR
//
//	00-12-12	RMA - Created
//	01-09-03	ONA - Updated categories includes
//	01-03-21:	GHP Modified  : renamed class
//*****************************************************************************

#include	"Root.h"
#include	"BehaviorManager.h"

#include INCL_KCOM(ComBasis/SingletonDescriptor)
#include INCL_KCORE(Time/Time_ABC)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/EntityBehaviorFactory)
#include INCL_3DENGINE(Behavior/Behaviors/Node Behaviors/CamFree)
#include INCL_3DENGINE(Behavior/Controllers/Entity Controllers/EntityControllerFactory)
#include INCL_3DENGINE(Behavior/Controllers/Container/Container)
#include INCL_3DENGINE(Behavior/AnimNode/AnimNode)
#include INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)
#include INCL_3DENGINE(Behavior/HipsAnim/HipsAnim)
#include INCL_3DENGINE(Behavior/Datas/NumericAnimData)
#include INCL_3DENGINE(Behavior/Datas/ColorAnimData)
#include INCL_3DENGINE(Behavior/Datas/TextureAnimData)
#include INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

BEHAVIOR_MANAGER*	BEHAVIOR_MANAGER::_pInstance = 0; 

//-----------------------------------------------------------------------------
// Name:		BEHAVIOR_MANAGER::Instance
// Object:		For singleton : Instance 
// 01-01-24		FKE Created: 
//-----------------------------------------------------------------------------
BEHAVIOR_MANAGER* BEHAVIOR_MANAGER::Instance()
{
	return	((BEHAVIOR_MANAGER *)GetSingletonDescriptor()->Instance());
}

//-----------------------------------------------------------------------------
// Name:		BEHAVIOR_MANAGER::Suicide
// Object:		For singleton : suicide
// 01-01-24		FKE Created: 
//-----------------------------------------------------------------------------
void BEHAVIOR_MANAGER::Suicide()
{
	GetSingletonDescriptor()->Release();
}

//-----------------------------------------------------------------------------
//	Name:		COM_Init
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::COM_Init(CLASS_DESCRIPTOR * const	pCD)
{
//	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

//	pCD->AddEnum( "CUSTOM_FOR_ALL_VIEWPORTS",			CUSTOM_FOR_ALL_VIEWPORTS,			"The option are controlled at viewport level.");

//	pM = new	METHOD_DESCRIPTOR("GetWindowSize", "Ask 3D Engine to document this command.");
//	pM->AddParameterReference("nSizeY", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
//	pCD->AddMethod(CMD_DM_GET_WINDOW_SIZE, pM, EM_GetWindowSize);

//	pM = new	METHOD_DESCRIPTOR("GetLodStartDistance", "Get the distance from which the lod starts.");

	//	Now register the class as a global variable
//	GLOBAL_VARIABLE::AddVariable(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_DISPLAY_MANAGER, "DISPLAY_MANAGER", pCD, _SD.GetSingletonPointer());
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_Construct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	BEHAVIOR_MANAGER *	pObject;
	pObject = static_cast<BEHAVIOR_MANAGER*>(pO);
	QDT_NEW_PLACED(pObject, BEHAVIOR_MANAGER)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_Destruct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	BEHAVIOR_MANAGER *	pObject;
	pObject = static_cast<BEHAVIOR_MANAGER*> (pO);
	pObject->BEHAVIOR_MANAGER::~BEHAVIOR_MANAGER();
}

//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER constructor
// Object: Constructor
//00 - 12 - 12 RMA Created:
//-----------------------------------------------------------------------------
BEHAVIOR_MANAGER::BEHAVIOR_MANAGER()
{
	
	_pInstance = this;

	ENTITY_BEHAVIOR_FACTORY::Instance();
	ENTITY_CONTROLLER_FACTORY::Instance();
}

//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER destructor 
// Object:	Destructor
//00 - 12 - 12 RMA Created:
//-----------------------------------------------------------------------------
BEHAVIOR_MANAGER::~BEHAVIOR_MANAGER()
{
	_vAnimatedNodes.Clear();
	_sAnimationInsts.Clear() ;

	_pInstance = NULL;

}

//-----------------------------------------------------------------------------
//	Name:		ApplyAnimationToNode
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
bool	BEHAVIOR_MANAGER::ApplyAnimationToNode(const COM_INSTANCE &NodeInst,
											  const COM_INSTANCE& Anim,
											  int& nIndex,
											  CONTAINER *pMainContainer )
{
	NODE				*pN;
	CONTAINER			*pC;
	ANIM_NODE_STRUCT	*pAN;
	int					nMax;

	///////////////////////////////////
	//	Avoid crash
	if (!NodeInst.HasHandle())	return (false);
	if ( !Anim.HasHandle() )	return ( false );
	///////////////////////////////////
	
	///////////////////////////////////
	//	Get node container
	pN = static_cast<NODE*> (NodeInst.GetCommunicator());
	CONTAINER::HasOrCreatesContainer( pN );
	pC = pN->GetContainer();
	///////////////////////////////////

	///////////////////////////////////
	//	Init container
	pC->SetMainContainer( pMainContainer );
	pC->SetIndex( nIndex );
	///////////////////////////////////

	///////////////////////////////////
	//	Get number anim tracks
	pAN = ANIM_NODE::GetStructFromInstance( Anim );
	nMax = pAN->GetNbrChildren();
	///////////////////////////////////

	///////////////////////////////////
	//	Check if head node
	if ( nIndex == -1 )
	{
		///////////////////////////////////
		pMainContainer = ProcessHeadNodeApply( pN, pAN, Anim );
		///////////////////////////////////
	}
	///////////////////////////////////

	///////////////////////////////////
	// Don't forget to register animation to update it!
	AddAnimatedNode(NodeInst);
	///////////////////////////////////

	///////////////////////////////////
	nIndex += pN->GetPotentialNbrTracks();		//	1 for node, or more for NODE_ARRAY
	if ( nIndex >= nMax )	return ( true );
	///////////////////////////////////
	
	///////////////////////////////////
	//	Apply on node children
	NODE *	pChild = pN->GetFirstChild();
	while	(pChild)
	{
		///////////////////////////////////
		//	apply on a children
		ApplyAnimationToNode( COM_INSTANCE(pChild), Anim, nIndex, pMainContainer );
		///////////////////////////////////
		if ( nIndex >= nMax )	return ( true );
		///////////////////////////////////

		pChild = pChild->GetBrother();
	}
	///////////////////////////////////
	
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ProcessHeadNodeApply
//	Object:		
//	04-01-19:	CVI - Created
//-----------------------------------------------------------------------------
CONTAINER *	BEHAVIOR_MANAGER::ProcessHeadNodeApply(NODE *				pN,
												   ANIM_NODE_STRUCT *	pAN,
												   const COM_INSTANCE& Anim )
{
	CONTAINER		*pC;

	QDT_ASSERT( pN->GetContainer() );
	pC = pN->GetContainer();
	pAN->SetContainer( pC );

	if ( pC->GetAnimNode() )
	{
		pC->GetAnimNodeStruct()->AddSequence( Anim );
	}
	else
	{
		pC->SetAnimNode( Anim );
		pAN->Activate( true );
		pAN->InitBeforeStart( NULL );
		pAN->InitBeforeUpdateStart( NULL );
		pC->SetActiveAnim( Anim );
	}
	pC->SetNbrChildren( pAN->GetNbrChildren() );
	return ( pC );
}

//-----------------------------------------------------------------------------
//	Name:		ApplyFreeCameraAnimationToNode
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------

COM_INSTANCE	BEHAVIOR_MANAGER::ApplyFreeCameraAnimationToNode(const COM_INSTANCE &	CameraInst)
{
	COM_INSTANCE	NodeInst = static_cast<CAMERA *>(CameraInst.GetCommunicator())->GetNodeInst();
	NODE *			pNode  = static_cast<NODE *>(NodeInst.GetCommunicator()); 

	COM_INSTANCE	Anim;
	COM_INSTANCE	CFInst;
	CAM_FREE		*pCF;
	int				nChildIndex = -1;
	
	CFInst = COM_INSTANCE(CAM_FREE::GetGenericCommunicatorDescriptor()->Create());
	pCF = static_cast<CAM_FREE *>(CFInst.GetCommunicator());
	pCF->SetEntityInst(CameraInst);

	Anim.SetHandle( CFInst.GetHandle() );
	
	CONTAINER::HasOrCreatesContainer(pNode);

	/////////////////////////////////////////////////////////////////
	//	Apply the behavior to the node
	if ( ! ApplyAnimationToNode(NodeInst , Anim, nChildIndex, NULL ) )
	{
		QDT_Warning(QDT_COMMENT("ApplyAnimationToNode failed"));
		Anim.Release();
	}
	/////////////////////////////////////////////////////////////////

	AddAnimatedNode(NodeInst);

	return(Anim);
}


//-----------------------------------------------------------------------------
//	Name:		SetAnimationSpeed
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::SetAnimationSpeed(const COM_INSTANCE &	ChildInst,
											COM_INSTANCE&		Anim,
											float				rSpeed)
{
	NODE			*pN;
	CONTAINER		*pC;

	///////////////////////////////////////
	pN = static_cast<NODE*> ( ChildInst.GetCommunicator() );
	pC = pN->GetContainer();
	if ( !pC )	return;
	pC->SetSpeed( Anim, rSpeed );
	///////////////////////////////////////
}




//-----------------------------------------------------------------------------
//	Name:		RemoveAllAnimationFromNode
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::RemoveAllAnimationFromNode( const COM_INSTANCE& ChildInst)
{
	CONTAINER		*pC;
	NODE			*pN;

	if ( !ChildInst.HasHandle() )	return;
	pN	= static_cast<NODE*>(ChildInst.GetCommunicator());
	if ( !pN )	return;
	pC = pN->GetContainer();
	if ( !pC )	return;

	pC->DeleteAllAnimation();
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAnimation
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateAnimation(float	rSpeedFactor)
{	
	UpdateAnimatedNodes(rSpeedFactor);
	UpdateNodesWithController(rSpeedFactor);
	UpdateColors();
	UpdateNumericAttributes();
	UpdateMaterialTextureAnim();
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAnimatedNodes
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateAnimatedNodes(float rSpeedFactor)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR it	  = _vAnimatedNodes.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = _vAnimatedNodes.GetTail();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itNext;
	COM_INSTANCE NodeCI;
	// Warning : update behavior can remove a node from the list
	// That's why I use itNext to make sure it is valid

	while (it != itTail)
	{

		itNext = it + 1;
		QDT_ASSERT((*it).HasHandle());
		NodeCI = *it;
		NODE * pNode = static_cast<NODE*>(NodeCI.GetCommunicator());
		pNode->UpdateBehavior(rSpeedFactor, false);

		if	(pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
		{
			static_cast<NODE_ARRAY *>(pNode)->UpdateAllMatrices(pNode->GetGlobalMatrix());
		}

		it = itNext;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateAnimatedNodesChildren
//	Object:		
//	05-04-04:	ELE - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateAnimatedNodesChildren()
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR it	  = _vAnimatedNodes.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = _vAnimatedNodes.GetTail();
	COM_INSTANCE NodeCI;

	while (it != itTail)
	{
		NodeCI = *it;
		NODE * pNode = static_cast<NODE*>(NodeCI.GetCommunicator());
		pNode->UpdateChildrenGlobalMatrix();

		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddAnimatedNode
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::AddAnimatedNode(const COM_INSTANCE &	NodeInst)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR it;
	it = _vAnimatedNodes.Find(_vAnimatedNodes.GetHead(), NodeInst);
	if (it == _vAnimatedNodes.GetTail())
	{
		_vAnimatedNodes.PushTail(NodeInst);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAnimatedNode
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::RemoveAnimatedNode(const COM_INSTANCE &	NodeInst)
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR it;
	it = _vAnimatedNodes.Find(_vAnimatedNodes.GetHead(), NodeInst);
	if (it != _vAnimatedNodes.GetTail())
	{
		_vAnimatedNodes.Remove(it);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateNodesWithController
//	Object:		
//	04-01-29:	AMA - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateNodesWithController(float	rSpeedFactor)
{
	QDT_DLIST <NODE *>::ITERATOR it	  = _vNodesWithController.GetHead();
	QDT_DLIST <NODE *>::ITERATOR itTail = _vNodesWithController.GetTail();
	
	while (it != itTail)
	{
		NODE * pNode = *it;
		pNode->UpdateController(rSpeedFactor);
		it++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddNodeWithController
//	Object:		
//	04-01-29:	AMA - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::AddNodeWithController(NODE * pNode)
{
	QDT_DLIST<NODE *>::ITERATOR it;
	it = _vNodesWithController.Find(_vNodesWithController.GetHead(), pNode);
	if (it == _vNodesWithController.GetTail())
	{
		_vNodesWithController.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveNodeWithController
//	Object:		
//	04-01-29:	AMA - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::RemoveNodeWithController(NODE * pNode)
{
	QDT_DLIST<NODE *>::ITERATOR it;
	it = _vNodesWithController.Find(_vNodesWithController.GetHead(), pNode);
	if (it != _vNodesWithController.GetTail())
	{
		_vNodesWithController.Remove(it);
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateHipsAnim
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	BEHAVIOR_MANAGER::CreateHipsAnim()
{
	COM_INSTANCE	CI;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CI = QueryHipsAnim();
	QDT_ASSERT( CI.HasHandle() );
	//increment reference of the one created by the behavior
	return ( CI );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		PlayAnim
//	Object:		
//	01-11-12:	cvi - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::PlayAnim(const COM_INSTANCE & NodeInst,
								   const COM_INSTANCE & Anim,
								   bool					bSeq )
{
	ANIM_STATUS		AnimStatus;
	int				nChildIndex = -1;	

	ApplyAnimationToNode(NodeInst, Anim, nChildIndex, NULL );
}

//-----------------------------------------------------------------------------
//	Name:		UpdateNumericAttributes
//	Object:		
//	03-11-18:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateNumericAttributes()
{
	if (_vAnimatedNumAttr.IsEmpty()) return;

	QDT_DLIST<NUMERIC_ANIM_DATA *>::ITERATOR itEach = _vAnimatedNumAttr.GetHead();
	QDT_DLIST<NUMERIC_ANIM_DATA *>::ITERATOR itTail = _vAnimatedNumAttr.GetTail();
	NUMERIC_ANIM_DATA *pData = NULL;

	float rDelay = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime();

	while (itEach != itTail)
	{	
		pData = (*itEach++);
		pData->UpdateBehavior(rDelay);
	}	
}

//-----------------------------------------------------------------------------
//	Name:		AddAnimatedNumericAttributes
//	Object:		
//	03-11-18:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::AddAnimatedNumericAttributes(NUMERIC_ANIM_DATA * pNumericAnimData)
{
	_vAnimatedNumAttr.PushTail(pNumericAnimData);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAnimatedNumericAttributes
//	Object:		
//	03-11-18:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::RemoveAnimatedNumericAttributes(NUMERIC_ANIM_DATA * pNumericAnimData)
{
	QDT_DLIST<NUMERIC_ANIM_DATA *>::ITERATOR itEach = _vAnimatedNumAttr.GetHead();
	QDT_DLIST<NUMERIC_ANIM_DATA *>::ITERATOR itTail = _vAnimatedNumAttr.GetTail();

	while (itEach != itTail)
	{
		if (*itEach == pNumericAnimData)
		{
			_vAnimatedNumAttr.Remove(itEach);
			return;
		}

		++itEach;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateMaterialTextureAnim
//	Object:		
//	03-12-08:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateMaterialTextureAnim()
{
	if (_vAnimatedTexture.IsEmpty()) return;

	QDT_DLIST<TEXTURE_ANIM_DATA *>::ITERATOR itEach = _vAnimatedTexture.GetHead();
	QDT_DLIST<TEXTURE_ANIM_DATA *>::ITERATOR itTail = _vAnimatedTexture.GetTail();
	TEXTURE_ANIM_DATA *pData = NULL;

	float rDelay = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime();

	while (itEach != itTail)
	{	
		pData = (*itEach++);
		pData->UpdateBehavior(rDelay);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddAnimatedMaterialTexture
//	Object:		
//	03-12-08:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::AddAnimatedMaterialTexture(TEXTURE_ANIM_DATA * pTextureAnimData)
{
	_vAnimatedTexture.PushTail(pTextureAnimData);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAnimatedMaterialTexture
//	Object:		
//	03-12-08:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::RemoveAnimatedMaterialTexture(TEXTURE_ANIM_DATA * pTextureAnimData)
{
	QDT_DLIST<TEXTURE_ANIM_DATA *>::ITERATOR itEach = _vAnimatedTexture.GetHead();
	QDT_DLIST<TEXTURE_ANIM_DATA *>::ITERATOR itTail = _vAnimatedTexture.GetTail();

	while (itEach != itTail)
	{
		if (*itEach == pTextureAnimData)
		{
			_vAnimatedTexture.Remove(itEach);
			return;
		}

		++itEach;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateColors
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::UpdateColors()
{
	if (_vAnimatedColors.IsEmpty()) return;

	QDT_DLIST<COLOR_ANIM_DATA*>::ITERATOR itEach = _vAnimatedColors.GetHead();
	QDT_DLIST<COLOR_ANIM_DATA*>::ITERATOR itTail = _vAnimatedColors.GetTail();
	COLOR_ANIM_DATA *pColorData = NULL;

	float rDelay = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime();

	while (itEach != itTail)
	{	
		pColorData = (*itEach++);
		pColorData->UpdateBehavior(rDelay);
	}	
}

//-----------------------------------------------------------------------------
//	Name:		AddAnimatedColor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::AddAnimatedColor(COLOR_ANIM_DATA * pColorAnimData)
{
	_vAnimatedColors.PushTail(pColorAnimData);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAnimatedColor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::RemoveAnimatedColor(COLOR_ANIM_DATA * pColorAnimData)
{
	QDT_DLIST<COLOR_ANIM_DATA *>::ITERATOR itEach = _vAnimatedColors.GetHead();
	QDT_DLIST<COLOR_ANIM_DATA *>::ITERATOR itTail = _vAnimatedColors.GetTail();

	while (itEach != itTail)
	{
		if (*itEach == pColorAnimData)
		{
			_vAnimatedColors.Remove(itEach);
			return;
		}

		++itEach;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_PlayFace
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_PlayFace(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_CreateHipsAnim
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_CreateHipsAnim(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE	AnimInst;
	COM_INSTANCE	*pC;

	AnimInst = BEHAVIOR_MANAGER::Instance()->CreateHipsAnim();

	pC = (COM_INSTANCE*) pR;
	QDT_NEW_PLACED( pC, COM_INSTANCE) () ;
	*pC = AnimInst;
}

//-----------------------------------------------------------------------------
//	Name:		EM_InterpolateBlendShape
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_InterpolateBlendShape(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	struct LOCAL_PARAM
	{
		COM_INSTANCE	_Node;
		int				_nTarget;
		int				_nDuration;
		float			_rWeight;
	};
	LOCAL_PARAM		PP;

	PP = *((LOCAL_PARAM*) ( pP ));

	float rConvert = TIME::Instance()->GetTargetFPS() / 1000.0f;
	float rDuration	= static_cast<float>(PP._nDuration  * rConvert);

	ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapeInterpolation( PP._Node, PP._nTarget, PP._rWeight, rDuration, 0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_BlinkBlendShape
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_BlinkBlendShape(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	struct LOCAL_PARAM
	{
		COM_INSTANCE	_Node;
		int			_nTarget;
		int			_nDuration;
		float		_rWeight;
	};
	LOCAL_PARAM		PP;

	PP = *((LOCAL_PARAM*) ( pP ));

	float rConvert = TIME::Instance()->GetTargetFPS() / 1000.0f;
	float rDuration	= static_cast<float>(PP._nDuration  * rConvert);


	ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapeBlink( PP._Node, PP._nTarget, PP._rWeight, rDuration, 0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_PeriodicBlinkBlendShape
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_PeriodicBlinkBlendShape(void * const	pO,
													 void * const	pR,
													 void * const	pP)
{
	struct LOCAL_PARAM
	{
		COM_INSTANCE	_Node;
		int				_nTarget;
		int				_nDuration;
		int				_nPeriod;
		int				_nRandom;
		float			_rWeight;
	};
	LOCAL_PARAM		PP;

	PP = *((LOCAL_PARAM*) ( pP ));

	float rConvert = TIME::Instance()->GetTargetFPS() / 1000.0f;

	float rDuration	= static_cast<float>(PP._nDuration  * rConvert);
	float rPeriod	= static_cast<float>(PP._nPeriod	* rConvert);
	float rRandom	= static_cast<float>(PP._nRandom	* rConvert);


	ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapePeriodicBlink( PP._Node, PP._nTarget, PP._rWeight, rDuration, rPeriod, rRandom, 0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AnimateBlendShape
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_AnimateBlendShape(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	struct LOCAL_PARAM
	{
		COM_INSTANCE	_Node;
		COM_INSTANCE	_BlendShapeAnimInst;
	};

	LOCAL_PARAM	 PP;

	PP = *( (LOCAL_PARAM*) pP );
	ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapeAnimation( PP._Node, PP._BlendShapeAnimInst, false, 0.0f, 1.0f, 0.0f, 0.0f, false, 0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetBlendShapeValue
//	Object:		
//	03-03-28:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_GetBlendShapeValue(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	struct LOCAL_PARAM
	{
		COM_INSTANCE	_Node;
		int				_nTarget;
		int				_nBlendShapeID;
	};

	LOCAL_PARAM		PP;
	float			*pF;


	PP = *( (LOCAL_PARAM*) pP );

	pF = (float*) pR;
	*pF = ENTITY_BEHAVIOR_FACTORY::Instance()->GetBlendShapeValue( PP._Node, PP._nTarget, PP._nBlendShapeID );
}

//-----------------------------------------------------------------------------
//	Name:		EM_ResetAnimPool
//	Object:		
//	04-11-10:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::EM_ResetAnimPool(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	BEHAVIOR_MANAGER::Instance()->ResetAnimPool();
}



//-----------------------------------------------------------------------------
//	Name:		GetSingletonDescriptor
//	Object:		
//	03-03-31:	RMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	BEHAVIOR_MANAGER::GetSingletonDescriptor()
{
	return (SINGLETON_DESCRIPTOR::FindSingDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BEHAVIOR_MANAGER));
}

//-----------------------------------------------------------------------------
//	Name:		CreateSingletonDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	BEHAVIOR_MANAGER::CreateSingletonDescriptor()
{
	return ( new SINGLETON_DESCRIPTOR(	QDT_COMMENT("BEHAVIOR_MANAGER"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BEHAVIOR_MANAGER, 
										QDT_COMMENT("950C7D44-3839-3586-905A-85DB9CD46F04"),
										sizeof(BEHAVIOR_MANAGER), 
										COM_Init,
										QDT_COMMENT("The Behavior Manager singleton")));//	For singleton
}

//-----------------------------------------------------------------------------
//	Name:		QueryHipsAnim
//	Object:		
//	03-06-05:	CVI - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	BEHAVIOR_MANAGER::QueryHipsAnim()
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR	It;
	COM_INSTANCE						CI;

	if ( _sAnimationInsts .GetSize() <= 0 )
	{
		ReserveHipsAnims( 2 );
	}
	
	It = _sAnimationInsts.GetHead();
	CI = *It;
	_sAnimationInsts.Remove( It );
	return ( CI );
}

//-----------------------------------------------------------------------------
//	Name:		ReserveHipsAnim
//	Object:		
//	03-06-05:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::ReserveHipsAnims( int n)
{
	int				i;
	COM_INSTANCE	CI;
	COM_HANDLE		*pC;

	for ( i = 0; i < n; ++i )
	{
		pC = HIPS_ANIM::GetGenericCommunicatorDescriptor()->Create();
		CI.SetHandle( pC );
		_sAnimationInsts.InsertHead( CI );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseHipsAnims
//	Object:		
//	03-06-05:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::ReleaseHipsAnims()
{
	_sAnimationInsts.Clear();
}
	
//-----------------------------------------------------------------------------
//	Name:		ResetAnimPool
//	Object:		
//	04-11-10:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::ResetAnimPool()
{
	ReleaseHipsAnims();
	
}


//-----------------------------------------------------------------------------
//	Name:		AddFreeHipsAnim
//	Object:		
//	03-06-05:	CVI - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::AddFreeHipsAnim( const COM_INSTANCE& CI )
{
	_sAnimationInsts.InsertHead( CI );
}

//-----------------------------------------------------------------------------
//	Name:		DumpAnimatedNodes
//	Object:		
//	04-08-25:	DAC - Created
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER::DumpAnimatedNodes(const QDT_STRING &	sPad,
											QDT_STRING &		sDump) const
{
	QDT_STRING	sTmp;
	QDT_DLIST <COM_INSTANCE>::ITERATOR it	  = _vAnimatedNodes.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = _vAnimatedNodes.GetTail();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itNext;
	COM_INSTANCE NodeCI;
	// Warning : update behavior can remove a node from the list
	// That's why I use itNext to make sure it is valid

	//	Dumping movies info
	sDump += sPad;
	sTmp.Format(" -> Anims(%i):\n", _vAnimatedNodes.GetSize());
	sDump += sTmp;

	while (it != itTail)
	{
		itNext = it + 1;
		QDT_ASSERT((*it).HasHandle());
		NodeCI = *it;
		NODE * pNode = static_cast<NODE*>(NodeCI.GetCommunicator());
		
		sDump += sPad;
		ANIM_NODE_STRUCT * pANS;
		pANS = pNode->GetContainer()->GetAnimNodeStruct();

		sTmp.Format("A: %s / N: %s (%s)\n",	"AnimNodeStruct",
										pNode->GetName().GetBuffer(), 
										pNode->IsPlaying()?"Playing":"Stopped");
		
		sDump += sTmp;

		it = itNext;
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
