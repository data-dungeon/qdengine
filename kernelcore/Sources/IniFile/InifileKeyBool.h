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
//	CLASS:	INIFILE_KEY_BOOL
//	The INIFILE_KEY_BOOL class implements ...
//
//	03-01-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_INIFILE_INIFILEKEYBOOL_H__
#define __KCORE_INIFILE_INIFILEKEYBOOL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
#include	INCL_KCORE(IniFile/IniFileKeyAbc)

namespace	QDT
{
	namespace KCORE
	{
		class	INIFILE_KEY_BOOL : public INIFILE_KEY_ABC
		{
		public:

								INIFILE_KEY_BOOL();
			virtual			   ~INIFILE_KEY_BOOL();

					void		SetValue(const bool bValue);
					bool		GetValue() const;
								
			virtual	KEY_TYPE	GetType();
								
			virtual	QDT_STRING	ToString() const;

			virtual	bool		IsValidSubclass() const;

			virtual	bool		ReadFromFileSubclass(const INI_FILE & Inifile);
			virtual	bool		WriteToFileSubclass(INI_FILE & Inifile) const;

			static	QDT_STRING	GetTrueString();
			static	QDT_STRING	GetFalseString();


		private:

			bool	_bValue;
		};
	}
}

#endif //__KCORE_INIFILE_INIFILEKEYBOOL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
