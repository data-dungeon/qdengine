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
//	CLASS:	LIGHT_TRACK
//
//	02-10-17:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(Services)
#include	INC_KRNCORE(Stream)
#include	INC_3DENGINE(M3D)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

COMMUNICATOR_DESCRIPTOR	LIGHT_TRACK::_CD(QDT_COMMENT("LIGHT_TRACK"), 
										 MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIGHT_TRACK,
										 QDT_COMMENT("28C700B4-2E09-F6B7-5AA9-E4DD541D4CCB"), 
										 sizeof(LIGHT_TRACK), 
										 Init, 
										 QDT_COMMENT("Private class of SeparateLightset"),
										 NULL);

//-----------------------------------------------------------------------------
//	Name:		LIGHT_TRACK constructor
//	Object:		
//	02-10-21:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_TRACK::LIGHT_TRACK()
:
COMMUNICATOR			(&_CD),
_bPreloaded				(false),
_nLightsetId			(-1),
_nFrameCount			(0),
_rDelay					(0),
_rDelayCount			(0),
_nNbrOfUpdatesPerFrame	(0),
_nMeshUpdate			(0),
_nMaterialUpdate		(0),
_rMeshRatio				(0.0f),
_rMaterialRatio			(0.0f),
_bLastLoop				(false)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		LIGHT_TRACK constructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_TRACK::LIGHT_TRACK(const LIGHT_TRACK & C)
:
COMMUNICATOR(&_CD)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		LIGHT_TRACK destructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_TRACK::~LIGHT_TRACK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_TRACK &	LIGHT_TRACK::operator=(const LIGHT_TRACK & C)
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
void	LIGHT_TRACK::Copy(const LIGHT_TRACK & C)
{
	_vMeshLightTracks		= C._vMeshLightTracks;
	_vMaterialLightTracks	= C._vMaterialLightTracks;
	_nLightsetId			= C._nLightsetId;
	_bPreloaded				= C._bPreloaded;
	_nFrameCount			= C._nFrameCount;
	_rDelay					= C._rDelay;
	_rDelayCount			= C._rDelayCount;
	_nNbrOfUpdatesPerFrame	= C._nNbrOfUpdatesPerFrame;
	_nMeshUpdate			= C._nMeshUpdate;
	_nMaterialUpdate		= C._nMaterialUpdate;
	_rMeshRatio				= C._rMeshRatio;
	_rMaterialRatio			= C._rMaterialRatio;
	_bLastLoop				= C._bLastLoop;
}

//-----------------------------------------------------------------------------
//	Name:		Push
//	Object:		
//	02-10-18:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::Push(const MESH_LIGHT_TRACK &	MeshLightTrack)
{
	_vMeshLightTracks.PushTail(MeshLightTrack);
}

//-----------------------------------------------------------------------------
//	Name:		Push
//	Object:		
//	02-10-28:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::Push(const MATERIAL_LIGHT_TRACK &	MaterialLightTrack)
{
	_vMaterialLightTracks.PushTail(MaterialLightTrack);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::Update(float	rDeltaFrameTime,
							bool   &bEnd)
{
	unsigned int i;

	_rDelayCount += rDeltaFrameTime;

	if (_rDelayCount > _rDelay) _rDelayCount = _rDelay;

	if (_rDelayCount == _rDelay && _nFrameCount == 0)
	{
		_bLastLoop = true;	
	}

	if (_nFrameCount == 0)
	{
		_nFrameCount = _nNbrFramesPerUpdate;
		_rMeshRatio = (_rDelayCount - _rPreviousDelayCount) / (_rDelay - _rPreviousDelayCount);
		_rMaterialRatio = (_rDelay - _rDelayCount) / _rDelay;
		_nMeshUpdate = 0;
		_nMaterialUpdate = 0;
		_rPreviousDelayCount = _rDelayCount;
	}

	--_nFrameCount;

	for (i = 0 ; i < _nNbrOfUpdatesPerFrame ; )
	{
		if (_nMeshUpdate < _vMeshLightTracks.GetSize())
		{
			_vMeshLightTracks[_nMeshUpdate].Update(_rMeshRatio);

			i += LIGHTSET_MESH_INFLUENCE;
			++_nMeshUpdate;
		}
		else if (_nMaterialUpdate <  _vMaterialLightTracks.GetSize())
		{
			_vMaterialLightTracks[_nMaterialUpdate].Update(_rMaterialRatio, _bLastLoop);

			i += LIGHTSET_MATERIAL_INFLUENCE;
			++_nMaterialUpdate;
		}
		else
		{
			++_nMaterialUpdate;
			break;
		}
	}

	if (_nMaterialUpdate == _vMaterialLightTracks.GetSize() && _bLastLoop)
	{
		bEnd = true;
	}
	else
	{
		bEnd = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Interpolate
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::Interpolate(float rDelay, int nNbrFramesPerUpdate)
{
	if (_bPreloaded)
	{
		_nFrameCount			= 0;
		_rDelayCount			= 0;
		_rDelay					= rDelay;
		_bLastLoop				= false;
		_rMeshRatio				= 0.0f;
		_rPreviousDelayCount	= 0.0f;
		_nNbrFramesPerUpdate	= (_vMeshLightTracks.GetSize() * LIGHTSET_MESH_INFLUENCE) + 
								  (_vMaterialLightTracks.GetSize() * LIGHTSET_MATERIAL_INFLUENCE);

		QDT_ASSERT(_nNbrFramesPerUpdate > 0);

		_nNbrOfUpdatesPerFrame = LIGHTSET_MATERIAL_INFLUENCE;

		int nNewNbrFramesPerUpdate = _nNbrFramesPerUpdate / _nNbrOfUpdatesPerFrame;

		if (_nNbrFramesPerUpdate % _nNbrOfUpdatesPerFrame) 
		{
			++nNewNbrFramesPerUpdate;
		}

		_nNbrFramesPerUpdate = nNewNbrFramesPerUpdate;
				
		_nMeshUpdate		= 0;
		_nMaterialUpdate	= 0;

		COM_INSTANCE Instance(GetHandle());
		LIGHTSET_MANAGER::Instance()->PushLightTrack(Instance);

		unsigned int nEachMat, nNbrMats = _vMaterialLightTracks.GetSize();
		for(nEachMat = 0 ; nEachMat < nNbrMats ; ++nEachMat)
		{
			_vMaterialLightTracks[nEachMat].UpdateTextureIndex();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::Read(IO_STREAM &	Stream)
{
	MESH_LIGHT_TRACK		MeshTrack;
	MATERIAL_LIGHT_TRACK	MatTrack;
	unsigned int i, nNbrMeshTracks, nNbrMatTracks;

	Stream >> _nLightsetId;

	// Find if we must preload or not
	COM_HANDLE			*pCH = COM_SERVICE::RetrieveComHandle(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SEPARATE_LIGHTSET, _nLightsetId);
	SEPARATE_LIGHTSET	*pLightset = static_cast<SEPARATE_LIGHTSET*>(pCH->GetCommunicator());
	bool				 bMustPreload = pLightset->IsPreloaded();

	if (bMustPreload)
	{
		Stream >> nNbrMeshTracks;
		for (i = 0 ; i < nNbrMeshTracks ; ++i)
		{
			_vMeshLightTracks.PushTail(MeshTrack);
			_vMeshLightTracks[i].Preload(Stream);
		}

		Stream >> nNbrMatTracks;
		for (i = 0 ; i < nNbrMatTracks ; ++i)
		{
			_vMaterialLightTracks.PushTail(MatTrack);
			_vMaterialLightTracks[i].Preload(Stream);
		}
		
	}
	else
	{
		Stream >> nNbrMeshTracks;	
		for (i = 0 ; i < nNbrMeshTracks ; ++i)
		{
			MeshTrack.LoadAndApply(Stream);
		}
		
		Stream >> nNbrMatTracks;	
		for (i = 0 ; i < nNbrMatTracks ; ++i)
		{
			MatTrack.LoadAndApply(Stream);
		}
	}

	_bPreloaded = true;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	LIGHT_TRACK::Write(IO_STREAM &	Stream)
{
	QDT_ASSERT(_nLightsetId > 0);

	Stream << _nLightsetId;

	Stream.DebugOutput("Number of meshes = ");
	Stream << _vMeshLightTracks.GetSize();
	Stream.DebugOutput("\r\n");

	unsigned int i, nNbrMeshTracks = _vMeshLightTracks.GetSize();
	for (i = 0 ; i < nNbrMeshTracks ; ++i)
	{
		_vMeshLightTracks[i].Write(Stream);
	}

	Stream.DebugOutput("Number of materials = ");
	Stream << _vMaterialLightTracks.GetSize();
	Stream.DebugOutput("\r\n");

	unsigned int nNbrMatTracks = _vMaterialLightTracks.GetSize();
	for (i = 0 ; i < nNbrMatTracks ; ++i)
	{
		_vMaterialLightTracks[i].Write(Stream);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::Init(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER_ONLY_DATABANK
	_CD.AddReadChunk("LightTrack", 1, CM_Read);
#endif
#ifndef _MASTER	
	_CD.AddWriteChunk("LightTrack", 1, CM_Write);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	LIGHT_TRACK::GetGenericCommunicatorDescriptor()
{
	return (&_CD);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	LIGHT_TRACK *	pObject;
	pObject = static_cast<LIGHT_TRACK *>(pO);
	QDT_NEW_PLACED(pObject, LIGHT_TRACK)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHT_TRACK::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	LIGHT_TRACK *	pObject;
	pObject = static_cast<LIGHT_TRACK*> (pO);
	pObject->LIGHT_TRACK::~LIGHT_TRACK();
}

//-----------------------------------------------------------------------------
//	Name:		CM_Read
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	LIGHT_TRACK::CM_Read(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	QDT_ASSERT(pCommunicator);

	LIGHT_TRACK *pLT = static_cast<LIGHT_TRACK*>(pCommunicator);
	QDT_ASSERT(pLT);

	pLT->Read(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_Write
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	LIGHT_TRACK::CM_Write(IO_STREAM &		Stream,
							  COMMUNICATOR *	pCommunicator)
{
	QDT_ASSERT(pCommunicator);

	LIGHT_TRACK *pLT = static_cast<LIGHT_TRACK*>(pCommunicator);
	QDT_ASSERT(pLT);

	pLT->Write(Stream);
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
