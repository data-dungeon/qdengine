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
//	CLASS:	INPUT_INIT_INFO
//	The INPUT_INIT_INFO class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_INPUTINITINFO_H__
#define __KINPUT_INPUT_INPUTINITINFO_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)

namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_INIT_INFO
		{
		public:
					INPUT_INIT_INFO();
					INPUT_INIT_INFO(const INPUT_INIT_INFO & C);
			virtual ~INPUT_INIT_INFO();
			
			INPUT_INIT_INFO &	operator=(const INPUT_INIT_INFO & C);
			void	Copy(const INPUT_INIT_INFO & C);

			void				SetDeviceName(const QDT_STRING& strDeviceName);
			const QDT_STRING&	GetDeviceName() const;

		private:

			QDT_STRING		_strDeviceName;

		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
