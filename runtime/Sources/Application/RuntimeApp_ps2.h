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
//	01-09-17:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


//typedef char u_long128;

class	RUNTIME_APP
{
public:
	RUNTIME_APP();
	~RUNTIME_APP();
	

	// Overridables
	bool	InitInstance(int argc, char *argv[]);
	bool	ExitInstance();
	bool	OnIdle(long	lCount);
	int		Run();

	const QDT_STRING&	GetBCName() const;
	const QDT_STRING&	GetBFName() const;

	QDT_STRING	GetRuntimeVersionString() const;

	void	ReadCommandLine(int argc, char *argv[]);
	void	ReadElfArguments(char* args);

protected:
	bool		InitOs();
	bool		InitIAMRuntime();

	void		SetFullScreen(const bool bFlag);
	bool		IsFullScreen() const;
	void		EnableShadows(const bool bFlag);
	bool		AreShadowsEnabled() const;

	QDT_STRING	GetClassCmdLine() const;
	QDT_STRING	GetBCCmdLine() const;
	
	QDT_STRING	GetDBCmdLine() const;
	QDT_STRING	GetWindowCmdLine() const;
	QDT_STRING	GetSoundCmdLine() const;
	QDT_STRING	GetDefaultClassToBoot() const;
	QDT_STRING	GetDefaultByteCodeName() const;
	QDT_STRING	GetIPCmdLine() const;
	QDT_STRING	GetBFCmdLine() const;
	QDT_STRING	GetDefaultBigFileName() const;
	QDT_STRING	GetNoShadowsCmdLine() const;
	QDT_STRING	GetNoDatabankCmdLine() const;	
	const QDT_STRING &	GetClassToBoot() const;

	void				SetBCName(const QDT_STRING& strBCName);
	void				SetBFName(const QDT_STRING& strBFName);
	void				SetIPAddress(const QDT_STRING& strIPAddress);
	void				SetClassToBoot(const QDT_STRING & strClassName);

private:
	static RUNTIME_APP	RA;

	static char	_s_sElfArgs[ 128 ] __attribute__((section(".args")));
	
	bool	_bQuit;
	bool	_bFullScreen;
	bool	_bShadowsEnabled;

	QDT_STRING	_strBCName;
	QDT_STRING	_strBFName;
	QDT_STRING	_strClassToBoot;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
