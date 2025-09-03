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
//	CLASS:	INI_FILE
//	The INI_FILE class implements ...
//
//	02-01-21:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_INIFILE_INIFILE_H__
#define __KCORE_INIFILE_INIFILE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(TextDataFile/TextDataFileAbc)
#include INCL_KCORE(Template/QDT_Vector)

namespace	QDT
{
	namespace KCORE
	{
		class	INI_FILE : public TEXT_DATA_FILE_ABC
		{
		public:
										INI_FILE();
			explicit					INI_FILE(const QDT_STRING& strFileName);
			explicit					INI_FILE(const INI_FILE & C);
			virtual						~INI_FILE();
			
					INI_FILE &	operator=(const INI_FILE & C);
					void				Copy(const INI_FILE & C);
										
					void				Reset();

					virtual bool		OpenFile(const QDT_STRING& strFileName);
					virtual bool		OpenFile(IO_STREAM& Stream);
					virtual bool		CloseFile();
					virtual bool		WriteFile(IO_STREAM& Stream);
					bool				EraseFile();
					void				PrepareForMemoryUsage(const QDT_STRING & sRootName);

					//Existance
					bool				DoesSectionExist(const QDT_STRING &	sSectionName) const;
					bool				DoesSectionAndKeyExist(const QDT_STRING& strSection, const QDT_STRING& strKey) const;

					//Getting
					QDT_STRING			GetString( const QDT_STRING& strSection, const QDT_STRING& strKey, const QDT_STRING& strDefault = "") const;
					int					GetInt(const QDT_STRING& strSection, const QDT_STRING& strKey, const int nDefault = 0) const;
					float				GetFloat(const QDT_STRING& strSection, const QDT_STRING& strKey, const float rDefault = 0.0) const;
					bool				GetBool(const QDT_STRING& strSection, const QDT_STRING& strKey, const bool bDefault = false) const;

					//Setting
					TEXT_DATA_FILE_ERRORS	WriteString(const QDT_STRING& strSection, const QDT_STRING& strKey, const QDT_STRING& strVal);
					TEXT_DATA_FILE_ERRORS	WriteInt(const QDT_STRING& strSection, const QDT_STRING& strKey, const int nVal);
					TEXT_DATA_FILE_ERRORS	WriteFloat(const QDT_STRING& strSection, const QDT_STRING& strKey, const float rVal);
					TEXT_DATA_FILE_ERRORS	WriteBool(const QDT_STRING& strSection, const QDT_STRING& strKey, const bool bVal);

					bool				PushEmptyLine();	//add an empty line to the inifile

		//Interface for XML like document
			virtual	bool				ResetToTop();
			virtual	bool				PushSection(const QDT_STRING & sName);
			virtual	bool				PopSection();
			virtual	bool				PushElement(const QDT_STRING & sName, const QDT_STRING & sValue);
			virtual	bool				PushText(const QDT_STRING & sText);

			virtual	TEXT_DATA_LIST_ABC*	GetSectionList(const QDT_STRING & sName);
			virtual	void				ChangeRelativeSearch(const QDT_STRING & sName);
			virtual	void				ResetRelativeSearch();

		protected:

			struct REG_KEY
			{
				QDT_STRING sKeyName;
				QDT_STRING sKeyValue;
			};

			struct REG_SECTION
			{
				QDT_STRING sSectionName;
				QDT_VECTOR<REG_KEY> vKey;
			};

			void	ClearAllSections();

			bool	ReadIniFileSection(const QDT_STRING& strIniFileName);
			bool	ReadIniFileSection(IO_STREAM& Stream);
			bool	WriteIniFileSection(const QDT_STRING& strIniFileName) const;
			bool	WriteIniFileSection(IO_STREAM& Stream) const;

			int		NewSection(const QDT_STRING& sLine);
			int		NewKey(const int nSection, const QDT_STRING& sLine);
			void	ChangeKeyValue(int nSectionIndex, int nKeyIndex, const QDT_STRING & sKeyValue);

			int					FindSection(const QDT_STRING &	sSectionName) const;
			int					FindKey(const int nSectionIndex, const QDT_STRING &	sKeyName) const;
			int					GetNbrSections() const;
			int					GetNbrKeys(const int nSectionIndex) const;
			const QDT_STRING&	GetSectionName(const int nSectionIndex) const;
			const QDT_STRING&	GetKeyName(const int nSectionIndex, const int nKeyIndex) const;
			const QDT_STRING&	GetKeyValue(const int nSectionIndex, const int nKeyIndex) const;

			int		GetMemPrivateProfileString(const QDT_STRING& strSection, const QDT_STRING& strKey, const QDT_STRING& strDefault, QDT_STRING& sDestBuffer, const int nSize, const QDT_STRING& strIniFileName ) const;
			bool	WriteMemPrivateProfileString(const QDT_STRING & sSectionName, const QDT_STRING & sKeyName, const QDT_STRING & sString, const QDT_STRING & sFileName);

			virtual TEXT_FILE_TYPE		GetFileType() const;
		private:
			enum
			{
				DEFAULT_BUFFER_SIZE = 1024,
			};

			QDT_VECTOR<REG_SECTION> _vSection;
			mutable QDT_STRING		_sCurrentSectionName;	//for speed optimisation so mutable is for const methods in reading process
			mutable int				_nCurrentSection;		//for speed optimisation so mutable is for const methods in reading process
			int						_nCurrentKey;
		};
	}
}

#endif //__KCORE_INIFILE_INIFILE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
