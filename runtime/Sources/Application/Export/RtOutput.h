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
// CLASS: RT_OUTPUT
//
//
//01-05-04 DAC Created: 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __RUNTIME_APPLICATION_TOOLS_RTOUTPUT_H__
#define __RUNTIME_APPLICATION_TOOLS_RTOUTPUT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Core/IOutput)

DECL_RUN(EXPORT_PROGRESS_BAR_DLG)

namespace	QDT
{
	namespace	RUN
	{
		class RT_OUTPUT : public I_OUTPUT
		{
		public:
			
			explicit RT_OUTPUT(EXPORT_PROGRESS_BAR_DLG* pGUI);
			virtual  ~RT_OUTPUT();
		    
			virtual void Output(const char *pcString, const bool bWarning);

		private:

			EXPORT_PROGRESS_BAR_DLG * _pGUI;
		};
	}
}

#endif	//__RUNTIME_APPLICATION_TOOLS_RTOUTPUT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
