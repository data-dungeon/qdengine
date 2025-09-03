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
//	CLASS:	TEXTURE_ANIM_DATA
//
//	03-11-06:	FPR - Created
//*****************************************************************************

#include	"Root.h"
#include	"TextureAnimData.h"

#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_ANIM_DATA constructor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
TEXTURE_ANIM_DATA::TEXTURE_ANIM_DATA()
:
COMMUNICATOR			(GetGenericCommunicatorDescriptor()),
_rTime					(0.0f),
_nInterpolationPower	(0),
_nNbrMaterials			(0),
_bUpdated				(false),
_bActive				(true),
_bReverseMode			(false),
_rRotationShiftU		(0.0f),
_rRotationShiftV		(0.0f)
{
	_vMaterials.Clear();
	_RotateTrack.ClearKeys();
	_RotateTrack._nLastIndiceUsed = 0;
	_ScaleUTrack.ClearKeys();
	_ScaleUTrack._nLastIndiceUsed = 0;
	_ScaleVTrack.ClearKeys();
	_ScaleVTrack._nLastIndiceUsed = 0;
	_TranslateUTrack.ClearKeys();
	_TranslateUTrack._nLastIndiceUsed = 0;
	_TranslateVTrack.ClearKeys();
	_TranslateVTrack._nLastIndiceUsed = 0;
}

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_ANIM_DATA destructor
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
TEXTURE_ANIM_DATA::~TEXTURE_ANIM_DATA()
{
	if (_bUpdated)
	{
		BEHAVIOR_MANAGER::Instance()->RemoveAnimatedMaterialTexture(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetLength
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
float	TEXTURE_ANIM_DATA::GetLength() const
{
	// Anyway they are all the same length...
	return (MATH::Max(0.0f, _RotateTrack.GetLength()));
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	03-12-08:	FPR - Created
//-----------------------------------------------------------------------------
float	TEXTURE_ANIM_DATA::GetValue(float rTime, TRACK_TYPE	eTrack) const
{
	float F = 0.0f;
	
	switch(eTrack)
	{
	default:
		QDT_FAIL();
		break;
		
	case TRACK_ROTATE:
		{
			switch(_nInterpolationPower)
			{
				default:
					QDT_FAIL();
					break;

				case 0:
					F = _RotateTrack.GetPureValue(rTime);
					break;

				case 1:
					F = _RotateTrack.GetLinearValue(rTime);
					break;

				case 2:
					{
						if (_RotateTrack.GetNbrKeys() > 2 )
						{
							F = _RotateTrack.GetBezierValue(rTime);
						}
						else
						{
							F = _RotateTrack.GetLinearValue(rTime);
						}	
					}
					break;

				case 3:
					{
						if (_RotateTrack.GetNbrKeys() == 2 )
						{
							F = _RotateTrack.GetRandomValue(rTime);
						}
						else
						{
							F = _RotateTrack.GetPureValue(rTime);
						}
					}
					break;
			}
		}
		break;
		
	case TRACK_SCALE_U:
		{
			switch(_nInterpolationPower)
			{
				default:
					QDT_FAIL();
					break;

				case 0:
					F = _ScaleUTrack.GetPureValue(rTime);
					break;

				case 1:
					F = _ScaleUTrack.GetLinearValue(rTime);
					break;

				case 2:
					{
						if (_ScaleUTrack.GetNbrKeys() > 2 )
						{
							F = _ScaleUTrack.GetBezierValue(rTime);
						}
						else
						{
							F = _ScaleUTrack.GetLinearValue(rTime);
						}	
					}
					break;

				case 3:
					{
						if (_ScaleUTrack.GetNbrKeys() == 2 )
						{
							F = _ScaleUTrack.GetRandomValue(rTime);
						}
						else
						{
							F = _ScaleUTrack.GetPureValue(rTime);
						}
					}
					break;
			}
		}
		break;
		
	case TRACK_SCALE_V:
		{
			switch(_nInterpolationPower)
			{
				default:
					QDT_FAIL();
					break;

				case 0:
					F = _ScaleVTrack.GetPureValue(rTime);
					break;

				case 1:
					F = _ScaleVTrack.GetLinearValue(rTime);
					break;

				case 2:
					{
						if (_ScaleVTrack.GetNbrKeys() > 2 )
						{
							F = _ScaleVTrack.GetBezierValue(rTime);
						}
						else
						{
							F = _ScaleVTrack.GetLinearValue(rTime);
						}	
					}
					break;

				case 3:
					{
						if (_ScaleVTrack.GetNbrKeys() == 2 )
						{
							F = _ScaleVTrack.GetRandomValue(rTime);
						}
						else
						{
							F = _ScaleVTrack.GetPureValue(rTime);
						}
					}
					break;
			}	
		}
		break;
		
	case TRACK_TRANSLATE_U:
		{
			switch(_nInterpolationPower)
			{
				default:
					QDT_FAIL();
					break;

				case 0:
					F = _TranslateUTrack.GetPureValue(rTime);
					break;

				case 1:
					F = _TranslateUTrack.GetLinearValue(rTime);
					break;

				case 2:
					{
						if (_TranslateUTrack.GetNbrKeys() > 2 )
						{
							F = _TranslateUTrack.GetBezierValue(rTime);
						}
						else
						{
							F = _TranslateUTrack.GetLinearValue(rTime);
						}	
					}
					break;

				case 3:
					{
						if (_TranslateUTrack.GetNbrKeys() == 2 )
						{
							F = _TranslateUTrack.GetRandomValue(rTime);
						}
						else
						{
							F = _TranslateUTrack.GetPureValue(rTime);
						}
					}
					break;
			}
		}
		break;
		
	case TRACK_TRANSLATE_V:
		{
			switch(_nInterpolationPower)
			{
				default:
					QDT_FAIL();
					break;

				case 0:
					F = _TranslateVTrack.GetPureValue(rTime);
					break;

				case 1:
					F = _TranslateVTrack.GetLinearValue(rTime);
					break;

				case 2:
					{
						if (_TranslateVTrack.GetNbrKeys() > 2 )
						{
							F = _TranslateVTrack.GetBezierValue(rTime);
						}
						else
						{
							F = _TranslateVTrack.GetLinearValue(rTime);
						}	
					}
					break;

				case 3:
					{
						if (_TranslateVTrack.GetNbrKeys() == 2 )
						{
							F = _TranslateVTrack.GetRandomValue(rTime);
						}
						else
						{
							F = _TranslateVTrack.GetPureValue(rTime);
						}
					}
					break;
			}
		}
		break;
	}	
	
	return (F);
}

//-----------------------------------------------------------------------------
//	Name:		AddMaterial
//	Object:		
//	03-12-08:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::AddMaterial(const COM_INSTANCE &	CI)
{
	_vMaterials.PushTail(CI);
	_nNbrMaterials++;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::SetFrame(float rFrame)
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
//	Name:		UpdateBehavior
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::UpdateBehavior(float	rDeltaFrame)
{
	if (_bActive)
	{
		// Get the value...
		if (_bReverseMode)
		{
			_rTime -= rDeltaFrame;
		}
		else
		{
			_rTime += rDeltaFrame;	
		}

		float rRotate		= GetValue(_rTime, TRACK_ROTATE);
		float rScaleU		= GetValue(_rTime, TRACK_SCALE_U);
		float rScaleV		= GetValue(_rTime, TRACK_SCALE_V);
		float rTranslateU	= GetValue(_rTime, TRACK_TRANSLATE_U);
		float rTranslateV	= -GetValue(_rTime, TRACK_TRANSLATE_V);

		//Prepare Shifting
		MATRIX mDecalRotation;
		MATRIX mInvDecalRotation;
		mDecalRotation.Identity();
		mInvDecalRotation.Identity();

		if( (_rRotationShiftU!=0.0f) || (_rRotationShiftV!=0.0f) )
		{
			mDecalRotation(2, 0) = _rRotationShiftU;
			mDecalRotation(2, 1) = -_rRotationShiftV;
			mInvDecalRotation(2, 0) = -_rRotationShiftU;
			mInvDecalRotation(2, 1) = _rRotationShiftV;
		}

		/*for(nEachMaterial=0; nEachMaterial<_nNbrMaterials ; ++nEachMaterial)
		{
			if (_vMaterials[nEachMaterial].HasHandle())
			{
				MATERIAL_ABC* pMaterial = static_cast<MATERIAL_ABC *>(_vMaterials[nEachMaterial].GetCommunicator());

				MATRIX mTexTransform;
				MATRIX mShiftedTexTransform;
				MATRIX Mul;

				Mul.Identity();
				mTexTransform.Identity();
				mShiftedTexTransform.Identity();

				//Translation
				MATRIX mTrans;
				mTrans.Identity();
				mTrans(2, 0) = rTranslateU;
				mTrans(2, 1) = rTranslateV;
				Mul.Mul3x3(mTrans, mTexTransform);
				mTexTransform = Mul;			
				
				if( (_rRotationShiftU!=0.0f) || (_rRotationShiftV!=0.0f) )
				{
					Mul.Mul3x3(mDecalRotation, mTexTransform);
					mTexTransform = Mul;					
				}

				//scale
				MATRIX mScale;
				mScale.Identity();
				mScale(0, 0) = rScaleU;
				mScale(1, 1) = rScaleV;
				Mul.Mul3x3(mScale, mTexTransform);
				mTexTransform = Mul;


				//Rotation
				MATRIX mRot;
				mRot.Identity();
				mRot.SetRotZ(MATH::DegreToRadian(rRotate));
				Mul.Mul3x3(mRot, mTexTransform);
				mTexTransform = Mul;
                
				if( (_rRotationShiftU!=0.0f) || (_rRotationShiftV!=0.0f) )
				{
					Mul.Mul3x3(mInvDecalRotation, mTexTransform);
					mTexTransform = Mul;
				}

				

				pMaterial->SetTextureMatrix(mTexTransform);
				pMaterial->SetUseTextureMatrix(true);
			}
		}*/
		QDT_FAIL();

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
COMMUNICATOR_DESCRIPTOR *	TEXTURE_ANIM_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TEXTURE_ANIM_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TEXTURE_ANIM_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("TEXTURE_ANIM_DATA"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TEXTURE_ANIM_DATA, 
											QDT_COMMENT("E2118C35-E566-42FF-9EAF-1E2C1811314D"), 
											sizeof(TEXTURE_ANIM_DATA), 
											Init, 
											QDT_COMMENT("This represents the datas used to animate uv attributes. It consists in 5 tracks: 1rotation, 2scales and 2translations. ")));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::Init(CLASS_DESCRIPTOR * const	pCD)
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

	pMD = METHOD_DESCRIPTOR__Allocate("SetLoop", "Set theif the animation loop or not.");
	METHOD_DESCRIPTOR__AddParameter(pMD, "bLoop", "Loop value", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_LOOP, pMD, EM_SetLoop);

	pMD = METHOD_DESCRIPTOR__Allocate("Pause", "Pause the animation");
	pCD->AddMethod(CMD_PAUSE, pMD, EM_Pause);

	pMD = METHOD_DESCRIPTOR__Allocate("Resume", "Resume the animation");
	pCD->AddMethod(CMD_RESUME, pMD, EM_Resume);

	pMD = METHOD_DESCRIPTOR__Allocate("IsPlaying", "Is the animation playing");
	pMD->SetReturn(QDT_COMMENT("bIsPlaynig"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_PLAYING, pMD, EM_IsPlaying);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TEXTUREANIMINFOS", 0, CM_ReadChunkTexAnimInfos0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TEXTUREANIMINFOS", 1, CM_ReadChunkTexAnimInfos1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TEXTUREANIMINFOS", 2, CM_ReadChunkTexAnimInfos2);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("TEXTUREANIMINFOS", 2, CM_WriteChunkTexAnimInfos2);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-11-06:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_Construct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	TEXTURE_ANIM_DATA *	pNumAnimData;	
	pNumAnimData = static_cast<TEXTURE_ANIM_DATA *>(pO);
	QDT_NEW_PLACED(pNumAnimData, TEXTURE_ANIM_DATA)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-01-21:	VMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_Construct_In_Memory(void * const	pO,
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
void	TEXTURE_ANIM_DATA::EM_Destruct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	TEXTURE_ANIM_DATA *	pNumAnimData;
	pNumAnimData = static_cast<TEXTURE_ANIM_DATA*> (pO);
	pNumAnimData->TEXTURE_ANIM_DATA::~TEXTURE_ANIM_DATA();
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos0
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::CM_ReadChunkTexAnimInfos0(IO_STREAM &		Stream,
													 COMMUNICATOR *	pCommunicator)
{	
	TEXTURE_ANIM_DATA	*pTexAnimData;
 	pTexAnimData = static_cast<TEXTURE_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;

	//Rotation
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
		pTexAnimData->_RotateTrack.PushKey(key);
	}

	//Scale U
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
		pTexAnimData->_ScaleUTrack.PushKey(key);
	}

	//Scale V
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
		pTexAnimData->_ScaleVTrack.PushKey(key);
	}

	//Translate U
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
		pTexAnimData->_TranslateUTrack.PushKey(key);
	}

	//Translate V
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
		pTexAnimData->_TranslateVTrack.PushKey(key);
	}

	// Materials
	Stream >> pTexAnimData->_nNbrMaterials;
	int nEachMat;
	for(nEachMat=0; nEachMat<pTexAnimData->_nNbrMaterials ; ++nEachMat)
	{
		COM_INSTANCE	tmpMat;	
		Stream >> tmpMat;	
		pTexAnimData->_vMaterials.PushTail(tmpMat);
	}

	// Settings
	Stream >> pTexAnimData->_nInterpolationPower;
	Stream >> pTexAnimData->_bLoop;

	BEHAVIOR_MANAGER::Instance()->AddAnimatedMaterialTexture(pTexAnimData);
	pTexAnimData->_bUpdated = true;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos1
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::CM_ReadChunkTexAnimInfos1(IO_STREAM &		Stream,
													 COMMUNICATOR *	pCommunicator)
{	
	TEXTURE_ANIM_DATA	*pTexAnimData;
 	pTexAnimData = static_cast<TEXTURE_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;

	//Rotation
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
		pTexAnimData->_RotateTrack.PushKey(key);
	}

	//Scale U
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
		pTexAnimData->_ScaleUTrack.PushKey(key);
	}

	//Scale V
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
		pTexAnimData->_ScaleVTrack.PushKey(key);
	}

	//Translate U
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
		pTexAnimData->_TranslateUTrack.PushKey(key);
	}

	//Translate V
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
		pTexAnimData->_TranslateVTrack.PushKey(key);
	}

	// Materials
	Stream >> pTexAnimData->_nNbrMaterials;
	int nEachMat;
	for(nEachMat=0; nEachMat<pTexAnimData->_nNbrMaterials ; ++nEachMat)
	{
		COM_INSTANCE	tmpMat;	
		Stream >> tmpMat;	
		pTexAnimData->_vMaterials.PushTail(tmpMat);
	}

	// Settings
	Stream >> pTexAnimData->_nInterpolationPower;
	Stream >> pTexAnimData->_bLoop;
	Stream >> pTexAnimData->_bActive;

	BEHAVIOR_MANAGER::Instance()->AddAnimatedMaterialTexture(pTexAnimData);
	pTexAnimData->_bUpdated = true;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkTexAnimInfos2
//	Object:		
//	04-05-25:	FPR - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::CM_ReadChunkTexAnimInfos2(IO_STREAM &	Stream,
													 COMMUNICATOR *	pCommunicator)
{
	TEXTURE_ANIM_DATA	*pTexAnimData;
 	pTexAnimData = static_cast<TEXTURE_ANIM_DATA *>(pCommunicator);
	int nNbrKeys, nEachKey;

	//Rotation
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
		pTexAnimData->_RotateTrack.PushKey(key);
	}

	//Scale U
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
		pTexAnimData->_ScaleUTrack.PushKey(key);
	}

	//Scale V
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
		pTexAnimData->_ScaleVTrack.PushKey(key);
	}

	//Translate U
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
		pTexAnimData->_TranslateUTrack.PushKey(key);
	}

	//Translate V
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
		pTexAnimData->_TranslateVTrack.PushKey(key);
	}

	// Materials
	Stream >> pTexAnimData->_nNbrMaterials;
	int nEachMat;
	for(nEachMat=0; nEachMat<pTexAnimData->_nNbrMaterials ; ++nEachMat)
	{
		COM_INSTANCE	tmpMat;	
		Stream >> tmpMat;	
		pTexAnimData->_vMaterials.PushTail(tmpMat);
	}

	// Settings
	Stream >> pTexAnimData->_rRotationShiftU;
	Stream >> pTexAnimData->_rRotationShiftV;
	Stream >> pTexAnimData->_nInterpolationPower;
	Stream >> pTexAnimData->_bLoop;
	Stream >> pTexAnimData->_bActive;

	BEHAVIOR_MANAGER::Instance()->AddAnimatedMaterialTexture(pTexAnimData);
	pTexAnimData->_bUpdated = true;
}


#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunkMatInfos
//	Object:		
//	03-11-13:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	TEXTURE_ANIM_DATA::CM_WriteChunkTexAnimInfos2(IO_STREAM &		Stream,
													  COMMUNICATOR *	pCommunicator)
{
	
	TEXTURE_ANIM_DATA	*pTexAnimData;
 	pTexAnimData = static_cast<TEXTURE_ANIM_DATA *>(pCommunicator);

	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("TEXTURE_ANIM_DATA : ");
	Stream.DebugOutput(pTexAnimData->GetName());
	Stream.DebugOutput("\r\n");

	//Rotation
	Stream.DebugOutput("Rotation Track : ");
	int nNbrKeys = pTexAnimData->_RotateTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<float>(pTexAnimData->_RotateTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pTexAnimData->_RotateTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}

	//Scale U
	Stream.DebugOutput("Scale U Track : ");
	nNbrKeys = pTexAnimData->_ScaleUTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<float>(pTexAnimData->_ScaleUTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pTexAnimData->_ScaleUTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}

	//Scale V
	Stream.DebugOutput("Scale V Track : ");
	nNbrKeys = pTexAnimData->_ScaleVTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<float>(pTexAnimData->_ScaleVTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pTexAnimData->_ScaleVTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}

	//Translate U
	Stream.DebugOutput("Translate U Track : ");
	nNbrKeys = pTexAnimData->_TranslateUTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<float>(pTexAnimData->_TranslateUTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pTexAnimData->_TranslateUTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}

	//Translate V
	Stream.DebugOutput("Translate V Track : ");
	nNbrKeys = pTexAnimData->_TranslateVTrack.GetNbrKeys(), nEachKey;
	Stream.DebugOutput("Nb keys : ");
	Stream << nNbrKeys;	
	Stream.DebugOutput("\r\n");

	for(nEachKey = 0; nEachKey<nNbrKeys ; ++nEachKey)
	{
		Stream << static_cast<float>(pTexAnimData->_TranslateVTrack.GetKey(nEachKey).GetValue());
		Stream.DebugOutput("\t");
		Stream << static_cast<float>(pTexAnimData->_TranslateVTrack.GetKey(nEachKey).GetTime());		
		Stream.DebugOutput("\r\n");
	}

	// Materials
	Stream.DebugOutput("Nbr Materials : ");
	Stream << pTexAnimData->_nNbrMaterials;
	Stream.DebugOutput("\r\n");

	int nEachMat;
	for(nEachMat = 0; nEachMat<pTexAnimData->_nNbrMaterials ; ++nEachMat)
	{
		Stream.DebugOutput("Material Inst : ");
		Stream << pTexAnimData->_vMaterials[nEachMat];
		Stream.DebugOutput("\r\n");
	}
	
	//Settings
	Stream.DebugOutput("Rotation Shift On U Coord : ");
	Stream << pTexAnimData->_rRotationShiftU;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Rotation Shift On V Coord : ");
	Stream << pTexAnimData->_rRotationShiftV;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Interpolation : ");
	Stream << pTexAnimData->_nInterpolationPower;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Loop : ");
	Stream << pTexAnimData->_bLoop;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Active : ");
	Stream << pTexAnimData->_bActive;
	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Play
//	Object:		
//	04-02-18:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::Play()
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
void	TEXTURE_ANIM_DATA::Pause()
{
	_bActive = false;
}
		
//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	04-11-02:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::Resume()
{
	_bActive = true;
}

//-----------------------------------------------------------------------------
//	Name:		PlayReverse
//	Object:		
//	04-05-04:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::PlayReverse()
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
void	TEXTURE_ANIM_DATA::Stop()
{
	_bActive = false;
	_rTime = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Play
//	Object:		
//	04-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_Play(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Play();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Stop
//	Object:		
//	04-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_Stop(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Stop();
}

//-----------------------------------------------------------------------------
//	Name:		EM_PlayReverse
//	Object:		
//	04-05-04:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_PlayReverse(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->PlayReverse();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFirstFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_SetFirstFrame(void * const pO, 
											void * const pR, 
											void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->SetFrame(0.0);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFirstFrame
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_SetLastFrame(void * const pO, 
										   void * const pR, 
										   void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->SetFrame(pAnimData->GetLength());
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLoop
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_ANIM_DATA::EM_SetLoop(void * const pO, 
									  void * const pR, 
									  void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	bool *				pbParam		  = static_cast<bool*>( pP );
	pAnimData->SetLoop(*pbParam		  );
}

//-----------------------------------------------------------------------------
//	Name:		EM_Pause
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_ANIM_DATA::EM_Pause(void * const pO, 
								   void * const pR, 
								   void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Pause();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Resume
//	Object:		
//	04-05-03:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_ANIM_DATA::EM_Resume(void * const pO, 
								  void * const pR, 
								  void * const pP)
{
	COM_INSTANCE *		pAnimDataInst = static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData	  = static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	pAnimData->Resume();
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsPlaying
//	Object:		
//	04-12-20:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_ANIM_DATA::EM_IsPlaying(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	COM_INSTANCE *		pAnimDataInst	= static_cast<COM_INSTANCE*>( pO );
	TEXTURE_ANIM_DATA *	pAnimData		= static_cast<TEXTURE_ANIM_DATA*>( pAnimDataInst->GetCommunicator() );
	bool *				pReturn			= static_cast<bool *>(pR);
	
	*pReturn = pAnimData->IsPlaying();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
