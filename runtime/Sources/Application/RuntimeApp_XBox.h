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

class	RUNTIME_APP
{
public:

								RUNTIME_APP();
	virtual					   ~RUNTIME_APP();


public:

	// Overridables
	virtual	BOOL				InitInstance();
			BOOL				ExitInstance();
			BOOL				OnIdle(long	lCount);
	virtual	int					Run();

			QDT_STRING			GetRuntimeVersionString() const;

			bool				InitOs();
			bool				InitIAMRuntime();

			void				ReadInifile();

			void				AddAdditionnalBigfiles();
private:

	static	RUNTIME_APP		RA;
										
			bool			_bQuit;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
