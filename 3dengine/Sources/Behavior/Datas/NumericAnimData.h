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
//	The NUMERIC_ANIM_DATA class implements ...
//
//	03-11-18:	FPR - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_NUMERIC_ANIM_DATA_H__
#define __M3D_NUMERIC_ANIM_DATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_3DENGINE(Behavior/Datas/GenericTrack)

namespace QDT
{
	namespace M3D
	{
		class	NUMERIC_ANIM_DATA : public COMMUNICATOR
		{
		public:

			// Constructor & destructor
			NUMERIC_ANIM_DATA();
			~NUMERIC_ANIM_DATA();

			void			Play();
			void			PlayReverse();
			void			Stop();
			void			Pause();
			void			Resume();
			_INLINE_	bool			IsPlaying() const { return _bActive; }

			void			UpdateBehavior(float rDeltaFrame);
			void			SetFrame(float rFrame);			

			// Modifiors
			_INLINE_    void			SetReverseMode(bool b)								{ _bReverseMode = b; }
			_INLINE_	void			SetNumericTrack(GENERIC_TRACK<float> pCT)			{ _NumTrack = pCT; }
			_INLINE_	void			SetComInstanceVector(QDT_VECTOR <COM_INSTANCE> v)	{ _vComInstance = v; }
			_INLINE_	void			SetAttrNameVector(QDT_VECTOR <QDT_STRING>	v)		{ _AttributeName = v; }
			_INLINE_	void			SetAttrIndexVector(QDT_VECTOR <int> v)				{ _AttributeIndex = v; }
			_INLINE_	void			SetNbrComInstance(int nValue)						{ _nNbrComInstance = nValue; }
			_INLINE_	void			SetInterpolation(int nValue)						{ _nInterpolationPower = nValue; }
			_INLINE_    void			SetActive(bool bValue)								{ _bActive = bValue; }
			_INLINE_	void			SetLoop(bool bValue)								{ _bLoop = bValue; }
			void			AddComInstanceAndAttribute(const COM_INSTANCE & CI, QDT_STRING string);				

			// Accessors
			_INLINE_	GENERIC_TRACK<float> GetNumericTrack() const						{ return (_NumTrack); }							

			float			GetNumericValue(float rTime) const;
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
				CMD_PAUSE,
				CMD_RESUME,
				CMD_IS_PLAYING,
				CMD_SET_LOOP,
				CMD_LAST_NUMERIC_ANIM,
			};

			GENERIC_TRACK<float>		_NumTrack;
			QDT_VECTOR <COM_INSTANCE>	_vComInstance;
			QDT_VECTOR <QDT_STRING>		_AttributeName;
			QDT_VECTOR <int>			_AttributeIndex;
			int							_nNbrComInstance;
			int							_nInterpolationPower;
			bool						_bLoop;
			bool						_bUpdated;
			bool						_bActive;
			bool						_bReverseMode;

			float						_rTime;

			static		void			Init(CLASS_DESCRIPTOR * const pCD);

			// IO
			// Chunks
			static void					CM_ReadChunkNumAnimInfos0(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void					CM_ReadChunkNumAnimInfos1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

#ifndef _MASTER	
			static void					CM_WriteChunkNumAnimInfos1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif		

			static		void			EM_Construct(void * const pO, void * const pR, void * const pP);
			static		void			EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static		void			EM_Destruct(void * const pO, void * const pR, void * const pP);
			static		void			EM_Play(void * const pO, void * const pR, void * const pP);
			static		void			EM_Stop(void * const pO, void * const pR, void * const pP);
			static		void			EM_PlayReverse(void * const pO, void * const pR, void * const pP);
			static		void			EM_SetFirstFrame(void * const pO, void * const pR, void * const pP);
			static		void			EM_SetLastFrame(void * const pO, void * const pR, void * const pP);
			static		void			EM_Pause(void * const pO, void * const pR, void * const pP);
			static		void			EM_Resume(void * const pO, void * const pR, void * const pP);
			static		void			EM_IsPlaying(void * const pO, void * const pR, void * const pP);
			static		void			EM_SetLoop(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // #define __M3D_NUMERIC_ANIM_DATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
