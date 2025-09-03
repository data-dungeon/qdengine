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
//	CLASS:	HDR_OBJECT
//	The HDR_OBJECT class implements ...
//
//	04-05-19:	FPR - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_HDROBJECT_H__
#define __DISPLAY_HDROBJECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(NODE)
DECL_3DENGINE(DISPLAY_PARAMETERS)

namespace QDT
{
	namespace M3D
	{
		class	HDR_OBJECT
		{
		public:
			HDR_OBJECT();
			HDR_OBJECT(NODE * pNode, unsigned int nGeometryIndex, bool bNormalDisplay);
			HDR_OBJECT(NODE * pNode, unsigned int nGeometryIndex, unsigned int nSkinClusterIndex, bool bNormalDisplay);
			
			~HDR_OBJECT();

			void					Display(DISPLAY_PARAMETERS & DisplayParameters);

			_INLINE_ void			SetNode(NODE * pNode);
			_INLINE_ void			SetGeometryIndex(unsigned int n);
			_INLINE_ void			SetSkinClusterIndex(unsigned int n);
			_INLINE_ void			SetNormalMeshDisplay(bool b);
			
			
			_INLINE_ unsigned int	GetSkinClusterIndex() const;
			_INLINE_ NODE *			GetNode();
			_INLINE_ bool			IsNormalMeshDisplay();

		private:
			NODE *					_pNode;
			unsigned int			_nGeometryIndex;
			unsigned int			_nSkinClusterIndex;
			bool					_bNormalMeshDisplay;
		};
	}
}

#ifndef _DEBUG
	#include	"HdrObject.inl"
#endif

#endif // __DISPLAY_HDROBJECT_H__
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
