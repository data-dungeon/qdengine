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
//	CLASS:	SPHERICAL_HARMONICS
//	The VECTOR_2 class implements ...
//
//	03-03-10:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_IRRADIANCE_VOLUME_H__
#define __KMATH_MATH_IRRADIANCE_VOLUME_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(MATRIX)
DECL_KMATH(SPHERICAL_HARMONICS_FIELD)
DECL_KMATH(SPHERICAL_HARMONICS)
DECL_KCORE(IO_STREAM)

#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

namespace	QDT
{
	namespace KMATH
	{
		class	IRRADIANCE_VOLUME
		{
		public:
			IRRADIANCE_VOLUME(IO_STREAM &s);
			IRRADIANCE_VOLUME();

			~IRRADIANCE_VOLUME();

			class NODE
			{
			public:
				UInt32   positiveXAdjacent;
				UInt32   negativeXAdjacent;
				UInt32   ShId[8];
				UInt32   Children[8];
				friend IO_STREAM&	operator <<(IO_STREAM &s, const NODE &m)
				{
					s << m.positiveXAdjacent;
					s << m.negativeXAdjacent;
					for (UInt32 i = 0; i < 8; i++) s << m.ShId[i];
					for (UInt32 j = 0; j < 8; j++) s << m.Children[j];
					return s;
				}
			};

			void SampleIrradiance(SPHERICAL_HARMONICS* sh, Float32 x, Float32 y, Float32 z);

			// Streams
			void				Read(IO_STREAM &s);
			void				Write(IO_STREAM &s) const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const IRRADIANCE_VOLUME &m)
			{
				m.Write(s);
				return s;
			}

			friend IO_STREAM&	operator >>(IO_STREAM &s, IRRADIANCE_VOLUME &m)
			{
				m.Read(s);
				return s;
			}

			// Accessors
			_INLINE_	NODE*&						Nodes() { return _pNodes; }
			_INLINE_	UInt32&						NodesCount() { return _uiNodesCount; }
			_INLINE_	SPHERICAL_HARMONICS_FIELD*&	Fields() { return _pShFields; }
			_INLINE_	UInt32&						FieldsCount() { return _uiNumFields; }
			_INLINE_	Float32&					MinX() { return _fMinX; }
			_INLINE_	Float32&					MaxX() { return _fMaxX; }
			_INLINE_	Float32&					MinY() { return _fMinY; }
			_INLINE_	Float32&					MaxY() { return _fMaxY; }
			_INLINE_	Float32&					MinZ() { return _fMinZ; }
			_INLINE_	Float32&					MaxZ() { return _fMaxZ; }

			IRRADIANCE_VOLUME::NODE* CellLookup(Float32 x, Float32 y, Float32 z, Float32*  bound = NULL);
			IRRADIANCE_VOLUME::NODE* CellLookup_rec(IRRADIANCE_VOLUME::NODE* rootnode,
													Float32	x,
													Float32	y,
													Float32	z,
													Float32*  bound);

		private:


			void SamplePositiveX(SPHERICAL_HARMONICS* sh, NODE* root, Float32* bound, Float32 x, Float32 y, Float32 z);
			void SampleNegativeX(SPHERICAL_HARMONICS* sh, NODE* root, Float32* bound, Float32 x, Float32 y, Float32 z);

			NODE*  _pNodes;
			UInt32 _uiNodesCount;

			SPHERICAL_HARMONICS_FIELD* _pShFields;
			UInt32 _uiNumFields;

			Float32 _fMinX;
			Float32 _fMaxX;
			Float32 _fMinY;
			Float32 _fMaxY;
			Float32 _fMinZ;
			Float32 _fMaxZ;
		};
	}
}

#endif //__KMATH_MATH_IRRADIANCE_VOLUME_H__
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
