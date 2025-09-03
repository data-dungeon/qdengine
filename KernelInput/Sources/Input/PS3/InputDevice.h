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
//	The INPUT_DEVICE class implements ...
//
//	02-06-26:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_PS3_INPUTDEVICE_H__
#define __KINPUT_INPUT_PS3_INPUTDEVICE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KINPUT(Input/InputDeviceABC)
#include	INCL_KINPUT(Input/PS3/Pad_PS3)

namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_DEVICE : public INPUT_DEVICE_ABC
		{
			public:
						INPUT_DEVICE();
						INPUT_DEVICE(const INPUT_DEVICE & C);
				virtual ~INPUT_DEVICE();
				

				INPUT_DEVICE &	operator=(const INPUT_DEVICE & C);
				void	Copy(const INPUT_DEVICE & C);
					
				virtual bool Init(const INPUT_INIT_INFO* pInitInfo);
				virtual void GetState(int handle);	
				virtual bool Flush();
				virtual void Vibrate( unsigned char nSmallEngine, unsigned char nBigEngine );

			private:
					void SetObject(int nOffset);
					bool Initialize(int	nSlot, int nPort);
					
					unsigned char *_pCorresTab;
					unsigned long _nCorresTabSize;

					unsigned long	_nCurrentObject;
					int				_nCurrentOffset;
					int				_nPort;
					int				_nSlot;

					PAD_PS3 *					_pPad;
					PAD_PS3::PAD_DESCRIPTOR *	_pPadDesc;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
