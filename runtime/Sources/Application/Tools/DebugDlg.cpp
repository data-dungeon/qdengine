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
//	CLASS:	DEBUG_DLG
//
//	04-03-15:	DAC - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include INC_KRNSERV(Tools)
#include INC_KRNCORE(Stream)

#undef	CWnd
BEGIN_MESSAGE_MAP( DEBUG_DLG, DIALOG )
END_MESSAGE_MAP()
#define	CWnd	DONT_USE_CWnd_USE_WND

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DEBUG_DLG constructor
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
DEBUG_DLG::DEBUG_DLG(WND * pParent)
:
DIALOG(IDD, pParent)
{

}

//-----------------------------------------------------------------------------
//	Name:		DEBUG_DLG constructor
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
DEBUG_DLG::DEBUG_DLG(const DEBUG_DLG & C)
{
		Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		DEBUG_DLG destructor
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
DEBUG_DLG::~DEBUG_DLG()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
DEBUG_DLG &	DEBUG_DLG::operator=(const DEBUG_DLG & C)
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
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
void	DEBUG_DLG::Copy(const DEBUG_DLG & C)
{
		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		OnInitDialog
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
BOOL	DEBUG_DLG::OnInitDialog()
{
	DIALOG::OnInitDialog();

	const QDT_STRING & sTmp = GetStrDisplayOnInit();
	if	(sTmp.IsEmpty() == false)
	{
		ConsoleOutput(sTmp);
	}

	return	(FALSE);
}

//-----------------------------------------------------------------------------
//	Name:		GetOutputListBox
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
LIST_BOX *	DEBUG_DLG::GetOutputListBox() const
{
	return	(static_cast<LIST_BOX*>(GetDlgItem(IDC_OUTPUT)));
}


//-----------------------------------------------------------------------------
//	Name:		ConsoleOutput
//	Object:		
//	04-01-16:	DAC - Created
//-----------------------------------------------------------------------------
void	DEBUG_DLG::ConsoleOutput(const QDT_STRING &	sMsg)
{
	QDT_STRING				sTmp;
	QDT_STRING_TOKENIZER	sTokenizer;
	CString					str;
	int						i, nNbrTokens;

	str = CTime::GetCurrentTime().Format("[%H:%M]");
	//sTmp.Format("%s : %s", str.GetBuffer(0), sMsg.GetBuffer());
	sTmp += str.GetBuffer(0);
	sTmp += " : ";
	sTmp += sMsg.GetBuffer();

	sTokenizer.Tokenize( sTmp, "\n");
	
	SetRedraw(FALSE);

	nNbrTokens = sTokenizer.GetNbrTokens();
	for	(i = 0; i < nNbrTokens; ++i)
	{
		GetOutputListBox()->AddString(sTokenizer.GetToken(i));
	}
	
	GetOutputListBox()->SetTopIndex( GetOutputListBox()->GetCount() - 1 );

	SetRedraw(TRUE);
	
	RedrawWindow();
}

//-----------------------------------------------------------------------------
//	Name:		SetStrDisplayOnInit
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
void	DEBUG_DLG::SetStrDisplayOnInit(const QDT_STRING &	sStr)
{
	_sStrDisplayOnInit = sStr;
}

//-----------------------------------------------------------------------------
//	Name:		GetStrDisplayOnInit
//	Object:		
//	04-03-15:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	DEBUG_DLG::GetStrDisplayOnInit() const
{
	return	(_sStrDisplayOnInit);
}

//-----------------------------------------------------------------------------
//	Name:		SetFileDisplayOnInit
//	Object:		
//	04-08-24:	DAC - Created
//-----------------------------------------------------------------------------
void	DEBUG_DLG::SetFileDisplayOnInit(const QDT_STRING &	sTextFileName)
{
	QDT_STRING			sTmp;
	FILE_TEXT_STREAM	File;
	QDT_STRING			strFileName;
	char				cBuffer[4096];
	
	if (File.Open( sTextFileName, FILE_STREAM::AT_READ_ONLY ))
	{
		// read
		while (File.IsEof() == false)
		{
			File.GetString(cBuffer, 4096);
			sTmp += cBuffer;
		}
		
		SetStrDisplayOnInit(sTmp);

		return;
	}

	QDT_Error("Can't open file %s to display dump info", sTextFileName.GetBuffer());
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
