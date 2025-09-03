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

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREECELL_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREECELL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)

namespace QDT
{
	namespace M3D
	{
		class	AABB_TREE_CELL
		{
		public:

			// Constructor & destructor
								AABB_TREE_CELL();
								~AABB_TREE_CELL();

			// Accessors
			AABB_TREE_CELL *	GetLeftChild() const					{ return (_pLeftChild); }
			AABB_TREE_CELL *	GetRightChild() const					{ return (_pRightChild); }
			const AABB &		GetBoundingBox() const					{ return (_BoundingBox); }

			// Modifiors
			void				SetIndex(unsigned int i, unsigned int nIndex);
			void				SetBoundingBox(const AABB & BoundingBox) { _BoundingBox = BoundingBox; }

			// Display
			void				Display(VECTOR * pPoints);

			// Ray cast
		//	bool				RayCast(VECTOR *pPoints, RAY_CAST_PARAMETERS & RayCastParameters, const RAY & Ray);

			// Test zone
			void				TestZone(VECTOR *pPoints, VERTICAL_RAY &Ray, bool & bAbove, bool & bBelow) const;

			// For export
			void				CreateChilds();
			void				CreateIndices(unsigned int nNbrIndices);

			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			void				Read0(IO_STREAM & Stream);
			void				Read1(IO_STREAM & Stream);
		#endif
		#ifndef _MASTER
			void				Write(IO_STREAM & Stream);
		#endif

		private:

			AABB				_BoundingBox;

			// Binary tree (for now)
			AABB_TREE_CELL *	_pLeftChild;
			AABB_TREE_CELL *	_pRightChild;

			// Indexed triangle list
			unsigned int *		_pIndices;
			unsigned int		_nNbrIndices;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREECELL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
