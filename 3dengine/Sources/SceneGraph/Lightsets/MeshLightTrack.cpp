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
//	CLASS:	MESH_LIGHT_TRACK
//
//	02-10-17:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)
#include	INC_3DENGINE(SceneGraph\Entities\Portal)
#include	INC_3DENGINE(M3D)

#ifdef	_DEBUG
#else
	#include	INL_3DENGINE(SceneGraph\Entities\Mesh)
	#include	INL_3DENGINE(SceneGraph\Entities\Portal)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MESH_LIGHT_TRACK constructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
MESH_LIGHT_TRACK::MESH_LIGHT_TRACK()
:
_MeshInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION) 
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MESH_LIGHT_TRACK constructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
MESH_LIGHT_TRACK::MESH_LIGHT_TRACK(const MESH_LIGHT_TRACK & C)
{
	Copy(C); 
}

//-----------------------------------------------------------------------------
//	Name:		MESH_LIGHT_TRACK destructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
MESH_LIGHT_TRACK::~MESH_LIGHT_TRACK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
MESH_LIGHT_TRACK &	MESH_LIGHT_TRACK::operator=(const MESH_LIGHT_TRACK & C)
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
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_LIGHT_TRACK::Copy(const MESH_LIGHT_TRACK & C)
{
	_nMeshType		 = C._nMeshType;
	_MeshInst		 = C._MeshInst;
	_vGeometryTracks = C._vGeometryTracks;
}

//-----------------------------------------------------------------------------
//	Name:		LoadAndApply
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_LIGHT_TRACK::LoadAndApply(IO_STREAM &	Stream)
{
	int						i, nNbrGeometries;
	GEOMETRY_LIGHT_TRACK	GeometryLightTrack;
	
	Stream >> _MeshInst;
	_nMeshType = _MeshInst.GetType();
	
	if (_nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH)
	{
		MESH *pMesh = GetMesh();
	
		Stream >> nNbrGeometries;
		for (i = 0 ; i < nNbrGeometries ; ++i)
		{
			GeometryLightTrack.ReadAndApplyColors(Stream, pMesh, i);
		}
	}
	else
	{
		// Portal
		PORTAL_ABC *pPortal = GetPortal();
	
		Stream >> nNbrGeometries;
		for (i = 0 ; i < nNbrGeometries ; ++i)
		{
			GeometryLightTrack.ReadAndApplyColors(Stream, pPortal, i);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Preload
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_LIGHT_TRACK::Preload(IO_STREAM &	Stream)
{
	int						i, nNbrGeometries;
	GEOMETRY_LIGHT_TRACK	GeometryLightTrack;

	Stream >> _MeshInst;
	_nMeshType = _MeshInst.GetType();

	QDT_ASSERT(_nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH ||
			   _nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MIRROR_PORTAL ||
			   _nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TWIN_PORTAL);

	Stream >> nNbrGeometries;
	for (i = 0 ; i < nNbrGeometries ; ++i)
	{
		_vGeometryTracks.PushTail(GeometryLightTrack);
		_vGeometryTracks[i].Read(Stream);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-10-18:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	MESH_LIGHT_TRACK::Write(IO_STREAM &	Stream) const
{
	unsigned int i;

	Stream.DebugOutput("Mesh : ");
	Stream.DebugOutput(_MeshInst.GetName());
	Stream.DebugOutput(" ");
	Stream << _MeshInst;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of geometries = ");
	Stream << GetNbrGeometryTracks();
	Stream.DebugOutput("\r\n");

	for (i = 0 ; i < GetNbrGeometryTracks() ; ++i)
	{
		Stream.DebugOutput("Geometry ");
		Stream.DebugOutput((int)i);
		Stream.DebugOutput("\r\n");
		GetGeometryTrack(i).Write(Stream);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Push
//	Object:		
//	02-10-18:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_LIGHT_TRACK::Push(const GEOMETRY_LIGHT_TRACK &	GLT)
{
	_vGeometryTracks.PushTail(GLT);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-10-18:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_LIGHT_TRACK::Clear()
{
	_vGeometryTracks.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_LIGHT_TRACK::Update(float	rRatio)
{
	switch (_nMeshType)
	{
	default:
		QDT_FAIL();
		break;
		
	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH :
		{	
			MESH *pMesh = GetMesh();
			
			unsigned int i, nNbrGeometries = _vGeometryTracks.GetSize();
			for (i = 0 ; i < nNbrGeometries ; ++i)
			{
				_vGeometryTracks[i].Update(pMesh->GetGeometry(i), rRatio);
			}
		}
		break;
		
	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MIRROR_PORTAL :
	case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TWIN_PORTAL :
		{
			PORTAL_ABC *pPortal = GetPortal();
			
			unsigned int i, nNbrGeometries = _vGeometryTracks.GetSize();
			for (i = 0 ; i < nNbrGeometries ; ++i)
			{
				_vGeometryTracks[i].Update(pPortal->GetGeometry(i), rRatio);
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetMesh
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
MESH *	MESH_LIGHT_TRACK::GetMesh() const
{
	QDT_ASSERT(_nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH);
	return (static_cast<MESH*>(GetMeshInst().GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		GetPortal
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
PORTAL_ABC *	MESH_LIGHT_TRACK::GetPortal() const
{
	QDT_ASSERT(_nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MIRROR_PORTAL ||
			   _nMeshType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TWIN_PORTAL);
	return (static_cast<PORTAL_ABC*>(GetMeshInst().GetCommunicator()));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
