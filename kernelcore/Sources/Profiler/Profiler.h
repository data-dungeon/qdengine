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
// CLASS: TIMER_MANAGER
//
//
//03-04-07 VMA Created: 
//*****************************************************************************


//=============================================================================
// Name: PROFILER
//03-04-07 VMA Created: 
//=============================================================================

#ifndef __KCORE_PROFILER_PROFILER_H__
#define __KCORE_PROFILER_PROFILER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(Template/QDT_Vector)
#include INCL_KCORE(Template/Singleton)

// #define _PROFILER_

namespace	QDT
{
	namespace KCORE
	{
		#ifdef _PROFILER_
			#define	BEGIN_PROFILE(PROFILE_ID)			PROFILER::Instance()->BeginProfile(PROFILE_ID);
			#define	END_PROFILE(PROFILE_ID)				PROFILER::Instance()->EndProfile(PROFILE_ID);
		#else
			#define	BEGIN_PROFILE(PROFILE_ID)
			#define	END_PROFILE(PROFILE_ID)
		#endif

		enum	ProfileId
		{
			PROFILE_MOTEUR3D_LOOP			= 0 ,
			PROFILE_UPDATE_BEHAVIOR				,
			PROFILE_MOTEUR3D_DISPLAY			,
			PROFILE_PREPARE_VIEWPORT			,
			PROFILE_SCENEGRAPH_DISPLAY			,
			PROFILE_DRAW_PRIMITIVE				,
			PROFILE_PHOTO_PIPELINE				,
			PROFILE_VBLANK						,
			PROFILE_CLEAR						,
			PROFILE_NODE_ARRAY_UPDATE_WORLD		,
			PROFILE_NAS		,
			PROFILE_NAS_00		,
			PROFILE_NAS_01		,
			PROFILE_NAS_02		,
			PROFILE_NAS_03		,
			PROFILE_NAS_04		,
			PROFILE_NAS_05		,
			PROFILE_NAS_06		,

			PROFILE_MAX,
		};

		#define			FL_SUB			(1<<0)
		#define			FL_OPEN			(1<<1)
		#define			TIMESTACK_SIZE	16

		// 
		struct Profile{
			QDT_STRING			_sName;
			float				_rCurTime;
			float				_rTime;
			float				_rFinalTime;
			float				_rFinalTune;
			float				_rMinTime;
			float				_rMaxTime;
			float				_rMinTune;
			float				_rMaxTune;
			int					_nNbCalls;
			int					_nFinalNbCalls;
			char				_Flag;
			QDT_VECTOR<int>		_SubProfile;

			Profile()
			{
			}

			Profile(const QDT_STRING & sName)
			{
				_sName			= sName;
				_rTime			= 0.f;
				_rFinalTune		= 0.f;
				_nNbCalls		= 0;
				_nFinalNbCalls  = 0;
				_Flag			= 0;
				_rMinTime		= 65536.f;
				_rMaxTime		= 0.f;
				_rMinTune		= 65536.f;
				_rMaxTune		= 0.f;
			}
		};

		class PROFILER : public SINGLETON<PROFILER>
		{
		public:

										PROFILER();
										~PROFILER();

			void					Init();
			void					Close();
			void					Loop();

			int						AddProfile(const QDT_STRING & sName,int nId,int nParentId = -1 );
			void					InitProfile(int nProfileId = -1 );
			void					BeginProfile(int nId);
			void					EndProfile(int nProfileId);
			void					OpenProfile(const QDT_STRING & PrfName);
			void					CloseProfile(const QDT_STRING & PrfName);
			void					OpenAllProfiles();
			void					CloseAllProfiles();
			void					SetMaxProfile(int nMaxProfile);

			//
			// Profile Display
			void						GetFolderTime(int nId,float &rTune,float &rTime);
			void						PrintProfile(const QDT_STRING & sPrfName );
			const Profile&				GetProfile( int nId);
			const QDT_VECTOR<int>&		GetRootProfile() { return _RootProfile;}
			bool						IsEnabled()		 { return _bEnabled; }
			void						Enabled(bool b) { _bEnabled = b; }
		public:	
			
			QDT_VECTOR<int>					_ProfileID;
			QDT_VECTOR<Profile>				_ProfileList;
			QDT_VECTOR<int>					_RootProfile;

			QDT_VECTOR<int>					_ProfileIdStack;

			static float					_TimeStack[TIMESTACK_SIZE];
			static int						_TimeStackPos;

			static float					_rLastTime;
			static float					_rStartTime;
			static int						_nNbFrames;
			static bool						_bEnabled;
		};
	}
}

#endif //__KCORE_PROFILER_PROFILER_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
