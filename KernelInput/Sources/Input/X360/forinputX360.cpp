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
//	03-05-05:	JPE - Created
//*****************************************************************************

#include "Root.h"
#include INCL_KINPUT(Input/X360/ForInputX360)
#include INCL_KCORE(Tools/QdtString)
#include INCL_KINPUT(Input/X360/InputInitInfoX360)
#include INCL_KINPUT(Input/X360/PadManagerX360)
#include INCL_KINPUT(Input/X360/InputDeviceX360)

//*****************************************************************************
// Construction / destruction
//*****************************************************************************
//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::FOR_INPUT
//	Object: 
//	XX-XX-XX:	ECO - Created
//-----------------------------------------------------------------------------
FOR_INPUT::FOR_INPUT()
:
_nNbDevices(0),
_nFlags( 0 )
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
	int					i;
	int					NbSlot = 0;
	int					k;
	int					j;
	QDT_STRING			sName;
	INPUT_INIT_INFO_X360	initInfo;

	PAD_MANAGER_X360::GetInstance()->Init();

	NbSlot = PAD_MANAGER_X360::GetInstance()->GetNbPadMax();
	
	//create devices
	for	(k = 0; k < NbSlot; ++k)
	{
		AddInputDevice(new INPUT_DEVICE());

		sName.Format("Pad %i", _nNbDevices);
		
		initInfo.SetDeviceName(sName);
		initInfo.SetPort(k);
		initInfo.SetSlot(0); // todo slot

		GetInputDevice(_nNbDevices)->Init(&initInfo);
		_nNbDevices++;
	}

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Loop
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool FOR_INPUT::Loop()
{
	//X360 process
	PAD_MANAGER_X360::GetInstance()->Update();

	//std Kernel Input process
	FOR_INPUT_ABC::Loop();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Close
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool FOR_INPUT::Close()
{
	PAD_MANAGER_X360::GetInstance()->Close();

	return	(FOR_INPUT_ABC::Close());
}

//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Update
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
void FOR_INPUT::Update()
{
	// Nothing to update here
}

//-----------------------------------------------------------------------------
//	Name:		Flush
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT::Flush() 
{
	return true;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
