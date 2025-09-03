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
//	CLASS:	AABB_TREE_CELL
//	The AABB_TREE_CELL class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREECELLCOMPRESSED_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREECELLCOMPRESSED_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)

namespace QDT
{
	namespace M3D
	{
		class	AABB_TREE_CELL_COMPRESSED
		{
		public:

			// Constructor & destructor
										AABB_TREE_CELL_COMPRESSED();
										~AABB_TREE_CELL_COMPRESSED();

			// Accessors
			AABB_TREE_CELL_COMPRESSED *	GetLeftChild() const					{ return (_pLeftChild); }
			AABB_TREE_CELL_COMPRESSED *	GetRightChild() const					{ return (_pRightChild); }
			const AABB &				GetBoundingBox() const					{ return (_BoundingBox); }

			// Modifiors
			void						SetIndex(unsigned short i, unsigned int nIndex);
			void						SetBoundingBox(const AABB & BoundingBox) { _BoundingBox = BoundingBox; }

			// Display
			void						Display(VECTOR * pPoints);

			// Ray cast
		//	bool						RayCast(VECTOR *pPoints, RAY_CAST_PARAMETERS & RayCastParameters, const RAY & Ray);

			// Test zone
			void						TestZone(VECTOR *pPoints, VERTICAL_RAY &Ray, bool & bAbove, bool & bBelow) const;

			// For export
			void						CreateChilds();
			void						CreateIndices(unsigned int nNbrIndices);

			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			void						Read(IO_STREAM & Stream);
		#endif
		#ifndef _MASTER
			void						Write(IO_STREAM & Stream);
		#endif

		private:

			AABB						_BoundingBox;

			// Binary tree (for now)
			AABB_TREE_CELL_COMPRESSED *	_pLeftChild;
			AABB_TREE_CELL_COMPRESSED *	_pRightChild;

			// Indexed triangle list
			unsigned short *			_pIndices;
			unsigned int				_nNbrIndices;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREECELLCOMPRESSED_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
