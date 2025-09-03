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
//	CLASS:	BASE_TIMER_ATTR
//	The BASE_TIMER_ATTR class implements ...
//
//	02-03-01:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BASE_TIMER_ATTR_H__
#define __M3D_BASE_TIMER_ATTR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace M3D
	{
		class	BASE_TIMER_ATTR
		{
		public:
			
			BASE_TIMER_ATTR();
			explicit			BASE_TIMER_ATTR(const BASE_TIMER_ATTR & C);
								~BASE_TIMER_ATTR();
			BASE_TIMER_ATTR&	operator=(const BASE_TIMER_ATTR & C);
			void				Copy(const BASE_TIMER_ATTR & C);
			
			bool		UpdateTime( float& rDelay );
			void		Reset();	
			
			_INLINE_	float	GetCurTime()const;
			_INLINE_	float	GetTimeLong()const;	
			_INLINE_	void	SetCurrentTime( float rVal );
			_INLINE_	void	SetTimeLong( float rVal );
			_INLINE_	bool	IsFinished()const;
			_INLINE_	void	Mirror();

		protected:

						bool	ComputeTime();
			
		protected:
			
			float				_rDelay;
			float				_rNewDelay;
			float				_rCurrentTime;
			float				_rTimeLong;
			float				_rTimeToGo;		//	Time in frame before ending
			float				_rSpeed;
			bool				_bFinished;
			
			
			
		};
	}
}

#ifndef _DEBUG
	#include "BaseTimerAttr.inl"
#endif

#endif // __M3D_BASE_TIMER_ATTR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
