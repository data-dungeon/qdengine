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
//	CLASS:	PAD_MANAGER_PS3
//
//	05-10-05:	SBE - Created
//						GRI : rajout des deep-switch. il faudrait renommer cette classe 
//						en INPUT_PS3_MANAGER
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/PS3/PadManager_PS3)
#include <sys/gpio.h>

// Included by the .h - not sure how theses includes behave, so i keep it commented right now
//#define QDT_INCLUDE_PS3_PAD
//#include INCL_KCORE(Includes_PS3/Includes_PS3)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

PAD_MANAGER_PS3 PAD_MANAGER_PS3::sPadManager;

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_PS3::Init()
{
	if (cellPadInit(MAX_PAD) != 0) 
	{
		// error todo
        //printf ("Initialize error\n");
        return;
    }

	for (UInt32 iPad=0; iPad<MAX_PAD; ++iPad)
	{
		_Pads[iPad]._nPort = iPad;
	}

	UInt64 dip_switch;
	int ret = sys_gpio_get(SYS_GPIO_DIP_SWITCH_DEVICE_ID, &dip_switch);
	if (ret != CELL_OK) 
	{
		//print("Can't read dip switches (%d)\n", ret);
	}
	else
	{
		UInt8 uDIP = ((char*)&dip_switch)[7];
		_OldDIP.Set(uDIP);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_PS3::Close()
{
	if (cellPadEnd() == 0)
	{
		// error todo
        //printf ("Initialize error\n");
        return;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_PS3::Update()
{
    if (cellPadGetInfo(&_PadInfo) != 0)
	{ // todo gestion erreur
        //printf ("Error%d : cellPadGetPadInfo\n", ret);
        return;
    }

	for (UInt32 iPad=0; iPad<MAX_PAD; ++iPad)
	{
		_Pads[iPad].Update(&_PadInfo);

	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPad
//	Object:		
//	05-10-05:	SBE - Created
//------------------------------------------------------	-----------------------
PAD_PS3 *	PAD_MANAGER_PS3::GetPad(UInt32	i)
{
	_Pads[i]._bUsed = true;
	return &_Pads[i]; 
}

//-----------------------------------------------------------------------------
//	Name:		ReleasePad
//	Object:		
//	05-10-05:	SBE - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_PS3::ReleasePad(UInt32	i)
{
	_Pads[i]._bUsed = false;
}


//-----------------------------------------------------------------------------
//	Name:		HasDipSwitchChanged
//	Object:		
//	06-04-26:	GRI - Created
//-----------------------------------------------------------------------------
bool	PAD_MANAGER_PS3::HasDipSwitchChanged(Int32	nNoSwitch)
{
	UInt64 dip_switch;
	int ret = sys_gpio_get(SYS_GPIO_DIP_SWITCH_DEVICE_ID, &dip_switch);
	// seul le dernier octet doit etre pris en compte et de plus les switchs
	// partent de droite a gaucbe
	UInt8 uDIP = ((char*)&dip_switch)[7];
	nNoSwitch = 7 - nNoSwitch;

	if (ret != CELL_OK) 
	{
		//fprintf(stderr, "Can't read dip switches (%d)\n", ret);
		return false;
	}

	bool bChanged = false;
	QDT_CHAR_FLAGS	NewDIP(uDIP);
	if  (NewDIP.IsSet(nNoSwitch) && (!_OldDIP.IsSet(nNoSwitch)) )
	{
		bChanged = true;
	}
	_OldDIP = NewDIP;
	return bChanged;
}


//-----------------------------------------------------------------------------
//	Name:		GetDeepSwitchs
//	Object:		
//	06-04-26:	GRI - Created
//-----------------------------------------------------------------------------
//QDT_FLAGS &	PAD_MANAGER_PS3::GetDeepSwitchs()
//{
//	uint64_t dip_switch;
//	int ret = sys_gpio_get(SYS_GPIO_DIP_SWITCH_DEVICE_ID, &dip_switch);
//	if (ret != CELL_OK) 
//	{
//		fprintf(stderr, "Can't read dip switches (%d)\n", ret);
//		break;
//	}
//	dip_switch = dip_switch & SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS;
//}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
