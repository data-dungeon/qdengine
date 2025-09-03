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
//	CLASS:	SEPARATE_LIGHTSET
//
//	02-10-17:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCOM(Services)
#include	INC_KRNCOM(BaseType)
#include	INC_3DENGINE(M3D)
#include	INC_KRNCORE(Stream)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

COMMUNICATOR_CONTAINER_DESCRIPTOR	SEPARATE_LIGHTSET::_CD(QDT_COMMENT("SEPARATE_LIGHTSET"), 
														   MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SEPARATE_LIGHTSET, 
														   QDT_COMMENT("52823EE2-C15F-D314-8270-B6E981DA57F2"), 
														   sizeof(SEPARATE_LIGHTSET), 
														   Init, 
														   QDT_COMMENT("Lightset which contain a new lighting for the whole scene"),
														   NULL);

//-----------------------------------------------------------------------------
//	Name:		SEPARATE_LIGHTSET constructor
//	Object:		
//	02-10-21:	RMA - Created
//-----------------------------------------------------------------------------
SEPARATE_LIGHTSET::SEPARATE_LIGHTSET()
:
COMMUNICATOR_CONTAINER	(&_CD),
_TrackInst				(LIGHT_TRACK::GetGenericCommunicatorDescriptor()->Create()),
_bPreloaded				(false),
_nTrackID				(-1)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SEPARATE_LIGHTSET constructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
SEPARATE_LIGHTSET::SEPARATE_LIGHTSET(const SEPARATE_LIGHTSET & C)
:
COMMUNICATOR_CONTAINER(&_CD)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		SEPARATE_LIGHTSET destructor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
SEPARATE_LIGHTSET::~SEPARATE_LIGHTSET()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
SEPARATE_LIGHTSET &	SEPARATE_LIGHTSET::operator=(const SEPARATE_LIGHTSET & C)
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
void	SEPARATE_LIGHTSET::Copy(const SEPARATE_LIGHTSET & C)
{
	_nTrackID	= C._nTrackID;
	_TrackInst	= C._TrackInst;
	_bPreloaded = C._bPreloaded;
}

//-----------------------------------------------------------------------------
//	Name:		GetLightTrack
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT_TRACK *	SEPARATE_LIGHTSET::GetLightTrack() const
{
	return (static_cast<LIGHT_TRACK*>(_TrackInst.GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		LoadAndSetTrack
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::LoadAndSetTrack()
{
	int			nTypeID = LIGHT_TRACK::GetGenericCommunicatorDescriptor()->GetOldID();
	COM_HANDLE *pCH		= COM_SERVICE::RetrieveComHandle(nTypeID, _nTrackID);
	
	if (pCH)
	{
		_bPreloaded = false;

		_TrackInst.SetHandle(pCH);
		pCH->GetCommunicator();
		_TrackInst = COM_INSTANCE::GetNullComInstance();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Preload
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::Preload()
{
	int			nTypeID = LIGHT_TRACK::GetGenericCommunicatorDescriptor()->GetOldID();
	COM_HANDLE *pCH		= COM_SERVICE::RetrieveComHandle(nTypeID, _nTrackID);
	
	if (pCH)
	{
		_bPreloaded = true;

		_TrackInst.SetHandle(pCH);
		pCH->GetCommunicator();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Interpolate
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::Interpolate(float rDelay, int nNbrFramesPerUpdate)
{
	if (_bPreloaded)
	{
		LIGHT_TRACK *pLightTrack = GetLightTrack();
		pLightTrack->Interpolate(rDelay * 1000.0f, nNbrFramesPerUpdate);	// Conversion s -> ms
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER_ONLY_DATABANK
	_CD.AddReadChunk("Lightset", 1, CM_Read);
#endif
#ifndef _MASTER	
	_CD.AddWriteChunk("Lightset", 1, CM_Write);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("LoadAndSetTrack", "Load & set track. This option donesn't take extra memory because the track is unloaded immediately after usage.");
	pCD->AddMethod(CMD_LOAD_AND_SET_TRACK, pM, EM_LoadAndSetTrack);

	pM = METHOD_DESCRIPTOR__Allocate("Preload", "Preload a track.");
	pCD->AddMethod(CMD_PRELOAD, pM, EM_Preload);

	pM = METHOD_DESCRIPTOR__Allocate("Interpolate", "Interpolate a track. ");
	METHOD_DESCRIPTOR__AddParameter(pM, "rDelay", "Delay in s", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "nNbrFramesPerUpdate", "Number of frames taken by one update of the lighting", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_INTERPOLATE, pM, EM_Interpolate);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	SEPARATE_LIGHTSET::GetGenericCommunicatorDescriptor()
{
	return (&_CD); 
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::EM_Construct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	SEPARATE_LIGHTSET *	pObject;
	pObject = static_cast<SEPARATE_LIGHTSET *>(pO);
	QDT_NEW_PLACED(pObject, SEPARATE_LIGHTSET)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::EM_Destruct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	SEPARATE_LIGHTSET *	pObject;
	pObject = static_cast<SEPARATE_LIGHTSET*> (pO);
	pObject->SEPARATE_LIGHTSET::~SEPARATE_LIGHTSET();
}

//-----------------------------------------------------------------------------
//	Name:		EM_LoadAndSetTrack
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::EM_LoadAndSetTrack(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	QDT_ASSERT(pO);

	COM_INSTANCE		*pObjet		= static_cast<COM_INSTANCE *>(pO);
	SEPARATE_LIGHTSET	*pLightset  = static_cast<SEPARATE_LIGHTSET*>(pObjet->GetCommunicator());
	QDT_ASSERT(pLightset);

	pLightset->LoadAndSetTrack();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Preload
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::EM_Preload(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	QDT_ASSERT(pO);

	COM_INSTANCE		*pObjet		= static_cast<COM_INSTANCE *>(pO);
	SEPARATE_LIGHTSET	*pLightset  = static_cast<SEPARATE_LIGHTSET*>(pObjet->GetCommunicator());
	QDT_ASSERT(pLightset);

	pLightset->Preload();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Interpolate
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::EM_Interpolate(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	struct PARAMETERS
	{
		float 	_rDelay;
		int		_nNbrFramesPerUpdate;
	};

	QDT_ASSERT(pO);
	QDT_ASSERT(pP);

	COM_INSTANCE		*pObjet		= static_cast<COM_INSTANCE *>(pO);
	SEPARATE_LIGHTSET	*pLightset  = static_cast<SEPARATE_LIGHTSET*>(pObjet->GetCommunicator());
	PARAMETERS			*pParams	= static_cast<PARAMETERS*>(pP);
	QDT_ASSERT(pLightset);
	QDT_ASSERT(pParams);

	pLightset->Interpolate(pParams->_rDelay, pParams->_nNbrFramesPerUpdate);
}

//-----------------------------------------------------------------------------
//	Name:		CM_Read
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	SEPARATE_LIGHTSET::CM_Read(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	SEPARATE_LIGHTSET *pLightset = static_cast<SEPARATE_LIGHTSET*>(pCommunicator);
	QDT_ASSERT(pLightset);

	pLightset->Read(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_Write
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	SEPARATE_LIGHTSET::CM_Write(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	SEPARATE_LIGHTSET *pLightset = static_cast<SEPARATE_LIGHTSET*>(pCommunicator);
	QDT_ASSERT(pLightset);

	pLightset->Write(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SEPARATE_LIGHTSET::Read(IO_STREAM &	Stream)
{
	Stream >> _nTrackID;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-10-17:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	SEPARATE_LIGHTSET::Write(IO_STREAM &	Stream)
{
	Stream.DebugOutput("LIGHTSET : ");
	Stream.DebugOutput(GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Track : ");
	Stream.DebugOutput(DATABASE_SERVICE::GetNameByTypeAndID(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIGHT_TRACK, _nTrackID));
	Stream.DebugOutput(" , Id = ");
	Stream << _nTrackID;
	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
