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
//	CLASS:	WAY_POINT
//	The WAY_POINT class implements ...
//
//	03-08-08:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_WAYPOINT_H__
#define __M3D_SCENEGRAPH_ENTITIES_WAYPOINT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(WAY_POINT_LINK)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		class	WAY_POINT : public ENTITY_ABC
		{
		public:

			// Constructor & destrictor
												WAY_POINT();
											~WAY_POINT();
			// Modifiors
			void								SetRadius(float r)						{ _rRadius = r; }
			void								AddConnection(WAY_POINT_LINK *pC)		{ _vConnections.PushTail(pC); }
			void								BreakConnection(WAY_POINT_LINK *pC)		{ _vConnections.Remove(pC); }
			void								ClearConnections()						{ _vConnections.Clear(); }

			// Accessors
			float								GetRadius() const						{ return (_rRadius); }
			unsigned int						GetNbrConnections() const				{ return (_vConnections.GetSize()); }
			WAY_POINT_LINK *					GetConnection(unsigned int i) const		{ return (_vConnections[i]); }

			// Derivation from ENTITY_ABC
			virtual void						Display(DISPLAY_PARAMETERS & DisplayParameters);
			virtual bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

			// Derivation from communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);

			float								_rRadius;
			QDT_VECTOR<WAY_POINT_LINK*>			_vConnections;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_WAYPOINT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
