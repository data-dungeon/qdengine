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
//	The LIGHT_TRACK class implements ...
//
//	02-10-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	LIGHT_TRACK : public COMMUNICATOR
{
public:

	// Constructor & destructor
							LIGHT_TRACK();
							LIGHT_TRACK(const LIGHT_TRACK & C);
						   ~LIGHT_TRACK();

	// Operators
	LIGHT_TRACK &			operator=(const LIGHT_TRACK & C);

	// Interpolate
	void					Interpolate(float rDelay, int nNbrFramesPerUpdate);

	// Update interpolation
	void					Update(float rDeltaFrameTime, bool &bEnd);

	// Modifiors
	void					SetLightsetId(int nId)		{ _nLightsetId = nId; }

	// Push objects
	void					Push(const MESH_LIGHT_TRACK & MeshLightTrack);
	void					Push(const MATERIAL_LIGHT_TRACK & MaterialLightTrack);

	// Read & Write
	void					Read(IO_STREAM &Stream);
#ifndef _MASTER
	void					Write(IO_STREAM &Stream);
#endif

	// Communication
	static void							Init(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	
private:

	void					Copy(const LIGHT_TRACK & C);

	static	void			EM_Construct(void * const pO, void * const pR, void * const pP);
	static	void			EM_Destruct(void * const pO, void * const pR, void * const pP);

#ifndef _MASTER_ONLY_DATABANK
	static	void			CM_Read(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static	void			CM_Write(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif

	static COMMUNICATOR_DESCRIPTOR		_CD;

	QDT_VECTOR<MESH_LIGHT_TRACK>		_vMeshLightTracks;
	QDT_VECTOR<MATERIAL_LIGHT_TRACK>	_vMaterialLightTracks;
	int									_nLightsetId;
	bool								_bPreloaded;

	int									_nNbrFramesPerUpdate;
	float								_rDelay;
	float								_rDelayCount;
	unsigned int						_nFrameCount;
	unsigned int						_nNbrOfUpdatesPerFrame;
	unsigned int						_nMeshUpdate;
	unsigned int						_nMaterialUpdate;
	float								_rMeshRatio;
	float								_rMaterialRatio;
	bool								_bLastLoop;
	float								_rPreviousDelayCount;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
