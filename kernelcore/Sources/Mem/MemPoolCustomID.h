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
//	CLASS:	MEM_VIRTUAL_ELEMENT
//	The MEM_VIRTUAL_ELEMENT class implements ...
//
//	06-02-23:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
/*
#ifndef __MEM_POOL_CUSTOM_ID_H__
#define __MEM_POOL_CUSTOM_ID_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

enum MEM_POOL_CUSTOM_ID
{
	MP_CUSTOMID_NOTCUSTOM = 0, // special case, used by the memory manager
	MP_CUSTOMID_QDT_STRING = 1,
	MP_CUSTOMID_COM_HANDLE,
	MP_CUSTOMID_COM_HANDLE_MUTEX,
	MP_CUSTOMID_ENUM_DESCRIPTOR,
	MP_CUSTOMID_EXEC_STACK,
	MP_CUSTOMID_METHOD_DESCRIPTOR,
	MP_CUSTOMID_READ_COMMUNICATOR,
	MP_CUSTOMID_READ_PARTITION,
	MP_CUSTOMID_UNLOAD_TASK,
};

#endif //__MEM_POOL_CUSTOM_ID_H__
*/

	ADD_POOL(QDT_STRING)

	ADD_POOL(COM_HANDLE)
	ADD_POOL(COM_HANDLE_MUTEX)
	ADD_POOL(ENUM_DESCRIPTOR)
	ADD_POOL(EXEC_STACK)
	ADD_POOL(METHOD_DESCRIPTOR)
	ADD_POOL(READ_COMMUNICATOR)
	ADD_POOL(READ_PARTITION)
	ADD_POOL(UNLOAD_TASK)

