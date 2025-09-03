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
//	CLASS:	COLOR_ANIM_DATA
//
//	03-11-06:	FPR - Created
//*****************************************************************************

#include	"Root.h"
#include	"ColorAnimData.h"

#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		COLOR_ANIM_DATA constructor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
COLOR_ANIM_DATA::COLOR_ANIM_DATA()
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
	_ColorTrack.ClearKeys();
	_ColorTrack._nLastIndiceUsed = 0;
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_ANIM_DATA destructor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
COLOR_ANIM_DATA::~COLOR_ANIM_DATA()
{
	if (_bUpdated)
	{
		BEHAVIOR_MANAGER::Instance()->RemoveAnimatedColor(this);
		_bUpdated = false;
	}		
}

//-----------------------------------------------------------------------------
//	Name:		GetLength
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
float	COLOR_ANIM_DATA::GetLength() const
{
	return (MATH::Max(0.0f, _ColorTrack.GetLength()));
}

//-----------------------------------------------------------------------------
//	Name:		GetDiffuseColor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
COLOR	COLOR_ANIM_DATA::GetColor(float	rTime) const
{
	COLOR C(255, 0, 0, 0);

	switch(_nInterpolationPower)
	{
		default:
			QDT_FAIL();
			break;

		case 0:
			C = _ColorTrack.GetPureValue(rTime);
			break;

		case 1:
			C = _ColorTrack.GetLinearValue(rTime);
			break;

		case 2:
			{
				if (_ColorTrack.GetNbrKeys() > 2 )
				{
					C = _ColorTrack.GetBezierValue(rTime);
				}
				else
				{
					C = _ColorTrack.GetLinearValue(rTime);
				}	
			}
			break;

		case 3:
			{
				if (_ColorTrack.GetNbrKeys() == 2 )
				{
					C = _ColorTrack.GetRandomValue(rTime);
				}
				else
				{
					C = _ColorTrack.GetPureValue(rTime);
				}
			}
			break;
	}

/*	
	if (_nInterpolationPower)
	{
		if (_ColorTrack.GetNbrKeys() > 2 && (_nInterpolationPower == 2))
		{
			C = _ColorTrack.GetBezierValue(rTime);
		}
		else
		{
			C = _ColorTrack.GetLinearValue(rTime);
		}	
	}
	else
	{
		C = _ColorTrack.GetPureValue(rTime);
	}
*/	
	return (C);
}

//-----------------------------------------------------------------------------
//	Name:		AddObjectAndAttribute
//	Object:		
//	03-12-10:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::AddComInstanceAndAttribute(const COM_INSTANCE &	CI,
											   QDT_STRING			string)
{
	_vComInstance.PushTail(CI);
	_AttributeName.PushTail(string);
	_nNbrComInstance++;

	// Get The descriptor to set the index value
	CLASS_DESCRIPTOR* pCD = CI.GetCommunicator()->GetClassDescriptor();
	_AttributeIndex.PushTail(pCD->FindAttributeIndex(string));
}

//-----------------------------------------------------------------------------
//	Name:		UpdateBehavior
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::UpdateBehavior(float rDeltaFrame)
{
	if (_bActive)
	{
		int nEach;
		
		if (_bReverseMode)
		{
			_rTime -= rDeltaFrame;
		}
		else
		{
			_rTime += rDeltaFrame;
		}

		COLOR color = GetColor(_rTime);
		
		for(nEach=0; nEach<_nNbrComInstance ; ++nEach)
		{
			if (_vComInstance[nEach].HasHandle() && _vComInstance[nEach].IsLoaded())
			{
				CLASS_DESCRIPTOR* pCD = _vComInstance[nEach].GetCommunicator()->GetClassDescriptor();	
				pCD->CallAttributeSet(_AttributeIndex[nEach], &_vComInstance[nEach], &color);	
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
COMMUNICATOR_DESCRIPTOR *	COLOR_ANIM_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLOR_ANIM_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	COLOR_ANIM_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("COLOR_ANIM_DATA"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLOR_ANIM_DATA, 
											QDT_COMMENT("F74D1DC3-BCFC-177E-0F73-011FCF78EA59"), 
											sizeof(COLOR_ANIM_DATA), 
											Init, 
											QDT_COMMENT("This represents the datas used to animate Color attributes. It consists in one track. ")));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::Init(CLASS_DESCRIPTOR * const	pCD)
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

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("COLORANIMINFOS", 0, CM_ReadChunkMatAnimInfos0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("COLORANIMINFOS", 1, CM_ReadChunkMatAnimInfos1);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("COLORANIMINFOS", 1, CM_WriteChunkMatAnimInfos1);
#endif
}
 
//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_Construct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COLOR_ANIM_DATA *	pColorAnimData;	
	pColorAnimData = static_cast<COLOR_ANIM_DATA *>(pO);
	QDT_NEW_PLACED(pColorAnimData, COLOR_ANIM_DATA)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-01-21:	VMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_Construct_In_Memory(void * const	pO,
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
void	COLOR_ANIM_DATA::EM_Destruct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COLOR_ANIM_DATA *	pColorAnimData;
	pColorAnimData = static_cast<COLOR_ANIM_DATA*> (pO);
	pColorAnimData->COLOR_ANIM_DATA::~COLOR_ANIM_DATA();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Play
//	Object:		
//	04-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_Play(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Play();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Stop
//	Object:		
//	04-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_Stop(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Stop();
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::CM_ReadChunkMatAnimInfos0(IO_STREAM &		Stream,
												   COMMUNICATOR *	pCommunicator)
{
	COLOR_ANIM_DATA	*pColorAnim;
 	pColorAnim = static_cast<COLOR_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;
	QDT_ASSERT( nNbrKeys > 0 );

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		COLOR_INT color;
		float rTime;
		Stream >> color;
		Stream >> rTime;

		GENERIC_KEY <COLOR_INT> key;
		key.SetTime(rTime);
		key.SetValue(color);
		pColorAnim->_ColorTrack.PushKey(key);
	}

	Stream >> pColorAnim->_nNbrComInstance;
	int nEach;

	for(nEach=0; nEach<pColorAnim->_nNbrComInstance ; ++nEach)
	{
		COM_INSTANCE	tmpMat;
		QDT_STRING		tmpAttName;
		int				nTmpAttIndex;

		Stream >> tmpMat;
		Stream >> tmpAttName;
		Stream >> nTmpAttIndex;

		pColorAnim->_vComInstance.PushTail(tmpMat);
		pColorAnim->_AttributeName.PushTail(tmpAttName);
		pColorAnim->_AttributeIndex.PushTail(nTmpAttIndex);
	}

	Stream >> pColorAnim->_nInterpolationPower;
	Stream >> pColorAnim->_bLoop;

	if ( nNbrKeys > 0 )
	{
		BEHAVIOR_MANAGER::Instance()->AddAnimatedColor(pColorAnim);
	}
	pColorAnim->_bUpdated = true;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos1
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::CM_ReadChunkMatAnimInfos1(IO_STREAM &		Stream,
												   COMMUNICATOR *	pCommunicator)
{
	COLOR_ANIM_DATA	*pColorAnim;
 	pColorAnim = static_cast<COLOR_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;
	Stream >> nNbrKeys;

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		COLOR_INT color;
		float rTime;
		Stream >> color;
		Stream >> rTime;

		GENERIC_KEY <COLOR_INT> key;
		key.SetTime(rTime);
		key.SetValue(color);
		pColorAnim->_ColorTrack.PushKey(key);
	}

	Stream >> pColorAnim->_nNbrComInstance;
	int nEach;

	for(nEach=0; nEach<pColorAnim->_nNbrComInstance ; ++nEach)
	{
		COM_INSTANCE	tmpMat;
		QDT_STRING		tmpAttName;
		int				nTmpAttIndex;

		Stream >> tmpMat;
		Stream >> tmpAttName;
		Stream >> nTmpAttIndex;

		pColorAnim->_vComInstance.PushTail(tmpMat);
		pColorAnim->_AttributeName.PushTail(tmpAttName);
		pColorAnim->_AttributeIndex.PushTail(nTmpAttIndex);
	}

	Stream >> pColorAnim->_nInterpolationPower;
	Stream >> pColorAnim->_bLoop;
	Stream >> pColorAnim->_bActive;

	BEHAVIOR_MANAGER::Instance()->AddAnimatedColor(pColorAnim);
	pColorAnim->_bUpdated = true;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunkMatInfos
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	COLOR_ANIM_DATA::CM_WriteChunkMatAnimInfos1(IO_STREAM &		Stream,
												    COMMUNICATOR *	pCommunicator)
{
	COLOR_ANIM_DATA	*pColorAnim;
 	pColorAnim = static_cast<COLOR_ANIM_DATA *>(pCommunicator);

	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("TEXTURE_ANIM_DATA : ");
	Stream.DebugOutput(pColorAnim->GetName());
	Stream.DebugOutput("\r\n");

	int nNbrKeys = pColorAnim->_ColorTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<COLOR_INT>(pColorAnim->_ColorTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pColorAnim->_ColorTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}
	Stream.DebugOutput("Nbr Attached Objects : ");
	Stream << pColorAnim->_nNbrComInstance;
	Stream.DebugOutput("\r\n");

	int nEach;
	for(nEach = 0; nEach<pColorAnim->_nNbrComInstance ; ++nEach)
	{
		Stream.DebugOutput("Material Inst : ");
		Stream << pColorAnim->_vComInstance[nEach];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Attribute Name : ");
		Stream << pColorAnim->_AttributeName[nEach];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Attribute Index : ");
		Stream << pColorAnim->_AttributeIndex[nEach];
		Stream.DebugOutput("\r\n");

	}
	
	Stream.DebugOutput("Interpolation : ");
	Stream << pColorAnim->_nInterpolationPower;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Loop Anim : ");
	Stream << pColorAnim->_bLoop;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Active : ");
	Stream << pColorAnim->_bActive;
	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Play
//	Object:		
//	04-02-18:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::Play()
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
void	COLOR_ANIM_DATA::Pause()
{
	_bActive = false;
}
		
//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::Resume()
{
	_bActive = true;
}

//-----------------------------------------------------------------------------
//	Name:		PlayReverse
//	Object:		
//	04-05-04:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::PlayReverse()
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
void	COLOR_ANIM_DATA::Stop()
{
	_bActive = false;
	_rTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		EM_PlayReverse
//	Object:		
//	04-05-04:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_PlayReverse(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->PlayReverse();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFirstFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_SetFirstFrame(void * const pO, 
										  void * const pR, 
										  void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->SetFrame(0.0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFirstFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::EM_SetLastFrame(void * const pO, 
										 void * const pR, 
										 void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->SetFrame(pAnimData->GetLength());
}

//-----------------------------------------------------------------------------
//	Name:		SetFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_ANIM_DATA::SetFrame(float rFrame)
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
void COLOR_ANIM_DATA::EM_Pause(void * const pO, 
							   void * const pR, 
							   void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Pause();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Resume
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void COLOR_ANIM_DATA::EM_Resume(void * const pO, 
							    void * const pR, 
								void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData	  = static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Resume();
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsPlaying
//	Object:		
//	04-12-20:	RMA - Created
//-----------------------------------------------------------------------------
void COLOR_ANIM_DATA::EM_IsPlaying(void * const pO, 
								   void * const pR, 
								   void * const pP)
{
	COM_INSTANCE *		pAnimDataInst	= static_cast<COM_INSTANCE*>( pO );
	COLOR_ANIM_DATA *	pAnimData		= static_cast<COLOR_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	bool *				pReturn			= static_cast<bool *>(pR);
	
	*pReturn = pAnimData->IsPlaying();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
