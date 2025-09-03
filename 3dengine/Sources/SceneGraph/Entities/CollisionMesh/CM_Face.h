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
//	CLASS:	CM_EDGE
//	The CM_EDGE class implements ...
//
//	03-08-07:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMFACE_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMFACE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace M3D
	{
		class	CM_FACE
		{
		public:
			// Constructor & destructor
									CM_FACE()											{ }
			inline					CM_FACE( int VtxIdx0, int VtxIdx1, int VtxIdx2 )	{ SetIdx(VtxIdx0, VtxIdx1, VtxIdx2); }
									~CM_FACE()											{ }

			inline	void			SetIdx( int VtxIdx0, int VtxIdx1, int VtxIdx2 )		{ _VtxIdx[0] = VtxIdx0; _VtxIdx[1] = VtxIdx1; _VtxIdx[2] = VtxIdx2; }
			inline	int				GetIdx( int idx )									{ return _VtxIdx[idx]; }

		protected:
			int			_VtxIdx[3];
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMFACE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
