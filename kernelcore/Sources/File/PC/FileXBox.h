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
//	CLASS:	FILE_XBOX
//	The FILE_XBOX class implements ...
//
//	03-05-05:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	FILE_XBOX : public FILE_ABC
{
public:

	enum
	{
		SECTOR_SIZE = 512
	};

	FILE_XBOX();
	~FILE_XBOX();

	int	 GetSectorSize() const;
	bool Open(const char * const pFileName, const F_MODE eMode, const bool bSharing);
	bool Close();
	bool IsIdle() const;
	bool IsExist(const char * const pFileName) const;
	bool IsWriteProtected(const char * const pFileName) const;
	bool Seek(const F_SEEK eInitialPosition, const int nOffset);
	bool Read(void *  pBuffer, const int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
	bool Write(void *  pBuffer, const int nPosition, const int nSize, const F_SYNC eSync = FS_NO_WAIT);
	bool Delete(const char * const pFileName) const;
	void Loop();

private:
	
	HANDLE		_Handle;
	OVERLAPPED	_Overlapped;

};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
