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
//	The PAD_X360 class implements ...
//
//	06-29-06:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#ifndef __KINPUT_INPUT_X360_PAD_X360_H__
#define __KINPUT_INPUT_X360_PAD_X360_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define BUTTONTYPE_TOGGLE	(1L)
#define BUTTONTYPE_PUSH		(1L<<1)
#define BUTTONTYPE_DIGITAL	(1L<<2)
#define BUTTONTYPE_ANALOG	(1L<<3)

#define AXISTYPE_ABSOLUTE	(1L)
#define AXISTYPE_RELATIVE	(1L<<1)
#define AXISTYPE_DIGITAL	(1L<<2)
#define AXISTYPE_ANALOG		(1L<<3)

#define MAX_PAD				4

#define QDT_INCLUDE_X360_XTL
#include INCL_KCORE(Includes_X360/Includes_X360)

namespace QDT
{
	namespace KINPUT
	{
		class PAD_X360
		{
		public:

			enum BUTTON_TYPE
			{
				BT_DPAD_UP         = 0,
				BT_DPAD_DOWN       = 1,
				BT_DPAD_LEFT       = 2,
				BT_DPAD_RIGHT      = 3,
				BT_START           = 4,
				BT_BACK            = 5,
				BT_LEFT_THUMB      = 6,
				BT_RIGHT_THUMB     = 7,
				BT_LEFT_SHOULDER   = 8,
				BT_RIGHT_SHOULDER  = 9,
				BT_DUMMY1		   = 10,               
				BT_DUMMY2		   = 11,               
				BT_A               = 12,
				BT_B               = 13,
				BT_X               = 14,
				BT_Y               = 15,

				BT_NB				
			};

			enum ANALOG_TYPE
			{
				AT_LEFT_HORZ			= 0,
				AT_LEFT_VERT			,
				AT_RIGHT_HORZ			,
				AT_RIGHT_VERT			,
				AT_LEFT_TRIGGER			,			
				AT_RIGHT_TRIGGER		,			

				AT_NB
			};
			
			struct DESCRIPTOR
			{
				UInt32 nType;
				UInt32 nID;
				Char * pName;
			};

			struct PAD_DESCRIPTOR
			{
				UInt32			nButtonsCount;
				DESCRIPTOR *	pButtonsDesc;
				UInt32			nAxisCount;
				DESCRIPTOR *	pAxisDesc;
			};

									PAD_X360();

			Bool 					IsButtonDown(BUTTON_TYPE BT) const;
			Bool 					IsButtonUp(BUTTON_TYPE BT) const;
			Bool 					IsButtonPressed(BUTTON_TYPE BT) const;
			Bool 					IsButtonReleased(BUTTON_TYPE BT) const;

			Int16					GetAnalogPosition(ANALOG_TYPE AT) const;

			void					Vibrate( UInt8	nSmallEngine, UInt8	nBigEngine );

			PAD_DESCRIPTOR *		GetDescriptor() const;

		private:
			friend class PAD_MANAGER_X360;

			void					Update();

			Bool	_bUsed;
			Int8	_nPort;

			XINPUT_STATE	_OldState, _CurState;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
