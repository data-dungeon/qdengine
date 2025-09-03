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
//	CLASS:	INPUT_INIT_INFO_X360
//	The INPUT_INIT_INFO_X360 class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_X360_INPUTINITINFOX360_H__
#define __KINPUT_INPUT_X360_INPUTINITINFOX360_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KINPUT(Input/InputInitInfo)

namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_INIT_INFO_X360 : public INPUT_INIT_INFO
		{
			public:
						INPUT_INIT_INFO_X360();
						INPUT_INIT_INFO_X360(const INPUT_INIT_INFO_X360 & C);
				virtual  ~INPUT_INIT_INFO_X360();
				

				INPUT_INIT_INFO_X360 &	operator=(const INPUT_INIT_INFO_X360 & C);
				void	Copy(const INPUT_INIT_INFO_X360 & C);

				void	SetPort(const int nPort);
				int		GetPort() const;

				void	SetSlot(const int nSlot);
				int		GetSlot() const;

			private:

				int		_nPort;
				int		_nSlot;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
