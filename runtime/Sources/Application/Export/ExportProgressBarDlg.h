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
//	The EXPORT_PROGRESS_BAR_DLG class implements ...
//
//	04-01-16:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __RUNTIME_APPLICATION_EXPORT_EXPORTPROGRESSBARDLG_H__
#define __RUNTIME_APPLICATION_EXPORT_EXPORTPROGRESSBARDLG_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_MFC_WRAPPER(Defines)
#include	"../../../Resources/resource.h"

DECL_RUN(RT_OUTPUT)

namespace	QDT
{
	namespace	RUN
	{
		class	EXPORT_PROGRESS_BAR_DLG : public DIALOG
		{
		public:
			enum
			{
				IDD = IDD_SHOW_PROGRESS
			};

			explicit				EXPORT_PROGRESS_BAR_DLG(WND * pParentWnd = NULL);
			virtual				   ~EXPORT_PROGRESS_BAR_DLG();

			static	EXPORT_PROGRESS_BAR_DLG	*	Instance();
			static	void						Suicide();

					void			SetDialogTitle(const QDT_STRING & sTitle);
									
					void			Step(const int nIdx, const QDT_STRING & sMsg);
					void			SetRangeAndStep(const int nIdx, const int nLower, const int nUpper, const int nStep);
					void			ResetPos(const int nIdx);
					void			SetPos(const int nIdx, const int nPos);
					
					void			ConsoleOutput(const QDT_STRING & sMsg);
					void			SetStatus(const QDT_STRING & sMsg);

		protected:

			virtual	BOOL			OnInitDialog();

					PROGRESS_CTRL *	GetProgressBarCtrl(const int nIdx) const;

					STATIC *		GetStaticText() const;
					LIST_BOX *		GetOutputListBox() const;
					
		private:

			QDT_STRING					_sTitle;
			RT_OUTPUT *					_pOutput;

			static  EXPORT_PROGRESS_BAR_DLG	*	_pProgressBarDlg;
		};
	}
}

#endif	//__RUNTIME_APPLICATION_EXPORT_EXPORTPROGRESSBARDLG_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

