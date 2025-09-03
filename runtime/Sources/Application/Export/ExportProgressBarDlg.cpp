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
//	CLASS:	EXPORT_PROGRESS_BAR_DLG
//
//	01-10-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_RUN(Application/Export/ExportProgressBarDlg)

#include	INCL_RUN(Application/Export/RtOutput)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
EXPORT_PROGRESS_BAR_DLG	*	EXPORT_PROGRESS_BAR_DLG::_pProgressBarDlg = NULL;

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	04-01-16:	DAC - Created
//-----------------------------------------------------------------------------
EXPORT_PROGRESS_BAR_DLG *	EXPORT_PROGRESS_BAR_DLG::Instance()
{
	if	(_pProgressBarDlg == NULL)
	{
		_pProgressBarDlg = new EXPORT_PROGRESS_BAR_DLG();
	}

	return	(_pProgressBarDlg);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	04-01-16:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::Suicide()
{
	delete _pProgressBarDlg;
	_pProgressBarDlg;
}

//-----------------------------------------------------------------------------
//	Name:		EXPORT_PROGRESS_BAR_DLG constructor
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
EXPORT_PROGRESS_BAR_DLG::EXPORT_PROGRESS_BAR_DLG(WND *	pParentWnd)
:
DIALOG	(IDD, pParentWnd),
_pOutput(NULL)
{
	_pOutput = new RT_OUTPUT(this);
}

//-----------------------------------------------------------------------------
//	Name:		~EXPORT_PROGRESS_BAR_DLG destructor
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
EXPORT_PROGRESS_BAR_DLG::~EXPORT_PROGRESS_BAR_DLG()
{
	delete _pOutput;
}

//-----------------------------------------------------------------------------
//	Name:		OnInitDialog
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	EXPORT_PROGRESS_BAR_DLG::OnInitDialog()
{
	DIALOG::OnInitDialog();

	if ( _sTitle.IsEmpty() == false )
	{
		SetWindowText( _sTitle );
	}
	
	return ( TRUE );
}

//-----------------------------------------------------------------------------
//	Name:		SetDialogTitle
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::SetDialogTitle(const QDT_STRING &	sTitle)
{
	_sTitle = sTitle;
}

//-----------------------------------------------------------------------------
//	Name:		Step
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::Step(const int nIdx, const QDT_STRING &	sMsg)
{
	int nLower, nUpper, nCur;
	
	nCur = GetProgressBarCtrl(nIdx)->GetPos();
	GetProgressBarCtrl(nIdx)->GetRange(nLower, nUpper);
	
	if	(nCur >= nUpper)
	{
		GetProgressBarCtrl(nIdx)->SetPos(0);
	}
	
	GetProgressBarCtrl(nIdx)->StepIt();
	ConsoleOutput(sMsg);
}

//-----------------------------------------------------------------------------
//	Name:		SetRangeAndStep
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::SetRangeAndStep(	const int	nIdx, 
													const int	nLower,
													const int	nUpper,
													const int	nStep)
{
	GetProgressBarCtrl(nIdx)->SetRange32( nLower, nUpper );
	GetProgressBarCtrl(nIdx)->SetStep( nStep );
}

//-----------------------------------------------------------------------------
//	Name:		SetPos
//	Object:		
//	02-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::SetPos(const int nIdx, const int	nPos)
{
	GetProgressBarCtrl(nIdx)->SetPos( nPos );
}

//-----------------------------------------------------------------------------
//	Name:		ResetPos
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::ResetPos(const int nIdx)
{
	GetProgressBarCtrl(nIdx)->SetPos( 0 );
}

//-----------------------------------------------------------------------------
//	Name:		GetProgressBarCtrl
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
PROGRESS_CTRL *	EXPORT_PROGRESS_BAR_DLG::GetProgressBarCtrl(const int nIdx) const
{
	PROGRESS_CTRL *	pBar;
	switch(nIdx)
	{
		case 0:
		{
			pBar = static_cast<PROGRESS_CTRL*>( GetDlgItem(IDC_PROGRESS_BAR_GENERIC) );
			QDT_ASSERT(pBar);
			break;
		}
		case 1:
		{
			pBar = static_cast<PROGRESS_CTRL*>( GetDlgItem(IDC_PROGRESS_BAR_GENERIC2) );
			QDT_ASSERT(pBar);
			break;
		}
		default:
			QDT_FAIL();
			pBar = NULL;
	}

	return(pBar);
}

//-----------------------------------------------------------------------------
//	Name:		GetOutputListBox
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
LIST_BOX *	EXPORT_PROGRESS_BAR_DLG::GetOutputListBox() const
{
	LIST_BOX *	pCtl;

	pCtl = static_cast<LIST_BOX*>( GetDlgItem(IDC_OUTPUT) );
	QDT_ASSERT(pCtl);

	return(pCtl);
}

//-----------------------------------------------------------------------------
//	Name:		GetStaticText
//	Object:		
//	01-10-24:	DAC - Created
//-----------------------------------------------------------------------------
STATIC *	EXPORT_PROGRESS_BAR_DLG::GetStaticText() const
{
	STATIC *	pText;

	pText = static_cast<STATIC*>( GetDlgItem(IDC_STATIC_TEXT) );
	QDT_ASSERT(pText);

	return(pText);
}

//-----------------------------------------------------------------------------
//	Name:		SetStatus
//	Object:		
//	04-01-16:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::SetStatus(const QDT_STRING &	sMsg)
{
	GetStaticText()->SetWindowText(sMsg);
}


//-----------------------------------------------------------------------------
//	Name:		ConsoleOutput
//	Object:		
//	04-01-16:	DAC - Created
//-----------------------------------------------------------------------------
void	EXPORT_PROGRESS_BAR_DLG::ConsoleOutput(const QDT_STRING &	sMsg)
{
	QDT_STRING sTmp;
	CString str;
	str = CTime::GetCurrentTime().Format("[%H:%M]");
	sTmp.Format("%s : %s", str.GetBuffer(0), sMsg.GetBuffer());
	
	SetRedraw(FALSE);

	GetOutputListBox()->AddString(sTmp);
	
	GetOutputListBox()->SetTopIndex( GetOutputListBox()->GetCount() - 1 );

	SetRedraw(TRUE);
	RedrawWindow();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
