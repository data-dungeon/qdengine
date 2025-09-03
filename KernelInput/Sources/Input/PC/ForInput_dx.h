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
//00-11-15 FKE Created: 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_PC_FORINPUTDX_H__
#define __KINPUT_INPUT_PC_FORINPUTDX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PC_DINPUT
#include	INCL_KCORE(Includes_PC/Includes_PC)
#include	INCL_KINPUT(Input/ForInputAbc)

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

			private:
				static BOOL CALLBACK GetNbDevicesCallback(const DIDEVICEINSTANCE *pCurrentDevice, void *pContext);
				static BOOL CALLBACK GetDevicesCallback(const DIDEVICEINSTANCE *pCurrentDevice, void *pContext);
				
				int				_nCurrentDevices;
				int				_nNbDevices;
				LPDIRECTINPUT8	_pDI;
				int				_nFlags;
				HWND			_WndHandle;
		};
	}
}

#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================