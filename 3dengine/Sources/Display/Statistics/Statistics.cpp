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
//	CLASS:	STATISTICS
//
//	01-10-25:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/Statistics/Statistics)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementText)
#include	INCL_KCOM(Services/ComService)
#include	INCL_KCORE(Time/Time_ABC)

#ifdef _DEBUG
	#include	"Statistics.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STATISTICS constructor
//	Object:		
//	01-10-25:	RMA - Created
//-----------------------------------------------------------------------------
STATISTICS::STATISTICS()
:
_FullStatsText					(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_ShortStatsText					(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_MemStatsText					(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_DataBankStatsText				(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_VMStatsText					(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_GraphicOptionsText				(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_SoundStatsText					(IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()->Create()),
_bDisplayShortStats				(false),
_bDisplayGraphicStats			(false),
_bDisplayMemStats				(false),
_bDisplayDataBankStats			(false),
_bChangeStats					(false),
_bDisplayVMStats				(false),
_bDisplayGraphicOptions			(false),
_bDisplaySoundStats				(false),
_nNbrFrustrumIntersections		(0),
_nNbrHexahedronIntersections	(0),
_nPeriod						(800),
_bFontIsInit					(false)
{
	_nNextRefresh = (unsigned int)(TIME::Instance()->GetSystemTime()) + _nPeriod;

	InitText( _MemStatsText		, 0.5f, 100.0f, 0.0f, 100.0f);
	InitText( _ShortStatsText	, 0.5f, 100.0f, 0.0f, 100.0f);
	InitText( _FullStatsText	, 0.5f, 100.0f, 0.0f, 100.0f);	
	InitText( _DataBankStatsText, 0.5f, 100.0f, 0.0f, 100.0f);	
	InitText( _VMStatsText		, 0.5f, 100.0f, 0.0f, 100.0f);	
	InitText( _GraphicOptionsText, 0.5f, 100.0f, 0.0f, 100.0f);
	InitText( _SoundStatsText	, 0.5f, 100.0f, 0.0f, 100.0f);
}

//-----------------------------------------------------------------------------
//	Name:		InitFonts
//	Object:		
//	03-03-20:	VPI - Created
//-----------------------------------------------------------------------------
void	STATISTICS::InitFonts()
{
	// Find FONT_ARIAL on database
	COM_INSTANCE ArialFontInst = COM_SERVICE::Instance()->RetrieveComHandle(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DISPLAY_FONT, 1);
	QDT_ASSERT(ArialFontInst.HasHandle());
	
	IHM_ELEMENT_TEXT * pShortStatsText		= (IHM_ELEMENT_TEXT *) _ShortStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pMemStatsText		= (IHM_ELEMENT_TEXT *) _MemStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pFullStatsText		= (IHM_ELEMENT_TEXT *) _FullStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pDataBankStatsText	= (IHM_ELEMENT_TEXT *) _DataBankStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pVMStatsText			= (IHM_ELEMENT_TEXT *) _VMStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pGraphicOptionsText	= (IHM_ELEMENT_TEXT *) _GraphicOptionsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pSoundStatsText		= (IHM_ELEMENT_TEXT *) _SoundStatsText.GetCommunicator();
	
	pShortStatsText->SetFontCI(ArialFontInst);
	pMemStatsText->SetFontCI(ArialFontInst);
	pFullStatsText->SetFontCI(ArialFontInst);
	pDataBankStatsText->SetFontCI(ArialFontInst);
	pVMStatsText->SetFontCI(ArialFontInst);
	pGraphicOptionsText->SetFontCI(ArialFontInst);
	pSoundStatsText->SetFontCI(ArialFontInst);
}

//-----------------------------------------------------------------------------
//	Name:		STATISTICS destructor
//	Object:		
//	01-10-25:	RMA - Created
//-----------------------------------------------------------------------------
STATISTICS::~STATISTICS()
{
	// do nothing
}

//-----------------------------------------------------------------------------
//	Name:		UpdateStatisticsBeforeDisplay
//	Object:		
//	01-10-25:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::UpdateStatisticsBeforeDisplay()
{
//	RENDER_MANAGER::Instance()->GetRenderContext()->GetRenderPipeline()->GetStats().UpdateStatisticsBeforeDisplay();

	_nNbrFrustrumIntersections		= 0;
	_nNbrHexahedronIntersections	= 0;
}

extern int g_nTotalSTLAllocatedSize;

//-----------------------------------------------------------------------------
//	Name:		UpdateStatisticsAfterDisplay
//	Object:		
//	01-10-25:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::UpdateStatisticsAfterDisplay()
{
	QDT_NOT_IMPLEMENTED();
/*
	QDT_STRING		String;
	RP_STATISTICS	&Stats = RENDER_MANAGER::Instance()->GetRenderContext()->GetRenderPipeline()->GetStats();

	IHM_ELEMENT_TEXT * pShortStatsText		= (IHM_ELEMENT_TEXT *) _ShortStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pMemStatsText		= (IHM_ELEMENT_TEXT *) _MemStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pFullStatsText		= (IHM_ELEMENT_TEXT *) _FullStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pDataBankStatsText	= (IHM_ELEMENT_TEXT *) _DataBankStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pVMStatsText			= (IHM_ELEMENT_TEXT *) _VMStatsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pGraphicOptionsText	= (IHM_ELEMENT_TEXT *) _GraphicOptionsText.GetCommunicator();
	IHM_ELEMENT_TEXT * pSoundStatsText		= (IHM_ELEMENT_TEXT *) _SoundStatsText.GetCommunicator();

	unsigned int time = TIME::Instance()->GetSystemTime();
	if ((time > _nNextRefresh)
		|| _bChangeStats)
	{
		_nNextRefresh = time + _nPeriod;
		_bChangeStats = false;

#ifdef PLATFORM_XBOX
		String += "{S:100}Padding{P}";
#endif

		if (_bDisplayShortStats)
		{
			// Base infos
			String.Format("{S:100}FPS: %03d / Poly: %3dK", 
						  static_cast<int>(TIME::Instance()->GetFPS()),
						  Stats.GetNbrDrawnPolygones() / 1000);
			pShortStatsText->SetText(String);
			pShortStatsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
		}

		if (_bDisplayMemStats)
		{
			MEM_STATS	MemStatStruct;

			MEM::GetMemStats(MemStatStruct);
			
			VM_SERVICE::VM_STATS Stats;
			VM_SERVICE::GetStats(Stats);

			QDT_STRING strTmp;

			strTmp.Format("{S:100}Sum Allocated: %iKb{P}", MemStatStruct._nSizeAllocatedMem / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Tot Headers: %iKb{P}", MemStatStruct._nTotalAllocatedMemHeader / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Sum Free: %iKb{P}", MemStatStruct._nSizeMaxMem / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Max allocable block: %iKb{P}", MemStatStruct._nSizeMaxBlock / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Blocks (Free/Allocated): %i{P}", MemStatStruct._nNbrBlocks);
			String += strTmp;
			strTmp.Format("{S:100}Average Max used: %iKb{P}", MemStatStruct._nMaxMemoryUsed / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Average current used: %iKb{P}", MemStatStruct._nCurrentMemoryUsed / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Tot Allocator + Buffer: %iKb{P}", MemStatStruct._nSizeMemAllocator / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Wasted: %iKb{P}", MemStatStruct._nWastedSize / 1024);
			String += strTmp;
			strTmp.Format("{S:100}---{P}");
			String += strTmp;
			strTmp.Format("{S:100}DataBanks: %iKb (%d DBs){P}", DATA_BANK::GetTotalLoadedSize() / 1024, DATA_BANK::GetNbLoadedDataBanks() );
			String += strTmp;
			strTmp.Format("{S:100}ComHandles: %iKb{P}", COM_HANDLE::GetTotalAllocatedSize() / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Streaming: %iKb{P}", COMMUNICATOR_PARTITION::GetTotalAllocatedSize() / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}STL: %iKb{P}", g_nTotalSTLAllocatedSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}ClassDescriptors: %iKb{P}", CLASS_DESCRIPTOR::GetMemoryUsed() / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}VM Stack: Def:%i Max:%i NbTH: %i{P}", Stats._nDefaultStackSize, 
																		Stats._nMaxStackUsed, 
																		Stats._nNbrInterpreters);
			String += strTmp;
			strTmp.Format("{S:100}Tot MemPools(A/F): %iKb/%iKb{P}", MEM_POOL_HEADER::GetMemoryUsed() / 1024, MEM_POOL_HEADER::GetMemoryFree() / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}STRING MemPools(A/F): %iKb/%iKb{P}", QDT_STRING::GetMemoryUsed() / 1024 , QDT_STRING::GetFreeMem() / 1024 );
			String += strTmp;

			pMemStatsText->SetText(String);
			pMemStatsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
//#ifdef _DEBUG			
//			QDT_Message(String);
//#endif
		}				

		if (_bDisplayGraphicStats)
		{
			QDT_STRING strTmp;

			// FPS
			strTmp.Format("{S:100}Frame Rate = %d{P}", static_cast<int>(TIME::Instance()->GetFPS()));
			String += strTmp;

			// Polygones
			strTmp.Format("{S:100}Drawn Polygones = %d{P}", Stats.GetNbrDrawnPolygones());
			String += strTmp;

#ifndef PLATFORM_PS2
			// number of primitives send to the card
			strTmp.Format("{S:100}Drawn Primitives = %d{P}", Stats.GetNbrDrawnPrimitives());
			String += strTmp;
#endif

			// number of set materials
			strTmp.Format("{S:100}Number of SetShader() = %d{P}", Stats.GetNbrSetMaterials());
			String += strTmp;

#ifndef PLATFORM_PS2
			// Number of skinned points
			strTmp.Format("{S:100}Skinned Points = %d(%d){P}", Stats.GetNbrSkinnedPoints(), Stats.GetNbrSkeletons());
			String += strTmp;
#endif
		
			// Number of particles
			strTmp.Format("{S:100}Particles = %d{P}", Stats.GetNbrParticles());
			String += strTmp;

#ifndef PLATFORM_PS2
			// Number of frustrum intersection
			strTmp.Format("{S:100}Frustum intersections = %d{P}", _nNbrFrustrumIntersections);
			String += strTmp;

			// Number of hexahedron intersection
			strTmp.Format("{S:100}Hexahedron intersections = %d{P}", _nNbrHexahedronIntersections);
			String += strTmp;
#endif

			// IsCollisions Enable
			if (COLLISION_MANAGER::Instance()->IsCollisionEnable())
			{
				strTmp = "Collision ON";
			}
			else
			{
				strTmp = "Collision OFF";
			}
			String += strTmp;

			pFullStatsText->SetText(String);
			pFullStatsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
		}	
		
		if (_bDisplayDataBankStats)
		{
			QDT_STRING strTmp;
#ifdef PLATFORM_PC
			strTmp.Format("{S:100}%d DataBanks loaded (%d Kb):{P}", DATA_BANK::GetNbLoadedDataBanks(), DATA_BANK::GetTotalLoadedSize() / 1024 );
#else
			strTmp.Format("{S:100}%d DataBanks loaded (%d Kb):{P}", DATA_BANK::GetNbLoadedDataBanks(), DATA_BANK::GetTotalLoadedSize() / 1024 );
#endif
			String += strTmp;

			int nSize = DATA_BANK::GetNbLoadedDataBanks();
				
			for( int i = 0 ; i < nSize ; i++ )
			{
#ifdef PLATFORM_PC
				strTmp.Format("{S:100}%s: %d Kb{P}", DATA_BANK::GetLoadedDataBankName(i).GetBuffer(), DATA_BANK::GetLoadedDataBankSize(i) / 1024 );
#else
#ifdef _DEBUG
				strTmp.Format("{S:75}%s: %d Kb{P}", DATA_BANK::GetLoadedDataBankName(i).GetBuffer(), DATA_BANK::GetLoadedDataBankSize(i) / 1024 );
#else
				strTmp.Format(" {S:100} %d: %d Kb{P}", DATA_BANK::GetLoadedDataBankComID(i).GetID(), DATA_BANK::GetLoadedDataBankSize(i) / 1024 );
#endif
#endif
				String += strTmp;
			}

			pDataBankStatsText->SetText(String);
			pDataBankStatsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
		}

		if (_bDisplayVMStats)
		{
			VM_SERVICE::VM_STATS Stats;
			VM_SERVICE::Instance()->GetStats(Stats);
			
			QDT_STRING strTmp;
			strTmp.Format("{S:100}Nbr ClassVM/Loaded: %i / %i{P}", Stats._nTotalNbrCVM, Stats._nTotalNbrCVMLoaded );
			String += strTmp;
			strTmp.Format("{S:100}Size ClassVM: %iKb{P}", Stats._nTotalNbrCVMSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Nbr MethodVM: %i{P}", Stats._nTotalNbrMVM );
			String += strTmp;
			strTmp.Format("{S:100}Size MethodVM: %iKb{P}", Stats._nTotalNbrMVMSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Nbr BC struct: %i{P}", Stats._nTotalNbrBCStruct );
			String += strTmp;
			strTmp.Format("{S:100}Size BC struct: %iKb{P}", Stats._nTotalSizeBCStruct / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}ByteCode (instructions) : %iKb{P}", Stats._nTotalBCSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Static Names : %iKb{P}", Stats._nTotalStaticNamesSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Map Names : %iKb{P}", Stats._nTotalMapNamesSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Statics: %iKb{P}", Stats._nTotalAllocatedStaticsSize / 1024 );
			String += strTmp;
			strTmp.Format("{S:100}Nbr Threads: %i{P}", Stats._nTotalNbrThreads );
			String += strTmp;
			strTmp.Format("{S:100}Threads stack size: %iKb{P}", Stats._nTotalThreadsStack / 1024);
			String += strTmp;
			strTmp.Format("{S:100}Classes with statics: %i{P}", Stats._nTotalClassWithStatics);
			String += strTmp;
			strTmp.Format("{S:100}DYN Allocator: %i{P}", Stats._nTotalDynAllocator);
			String += strTmp;

			pVMStatsText->SetText(String);
			pVMStatsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
		}
#ifdef PLATFORM_PC
		if (_bDisplayGraphicOptions)
		{

			//Anti Aliasing Mode
			RENDER_CONTEXT	*pContext	= RENDER_MANAGER::Instance()->GetRenderContext();
			ANTI_ALIASING	eAAMode		= pContext->GetAntiAliasingMode();
			QDT_STRING		sAAMode;
			switch(eAAMode)
			{
			default:
			case	AA_NONE:
				sAAMode = "None";
				break;

			case	AA_FS2X:
				sAAMode = "2X";
				break;

			case	AA_FS4X:
				sAAMode = "4X";
				break;

			case	AA_FS6X:
				sAAMode = "6X";
				break;

			case	AA_FS8X:
				sAAMode = "8X";
				break;

			}

			//Resolution
			int nWidth, nHeight, nDepth;
			DISPLAY_MANAGER::Instance()->GetCurrentDisplayModeInfos(nWidth, nHeight, nDepth);
			switch(nDepth)
			{
			default:
			case	3:
				nDepth = 16;
				break;

			case	4:
				nDepth = 32;
				break;
			}

			//Shadows
			QDT_STRING sShadows;
			if(DISPLAY_MANAGER::Instance()->AreShadowsEnabled())
			{
				sShadows = "Enabled";
			}
			else
			{
				sShadows = "Disabled";
			}

			//Filter Type
			TEXTURE_FILTERTYPE nFilterType = DISPLAY_MANAGER::Instance()->GetFilterType();
			QDT_STRING sFilterType;
			switch(nFilterType)
			{
			default:
			case	TFT_NONE:
				sFilterType = "None";
				break;

			case	TFT_BILINEAR:
				sFilterType = "Bilinear";
				break;

			case	TFT_TRILINEAR:
				sFilterType = "Trilinear";
				break;

			case	TFT_ANISOTROPIC:
				sFilterType = "Anisotropic";
				break;	
			}

			//Env Map Type
			ENVMAP_TYPE nEnvMapType = DISPLAY_MANAGER::Instance()->GetEnvMapType();
			QDT_STRING sEnvMapType;
			switch(nEnvMapType)
			{
			default:
			case	EMT_NONE:
				sEnvMapType = "None";
				break;

			case	EMT_SIMPLE_TEX:
				sEnvMapType = "Medium";
				break;

			case	EMT_CUBE_TEX:
				sEnvMapType = "High";
				break;
			}

			//Detail Map Type
			QDT_STRING sDetailMapType;
			if(DISPLAY_MANAGER::Instance()->IsDetailMapEnabled())
			{
				sDetailMapType = "Enabled";
			}
			else
			{
				sDetailMapType = "Disabled";
			}

			//Front Buffer Effects
			QDT_STRING sFBEffects;
			if(DISPLAY_MANAGER::Instance()->IsFrontBufferEffectsEnabled())
			{
				sFBEffects = "Enabled";
			}
			else
			{
				sFBEffects = "Disabled";
			}

			//Env Bump
			QDT_STRING sEnvBumpEnabled;
			if(DISPLAY_MANAGER::Instance()->IsBumpEnvMapEnabled())
			{
				sEnvBumpEnabled = "Enabled";
			}
			else
			{
				sEnvBumpEnabled = "Disabled";
			}

			//Texture Quality Mode
			TEXTURE_QUALITY nTexQuality = DISPLAY_MANAGER::Instance()->GetTextureQuality();
			QDT_STRING sTexQuality;
			switch(nTexQuality)
			{
			default:
			case	TQ_HIGH:
				sTexQuality = "High";
				break;

			case	TQ_LOW:
				sTexQuality = "Low";
				break;
			}

			//Texture Quality Mode
			DYN_LIGHT_QUALITY nDLQuality = DISPLAY_MANAGER::Instance()->GetDynLightingQuality();
			QDT_STRING sDLQuality;
			switch(nDLQuality)
			{
			default:
			case	DLQ_HIGH:
				sDLQuality = "High";
				break;

			case	DLQ_LOW:
				sDLQuality = "Low";
				break;
			}

			// Vertical Synch
			QDT_STRING sVSynchEnabled;
			if(pContext->GetVerticalSynch())
			{
				sVSynchEnabled = "Enabled";
			}
			else
			{
				sVSynchEnabled = "Disabled";
			}

			// Gamma Control
			float rGammaControl = pContext->GetGammaModifior();

			// Base infos
			QDT_STRING strTmp;
			strTmp.Format("{S:100}Resolution: %d * %d * %d{P}", nWidth, nHeight, nDepth);
			String += strTmp;
			strTmp.Format("{S:100}Anti Aliasing Mode: %s{P}", sAAMode.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Front Buffer Effects: %s{P}", sFBEffects.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Texture Filter: %s{P}", sFilterType.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Dynamic Shadows: %s{P}", sShadows.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Environnement Map: %s{P}", sEnvMapType.GetBuffer());
			String += strTmp;	
			strTmp.Format("{S:100}Detail Map: %s{P}", sDetailMapType.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Environemental Bump-Map: %s{P}", sEnvBumpEnabled.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Texture Quality: %s{P}", sTexQuality.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Dynamic Lighting Quality: %s{P}", sTexQuality.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Vertical Synchronization: %s{P}", sVSynchEnabled.GetBuffer());
			String += strTmp;
			strTmp.Format("{S:100}Gamma Factor: %.1f{P}", rGammaControl);
			String += strTmp;
			pGraphicOptionsText->SetText(String);
			pGraphicOptionsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
		}
#endif
		if (_bDisplaySoundStats)
		{
			int			nType;
			int			nSize;
			bool		bStereo;
			int			nSamplesPerSec;
			int			nBitsPerSample;
			QDT_STRING	sStrereo;
			int			nIndex;
			int			nNbrSound;
			int			nNbrStreamedSound, nNbrFlow;
			QDT_STRING	strTmp;

			SOUND_MANAGER_ABC * pSoundManager = SOUND::Instance()->GetSoundManager();

			nNbrSound = pSoundManager->GetNbrSoundBuffer();

			nNbrFlow = 0;
			for ( nIndex = 0 ; nIndex < nNbrSound ; ++nIndex )
			{
				if ( ((SOUND_DATA *)pSoundManager->GetSoundBuffer(nIndex)->GetSoundData())->GetContainer().HasHandle() )
				{
					++nNbrFlow;
				}
			}

			nNbrStreamedSound = nNbrSound - nNbrFlow;

			strTmp.Format("{S:100}%x Streamed Flow(s){P}", nNbrFlow);
			String += strTmp;

			for ( nIndex = 0 ; nIndex < nNbrSound ; ++nIndex )
			{
				SOUND_BUFFER_ABC * pSoundBuffer = pSoundManager->GetSoundBuffer(nIndex);
				SOUND_DATA * pSoundData = (SOUND_DATA *)pSoundBuffer->GetSoundData();

				if ( pSoundData->GetContainer().HasHandle() )
				{
					COMMUNICATOR * pFlowDialog = (COMMUNICATOR *) pSoundData->GetContainer().GetCommunicator();
					pSoundData->ReadHeader(nType, nSize, bStereo, nSamplesPerSec, nBitsPerSample);
					if ( bStereo )
					{
						sStrereo = "S";
					}
					else
					{
						sStrereo = "M";
					}
					
					strTmp.Format("{S:100}%s(%d) [%d%s] Volume=%d{P}", pFlowDialog->GetName().GetBuffer(), pFlowDialog->GetID(), (nSamplesPerSec / 1000), sStrereo.GetBuffer(), pSoundBuffer->GetVolume());
					String += strTmp;
				}
			}

			strTmp.Format("{S:100}%x Streamed Sound(s){P}", nNbrSound);
			String += strTmp;

			for ( nIndex = 0 ; nIndex < nNbrSound ; ++nIndex )
			{
				SOUND_BUFFER_ABC * pSoundBuffer = pSoundManager->GetSoundBuffer(nIndex);
				SOUND_DATA * pSoundData = (SOUND_DATA *)pSoundBuffer->GetSoundData();
				
				if ( pSoundData->GetContainer().HasHandle() ==false )
				{
					pSoundData->ReadHeader(nType, nSize, bStereo, nSamplesPerSec, nBitsPerSample);
					if ( bStereo )
					{
						sStrereo = "S";
					}
					else
					{
						sStrereo = "M";
					}
					
					strTmp.Format("{S:100}%s(%d) [%d%s] Volume=%d{P}", pSoundData->GetName().GetBuffer(), pSoundData->GetID(), (nSamplesPerSec / 1000), sStrereo.GetBuffer(), pSoundBuffer->GetVolume());
					String += strTmp;
				}
			}

			nNbrSound = pSoundManager->GetNbrSoundBufferFX();

			strTmp.Format("{S:100}%x FX Sound(s){P}", nNbrSound);
			String += strTmp;

			for ( nIndex = 0 ; nIndex < nNbrSound ; ++nIndex )
			{
				SOUND_BUFFER_FX_ABC * pSoundBufferFX = pSoundManager->GetSoundBufferFX(nIndex);
				SOUND_DATA * pSoundData = (SOUND_DATA *) pSoundBufferFX->GetSoundFX();

				pSoundData->ReadHeader(nType, nSize, bStereo, nSamplesPerSec, nBitsPerSample);
				if ( bStereo )
				{
					sStrereo = "S";
				}
				else
				{
					sStrereo = "M";
				}
				
				strTmp.Format("{S:100}%s(%d) [%d%s] Volume=%d{P}", pSoundData->GetName().GetBuffer(), pSoundData->GetID(), (nSamplesPerSec / 1000), sStrereo.GetBuffer(), pSoundBufferFX->GetVolume());
				String += strTmp;
			}

			pSoundStatsText->SetText(String);
			pSoundStatsText->SetVerticalAlignement( IHM_ELEMENT_TEXT::VALIGN_TOP );
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		AddText
//	Object:		
//	02-01-22:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::AddText(const COM_INSTANCE &Text) const
{
	DISPLAY_MANAGER::Instance()->AddIhmElement(Text);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveText
//	Object:		
//	02-01-22:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::RemoveText(const COM_INSTANCE &Text) const
{	
	DISPLAY_MANAGER::Instance()->RemoveIhmElement(Text);
}

//-----------------------------------------------------------------------------
//	Name:		InitText
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::InitText(const COM_INSTANCE	&Text,
							 float			rLeft,
							 float			rRight,
							 float			rTop,
							 float			rBottom) const
{
	IHM_ELEMENT_TEXT * pText = (IHM_ELEMENT_TEXT *) Text.GetCommunicator();
	QDT_ASSERT(pText);

	pText->SetRelativePosLeft	(rLeft);		
	pText->SetRelativePosRight	(rRight);	
	pText->SetRelativePosTop	(rTop);		
	pText->SetRelativePosBottom	(rBottom);

	pText->SetDefaultColor(COLOR(255, 128, 128, 128));

	// Can't be displayed in wireframe
	pText->SetWire(false);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayFrameRate
//	Object:		
//	01-10-25:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplayShortStats(bool bValue)
{
	_bDisplayShortStats = bValue;
	_bChangeStats = true;

	if (bValue)
	{
		if (_bFontIsInit == false)
		{
			InitFonts();
		}

		DisplayGraphicStats(false);
		DisplayMemStats(false);
		DisplayDataBankStats(false);
		DisplaySoundStats(false);

		AddText(_ShortStatsText);
	}
	else
	{
		RemoveText(_ShortStatsText);
	}
}
//-----------------------------------------------------------------------------
//	Name:		IsShortStatsDisplay
//	Object:		
//	02-05-30:	DAC - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsShortStatsDisplay() const
{
	return	(_bDisplayShortStats);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayGraphicOptions
//	Object:		
//	04-09-09:	FPR - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplayGraphicOptions(bool	bValue)
{
	_bDisplayGraphicOptions = bValue;
	_bChangeStats = true;

	if (_bFontIsInit == false)
	{
		InitFonts();
	}

	if (bValue)
	{
		DisplayGraphicStats(false);
		DisplayMemStats(false);
		DisplayDataBankStats(false);
		DisplayShortStats(false);
		DisplaySoundStats(false);

		AddText(_GraphicOptionsText);
	}
	else
	{
		RemoveText(_GraphicOptionsText);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsGraphicOptionsDisplay
//	Object:		
//	04-09-09:	FPR - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsGraphicOptionsDisplay() const
{
	return (_bDisplayGraphicOptions);
}


//-----------------------------------------------------------------------------
//	Name:		DisplayCompleteStats
//	Object:		
//	02-01-22:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplayGraphicStats(bool	bValue)
{
	_bDisplayGraphicStats = bValue;
	_bChangeStats = true;

	if (bValue)
	{
		if (_bFontIsInit == false)
		{
			InitFonts();
		}

		DisplayShortStats(false);
		DisplayMemStats(false);
		DisplayDataBankStats(false);
		DisplaySoundStats(false);

		AddText(_FullStatsText);
	}
	else
	{
		RemoveText(_FullStatsText);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsGraphicStatsDisplay
//	Object:		
//	02-05-30:	DAC - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsGraphicStatsDisplay() const
{
	return	(_bDisplayGraphicStats);
}


//-----------------------------------------------------------------------------
//	Name:		DisplayMemStats
//	Object:		
//	02-01-24:	RMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplayMemStats(bool	bValue)
{
	_bDisplayMemStats = bValue;
	_bChangeStats = true;

	if (bValue)
	{
		if (_bFontIsInit == false)
		{
			InitFonts();
		}

		DisplayShortStats(false);
		DisplayGraphicStats(false);
		DisplayDataBankStats(false);
		DisplaySoundStats(false);

		AddText(_MemStatsText);
	}
	else
	{
		RemoveText(_MemStatsText);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsMemStatsDisplay
//	Object:		
//	02-05-30:	DAC - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsMemStatsDisplay() const
{
	return	(_bDisplayMemStats);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayDataBankStats
//	Object:		
//	04-02-12:	VMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplayDataBankStats(bool	bValue)
{
	_bDisplayDataBankStats = bValue;
	_bChangeStats = true;	

	if (bValue)
	{
		if (_bFontIsInit == false)
		{
			InitFonts();
		}

		DisplayShortStats(false);
		DisplayGraphicStats(false);
		DisplayMemStats(false);
		DisplaySoundStats(false);

		AddText(_DataBankStatsText);
	}
	else
	{
		RemoveText(_DataBankStatsText);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsDataBankStatsDisplay
//	Object:		
//	04-02-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsDataBankStatsDisplay() const
{
	return	(_bDisplayDataBankStats);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayVMStats
//	Object:		
//	04-06-25:	DAC - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplayVMStats(bool	bValue)
{
	_bDisplayVMStats	= bValue;
	_bChangeStats		= true;

	if (bValue)
	{
		if (_bFontIsInit == false)
		{
			InitFonts();
		}

		DisplayShortStats(false);
		DisplayGraphicStats(false);
		DisplayMemStats(false);
		DisplayDataBankStats(false);
		DisplaySoundStats(false);

		AddText(_VMStatsText);
	}
	else
	{
		RemoveText(_VMStatsText);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsVMStatsDisplay
//	Object:		
//	04-02-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsVMStatsDisplay() const
{
	return	(_bDisplayVMStats);
}

//-----------------------------------------------------------------------------
//	Name:		DisplaySoundStats
//	Object:		
//	05-03-21:	AMA - Created
//-----------------------------------------------------------------------------
void	STATISTICS::DisplaySoundStats(bool	bValue)
{
	_bDisplaySoundStats	= bValue;
	_bChangeStats		= true;

	if (bValue)
	{
		if (_bFontIsInit == false)
		{
			InitFonts();
		}

		DisplayShortStats(false);
		DisplayGraphicStats(false);
		DisplayMemStats(false);
		DisplayDataBankStats(false);
		DisplayVMStats(false);

		AddText(_SoundStatsText);
	}
	else
	{
		RemoveText(_SoundStatsText);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsSoundStatsDisplay
//	Object:		
//	05-03-21:	AMA - Created
//-----------------------------------------------------------------------------
bool	STATISTICS::IsSoundStatsDisplay() const
{
	return	(_bDisplaySoundStats);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
