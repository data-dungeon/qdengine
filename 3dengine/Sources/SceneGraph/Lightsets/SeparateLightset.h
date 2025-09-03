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
//	The SEPARATE_LIGHTSET class implements ...
//
//	02-10-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	SEPARATE_LIGHTSET : public COMMUNICATOR_CONTAINER
{
public:

	// Constructor & destructor
							SEPARATE_LIGHTSET();
							SEPARATE_LIGHTSET(const SEPARATE_LIGHTSET & C);
						   ~SEPARATE_LIGHTSET();
	
	// Operators
	SEPARATE_LIGHTSET &		operator = (const SEPARATE_LIGHTSET & C);

	// Accessors
	int						GetTrackID() const		{ return (_nTrackID); }
	bool					IsPreloaded() const		{ return (_bPreloaded); }
	
	// Modifiors
	void					SetTrackID(int nID)		{ _nTrackID = nID; }
	
	// Load a track & immediately set it
	void					LoadAndSetTrack();
	
	// Preload a track
	void					Preload();
	
	// Interpolate
	void					Interpolate(float rDelay, int nNbrFramesPerUpdate);

	// Read & Write
	void					Read(IO_STREAM &Stream);
#ifndef _MASTER
	void					Write(IO_STREAM &Stream);
#endif

	// Communication
	enum
	{
		CMD_LOAD_AND_SET_TRACK = CMD_COM_USER_START,
		CMD_PRELOAD,
		CMD_INTERPOLATE,
	};

	static void									Init(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_CONTAINER_DESCRIPTOR *	GetGenericCommunicatorDescriptor();

private:

	static	void	EM_Construct(void * const pO, void * const pR, void * const pP);
	static	void	EM_Destruct(void * const pO, void * const pR, void * const pP);
	static	void	EM_LoadAndSetTrack(void * const pO, void * const pR, void * const pP);
	static	void	EM_Preload(void * const pO, void * const pR, void * const pP);
	static	void	EM_Interpolate(void * const pO, void * const pR, void * const pP);

#ifndef _MASTER_ONLY_DATABANK
	static	void	CM_Read(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static	void	CM_Write(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif

	void			Copy(const SEPARATE_LIGHTSET & C);

	LIGHT_TRACK*	GetLightTrack() const;

	static COMMUNICATOR_CONTAINER_DESCRIPTOR	_CD;

	int				_nTrackID;
	COM_INSTANCE	_TrackInst;
	bool			_bPreloaded;

};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
