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
//	CLASS:	BLOC_READ_TASK
//	The BLOC_READ_TASK class implements ...
//
//	02-04-17:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	BLOC_READ_TASK : public READ_TASK
{
public:
	/////////////////////////////////////////////////////////////
						BLOC_READ_TASK();
	explicit			BLOC_READ_TASK(const BLOC_READ_TASK & C);
	~BLOC_READ_TASK();
	BLOC_READ_TASK&		operator=(const BLOC_READ_TASK & C);
	void				Copy(const BLOC_READ_TASK & C);
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	bool				PreLoad( READ_TASK *pRT );
	void				ChildsPostLoad( READ_TASK *pRT );
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	BLOC*				GetBloc();
	void				SetBloc( BLOC *pB );
	/////////////////////////////////////////////////////////////

protected:
	/////////////////////////////////////////////////////////////
	BLOC				*_pBloc;
	/////////////////////////////////////////////////////////////
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
