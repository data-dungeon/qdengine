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
//	CLASS:	CM_SECTOR
//	The CM_SECTOR class implements ...
//
//	03-08-27:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMSECTOR_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMSECTOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(CM_EDGE)
DECL_KMATH(VECTOR)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

namespace QDT
{
	namespace M3D
	{
		class	CM_SECTOR
		{
		public:
			// Constructor & destructor
									CM_SECTOR();
									~CM_SECTOR();

			enum CM_SECTOR_FLAG
			{
				CMSF_NONE			= 0x00000000,
				CMSF_CAMERA_DISABLE	= 0x00000001,

				CMSF_FORCE_DWORD	= 0xffffffff
			};


			enum CM_SECTOR_RC
			{
				CMSRC_NO_INTERSECTION	= 0x00000000,
				CMSRC_PARALLEL,
				CMSRC_FULLINTERSECT,

				CMSRC_FORCEDWORD		= 0xffffffff
			};

					void			SetNbElements(int nElements);
			inline	int				GetNbElements(void)					{ return _nEntries; }
					void			SetNbFaces(int nbFaces);
			inline	int				GetNbFaces(void)					{ return _nFacesEntries; }
			inline	void			SetValue(int iIndex, int iValue)	{ _pIndexes[iIndex] = iValue; }
			inline	int				GetValue(int iIndex)				{ return _pIndexes[iIndex]; }
			inline	void			SetFaceValue(int iIdx, int iFace)	{ _pFaceIndexes[iIdx] = iFace; }
			inline	int				GetFaceValue(int iIdx)				{ return _pFaceIndexes[iIdx]; }

			inline	void			SetOrigin(float rX, float rY)		{ _Origin.SetXY(rX, rY); }
			inline	void			SetMax(float rX, float rY)			{ _Max.SetXY(rX, rY); }

			inline	POINT2D			&GetOrigin(void)					{ return _Origin; }
			inline	POINT2D			&GetMax(void)						{ return _Max; }

			inline	void			SetFlags(int Flag)					{ _Flags |= Flag; }
			inline	int				IsFlagSet(int Flag)					{ return (_Flags & Flag); }
			inline	int				GetFlags(void)						{ return _Flags; }
			inline	void			ClearFlags(int Flag)				{ _Flags &= ~Flag; }

					void			ClearEdgeUsingFlags(CM_EDGE *pEdges);


					CM_SECTOR_RC	GetIntersection(VECTOR &v1, VECTOR &v2, POINT2D *pVertices, POINT2D *pNormals, CM_EDGE *pEdges, POINT2D &Intersect, int &EdgeID);

		protected:
					void			Free();

		protected:
			POINT2D		_Origin;
			POINT2D		_Max;
			int			_nEntries;
			int			_nFacesEntries;
			int			*_pIndexes;
			int			*_pFaceIndexes;
			int			_Flags;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_COLLISIONMESH_CMSECTOR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
