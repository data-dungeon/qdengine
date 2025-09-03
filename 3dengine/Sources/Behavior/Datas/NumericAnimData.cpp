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
//	CLASS:	NUMERIC_ANIM_DATA
//
//	03-11-06:	FPR - Created
//*****************************************************************************

#include	"Root.h"
#include	"NumericAnimData.h"

#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		NUMERIC_ANIM_DATA constructor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
NUMERIC_ANIM_DATA::NUMERIC_ANIM_DATA()
:
COMMUNICATOR			(GetGenericCommunicatorDescriptor()),
_rTime					(0.0f),
_nInterpolationPower	(0),
_nNbrComInstance		(0),
_bLoop					(true),
_bUpdated				(false),
_bActive				(true),
_bReverseMode			(false)
{
	_vComInstance.Clear();
	_AttributeName.Clear();
	_AttributeIndex.Clear();
	_NumTrack.ClearKeys();
	_NumTrack._nLastIndiceUsed = 0;
}

//-----------------------------------------------------------------------------
//	Name:		NUMERIC_ANIM_DATA destructor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
NUMERIC_ANIM_DATA::~NUMERIC_ANIM_DATA()
{
	if (_bUpdated)
	{
		BEHAVIOR_MANAGER::Instance()->RemoveAnimatedNumericAttributes(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetLength
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
float	NUMERIC_ANIM_DATA::GetLength() const
{
	return (MATH::Max(0.0f, _NumTrack.GetLength()));
}

//-----------------------------------------------------------------------------
//	Name:		GetDiffuseColor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
float	NUMERIC_ANIM_DATA::GetNumericValue(float	rTime) const
{
	float C = 0.0f;

	switch(_nInterpolationPower)
	{
		default:
			QDT_FAIL();
			break;

		case 0:
			C = _NumTrack.GetPureValue(rTime);
			break;

		case 1:
			C = _NumTrack.GetLinearValue(rTime);
			break;

		case 2:
			{
				if (_NumTrack.GetNbrKeys() > 2 )
				{
					C = _NumTrack.GetBezierValue(rTime);
				}
				else
				{
					C = _NumTrack.GetLinearValue(rTime);
				}	
			}
			break;

		case 3:
			{
				if (_NumTrack.GetNbrKeys() == 2 )
				{
					C = _NumTrack.GetRandomValue(rTime);
				}
				else
				{
					C = _NumTrack.GetPureValue(rTime);
				}
			}
			break;
	}

	return (C);
}

//-----------------------------------------------------------------------------
//	Name:		AddObjectAndAttribute
//	Object:		
//	03-12-10:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::AddComInstanceAndAttribute(const COM_INSTANCE &	CI,
												 QDT_STRING				string)
{
	_vComInstance.PushTail(CI);
	_AttributeName.PushTail(string);
	_nNbrComInstance++;

	// Get The descriptor to set the index value
	// PRELIT MATERIAL? it worked....
	CLASS_DESCRIPTOR* pCD = CI.GetCommunicator()->GetClassDescriptor();
	_AttributeIndex.PushTail(pCD->FindAttributeIndex(string));
}

//-----------------------------------------------------------------------------
//	Name:		UpdateBehavior
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::UpdateBehavior(float	rDeltaFrame)
{
	if (_bActive)
	{
		int nEachComInstance;
		
		if (_bReverseMode)
		{
			_rTime -= rDeltaFrame;
		}
		else
		{
			_rTime += rDeltaFrame;
		}

		float num = GetNumericValue(_rTime);
		
		for(nEachComInstance=0; nEachComInstance<_nNbrComInstance ; ++nEachComInstance)
		{
			if ( _vComInstance[nEachComInstance].HasHandle())
			{
				CLASS_DESCRIPTOR* pCD = _vComInstance[nEachComInstance].GetCommunicator()->GetClassDescriptor();
				pCD->CallAttributeSet(_AttributeIndex[nEachComInstance], &_vComInstance[nEachComInstance], &num);
			}
		}
		
		if (_bReverseMode)
		{
			if (_rTime <= 0.0f)
			{
				_rTime = GetLength();

				if (!_bLoop)
				{
					_bActive = false;
				}
			}
		}
		else
		{
			if (_rTime >= GetLength())
			{
				_rTime = 0.0f;

				if (!_bLoop)
				{
					_bActive = false;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NUMERIC_ANIM_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NUMERIC_ANIM_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NUMERIC_ANIM_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("NUMERIC_ANIM_DATA"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NUMERIC_ANIM_DATA, 
											QDT_COMMENT("056D9221-83A2-584B-8ECB-BDD8E90DAE59"), 
											sizeof(NUMERIC_ANIM_DATA), 
											Init, 
											QDT_COMMENT("This represents the datas used to animate numeric attributes. It consists in one track. ")));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR * pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	pMD = METHOD_DESCRIPTOR__Allocate("Play", "Play the animation");
	pCD->AddMethod(CMD_PLAY, pMD, EM_Play);

	pMD = METHOD_DESCRIPTOR__Allocate("Stop", "Stop the animation");
	pCD->AddMethod(CMD_STOP, pMD, EM_Stop);

	pMD = METHOD_DESCRIPTOR__Allocate("PlayReverse", "Play the animation in reverse mode");
	pCD->AddMethod(CMD_PLAY_REVERSE, pMD, EM_PlayReverse);

	pMD = METHOD_DESCRIPTOR__Allocate("SetFirstFrame", "Set the animation to its first frame");
	pCD->AddMethod(CMD_SET_FIRST_FRAME, pMD, EM_SetFirstFrame);

	pMD = METHOD_DESCRIPTOR__Allocate("SetLastFrame", "Set the animation to its last frame");
	pCD->AddMethod(CMD_SET_LAST_FRAME, pMD, EM_SetLastFrame);

	pMD = METHOD_DESCRIPTOR__Allocate("Pause", "Pause the animation");
	pCD->AddMethod(CMD_PAUSE, pMD, EM_Pause);

	pMD = METHOD_DESCRIPTOR__Allocate("Resume", "Resume the animation");
	pCD->AddMethod(CMD_RESUME, pMD, EM_Resume);

	pMD = METHOD_DESCRIPTOR__Allocate("IsPlaying", "Is the animation playing");
	pMD->SetReturn(QDT_COMMENT("bIsPlaynig"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_PLAYING, pMD, EM_IsPlaying);

	pMD = METHOD_DESCRIPTOR__Allocate("SetLoop", "");
	pMD->AddParameter(QDT_COMMENT("bLoop"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_LOOP, pMD, EM_SetLoop);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("NUMERICANIMINFOS", 0, CM_ReadChunkNumAnimInfos0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("NUMERICANIMINFOS", 1, CM_ReadChunkNumAnimInfos1);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("NUMERICANIMINFOS", 1, CM_WriteChunkNumAnimInfos1);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_Construct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	NUMERIC_ANIM_DATA *	pNumAnimData;
	pNumAnimData = static_cast<NUMERIC_ANIM_DATA *>(pO);
	QDT_NEW_PLACED(pNumAnimData, NUMERIC_ANIM_DATA)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-01-21:	VMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_Construct_In_Memory(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_Destruct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	NUMERIC_ANIM_DATA *	pNumAnimData;
	pNumAnimData = static_cast<NUMERIC_ANIM_DATA*> (pO);
	pNumAnimData->NUMERIC_ANIM_DATA::~NUMERIC_ANIM_DATA();
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::CM_ReadChunkNumAnimInfos0(IO_STREAM &		Stream,
													 COMMUNICATOR *	pCommunicator)
{
	NUMERIC_ANIM_DATA	*pNumAnimData;
 	pNumAnimData = static_cast<NUMERIC_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float num;
		float rTime;
		Stream >> num;
		Stream >> rTime;

		GENERIC_KEY <float> key;
		key.SetTime(rTime);
		key.SetValue(num);
		pNumAnimData->_NumTrack.PushKey(key);
	}

	Stream >> pNumAnimData->_nNbrComInstance;
	int nEach;

	for(nEach=0; nEach<pNumAnimData->_nNbrComInstance ; ++nEach)
	{
		COM_INSTANCE	tmpMat;
		QDT_STRING		tmpAttName;
		int				nTmpAttIndex;

		Stream >> tmpMat;
		Stream >> tmpAttName;
		Stream >> nTmpAttIndex;

		pNumAnimData->_vComInstance.PushTail(tmpMat);
		pNumAnimData->_AttributeName.PushTail(tmpAttName);
		pNumAnimData->_AttributeIndex.PushTail(nTmpAttIndex);
	}

	Stream >> pNumAnimData->_nInterpolationPower;
	Stream >> pNumAnimData->_bLoop;

	BEHAVIOR_MANAGER::Instance()->AddAnimatedNumericAttributes(pNumAnimData);
	pNumAnimData->_bUpdated = true;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::CM_ReadChunkNumAnimInfos1(IO_STREAM &		Stream,
													 COMMUNICATOR *	pCommunicator)
{
	NUMERIC_ANIM_DATA	*pNumAnimData;
 	pNumAnimData = static_cast<NUMERIC_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		float num;
		float rTime;
		Stream >> num;
		Stream >> rTime;

		GENERIC_KEY <float> key;
		key.SetTime(rTime);
		key.SetValue(num);
		pNumAnimData->_NumTrack.PushKey(key);
	}

	Stream >> pNumAnimData->_nNbrComInstance;
	int nEach;

	for(nEach=0; nEach<pNumAnimData->_nNbrComInstance ; ++nEach)
	{
		COM_INSTANCE	tmpMat;
		QDT_STRING		tmpAttName;
		int				nTmpAttIndex;

		Stream >> tmpMat;
		Stream >> tmpAttName;
		Stream >> nTmpAttIndex;

		pNumAnimData->_vComInstance.PushTail(tmpMat);
		pNumAnimData->_AttributeName.PushTail(tmpAttName);
		pNumAnimData->_AttributeIndex.PushTail(nTmpAttIndex);
	}

	Stream >> pNumAnimData->_nInterpolationPower;
	Stream >> pNumAnimData->_bLoop;
	Stream >> pNumAnimData->_bActive;

	BEHAVIOR_MANAGER::Instance()->AddAnimatedNumericAttributes(pNumAnimData);
	pNumAnimData->_bUpdated = true;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunkMatInfos
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NUMERIC_ANIM_DATA::CM_WriteChunkNumAnimInfos1(IO_STREAM &		Stream,
													  COMMUNICATOR *	pCommunicator)
{
	NUMERIC_ANIM_DATA	*pNumAnim;
 	pNumAnim = static_cast<NUMERIC_ANIM_DATA *>(pCommunicator);

	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("NUMERIC_ANIM_DATA : ");
	Stream.DebugOutput(pNumAnim->GetName());
	Stream.DebugOutput("\r\n");

	int nNbrKeys = pNumAnim->_NumTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<float>(pNumAnim->_NumTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pNumAnim->_NumTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}
	Stream.DebugOutput("Nbr Object Attached : ");
	Stream << pNumAnim->_nNbrComInstance;
	Stream.DebugOutput("\r\n");

	int nEach;
	for(nEach = 0; nEach<pNumAnim->_nNbrComInstance ; ++nEach)
	{
		Stream.DebugOutput("Com Inst : ");
		Stream << pNumAnim->_vComInstance[nEach];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Attribute Name : ");
		Stream << pNumAnim->_AttributeName[nEach];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Attribute Index : ");
		Stream << pNumAnim->_AttributeIndex[nEach];
		Stream.DebugOutput("\r\n");

	}
	
	Stream.DebugOutput("Interpolation : ");
	Stream << pNumAnim->_nInterpolationPower;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Loop : ");
	Stream << pNumAnim->_bLoop;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Active : ");
	Stream << pNumAnim->_bActive;
	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Play
//	Object:		
//	04-02-18:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::Play()
{
	_bReverseMode = false;
	_bActive = true;
	_rTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		Pause
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::Pause()
{
	_bActive = false;
}
		
//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::Resume()
{
	_bActive = true;
}

//-----------------------------------------------------------------------------
//	Name:		PlayReverse
//	Object:		
//	04-05-04:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::PlayReverse()
{
	_bReverseMode = true;
	_bActive = true;
	_rTime = GetLength();
}

//-----------------------------------------------------------------------------
//	Name:		Stop
//	Object:		
//	04-02-18:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::Stop()
{
	_bActive = false;
	_rTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Play
//	Object:		
//	04-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_Play(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Play();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Stop
//	Object:		
//	04-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_Stop(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Stop();
}

//-----------------------------------------------------------------------------
//	Name:		EM_PlayReverse
//	Object:		
//	04-05-04:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_PlayReverse(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->PlayReverse();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFirstFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_SetFirstFrame(void * const pO, 
											void * const pR, 
											void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->SetFrame(0.0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFirstFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::EM_SetLastFrame(void * const pO, 
										   void * const pR, 
										   void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->SetFrame(pAnimData->GetLength());
}

//-----------------------------------------------------------------------------
//	Name:		SetFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NUMERIC_ANIM_DATA::SetFrame(float rFrame)
{
	if (_bActive == false)
	{
		_bActive = true;
		_rTime = 0.0f;

		UpdateBehavior(rFrame);

		_bActive = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_Pause
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void NUMERIC_ANIM_DATA::EM_Pause(void * const pO, 
								 void * const pR, 
								 void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Pause();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Resume
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void NUMERIC_ANIM_DATA::EM_Resume(void * const pO, 
								  void * const pR, 
								  void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData	  = static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Resume();
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsPlaying
//	Object:		
//	04-12-20:	RMA - Created
//-----------------------------------------------------------------------------
void NUMERIC_ANIM_DATA::EM_IsPlaying(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	COM_INSTANCE *		pAnimDataInst	= static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData		= static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	bool *				pReturn			= static_cast<bool *>(pR);
	
	*pReturn = pAnimData->IsPlaying();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLoop
//	Object:		
//	04-12-20:	RMA - Created
//-----------------------------------------------------------------------------
void NUMERIC_ANIM_DATA::EM_SetLoop(void * const pO, 
								   void * const pR,
								   void * const pP)
{
	COM_INSTANCE *		pAnimDataInst	= static_cast<COM_INSTANCE*>( pO );
	NUMERIC_ANIM_DATA *	pAnimData		= static_cast<NUMERIC_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	bool *				pParam			= static_cast<bool *>(pP);
	
	pAnimData->SetLoop(*pParam);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
