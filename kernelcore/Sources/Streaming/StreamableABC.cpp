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
//
//	03-04-10:	AMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SOUND_DATA_ABC constructor
//	Object:		
//	03-04-10:	AMA - Created
//-----------------------------------------------------------------------------
SOUND_DATA_ABC::SOUND_DATA_ABC()
:
_nNbrRef(0),
_nLastLoadedBlockID(-1)
{
	//Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		SOUND_DATA_ABC destructor
//	Object:		
//	03-04-10:	AMA - Created
//-----------------------------------------------------------------------------
SOUND_DATA_ABC::~SOUND_DATA_ABC()
{
	Clean();
}

//-----------------------------------------------------------------------------
//	Name:		Clean
//	Object:		
//	03-04-28:	AMA - Created
//-----------------------------------------------------------------------------
void	SOUND_DATA_ABC::Clean()
{
	int nIndex;

	for ( nIndex = ( _vSoundBlock.GetSize() - 1 ); nIndex >= 0 ; --nIndex )
	{
		SOUND_BLOCK * pSoundBlock;
		pSoundBlock = _vSoundBlock[nIndex];

		_vSoundBlock.Remove(_vSoundBlock.GetIterator(nIndex));
		delete pSoundBlock;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBlockLoaded
//	Object:		
//	03-04-28:	AMA - Created
//-----------------------------------------------------------------------------
int	SOUND_DATA_ABC::GetNbrBlockLoaded()
{
	return (_vSoundBlock.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseBlock
//	Object:		
//	03-04-10:	AMA - Created
//-----------------------------------------------------------------------------
void	SOUND_DATA_ABC::ReleaseBlock(int	nID)
{
	int nIndex;

	for ( nIndex = ( _vSoundBlock.GetSize() - 1 ); nIndex >= 0 ; --nIndex )
	{
		SOUND_BLOCK * pSoundBlock;
		pSoundBlock = _vSoundBlock[nIndex];
		if ( pSoundBlock->GetID() == nID )
		{
			// PB avec les références pour le moment....
			//if ( pSoundBlock->GetNbrReference() == 1 )
			//{
				_vSoundBlock.Remove(_vSoundBlock.GetIterator(nIndex));
				delete pSoundBlock;
			//}
			//else
			//{
			//	_vSoundBlock[nIndex]->DecreaseReference();
			//}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetBlock
//	Object:		
//	03-04-10:	AMA - Created
//-----------------------------------------------------------------------------
SOUND_BLOCK *	SOUND_DATA_ABC::GetBlock(int	nID)
{
	SOUND_BLOCK * pSoundBlock;

	pSoundBlock = FindBlock(nID);

	if ( pSoundBlock )
	{
		if ( ( (nID + 1) > _nLastLoadedBlockID ) && ( _nLastLoadedBlockID < (GetNbrPartitions() - 1) ) )
		{
			LoadPartition(_nLastLoadedBlockID + 1, false);
		}
		//pSoundBlock->IncreaseReference();
		return pSoundBlock;
	}

	// The block is not Loaded!!!!!
	// We load it but it's too late....
	LoadPartition(nID, true);

	return ( GetBlock(nID) );
}

//-----------------------------------------------------------------------------
//	Name:		FindBlock
//	Object:		
//	03-04-15:	AMA - Created
//-----------------------------------------------------------------------------
SOUND_BLOCK *	SOUND_DATA_ABC::FindBlock(int	nID)
{
	int nIndex;

	for ( nIndex = 0 ; nIndex < _vSoundBlock.GetSize() ; ++nIndex )
	{
		if ( _vSoundBlock[nIndex]->GetID() == nID )
		{
			return (_vSoundBlock[nIndex]);
		}
	}
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		CreateBlock
//	Object:		
//	03-04-15:	AMA - Created
//-----------------------------------------------------------------------------
void	SOUND_DATA_ABC::CreateBlock(int				nID,
									MEM_STREAM *	pMemStream)
{
	// MEM_POOL_HEADER a rajouter
	SOUND_BLOCK * pSoundBlock;
	pSoundBlock = new SOUND_BLOCK(nID, pMemStream);
	_vSoundBlock.PushTail(pSoundBlock);
	_nLastLoadedBlockID = nID;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
