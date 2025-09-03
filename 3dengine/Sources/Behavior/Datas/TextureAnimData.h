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
//	The TEXTURE_ANIM_DATA class implements ...
//
//	03-12-08:	FPR - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TEXTURE_ANIM_DATA_H__
#define __M3D_TEXTURE_ANIM_DATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_3DENGINE(Behavior/Datas/GenericTrack)

namespace QDT
{
	namespace M3D
	{
		class	TEXTURE_ANIM_DATA: public COMMUNICATOR
		{
		public:

			enum TRACK_TYPE
			{
				TRACK_ROTATE = 0,
				TRACK_SCALE_U,
				TRACK_SCALE_V,
				TRACK_TRANSLATE_U,
				TRACK_TRANSLATE_V,
			};

			// Constructor & destructor
			TEXTURE_ANIM_DATA();
			~TEXTURE_ANIM_DATA();

			void			Play();
			void			PlayReverse();
			void			Stop();
			void			Pause();
			void			Resume();
			_INLINE_	bool			IsPlaying() const { return _bActive; }

			void			UpdateBehavior(float rDeltaFrame);		
			void			SetFrame(float rFrame);

			// Modifiors
			_INLINE_    void			SetReverseMode(bool b)							{ _bReverseMode = b; }
			_INLINE_	void			SetRotateTrack(GENERIC_TRACK<float> pCT)		{ _RotateTrack = pCT; }
			_INLINE_	void			SetScaleUTrack(GENERIC_TRACK<float> pCT)		{ _ScaleUTrack = pCT; }
			_INLINE_	void			SetScaleVTrack(GENERIC_TRACK<float> pCT)		{ _ScaleVTrack = pCT; }
			_INLINE_	void			SetTranslateUTrack(GENERIC_TRACK<float> pCT)	{ _TranslateUTrack = pCT; }
			_INLINE_	void			SetTranslateVTrack(GENERIC_TRACK<float> pCT)	{ _TranslateVTrack = pCT; }
			_INLINE_    void			SetActive(bool bValue)								{ _bActive = bValue; }

			_INLINE_	void			SetMaterialVector(QDT_VECTOR <COM_INSTANCE> v)	{ _vMaterials = v; }
			_INLINE_	void			SetNbrMaterials(int nValue)						{ _nNbrMaterials = nValue; }
			_INLINE_	void			SetInterpolation(int nValue)					{ _nInterpolationPower = nValue; }
			_INLINE_	void			SetLoop(bool bValue)							{ _bLoop = bValue; }
			_INLINE_	void			SetRotationShiftU(float rValue)					{ _rRotationShiftU = rValue; }
			_INLINE_	void			SetRotationShiftV(float rValue)					{ _rRotationShiftV = rValue; }
			void			AddMaterial(const COM_INSTANCE & CI);

			// Accessors
			_INLINE_	GENERIC_TRACK<float>	GetRotateTrack()	const				{ return (_RotateTrack); }
			_INLINE_	GENERIC_TRACK<float>	GetScaleUTrack()	const				{ return (_ScaleUTrack); }
			_INLINE_	GENERIC_TRACK<float>	GetScaleVTrack()	const				{ return (_ScaleVTrack); }
			_INLINE_	GENERIC_TRACK<float>	GetTranslateUTrack()	const			{ return (_TranslateUTrack); }
			_INLINE_	GENERIC_TRACK<float>	GetTranslateVTrack()	const			{ return (_TranslateVTrack); }

			float			GetValue(float rTime, TRACK_TYPE eTrack) const;
			float			GetLength() const;

			static		COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static		COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			enum
			{
				CMD_PLAY = CMD_COM_USER_START,
				CMD_STOP,
				CMD_PLAY_REVERSE,
				CMD_SET_FIRST_FRAME,
				CMD_SET_LAST_FRAME,
				CMD_SET_LOOP,
				CMD_PAUSE,
				CMD_RESUME,
				CMD_IS_PLAYING,
				CMD_LAST_TEXTURE_ANIM,
			};

			GENERIC_TRACK<float>		_RotateTrack;
			GENERIC_TRACK<float>		_ScaleUTrack;
			GENERIC_TRACK<float>		_ScaleVTrack;
			GENERIC_TRACK<float>		_TranslateUTrack;
			GENERIC_TRACK<float>		_TranslateVTrack;

			float						_rRotationShiftU;
			float						_rRotationShiftV;

			QDT_VECTOR <COM_INSTANCE>	_vMaterials;

			int							_nNbrMaterials;
			int							_nInterpolationPower;
			bool						_bLoop;
			bool						_bUpdated;
			bool						_bActive;
			bool						_bReverseMode;

			float						_rTime;

			static		void			Init(CLASS_DESCRIPTOR * const pCD);

			// IO
			// Chunks
			static void					CM_ReadChunkTexAnimInfos0(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void					CM_ReadChunkTexAnimInfos1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void					CM_ReadChunkTexAnimInfos2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

#ifndef _MASTER
			static void					CM_WriteChunkTexAnimInfos2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif

			static		void			EM_Construct(void * const pO, void * const pR, void * const pP);
			static		void			EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static		void			EM_Destruct(void * const pO, void * const pR, void * const pP);
			static		void			EM_Play(void * const pO, void * const pR, void * const pP);
			static		void			EM_Stop(void * const pO, void * const pR, void * const pP);
			static		void			EM_PlayReverse(void * const pO, void * const pR, void * const pP);
			static		void			EM_SetFirstFrame(void * const pO, void * const pR, void * const pP);
			static		void			EM_SetLastFrame(void * const pO, void * const pR, void * const pP);
			static		void			EM_SetLoop(void * const pO, void * const pR, void * const pP);
			static		void			EM_Pause(void * const pO, void * const pR, void * const pP);
			static		void			EM_Resume(void * const pO, void * const pR, void * const pP);
			static		void			EM_IsPlaying(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_TEXTURE_ANIM_DATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
