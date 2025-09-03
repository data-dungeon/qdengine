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
//	The INPUT_OBJECT class implements ...
//
//	02-06-26:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_X360_INPUTOBJECT_H__
#define __KINPUT_INPUT_X360_INPUTOBJECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KINPUT(Input/InputObjectABC)

namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_OBJECT : public INPUT_OBJECT_ABC
		{
		public:
					INPUT_OBJECT();
					INPUT_OBJECT(const INPUT_OBJECT & C);
			virtual ~INPUT_OBJECT();
			

			INPUT_OBJECT &	operator=(const INPUT_OBJECT & C);
			void	Copy(const INPUT_OBJECT & C);

			void	SetReadHandle(UInt32 nRH);
			UInt32	GetReadHandle() const;

			void	SetState(Float32 nState);
			Float32	GetState() const;

			
			static void NewDevice();

		private:
			
			static int	_nObjTypeNum[INPUT_OBJECT_ABC::MAX_OBJ_TYPE];

			enum
			{
				FLG_BUTTON = 1 << 0,
				FLG_AXIS = 1 << 1,
			};


			UInt32		_nReadHandle;

			UInt32		_nFlags;
			Float32		_nState;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
