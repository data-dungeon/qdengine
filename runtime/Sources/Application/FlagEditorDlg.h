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
//	The FLAG_EDITOR_DLG class implements ...
//
//	02-07-08:	VPI - Created
//*****************************************************************************

class OBJECT_FLAG
{
};

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	FLAG_EDITOR_DLG : public QDT::WMFC::DIALOG
{
	enum
	{
		IDD = IDD_FLAG_EDITOR_DLG,
		//IDC_TREE_FLAG_LIST
		//IDC_TREE_OBJECT
		//IDC_COMBO_OBJ_TYPE
	};
	
	DECLARE_MESSAGE_MAP()
		
public:
	FLAG_EDITOR_DLG(WND* pParentWnd = NULL);
	~FLAG_EDITOR_DLG();
	//	REMEMBER To make the destructor virtual if 
	//	you create a subclass, or if you have one

	void	SetObjFlag(OBJECT_FLAG* pObjFlag);

protected:
	virtual BOOL OnInitDialog( );
	
	afx_msg void OnTypeChange();
	afx_msg void OnObjChanged(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnFlagClick(NMHDR * pNotifyStruct, LRESULT * result);
	
private:
	OBJECT_FLAG* _pObjFlag;

	//Testing Interface;
	int GetNbrTypes();
	QDT_STRING GetTypeName(int nIndex);
	void SetCurrentType(int nIndex);
	int GetNbrObj();
	QDT_STRING GetObjName(int nIndex);
	int GetNbrFlagsForCurrentType();
	QDT_STRING GetFlagName(int nIndex);
	bool GetFlagState(int nObjIndex, int nFlagIndex);
	void SetFlagState(int nObjIndex, int nFlagIndex, bool bState);
	int _nCurType;

};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
