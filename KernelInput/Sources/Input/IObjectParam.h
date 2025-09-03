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
//	CLASS:	IOBJECT_PARAM
//	The IOBJECT_PARAM class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_IOBJECTPARAM_H__
#define __KINPUT_INPUT_IOBJECTPARAM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KINPUT
	{
		class	IOBJECT_PARAM
		{
		public:
			

					IOBJECT_PARAM();
					IOBJECT_PARAM(const IOBJECT_PARAM & C);
			virtual ~IOBJECT_PARAM();
			

			IOBJECT_PARAM &	operator=(const IOBJECT_PARAM & C);
			void	Copy(const IOBJECT_PARAM & C);

			virtual void ProcessEvent(float rVal) = 0;
			virtual bool IsEventActive() const = 0;

		private:
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
