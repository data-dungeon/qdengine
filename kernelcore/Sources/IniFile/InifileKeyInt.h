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
//	CLASS:	INIFILE_KEY_INT
//	The INIFILE_KEY_INT class implements ...
//
//	03-01-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_INIFILE_INIFILEKEYINT_H__
#define __KCORE_INIFILE_INIFILEKEYINT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(IniFile/IniFileKeyAbc)

namespace	QDT
{
	namespace KCORE
	{
		class	INIFILE_KEY_INT : public INIFILE_KEY_ABC
		{
		public:

								INIFILE_KEY_INT();
			virtual			   ~INIFILE_KEY_INT();

					void		SetValue(const int nValue);
					int			GetValue() const;

			virtual	KEY_TYPE	GetType();
								
			virtual	QDT_STRING	ToString() const;

			virtual	bool		IsValidSubclass() const;

			virtual	bool		ReadFromFileSubclass(const INI_FILE & Inifile);
			virtual	bool		WriteToFileSubclass(INI_FILE & Inifile) const;

			
		private:

			int	_nValue;
		};
	}
}

#endif //__KCORE_INIFILE_INIFILEKEYINT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
