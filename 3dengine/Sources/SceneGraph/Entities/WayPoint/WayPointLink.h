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
//	CLASS:	WAY_POINT_LINK
//	The WAY_POINT_LINK class implements ...
//
//	03-08-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_WAYPOINTLINK_H__
#define __M3D_SCENEGRAPH_ENTITIES_WAYPOINTLINK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(WAY_POINT)
DECL_KMATH(VECTOR)

#include	INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	WAY_POINT_LINK
		{
		public:

									WAY_POINT_LINK();
								~WAY_POINT_LINK();

			const COM_INSTANCE &	GetWayPointNode1() const;
			const COM_INSTANCE &	GetWayPointNode2() const;
			WAY_POINT *				GetWayPoint1() const;
			WAY_POINT *				GetWayPoint2() const;
			bool					IsOneWay() const						{ return _bOneWay; }

			void					SetWayPoint1(const COM_INSTANCE & WP)	{ _WP1 = WP; }
			void					SetWayPoint2(const COM_INSTANCE & WP)	{ _WP2 = WP; }
			float					SqrDistance(const VECTOR & Pos);
			void					SetOneWay(bool b) 						{ _bOneWay = b; }

		private:

			COM_INSTANCE			_WP1;
			COM_INSTANCE			_WP2;
			bool					_bOneWay;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_WAYPOINTLINK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
