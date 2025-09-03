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
//	CLASS:	CLOCK
//	The CLOCK class implements ...
//
//	02-02-27:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TIME_CLOCK_H__
#define __KCORE_TIME_CLOCK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		#define MAX_SPEED_FACTOR 3

//		#undef GetCurrentTime
//		#define GetCurrentTime			PLEASE_RENAME_GET_CURRENT_TIME

		class	CLOCK
		{
		public:
							CLOCK();
							CLOCK(const CLOCK & Clock); 
							~CLOCK();

			CLOCK &			operator = (const CLOCK & Clock);

			void			Update();

			void			Pause();
			void			Continue();

			Float32			GetCurTime() const;
			void			SetDeltaFrameTime(const Float32& rDeltaFrameTime);
			Float32			GetDeltaFrameTime() const							{ return (_rDeltaFrameTime); }
			Float32			GetDeltaFrameTimeInS() const						{ return (_rDeltaFrameTimeInS); }

			Float32			GetScale() const									{ return (_rScale); }
			Bool			IsPaused() const									{ return (_bPause); }
			
			/** Modifiors */
			void			SetScale(Float32 rScale)							{ _rScale = rScale; };

		private:

			void			Copy(const CLOCK & Clock);

			Bool			_bPause;
			Float32			_rScale;						// Scale time

			Float32			_rPausedCurrentTime;			// In ms
			Float32			_rOldTime;						// In ms
			Float32			_rCurrentTimeDecalage;			// In ms
			Float32			_rDeltaFrameTime;				// In ms
			Float32			_rDeltaFrameTimeInS;			// In S

			Float32			_rPausePausedCurrentTime;		// In ms
			Float32			_rPauseOldTime;					// In ms
			Float32			_rPauseCurrentTimeDecalage;		// In ms
			Float32			_rPauseDeltaFrameTime;			// In ms
			Float32			_rPauseDeltaFrameTimeInS;		// In S
		};
	}
}

#endif //__KCORE_TIME_CLOCK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
