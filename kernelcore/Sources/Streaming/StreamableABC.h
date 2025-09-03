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
//	CLASS:	SOUND_DATA_ABC
//	The SOUND_DATA_ABC class implements ...
//
//	03-04-10:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	SOUND_DATA_ABC
{
public:
	SOUND_DATA_ABC();
	~SOUND_DATA_ABC();

	void			ReleaseBlock(int nID);
	SOUND_BLOCK *	GetBlock(int nID);
	void			Clean();

	virtual void	Loop() = 0;
	virtual	int		GetNbrPartitions() = 0;
	virtual void	ReadHeader(int & nType, int & nSize, bool & bStereo, int & nSamplesPerSec, int & nBitsPerSample) = 0;

	int				GetNbrBlockLoaded();

protected:

	void			CreateBlock(int nID, MEM_STREAM * pMemStream);
	virtual void	LoadPartition(int nID, bool bForceLoad) = 0;
	virtual bool	PartitionIsLoaded(int nID) = 0;

private:

	SOUND_BLOCK *	FindBlock(int nID);
	
	QDT_VECTOR<SOUND_BLOCK *>	_vSoundBlock;
	int							_nNbrRef;
	int							_nLastLoadedBlockID;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
