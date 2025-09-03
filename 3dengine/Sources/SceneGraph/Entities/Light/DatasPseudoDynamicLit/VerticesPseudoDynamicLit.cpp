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
//	CLASS:	VERTICES_PSEUDO_DYNAMIC_LIT
//
//	02-06-28:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(OldCom/Database\DatabaseManager)
#include	INC_KRNCORE(Display\VerticesPseudoDynamicLitABC)
#include	INC_KRNCORE(Display\GeometryABC)
#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)

#ifndef _DEBUG
#include	INL_KRNCORE(Display\GeometryABC)
#include	INL_KRNCORE(Display/RenderManager)
#include	INL_3DENGINE(SceneGraph\Entities\Mesh)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTICES_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
VERTICES_PSEUDO_DYNAMIC_LIT::VERTICES_PSEUDO_DYNAMIC_LIT()
:
_MeshInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_nNumGeometry(0),
_pVertices(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VERTICES_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
VERTICES_PSEUDO_DYNAMIC_LIT::VERTICES_PSEUDO_DYNAMIC_LIT(const VERTICES_PSEUDO_DYNAMIC_LIT & C)
:
_MeshInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_nNumGeometry(0),
_pVertices(0)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		VERTICES_PSEUDO_DYNAMIC_LIT destructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
VERTICES_PSEUDO_DYNAMIC_LIT::~VERTICES_PSEUDO_DYNAMIC_LIT()
{
	delete _pVertices;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
VERTICES_PSEUDO_DYNAMIC_LIT &	VERTICES_PSEUDO_DYNAMIC_LIT::operator=(const VERTICES_PSEUDO_DYNAMIC_LIT & C)
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
void	VERTICES_PSEUDO_DYNAMIC_LIT::Copy(const VERTICES_PSEUDO_DYNAMIC_LIT & C)
{
	_nNumGeometry	= C._nNumGeometry;
	_MeshInst		= C._MeshInst;
	
	delete _pVertices;
	_pVertices = VERTICES_PSEUDO_DYNAMIC_LIT_FACTORY::Create(C._pVertices);
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticesPseudoDynamicLitABC
//	Object:		
//	02-11-18:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::SetVerticesPseudoDynamicLitABC(VERTICES_PSEUDO_DYNAMIC_LIT_ABC *	pVertices)
{
	_pVertices = pVertices;
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometry
//	Object:		
//	02-10-29:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::SetGeometry(unsigned int	nGeometry)
{
	_nNumGeometry = nGeometry;
}

//-----------------------------------------------------------------------------
//	Name:		SetMesh
//	Object:		
//	02-10-29:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::SetMesh(const COM_INSTANCE & MeshInst)
{
	_MeshInst = MeshInst;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::Write(IO_STREAM &	Stream) const
{
	QDT_ASSERT(_MeshInst.HasHandle());
	QDT_ASSERT(_pVertices);

	Stream.DebugOutput("VERTICES_PSEUDO_DYNAMIC_LIT Mesh: ");
	Stream.DebugOutput(_MeshInst.GetName());
	Stream.DebugOutput("\r\n");
	Stream << _MeshInst;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Geometry: ");
	Stream << _nNumGeometry;
	Stream.DebugOutput("\r\n");

	Stream << _pVertices->GetFormatType();
	_pVertices->Write(Stream);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::ReadV0(IO_STREAM &	Stream)
{
	Stream >> _MeshInst;	
	Stream >> _nNumGeometry;

	MESH			*pMesh		= static_cast<MESH*>(_MeshInst.GetCommunicator());
	GEOMETRY_ABC	*pGeometry	= pMesh->GetGeometry(_nNumGeometry);

	_pVertices = VERTICES_PSEUDO_DYNAMIC_LIT_FACTORY::Create(VERTICES_PSEUDO_DYNAMIC_LIT_ABC::FORMAT_TYPE_PC);
	_pVertices->Read(Stream, pGeometry);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::ReadV1(IO_STREAM &	Stream)
{
	int nFormat;

	Stream >> _MeshInst;
	
	Stream >> _nNumGeometry;

	Stream >> nFormat;

	MESH			*pMesh		= static_cast<MESH*>(_MeshInst.GetCommunicator());
	GEOMETRY_ABC	*pGeometry	= pMesh->GetGeometry(_nNumGeometry);

	_pVertices = VERTICES_PSEUDO_DYNAMIC_LIT_FACTORY::Create(static_cast<VERTICES_PSEUDO_DYNAMIC_LIT_ABC::FORMAT_TYPE>(nFormat));
	_pVertices->Read(Stream, pGeometry); 
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-10-29:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICES_PSEUDO_DYNAMIC_LIT::Update(const COLOR &PreviousColor, 
											const COLOR &NewColor)
{

	QDT_ASSERT(_MeshInst.HasHandle());
	QDT_ASSERT(_MeshInst.IsLoaded());

	MESH			*pMesh		= static_cast<MESH*>(_MeshInst.GetCommunicator());

	GEOMETRY_ABC	*pGeometry	= pMesh->GetGeometry(_nNumGeometry);

	if (pGeometry->GetVersion() > 8) // Converted geometry => can't be updated (a virer quand on sera sur de ne plus lire d'anciens meshes)
	{
		_pVertices->UpdateGeometry(pGeometry, PreviousColor, NewColor);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
