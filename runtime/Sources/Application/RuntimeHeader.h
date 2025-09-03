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
//	CLASS:	RUNTIME_HEADER
//	The RUNTIME_HEADER class implements ...
//
//	03-01-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__RUNTIME_SOURCES_APPLICATION_RUNTIME_HEADER_H__
#define	__RUNTIME_SOURCES_APPLICATION_RUNTIME_HEADER_H__

#include	INCL_KCORE(IniFile/IniFileKeyHolder)

class	RUNTIME_HEADER : public INIFILE_KEY_HOLDER
{
public:
							
						RUNTIME_HEADER();
	virtual			   ~RUNTIME_HEADER();

			bool		GetFullscreen() const;
			bool		GetDatabaseConnect() const;
			QDT_STRING	GetDatabaseName() const;
			QDT_STRING	GetUserDBLogin() const;
			bool		GetNoSound() const;
			QDT_STRING	GetBigFileName() const;
			QDT_STRING	GetLUAFileName() const;
			bool		GetShadows() const;
			bool		GetRecordObjectLoadings() const;
			bool		GetEnableDataBank() const;
			bool		GetEnableCheat() const;
			bool		GetEnableNoMessageBox() const;
			bool		GetEnableLoadingMsg() const;
			bool		GetEnableRetrieveLog() const;
			bool		GetEnableVibration() const;
			bool		GetEnableDebugMenu() const;
			bool		GetQdtErrorOnForceLoad() const;
			bool		GetEnablePS2Emulation() const;
			bool		GetEnableScriptOutput() const;
			bool		GetForcePackedMode() const;
			bool		GetWindowOnTop() const;
			bool		GetViewerMode() const;
			bool		GetUSMode() const;

			QDT_STRING	GetDataBankExportFile();

			QDT_STRING	GetMainSection() const;
			
			QDT_STRING	GetDataVersion() const;

	static	const char *	SECTION;
	static	const char *	DATABASE_KEY;
	static	const char *	FULLSCREEN_KEY;
	static	const char *	NO_SOUND_KEY;
	static	const char *	DATABASE_NAME_KEY;
	static	const char *	USER_DB_LOGIN_KEY;
	static	const char *	BIGFILE_NAME_KEY;
	static	const char *	LUAFILE_NAME_KEY;
	static	const char *	DATABANK_EXPORT_FILE_KEY;
	static	const char *	RECORD_OBJECT_LOADINGS;
	static	const char *	ENABLE_DATABANK_KEY;
	static	const char *	ENABLE_CHEAT_KEY;
	static	const char *	ENABLE_NO_MESSAGE_BOX_KEY;
	static	const char *	ENABLE_LOADING_MSG;
	static	const char *	ENABLE_RETRIEVE_LOG;
	static	const char *	ENABLE_VIBRATION_KEY;
	static	const char *	DATA_VERSION;
	static	const char *	ENABLE_DEBUG_MENU;
	static	const char *	QDT_ERROR_ON_FORCE_LOAD;
	static	const char *	ENABLE_PS2_EMULATION;
	static	const char *	ENABLE_SCRIPT_OUTPUT;
	static  const char *	ENABLE_SHADOWS;
	static  const char *	FORCE_PACKED_MODE;
	static  const char *	WINDOW_ON_TOP;
	static  const char *	VIEWER_MODE;
	static	const char *	US_MODE;

private:

	bool	GetBool(const char * sKey) const;

};


#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


