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
//	CLASS:	RENDER_WINDOW_SUBCLASS
//
//	04-01-19:	DAC - Created
//*****************************************************************************

#ifndef _MASTER

#include	"Root.h"
#include	INCL_RUN(Application/Tools/RenderWindowSubclass)

#include	"../../../Resources\resource.h"
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_KCORE(Stream/FileTextStream)
#include	INCL_SINT(World/World)
#include	INCL_SINT(Movie/Movie)
#include	INCL_SINT(Automat/DialogAutomat/DialogAutomat)
#include	INCL_SINT(Automat/DialogAutomat/DialogManager)
#include	INCL_KCOM(Services/LoadingsRecorder)
#include	INCL_KSERV(KernelObject/Kernel)
#include	INCL_SINT(Sound/ScriptSoundManager)
#include	INCL_SINT(Dialog/FlowDialog)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

BEGIN_MESSAGE_MAP(RENDER_WINDOW_SUBCLASS, WND)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//	Name:		RENDER_WINDOW_SUBCLASS constructor
//	Object:		
//	04-01-19:	DAC - Created
//-----------------------------------------------------------------------------
RENDER_WINDOW_SUBCLASS::RENDER_WINDOW_SUBCLASS(HWND HWnd)
{
	QDT_ASSERT( HWnd );
	WND * pRenderWindow = WND::FromHandle( HWnd );
	QDT_ASSERT( pRenderWindow );

	BOOL bOK = SubclassWindow( HWnd );
	QDT_ASSERT( bOK );
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_WINDOW_SUBCLASS destructor
//	Object:		
//	04-01-19:	DAC - Created
//-----------------------------------------------------------------------------
RENDER_WINDOW_SUBCLASS::~RENDER_WINDOW_SUBCLASS()
{
	if	(::IsWindow(m_hWnd))
	{
		UnsubclassWindow();
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnRButtonDblClk
//	Object:		
//	04-01-19:	DAC - Created
//-----------------------------------------------------------------------------
void	RENDER_WINDOW_SUBCLASS::OnRButtonDown(UINT	nFlags,
												CPoint	point)
{
	if	( (nFlags & MK_CONTROL) != 0)
	{
		SpawnPopup();
	}
}


//-----------------------------------------------------------------------------
// Name: SpawnPopup
// Object: 
// 04-01-19 DAC Created: 
//-----------------------------------------------------------------------------
void RENDER_WINDOW_SUBCLASS::SpawnPopup()
{
	try
	{
		MENU menu;
		CRect rect;

		GetClientRect(rect);
		ClientToScreen(rect);
		
		CPoint PTopLeft = rect.TopLeft( );
		CPoint PTopRight;
		PTopRight.x = PTopLeft.x + rect.Width();
		PTopRight.y = PTopLeft.y;
		
		QDT_VERIFY(menu.LoadMenu(IDR_DEBUG_MENU));

		MENU* pPopup = menu.GetSubMenu(0);
    
		QDT_ASSERT(pPopup != NULL);

		WND* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
		{
			pWndPopupOwner = static_cast<WND*>(pWndPopupOwner->GetParent());
		}

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, PTopRight.x, PTopRight.y, pWndPopupOwner);

	}
	catch( ... )
	{
	}

}


//-----------------------------------------------------------------------------
//	Name:		OnCommand
//	Object:		
//	04-01-19:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	RENDER_WINDOW_SUBCLASS::OnCommand(WPARAM	wParam,
											LPARAM	lParam)
{
	switch (wParam)
	{
		case IDM_DUMP_LOADING_RECORDS:
		{
			QDT_STRING	sFName;
			QDT_STRING	sFilter;

			//	Dump Recording infos
			if	(LOADINGS_RECORDER::AreLoadingsRecorded())
			{
				sFilter.Format("Loading recorder dump (*.%s) | *.%s||",LOADINGS_RECORDER::LOADINGS_RECORDER_DUMP_EXT,LOADINGS_RECORDER::LOADINGS_RECORDER_DUMP_EXT);

				FILE_DIALOG fdDialog( false, 
									 LOADINGS_RECORDER::LOADINGS_RECORDER_DUMP_EXT, 
									 "LoadingsRecord", 
									 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
									 sFilter.GetBuffer(),
									 this);

				//	set dialog to current working dir
				char pcCurrentDir[ _MAX_PATH ];
				GetCurrentDirectory( _MAX_PATH, pcCurrentDir );
				fdDialog.m_ofn.lpstrInitialDir	= pcCurrentDir;

				if	(fdDialog.DoModal() == IDOK)
				{
					sFName = fdDialog.GetPathName();	
					FILE_STREAM F;
					if	(F.Open(sFName, FILE_STREAM::AT_CREATE))
					{
						LOADINGS_RECORDER::GetDefaultRecorder()->Write(F);
						F.Close();
					}
				}
			}
			break;
		}
		
		case IDM_START_LOADING_RECORDS:
		{	
			//	Disabled by default, record is started using menu
			LOADINGS_RECORDER::RecordLoadings(true);
			break;
		}

		case IDM_LIST_THREADS:
		{	
			//	Display all the threads from the VM
			//QDT_STRING	sTmp;
			//sTmp.Reserve(16384);

			//SCHEDULER::DumpSchedulerInfo(sTmp);
			
			//I_STAT_TOOLS_INTERFACE::GetInterface()->ShowDebugDlg(this, sTmp);
			break;
		}

		case IDM_LIST_WORLDS:
		{
			DumpWorldsAndSint();

			break;
		}

		case IDM_PAUSE_GAME:
		{	
			// Pause the whole game
			MODULE_ABC::Pause();
			break;
		}

		case IDM_RESUME_GAME:
		{	
			// Resume the whole game
			MODULE_ABC::Resume();
			break;
		}

		case IDM_ENTITIES_ANALYSER:
		{
			//I_STAT_TOOLS_INTERFACE::GetInterface()->ShowEntitiesAnalyser( reinterpret_cast<WND*>( this ));
			break;
		}

		case IDM_MEM_DUMP_INFO:
		{
			break;
		}

		case IDM_DUMP_STRINGS:
		{
			DumpStrings();

			break;
		}
		
		case IDM_BOUND_FPS:
		{
			DYN_DIALOG_EX Dlg;
			Dlg.SetWindowTitle("SETTING BOUND FPS");
			
			QDT_STRING sValue;
			sValue.ValueOf(KERNEL::GetBoundFPS());

			CString sTmp = sValue;
			UINT Id = Dlg.AddDlgControl(_T("EDIT"), _T("Bound FPS:"), STYLE_EDIT, EXSTYLE_EDIT, NULL, &sTmp);
			Dlg.DoModal();
			
			sValue = sTmp.GetBuffer(0);
			KERNEL::SetBoundFPS(sValue.Atof());
			break;
		};
	}

	return	(WND::OnCommand(wParam, lParam));
}

//-----------------------------------------------------------------------------
//	Name:		DumpWorldsAndSint
//	Object:		
//	04-08-24:	DAC - Created
//-----------------------------------------------------------------------------
void	RENDER_WINDOW_SUBCLASS::DumpWorldsAndSint()
{
	QDT_STRING			strPathName;
	int					nSessionIndex	= 1;
	FILE_TEXT_STREAM	File;
	QDT_STRING			strFileName, sFilter, sTmp;
	
	//	set dialog to current working dir
	char pcCurrentDir[ _MAX_PATH ];
	GetCurrentDirectory( _MAX_PATH, pcCurrentDir );
	strPathName = pcCurrentDir;
	
	strFileName.Format( "%s/DumpWorldInfo - Session%d.txt", strPathName.GetBuffer(), nSessionIndex );

	// Check for Existing Files
	while	( File.Open( strFileName, FILE_STREAM::AT_READ_ONLY ) )
	{
		++nSessionIndex;
		strFileName.Format( "%s/DumpWorldInfo - Session%d.txt", strPathName.GetBuffer(), nSessionIndex );
	}

	// Create File
	File.Open( strFileName, FILE_STREAM::AT_CREATE );
	QDT_ASSERT( File.IsValid() );

	//	Display all world infos
	File.PutString("Begin of Dump\n");
	WORLD::DumpAllWorldInfo(sTmp);
	File.PutString(sTmp.GetBuffer());
	
	MOVIE::DumpAllMoviesInfo("  ", sTmp);
	File.PutString(sTmp.GetBuffer());
	
	DIALOG_MANAGER::DumpAllDialogsInfo("  ", sTmp);
	File.PutString(sTmp.GetBuffer());
	
	SCRIPT_SOUND_MANAGER::DumpPlayingSounds("  ", sTmp);
	File.PutString(sTmp.GetBuffer());

	FLOW_DIALOG::DumpFlows("  ", sTmp);
	File.PutString(sTmp.GetBuffer());
	
	BEHAVIOR_MANAGER::Instance()->DumpAnimatedNodes("  ", sTmp);
	File.PutString(sTmp.GetBuffer());

	File.PutString("End of dump\n");
	File.Close();

	//I_STAT_TOOLS_INTERFACE::GetInterface()->ShowDebugDlg(this, strFileName);
}

//-----------------------------------------------------------------------------
//	Name:		DumpStrings
//	Object:		
//	04-09-21:	DAC - Created
//-----------------------------------------------------------------------------
void	RENDER_WINDOW_SUBCLASS::DumpStrings()
{
	QDT_STRING			strPathName;
	int					nSessionIndex	= 1;
	FILE_TEXT_STREAM	File;
	QDT_STRING			strFileName, sFilter, sTmp;
	
	//	set dialog to current working dir
	char pcCurrentDir[ _MAX_PATH ];
	GetCurrentDirectory( _MAX_PATH, pcCurrentDir );
	strPathName = pcCurrentDir;
	
	strFileName.Format( "%s/DumpStrings - Session%d.txt", strPathName.GetBuffer(), nSessionIndex );

	// Check for Existing Files
	while	( File.Open( strFileName, FILE_STREAM::AT_READ_ONLY ) )
	{
		++nSessionIndex;
		strFileName.Format( "%s/DumpStrings - Session%d.txt", strPathName.GetBuffer(), nSessionIndex );
	}

	// Create File
	File.Open( strFileName, FILE_STREAM::AT_CREATE );
	QDT_ASSERT( File.IsValid() );
	
	File.PutString("Begin of Dump\n");

	QDT_STRING::Print(&File);

	File.PutString("End of dump\n");
	File.Close();
}


#endif // _MASTER

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
