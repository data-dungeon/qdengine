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
//
//	03-01-08:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_RUN(Application/RuntimeHeader)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

const char *	RUNTIME_HEADER::SECTION						= "INI";
const char *	RUNTIME_HEADER::DATABASE_KEY				= "DatabaseConnect";
const char *	RUNTIME_HEADER::FULLSCREEN_KEY				= "Fullscreen";
const char *	RUNTIME_HEADER::NO_SOUND_KEY				= "NoSound";
const char *	RUNTIME_HEADER::DATABASE_NAME_KEY			= "DatabaseName";
const char *	RUNTIME_HEADER::USER_DB_LOGIN_KEY			= "UserDBLogin";
const char *	RUNTIME_HEADER::BIGFILE_NAME_KEY			= "Bigfile";
const char *	RUNTIME_HEADER::LUAFILE_NAME_KEY			= "LUAFile";
const char *	RUNTIME_HEADER::DATABANK_EXPORT_FILE_KEY	= "DatabankFile";
const char *	RUNTIME_HEADER::RECORD_OBJECT_LOADINGS		= "RecordObjectLoadings";
const char *	RUNTIME_HEADER::ENABLE_DATABANK_KEY			= "EnableDatabank";
const char *	RUNTIME_HEADER::ENABLE_VIBRATION_KEY		= "EnableVibration";
const char *	RUNTIME_HEADER::ENABLE_CHEAT_KEY			= "Cheat";
const char *	RUNTIME_HEADER::ENABLE_NO_MESSAGE_BOX_KEY	= "NoMessageBox";
const char *	RUNTIME_HEADER::ENABLE_LOADING_MSG			= "LoadingMsg";
const char *	RUNTIME_HEADER::ENABLE_RETRIEVE_LOG			= "RetrieveLog";
const char *	RUNTIME_HEADER::DATA_VERSION				= "DataVersion";
const char *	RUNTIME_HEADER::ENABLE_DEBUG_MENU			= "DebugMenu";
const char *	RUNTIME_HEADER::QDT_ERROR_ON_FORCE_LOAD		= "QdtErrorOnForceLoad";
const char *	RUNTIME_HEADER::ENABLE_PS2_EMULATION		= "PS2Emulation";
const char *	RUNTIME_HEADER::ENABLE_SCRIPT_OUTPUT		= "ScriptOutput";
const char *	RUNTIME_HEADER::ENABLE_SHADOWS				= "Shadows";
const char *	RUNTIME_HEADER::FORCE_PACKED_MODE			= "ForcePackedMode";
const char *	RUNTIME_HEADER::WINDOW_ON_TOP				= "WindowOnTop";
const char *	RUNTIME_HEADER::VIEWER_MODE					= "ViewerMode";
const char *	RUNTIME_HEADER::US_MODE						= "USMode";

//-----------------------------------------------------------------------------
//	Name:		RUNTIME_HEADER constructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
RUNTIME_HEADER::RUNTIME_HEADER()
{
	AddBoolKey(	  SECTION, DATABASE_KEY,   true);
	AddBoolKey(	  SECTION, FULLSCREEN_KEY, false );
	AddBoolKey(	  SECTION, NO_SOUND_KEY,   false );
	AddStringKey( SECTION, DATABASE_NAME_KEY, "Base Nomad Soul 2");
	AddStringKey( SECTION, USER_DB_LOGIN_KEY, "ELE");
	AddStringKey( SECTION, BIGFILE_NAME_KEY, "BigFile");
	AddStringKey( SECTION, LUAFILE_NAME_KEY, "");
	AddStringKey( SECTION, DATABANK_EXPORT_FILE_KEY, "");
	AddStringKey( SECTION, DATA_VERSION, "PC");
	AddBoolKey(	  SECTION, RECORD_OBJECT_LOADINGS, false);
	AddBoolKey(	  SECTION, ENABLE_DATABANK_KEY, false);
	AddBoolKey(	  SECTION, ENABLE_VIBRATION_KEY, true);
	AddBoolKey(	  SECTION, ENABLE_CHEAT_KEY, true);
	AddBoolKey(	  SECTION, ENABLE_NO_MESSAGE_BOX_KEY, false);
	AddBoolKey(	  SECTION, ENABLE_LOADING_MSG, false);
	AddBoolKey(	  SECTION, ENABLE_RETRIEVE_LOG, false);
	AddBoolKey(	  SECTION, ENABLE_DEBUG_MENU, false);
	AddBoolKey(	  SECTION, QDT_ERROR_ON_FORCE_LOAD, true);
	AddBoolKey(	  SECTION, ENABLE_PS2_EMULATION, false);
	AddBoolKey(	  SECTION, ENABLE_SCRIPT_OUTPUT, true);
	AddBoolKey(	  SECTION, ENABLE_SHADOWS, true);
	AddBoolKey(	  SECTION, FORCE_PACKED_MODE, false);
	AddBoolKey(	  SECTION, WINDOW_ON_TOP, false);
	AddBoolKey(	  SECTION, VIEWER_MODE, false);
	AddBoolKey(	  SECTION, US_MODE, false);
}   

//-----------------------------------------------------------------------------
//	Name:		~RUNTIME_HEADER destructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
RUNTIME_HEADER::~RUNTIME_HEADER()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetMainSection
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetMainSection() const
{
	return ( SECTION );
}

//-----------------------------------------------------------------------------
//	Name:		GetFullscreen
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetFullscreen() const
{
	return	(GetBool(FULLSCREEN_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetDatabaseConnect
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetDatabaseConnect() const
{
	return	(GetBool(DATABASE_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetNoSound
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetNoSound() const
{
	return	(GetBool(NO_SOUND_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetDatabaseName
//	Object:		
//	03-12-15:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetDatabaseName() const
{
	QDT_ASSERT( IsKeyString( SECTION, DATABASE_NAME_KEY) == true );

	QDT_STRING	sValue;

	GetStringKeyValue( SECTION, DATABASE_NAME_KEY, sValue );

	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetUserDBLogin
//	Object:		
//	03-12-15:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetUserDBLogin() const
{
	QDT_ASSERT( IsKeyString( SECTION, USER_DB_LOGIN_KEY) == true );

	QDT_STRING	sValue;

	GetStringKeyValue( SECTION, USER_DB_LOGIN_KEY, sValue );

	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetBigFileName
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetBigFileName() const
{
	QDT_ASSERT( IsKeyString( SECTION, BIGFILE_NAME_KEY ) == true );

	QDT_STRING	sValue;

	GetStringKeyValue( SECTION, BIGFILE_NAME_KEY , sValue );

	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetDataBankExportFile
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetLUAFileName() const
{
	QDT_ASSERT( IsKeyString( SECTION, LUAFILE_NAME_KEY ) == true );
	QDT_STRING	sValue;
	GetStringKeyValue( SECTION, LUAFILE_NAME_KEY , sValue );
	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetDataBankExportFile
//	Object:		
//	04-01-12:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetDataBankExportFile()
{
	QDT_ASSERT( IsKeyString( SECTION, DATABANK_EXPORT_FILE_KEY ) == true );

	QDT_STRING	sValue;

	GetStringKeyValue( SECTION, DATABANK_EXPORT_FILE_KEY , sValue );

	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetRecordObjectLoadings
//	Object:		
//	04-01-19:	DAC - Created
//-----------------------------------------------------------------------------
bool RUNTIME_HEADER::GetRecordObjectLoadings() const
{
	return	(GetBool(RECORD_OBJECT_LOADINGS));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableDataBank
//	Object:		
//	04-01-20:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableDataBank() const
{
	return	(GetBool(ENABLE_DATABANK_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetBool
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetBool(const char *	sKey) const
{
	QDT_ASSERT( IsKeyBool( SECTION, sKey ) == true );

	bool	bValue;

	GetBoolKeyValue( SECTION, sKey, bValue );

	return ( bValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableCheat
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableCheat() const
{
	return	(GetBool(ENABLE_CHEAT_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableNoMessageBox
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableNoMessageBox() const
{
	return	(GetBool(ENABLE_NO_MESSAGE_BOX_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetShadows
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetShadows() const
{
	return	(GetBool(ENABLE_SHADOWS));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableLoadingMsg
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableLoadingMsg() const
{
	return	(GetBool(ENABLE_LOADING_MSG));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableRetrieveLog
//	Object:		
//	04-02-19:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableRetrieveLog() const
{
	return	(GetBool(ENABLE_RETRIEVE_LOG));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableDebugMenu
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableDebugMenu() const
{
	return	(GetBool(ENABLE_DEBUG_MENU));
}


//-----------------------------------------------------------------------------
//	Name:		GetDataVersion
//	Object:		
//	04-03-10:	AMA - Created
//-----------------------------------------------------------------------------
QDT_STRING	RUNTIME_HEADER::GetDataVersion() const
{
	QDT_ASSERT( IsKeyString( SECTION, DATA_VERSION ) == true );

	QDT_STRING	sValue;

	GetStringKeyValue( SECTION, DATA_VERSION , sValue );

	return	(sValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableVibration
//	Object:		
//	04-03-19:	VMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableVibration() const
{
	return	(GetBool(ENABLE_VIBRATION_KEY));
}

//-----------------------------------------------------------------------------
//	Name:		GetQdtErrorOnForceLoad
//	Object:		
//	04-05-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetQdtErrorOnForceLoad() const
{
	return	(GetBool(QDT_ERROR_ON_FORCE_LOAD));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnablePS2Emulation()
//	Object:		
//	04-05-03:	AMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnablePS2Emulation() const
{
	return	(GetBool(ENABLE_PS2_EMULATION));
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableScriptOutput
//	Object:		
//	04-05-03:	AMA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetEnableScriptOutput() const
{
	return	(GetBool(ENABLE_SCRIPT_OUTPUT));
}

//-----------------------------------------------------------------------------
//	Name:		GetForcePackedMode
//	Object:		
//	04-09-02:	JJA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetForcePackedMode() const
{
	return	(GetBool(FORCE_PACKED_MODE));
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowOnTop
//	Object:		
//	04-09-28:	JJA - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetWindowOnTop() const
{
	return	(GetBool(WINDOW_ON_TOP));
}

//-----------------------------------------------------------------------------
//	Name:		GetViewerMode
//	Object:		
//	04-10-22:	VPI - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetViewerMode() const
{
	return	(GetBool(VIEWER_MODE));
}

//-----------------------------------------------------------------------------
//	Name:		GetUSMode
//	Object:		
//	05-04-26:	DAC - Created
//-----------------------------------------------------------------------------
bool	RUNTIME_HEADER::GetUSMode() const
{
	return	(GetBool(US_MODE));
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
