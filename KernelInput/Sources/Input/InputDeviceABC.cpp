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
//	CLASS:	INPUT_DEVICE_ABC
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/InputDeviceABC)

#include	INCL_KINPUT(Input/IDeviceParam)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE_ABC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_ABC::INPUT_DEVICE_ABC()
:
_nNbAbsAxis(0),
_nNbRelAxis(0),
_nNbPushButton(0),
_nNbToggleButton(0),
_nFlags(FLG_VIBRATION_ENABLE),
_strDeviceName("unknown device name")
{
	_tVectObserver.Reserve(512);
	_tVectInputObjects.Reserve(32);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE_ABC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_ABC::INPUT_DEVICE_ABC(const INPUT_DEVICE_ABC & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_DEVICE_ABC destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_ABC::~INPUT_DEVICE_ABC()
{
	ClearAllInputObject();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_ABC &	INPUT_DEVICE_ABC::operator=(const INPUT_DEVICE_ABC & C)
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
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::Copy(const INPUT_DEVICE_ABC & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		AddInputObject
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::AddInputObject(INPUT_OBJECT_ABC *	pInputObj)
{
	QDT_ASSERT(pInputObj);

	_tVectInputObjects.PushTail(pInputObj);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveInputObject
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	INPUT_DEVICE_ABC::RemoveInputObject(INPUT_OBJECT_ABC *	pInputObj)
{
	QDT_ASSERT( pInputObj );
	if( pInputObj )
	{
		QDT_VECTOR< INPUT_OBJECT_ABC*>::ITERATOR ItFound = _tVectInputObjects.Find( _tVectInputObjects.GetHead(), pInputObj );
		if( ItFound != _tVectInputObjects.GetTail() )
		{	
			delete (*ItFound);
			_tVectInputObjects.Remove( ItFound );
			return( true );
		}
		else
		{
			return( false );
		}
	}
	else
	{
		return( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrInputObject
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_ABC::GetNbrInputObject() const
{
	return	(_tVectInputObjects.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetInputObject
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT_ABC *	INPUT_DEVICE_ABC::GetInputObject(const int	nIndex)
{
	return	(_tVectInputObjects[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		ClearAllInputObject
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::ClearAllInputObject()
{
	_tVectInputObjects.ClearAll();
}

//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE::AddObserver
// Object: 
//01-07-25 FKE Created: 
//-----------------------------------------------------------------------------
bool INPUT_DEVICE_ABC::AddObserver(IDEVICE_PARAM	*pDeviceObs)
{

	QDT_ASSERT( pDeviceObs );
	
	//if not null and not already in
	bool bAlreadyIn = (_tVectObserver.Find( _tVectObserver.GetHead(), pDeviceObs) != _tVectObserver.GetTail());
	if( pDeviceObs && (bAlreadyIn == false) )
	{
		//add
		_tVectObserver.PushTail( pDeviceObs );
		return true;
	}
	else //yell !
	{
		QDT::KCORE::QDT_Warning(QDT_COMMENT("device Observer déjà utilisé !"));
		return false;
	}
}

//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE
// Object: 
//01-07-25 FKE Created: 
//-----------------------------------------------------------------------------
bool INPUT_DEVICE_ABC::RemoveObserver(IDEVICE_PARAM	*pDeviceObs)
{

	QDT_ASSERT( pDeviceObs );

	if( pDeviceObs )
	{
		QDT_VECTOR<IDEVICE_PARAM*>::ITERATOR itFound = _tVectObserver.Find( _tVectObserver.GetHead(), pDeviceObs );
		
		//if found, remove
		if( itFound != _tVectObserver.GetTail() )
		{
			delete (*itFound);
			_tVectObserver.Remove( itFound );
			return( true );
		}
		else
		{
			//QDT::KRN::QDT_Warning(QDT_COMMENT("INPUT_DEVICE_ABC::RemoveObserver IDEVICE_PARAM not found"));
			return false;
		}
	}
	else
	{
		QDT::KCORE::QDT_Warning(QDT_COMMENT("INPUT_DEVICE_ABC::RemoveObserver IDEVICE_PARAM is null"));
		return false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_ABC::GetNbrObserver() const
{
	return	(_tVectObserver.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetDeviceObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
IDEVICE_PARAM *	INPUT_DEVICE_ABC::GetDeviceObserver(const int	nIndex)
{
	return	(_tVectObserver[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		TransferObserversTo
//	Object:		
//	05-04-04:	JPE - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::TransferObserversTo(INPUT_DEVICE_ABC *	pTarget)
{
	while( _tVectObserver.GetSize() )
	{
		pTarget->AddObserver( _tVectObserver[0] );
		_tVectObserver.RemoveIndex( 0 );
	}
}

//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE_ABC
// Object: 
//01-07-17 FKE Created: 
//-----------------------------------------------------------------------------
bool INPUT_DEVICE_ABC::AddObjectObserver(INPUT_OBJECT_ABC::OBJ_TYPE ObjectType, int nObjectIndex, IOBJECT_PARAM *pObjObs)
{
	int nNbObject = _nNbRelAxis + _nNbAbsAxis + _nNbPushButton + _nNbToggleButton;
	int i;
	for (i = 0; i < nNbObject; i++)
	{
		if (ObjectType == GetInputObject(i)->GetType())
		{
			if (nObjectIndex == 0)
			{
				break;
			}
			nObjectIndex--;
		}
	}
	if (i == nNbObject)
	{
		return false;
	}
	GetInputObject(i)->AddObserver(pObjObs);
	return true;
}

//-----------------------------------------------------------------------------
// Name: INPUT_DEVICE_ABC
// Object: 
//01-07-25 FKE Created: 
//-----------------------------------------------------------------------------
bool INPUT_DEVICE_ABC::RemoveObjectObserver(IOBJECT_PARAM *pObjObs)
{
	int nNbObject = _nNbRelAxis + _nNbAbsAxis + _nNbPushButton + _nNbToggleButton;
	for (int i = 0;i < nNbObject;i++)
	{
		if (GetInputObject(i)->RemoveObserver(pObjObs))
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		SetDeviceName
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::SetDeviceName(const QDT_STRING &	strDeviceName)
{
	_strDeviceName = strDeviceName;
}

//-----------------------------------------------------------------------------
//	Name:		GetDeviceName
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INPUT_DEVICE_ABC::GetDeviceName() const
{
	return	(_strDeviceName);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRelativeAxes
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_ABC::GetNbrRelativeAxes() const
{
	return	( _nNbRelAxis );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrAbsoluteAxes
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_ABC::GetNbrAbsoluteAxes() const
{
	return	( _nNbAbsAxis );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPushButtons
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_ABC::GetNbrPushButtons() const
{
	return	( _nNbPushButton );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrToggleButtons
//	Object:		
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_DEVICE_ABC::GetNbrToggleButtons() const
{
	return	( _nNbToggleButton );
}

//-----------------------------------------------------------------------------
//	Name:		DisplayEventVal
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::DisplayEventVal()
{
	if (_nFlags & FLG_DISPLAY_EVENT_VAL)
	{
		_nFlags &= ~FLG_DISPLAY_EVENT_VAL;
	}
	else
	{
		_nFlags |= FLG_DISPLAY_EVENT_VAL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Has
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
bool	INPUT_DEVICE_ABC::Has(INPUT_DEVICE_INFO &	DeviceInfo)
{
	return	(	( DeviceInfo.GetNbrXAbsAxes()	<= _DeviceInfo.GetNbrXAbsAxes() )
			&&	( DeviceInfo.GetNbrYAbsAxes()	<= _DeviceInfo.GetNbrYAbsAxes() )
			&&	( DeviceInfo.GetNbrZAbsAxes()	<= _DeviceInfo.GetNbrZAbsAxes() )
			&&	( DeviceInfo.GetNbrXRelAxes()	<= _DeviceInfo.GetNbrXRelAxes() )
			&&	( DeviceInfo.GetNbrYRelAxes()	<= _DeviceInfo.GetNbrYRelAxes() )
			&&	( DeviceInfo.GetNbrZRelAxes()	<= _DeviceInfo.GetNbrZRelAxes() )
			&&	( DeviceInfo.GetNbrRXAxes()		<= _DeviceInfo.GetNbrRXAxes() )
			&&	( DeviceInfo.GetNbrRYAxes()		<= _DeviceInfo.GetNbrRYAxes() )
			&&	( DeviceInfo.GetNbrRZAxes()		<= _DeviceInfo.GetNbrRZAxes() )
			&&	( DeviceInfo.GetNbrSliderAxes() <= _DeviceInfo.GetNbrSliderAxes() )
			&&	( DeviceInfo.GetNbrButtons()	<= _DeviceInfo.GetNbrButtons() )
			&&	( DeviceInfo.GetNbrKeys()		<= _DeviceInfo.GetNbrKeys() )
			&&	( DeviceInfo.GetNbrPOVs()		<= _DeviceInfo.GetNbrPOVs() ) );
}

//-----------------------------------------------------------------------------
//	Name:		DispatchEvent
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::DispatchEvent(int		nObjectType,
										int		nIndex,
										float	rVal)
{
	int				j , nNbrDeviceObs;
	IDEVICE_PARAM	*pDeviceParam;

	nNbrDeviceObs = GetNbrObserver();
	for (j = 0;j < nNbrDeviceObs; ++j)
	{	
		pDeviceParam = GetDeviceObserver(j);
		QDT_ASSERT(pDeviceParam);
				
		if (pDeviceParam->IsEventActive())
		{
			pDeviceParam->ProcessEvent(nObjectType, nIndex, rVal);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		DispatchConnectEvent
//	Object:		
//	04-03-09:	SBE - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::DispatchConnectEvent()
{
	int				j , nNbrDeviceObs;
	IDEVICE_PARAM	*pDeviceParam;

	nNbrDeviceObs = GetNbrObserver();
	for (j = 0;j < nNbrDeviceObs; ++j)
	{	
		pDeviceParam = GetDeviceObserver(j);
		QDT_ASSERT(pDeviceParam);
				
		if (pDeviceParam->IsEventActive())
		{
			pDeviceParam->ProcessConnectEvent();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		DispatchDisconnectEvent
//	Object:		
//	04-03-09:	SBE - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::DispatchDisconnectEvent()
{
	int				j , nNbrDeviceObs;
	IDEVICE_PARAM	*pDeviceParam;

	nNbrDeviceObs = GetNbrObserver();
	for (j = 0;j < nNbrDeviceObs; ++j)
	{	
		pDeviceParam = GetDeviceObserver(j);
		QDT_ASSERT(pDeviceParam);
				
		if (pDeviceParam->IsEventActive())
		{
			pDeviceParam->ProcessDisconnectEvent();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateDeviceInfo
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::UpdateDeviceInfo(const int	nObjectType)
{
	switch	( nObjectType )
	{
	case INPUT_OBJECT_ABC::OBJ_X_AXIS_REL:
		_DeviceInfo.SetNbrXRelAxes( _DeviceInfo.GetNbrXRelAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_Y_AXIS_REL:
		_DeviceInfo.SetNbrYRelAxes( _DeviceInfo.GetNbrYRelAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_Z_AXIS_REL:
		_DeviceInfo.SetNbrZRelAxes( _DeviceInfo.GetNbrZRelAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_X_AXIS_ABS:
		_DeviceInfo.SetNbrXAbsAxes( _DeviceInfo.GetNbrXAbsAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_Y_AXIS_ABS:
		_DeviceInfo.SetNbrYAbsAxes( _DeviceInfo.GetNbrYAbsAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_Z_AXIS_ABS:
		_DeviceInfo.SetNbrZAbsAxes( _DeviceInfo.GetNbrZAbsAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_RX_AXIS:
		_DeviceInfo.SetNbrRXAxes( _DeviceInfo.GetNbrRXAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_RY_AXIS:
		_DeviceInfo.SetNbrRYAxes( _DeviceInfo.GetNbrRYAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_RZ_AXIS:
		_DeviceInfo.SetNbrRZAxes( _DeviceInfo.GetNbrRZAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS:
		_DeviceInfo.SetNbrSliderAxes( _DeviceInfo.GetNbrSliderAxes() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_BUTTON:
		_DeviceInfo.SetNbrButtons( _DeviceInfo.GetNbrButtons() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_KEY:
		_DeviceInfo.SetNbrKeys( _DeviceInfo.GetNbrKeys() + 1 );
		break;

	case INPUT_OBJECT_ABC::OBJ_POV:
		_DeviceInfo.SetNbrPOVs( _DeviceInfo.GetNbrPOVs() + 1 );
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDeviceInfo
//	Object:		
//	03-03-13:	DAC - Created
//-----------------------------------------------------------------------------
const INPUT_DEVICE_INFO &	INPUT_DEVICE_ABC::GetDeviceInfo() const
{
	return	(_DeviceInfo);
}

//-----------------------------------------------------------------------------
//	Name:		EnableVibration
//	Object:		
//	04-03-19:	VMA - Created
//-----------------------------------------------------------------------------
void	INPUT_DEVICE_ABC::EnableVibration(bool	_bEnable)
{
	if ( _bEnable )
	{
		_nFlags |= FLG_VIBRATION_ENABLE;
	}
	else
	{
		Vibrate(0,0);
		_nFlags &= ~FLG_VIBRATION_ENABLE;
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsVibrationEnabled
//	Object:		
//	04-03-19:	VMA - Created
//-----------------------------------------------------------------------------
bool	INPUT_DEVICE_ABC::IsVibrationEnabled()
{
	return ( (_nFlags & FLG_VIBRATION_ENABLE) != 0 );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
