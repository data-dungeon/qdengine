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
//	CLASS:	DATAS_PSEUDO_DYNAMIC_LIT
//
//	02-06-28:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DATAS_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
DATAS_PSEUDO_DYNAMIC_LIT::DATAS_PSEUDO_DYNAMIC_LIT()
:
_PreviousColor(0.0f, 0.0f, 0.0f, 0.0f)
{
	// Do  nothing
}

//-----------------------------------------------------------------------------
//	Name:		DATAS_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
DATAS_PSEUDO_DYNAMIC_LIT::DATAS_PSEUDO_DYNAMIC_LIT(const DATAS_PSEUDO_DYNAMIC_LIT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		DATAS_PSEUDO_DYNAMIC_LIT destructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
DATAS_PSEUDO_DYNAMIC_LIT::~DATAS_PSEUDO_DYNAMIC_LIT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
DATAS_PSEUDO_DYNAMIC_LIT &	DATAS_PSEUDO_DYNAMIC_LIT::operator=(const DATAS_PSEUDO_DYNAMIC_LIT & C)
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
void	DATAS_PSEUDO_DYNAMIC_LIT::Copy(const DATAS_PSEUDO_DYNAMIC_LIT & C)
{
	_vMaterials			= C._vMaterials;
	_vVertices			= C._vVertices;
	_vVerticesPacked	= C._vVerticesPacked;
}

//-----------------------------------------------------------------------------
//	Name:		PushLightmappedMaterial
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::PushMaterial(const MATERIAL_PSEUDO_DYNAMIC_LIT &	MaterialPDL)
{
	_vMaterials.PushTail(MaterialPDL);
}

//-----------------------------------------------------------------------------
//	Name:		PushPureVertices
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::PushVertices(const VERTICES_PSEUDO_DYNAMIC_LIT &	VerticesPDL)
{
	_vVertices.PushTail(VerticesPDL);
}

//-----------------------------------------------------------------------------
//	Name:		PushPureVertices
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::PushVerticesPacked(const VERTICES_PSEUDO_DYNAMIC_LIT_PACKED &	VerticesPDL)
{
	_vVerticesPacked.PushTail(VerticesPDL);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	DATAS_PSEUDO_DYNAMIC_LIT::Write(IO_STREAM &	Stream) const
{
	Stream.DebugOutput("Number of materials : ");
	Stream << _vMaterials.GetSize();
	Stream.DebugOutput("\r\n");

	unsigned int nEach = 0;
	while (nEach < _vMaterials.GetSize())
	{
		Stream << _vMaterials[nEach];
		++nEach;
	}

	Stream.DebugOutput("Number of pure vertices : ");
	Stream << _vVertices.GetSize();
	Stream.DebugOutput("\r\n");

	nEach = 0;
	while (nEach < _vVertices.GetSize())
	{
		_vVertices[nEach].Write(Stream);
		++nEach;
	}
}
#endif

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::ReadV0(IO_STREAM & Stream)
{
	MATERIAL_PSEUDO_DYNAMIC_LIT			Mat;
	VERTICES_PSEUDO_DYNAMIC_LIT			Vert;
	VERTICES_PSEUDO_DYNAMIC_LIT_PACKED	VertPacked;
	unsigned int nNbr;
	unsigned int nEach;

	Stream >> nNbr;
	nEach = 0;
	while (nEach < nNbr)
	{
		_vMaterials.PushTail(Mat);
		Stream >> _vMaterials[nEach];
		++nEach;
	}

	Stream >> nNbr;
	nEach = 0;
	while (nEach < nNbr)
	{
		_vVertices.PushTail(Vert);
		_vVertices[nEach].ReadV0(Stream);
		++nEach;
	}

	Stream >> nNbr;
	nEach = 0;
	while (nEach < nNbr)
	{
		_vVerticesPacked.PushTail(VertPacked);
		_vVerticesPacked[nEach].ReadV0(Stream);
		++nEach;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::ReadV1(IO_STREAM & Stream)
{
	MATERIAL_PSEUDO_DYNAMIC_LIT			Mat;
	VERTICES_PSEUDO_DYNAMIC_LIT			Vert;
	VERTICES_PSEUDO_DYNAMIC_LIT_PACKED	VertPacked;
	unsigned int nNbr;
	unsigned int nEach;

	Stream >> nNbr;
	nEach = 0;
	while (nEach < nNbr)
	{
		_vMaterials.PushTail(Mat);
		Stream >> _vMaterials[nEach];
		++nEach;
	}

	Stream >> nNbr;
	nEach = 0;
	while (nEach < nNbr)
	{
		_vVertices.PushTail(Vert);
		_vVertices[nEach].ReadV1(Stream);
		++nEach;
	}
}

#endif

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-24:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	DATAS_PSEUDO_DYNAMIC_LIT::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(DATAS_PSEUDO_DYNAMIC_LIT) );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		InitBinary
//	Object:		
//	03-11-24:	VMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::InitBinary()
{
}

//-----------------------------------------------------------------------------
//	Name:		CloseBinary
//	Object:		
//	04-05-25:	VMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::CloseBinary()
{
	_vMaterials.Destroy();
	_vVertices.Destroy();
	_vVerticesPacked.Destroy();
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-07-04:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::Update(const COLOR &NewColor)
{
	if (_PreviousColor != NewColor)
	{		
		unsigned int nEach, nNbr = _vMaterials.GetSize();
		for (nEach = 0 ; nEach < nNbr ; ++nEach)
		{
			_vMaterials[nEach].Update(NewColor);
		}
			
		nNbr = _vVertices.GetSize();
		for (nEach = 0 ; nEach < nNbr ; ++nEach)
		{
			_vVertices[nEach].Update(_PreviousColor, NewColor);
		}
		
		nNbr = _vVerticesPacked.GetSize();
		for (nEach = 0 ; nEach < nNbr ; ++nEach)
		{
			_vVerticesPacked[nEach].Update(_PreviousColor, NewColor);
		}
		
		_PreviousColor = NewColor;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-11-14:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::SetToZero(unsigned int	nIndex,
											bool			bPacked)
{
	const static COLOR Zero(255, 0, 0, 0);

	if (bPacked)
	{
		_vVerticesPacked[nIndex].Update(_PreviousColor, Zero);
	}
	else
	{
		_vVertices[nIndex].Update(_PreviousColor, Zero);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Restore
//	Object:		
//	02-11-14:	RMA - Created
//-----------------------------------------------------------------------------
void	DATAS_PSEUDO_DYNAMIC_LIT::Restore(unsigned int	nIndex,
										  bool			bPacked)
{
	const static COLOR Zero(255, 0, 0, 0);

	if (bPacked)
	{
		_vVerticesPacked[nIndex].Update(Zero, _PreviousColor);
	}
	else
	{
		_vVertices[nIndex].Update(Zero, _PreviousColor);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
