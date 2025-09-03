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
//	CLASS:	PAD_PS3
//
//	05-10-05:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/PS3/Pad_PS3)

#define QDT_INCLUDE_STRING
#include INCL_KCORE(Includes/Includes)

// Included by the .h - not sure how theses includes behave, so i keep it commented right now
//#define QDT_INCLUDE_PS3_PAD
//#include INCL_KCORE(Includes_PS3/Includes_PS3)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PAD_PS3 constructor
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
PAD_PS3::PAD_PS3()
:
_bUsed(false),
_nStatus(0),
_nPort(0xFF)
{
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
void	PAD_PS3::Update(CellPadInfo *	pPadInfo)
{

	if ( (_bUsed == false) || (pPadInfo->status[_nPort] == 0) )
	{
		return;
	}

	CellPadData PadData;

	// si on arrive ici c'est qu'un pad est connecté ET qu'on en a besoin
    if (_nStatus == 0) 
	{
        //printf ("New gamepad %d is connected: VENDOR_ID=%d PRODUCT_ID=%d\n", _nPort, pPadInfo->vendor_id[_nPort], pPadInfo->product_id[_nPort]);
    }

    if (cellPadGetData(_nPort, &PadData) == CELL_PAD_ERROR_UNSUPPRTED_GAMEPAD)
	{
        if (cellPadGetData(_nPort, &PadData) != 0)
		{
			return;
		}
	}

	QDT_MEMCOPY(&_OldState, &_CurState, sizeof(STATE));

    if (PadData.len == 0)
	{
        return;
	}

	if ( pPadInfo->vendor_id[_nPort] == 2883 )
	{
		// PS2 pad with Super Box 3
		
		_CurState._Buttons =  (((PadData.button[1] & (1<<0)) ? 1 : 0) << 0)   // BT_SELECT
							| (((PadData.button[1] & (1<<3)) ? 1 : 0) << 1)   // BT_L3
							| (((PadData.button[1] & (1<<2)) ? 1 : 0) << 2)   // BT_R3
							| (((PadData.button[1] & (1<<1)) ? 1 : 0) << 3)   // BT_START
							| (((PadData.button[1] & (1<<4)) ? 1 : 0) << 4)   // BT_UP
							| (((PadData.button[1] & (1<<7)) ? 1 : 0) << 5)   // BT_LEFT
							| (((PadData.button[1] & (1<<6)) ? 1 : 0) << 6)   // BT_DOWN
							| (((PadData.button[1] & (1<<5)) ? 1 : 0) << 7)   // BT_RIGHT

							| (((PadData.button[0] & (1<<4)) ? 1 : 0) << 8)   // BT_L2
							| (((PadData.button[0] & (1<<5)) ? 1 : 0) << 9)   // BT_R2
							| (((PadData.button[0] & (1<<6)) ? 1 : 0) << 10)  // BT_L1
							| (((PadData.button[0] & (1<<7)) ? 1 : 0) << 11)  // BT_R1
							| (((PadData.button[0] & (1<<0)) ? 1 : 0) << 12)  // BT_TRIANGLE
							| (((PadData.button[0] & (1<<1)) ? 1 : 0) << 13)  // BT_CIRCLE
							| (((PadData.button[0] & (1<<2)) ? 1 : 0) << 14)  // BT_CROSS
							| (((PadData.button[0] & (1<<3)) ? 1 : 0) << 15); // BT_SQUARE

		_CurState._AnalogPositions[AT_LEFT_HORZ] = PadData.button[2];
		_CurState._AnalogPositions[AT_LEFT_VERT] = PadData.button[3];
		_CurState._AnalogPositions[AT_RIGHT_HORZ] = PadData.button[4];
		_CurState._AnalogPositions[AT_RIGHT_VERT] = PadData.button[5];

		if (PadData.button[6] == 0x73)	_CurState._Buttons |= (1 << BT_ANALOG);
		else							_CurState._Buttons &= ~(1 << BT_ANALOG);
	}
	else
	{
		// PS3 pad
		_CurState._Buttons = PadData.button[2] | (PadData.button[3] << 8);

		_CurState._AnalogPositions[AT_RIGHT_HORZ] = PadData.button[4];
		_CurState._AnalogPositions[AT_RIGHT_VERT] = PadData.button[5];
		_CurState._AnalogPositions[AT_LEFT_HORZ] = PadData.button[6];
		_CurState._AnalogPositions[AT_LEFT_VERT] = PadData.button[7];

		if (PadData.button[1] == 0x73)	_CurState._Buttons |= (1 << BT_ANALOG);
		else							_CurState._Buttons &= ~(1 << BT_ANALOG);
	}

	_nStatus = pPadInfo->status[_nPort];
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonDown
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
Bool	PAD_PS3::IsButtonDown(BUTTON_TYPE	BT) const
{
	return (_CurState._Buttons & (1 << BT));
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonUp
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
Bool	PAD_PS3::IsButtonUp(BUTTON_TYPE	BT) const
{
	return !(_CurState._Buttons & (1 << BT));
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonPressed
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
Bool	PAD_PS3::IsButtonPressed(BUTTON_TYPE	BT) const
{
	return ( (_CurState._Buttons & (1 << BT)) && ! (_OldState._Buttons & (1 << BT)) );
}

//-----------------------------------------------------------------------------
//	Name:		IsButtonReleased
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
Bool	PAD_PS3::IsButtonReleased(BUTTON_TYPE	BT) const
{
	return ( ! (_CurState._Buttons & (1 << BT)) && (_OldState._Buttons & (1 << BT)) );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnalogPosition
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
UInt8	PAD_PS3::GetAnalogPosition(ANALOG_TYPE	AT) const
{
	return _CurState._AnalogPositions[AT];
}

//-----------------------------------------------------------------------------
//	Name:		GetDescriptor
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
PAD_PS3::PAD_DESCRIPTOR *	PAD_PS3::GetDescriptor() const
{
	static DESCRIPTOR AnalogAxisDesc[AT_NB] = { {AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_RIGHT_HORZ, "RIGHT_HORZ"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_RIGHT_VERT, "RIGHT_VERT"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_LEFT_HORZ,  "LEFT_HORZ"},
												{AXISTYPE_ABSOLUTE | AXISTYPE_ANALOG, AT_LEFT_VERT,  "LEFT_VERT"} };

	static DESCRIPTOR DigitalButtonDesc[BT_NB] = {	{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_TRIANGLE, "TRIANGLE"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_CIRCLE,	"CIRCLE"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_CROSS,	"CROSS"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_SQUARE,	"SQUARE"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_L2,		"L2"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_R2,		"R2"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_L1,		"L1"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_R1,		"R1"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_SELECT,	"SELECT"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_START,	"START"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_R3,		"R3"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_L3,		"L3"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_UP,		"UP"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_RIGHT,	"RIGHT"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_DOWN,		"DOWN"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_LEFT,		"LEFT"}, 
													{BUTTONTYPE_PUSH | BUTTONTYPE_DIGITAL, BT_ANALOG,	"ANALOB"} };

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
