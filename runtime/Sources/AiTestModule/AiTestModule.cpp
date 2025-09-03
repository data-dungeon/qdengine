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
//	CLASS:	AI_TEST_MODULE
//
//	05-02-01:	CVI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include INC_AIWRAPPER(AiWrapperManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


//-----------------------------------------------------------------------------
//	Name:		AI_TEST_MODULE constructor
//	Object:		
//	05-02-01:	CVI - Created
//-----------------------------------------------------------------------------
AI_TEST_MODULE::AI_TEST_MODULE()
{

	_Entities.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		AI_TEST_MODULE destructor
//	Object:		
//	05-02-01:	CVI - Created
//-----------------------------------------------------------------------------
AI_TEST_MODULE::~AI_TEST_MODULE()
{
	if ( _Entities.IsEmpty() == false )
	{
		_Entities.ClearAll();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Run
//	Object:		
//	05-02-01:	CVI - Created
//-----------------------------------------------------------------------------
void	AI_TEST_MODULE::Run()
{
	int				i, n;
	QDT_STRING		Name;
	int				nId = 0;
	AI_ENTITY		*pEntity;
	bool			bRes;

	n = 2;

	for ( i = 0; i < n; ++i )
	{
		Name.Format("Character %d", i);
		bRes = AI_WRAPPER_MANAGER::Instance()->CreateEntity( Name, &pEntity );
		QDT_ASSERT(bRes);
		AddEntity( pEntity );
	}

}


//-----------------------------------------------------------------------------
//	Name:		AddEntity
//	Object:		
//	05-02-02:	CVI - Created
//-----------------------------------------------------------------------------
void	AI_TEST_MODULE::AddEntity(AI_ENTITY *	pEntity)
{
	_Entities.PushTail( pEntity );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
