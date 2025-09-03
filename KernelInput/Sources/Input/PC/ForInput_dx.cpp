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
//00-11-09 ECO Created: 
//*****************************************************************************

#include "Root.h"
#include INCL_KINPUT(Input/PC/ForInput_dx)

#include INCL_KINPUT(Input/InputDeviceABC)
#include INCL_KINPUT(Input/PC/InputInitInfoPC)
#include INCL_KINPUT(Input/PC/InputDevice)


#include INCL_KCORE(OS/OS)

//*****************************************************************************
// Construction / destruction
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name: JoystickCallback
//	Object:	Callback function that gets the joystick(s) attached to the PC.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK FOR_INPUT::GetNbDevicesCallback(const DIDEVICEINSTANCE *	pCurrentDevice,
								 void *						pContext ) 
{
	((FOR_INPUT*)pContext)->_nNbDevices++;
	return ( DIENUM_CONTINUE ); 
}

//-----------------------------------------------------------------------------
//	Name: JoystickCallback
//	Object:	Callback function that gets the joystick(s) attached to the PC.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK FOR_INPUT::GetDevicesCallback(const DIDEVICEINSTANCE *	pCurrentDevice,
											void *						pContext ) 
{
	FOR_INPUT	*pForInput;
	INPUT_INIT_INFO_PC initInfo;
	LPDIRECTINPUTDEVICE8 pDID8;
	HRESULT nHr;

	pForInput = (FOR_INPUT*)pContext;

#ifdef BYPASS_DIRECT_INPUT
/*
	switch(pCurrentDevice->dwDevType)
	{
	default:
		{
	nHr = pForInput->_pDI->CreateDevice(pCurrentDevice->guidInstance, &pDID8, NULL ); 
	if (!(FAILED( nHr )))
	{ 
		initInfo.SetDeviceName(pCurrentDevice->tszInstanceName);
		initInfo.SetInputDevice(pDID8);
				if (!pForInput->GetInputDevice(pForInput->_nCurrentDevices)->Init(&initInfo))
				{
					QDT::KRN::QDT_Warning("Can't Init Input device of %s", initInfo.GetDeviceName().GetBuffer());
				}
			}
		}
		break;

	case 1043:
		{ 
			initInfo.SetDeviceName("QD_DEBUG_KEYBOARD");
			initInfo.SetInputDevice(pDID8);
			if (!pForInput->GetInputDevice(pForInput->_nCurrentDevices)->Init(&initInfo))
			{
				QDT::KRN::QDT_Warning("Can't Init Input device of %s", initInfo.GetDeviceName().GetBuffer());
			}
		}
		break;

	case 274:
		{ 
			initInfo.SetDeviceName("QD_DEBUG_MOUSE");
			initInfo.SetInputDevice(pDID8);
		if (!pForInput->GetInputDevice(pForInput->_nCurrentDevices)->Init(&initInfo))
		{
			QDT::KRN::QDT_Warning("Can't Init Input device of %s", initInfo.GetDeviceName().GetBuffer());
		}
	}
		break;
	}*/
	
#else
	nHr = pForInput->_pDI->CreateDevice(pCurrentDevice->guidInstance, &pDID8, NULL ); 
	if (!(FAILED( nHr )))
	{ 
		initInfo.SetDeviceName(pCurrentDevice->tszInstanceName);
		initInfo.SetInputDevice(pDID8);
		if (!pForInput->GetInputDevice(pForInput->_nCurrentDevices)->Init(&initInfo))
		{
			QDT::KCORE::QDT_Warning("Can't Init Input device of %s", initInfo.GetDeviceName().GetBuffer());
		}
	}
#endif
	pForInput->_nCurrentDevices++;
	

	return	(DIENUM_CONTINUE); 
}




//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::FOR_INPUT
//	Object: 
//	XX-XX-XX:	ECO - Created
//-----------------------------------------------------------------------------
FOR_INPUT::FOR_INPUT()
:
_WndHandle      (NULL),
_nFlags         (0),
_pDI            (NULL),
_nNbDevices		(0)
{

}


//-----------------------------------------------------------------------------
// Name: FOR_INPUT::~FOR_INPUT
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
FOR_INPUT::~FOR_INPUT()
{
	Close();
}


//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Init
//	Object: 
//	XX-XX-XX:	ECO - Created
//	01-03-06:	GGO - Rewriten and DX8 updated
//-----------------------------------------------------------------------------
bool FOR_INPUT::Init()
{
	HRESULT nHr;
	int		i;
	// Create DirectInput object:
	nHr = DirectInput8Create( (HINSTANCE) OS::GetAppHandle(),
							  DIRECTINPUT_VERSION,
							  IID_IDirectInput8,
							  (void**)&_pDI,
							  NULL ); 
	if ( FAILED( nHr ) )
	{
		QDT_Warning("Init, Error while creating DirectInput object" );
		return ( false );
	}
	
	_nNbDevices = 0;
	_pDI->EnumDevices( DI8DEVCLASS_ALL, GetNbDevicesCallback, (void*)this, DIEDFL_ATTACHEDONLY );
	if (_nNbDevices == 0)
	{
		QDT_Warning("No Input Devices !!!! Who  want to play ?" );
		return false;
	}
	
	if (GetNbrInputDevice() != 0)
	{
		ClearAllInputDevice();
	}

	//Create empty devices
	for	( i = 0; i < _nNbDevices; ++i)
	{
		AddInputDevice( new INPUT_DEVICE() );
	}

	// RMA : On ne devrait pas utiliser le handle du display pour le son!
	// De toute facon, l'OS ne connaît plus le renderer
	// J'espere que ça va fonctionner...
	// _WndHandle = (HWND) OS::GetDispHandle();
	_WndHandle = (HWND) OS::GetWinHandle();

	_nCurrentDevices = 0;
	_pDI->EnumDevices( DI8DEVCLASS_ALL, GetDevicesCallback, (void*)this, DIEDFL_ATTACHEDONLY );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Close
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool FOR_INPUT::Close()
{
	return	(FOR_INPUT_ABC::Close());
}


//-----------------------------------------------------------------------------
//	Name:		Flush
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT::Flush()
{
	return(FOR_INPUT_ABC::Flush());
}

//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Update
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
void FOR_INPUT::Update()
{
	// Get the window of the engine.
	if (_WndHandle != (HWND) OS::GetDispHandle())
	{
		_WndHandle = (HWND) OS::GetDispHandle();
	}
	if (_WndHandle == NULL)
	{
		return;
	}

	// We get the current active window.
	HWND	hwnd = GetActiveWindow();


	// The engine window is NOT active (in background) so we unacquire all the devices.
	if ( hwnd != _WndHandle )
	{
	}

	// The engine window is active so we try to acquire back all the devices.
	if ( hwnd == _WndHandle )
	{
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
