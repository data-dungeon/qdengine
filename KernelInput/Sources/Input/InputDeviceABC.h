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
//	The INPUT_DEVICE_ABC class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_INPUTDEVICEABC_H__
#define __KINPUT_INPUT_INPUTDEVICEABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KINPUT(IDEVICE_PARAM)
DECL_KINPUT(INPUT_INIT_INFO)

#include INCL_KINPUT(Input/InputObjectABC)
#include INCL_KINPUT(Input/InputDeviceAbc)
#include INCL_KINPUT(Input/InputDeviceInfo)


namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_DEVICE_ABC
		{
		public:
			enum
			{
				SAMPLE_BUFFER_SIZE		= 128,
				FLG_ENABLE				= 1 << 0,
				FLG_INITIALIZED			= 1 << 1,
				FLG_DISPLAY_EVENT_VAL	= 1 << 2,
				FLG_VIBRATION			= 1 << 3,
				FLG_VIBRATION_ENABLE	= 1 << 4,
				FLG_CONNECTION			= 1 << 5,
			};


					INPUT_DEVICE_ABC();
					INPUT_DEVICE_ABC(const INPUT_DEVICE_ABC & C);
			virtual	~INPUT_DEVICE_ABC();
			

			INPUT_DEVICE_ABC &	operator=(const INPUT_DEVICE_ABC & C);
			void	Copy(const INPUT_DEVICE_ABC & C);

			//virtual stuff to implement
			virtual bool Init(const INPUT_INIT_INFO* pInitInfo) = 0;
			virtual void GetState(int handle) = 0;
			virtual bool Flush() = 0;
			virtual void Vibrate( unsigned char nSmallEngine, unsigned char nBigEngine ) = 0;
			
			bool			AddObserver(IDEVICE_PARAM	*pDeviceObs);
			bool			RemoveObserver(IDEVICE_PARAM	*pDeviceObs);
			int				GetNbrObserver() const;
			IDEVICE_PARAM*	GetDeviceObserver(const int nIndex);

			bool			AddObjectObserver(INPUT_OBJECT_ABC::OBJ_TYPE ObjectType, int nObjectIndex, IOBJECT_PARAM *pObjObs);
			bool			RemoveObjectObserver(IOBJECT_PARAM *pObjObs);

			bool			Has( INPUT_DEVICE_INFO& DeviceInfo );

			void			DisplayEventVal();

			void			EnableVibration(bool _bEnable);
			bool			IsVibrationEnabled();
			
			void				SetDeviceName(const QDT_STRING& strDeviceName);
			const QDT_STRING&	GetDeviceName() const;

			void				TransferObserversTo(INPUT_DEVICE_ABC *pTarget);

			virtual void		DispatchEvent(int nObjectType, int nIndex , float rVal);
			virtual void		DispatchConnectEvent();
			virtual void		DispatchDisconnectEvent();

			virtual void		Acquire(){};
			virtual void		Unacquire(){};

			int					GetNbrInputObject() const;
			INPUT_OBJECT_ABC*	GetInputObject(const int nIndex);
			int					GetNbrRelativeAxes() const;
			int					GetNbrAbsoluteAxes() const;
			int					GetNbrPushButtons() const;
			int					GetNbrToggleButtons() const;

			void				UpdateDeviceInfo( const int nObjectType );
			
			const INPUT_DEVICE_INFO & GetDeviceInfo() const;

		protected:
			
			void				AddInputObject(INPUT_OBJECT_ABC* pInputObj);
			bool				RemoveInputObject(INPUT_OBJECT_ABC* pInputObj);
			void				ClearAllInputObject();

			int					_nNbRelAxis; 
			int					_nNbAbsAxis;
			int					_nNbPushButton;
			int					_nNbToggleButton;
			int					_nFlags;

			INPUT_DEVICE_INFO	_DeviceInfo;
			QDT_VECTOR<INPUT_OBJECT_ABC*>	_tVectInputObjects;

		private:

			QDT_VECTOR<IDEVICE_PARAM*>		_tVectObserver;
			QDT_STRING						_strDeviceName;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
