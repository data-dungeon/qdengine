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

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMEDGE_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMEDGE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KMATH(POINT2D)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

namespace QDT
{
	namespace M3D
	{
		class	CM_EDGE
		{
		public:
			enum EDGE_VTX_FLAG
			{
				EVF_NONE		= 0,
				EVF_BLOCKING,

				EVF_FORCEDWORD	= 0xffffffff
			};

			enum EDGE_SIDE
			{
				SIDE_P1,
				SIDE_P2,
				SIDE_INSIDE,

				SIDE_FORCEDWORD	= 0xffffffff
			};

			enum EDGE_INTERSECTION_RC
			{
				EIRC_NO_INTERSECTION	= 0x00000000,
				EIRC_PARALLEL,
				EIRC_FULLINTERSECT,

				EIRC_FORCEDWORD		= 0xffffffff
			};

			// Constructor & destructor
			inline					CM_EDGE()								{ memset(_VtxFlag, 0, sizeof(_VtxFlag)); }
			inline					CM_EDGE(int VtxIdx0, int VtxIdx1)		{ memset(_VtxFlag, 0, sizeof(_VtxFlag)); SetIndexes(VtxIdx0, VtxIdx1); }
									~CM_EDGE()								{}

			inline	void					SetVtxFlags(int VtxFlag0, int VtxFlag1)	{ _VtxFlag[0] = VtxFlag0; _VtxFlag[1] = VtxFlag1; }
			inline	EDGE_VTX_FLAG			GetVtxFlag(int VtxID)					{ return (EDGE_VTX_FLAG)_VtxFlag[VtxID]; }
			inline	void					SetIndexes(int VtxIdx0, int VtxIdx1)	{ _VtxIdx[0] = VtxIdx0; _VtxIdx[1] =VtxIdx1; }
			inline	int						GetIndex(int Idx)						{ return _VtxIdx[Idx]; }
			inline	int						IsUsed(void)							{ return _Used;	}
			inline	void					SetUsed(int bUsed)						{ _Used = bUsed; }
					EDGE_INTERSECTION_RC	GetIntersection(VECTOR &v1, VECTOR &v2, POINT2D *Vertices, POINT2D &Intersect);
					int						IsPointInside(POINT2D &pt, POINT2D *Vertices, POINT2D *pV1=0, POINT2D *pV2=0 );

		protected:
			int			_VtxFlag[2];
			int			_VtxIdx[2];
			int			_Used;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMEDGE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
