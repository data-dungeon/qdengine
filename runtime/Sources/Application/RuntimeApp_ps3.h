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
//	CLASS:	RUNTIME_APP
//	The RUNTIME_APP class implements ...
//
//	05-11-14:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __RUNTIME_APPLICATION_RUNTIMEAPP_H__
#define __RUNTIME_APPLICATION_RUNTIMEAPP_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(ComBasis/Types)

DECL_KCORE(FILE_ABC)

namespace	QDT
{
	namespace	RUN
	{
		class	RUNTIME_APP
		{
		public:
			RUNTIME_APP();
			~RUNTIME_APP();

			bool				InitInstance(int argc, char * argv[]);
			bool				ExitInstance();
			bool				OnIdle(long	lCount);

		private:
			bool				InitOs();
			bool				InitIAMRuntime();

			void				ReadCommandLine(int argc, char *argv[]);

			const QDT_STRING&	GetBCName() const;
			const QDT_STRING&	GetLuaFileName() const;
			const QDT_STRING&	GetBFName() const;

			QDT_STRING			GetRuntimeVersionString() const;
			QDT_STRING			GetClassCmdLine() const;
			QDT_STRING			GetBCCmdLine() const;
			QDT_STRING			GetLUACmdLine() const;
			QDT_STRING			GetDefaultClassToBoot() const;
			QDT_STRING			GetDefaultByteCodeName() const;
			QDT_STRING			GetDefaultLuaFileName() const;
			QDT_STRING			GetBFCmdLine() const;
			QDT_STRING			GetDefaultBigFileName() const;
			const QDT_STRING &	GetClassToBoot() const;

			void				SetBCName(const QDT_STRING& strBCName);
			void				SetLuaFileName(const QDT_STRING& strBCName);
			void				SetBFName(const QDT_STRING& strBFName);
			void				SetClassToBoot(const QDT_STRING & strClassName);

			static	int			LUA_Output(COM_PARAM P);
			int			LUA_RunFile(const char* sFilePath);
			int			LUA_RunFile(FILE_ABC *	pFile);

		private:
			static RUNTIME_APP	RA;

			QDT_STRING	_strBCName;
			QDT_STRING	_strLuaFileName;
			QDT_STRING	_strBFName;
			QDT_STRING	_strClassToBoot;
			QDT_STRING	_strFixedIPArg;
			QDT_STRING	_strRemoteFileServingRootDir;		// typically : d:/HOME_PS3
		};
	}
}


#endif //__RUNTIME_APPLICATION_RUNTIMEAPP_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
