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
//	The FOR_INPUT_ABC class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_FORINPUTABC_H__
#define __KINPUT_INPUT_FORINPUTABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KINPUT(INPUT_DEVICE_INFO)
DECL_KINPUT(IDEVICE_PARAM)
DECL_KINPUT(INPUT_DEVICE_ABC)

#include INCL_KINPUT(Input/InputObjectABC)
#include INCL_KCORE(Template/Qdt_Vector)

namespace QDT
{
	namespace KINPUT
	{
		class	FOR_INPUT_ABC
		{
		public:
					FOR_INPUT_ABC();
					FOR_INPUT_ABC(const FOR_INPUT_ABC & C);
			virtual ~FOR_INPUT_ABC();
			

			FOR_INPUT_ABC &	operator=(const FOR_INPUT_ABC & C);
			void	Copy(const FOR_INPUT_ABC & C);

			/** Initialization of Input. */
			virtual bool Init() = 0;
			/** Called to refresh and get datas from the devices. */
 			virtual bool Loop();
			
			virtual void Update() = 0;
			
			virtual bool Flush();

			/** Close and release all the devices created during the initialization. */
			virtual bool Close();

			int					GetDevice( INPUT_DEVICE_INFO&	DeviceInfo, int nPadIdx=0 );
			int					GetDevices( INPUT_DEVICE_INFO&	DeviceInfo, QDT_VECTOR<int>& vnDevices );
			int					GetNbrDevices( INPUT_DEVICE_INFO&	DeviceInfo );

			void	ForceAllInstancesOnPad(int nTargetPad);

			bool	AddDeviceObserver(int nDeviceHandle, IDEVICE_PARAM *pDevParam);
			bool	RemoveDeviceObserver(IDEVICE_PARAM *pDevParam);

			bool	AddObjectObserver(int nDeviceHandle, INPUT_OBJECT_ABC::OBJ_TYPE ObjectType, int nObjectIndex, IOBJECT_PARAM *pObjParam);
			bool	RemoveObjectObserver(IOBJECT_PARAM *pObjParam);

			bool	DisplayEventVal(int handle);

			void	EnableVibration(bool bEnable);
			void	Acquire();
			void	Unacquire();
			
			const INPUT_DEVICE_INFO	&	GetInputDeviceInfo(const int nIndex) const;
			int							GetNbrInputDevice() const;
			INPUT_DEVICE_ABC*			GetInputDevice(const int nIndex);
			const INPUT_DEVICE_ABC*		GetInputDevice(const int nIndex) const;
			const QDT_STRING &			GetInputDeviceName(const int nIndex) const;

		protected:

			void				AddInputDevice(INPUT_DEVICE_ABC		*pDevice);
			bool				RemoveInputDevice(INPUT_DEVICE_ABC	*pDevice);
			void				ClearAllInputDevice();

		private:
			
			QDT_VECTOR<INPUT_DEVICE_ABC*>	_tVectInputDevices;
			
		};
	}
}

#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
