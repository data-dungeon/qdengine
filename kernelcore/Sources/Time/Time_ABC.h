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
// CLASS: TIME
//
//
// 00-11-22 FKE Created
// 02-02-27 RMA Redone
//*****************************************************************************

#ifndef __KCORE_TIME_TIME_ABC_H__
#define __KCORE_TIME_TIME_ABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define MAX_CLOCK 50

#include INCL_KCORE(Time/Clock)

class MEM_POOL_MANAGER;

namespace QDT
{
	namespace KCORE
	{
		class TIME
		{
		friend class MEM_POOL_MANAGER;

		public:

			static TIME*				GetInstance() { return _pInstance; }

			/** Singleton */
			static TIME*				Instance();
			static void					Suicide();
			static void					CreateInstance();

			/** Accessors */
			Float32						GetFrameRate() const			{ return (_rFPS); }
			CLOCK *						GetDefaultClock()				{ return (&_DefaultClock); }
			CLOCK *						GetClock(int nNumClock) 		{ return (&_Clocks[nNumClock]); }

			virtual Float64				GetSystemTime() const = 0;
			virtual UInt64				GetUniversalTime() const = 0;

			Float32						GetFPS() const					{ return (_rFPS); }
			Float32						GetTargetFPS() const			{ return (_rTargetFPS); }
			Float32						GetFrameTime() const			{ return (_rFrameTime); }
			
		#if (!defined(_MASTER_FINAL_BUILD) && (defined(PLATFORM_PC)||defined(PLATFORM_PS3)||defined(PLATFORM_X360)))
			void						UnlockFramerate(void)			{ SetFrameRateLock( -1.0f ); }
			void						SetFrameRateLock(Float32 rLock)	{ _rFrameRateLock = rLock; }
			Float32						GetFrameRateLock(void)			{ return _rFrameRateLock; }
		#endif

			/** Call it once per frame */
			void						Update();
			virtual void				UpdateSystemTime() = 0;

			/** Init the time */
			void						Init();

			/** Create a new clock */
			int							CreateNewClock();

										TIME();
			virtual					   ~TIME();

		private:

			static TIME*				_pInstance;

			TIME &						operator = (const TIME & Time);

			void						UpdateFrameRate();

			CLOCK						_DefaultClock;
			CLOCK						_Clocks[MAX_CLOCK];
			UInt32						_nNbrClocks;

			const Float32				_rTargetFPS;
			const Float32				_rComputeFPSPeriodTime;

			Float32						_rNextComputeFPSTime;
			unsigned int				_nNbrFrames;
			Float32						_rFPS;

		protected:	
			
			Float32						_rFrameTime;

		#if (!defined(_MASTER_FINAL_BUILD) && (defined(PLATFORM_PC)||defined(PLATFORM_PS3)||defined(PLATFORM_X360)))
			Float32						_rFrameRateLock;		// in ms
		#endif
		};
	}
}

#endif //__KCORE_TIME_TIME_ABC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
