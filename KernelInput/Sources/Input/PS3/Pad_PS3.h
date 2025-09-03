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
//	The PAD_PS3 class implements ...
//
//	05-10-05:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#ifndef __KINPUT_INPUT_PS3_PAD_PS3_H__
#define __KINPUT_INPUT_PS3_PAD_PS3_H__

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

#define AXIS_DEAD_ZONE		24

#define QDT_INCLUDE_PS3_PAD
#include INCL_KCORE(Includes_PS3/Includes_PS3)

namespace QDT
{
	namespace KINPUT
	{
		class PAD_PS3
		{
		public:

			enum BUTTON_TYPE
			{
				BT_SELECT		 = 0,
				BT_L3       	 = 1,
				BT_R3       	 = 2,
				BT_START    	 = 3,
				BT_UP       	 = 4,
				BT_LEFT			 = 5,
				BT_DOWN     	 = 6,
				BT_RIGHT    	 = 7,
				BT_L2			 = 8,
				BT_R2			 = 9,
				BT_L1			 = 10,
				BT_R1			 = 11,
				BT_TRIANGLE		 = 12,
				BT_CIRCLE		 = 13,
				BT_CROSS		 = 14,
				BT_SQUARE		 = 15,
				BT_ANALOG		 = 16,

				BT_NB				
			};

			enum ANALOG_TYPE
			{
				AT_LEFT_HORZ			= 0,
				AT_LEFT_VERT			,
				AT_RIGHT_HORZ			,
				AT_RIGHT_VERT			,

				AT_NB
			};
			struct STATE
			{
				UInt32	_Buttons;
				UInt8	_AnalogPositions[AT_NB];
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

									PAD_PS3();

			Bool 					IsButtonDown(BUTTON_TYPE BT) const;
			Bool 					IsButtonUp(BUTTON_TYPE BT) const;
			Bool 					IsButtonPressed(BUTTON_TYPE BT) const;
			Bool 					IsButtonReleased(BUTTON_TYPE BT) const;

			UInt8					GetAnalogPosition(ANALOG_TYPE AT) const;

			PAD_DESCRIPTOR *		GetDescriptor() const;

		private:
			friend class PAD_MANAGER_PS3;

			void					Update(CellPadInfo *);

			Bool	_bUsed;
			UInt8	_nStatus;
			UInt8	_nPort;

			STATE	_OldState,
					_CurState;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
