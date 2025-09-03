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
//	CLASS:	SHADOW_OBJECT
//	The SHADOW_OBJECT class implements ...
//
//	03-10-23:	FPR - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __SCENEGRAPH_NODE_SHADOWOBJECT_H__
#define __SCENEGRAPH_NODE_SHADOWOBJECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	SHADOW_OBJECT
		{
		public:
			SHADOW_OBJECT();
			~SHADOW_OBJECT();

			// Accessors
			COM_INSTANCE&				GetNodeInst()										{ return (_Node); }
			QDT_VECTOR <unsigned int> &	GetGeomIndexes()									{ return (_vGeomIndex); }			
			unsigned int				GetGeomIndex(unsigned int i)						{ return (_vGeomIndex[i]); }			
			unsigned int				GetNbrGeomIndex()									{ return (_vGeomIndex.GetSize()); }

			// Modifiors
			void						SetNodeInst(const COM_INSTANCE& NodeInst)					{ _Node = NodeInst; }
			void						AddGeomIndex(unsigned int nIndex)					{ _vGeomIndex.PushTail(nIndex); }			

		private:

			COM_INSTANCE				_Node;
			QDT_VECTOR <unsigned int>	_vGeomIndex;
		};
	}
}

#endif // __SCENEGRAPH_NODE_SHADOWOBJECT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
