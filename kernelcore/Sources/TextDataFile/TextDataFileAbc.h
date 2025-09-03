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
//	CLASS:	TEXT_DATA_FILE_ABC
//	The TEXT_DATA_FILE_ABC class implements ...
//
//	05-05-31:	VPI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TEXTDATAFILE_TEXTDATAFILEABC_H__
#define __KCORE_TEXTDATAFILE_TEXTDATAFILEABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KCORE(TEXT_DATA_LIST_ABC)

#include INCL_KCORE(Tools/QdtString)

namespace	QDT
{
	namespace KCORE
	{
		class	TEXT_DATA_FILE_ABC
		{
		public:
			enum TEXT_DATA_FILE_ERRORS
			{
				UNKNOWN_ERRORS = -255,
				ERROR_READ,
				ERROR_WRITE,
				NO_ERRORS = 1,
			};

			static const char*			DEFAULT_EXISTS_SECTION_STR;

										TEXT_DATA_FILE_ABC();
			explicit					TEXT_DATA_FILE_ABC(const QDT_STRING& strFileName);
			virtual						~TEXT_DATA_FILE_ABC();

			//Need copy constructors ans Copy operators

			virtual void				Reset() = 0;

			virtual bool				OpenFile(const QDT_STRING& strFileName) = 0;
			virtual bool				OpenFile(IO_STREAM& Stream) = 0;
			virtual bool				CloseFile() = 0;
			virtual bool				WriteFile(IO_STREAM& Stream) = 0;
			
			virtual const QDT_STRING &	GetFileName() const;
			virtual void				SetFileName(const QDT_STRING & sName);
			
			virtual bool				EraseFile() = 0;
			virtual void				PrepareForMemoryUsage(const QDT_STRING & sRootName = "") = 0;

			//Existance
			virtual bool				DoesSectionExist(const QDT_STRING &	sSectionName) const = 0;
			virtual bool				DoesSectionAndKeyExist(const QDT_STRING& strSection, const QDT_STRING& strKey) const = 0;

			//Getting
			virtual QDT_STRING			GetString( const QDT_STRING& strSection, const QDT_STRING& strKey, const QDT_STRING& strDefault = "") const = 0;
			virtual int					GetInt(const QDT_STRING& strSection, const QDT_STRING& strKey, const int nDefault = 0) const = 0;
			virtual float				GetFloat(const QDT_STRING& strSection, const QDT_STRING& strKey, const float rDefault = 0.0) const = 0;
			virtual bool				GetBool(const QDT_STRING& strSection, const QDT_STRING& strKey, const bool bDefault = false) const = 0;

			//Setting
			virtual TEXT_DATA_FILE_ERRORS	WriteString(const QDT_STRING& strSection, const QDT_STRING& strKey, const QDT_STRING& strVal) = 0;
			virtual TEXT_DATA_FILE_ERRORS	WriteInt(const QDT_STRING& strSection, const QDT_STRING& strKey, const int nVal) = 0;
			virtual TEXT_DATA_FILE_ERRORS	WriteFloat(const QDT_STRING& strSection, const QDT_STRING& strKey, const float rVal) = 0;
			virtual TEXT_DATA_FILE_ERRORS	WriteBool(const QDT_STRING& strSection, const QDT_STRING& strKey, const bool bVal) = 0;

			virtual bool				PushEmptyLine() = 0;	//add an empty line to the file

			virtual bool				IsGood() const;
			virtual bool				HasChanged() const;
			virtual bool				IsFileReadOnly() const;

			virtual bool				BackupFile() const;	//	Create a copy of the file on the hardrive

			virtual	const QDT_STRING 	GetLastErrorReason() const;


		//Interface for XML like document
			virtual	bool				ResetToTop() = 0;
			virtual	bool				PushSection(const QDT_STRING & sName) = 0;
			virtual	bool				PopSection() = 0;
			virtual	bool				PushElement(const QDT_STRING & sName, const QDT_STRING & sValue) = 0;
			virtual	bool				PushText(const QDT_STRING & sText) = 0;

			virtual	TEXT_DATA_LIST_ABC*	GetSectionList(const QDT_STRING & sName) = 0;
			virtual	void				ChangeRelativeSearch(const QDT_STRING & sName) = 0;
			virtual	void				ResetRelativeSearch() = 0;

			enum	TEXT_FILE_TYPE
			{
				TFT_UNKNOW = -1,
				TFT_XML,
				TFT_INI,
				TFT_MAX
			};
			virtual TEXT_FILE_TYPE		GetFileType() const = 0;
		protected:
			void						SetIsGood(const bool bFlag);
			virtual void				SetHasChanged(const bool bFlag) const; //mutable...
			virtual void				SetLastErrorReason(const QDT_STRING & sReason);

			QDT_STRING					_sLastErrorReason;
			QDT_STRING					_strFileName;
			bool						_bIsGood;
			mutable bool				_bHasChanged;				//mutable is for const methods in reading process, because the file can change without the class (this)

		private:

		};
	}
}

#endif //__KCORE_TEXTDATAFILE_TEXTDATAFILEABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
