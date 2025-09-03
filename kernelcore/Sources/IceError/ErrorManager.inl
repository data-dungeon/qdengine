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
//	CLASS:	ERROR_MANAGER
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		Creates the Singleton.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::Create()
{
	QDT_ASSERT(!_pSingleton);
	_pSingleton = new ERROR_MANAGER();
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		Releases the Singleton.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::Release()
{
	QDT_ASSERT(_pSingleton);
	delete _pSingleton;
	_pSingleton = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		Access the Singleton.
//
//	@return		the error manager instance.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MANAGER *	ERROR_MANAGER::Instance()
{
	QDT_ASSERT(_pSingleton);
	return _pSingleton;
}

//-----------------------------------------------------------------------------
//	Name:		SetInstance
//	Object:		
//	06-02-20:	AMA - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::SetInstance(ERROR_MANAGER *	pErrorManager)
{
	_pSingleton = pErrorManager;
}

//-----------------------------------------------------------------------------
//	Name:		IsInstancied
//	Object:		
//	06-03-06:	DAC - Created
//-----------------------------------------------------------------------------
bool	ERROR_MANAGER::IsInstancied()
{
	return (_pSingleton != NULL);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
