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
//	CLASS:	PAD_X360
//
//	06-29-06:	VMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/X360/PadX360)

#define QDT_INCLUDE_STRING
#include INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PAD_X360 constructor
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
PAD_X360::PAD_X360()
:
_bUsed(false),
_nPort(-1)
{
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
void	PAD_X360::Update()
{

	if ( (_bUsed == false) )
	{
		return;
	}

	memcpy ( &_OldState, &_CurState, sizeof(XINPUT_STATE) );

	if ( _nPort == -1 || XInputGetState( _nPort, &_CurState) != ERROR_SUCCESS )
	{
		for (UInt8 nPort = 0 ; nPort < MAX_PAD ;  nPort++)
		{
			if ( XInputGetState( nPort, &_CurState ) == ERROR_SUCCESS )
			{
				_nPort = nPort;
				_CurState.dwPacketNumber = -1;
				break;
			}

			if ( nPort == MAX_PAD - 1 )
			{
				_nPort = -1;
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonDown
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
Bool	PAD_X360::IsButtonDown(BUTTON_TYPE	BT) const
{
	return ( (_CurState.Gamepad.wButtons & (1 << BT)) != 0);
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonUp
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
Bool	PAD_X360::IsButtonUp(BUTTON_TYPE	BT) const
{
	return ( (_CurState.Gamepad.wButtons & (1 << BT)) == 0);
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonPressed
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
Bool	PAD_X360::IsButtonPressed(BUTTON_TYPE	BT) const
{
	return ( ((_CurState.Gamepad.wButtons & (1 << BT)) != 0) && ( (_OldState.Gamepad.wButtons & (1 << BT)) == 0) );
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonReleased
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
Bool	PAD_X360::IsButtonReleased(BUTTON_TYPE	BT) const
{
	return ( ( (_CurState.Gamepad.wButtons & (1 << BT)) == 0) && ( (_OldState.Gamepad.wButtons & (1 << BT)) != 0) );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnalogPosition
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
Int16	PAD_X360::GetAnalogPosition(ANALOG_TYPE	AT) const
{
	switch(AT)
	{
		case AT_LEFT_HORZ:
			return (_CurState.Gamepad.sThumbLX);

		case AT_LEFT_VERT:
			return (_CurState.Gamepad.sThumbLY);

		case AT_RIGHT_HORZ:
			return (_CurState.Gamepad.sThumbRX);

		case AT_RIGHT_VERT:
			return (_CurState.Gamepad.sThumbRY);

		case AT_LEFT_TRIGGER:
			return (_CurState.Gamepad.bLeftTrigger);

		case AT_RIGHT_TRIGGER:
			return (_CurState.Gamepad.bLeftTrigger);

		default:
			return 0;

	}
}

//-----------------------------------------------------------------------------
//	Name:		Vibrate
//	Object:		
//	06-06-29:	VMA - Created
//-----------------------------------------------------------------------------
void	PAD_X360::Vibrate(UInt8	nSmallEngine,
						  UInt8	nBigEngine)
{
	XINPUT_VIBRATION	Vibration;
	Vibration.wLeftMotorSpeed = nSmallEngine * 256;
	Vibration.wRightMotorSpeed = nBigEngine * 256;
	XInputSetState( _nPort, &Vibration );
}

//-----------------------------------------------------------------------------
//	Name:		GetDescriptor
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
PAD_X360::PAD_DESCRIPTOR *	PAD_X360::GetDescriptor() const
{
	static DESCRIPTOR AnalogAxisDesc[AT_NB] = { {AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_RIGHT_HORZ,	"RIGHT_HORZ"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_RIGHT_VERT,	"RIGHT_VERT"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_LEFT_HORZ,		"LEFT_HORZ"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_LEFT_VERT,		"LEFT_VERT"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_LEFT_TRIGGER,  "LEFT_TRIGGER"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_RIGHT_TRIGGER, "RIGHT TRIGGER"}};

	static DESCRIPTOR DigitalButtonDesc[BT_NB] = {	{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DPAD_UP,			"UP"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DPAD_DOWN,		"DOWN"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DPAD_LEFT,		"LEFT"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DPAD_RIGHT,		"RIGHT"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_START,			"START"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_BACK,				"BACK"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_LEFT_THUMB,		"LEFFT THUMB"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_RIGHT_THUMB,		"RIGHT THUMB"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_LEFT_SHOULDER,	"LEFT SHOULDER"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_RIGHT_SHOULDER,	"RIGHT SHOULDER"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DUMMY1,			"DUMMY1"},
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DUMMY2,			"DUMMY2"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_A,				"A"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_B,				"B"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_X,				"X"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_Y,				"Y"}};

	PAD_DESCRIPTOR * pDesc = new PAD_DESCRIPTOR;

	pDesc->nAxisCount = AT_NB;
	pDesc->pAxisDesc = AnalogAxisDesc;
	pDesc->nButtonsCount = BT_NB;
	pDesc->pButtonsDesc = DigitalButtonDesc;

	return pDesc;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
