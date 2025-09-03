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
//	CLASS:	FLAG_EDITOR_DLG
//
//	02-07-08:	VPI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
#undef CCmdTarget
BEGIN_MESSAGE_MAP (FLAG_EDITOR_DLG, DIALOG)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ_TYPE, OnTypeChange)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJECT, OnObjChanged)
	ON_NOTIFY(NM_CLICK, IDC_TREE_FLAG_LIST, OnFlagClick)
END_MESSAGE_MAP()
#define CCmdTarget DONT_USE_CCmdTarget_USE_CMD_TARGET

//-----------------------------------------------------------------------------
//	Name:		FLAG_EDITOR_DLG constructor
//	Object:		
//	02-07-08:	VPI - Created
//-----------------------------------------------------------------------------
FLAG_EDITOR_DLG::FLAG_EDITOR_DLG(WND *	pParentWnd)
:
DIALOG(IDD, pParentWnd),
_pObjFlag(NULL)
{
	Create(IDD, pParentWnd);
}

//-----------------------------------------------------------------------------
//	Name:		FLAG_EDITOR_DLG destructor
//	Object:		
//	02-07-08:	VPI - Created
//-----------------------------------------------------------------------------
FLAG_EDITOR_DLG::~FLAG_EDITOR_DLG()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		SetObjFlag
//	Object:		
//	02-07-09:	VPI - Created
//-----------------------------------------------------------------------------
void	FLAG_EDITOR_DLG::SetObjFlag(OBJECT_FLAG *	pObjFlag)
{
	_pObjFlag = pObjFlag;
}

//-----------------------------------------------------------------------------
//	Name:		OnInitDialog
//	Object:		
//	02-07-09:	VPI - Created
//-----------------------------------------------------------------------------
BOOL	FLAG_EDITOR_DLG::OnInitDialog()
{
	BOOL bResult = DIALOG::OnInitDialog();
	//Init Type Combo.
	COMBO_BOX * pTypeCombo = reinterpret_cast<COMBO_BOX *>(GetDlgItem(IDC_COMBO_OBJ_TYPE));

	QDT_STRING strName;
	int nMax = GetNbrTypes();
	int i;

	for (i = 0; i < nMax; ++i)
	{
		strName = GetTypeName(i);
		int nPos = pTypeCombo->InsertString(i, strName.GetBuffer());
		pTypeCombo->SetItemData(nPos, i);
	}

	pTypeCombo->SetCurSel(0);
	OnTypeChange();

	return (bResult);
}

//-----------------------------------------------------------------------------
//	Name:		OnTypeChange
//	Object:		
//	02-07-09:	VPI - Created
//-----------------------------------------------------------------------------
void	FLAG_EDITOR_DLG::OnTypeChange()
{
	COMBO_BOX * pTypeCombo = reinterpret_cast<COMBO_BOX *>(GetDlgItem(IDC_COMBO_OBJ_TYPE));
	TREE_CTRL * pObjTree = reinterpret_cast<TREE_CTRL *>(GetDlgItem(IDC_TREE_OBJECT));
	TREE_CTRL * pFlagTree = reinterpret_cast<TREE_CTRL *>(GetDlgItem(IDC_TREE_FLAG_LIST));
	HTREEITEM Item;
	QDT_STRING strName;

	int nIndex = pTypeCombo->GetCurSel();
	nIndex = pTypeCombo->GetItemData(nIndex);
	SetCurrentType(nIndex);

	pObjTree->DeleteAllItems();
	pFlagTree->DeleteAllItems();

	int nMax = GetNbrObj();
	int i;

	for (i = 0; i < nMax; ++i)
	{
		strName = GetObjName(i);
		Item = pObjTree->InsertItem(strName.GetBuffer(), TVI_ROOT, TVI_SORT);
		pObjTree->SetItemData(Item, i);
	}

	nMax = GetNbrFlagsForCurrentType();
	for (i = 0; i < nMax; ++i)
	{
		strName = GetFlagName(i);
		Item = pFlagTree->InsertItem(strName.GetBuffer(), TVI_ROOT, TVI_SORT);
		pObjTree->SetItemData(Item, i);
	}
	pFlagTree->EnableWindow(FALSE);
}

//-----------------------------------------------------------------------------
//	Name:		OnObjChanged
//	Object:		
//	02-07-09:	VPI - Created
//-----------------------------------------------------------------------------
void	FLAG_EDITOR_DLG::OnObjChanged(NMHDR *	pNotifyStruct,
									  LRESULT *	result)
{
	TREE_CTRL * pObjTree = reinterpret_cast<TREE_CTRL *>(GetDlgItem(IDC_TREE_OBJECT));
	TREE_CTRL * pFlagTree = reinterpret_cast<TREE_CTRL *>(GetDlgItem(IDC_TREE_FLAG_LIST));
	HTREEITEM Item;
	int nObjIndex, nFlagIndex;

	Item = pObjTree->GetSelectedItem();
	if (Item == NULL)
	{
		return;
	}
	nObjIndex = pObjTree->GetItemData(Item);

	pFlagTree->EnableWindow(TRUE);
	
	//int nMax = pFlagTree->GetCount();
	Item = pFlagTree->GetNextItem(NULL, TVGN_ROOT);
	while (Item != NULL)
	{
		nFlagIndex = pFlagTree->GetItemData(Item);
		bool bStatus = GetFlagState(nObjIndex, nFlagIndex);
		pFlagTree->SetCheck(Item, bStatus);
		Item = pFlagTree->GetNextItem(Item, TVGN_NEXT);
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnFlagClick
//	Object:		
//	02-07-10:	VPI - Created
//-----------------------------------------------------------------------------
void	FLAG_EDITOR_DLG::OnFlagClick(NMHDR *	pNotifyStruct,
									 LRESULT *	result)
{
	TREE_CTRL * pObjTree = reinterpret_cast<TREE_CTRL *>(GetDlgItem(IDC_TREE_OBJECT));
	TREE_CTRL * pFlagTree = reinterpret_cast<TREE_CTRL *>(GetDlgItem(IDC_TREE_FLAG_LIST));
	UINT flag;
	HTREEITEM Item;
	int nObjIndex, nFlagIndex;

	Item = pObjTree->GetSelectedItem();
	if (Item == NULL)
	{
		return;
	}
	nObjIndex = pObjTree->GetItemData(Item);

	POINT Point;
	GetCursorPos(&Point);
	pFlagTree->ScreenToClient(&Point);

	Item = pFlagTree->HitTest(Point, &flag);

	if (flag == TVHT_ONITEMSTATEICON)
	{
		//not is because this event is call before default threatment
		bool bValue = ! pFlagTree->GetCheck(Item);
		nFlagIndex = pObjTree->GetItemData(Item);
		SetFlagState(nObjIndex, nFlagIndex, bValue);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// testing interface
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int	FLAG_EDITOR_DLG::GetNbrTypes()
{
	return(2);
}

QDT_STRING	FLAG_EDITOR_DLG::GetTypeName(int	nIndex)
{
	switch (nIndex)
	{
		default:
			return("");
		case 0:
			return("Type0");
		case 1:
			return("Type1");
	}
}

void	FLAG_EDITOR_DLG::SetCurrentType(int	nIndex)
{
	_nCurType = nIndex;
}

int	FLAG_EDITOR_DLG::GetNbrObj()
{
	switch (_nCurType)
	{
		default:
			return(0);
		case 0:
			return(1);
		case 1:
			return(2);
	}
}

QDT_STRING	FLAG_EDITOR_DLG::GetObjName(int	nIndex)
{
	switch (_nCurType)
	{
		default:
			return("");
		case 0:
			switch (nIndex)
			{
				default:
					return ("");
				case 0:
					return ("Obj00");
			}
		case 1:
			switch (nIndex)
			{
				default:
					return ("");
				case 0:
					return ("Obj10");
				case 1:
					return ("Obj11");
			}
	}
}

int	FLAG_EDITOR_DLG::GetNbrFlagsForCurrentType()
{
	switch (_nCurType)
	{
		default:
			return(0);
		case 0:
			return(2);
		case 1:
			return(3);
	}
}

QDT_STRING	FLAG_EDITOR_DLG::GetFlagName(int	nIndex)
{
	switch (_nCurType)
	{
		default:
			return("");
		case 0:
			switch (nIndex)
			{
				default:
					return ("");
				case 0:
					return ("Flag00");
				case 1:
					return ("Flag01");
			}
		case 1:
			switch (nIndex)
			{
				default:
					return ("");
				case 0:
					return ("Flag10");
				case 1:
					return ("Flag11");
				case 2:
					return ("Flag12");
			}
	}
}

bool	FLAG_EDITOR_DLG::GetFlagState(int	nObjIndex,
									  int	nFlagIndex)
{
	switch (_nCurType)
	{
		default:
			return(false);
		case 0:
			return (false);
		case 1:
			return (true);
	}
}

void	FLAG_EDITOR_DLG::SetFlagState(int	nObjIndex,
									  int	nFlagIndex,
									  bool	bState)
{
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
