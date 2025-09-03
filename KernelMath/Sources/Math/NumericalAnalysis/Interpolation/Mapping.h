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
//	CLASS:	MAPPING
//	The MAPPING class implements ...
//
//	04-08-26:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_INTERPOLATION_MAPPING_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_INTERPOLATION_MAPPING_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)

#include INCL_KCORE(Template/QDT_Vector)

namespace	QDT
{
	namespace KMATH
	{
		class MAPPING
		{
		public:

			enum	INTERPOLATION_ALGO
			{
				MIA_LINEAR,
				MIA_CUBIC,
			};


						MAPPING();
						MAPPING(const MAPPING & Mapping);

			void		Copy(const MAPPING & Mapping);
			void		operator=(const MAPPING & Mapping);

			void		Read(IO_STREAM & Stream);
			void		Write(IO_STREAM & Stream) const;

		#ifndef _MASTER
			void			WriteDependenciesBinary(int nThisOffset, IO_STREAM & Stream);
		#endif
			friend IO_STREAM & operator <<(IO_STREAM & Stream, const MAPPING & Mapping)
			{
				Mapping.Write( Stream );
				return ( Stream );
			}
			friend IO_STREAM & operator >>(IO_STREAM & Stream, MAPPING & Mapping)
			{
				Mapping.Read( Stream );
				return ( Stream );
			}

			int			GetInterpolationPointsCount() const;
			int			AddInterpolationPoint(float rX, float rY); // rX must be between 0 and 1.
			void		RemoveInterpolationPoint(int nPointIndex);

			void		SetInterpolationPoint(int nPointIndex, float rY);
			void		SetInterpolationPoint(int nPointIndex, float rX, float rY);

			float		GetInterpolationPointXCoord(int nPointIndex) const;
			float		GetInterpolationPointYCoord(int nPointIndex) const;

			float		GetValue(float rX, int nAlgo) const;  // rX must be between 0 and 1.

			void		SetMinMax(bool bHasMin, bool bHasMax, float rMin, float rMax);

			void		Clear(float rStartValue, float rEndValue);


		private:

			int			CircularExtension(int nPointIndex) const;
			void		GetCircularExtension(int nPointIndex, float & rCoordX, float & rCoordY) const;

			float		GetValueLinear(float rX) const;
			float		GetValueCubic(float rX) const;


		private:

			struct	INTERPOLATION_POINT
			{
				float	_rX;
				float	_rY;
			};
			QDT_VECTOR<INTERPOLATION_POINT>	_InterpolationPoints;
			float							_rStartValue;
			float							_rEndValue;
			bool							_bHasMin;
			bool							_bHasMax;
			float							_rMin;
			float							_rMax;

			static	const float				EPSILON;

			typedef	QDT_VECTOR<INTERPOLATION_POINT>::ITERATOR	MAP_IT;
		};
	}
}

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_INTERPOLATION_MAPPING_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
