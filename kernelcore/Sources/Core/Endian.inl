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
//	CLASS:	ENDIAN
//
//	05-11-22:	VMA - Created
//*****************************************************************************

//#include	"Root.h"
//#include	"Endian.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENDIAN constructor
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
ENDIAN::ENDIAN()
{
	_bSwap = false;;
}

//-----------------------------------------------------------------------------
//	Name:		~ENDIAN destructor
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
ENDIAN::~ENDIAN()
{
}

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::Create()
{
	QDT_ASSERT ( _pInstance == NULL );
	_pInstance = new ENDIAN;
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::Release()
{
	QDT_ASSERT ( _pInstance );
	delete _pInstance;
	_pInstance = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
ENDIAN *	ENDIAN::Instance()
{
	QDT_ASSERT ( _pInstance );
	return(_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		SetInstance
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::SetInstance(ENDIAN *	pInstance)
{
	_pInstance = pInstance;
}

//-----------------------------------------------------------------------------
//	Name:		Exist
//	Object:		
//	06-03-29:	VMA - Created
//-----------------------------------------------------------------------------
bool	ENDIAN::Exist()
{
	return (_pInstance != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		EnableSwap
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::EnableSwap(Bool	bSwap)
{
	_bSwap = bSwap;
}

//-----------------------------------------------------------------------------
//	Name:		EnableSwap
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::EnableSwap()
{
	 _bSwap = true;
}

//-----------------------------------------------------------------------------
//	Name:		DisableSwap
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::DisableSwap()
{
	_bSwap = false;
}

//-----------------------------------------------------------------------------
//	Name:		IsSwapEnabled
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Bool	ENDIAN::IsSwapEnabled()
{
	return (_bSwap);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
