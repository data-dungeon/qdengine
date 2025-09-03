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
//	02-06-26:	DAC - Created
//*****************************************************************************

#include "Root.h"
#include INCL_KINPUT(Input/PS3/InputDevice)

#include INCL_KMATH(Math/Math)
#include INCL_KINPUT(Input/PS3/InputObject)
#include INCL_KINPUT(Input/PS3/PadManager_PS3)
#include INCL_KINPUT(Input/PS3/InputInitInfoPS3)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE constructor
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE::INPUT_DEVICE()
{
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE constructor
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE::INPUT_DEVICE(const INPUT_DEVICE & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE destructor
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE::~INPUT_DEVICE()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE &	INPUT_DEVICE::operator=(const INPUT_DEVICE & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE::Copy(const INPUT_DEVICE & C)
{
	QDT_NOT_IMPLEMENTED();
}


//-----------------------------------------------------------------------------
//	Name:		Initialize
//	Object:		
//	04-03-08:	SBE - Created
//-----------------------------------------------------------------------------
bool	INPUT_DEVICE::Initialize(int	nSlot,
								 int	nPort)
{
	unsigned int				i;
	int							j;
	INPUT_OBJECT				*pInputObj;

	_nNbAbsAxis = 0;
	_nNbRelAxis = 0;
	_nNbPushButton = 0;
	_nNbToggleButton = 0;
	_nFlags = 0;

	_pPad = PAD_MANAGER_PS3::GetInstance()->GetPad(nPort);
	if (_pPad)	_pPadDesc = _pPad->GetDescriptor();

	if ( (_pPad == NULL) || (_pPadDesc == NULL) )
	{
		_nFlags &= ~FLG_INITIALIZED;

		return false;
	}
	
	_nFlags |= FLG_INITIALIZED;
	
	if (_tVectInputObjects.GetSize() == 0)
	{
		for	(j = 0; j < _pPadDesc->nAxisCount + _pPadDesc->nButtonsCount; ++j)
		{
			AddInputObject(new INPUT_OBJECT());
		}
	}

	for (i = 0;i < _pPadDesc->nButtonsCount;i++)
	{
		pInputObj =  static_cast<INPUT_OBJECT*>(GetInputObject(i));

		PAD_PS3::DESCRIPTOR * pDesc = &_pPadDesc->pButtonsDesc[i];
		
		switch (pDesc->nType & 3)
		{
		case BUTTONTYPE_TOGGLE:
			_nNbToggleButton++;
			pInputObj->SetType(INPUT_OBJECT_ABC::OBJ_BUTTON);
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_BUTTON );
			break;
		case BUTTONTYPE_PUSH: 
			_nNbPushButton++;
			pInputObj->SetType(INPUT_OBJECT_ABC::OBJ_BUTTON);
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_BUTTON );
			break;
		}

		pInputObj->SetObjectName(pDesc->pName);
		pInputObj->SetReadHandle(pDesc->nID);
	}
	for (i = 0;i < _pPadDesc->nAxisCount;i++)
	{
		PAD_PS3::DESCRIPTOR * pDesc = &_pPadDesc->pAxisDesc[i];

		switch (pDesc->nType & 3)
		{
		case AXISTYPE_RELATIVE:
			_nNbRelAxis++;
			break;
		case AXISTYPE_ABSOLUTE:
			_nNbAbsAxis++;
			break;
		}
		switch (pDesc->nID)
		{
		case PAD_PS3::AT_RIGHT_HORZ:
			GetInputObject(i + _pPadDesc->nButtonsCount)->SetType(INPUT_OBJECT_ABC::OBJ_RZ_AXIS);
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_RZ_AXIS );
			break;
		case PAD_PS3::AT_RIGHT_VERT:
			GetInputObject(i + _pPadDesc->nButtonsCount)->SetType(INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS);
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS );
			break;
		case PAD_PS3::AT_LEFT_HORZ:
			GetInputObject(i + _pPadDesc->nButtonsCount)->SetType(INPUT_OBJECT_ABC::OBJ_X_AXIS_ABS);
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_X_AXIS_ABS );
			break;
		case PAD_PS3::AT_LEFT_VERT:
			GetInputObject(i + _pPadDesc->nButtonsCount)->SetType(INPUT_OBJECT_ABC::OBJ_Y_AXIS_ABS);
			UpdateDeviceInfo( INPUT_OBJECT_ABC::OBJ_Y_AXIS_ABS );
			break;
		default:
//			QDT_Error("FOR_INPUT::DEVICE::Init - Axis NotExist!!!!");
			break;
		}
		
		pInputObj =  static_cast<INPUT_OBJECT*>(GetInputObject(i + _pPadDesc->nButtonsCount));
		
		pInputObj->SetObjectName(pDesc->pName);
		pInputObj->SetReadHandle(pDesc->nID);
	}	

	return true;
}


//-----------------------------------------------------------------------------
// Name: Init
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
bool INPUT_DEVICE::Init(const INPUT_INIT_INFO* pInitInfo)
{
	const INPUT_INIT_INFO_PS3	*pInitInf;

	pInitInf = static_cast<const INPUT_INIT_INFO_PS3*>(pInitInfo);
	QDT_ASSERT(pInitInf);

	_nSlot = pInitInf->GetSlot();
	_nPort = pInitInf->GetPort();

	return Initialize(_nSlot, _nPort);
}

//-----------------------------------------------------------------------------
// Name: FOR_INPUT::DEVICE
// Object: 
//01-06-22 FKE Created: 
//-----------------------------------------------------------------------------
void INPUT_DEVICE::GetState(int handle)
{
	int				i, j;
	INPUT_OBJECT	*pInputObj;

	if (!(_nFlags & FLG_INITIALIZED))
	{
		if (!Initialize(_nSlot, _nPort))
		{
			DispatchDisconnectEvent();
		}
		else
		{
			_nFlags |= FLG_CONNECTION;
		}
	}

	if (_nFlags & FLG_CONNECTION)
	{
		_nFlags &= ~FLG_CONNECTION;
		DispatchConnectEvent();
	}

	Float32 State, NewState;

	for (i = 0;i < _nNbPushButton + _nNbToggleButton;i++)
	{
		pInputObj = static_cast<INPUT_OBJECT*>( GetInputObject(i) );

		State = pInputObj->GetState();
		NewState = (Float32)(_pPad->IsButtonDown((PAD_PS3::BUTTON_TYPE) pInputObj->GetReadHandle())); // todo pressure qd devkit final

		if (State != NewState)
		{
			pInputObj->DispatchEvent(NewState);
			pInputObj->SetState((char)NewState);
			
			DispatchEvent( pInputObj->GetType(), i, NewState);
		}			
	}

	for (;i < _nNbRelAxis + _nNbAbsAxis + _nNbPushButton + _nNbToggleButton;i++)
	{
		pInputObj = static_cast<INPUT_OBJECT*>( GetInputObject(i) );

		State = pInputObj->GetState();
		NewState = (Float32)(_pPad->GetAnalogPosition((PAD_PS3::ANALOG_TYPE) pInputObj->GetReadHandle()) - 128);

		if (State != NewState)
		{
			if (MATH::Abs(NewState) >= AXIS_DEAD_ZONE)
			{
				// Remap range [AXIS_DEAD_ZONE;128] to range [0;128] so we dont lose information
				#define FACTOR ( 128.f / ( 128.f - AXIS_DEAD_ZONE) )
				if (NewState > 0.0f)		NewState = ( FACTOR *(NewState - AXIS_DEAD_ZONE) ) / 128.f;
				else if (NewState < 0.0f)	NewState = ( FACTOR *(NewState + AXIS_DEAD_ZONE) ) / 128.f;
			}
			else
			{
				NewState = 0.0f;
			}

			QDT_ASSERT( NewState >= -1.f && NewState <= 1.f );
			pInputObj->SetState(NewState);

			pInputObj->DispatchEvent(NewState);
			DispatchEvent( pInputObj->GetType(), i, NewState);
		}			
	} 
}

//-----------------------------------------------------------------------------
//	Name:		Flush
//	Object:		
//	02-07-10:	SBE - Created
//-----------------------------------------------------------------------------
bool	INPUT_DEVICE::Flush()
{

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
//		_pInstance->SetPadActuatorFrequency(_nPort, _nSlot, 1, nSmallEngine);
//		_pInstance->SetPadActuatorFrequency(_nPort, _nSlot, 2, nBigEngine);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
