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
//	The DEBUG_DLG class implements ...
//
//	04-03-15:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	DEBUG_DLG : public DIALOG
{
public:
	enum
	{
		IDD = IDD_DEBUG_DIALOG
	};

		DEBUG_DLG(WND * pParent = NULL);
		explicit DEBUG_DLG(const DEBUG_DLG & C);
		~DEBUG_DLG();
		
		void	ConsoleOutput(const QDT_STRING & sMsg);

		void				SetStrDisplayOnInit(const QDT_STRING & sStr);
		const QDT_STRING &  GetStrDisplayOnInit() const;
		
		void				SetFileDisplayOnInit(const QDT_STRING & sTextFileName);

protected:

	BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP();

	LIST_BOX *		GetOutputListBox() const;

private:
		DEBUG_DLG &	operator=(const DEBUG_DLG & C);
		void	Copy(const DEBUG_DLG & C);

		QDT_STRING	_sStrDisplayOnInit;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
