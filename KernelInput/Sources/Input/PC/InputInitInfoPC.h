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
//	CLASS:	INPUT_INIT_INFO_PC
//	The INPUT_INIT_INFO_PC class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_PC_INPUTINITINFOPC_H__
#define __KINPUT_INPUT_PC_INPUTINITINFOPC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PC_DINPUT
#include	INCL_KCORE(Includes_PC/Includes_PC)
#include	INCL_KINPUT(Input/InputInitInfo)


namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_INIT_INFO_PC : public INPUT_INIT_INFO
		{
			public:
						INPUT_INIT_INFO_PC();
						INPUT_INIT_INFO_PC(const INPUT_INIT_INFO_PC & C);
				virtual ~INPUT_INIT_INFO_PC();
				

				INPUT_INIT_INFO_PC &	operator=(const INPUT_INIT_INFO_PC & C);
				void	Copy(const INPUT_INIT_INFO_PC & C);

				void					SetInputDevice(LPDIRECTINPUTDEVICE8 pDID8);
				LPDIRECTINPUTDEVICE8	GetInputDevice() const;

			private:

				LPDIRECTINPUTDEVICE8 _pDID8;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
