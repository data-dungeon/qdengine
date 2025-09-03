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
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/PC/InputDevice)

#include	INCL_KINPUT(Input/PC/InputObject)
#include	INCL_KINPUT(Input/PC/InputInitInfoPC)
#include	INCL_KCORE(OS/OS)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//PARAMETER_STACK INPUT_DEVICE::_ParamStack;
#ifdef BYPASS_DIRECT_INPUT
LONG			INPUT_DEVICE::_OldWndProc;
INPUT_DEVICE*	INPUT_DEVICE::_pKeyboard;
#endif


//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
INPUT_DEVICE::INPUT_DEVICE()
:
_pDIDevices(NULL),
_pDIEffect(NULL),
_nNbrForceFeedBack(0)
{

}

//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
INPUT_DEVICE::~INPUT_DEVICE()
{
	if (_nFlags & FLG_INITIALIZED)
	{
		if ( _pDIEffect )
		{
			_pDIEffect->Release();
			_pDIEffect = NULL;
		}

		if( _pDIDevices )
		{
			_pDIDevices->Unacquire();
			_pDIDevices->Release();
			_pDIDevices = NULL;
		}
		
		delete []_pDIObjectDataFormat;
		_pDIObjectDataFormat = NULL;
		
		delete [] _pCorresTab;
		_pCorresTab = NULL;
	}
}

#ifdef BYPASS_DIRECT_INPUT
//-----------------------------------------------------------------------------
//	Name:		WindowProc
//	Object:		
//	05-01-14:	FPR - Created
//-----------------------------------------------------------------------------
LRESULT CALLBACK	INPUT_DEVICE::WindowProc(HWND	hwnd,
											 UINT	uMsg,
											 WPARAM	wParam,
											 LPARAM	lParam)
{
	switch(uMsg)
	{	
	case WM_KEYDOWN:
		{
			_pKeyboard->DispatchEvent(INPUT_OBJECT_ABC::OBJ_KEY, wParam, 1);
		}
		break;

	case WM_KEYUP:
		{
			_pKeyboard->DispatchEvent(INPUT_OBJECT_ABC::OBJ_KEY, wParam, 0);
		}
		break;


	}
	return CallWindowProc((WNDPROC)_OldWndProc, hwnd, uMsg, wParam, lParam);	
}
#endif

//-----------------------------------------------------------------------------
// Name: Init
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
bool INPUT_DEVICE::Init(const INPUT_INIT_INFO* pInitInfo)
{
#ifdef BYPASS_DIRECT_INPUT
	if(pInitInfo->GetDeviceName() == "QD_DEBUG_KEYBOARD")
	{
		int nEachButton;
		_pDIObjectDataFormat = new DIOBJECTDATAFORMAT[256];
		_pCorresTab	= new unsigned char[256];
		_nCurrentObject = 0;
		for	(nEachButton = 0; nEachButton < 126; ++nEachButton)
		{
			GUID guidType = GUID_Key;
			INPUT_OBJECT	*pInputObj = new INPUT_OBJECT;
			QDT_ASSERT(pInputObj);

			pInputObj->SetType(INPUT_OBJECT_ABC::OBJ_KEY);
			pInputObj->SetGuidAndName(&guidType, "QdDebugKey");
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_KEY );

			_pDIObjectDataFormat[_nCurrentObject].pguid = &guidType;
			_pDIObjectDataFormat[_nCurrentObject].dwOfs = nEachButton;
			_pDIObjectDataFormat[_nCurrentObject].dwType = DIDFT_PSHBUTTON | DIDFT_ANYINSTANCE;
			_pDIObjectDataFormat[_nCurrentObject].dwFlags = 0;

			AddInputObject(pInputObj);

			QDT_ASSERT(nEachButton < _nCorresTabSize);
			_pCorresTab[nEachButton] = _nCurrentObject;

			if (_nCurrentOffset <= (int)nEachButton)
			{
				_nCurrentOffset = nEachButton + 1;
			}
			
			_nCurrentObject++;
			
		}

		_pKeyboard = this;

		return (true);
	}
#endif

	int							i;
	const INPUT_INIT_INFO_PC	*pInitInf;
	
	pInitInf = static_cast<const INPUT_INIT_INFO_PC*>(pInitInfo);
	QDT_ASSERT(pInitInf);

	_pDIDevices = pInitInf->GetInputDevice();
	_nNbRelAxis = 0;
	_nNbAbsAxis = 0;
	_nNbPushButton = 0;
	_nNbToggleButton = 0;
	_nCorresTabSize = 0;
	
	SetDeviceName(pInitInf->GetDeviceName());
	
	_pDIDevices->EnumObjects(GetNbAbsAxisCallback, (void*)this, DIDFT_ABSAXIS);
	_pDIDevices->EnumObjects(GetNbRelAxisCallback, (void*)this, DIDFT_RELAXIS);
	_pDIDevices->EnumObjects(GetNbPushButtonsCallback, (void*)this, DIDFT_PSHBUTTON);
	_pDIDevices->EnumObjects(GetNbToggleButtonsCallback, (void*)this, DIDFT_TGLBUTTON);
//pour les autres propriétés, ajouté des enums (genre Force Feedback...)

	_pDIObjectDataFormat = new DIOBJECTDATAFORMAT[_nNbAbsAxis + _nNbRelAxis + _nNbPushButton + _nNbToggleButton];
	
	//_pObject = new OBJECT[_nNbAbsAxis + _nNbRelAxis + _nNbPushButton + _nNbToggleButton];
	for	(i = 0; i < _nNbAbsAxis + _nNbRelAxis + _nNbPushButton + _nNbToggleButton; ++i)
	{
		AddInputObject( new INPUT_OBJECT() );
	}

	_pCorresTab = new unsigned char [_nCorresTabSize];
	_nCurrentObject = 0;
	_nCurrentOffset = 0;
	
	INPUT_OBJECT::NewDevice();

//on doit traité impérativement dans cet ordre (problème d'alignement sinon!!!!)
	_pDIDevices->EnumObjects(GetAbsAxisCallback, (void*)this, DIDFT_ABSAXIS);
	_pDIDevices->EnumObjects(GetRelAxisCallback, (void*)this, DIDFT_RELAXIS);
	_pDIDevices->EnumObjects(GetPushButtonsCallback, (void*)this, DIDFT_PSHBUTTON);
	_pDIDevices->EnumObjects(GetToggleButtonsCallback, (void*)this, DIDFT_TGLBUTTON);

	_DIDataFormat.dwSize = sizeof(_DIDataFormat);
    _DIDataFormat.dwObjSize = sizeof(DIOBJECTDATAFORMAT); 
	if (_nNbRelAxis > 0 && _nNbAbsAxis == 0)
	{
		_DIDataFormat.dwFlags = DIDF_RELAXIS ;
	}
	else
	{
		_DIDataFormat.dwFlags = DIDF_ABSAXIS ;
	}
	_nCurrentOffset += (-_nCurrentOffset) & 3;
    _DIDataFormat.dwDataSize = _nCurrentOffset; 
    _DIDataFormat.dwNumObjs = _nNbAbsAxis + _nNbRelAxis + _nNbPushButton + _nNbToggleButton; 
    _DIDataFormat.rgodf = _pDIObjectDataFormat; 
	HRESULT hr = _pDIDevices->SetDataFormat(&_DIDataFormat);
	
	if (FAILED(hr))
	{
		delete []_pDIObjectDataFormat;
		_pDIObjectDataFormat = NULL;
		
		ClearAllInputObject();

		QDT::KCORE::QDT_Error("Init Input device, pb with data format");
		return false;
	}

	if ( _nFlags & FLG_VIBRATION )
	{
		_pDIDevices->SetCooperativeLevel((HWND)QDT::KCORE::OS::GetWinHandle(), DISCL_EXCLUSIVE|DISCL_BACKGROUND);
	}
	else
	{
		_pDIDevices->SetCooperativeLevel((HWND)QDT::KCORE::OS::GetWinHandle(), DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
	}
		
	DIPROPDWORD dipdw;
	// the header
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	// the data
	dipdw.dwData            = SAMPLE_BUFFER_SIZE;

	hr = _pDIDevices->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
 
	if (FAILED(hr)) 
	{
		delete []_pDIObjectDataFormat;
		_pDIObjectDataFormat = NULL;
		
		ClearAllInputObject();

		return false;
	}

    _pDIDevices->Acquire();
	_nFlags |= FLG_INITIALIZED;

	if ( _nFlags & FLG_VIBRATION )
	{
		////////////

		// This simple sample only supports one or two axis joysticks
		if( _nNbrForceFeedBack > 2 )
			_nNbrForceFeedBack = 2;

		// This application needs only one effect: Applying raw forces.
		DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
		LONG            rglDirection[2] = { 0, 0 };
		DICONSTANTFORCE cf              = { 0 };

		DIEFFECT eff;
		ZeroMemory( &eff, sizeof(eff) );
		eff.dwSize                  = sizeof(DIEFFECT);
		eff.dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration              = INFINITE;
		eff.dwSamplePeriod          = 0;
		eff.dwGain                  = DI_FFNOMINALMAX;
		eff.dwTriggerButton         = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.cAxes                   = _nNbrForceFeedBack;
		eff.rgdwAxes                = rgdwAxes;
		eff.rglDirection            = rglDirection;
		eff.lpEnvelope              = 0;
		eff.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams   = &cf;
		eff.dwStartDelay            = 0;

		// Create the prepared effect
		if( FAILED( hr = _pDIDevices->CreateEffect( GUID_ConstantForce, 
												&eff, &_pDIEffect, NULL ) ) )
		{
			return false;
		}
		///////////
		/*DWORD    dwAxes[2] = { DIJOFS_X, DIJOFS_Y };
		LONG     lDirection[2] = { 18000, 0 };
 
		DICONSTANTFORCE diConstantForce;
		diConstantForce.lMagnitude = DI_FFNOMINALMAX;

		DIEFFECT	diEffect;
		diEffect.dwSize = sizeof(DIEFFECT); 
		diEffect.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS; 
		diEffect.dwDuration = INFINITE; 
		diEffect.dwSamplePeriod = 0; 
		diEffect.dwGain = DI_FFNOMINALMAX; 
		diEffect.dwTriggerButton = DIEB_NOTRIGGER; 
		diEffect.dwTriggerRepeatInterval = INFINITE ;
		diEffect.cAxes = 2; 
		diEffect.rgdwAxes = dwAxes; 
		diEffect.rglDirection = lDirection; 
		diEffect.lpEnvelope = NULL; 
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE); 
		diEffect.lpvTypeSpecificParams = &diConstantForce; 
		diEffect.dwStartDelay = 0; 

		HRESULT hRes = _pDIDevices->CreateEffect( GUID_ConstantForce, &diEffect, &_pDIEffect, NULL );
#ifdef QDT_DEBUG
		switch(hRes)
		{
			case DI_OK:
				//Success
				break;
			case DIERR_DEVICEFULL:
				QDT::KRN::QDT_Warning("IDirectInputDevice8::CreateEffect() DIERR_DEVICEFULL");
				break;
			case DIERR_DEVICENOTREG:
				QDT::KRN::QDT_Warning("IDirectInputDevice8::CreateEffect() DIERR_DEVICENOTREG");
				break;
			case DIERR_INVALIDPARAM: 
				QDT::KRN::QDT_Warning("IDirectInputDevice8::CreateEffect() DIERR_INVALIDPARAM");
				break;
			case DIERR_NOTINITIALIZED:
				QDT::KRN::QDT_Warning("IDirectInputDevice8::CreateEffect() DIERR_NOTINITIALIZED");
				break;
			default:
				QDT::KRN::QDT_Warning("IDirectInputDevice8::CreateEffect() Unknown error");
				break;
		}
#endif
		*/
	}
	
	return true;
}


//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
void INPUT_DEVICE::GetState(int handle)
{
	HRESULT				hr;
	DIDEVICEOBJECTDATA	DiDeObjDat[SAMPLE_BUFFER_SIZE];
	INPUT_OBJECT		*pInputObj;
	int					i, nElements;
	
	if	(_nFlags & FLG_INITIALIZED)
	{
		if	(_pDIDevices == NULL)
		{
			QDT::KCORE::QDT_Error("_pDIDevices is null");
			return;
		}

		if	( FAILED(_pDIDevices->Acquire()) )
		{
			return;
		}

		_pDIDevices->Poll();

		nElements	= SAMPLE_BUFFER_SIZE;
		hr			= _pDIDevices->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), DiDeObjDat, (unsigned long *)&nElements, 0 );
		
		if (FAILED(hr))
		{
			return;
		}
		
		if	(nElements > 0)
		{
			Float32 rObjectValue;
			for	(i = 0; i < nElements; i++)
			{
				pInputObj = static_cast<INPUT_OBJECT*>(GetInputObject(_pCorresTab[DiDeObjDat[i].dwOfs]));
				QDT_ASSERT(pInputObj);

				if (pInputObj->IsAnAxis())
				{
					Int32 nFake = (Int32)DiDeObjDat[i].dwData;
					rObjectValue = (Float32)(nFake);
					// Convert from [-MinValue; MaxValue] to [-1.0f; 1.0f]
					if (rObjectValue > 0.0f)
					{
						rObjectValue /= INPUT_OBJECT_ABC::GetDefaultMaxAbsAxisValue();
					}
					else
					{
						rObjectValue /= -INPUT_OBJECT_ABC::GetDefaultMinAbsAxisValue();
					}

					// Remap range [AXIS_DEAD_ZONE;1.0f] to range [0.0f;1.0f] so we dont lose information
					#define AXIS_DEAD_ZONE		0.1875f
					#define FACTOR				(1.0f /(1.0f-AXIS_DEAD_ZONE))

					if (rObjectValue>AXIS_DEAD_ZONE)	
					{
						rObjectValue = (FACTOR *(rObjectValue - AXIS_DEAD_ZONE));
					}
					else if (rObjectValue<-AXIS_DEAD_ZONE)
					{
						rObjectValue = (FACTOR *(rObjectValue + AXIS_DEAD_ZONE));
					}
					else
					{
						rObjectValue = 0.0f;
					}
				}
				else
				{
					rObjectValue = (float)DiDeObjDat[i].dwData / 128.0f;
				}

			#ifndef _MASTER
				if (_nFlags & FLG_DISPLAY_EVENT_VAL)
				{
					QDT::KCORE::QDT_Message("device %s obj_name: %s obj_type: %s Handle: %i  Val: %f",	GetDeviceName().GetBuffer(), 
																										pInputObj->GetObjectName().GetBuffer(), 
																										pInputObj->GetTypeString().GetBuffer(), 
																										pInputObj->GetHandle(), 
																										rObjectValue);
				}
			#endif

				//Dispatch event for the device observer for the device containing the object 
				//(send object type, index of the object, and the value)
				DispatchEvent( pInputObj->GetType(), pInputObj->GetHandle(), rObjectValue );

				//Dispatch event on the object observer
				pInputObj->DispatchEvent(rObjectValue);
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		Flush
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	INPUT_DEVICE::Flush()
{
	HRESULT hr;
	DIDEVICEOBJECTDATA DiDeObjDat[SAMPLE_BUFFER_SIZE];

	if (_nFlags & FLG_INITIALIZED)
	{
		
		if( _pDIDevices == NULL )
		{
			QDT::KCORE::QDT_Error("_pDIDevices is null");
			return false;
		}

		if (FAILED(_pDIDevices->Acquire()))
		{
			return false;
		}
		_pDIDevices->Poll();

		int nElements = SAMPLE_BUFFER_SIZE;
		hr = _pDIDevices->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), DiDeObjDat, (unsigned long *)&nElements, 0 );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
void INPUT_DEVICE::SetObject(const DIDEVICEOBJECTINSTANCE *pObj, int nOffset, const INPUT_OBJECT_ABC::OBJ_TYPE eObjType)
{
	INPUT_OBJECT	*pInputObj;

	pInputObj = static_cast<INPUT_OBJECT*>(GetInputObject(_nCurrentObject));
	QDT_ASSERT(pInputObj);

	pInputObj->SetType(eObjType);
	pInputObj->SetGuidAndName(&pObj->guidType, pObj->tszName);
	UpdateDeviceInfo( eObjType );

	_pDIObjectDataFormat[_nCurrentObject].pguid = pInputObj->GetGuid();
	_pDIObjectDataFormat[_nCurrentObject].dwOfs = pObj->dwOfs;
	_pDIObjectDataFormat[_nCurrentObject].dwType = pObj->dwType | DIDFT_ANYINSTANCE;
	_pDIObjectDataFormat[_nCurrentObject].dwFlags = 0;
	
	QDT_ASSERT(pObj->dwOfs < _nCorresTabSize);
	_pCorresTab[pObj->dwOfs] = (unsigned char)_nCurrentObject;

	if (_nCurrentOffset <= (int)pObj->dwOfs)
	{
		_nCurrentOffset = pObj->dwOfs + nOffset;
	}
	
	_nCurrentObject++;
}


//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetNbAbsAxisCallback(const DIDEVICEOBJECTINSTANCE *pObj,
													  void *pContext)
{

	INPUT_DEVICE * pDevice = static_cast<INPUT_DEVICE*>(pContext);
	pDevice->_nNbAbsAxis++;
	if (pObj->dwOfs >= ((INPUT_DEVICE	*)pContext)->_nCorresTabSize)
	{
		pDevice->_nCorresTabSize = pObj->dwOfs + 1;
	}

	if ( pObj->dwFlags & DIDOI_FFACTUATOR )
	{
		pDevice->_nFlags |= FLG_VIBRATION;
	}

	return (DIENUM_CONTINUE); 
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetAbsAxisCallback(const DIDEVICEOBJECTINSTANCE *pObj,
													void *pContext )
{
	INPUT_OBJECT_ABC::OBJ_TYPE eObjType;

	if (memcmp(&pObj->guidType, &GUID_XAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_X_AXIS_ABS;
	}
	else if (memcmp(&pObj->guidType, &GUID_YAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_Y_AXIS_ABS;
	}
	else if (memcmp(&pObj->guidType, &GUID_ZAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_Z_AXIS_ABS;
	}
	else if (memcmp(&pObj->guidType, &GUID_RxAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_RX_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_RyAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_RY_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_Slider, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_RzAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_RZ_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_POV, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_POV;
	}
	else
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_UNKNOWN;
		QDT_FAIL();
	}

	((INPUT_DEVICE	*)pContext)->SetObject(pObj, 4, eObjType);
	
	DIPROPRANGE diprg; 
	diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	diprg.diph.dwHow        = DIPH_BYID; 
	diprg.diph.dwObj        = pObj->dwType; 
	diprg.lMin              = INPUT_OBJECT_ABC::GetDefaultMinAbsAxisValue(); 
	diprg.lMax              = INPUT_OBJECT_ABC::GetDefaultMaxAbsAxisValue(); 

	// Increment number of forcefeedback axis
	if( (pObj->dwFlags & DIDOI_FFACTUATOR) != 0 )
	{
		++(((INPUT_DEVICE	*)pContext)->_nNbrForceFeedBack);
	}

	((INPUT_DEVICE	*)pContext)->_pDIDevices->SetProperty(DIPROP_RANGE, &diprg.diph);

	return ( DIENUM_CONTINUE ); 
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetNbRelAxisCallback(const DIDEVICEOBJECTINSTANCE *pObj,
													  void *pContext)
{
	((INPUT_DEVICE	*)pContext)->_nNbRelAxis++;
	if (pObj->dwOfs >= ((INPUT_DEVICE	*)pContext)->_nCorresTabSize)
	{
		((INPUT_DEVICE	*)pContext)->_nCorresTabSize = pObj->dwOfs + 1;
	}
	return	(DIENUM_CONTINUE); 
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetRelAxisCallback(const DIDEVICEOBJECTINSTANCE *pObj,
													void *pContext )
{
	INPUT_OBJECT_ABC::OBJ_TYPE eObjType;

	if (memcmp(&pObj->guidType, &GUID_XAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_X_AXIS_REL;
	}
	else if (memcmp(&pObj->guidType, &GUID_YAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_Y_AXIS_REL;
	}
	else if (memcmp(&pObj->guidType, &GUID_ZAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_Z_AXIS_REL;
	}
	else if (memcmp(&pObj->guidType, &GUID_RxAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_RX_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_RyAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_RY_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_Slider, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_RzAxis, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_RZ_AXIS;
	}
	else if (memcmp(&pObj->guidType, &GUID_POV, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_POV;
	}
	else
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_UNKNOWN;
		QDT_FAIL();
	}

	// Increment number of forcefeedback axis
	if( (pObj->dwFlags & DIDOI_FFACTUATOR) != 0 )
	{
		++(((INPUT_DEVICE	*)pContext)->_nNbrForceFeedBack);
	}

	((INPUT_DEVICE	*)pContext)->SetObject(pObj, 4, eObjType);	
	return	(DIENUM_CONTINUE);
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetNbPushButtonsCallback(const DIDEVICEOBJECTINSTANCE *pObj,
														  void *pContext )
{
	((INPUT_DEVICE	*)pContext)->_nNbPushButton++;
	if (pObj->dwOfs >= ((INPUT_DEVICE	*)pContext)->_nCorresTabSize)
	{
		((INPUT_DEVICE	*)pContext)->_nCorresTabSize = pObj->dwOfs + 1;
	}
	return ( DIENUM_CONTINUE ); 
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetPushButtonsCallback(const DIDEVICEOBJECTINSTANCE *pObj,
														void *pContext )
{
	INPUT_OBJECT_ABC::OBJ_TYPE eObjType;

	if (memcmp(&pObj->guidType, &GUID_Key, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_KEY;
	}
	else if (memcmp(&pObj->guidType, &GUID_Button, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_BUTTON;
	}
	else
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_UNKNOWN;
		QDT_FAIL();
	}

	((INPUT_DEVICE	*)pContext)->SetObject(pObj, 1, eObjType);
	return ( DIENUM_CONTINUE ); 
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetNbToggleButtonsCallback(const DIDEVICEOBJECTINSTANCE *pObj,
															void *pContext )
{
	((INPUT_DEVICE	*)pContext)->_nNbToggleButton++;
	if (pObj->dwOfs >= ((INPUT_DEVICE	*)pContext)->_nCorresTabSize)
	{
		((INPUT_DEVICE	*)pContext)->_nCorresTabSize = pObj->dwOfs + 1;
	}
	return ( DIENUM_CONTINUE );
}

//-----------------------------------------------------------------------------
//	Name: AxisCallback
//	Object:	Callback function that goes through the axis of a joystick
//			and that sets the properties of each axis correctly.
//	01-03-08:	GGO - Created
//-----------------------------------------------------------------------------
BOOL CALLBACK INPUT_DEVICE::GetToggleButtonsCallback(const DIDEVICEOBJECTINSTANCE *pObj,
														  void *pContext )
{
	INPUT_OBJECT_ABC::OBJ_TYPE eObjType;

	if (memcmp(&pObj->guidType, &GUID_Key, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_KEY;
	}
	else if (memcmp(&pObj->guidType, &GUID_Button, sizeof(GUID)) == 0)
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_BUTTON;
	}
	else
	{
		eObjType = INPUT_OBJECT_ABC::OBJ_UNKNOWN;
		QDT_FAIL();
	}

	((INPUT_DEVICE	*)pContext)->SetObject(pObj, 1, eObjType);
	return ( DIENUM_CONTINUE );
}

//-----------------------------------------------------------------------------
//	Name:		Vibrate
//	Object:		
//	04-01-02:	VMA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE::Vibrate(unsigned char	nSmallEngine,
							  unsigned char	nBigEngine)
{
	if ( IsVibrationEnabled() )
	{
		if ( _pDIEffect )
		{
			// Modifying an effect is basically the same as creating a new one, except
			// you need only specify the parameters you are modifying
			LONG rglDirection[2] = { 0, 0 };

			DICONSTANTFORCE cf;

			if( _nNbrForceFeedBack == 1 )
			{
				// If only one force feedback axis, then apply only one direction and 
				// keep the direction at zero
				if ( nSmallEngine > 0 || nBigEngine > 0 )
				{
					cf.lMagnitude = -DI_FFNOMINALMAX + 2 * DI_FFNOMINALMAX * ( 25 * nSmallEngine + 75 * nBigEngine )  / 256000;
				}
				else
				{
					cf.lMagnitude = 0;
				}
				rglDirection[0] = 0;
			}
			else
			{
				// If two force feedback axis, then apply magnitude from both directions 
				rglDirection[0] = 0;
				rglDirection[1] = 0;
			
				if ( nSmallEngine > 0 || nBigEngine > 0 )
				{
					cf.lMagnitude = -DI_FFNOMINALMAX + 2 * DI_FFNOMINALMAX * ( 25 * nSmallEngine + 75 * nBigEngine )  / 256000;
				}
				else
				{
					cf.lMagnitude = 0;
				}

			}

			DIEFFECT eff;
			ZeroMemory( &eff, sizeof(eff) );
			eff.dwSize                = sizeof(DIEFFECT);
			eff.dwFlags               = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
			eff.cAxes                 = _nNbrForceFeedBack;
			eff.rglDirection          = rglDirection;
			eff.lpEnvelope            = 0;
			eff.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
			eff.lpvTypeSpecificParams = &cf;
			eff.dwStartDelay            = 0;

			// Now set the new parameters and start the effect immediately.
			_pDIEffect->SetParameters( &eff, DIEP_DIRECTION |DIEP_TYPESPECIFICPARAMS | DIEP_START );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Acquire
//	Object:		
//	05-02-10:	VMA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE::Acquire()
{
	if( _pDIDevices )
	{
		_pDIDevices->Acquire();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unacquire
//	Object:		
//	05-02-10:	VMA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE::Unacquire()
{
	if( _pDIDevices )
	{
		_pDIDevices->Unacquire();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
