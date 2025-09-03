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
//	CLASS:	MOTEUR_3D
//
//	02-07-10:	ONA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3D)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/Viewport/ViewportList)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementText)
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KCOM(Services/LoadingScreenService)
#include	INCL_KCOM(BaseType/TypeString)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCORE(Profiler/Profiler)
#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(Stream/FileStream)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Node/NodeManager)

#ifdef _DEBUG
	#include "Moteur3D.inl"
#endif //_DEBUG

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MOTEUR_3D constructor
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
MOTEUR_3D::MOTEUR_3D()
:
_bEnableLoop(true)
{
	// Initialize sub
	DISPLAY_MANAGER::Instance();
	BEHAVIOR_MANAGER::Instance();
	INTERFACE_MANAGER::Instance();
	PHYSIC_MANAGER::Instance();

#ifdef PLATFORM_PC
	PHYSIC_CONTEXT::Create(NOVODEX_PHYSICS_SDK); 
#else
	PHYSIC_CONTEXT::Create(FAKE_PHYSICS_SDK); 
#endif
	PHYSIC_CONTEXT::Instance()->SetMeterSize(100.0f); // 1 = 1cm for our graphic team so 100 = 1m
	PHYSIC_CONTEXT::Instance()->SetSkinWidth(0.02f); // in meters
	PHYSIC_CONTEXT::Instance()->SetDefaultSleepLinVelocity(0.3f * 0.3f);
	PHYSIC_CONTEXT::Instance()->SetDefaultSleepAngVelocity(0.3f * 0.3f);
	PHYSIC_CONTEXT::Instance()->SetBounceTreshold(-2.0f);

	_vNodes.Reserve(512);
	_vGlobalMatrices.Reserve(512);
	_vParents.Reserve(512);
}

//-----------------------------------------------------------------------------
//	Name:		~MOTEUR_3D destructor
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
MOTEUR_3D::~MOTEUR_3D()
{
	BEHAVIOR_MANAGER::Suicide();
	PHYSIC_MANAGER::Suicide();

	//	Remove all scenegraph
	while (_SceneGraphsList.IsEmpty() == false)
	{
		SCENE_GRAPH * pSG = *(_SceneGraphsList.GetHead());
		RemoveSceneGraph(pSG);
	}

	VIEWPORT_LIST * pList = DISPLAY_MANAGER::Instance()->GetViewportList();
	if (pList)
	{
		pList->Clear();
	}

	DISPLAY_MANAGER::Instance()->ClearIhmElements();
	DISPLAY_MANAGER::Instance()->ClearIHMGeometries();

	DISPLAY_MANAGER::Release();
	INTERFACE_MANAGER::Suicide();
	NODE_MANAGER::Suicide();
	PHYSIC_CONTEXT::Release();
}

//-----------------------------------------------------------------------------
//	Name:		OpenTools
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::OpenTools()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CloseTools
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::CloseTools()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AddIhmElement
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::AddIhmElement(const COM_INSTANCE &	Element,
								 bool					bUnderInterface)
{
	DISPLAY_MANAGER::Instance()->AddIhmElement(Element, bUnderInterface);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIhmElement
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::RemoveIhmElement(const COM_INSTANCE &	Element)
{
	DISPLAY_MANAGER::Instance()->RemoveIhmElement(Element);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveIhmElement
//	Object:		
//	04-04-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::ClearIhmElements()
{
	DISPLAY_MANAGER::Instance()->ClearIhmElements();
}

//-----------------------------------------------------------------------------
//	Name:		EnableLoop
//	Object:		
//	04-02-06:	VPI - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EnableLoop(const bool	bEnableLoop)
{
		_bEnableLoop = bEnableLoop;
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::Loop()
{
	BEGIN_PROFILE(PROFILE_MOTEUR3D_LOOP);

	if (_bEnableLoop)
	{
#ifndef _MASTER
#ifdef PLATFORM_XBOX
		RENDER_MANAGER::Instance()->GetRenderContext()->BeginPixEvent("Engine loop", COLOR(255, 255, 255, 0));
#endif
#endif

		float	rSpeedFactor = TIME::Instance()->GetDefaultClock()->GetSpeedFactor();

		BEGIN_PROFILE(PROFILE_UPDATE_BEHAVIOR);
		BEHAVIOR_MANAGER::Instance()->UpdateAnimation(rSpeedFactor);
		END_PROFILE(PROFILE_UPDATE_BEHAVIOR);

		// Update particle emitters
/*		QDT_DLIST <COM_INSTANCE>::ITERATOR itEach = NODE_MANAGER::Instance()->GetParticleEmitters().GetHead();
		QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = NODE_MANAGER::Instance()->GetParticleEmitters().GetTail();
		
		NODE	*pPENode = NULL;

		while (itEach != itTail)
		{
			if ((*itEach).IsLoaded())
			{
				pPENode = static_cast <NODE *>((*itEach).GetCommunicator());
				pPENode->SetNeedUpdateParticleEmitter();
			}
			++itEach;
		}*/

		INTERFACE_MANAGER::Instance()->Loop(rSpeedFactor);
		PHYSIC_MANAGER::Instance()->Loop();

		#ifndef _MASTER
		#ifdef PLATFORM_XBOX
			RENDER_MANAGER::Instance()->GetRenderContext()->EndPixEvent();
		#endif
		#endif
	}

	END_PROFILE(PROFILE_MOTEUR3D_LOOP);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::Display()
{
	BEGIN_PROFILE(PROFILE_MOTEUR3D_DISPLAY);

	LOADING_SCREEN_SERVICE::Instance()->UpdateDisplay();

	if	(RENDER_CONTEXT::IsDisplayWindowActive())
	{			
		RENDER_CONTEXT::BeginScene();
		DISPLAY_MANAGER::Instance()->Display();
		RENDER_CONTEXT::EndScene();

		RENDER_CONTEXT::Present();
	}

	END_PROFILE(PROFILE_MOTEUR3D_DISPLAY);
}

//-----------------------------------------------------------------------------
//	Name:		AddSceneGraph
//	Object:		Add a scene graph to the list of scene graphs
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::AddSceneGraph(SCENE_GRAPH * pSG)
{
	_SceneGraphsList.InsertTail(pSG);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSceneGraph
//	Object:		Remove a scene graph from the list of scene graphs
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::RemoveSceneGraph(SCENE_GRAPH * pSG)
{
	QDT_DLIST<SCENE_GRAPH *>::ITERATOR It = _SceneGraphsList.GetHead() ;
	QDT_DLIST<SCENE_GRAPH *>::ITERATOR ItEnd = _SceneGraphsList.GetTail() ;

	bool bFound = false;

	while (It != ItEnd)
	{
		if (*It == pSG)
		{
			//	Remove connections between scene graph nodes
/*			QDT_VECTOR<COM_INSTANCE> vNodes;	
			pSGNode->BuildNodeArrayRec(vNodes); // -> Save COM_INSTANCE
			pSGNode->BreakConnectionsRec();		// No destruction can occur
			vNodes.Clear();						// Destruction can occur*/

			//	Then remove the SG root from the engine
			_SceneGraphsList.Remove(It) ;
			
			bFound = true ;
			break;
		}
		++It ;	
	}

	QDT_ASSERT(bFound) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetSceneGraphsList
//	Object:		Get the list of scene_graphs
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_DLIST<SCENE_GRAPH *> &	MOTEUR_3D::GetSceneGraphsList() const
{
	return (_SceneGraphsList);
}

//-----------------------------------------------------------------------------
//	Name:		DumpSceneGraph
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	MOTEUR_3D::DumpSceneGraph(const QDT_STRING &	sFileName)
{
	QDT_NOT_IMPLEMENTED();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Pause
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::Pause()
{
	TIME::Instance()->GetDefaultClock()->Pause();
}

//-----------------------------------------------------------------------------
//	Name:		Continue
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::Continue()
{
	TIME::Instance()->GetDefaultClock()->Continue();
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetLoopMethod(EM_Loop);
	pCD->SetDisplayMethod(EM_Display);
	pCD->SetPauseMethodCmds(CMD_PAUSE, CMD_RESUME, CMD_INVALID);

	pM = METHOD_DESCRIPTOR::Allocate("AddIhmElement", "Ask 3D Engine to document this command.",ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "IhmElement", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_M3D_ADD_IHM_ELEMENT, pM, CM_AddIhmElement, EM_AddIhmElement);

	pM = METHOD_DESCRIPTOR::Allocate("RemoveIhmElement", "",ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "IhmElement", "", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_M3D_REMOVE_IHM_ELEMENT, pM, CM_RemoveIhmElement, EM_RemoveIhmElement);

#ifndef _MASTER
	pM = METHOD_DESCRIPTOR__Allocate("DumpSceneGraph", "");
	METHOD_DESCRIPTOR__AddParameter(pM, "FileName", "", TYPE_STRING::GetDescriptor());
	pCD->AddMethod(CMD_M3D_DUMP_SCENE_GRAPH, pM, EM_DumpSceneGraph);
#else
	pCD->AddMethod(CMD_M3D_DUMP_SCENE_GRAPH, CLASS_DESCRIPTOR::_pDoNothingMD, NULL);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("Pause", "Pause the 3DEngine");
	pCD->AddMethod(CMD_PAUSE, pM, EM_Pause);

	pM = METHOD_DESCRIPTOR__Allocate("Resume", "Resume the 3DEngine");
	pCD->AddMethod(CMD_RESUME, pM, EM_Resume);

	pM = METHOD_DESCRIPTOR__Allocate("ClearIhmElements", "");
	pCD->AddMethod(CMD_CLEAR_IHM_ELEMENT, pM, EM_ClearIhmElements);

	pM = METHOD_DESCRIPTOR__Allocate("AddIhmElementEx", "Add ihm to the engine itself rather then on a specific viewport");
	METHOD_DESCRIPTOR__AddParameter(pM, "IhmElement", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "bUnderInterface", "Allows to push the Ihm above any other IHM attached to viewports", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_M3D_ADD_IHM_ELEMENT_EX, pM, EM_AddIhmElementEx);

	pM = METHOD_DESCRIPTOR__Allocate("SaveState", "");
	pCD->AddMethod(CMD_SAVE_STATE, pM, EM_SaveState);

	pM = METHOD_DESCRIPTOR__Allocate("RestoreState", "");
	pCD->AddMethod(CMD_RESTORE_STATE, pM, EM_RestoreState);

	pM = METHOD_DESCRIPTOR__Allocate("ClearState", "");
	pCD->AddMethod(CMD_CLEAR_STATE, pM, EM_ClearState);

	//	Reset engine 
	pM = METHOD_DESCRIPTOR__Allocate("Reset3DEngine", "");
	pCD->AddMethod(CMD_RESET_ENGINE, pM, EM_ResetEngine);

	pM = METHOD_DESCRIPTOR__Allocate("SaveDebugPhysicsFile", "");
	METHOD_DESCRIPTOR__AddParameter(pM, "FileName", "", TYPE_STRING::GetDescriptor());
	pCD->AddMethod(CMD_SAVE_DEBUG_PHYSICS_FILE, pM, EM_SaveDebugPhysicsFile);

	pM = METHOD_DESCRIPTOR__Allocate("AddVideoIhmElement", "Add video ihm to the engine");
	METHOD_DESCRIPTOR__AddParameter(pM, "IhmElement", "", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_ADD_VIDEO_IHM_ELEMENT, pM, EM_AddVideoIhmElement);

	pM = METHOD_DESCRIPTOR__Allocate("RemoveVideoIhmElement", "Remove video ihm of the engine");
	METHOD_DESCRIPTOR__AddParameter(pM, "IhmElement", "", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_REMOVE_VIDEO_IHM_ELEMENT, pM, EM_RemoveVideoIhmElement);

	pM = METHOD_DESCRIPTOR__Allocate("ClearVideoIhmElement", "Clear video ihm of the engine");
	pCD->AddMethod(CMD_CLEAR_VIDEO_IHM_ELEMENT, pM, EM_ClearVideoIhmElement);

	pM = METHOD_DESCRIPTOR__Allocate("AddFindAndReplace", "This methods concerns all ihm element text.");
	METHOD_DESCRIPTOR__AddParameter(pM, "sFind", "", TYPE_STRING::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "sReplace", "", TYPE_STRING::GetDescriptor());
	pCD->AddMethod(CMD_ADD_FIND_AND_REPLACE, pM, EM_AddFindAndReplace);

	pM = METHOD_DESCRIPTOR__Allocate("ClearFindAndReplace", "This methods concerns all ihm element text.");
	pCD->AddMethod(CMD_CLEAR_FIND_AND_REPLACE, pM, EM_ClearFindAndReplace);

	//	Now register the class as a global variable
	GLOBAL_VARIABLE::AddVariable(101, "MOTEUR_3D", pCD, CreateSingletonDescriptor()->GetSingletonPointer());
}

//-----------------------------------------------------------------------------
//	Name:		GetSingletonDescriptor
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	MOTEUR_3D::GetSingletonDescriptor()
{
	return	(SINGLETON_DESCRIPTOR::FindSingDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D));
}

//-----------------------------------------------------------------------------
//	Name:		CreateSingletonDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	MOTEUR_3D::CreateSingletonDescriptor()
{
	return (new SINGLETON_DESCRIPTOR(	QDT_COMMENT("MOTEUR_3D"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D,
										QDT_COMMENT("F331D8D5-7F86-F49F-09CF-5CCCDC3164ED"),
										sizeof(MOTEUR_3D),
										Init,
										QDT_COMMENT("The 3D engine singleton"),
										NULL,
										ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL));
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddIhmElement
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_AddIhmElement(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	QDT_ASSERT(pR == NULL);
	
	COM_INSTANCE *	pElement = (COM_INSTANCE *)pP;
	
	MOTEUR_3D::Instance()->AddIhmElement(*pElement);
}

//-----------------------------------------------------------------------------
//	Name:		CM_AddIhmElementEx
//	Object:		
//	06-03-20:	VHA - Created
//-----------------------------------------------------------------------------
int	MOTEUR_3D::CM_AddIhmElement(COM_PARAM	P)
{
	COM_STACK		Stack(P, GetSingletonDescriptor(), CMD_M3D_ADD_IHM_ELEMENT);
	COM_INSTANCE	ihmElementCI;
	
	Stack.GetParameter(0, ihmElementCI);
	
	MOTEUR_3D::Instance()->AddIhmElement(ihmElementCI);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearIhmElements
//	Object:		
//	04-04-07:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::EM_ClearIhmElements(void * const pO, 
									void * const pR, 
									void * const pP)
{
	MOTEUR_3D::Instance()->ClearIhmElements();
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveIhmElement
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_RemoveIhmElement(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	QDT_ASSERT(pR == NULL);

	COM_INSTANCE *	pElement = (COM_INSTANCE *)pP;

	MOTEUR_3D::Instance()->RemoveIhmElement(*pElement);
}

//-----------------------------------------------------------------------------
//	Name:		CM_RemoveIhmElement
//	Object:		
//	06-03-20:	VHA - Created
//-----------------------------------------------------------------------------
int	MOTEUR_3D::CM_RemoveIhmElement(COM_PARAM	P)
{
	COM_STACK		Stack(P, GetSingletonDescriptor(), CMD_M3D_REMOVE_IHM_ELEMENT);
	COM_INSTANCE	ihmElementCI;
	
	Stack.GetParameter(0, ihmElementCI);
	
	MOTEUR_3D::Instance()->RemoveIhmElement(ihmElementCI);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}


//-----------------------------------------------------------------------------
//	Name:		EM_DumpSceneGraph
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	MOTEUR_3D::EM_DumpSceneGraph(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	QDT_ASSERT(pR == NULL);
	QDT_STRING *	pString;

	pString = (QDT_STRING *)pP;

	MOTEUR_3D::Instance()->DumpSceneGraph(*pString);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	MOTEUR_3D *	pM;
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);

	pM = (MOTEUR_3D *)pO;
	QDT_NEW_PLACED(pM, MOTEUR_3D)();

	MODULE_ABC::AddClassDescriptorLoop(MOTEUR_3D_MODULE::ENGINE_LOOP, GetSingletonDescriptor(), NULL);
	MODULE_ABC::AddClassDescriptorDisplay(MOTEUR_3D_MODULE::ENGINE_DISPLAY, GetSingletonDescriptor(), NULL);
	MODULE_ABC::AddClassDescriptorPause(GetSingletonDescriptor());
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-07-10:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	MOTEUR_3D *	pM;
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);

	pM = (MOTEUR_3D *)pO;
	QDT_DELETE_PLACED(pM, MOTEUR_3D);

	MODULE_ABC::RemoveClassDescriptorLoop(pM);
	MODULE_ABC::RemoveClassDescriptorDisplay(pM);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Loop
//	Object:		
//	02-08-06:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_Loop(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);

	MOTEUR_3D::Instance()->Loop();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Pause
//	Object:		
//	04-03-08:	DAC - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_Pause(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);
	
	MOTEUR_3D::Instance()->Pause();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Resume
//	Object:		
//	04-03-08:	DAC - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_Resume(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);
	
	MOTEUR_3D::Instance()->Continue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Display
//	Object:		
//	02-08-07:	ONA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_Display(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	QDT_ASSERT(pR == NULL);
	QDT_ASSERT(pP == NULL);

	MOTEUR_3D::Instance()->Display();
}


//-----------------------------------------------------------------------------
//	Name:		EM_AddIhmElementEx
//	Object:		
//	04-05-07:	DAC - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_AddIhmElementEx(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	struct PARAMS
	{
		COM_INSTANCE	_CI;
		bool			_bUnderInterface;
	};

	QDT_ASSERT(pR == NULL);

	PARAMS * pParams;
	pParams = static_cast<PARAMS*>(pP);

	MOTEUR_3D::Instance()->AddIhmElement(pParams->_CI, pParams->_bUnderInterface);
}

//-----------------------------------------------------------------------------
//	Name:		SaveState
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::SaveState()
{
	QDT_NOT_IMPLEMENTED();
/*	ClearState();

	QDT_DLIST <COM_INSTANCE>::ITERATOR itEach = _SceneGraphsList.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = _SceneGraphsList.GetTail();

	while (itEach != itTail)
	{
		NODE * pSGNode = (NODE *)(*itEach).GetCommunicator();
		pSGNode->SaveState(_vNodes, _vGlobalMatrices, _vParents);
		++itEach;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		RestoreState
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::RestoreState()
{
	QDT_NOT_IMPLEMENTED();
	/*
	unsigned int i, nCount = _vNodes.GetSize();
	for (i = 0 ; i < nCount ; ++i)
	{
		COM_INSTANCE & ciNode =_vNodes[i];
		if (ciNode.IsLoaded())
		{
			NODE * pNode = static_cast<NODE *>(ciNode.GetCommunicator());
			pNode->RemoveContainer();
			if (_vParents[i].HasHandle() && _vParents[i].IsLoaded())
			{
				NODE * pParent = static_cast<NODE *>(_vParents[i].GetCommunicator());
				QDT_ASSERT(pParent);

				pParent->AttachChild(ciNode);
			}
			pNode->SetGlobalMatrix(_vGlobalMatrices[i]);
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		ClearState
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::ClearState()
{
	_vNodes.Clear();
	_vGlobalMatrices.Clear();
	_vParents.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SaveState
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::EM_SaveState(void * const pO, 
							 void * const pR, 
							 void * const pP)
{
	Instance()->SaveState();
}

//-----------------------------------------------------------------------------
//	Name:		EM_RestoreState
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::EM_RestoreState(void * const pO, 
								void * const pR, 
								void * const pP)
{
	Instance()->RestoreState();
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearState
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void MOTEUR_3D::EM_ClearState(void * const pO, 
							  void * const pR, 
							  void * const pP)
{
	Instance()->ClearState();
}

//-----------------------------------------------------------------------------
//	Name:		EM_ResetEngine
//	Object:		
//	04-11-10:	CVI - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_ResetEngine(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	Instance()->ResetEngine();
}


//-----------------------------------------------------------------------------
//	Name:		ResetEngine
//	Object:		
//	04-11-10:	CVI - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::ResetEngine()
{
	//	Add here any reset that may be called in reset cases ( back to menu, etc... )
	BEHAVIOR_MANAGER::Instance()->ResetAnimPool();
}

//-----------------------------------------------------------------------------
//	Name:		SaveDebugPhysicsFile
//	Object:		
//	05-02-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::SaveDebugPhysicsFile(const QDT_STRING & sFileName) const
{
	PHYSIC_CONTEXT::Instance()->WriteDebugFile(sFileName);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SaveDebugPhysicsFile
//	Object:		
//	05-02-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_SaveDebugPhysicsFile(void * const pO, 
										   void * const pR, 
										   void * const pP)
{
	QDT_ASSERT(pR == NULL);
	QDT_STRING * pString = (QDT_STRING *)pP;

	MOTEUR_3D::Instance()->SaveDebugPhysicsFile(*pString);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddVideoIhmElement
//	Object:		
//	05-03-11:	ELE - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_AddVideoIhmElement(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE	*pIHMInst = static_cast<COM_INSTANCE *>(pP);

	DISPLAY_MANAGER::Instance()->AddVideoIhmElement(*pIHMInst);
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveVideoIhmElement
//	Object:		
//	05-03-11:	ELE - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_RemoveVideoIhmElement(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE	*pIHMInst = static_cast<COM_INSTANCE *>(pP);

	DISPLAY_MANAGER::Instance()->RemoveVideoIhmElement(*pIHMInst);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearVideoIhmElement
//	Object:		
//	05-03-11:	ELE - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_ClearVideoIhmElement(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	DISPLAY_MANAGER::Instance()->ClearVideoIhmElements();
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddFindAndReplace
//	Object:		
//	05-03-21:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_AddFindAndReplace(void * const pO, 
										void * const pR, 
										void * const pP)
{
	struct PARAMETER
	{
		QDT_STRING _Find;
		QDT_STRING _Replace;
	};

	PARAMETER * pParam = static_cast<PARAMETER *>(pP);

	IHM_ELEMENT_TEXT::AddFindAndReplace(pParam->_Find, pParam->_Replace);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearFindAndReplace
//	Object:		
//	05-03-21:	RMA - Created
//-----------------------------------------------------------------------------
void	MOTEUR_3D::EM_ClearFindAndReplace(void * const pO, 
										  void * const pR, 
										  void * const pP)
{
	IHM_ELEMENT_TEXT::ClearFindAndReplace();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
