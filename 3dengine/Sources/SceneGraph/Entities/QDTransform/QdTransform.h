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
//	CLASS:	QD_TRANSFORM
//	The QD_TRANSFORM class implements ...
//
//	01-11-28:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_QDTRANSFORM_H__
#define __M3D_SCENEGRAPH_ENTITIES_QDTRANSFORM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		class	QD_TRANSFORM : public ENTITY_ABC
		{
		public:

			QD_TRANSFORM();
			~QD_TRANSFORM();

			virtual void						Instanciate(COM_INSTANCE & Instance);
			virtual void						InstanciateRec(COM_INSTANCE & Instance);
			virtual bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);

		#ifndef _MASTER_ONLY_DATABANK
			static	void						CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static	void						CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_QDTRANSFORM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
