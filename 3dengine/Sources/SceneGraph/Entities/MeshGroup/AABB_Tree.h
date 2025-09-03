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
//	CLASS:	AABB_TREE
//	The AABB_TREE class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREE_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KMATH(VERTICAL_RAY)
DECL_3DENGINE(AABB_TREE_CELL)
DECL_3DENGINE(AABB_TREE_CELL_COMPRESSED)
DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace M3D
	{
		class	AABB_TREE
		{
		public:

										AABB_TREE();
										~AABB_TREE();

			// Accessors
			AABB_TREE_CELL *			GetRoot() const;
			AABB_TREE_CELL_COMPRESSED *	GetCompressedRoot() const;
			
			// Modifiors
			void						SetPoint(unsigned int i, const VECTOR& P);

			// Display
			void						Display();

			// Test zone
			void						TestZone(VERTICAL_RAY &Ray, bool & bAbove, bool & bBelow) const;

			// Ray cast
		//	bool						RayCast(RAY_CAST_PARAMETERS & RayCastParameters);
		//	bool						RayCast(RAY_CAST_PARAMETERS & RayCastParameters, const RAY & LocalRay);

			// For export
			void						CreateRoot();
			void						CreateCompressedRoot();
			void						CreatePoints(unsigned int nNbrPoints);

			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			void						Read0(IO_STREAM & Stream);
			void						Read1(IO_STREAM & Stream);
			void						Read2(IO_STREAM & Stream);
		#endif
		#ifndef _MASTER
			void						Write(IO_STREAM & Stream);
		#endif

		private:

			VECTOR *					_pPoints;
			AABB_TREE_CELL *			_pRoot;
			AABB_TREE_CELL_COMPRESSED * _pCompressedRoot;
			unsigned int				_nNbrPoints;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_AABBTREE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
