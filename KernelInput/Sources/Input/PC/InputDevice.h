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
//	CLASS:	INPUT_DEVICE
//	The INPUT_DEVICE class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_PC_INPUTDEVICE_H__
#define __KINPUT_INPUT_PC_INPUTDEVICE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PC_DINPUT
#include	INCL_KCORE(Includes_PC/Includes_PC)
#include	INCL_KINPUT(Input/InputDeviceABC)

namespace QDT
{
	namespace KINPUT
	{
		class INPUT_DEVICE : public INPUT_DEVICE_ABC
		{
		public:
		//les callbacks!!
			static BOOL CALLBACK GetNbPushButtonsCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetPushButtonsCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetNbToggleButtonsCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetToggleButtonsCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetNbRelAxisCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetRelAxisCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetNbAbsAxisCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);
			static BOOL CALLBACK GetAbsAxisCallback( const DIDEVICEOBJECTINSTANCE *pCurrentAxis, void *pContext);


					INPUT_DEVICE();
					~INPUT_DEVICE();

			virtual bool	Init(const INPUT_INIT_INFO* pInitInfo);

			virtual void	GetState(int handle);
			virtual bool	Flush();
			virtual void	Vibrate( unsigned char nSmallEngine, unsigned char nBigEngine );
			virtual void	Acquire();
			virtual void	Unacquire();
		#ifdef BYPASS_DIRECT_INPUT
			static	LRESULT CALLBACK		WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static LONG						_OldWndProc;
			static INPUT_DEVICE*			_pKeyboard;
		#endif	
		private:
			
			void	SetObject(const DIDEVICEOBJECTINSTANCE *pObj, int nOffset, const INPUT_OBJECT_ABC::OBJ_TYPE eObjType);


			unsigned char	*_pCorresTab;
			unsigned long	_nCorresTabSize;

			int				_nNbrForceFeedBack;

			LPDIRECTINPUTDEVICE8	_pDIDevices;
			LPDIRECTINPUTEFFECT		_pDIEffect;
			DIDATAFORMAT			_DIDataFormat;
			DIOBJECTDATAFORMAT		*_pDIObjectDataFormat;
			
			unsigned long	_nCurrentObject;
			int				_nCurrentOffset;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
