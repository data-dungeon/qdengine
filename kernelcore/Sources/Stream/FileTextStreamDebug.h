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
//	CLASS:	FILE_TEXT_STREAM_DEBUG
//	The FILE_TEXT_STREAM_DEBUG class implements ...
//
//	02-02-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_STREAM_FILETEXTSTREAMDEBUG_H__
#define __KCORE_STREAM_FILETEXTSTREAMDEBUG_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Stream/FileTextStream)

namespace QDT
{
	namespace KCORE
	{
		class	FILE_TEXT_STREAM_DEBUG : public FILE_TEXT_STREAM
		{
		public:
			FILE_TEXT_STREAM_DEBUG();
			FILE_TEXT_STREAM_DEBUG(const char *pFilename, int nAccessType);

			IO_STREAM & operator =(const unsigned int s);

		#ifndef _MASTER
			virtual void	DebugOutput(const char *pTxt);
			virtual void	DebugOutput(unsigned char Char);
			virtual void	DebugOutput(int Int);
			virtual void	DebugOutput(float Float);
			virtual void	DebugOutputBinary(int Int);
		#else
			virtual void	DebugOutput(const char *pTxt){}
			virtual void	DebugOutput(unsigned char Char){}
			virtual void	DebugOutput(int Int){}
			virtual void	DebugOutput(float Float){}
			virtual void	DebugOutputBinary(int Int){}
		#endif

			bool IsFileTextStreamDebug();

		private:
		};
	}
}

#endif //__KCORE_STREAM_FILETEXTSTREAMDEBUG_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
