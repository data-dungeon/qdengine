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
//	CLASS:	INIFILE_KEY_HOLDER
//	The INIFILE_KEY_HOLDER class implements ...
//
//	03-01-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_INIFILE_INIFILEKEYHOLDER_H__
#define __KCORE_INIFILE_INIFILEKEYHOLDER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KCORE(INIFILE_KEY_ABC)

#include INCL_KCORE(Template/QDT_Vector)

namespace	QDT
{
	namespace KCORE
	{
		class	INIFILE_KEY_HOLDER
		{
		public:

											INIFILE_KEY_HOLDER();
											~INIFILE_KEY_HOLDER();
									
					void					AddBoolKey(const QDT_STRING & sSection, const QDT_STRING & sKey, const bool bValue);
					void					AddIntKey(const QDT_STRING & sSection, const QDT_STRING & sKey, const int nValue);
					void					AddStringKey(const QDT_STRING & sSection, const QDT_STRING & sKey, const QDT_STRING & sValue);
									
					bool					GetBoolKeyValue(const QDT_STRING & sSection, const QDT_STRING & sKey, bool & bValue) const;
					bool					GetIntKeyValue(const QDT_STRING & sSection, const QDT_STRING & sKey, int & nValue) const;
					bool					GetStringKeyValue(const QDT_STRING & sSection, const QDT_STRING & sKey, QDT_STRING & sValue) const;

					void					SetBoolKey(const QDT_STRING & sSection, const QDT_STRING & sKey, const bool bValue);
					void					SetIntKey(const QDT_STRING & sSection, const QDT_STRING & sKey, const int nValue);
					void					SetStringKey(const QDT_STRING & sSection, const QDT_STRING & sKey, const QDT_STRING & sValue);
									
					bool					HasKey(const QDT_STRING & sSection, const QDT_STRING & sKey) const;
					bool					IsKeyBool(const QDT_STRING & sSection, const QDT_STRING & sKey) const;
					bool					IsKeyInt(const QDT_STRING & sSection, const QDT_STRING & sKey) const;
					bool					IsKeyString(const QDT_STRING & sSection, const QDT_STRING & sKey) const;
									
					bool					ReadFromFile(const QDT_STRING & sFileName);
					bool					SaveToFile(const QDT_STRING & sFileName) const;
									
					void					Clear();
					void					Copy(const INIFILE_KEY_HOLDER & C);
					INIFILE_KEY_HOLDER &	operator=(const INIFILE_KEY_HOLDER & C);

					int						GetNbrKeys() const;
					const QDT_STRING &		GetKeyName(const int nKeyIndex) const;
					QDT_STRING				GetKeyValueString(const int nKeyIndex) const;

			static	QDT_STRING				GetTrueString();
			static	QDT_STRING				GetFalseString();
											
											
		private:							
											
			INIFILE_KEY_ABC *				FindKey(const QDT_STRING & sSection, const QDT_STRING & sKey) const;
			INIFILE_KEY_ABC *				GetKey(const int nKeyIndex) const;


		private:

			QDT_VECTOR<INIFILE_KEY_ABC*>	_Keys;
		};
	}
}

#endif //__KCORE_INIFILE_INIFILEKEYHOLDER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
