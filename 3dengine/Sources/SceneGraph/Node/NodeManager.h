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
//	CLASS:	NODE_MANAGER
//	The NODE_MANAGER class implements ...
//
//	02-04-05:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __SCENEGRAPH_NODE_NODEMANAGER_H__
#define __SCENEGRAPH_NODE_NODEMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	NODE_MANAGER
		{
		public:

			static NODE_MANAGER *		Instance();
			static void 				Suicide();

			void						AddNode(const COM_INSTANCE & NodeInst, int nEntityType);
			void						RemoveNode(const COM_INSTANCE & NodeInst, int nEntityType);

			_INLINE_ const QDT_DLIST <COM_INSTANCE> & GetParticleEmitters() const	{ return (_dlParticleEmitters); }

		private:

										NODE_MANAGER();
									~NODE_MANAGER();

			void						AddNode(const COM_INSTANCE & Inst, QDT_DLIST <COM_INSTANCE> & dList);
			void						RemoveNode(const COM_INSTANCE & Ins, QDT_DLIST <COM_INSTANCE> & dList);

			static NODE_MANAGER *		_pInstance;

			QDT_DLIST <COM_INSTANCE>	_dlParticleEmitters;
		};
	}
}

#endif // __SCENEGRAPH_NODE_NODEMANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
