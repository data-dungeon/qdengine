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
// CLASS: FOR_INPUT
//
//
//	03-05-05:	JPE - Created
//*****************************************************************************


//=============================================================================
// Name: FOR_INPUT X360 
//=============================================================================

#ifndef __KINPUT_INPUT_X360_FORINPUT_H__
#define __KINPUT_INPUT_X360_FORINPUT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KINPUT(Input/ForInputABC)
#include	INCL_KINPUT(Input/X360/InputDeviceX360)

namespace QDT
{
	namespace KINPUT
	{
		class FOR_INPUT : public FOR_INPUT_ABC
		{
		public:	
			explicit FOR_INPUT();
					~FOR_INPUT();

			virtual bool	Init();
			virtual bool	Close();
			virtual void	Update();
			virtual bool	Flush();
			virtual	bool	Loop();

		private:
			int		_nCurrentDevices;
			int		_nNbDevices;
				
			int		_nFlags;
		};
	}
}

#endif	//__KINPUT_INPUT_X360_FORINPUT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================