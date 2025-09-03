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
//	CLASS:	STATS_ENTITY_TEXT
//	The STATS_ENTITY_TEXT class implements ...
//
//	06-03-24:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __STATS_ENTITY_TEXT_H__
#define __STATS_ENTITY_TEXT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(Display/Statistics/StatsEntityABC)

class	STATS_ENTITY_TEXT : public STATS_ENTITY_ABC
{
public:
		STATS_ENTITY_TEXT();
		~STATS_ENTITY_TEXT();

		virtual void Display(void);

		void SetShowTotal(Bool bShowTotal) {_bShowTotal = bShowTotal;}
private:
	Bool _bShowTotal;
	Float32 _bTotalSize;
};

#endif //__STATS_ENTITY_TEXT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
