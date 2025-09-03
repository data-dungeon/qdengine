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
//	CLASS:	INIFILE_KEY_ABC
//	The INIFILE_KEY_ABC class implements ...
//
//	03-01-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_INIFILE_INIFILEKEYABC_H__
#define __KCORE_INIFILE_INIFILEKEYABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(INI_FILE)

#include INCL_KCORE(Tools/QdtString)

namespace	QDT
{
	namespace KCORE
	{
		class	INIFILE_KEY_ABC
		{
		public:

			enum	KEY_TYPE
			{
				KT_BOOL,
				KT_INT,
				KT_STRING,
			};

										INIFILE_KEY_ABC();
			virtual					   ~INIFILE_KEY_ABC();
			
			virtual	void				SetSectionName(const QDT_STRING & sName);
			virtual	const QDT_STRING &	GetSectionName() const;

			virtual	void				SetKeyName(const QDT_STRING & sName);
			virtual	const QDT_STRING &	GetKeyName() const;

			virtual	KEY_TYPE			GetType() = 0;

			virtual	QDT_STRING			ToString() const = 0;

			virtual	bool				IsValid() const;

			virtual	bool				ReadFromFile(const INI_FILE & Inifile);
			virtual	bool				WriteToFile(INI_FILE & Inifile) const;
			

		protected:

			virtual	bool				IsValidSubclass() const = 0;

			virtual	bool				ReadFromFileSubclass(const INI_FILE & Inifile) = 0;
			virtual	bool				WriteToFileSubclass(INI_FILE & Inifile) const = 0;


		protected:

			QDT_STRING	_sSection;
			QDT_STRING	_sKey;
		};
	}
}

#endif //__KCORE_INIFILE_INIFILEKEYABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
