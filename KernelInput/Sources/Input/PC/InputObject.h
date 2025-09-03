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
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_PC_INPUTOBJECT_H__
#define __KINPUT_INPUT_PC_INPUTOBJECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_PC_DINPUT
#include	INCL_KCORE(Includes_PC/Includes_PC)
#include	INCL_KINPUT(Input/InputObjectABC)

namespace QDT
{
	namespace KINPUT
	{
		class INPUT_OBJECT : public INPUT_OBJECT_ABC
		{
			public:

				INPUT_OBJECT();
				~INPUT_OBJECT();

				void		SetGuidAndName(const GUID *pGuid, const char *pName);
				const		GUID *GetGuid() const;

				void		SetAxis();
				void		SetButton();

				static void NewDevice();

			private:

				enum
				{
					FLG_BUTTON = 1 << 0,
					FLG_AXIS = 1 << 1,
				};

				GUID		_Guid;
				int			_nFlags;
				
				static int	_nObjTypeNum[MAX_OBJ_TYPE];
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
