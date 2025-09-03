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
//	CLASS:	INPUT_OBJECT
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/PC/InputObject)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//PARAMETER_STACK INPUT_OBJECT::_ParamStack;
int INPUT_OBJECT::_nObjTypeNum[INPUT_OBJECT::MAX_OBJ_TYPE];

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT::INPUT_OBJECT()
:
_nFlags(0)
{

}

//-----------------------------------------------------------------------------
//	Name:		~INPUT_OBJECT destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT::~INPUT_OBJECT()
{

}

//-----------------------------------------------------------------------------
// Name: SetGuidAndName
// Object: 
//01-07-17 FKE Created: 
//-----------------------------------------------------------------------------
void INPUT_OBJECT::SetGuidAndName(const GUID *pGuid, const char *pName)
{
/*	if (memcmp(pGuid, &GUID_XAxis, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_X_AXIS);
	}
	else if (memcmp(pGuid, &GUID_YAxis, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_Y_AXIS);
	}
	else if (memcmp(pGuid, &GUID_ZAxis, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_Z_AXIS);
	}
	else if (memcmp(pGuid, &GUID_RxAxis, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_RX_AXIS);
	}
	else if (memcmp(pGuid, &GUID_RyAxis, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_RY_AXIS);
	}
	else if (memcmp(pGuid, &GUID_Slider, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS);
	}
	else if (memcmp(pGuid, &GUID_RzAxis, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_RZ_AXIS);
	}
	else if (memcmp(pGuid, &GUID_Button, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_BUTTON);
	}
	else if (memcmp(pGuid, &GUID_Key, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_KEY);
	}
	else if (memcmp(pGuid, &GUID_POV, sizeof(GUID)) == 0)
	{
		SetType(INPUT_OBJECT_ABC::OBJ_POV);
	}
	else
	{
		return;
	}*/
	_Guid = *pGuid;
	SetObjectName(pName);

	_nHandle = _nObjTypeNum[ GetType() ];
	_nObjTypeNum[ GetType() ]++;
}

//-----------------------------------------------------------------------------
//	Name:		GetGuid
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
const GUID *	INPUT_OBJECT::GetGuid() const
{
	return &_Guid;
}

//-----------------------------------------------------------------------------
//	Name:		SetAxis
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::SetAxis() 
{
	_nFlags |= FLG_AXIS; 
	_nFlags &= ~FLG_BUTTON;
}

//-----------------------------------------------------------------------------
//	Name:		SetButton
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::SetButton() 
{
	_nFlags |= FLG_BUTTON; 
	_nFlags &= ~FLG_AXIS;
}

//-----------------------------------------------------------------------------
//	Name:		NewDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::NewDevice() 
{
	memset(_nObjTypeNum, 0, INPUT_OBJECT_ABC::MAX_OBJ_TYPE * sizeof(int));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
