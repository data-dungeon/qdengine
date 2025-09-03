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
//	CLASS:	MATERIAL_PSEUDO_DYNAMIC_LIT
//
//	02-06-28:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(OldCom/Database\DatabaseManager)
#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(M3D)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MATERIAL_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_PSEUDO_DYNAMIC_LIT::MATERIAL_PSEUDO_DYNAMIC_LIT()
:
_MaterialInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_nNumLayer		(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MATERIAL_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_PSEUDO_DYNAMIC_LIT::MATERIAL_PSEUDO_DYNAMIC_LIT(const MATERIAL_PSEUDO_DYNAMIC_LIT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		MATERIAL_PSEUDO_DYNAMIC_LIT destructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_PSEUDO_DYNAMIC_LIT::~MATERIAL_PSEUDO_DYNAMIC_LIT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
MATERIAL_PSEUDO_DYNAMIC_LIT &	MATERIAL_PSEUDO_DYNAMIC_LIT::operator=(const MATERIAL_PSEUDO_DYNAMIC_LIT & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_PSEUDO_DYNAMIC_LIT::Copy(const MATERIAL_PSEUDO_DYNAMIC_LIT & C)
{
	_MaterialInst = C._MaterialInst;
	_nNumLayer	  = C._nNumLayer;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	MATERIAL_PSEUDO_DYNAMIC_LIT::Write(IO_STREAM &Stream) const
{
	Stream.DebugOutput("Material: ");
	Stream.DebugOutput(_MaterialInst.GetName());
	Stream.DebugOutput("(ID : ");
	Stream << _MaterialInst;
	Stream.DebugOutput(")\t");

	Stream.DebugOutput("Layer: ");
	Stream << _nNumLayer;
	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	MATERIAL_PSEUDO_DYNAMIC_LIT::Read(IO_STREAM &Stream)
{
	Stream >> _MaterialInst;
	Stream >> _nNumLayer;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-07-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATERIAL_PSEUDO_DYNAMIC_LIT::Update(const COLOR &C)
{
	if (_MaterialInst.GetHandle())
	{
		MATERIAL *pMaterial = static_cast<MATERIAL*>(_MaterialInst.GetCommunicator());
		if (pMaterial)
		{
			pMaterial->SetDynamicLightmapFactor(_nNumLayer, C);
		}
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
