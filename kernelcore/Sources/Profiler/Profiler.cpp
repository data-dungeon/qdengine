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
//	CLASS:	PROFILER
//
//	03-04-07:	VMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Profiler/Profiler)
#include	INCL_KCORE(Time/Time_ABC)

#define NB_FRAME_PROFILE	10
#define FRAME_TIME			( 1000.0f / 25.0f )

//#define PROFILE_MAX			32
#define	MAX_NAME_LEN		24

#define GET_TIME()	(float) TIME::Instance()->GetSystemTime()

float					PROFILER::_TimeStack[TIMESTACK_SIZE];
int						PROFILER::_TimeStackPos = -1;

float					PROFILER::_rLastTime;
float					PROFILER::_rStartTime;
int						PROFILER::_nNbFrames;
bool					PROFILER::_bEnabled;

//#define	_DEBUG
//#undef QDT_ASSERT
//#define QDT_ASSERT QDT_ENSURE

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PROFILER constructor
//	Object:	get	
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
PROFILER::PROFILER()
{
	Init();
}

//-----------------------------------------------------------------------------
//	Name:		~PROFILER destructor
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
PROFILER::~PROFILER()
{
	Close();
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::Init()
{
	SetMaxProfile(PROFILE_MAX);

	AddProfile(QDT_COMMENT("Moteur3D::Loop"), PROFILE_MOTEUR3D_LOOP);
	{
		AddProfile(QDT_COMMENT("Moteur3D::UpdateAnimation"), PROFILE_UPDATE_BEHAVIOR, PROFILE_MOTEUR3D_LOOP);
	}
	
	AddProfile(QDT_COMMENT("Moteur3D::Display"), PROFILE_MOTEUR3D_DISPLAY);
	{
		AddProfile(QDT_COMMENT("Prepare Viewport"),	PROFILE_PREPARE_VIEWPORT, PROFILE_MOTEUR3D_DISPLAY);
		AddProfile(QDT_COMMENT("SceneGraph::Display"),	PROFILE_SCENEGRAPH_DISPLAY, PROFILE_MOTEUR3D_DISPLAY);
		{
			AddProfile(QDT_COMMENT("Draw Primitive"), PROFILE_DRAW_PRIMITIVE, PROFILE_SCENEGRAPH_DISPLAY);
		}
		AddProfile(QDT_COMMENT("Photographic Pipeline"), PROFILE_PHOTO_PIPELINE, PROFILE_MOTEUR3D_DISPLAY);
		AddProfile(QDT_COMMENT("VBLANK"), PROFILE_VBLANK, PROFILE_MOTEUR3D_DISPLAY);
	}

	AddProfile(QDT_COMMENT("Node Array World Matrices Update"), PROFILE_NODE_ARRAY_UPDATE_WORLD);

	AddProfile(QDT_COMMENT("New Animation System"), PROFILE_NAS);

	AddProfile(QDT_COMMENT("New Animation System --> 00"), PROFILE_NAS_00);
	AddProfile(QDT_COMMENT("New Animation System --> 01"), PROFILE_NAS_01);
	AddProfile(QDT_COMMENT("New Animation System --> 02"), PROFILE_NAS_02);
	AddProfile(QDT_COMMENT("New Animation System --> 03"), PROFILE_NAS_03);
	AddProfile(QDT_COMMENT("New Animation System --> 04"), PROFILE_NAS_04);
	AddProfile(QDT_COMMENT("New Animation System --> 05"), PROFILE_NAS_05);
	AddProfile(QDT_COMMENT("New Animation System --> 06"), PROFILE_NAS_06);
	
		
	OpenAllProfiles();

	_nNbFrames = 0;
	_rStartTime = 0.f;
	_bEnabled = true;
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::Close()
{
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::Loop()
{
	if (!_bEnabled) return;

#ifdef _DEBUG
	if ( _ProfileIdStack.GetSize() != 0 )
	{
		QDT_Error( "Profile %d is not valid", _ProfileIdStack[0] );
	}
#endif

	float rFrameTime = GET_TIME();

	// Update Time of Profiler
	int nSize = _ProfileList.GetSize();
	int nProfileIndex;

	for ( nProfileIndex=0; nProfileIndex < nSize ; nProfileIndex++)
	{
		// Update of Tuning value
		_ProfileList[ nProfileIndex ]._rTime += _ProfileList[ nProfileIndex ]._rCurTime;

		if ( !_nNbFrames )
		{
			_ProfileList[ nProfileIndex ]._rFinalTune	 = _ProfileList[ nProfileIndex ]._rTime * 100.f / ( NB_FRAME_PROFILE * rFrameTime );
			_ProfileList[ nProfileIndex ]._rFinalTime	 = _ProfileList[ nProfileIndex ]._rTime / NB_FRAME_PROFILE;
			_ProfileList[ nProfileIndex ]._nFinalNbCalls = _ProfileList[ nProfileIndex ]._nNbCalls;
			_ProfileList[ nProfileIndex ]._rTime = 0;

			if ( _ProfileList[ nProfileIndex ]._rFinalTime != 0.f)
			{
				// Set Min/Max
				if (_ProfileList[ nProfileIndex ]._rFinalTime < _ProfileList[ nProfileIndex ]._rMinTime)
					_ProfileList[ nProfileIndex ]._rMinTime   = _ProfileList[ nProfileIndex ]._rFinalTime;
				if (_ProfileList[ nProfileIndex ]._rFinalTime > _ProfileList[ nProfileIndex ]._rMaxTime)
					_ProfileList[ nProfileIndex ]._rMaxTime   = _ProfileList[ nProfileIndex ]._rFinalTime;

				if (_ProfileList[ nProfileIndex ]._rFinalTune < _ProfileList[ nProfileIndex ]._rMinTune)
					_ProfileList[ nProfileIndex ]._rMinTune   = _ProfileList[ nProfileIndex ]._rFinalTune;
				if (_ProfileList[ nProfileIndex ]._rFinalTune > _ProfileList[ nProfileIndex ]._rMaxTune)
					_ProfileList[ nProfileIndex ]._rMaxTune   = _ProfileList[ nProfileIndex ]._rFinalTune;
			}
		}
	}

	
	InitProfile();
	
	_nNbFrames++;
	if ( _nNbFrames > NB_FRAME_PROFILE )
	{
		_nNbFrames = 0;
	}
}


//-----------------------------------------------------------------------------
//	Name:		AddProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
// Add a Profile to ProfileList
int PROFILER::AddProfile(const QDT_STRING & sName,int nId,int nParentId)
{
	Profile Prof(sName);
	_ProfileList.PushTail(Prof);
	_ProfileID[ nId ]= _ProfileList.GetIndex( _ProfileList.GetLast() );

	if ( nParentId < 0 )
	{
		_RootProfile.PushTail( _ProfileID[ nId ] );
	}
	else
	{
		_ProfileList[ _ProfileID[ nParentId ] ]._SubProfile.PushTail( _ProfileID[ nId ] );
		_ProfileList[ _ProfileID[ nId ] ]._Flag |= FL_SUB;
	}

	return _ProfileID[ nId ];
}

//-----------------------------------------------------------------------------
//	Name:		OpenProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
// Opens a Profile Folder
void PROFILER::OpenProfile(const QDT_STRING & sPrfName )
{
	int nSize = _ProfileList.GetSize();

	for (int i = 0; i < nSize ; i++)
	{
		if ( _ProfileList[i]._sName.CompareNoCase( sPrfName ) )
		{
			_ProfileList[i]._Flag |= FL_OPEN;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CloseProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
// Close a Profile Folder
void PROFILER::CloseProfile(const QDT_STRING & sPrfName)
{
	int nSize = _ProfileList.GetSize();

	for (int i=0; i < nSize; i++)
	{
		if ( _ProfileList[i]._sName.CompareNoCase( sPrfName ) )
		{
			_ProfileList[i]._Flag &=~ FL_OPEN;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OpenAllProfiles
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
// Open All Profile Folders
void PROFILER::OpenAllProfiles()
{
	int nSize = _ProfileList.GetSize();

	for (int i=0; i< nSize ; i++)
	{
		_ProfileList[i]._Flag |= FL_OPEN;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CloseAllProfiles
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
// Close All Profile Folders
void PROFILER::CloseAllProfiles()
{
	int nSize = _ProfileList.GetSize();

	for (int i=0; i< nSize ; i++)
	{
		_ProfileList[i]._Flag &=~ FL_OPEN;
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::InitProfile( int nProfileId )
{
	if ( nProfileId == -1 )
	{
		int nSize = _ProfileList.GetSize();

		for (int nProfileIndex=0; nProfileIndex < nSize; nProfileIndex++)
		{
			_ProfileList[ nProfileIndex ]._rCurTime = 0;
			_ProfileList[ nProfileIndex ]._nNbCalls = 0;
		}
	}
	else
	{
		QDT_ASSERT( _ProfileID[ nProfileId ] != -1 );

		_ProfileList[ _ProfileID[ nProfileId ] ]._rCurTime = 0;
		_ProfileList[ _ProfileID[ nProfileId ] ]._nNbCalls = 0;
	}

	_rLastTime = _rStartTime;

	_rStartTime = GET_TIME();

//	QDT_ASSERT( _TimeStack.IsEmpty() );
}

//-----------------------------------------------------------------------------
//	Name:		BeginProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::BeginProfile(int nProfileId )
{
	QDT_ASSERT( _ProfileID[ nProfileId ] !=-1 );
//	if (_ProfileID[ nProfileId ] ==-1) return;

	_TimeStackPos++;
	QDT_ASSERT( _TimeStackPos < TIMESTACK_SIZE );
	float & rStartTime = _TimeStack[_TimeStackPos];

	rStartTime = GET_TIME();
	
#ifdef _DEBUG
	_ProfileIdStack.PushTail( nProfileId );
#endif
	
}

//-----------------------------------------------------------------------------
//	Name:		EndProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::EndProfile( int nProfileId )
{
	QDT_ASSERT( _ProfileID[ nProfileId ] !=-1 );
//	if (_ProfileID[ nProfileId ] ==-1) return;
	
#ifdef _DEBUG
	int nProfId;
	nProfId = *_ProfileIdStack.GetLast();
	_ProfileIdStack.PopTail();
	QDT_ASSERT( nProfId == nProfileId);
#endif

	float rStartTime, rCurTime = GET_TIME();
	
	rStartTime =  _TimeStack[_TimeStackPos];
	_TimeStackPos--;
	QDT_ASSERT(_TimeStackPos >= -1);

	float 	rTime = rCurTime - rStartTime;	

	int nId = _ProfileID[ nProfileId ];

	_ProfileList[ nId ]._rCurTime	+= rTime;	
	_ProfileList[ nId ]._nNbCalls ++;
}


//-----------------------------------------------------------------------------
//	Name:		GetFolderTime
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
// Get Folder Time
void PROFILER::GetFolderTime( int nId, float &Tune, float &Time )
{
	int nSize = _ProfileList[ nId ]._SubProfile.GetSize();

	for (int i=0; i < nSize; i++)
	{
		GetFolderTime( _ProfileList[ nId ]._SubProfile[i], Tune, Time );
	}

	Tune	+= _ProfileList[ nId ]._rFinalTune;
	Time	+= _ProfileList[ nId ]._rFinalTime;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::SetMaxProfile(int nMaxProfile)
{
	int nStart = _ProfileID.GetSize();
	_ProfileID.Reserve( nMaxProfile );
	_ProfileList.Reserve( nMaxProfile );

	for (int i = nStart; i < nMaxProfile; i++)
	{
		_ProfileID.PushTail( -1 );
	}

	for (int j = 0; j < TIMESTACK_SIZE; j++)
	{
		_TimeStack[j] = 0.f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFolderTime
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
void PROFILER::PrintProfile(const QDT_STRING & sPrfName)
{
#ifndef _MASTER
	int nSize = _ProfileList.GetSize();

	for (int i=0; i<nSize; i++)
	{
		if ( _ProfileList[i]._sName.CompareNoCase( sPrfName ) )
		{
			QDT_Message("=============================================");
			QDT_Message("Profile *** %s ***",sPrfName.GetBuffer() );
			QDT_Message("Min = %f, Max = %f, Average = %f",	_ProfileList[i]._rMinTime,
															_ProfileList[i]._rMaxTime,
															_ProfileList[i]._rFinalTime);
			QDT_Message("Percent : Min = %f%, Max = %f%, Average = %f%",_ProfileList[i]._rMinTune,
															_ProfileList[i]._rMaxTune,
															_ProfileList[i]._rFinalTune);
			QDT_Message("=============================================");
			break;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetProfile
//	Object:		
//	03-04-07:	VMA - Created
//-----------------------------------------------------------------------------
const Profile&	PROFILER::GetProfile( int nId)
{
	return _ProfileList[ nId ];
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
