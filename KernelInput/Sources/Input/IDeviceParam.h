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
//	CLASS:	IDEVICE_PARAM
//	The IDEVICE_PARAM class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_IDEVICEPARAM_H__
#define __KINPUT_INPUT_IDEVICEPARAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KINPUT
	{
		class	IDEVICE_PARAM
		{
		public:
					IDEVICE_PARAM();
					IDEVICE_PARAM(const IDEVICE_PARAM & C);
			virtual ~IDEVICE_PARAM();
			
			IDEVICE_PARAM &	operator=(const IDEVICE_PARAM & C);
			void	Copy(const IDEVICE_PARAM & C);

			virtual void ProcessEvent(const int	nObjectType, const int	nIndex, const float	rVal) = 0;
			virtual void ProcessConnectEvent() = 0;
			virtual void ProcessDisconnectEvent() = 0;
			virtual bool IsEventActive() = 0;

		private:
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
