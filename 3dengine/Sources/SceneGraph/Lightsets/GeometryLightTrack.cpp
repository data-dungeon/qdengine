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
//	CLASS:	GEOMETRY_LIGHT_TRACK
//
//	02-10-17:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCORE(Stream)
#include	INC_KRNCORE(Math\Color)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)
#include	INC_3DENGINE(SceneGraph\Entities\Portal)
#include	INC_3DENGINE(SceneGraph\Entities\Light)
#include	INC_3DENGINE(SceneGraph\Entities\Light\DatasPseudoDynamicLit)

#ifdef	_DEBUG
#else
	#include	INL_3DENGINE(SceneGraph\Entities\Mesh)
	#include	INL_3DENGINE(SceneGraph\Entities\Portal)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GEOMETRY_LIGHT_TRACK constructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_LIGHT_TRACK::GEOMETRY_LIGHT_TRACK()
:
_nNbrVertices(0),
_pColors(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GEOMETRY_LIGHT_TRACK constructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_LIGHT_TRACK::GEOMETRY_LIGHT_TRACK(const GEOMETRY_LIGHT_TRACK & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		GEOMETRY_LIGHT_TRACK destructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_LIGHT_TRACK::~GEOMETRY_LIGHT_TRACK()
{
	delete [] _pColors;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_LIGHT_TRACK &	GEOMETRY_LIGHT_TRACK::operator = (const GEOMETRY_LIGHT_TRACK & C)
{
	if	(this != &C)
	{
		delete [] _pColors;
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::Copy(const GEOMETRY_LIGHT_TRACK &	C)
{
	_nNbrVertices = C._nNbrVertices;
	_pColors = new COLOR [_nNbrVertices];
	for(int i = 0 ; i < _nNbrVertices ; ++i)
	{
		_pColors[i] = C._pColors[i];
	}

	_vPseudoDynamicLightInst		= C._vPseudoDynamicLightInst;
	_vVerticesPseudoDynamicLitIndex = C._vVerticesPseudoDynamicLitIndex;
	_vbPacked						= C._vbPacked;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-10-18:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	GEOMETRY_LIGHT_TRACK::Read(IO_STREAM &	Stream)
{
	int				i, nIndex, nNbrVertices;
	int				nNbrPseudoDynamicLights;
	bool			bPacked;
	COM_INSTANCE	Inst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

	Stream >> nNbrVertices;
	SetNbrVertices(nNbrVertices);
	for (i = 0 ; i < GetNbrVertices() ; ++i)
	{
		Stream >> _pColors[i];
	}

	Stream >> nNbrPseudoDynamicLights;
	for (i = 0 ; i < nNbrPseudoDynamicLights ; ++i)
	{
		Stream >> Inst;
		Stream >> nIndex;
		Stream >> bPacked;
		PushPseudoDynamicInfos(Inst, nIndex, bPacked);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-10-18:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	GEOMETRY_LIGHT_TRACK::Write(IO_STREAM &	Stream) const
{
	int i;

	Stream.DebugOutput("Number of vertices : ");
	Stream << GetNbrVertices();
	Stream.DebugOutput("\r\n");

	for (i = 0 ;  i < GetNbrVertices() ; ++i)
	{
		Stream.DebugOutput("Color ");
		Stream.DebugOutput(i);
		Stream.DebugOutput(" : ");
		Stream << GetColor(i);
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of vertices pseudo dynamic lit: ");
	Stream << _vVerticesPseudoDynamicLitIndex.GetSize();
	Stream.DebugOutput("\r\n");

	unsigned int j;
	for (j= 0 ; j < _vVerticesPseudoDynamicLitIndex.GetSize() ; ++j)
	{
		Stream << _vPseudoDynamicLightInst[j];
		Stream << _vVerticesPseudoDynamicLitIndex[j];
		Stream << _vbPacked[j];
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::ReadAndApplyColors(IO_STREAM &			Stream, 
												 MESH				   *pMesh,
												 int					nNumGeometry)
{
	unsigned int	i, nNbrLights, nIndex;
	bool			bPacked;
	COM_INSTANCE	Inst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

	QDT_ASSERT(pMesh);
	QDT_ASSERT(nNumGeometry < pMesh->GetNbrGeometries());

	Stream >> _nNbrVertices;

	GEOMETRY_ABC *pGeometries = pMesh->GetGeometries()[nNumGeometry];
	pGeometries->UpdateColors(Stream);

	Stream >> nNbrLights;
	for (i = 0 ; i < nNbrLights ; ++i)
	{
		Stream >> Inst;
		Stream >> nIndex;
		Stream >> bPacked;
		PushPseudoDynamicInfos(Inst, nIndex, bPacked);
	}

	for (i = 0 ; i < _vVerticesPseudoDynamicLitIndex.GetSize() ; ++i)
	{
		LIGHT_ABC					*pLight	= static_cast<LIGHT_ABC*>(_vPseudoDynamicLightInst[i].GetCommunicator());
		DATAS_PSEUDO_DYNAMIC_LIT	*pDatas	= pLight->GetPseudoDynamicDatas();

		pDatas->Restore(_vVerticesPseudoDynamicLitIndex[i], _vbPacked[i] > 0);
	}

	_vPseudoDynamicLightInst.Clear();
	_vVerticesPseudoDynamicLitIndex.Clear();
	_vbPacked.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ReadAndApplyColors
//	Object:		
//	03-02-19:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::ReadAndApplyColors(IO_STREAM	&Stream,
											     PORTAL_ABC	*pPortal, 
											     int		 nNumGeometry)
{
	unsigned int	i, nNbrLights, nIndex;
	bool			bPacked;
	COM_INSTANCE	Inst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

	QDT_ASSERT(pPortal);
	QDT_ASSERT(nNumGeometry < pPortal->GetNbrGeometries());

	Stream >> _nNbrVertices;

	GEOMETRY_ABC *pGeometries = pPortal->GetGeometries()[nNumGeometry];
	pGeometries->UpdateColors(Stream);

	Stream >> nNbrLights;
	for (i = 0 ; i < nNbrLights ; ++i)
	{
		Stream >> Inst;
		Stream >> nIndex;
		Stream >> bPacked;
		PushPseudoDynamicInfos(Inst, nIndex, bPacked);
	}

	for (i = 0 ; i < _vVerticesPseudoDynamicLitIndex.GetSize() ; ++i)
	{
		LIGHT_ABC					*pLight	= static_cast<LIGHT_ABC*>(_vPseudoDynamicLightInst[i].GetCommunicator());
		DATAS_PSEUDO_DYNAMIC_LIT	*pDatas	= pLight->GetPseudoDynamicDatas();

		pDatas->Restore(_vVerticesPseudoDynamicLitIndex[i], _vbPacked[i] > 0);
	}

	_vPseudoDynamicLightInst.Clear();
	_vVerticesPseudoDynamicLitIndex.Clear();
	_vbPacked.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrVertices
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::SetNbrVertices(int n)		
{ 
	_nNbrVertices = n ; 
	delete [] _pColors; 
	_pColors = new COLOR[n]; 
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::Update(GEOMETRY_ABC	*pGeometry,
									 float			 rRatio)
{
	QDT_ASSERT(_nNbrVertices == pGeometry->GetNbrVertices());

	int nRatio = static_cast<unsigned char>(255.0f * rRatio);
	COLOR  NewColor(255, 255, 255, 255);
	unsigned int i;

	for (i = 0 ; i < _vVerticesPseudoDynamicLitIndex.GetSize() ; ++i)
	{
		LIGHT_ABC					*pLight	= static_cast<LIGHT_ABC*>(_vPseudoDynamicLightInst[i].GetCommunicator());
		DATAS_PSEUDO_DYNAMIC_LIT	*pDatas	= pLight->GetPseudoDynamicDatas();

		pDatas->SetToZero(_vVerticesPseudoDynamicLitIndex[i], _vbPacked[i] > 0);
	}

	pGeometry->UpdateColors(_pColors, nRatio);
	
	for (i = 0 ; i < _vVerticesPseudoDynamicLitIndex.GetSize() ; ++i)
	{
		LIGHT_ABC					*pLight	= static_cast<LIGHT_ABC*>(_vPseudoDynamicLightInst[i].GetCommunicator());
		DATAS_PSEUDO_DYNAMIC_LIT	*pDatas	= pLight->GetPseudoDynamicDatas();

		pDatas->Restore(_vVerticesPseudoDynamicLitIndex[i], _vbPacked[i] > 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushPseudoDynamicInfos
//	Object:		
//	02-11-13:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::PushPseudoDynamicInfos(const COM_INSTANCE &	LightInst, 
													 int					nIndex,
													 bool					bPacked)
{
	_vPseudoDynamicLightInst.PushTail(LightInst);
	_vVerticesPseudoDynamicLitIndex.PushTail(nIndex);
	_vbPacked.PushTail(bPacked);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-11-13:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_LIGHT_TRACK::Clear()
{
	_vPseudoDynamicLightInst.Clear();
	_vVerticesPseudoDynamicLitIndex.Clear();
	_vbPacked.Clear();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
