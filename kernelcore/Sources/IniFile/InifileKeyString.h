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
//	CLASS:	INIFILE_KEY_STRING
//	The INIFILE_KEY_STRING class implements ...
//
//	03-01-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_INIFILE_INIFILEKEYSTRING_H__
#define __KCORE_INIFILE_INIFILEKEYSTRING_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(IniFile/IniFileKeyAbc)

namespace	QDT
{
	namespace KCORE
	{
		class	INIFILE_KEY_STRING : public INIFILE_KEY_ABC
		{
		public:

										INIFILE_KEY_STRING();
			virtual					   ~INIFILE_KEY_STRING();

					void				SetValue(const QDT_STRING & sValue);
					const QDT_STRING &	GetValue() const;

			virtual	KEY_TYPE			GetType();
										
			virtual	QDT_STRING			ToString() const;

			virtual	bool				IsValidSubclass() const;
										
			virtual	bool				ReadFromFileSubclass(const INI_FILE & Inifile);
			virtual	bool				WriteToFileSubclass(INI_FILE & Inifile) const;


		private:

			QDT_STRING	_sValue;
		};
	}
}

#endif //__KCORE_INIFILE_INIFILEKEYSTRING_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
