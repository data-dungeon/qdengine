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
//	CLASS:	SPEECH ELEMENT
//
//	04-01-05:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_COLLISION_COLLISIONNOTIFIER_H__
#define __M3D_COLLISION_COLLISIONNOTIFIER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	COLLISION_NOTIFIER : public COMMUNICATOR
		{
		public:
			enum COLLISION_NOTIFIER_METHODS
			{
				CMD_COLLISION_NOTIFIER_LAST = CMD_COM_USER_START
			};

			virtual	void		OnGroundEvent(	const COM_INSTANCE &SourceNodeCI, int nGroundPointID, float yAlt)	= 0;
			virtual	void		OnWallEvent(	const COM_INSTANCE &SourceNodeCI, int nOccurences				)	= 0;
			virtual void		OnDynamicEvent(	const COM_INSTANCE &SourceNodeCI, const COM_INSTANCE &NodeWith	)	= 0;
			virtual	void		EntityIsBeingRemoved(void)					{};

												COLLISION_NOTIFIER();
												COLLISION_NOTIFIER(CLASS_DESCRIPTOR * const pCD, COM_HANDLE * pHandle);
			static	COMMUNICATOR_DESCRIPTOR		*GetGenericCommunicatorDescriptor();
			static	COMMUNICATOR_DESCRIPTOR		*CreateGenericCommunicatorDescriptor();

			static	void		EM_Init					(CLASS_DESCRIPTOR * const pCD);	
			static	void		EM_Construct			(void * const pO, void * const pR, void * const pP);
			static	void		EM_Destruct				(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_COLLISION_COLLISIONNOTIFIER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
