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
//	CLASS:	MEMORYCARD_MANAGER
//	The MEMORYCARD_MANAGER class implements ...
//
//	03-05-21:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	MEMORYCARD_MANAGER
{
public:
	
	enum MC_ERROR
	{
		MC_NO_ERROR	= 0,
		MC_FILE_NOT_FOUND	= -1,
		MC_READ_FAILED		= -2,
		MC_OPEN_FAILED		= -3,
		MC_CLOSE_FAILED		= -4,
		MC_NOT_FOUND		= -5,
		MC_NOT_FORMATTED	= -6,
		MC_NOT_ENOUGH_SPACE = -7,
		MC_CORRUPTED		= -8,
		MC_CHANGED_CARD		= -9,
	};
	
	static	FILE_ABC*	OpenFile(const char * const pFileName, const FILE_ABC::F_MODE eMode, MC_ERROR &eError);
	static	MC_ERROR	CloseFile(FILE_ABC* pFile);
	static	MC_ERROR	DeleteFile(const char * const pFileName);
	static	bool		IsFileExist(const char * const pFileName, MC_ERROR & eError );
	
	static	QDT_STRING	GetFirstFileName(const QDT_STRING & sFindStr, MC_ERROR & eError);
	static	QDT_STRING	GetNextFileName(const QDT_STRING & sFindStr, MC_ERROR & eError);

	static	bool		Init(MC_ERROR & eError);
	static	bool		FormatMemoryCard( MC_ERROR & eError );
	static	bool		IsMemoryCardReady( int nNeededSpace, MC_ERROR & eError );
	static	bool		IsMemoryCardPrepared(MC_ERROR & eError);
	static	bool		PrepareMemoryCard( MC_ERROR &	eError);
	static	bool		CleanMemoryCard( MC_ERROR &	eError);

	static	int			GetFreeSize( void );
	static	void		ExternalAppFreeSpace( int nbBlocksNeeded );

	static	char *			ComputeSignature(MEM_STREAM &inBuffer, unsigned int nSize, unsigned int &nSigSize);
	static	unsigned int	GetSignatureSize();

protected:
	MEMORYCARD_MANAGER();
	~MEMORYCARD_MANAGER();

#ifdef PLATFORM_XBOX
	static	void		CreateSaveFileName(char *pBuffer, const int nBufferSize, const char * const pFileName);
#endif

};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
