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
//	CLASS:	FOR_INPUT_ABC
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include "Root.h"
#include INCL_KINPUT(Input/ForInputABC)

#include INCL_KINPUT(Input/InputDeviceABC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FOR_INPUT_ABC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
FOR_INPUT_ABC::FOR_INPUT_ABC()
{

}

//-----------------------------------------------------------------------------
//	Name:		FOR_INPUT_ABC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
FOR_INPUT_ABC::FOR_INPUT_ABC(const FOR_INPUT_ABC & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		FOR_INPUT_ABC destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
FOR_INPUT_ABC::~FOR_INPUT_ABC()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
FOR_INPUT_ABC &	FOR_INPUT_ABC::operator=(const FOR_INPUT_ABC & C)
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
void	FOR_INPUT_ABC::Copy(const FOR_INPUT_ABC & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		AddInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	FOR_INPUT_ABC::AddInputDevice(INPUT_DEVICE_ABC *	pDevice)
{
	QDT_ASSERT(pDevice);

	_tVectInputDevices.PushTail(pDevice);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::RemoveInputDevice(INPUT_DEVICE_ABC *	pDevice)
{
	QDT_ASSERT( pDevice );

	QDT_VECTOR<INPUT_DEVICE_ABC*>::ITERATOR itFound;

	if	(pDevice)
	{
		itFound = _tVectInputDevices.Find( _tVectInputDevices.GetHead(), pDevice );
		
		//if found, remove
		if( itFound != _tVectInputDevices.GetTail() )
		{
			_tVectInputDevices.Remove( itFound );
			return	(true);
		}
		else
		{
			QDT::KCORE::QDT_Warning(QDT_COMMENT("FOR_INPUT_ABC::RemoveInputDevice INPUT_DEVICE_ABC not found"));
			return	(false);
		}
	}
	else
	{
		QDT::KCORE::QDT_Warning(QDT_COMMENT("FOR_INPUT_ABC::RemoveInputDevice INPUT_DEVICE_ABC is null"));
		return	(false);
	}
}


//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Loop
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool FOR_INPUT_ABC::Loop()
{
	int i, nNbrDevices;
	
	nNbrDevices  = GetNbrInputDevice();
	for (i = 0; i < nNbrDevices; i++)
	{
		GetInputDevice(i)->GetState(i);
	}
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::Close
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool FOR_INPUT_ABC::Close()
{
	ClearAllInputDevice();
	
	return ( true);
}


//-----------------------------------------------------------------------------
//	Name:		Flush
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::Flush()
{
	int i, nNbrDevices;

	nNbrDevices = GetNbrInputDevice();
	for (i = 0; i < nNbrDevices; i++)
	{
		GetInputDevice(i)->Flush();
	}

	return (true);
}


//-----------------------------------------------------------------------------
//	Name:		ClearAllInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	FOR_INPUT_ABC::ClearAllInputDevice()
{
	_tVectInputDevices.ClearAll();
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	FOR_INPUT_ABC::GetNbrInputDevice() const
{
	return	(_tVectInputDevices.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_DEVICE_ABC *	FOR_INPUT_ABC::GetInputDevice(const int	nIndex)
{
	if (nIndex < (Int32)_tVectInputDevices.GetSize() && nIndex >= 0)
	{
		return	(_tVectInputDevices[nIndex]);
	}
	else
	{
		return (NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetInputDevice
//	Object:		
//	03-03-13:	DAC - Created
//-----------------------------------------------------------------------------
const INPUT_DEVICE_ABC *	FOR_INPUT_ABC::GetInputDevice(const int	nIndex) const
{
	return	(_tVectInputDevices[nIndex]);
}


//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::AddDeviceObserver(int nDeviceHandle, IDEVICE_PARAM *pDevParam)
{
	if (nDeviceHandle >= GetNbrInputDevice())
	{
		return false;
	}
	return (GetInputDevice(nDeviceHandle)->AddObserver(pDevParam));
}

//-----------------------------------------------------------------------------
//	Name: FOR_INPUT::
//	Object: 
//	01-03-12:	GGO - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::AddObjectObserver(int nDeviceHandle, INPUT_OBJECT_ABC::OBJ_TYPE ObjectType, int nObjectIndex, IOBJECT_PARAM *pObjObs)
{
	if (nDeviceHandle >= GetNbrInputDevice())
	{
		return false;
	}
	return GetInputDevice(nDeviceHandle)->AddObjectObserver(ObjectType, nObjectIndex, pObjObs);
}

//-----------------------------------------------------------------------------
//	Name:		ForceAllInstancesOnPad
//	Object:		
//	05-04-04:	JPE - Created
//-----------------------------------------------------------------------------
void	FOR_INPUT_ABC::ForceAllInstancesOnPad(int	nTargetPad)
{
	int iDevice;

	if (nTargetPad >= GetNbrInputDevice())
	{
		return;
	}

	INPUT_DEVICE_ABC *pTarget = GetInputDevice(nTargetPad);

	for(iDevice=0; iDevice<GetNbrInputDevice(); iDevice++)
	{
		if(iDevice != nTargetPad)
		{
			GetInputDevice(iDevice)->TransferObserversTo( pTarget );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayEventVal
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::DisplayEventVal(int	handle)
{
	if ( (handle < 0) || (handle > GetNbrInputDevice()) )
	{
		return false;
	}

	GetInputDevice(handle)->DisplayEventVal();

	return true;
}

//-----------------------------------------------------------------------------
// Name: FOR_INPUT::
// Object: 
//01-07-25 FKE Created: 
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::RemoveDeviceObserver(IDEVICE_PARAM *pDevParam)
{
	int i , nNbrDevices;

	nNbrDevices = GetNbrInputDevice();
	for (i = 0; i < nNbrDevices; i++)
	{
		if (GetInputDevice(i)->RemoveObserver(pDevParam))
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// Name: FOR_INPUT::DEVICE
// Object: 
//01-07-25 FKE Created: 
//-----------------------------------------------------------------------------
bool	FOR_INPUT_ABC::RemoveObjectObserver(IOBJECT_PARAM *pObjObs)
{
	int i , nNbrDevices;

	if	(GetNbrInputDevice() == 0)
	{
		return	(false);
	}
	
	nNbrDevices = GetNbrInputDevice();
	for (i = 0;i < nNbrDevices;i++)
	{
		if	( GetInputDevice(i)->RemoveObjectObserver(pObjObs))
		{
			return	(true);
		}
	}
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		GetDevice
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	FOR_INPUT_ABC::GetDevice(INPUT_DEVICE_INFO &	DeviceInfo, int nPadIdx)
{
	int i, nNbrDevices;
	int	nPadCount = 0;
	
	nNbrDevices = GetNbrInputDevice();

	for (i = 0;i < nNbrDevices; i++)
	{
		if	( GetInputDevice(i)->Has( DeviceInfo ) )
		{
			if(nPadIdx == nPadCount)
			{
				return (i);
			}
			++nPadCount;
		}
	}
	
	return (-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetDevices
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	FOR_INPUT_ABC::GetDevices(INPUT_DEVICE_INFO &	DeviceInfo,
							  QDT_VECTOR < int > &	vnDevices)
{
	int i, nNbrDevices;
	
	nNbrDevices = GetNbrInputDevice();

	for (i = 0;i < nNbrDevices; i++)
	{
		if ( GetInputDevice(i)->Has( DeviceInfo ) )
		{
			vnDevices.PushTail( i );
		}
	}
	
	return ( vnDevices.GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrDevices
//	Object:		
//	03-01-10:	JJA - Created
//-----------------------------------------------------------------------------
int	FOR_INPUT_ABC::GetNbrDevices(INPUT_DEVICE_INFO &	DeviceInfo)
{
	int i, nNbrDevices, nNbrDevicesFound = 0;
	
	nNbrDevices = GetNbrInputDevice();

	for (i = 0;i < nNbrDevices; i++)
	{
		if ( GetInputDevice(i)->Has( DeviceInfo ) )
		{
			++nNbrDevicesFound;
		}
	}
	
	return ( nNbrDevicesFound );
}

//-----------------------------------------------------------------------------
//	Name:		GetInputDeviceInfo
//	Object:		
//	03-03-13:	DAC - Created
//-----------------------------------------------------------------------------
const INPUT_DEVICE_INFO	& FOR_INPUT_ABC::GetInputDeviceInfo(const int	nIndex) const
{
	return	(GetInputDevice(nIndex)->GetDeviceInfo());
}

//-----------------------------------------------------------------------------
//	Name:		GetInputDeviceName
//	Object:		
//	03-03-13:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	FOR_INPUT_ABC::GetInputDeviceName(const int	nIndex) const
{
	return	(GetInputDevice(nIndex)->GetDeviceName());
}

//-----------------------------------------------------------------------------
//	Name:		EnableVibration
//	Object:		
//	04-03-19:	VMA - Created
//-----------------------------------------------------------------------------
void	FOR_INPUT_ABC::EnableVibration(bool	bEnable)
{
	int i, nNbrDevices;
	
	nNbrDevices = GetNbrInputDevice();

	for ( i = 0 ; i < nNbrDevices ; i++ )
	{
		GetInputDevice(i)->EnableVibration(bEnable);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Acquire
//	Object:		
//	05-02-10:	VMA - Created
//-----------------------------------------------------------------------------
void	FOR_INPUT_ABC::Acquire()
{
	int i, nNbrDevices;
	
	nNbrDevices = GetNbrInputDevice();

	for ( i = 0 ; i < nNbrDevices ; i++ )
	{
		GetInputDevice(i)->Acquire();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unacquire
//	Object:		
//	05-02-10:	VMA - Created
//-----------------------------------------------------------------------------
void	FOR_INPUT_ABC::Unacquire()
{
	int i, nNbrDevices;
	
	nNbrDevices = GetNbrInputDevice();

	for ( i = 0 ; i < nNbrDevices ; i++ )
	{
		GetInputDevice(i)->Unacquire();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
