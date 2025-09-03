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
// CLASS: IOUPUT
//
//
//01-04-23 FKE Created: 
//*****************************************************************************

#ifndef __KCORE_CORE_IOUTPUT_H__
#define __KCORE_CORE_IOUTPUT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class I_OUTPUT
		{
		public:
			virtual void Output(const char *pcString, const bool bIsWarning) = 0;
			static void AddToList(I_OUTPUT *);
			static void RemoveFromList(I_OUTPUT *);
			static void Push(const char *pcString, const bool bIsWarning = false);
		private:
			enum
			{
				MAX_IOUTPUT = 20,
			}; 
			static int _nNbIOutput;
			static I_OUTPUT	*_pIOutput[MAX_IOUTPUT];
		};
	}
}

#endif
//=============================================================================
// CODE ENDS HERE
//=============================================================================
