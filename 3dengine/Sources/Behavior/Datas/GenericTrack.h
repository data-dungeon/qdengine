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
//	CLASS:	GENERIC_TRACK
//	The GENERIC_TRACK class implements ...
//
//	02-06-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_GENERIC_TRACK_H__
#define __M3D_GENERIC_TRACK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Template/QDT_Vector)
#include INCL_KMATH(Math/Math)
#include INCL_KMATH(Math/numerical analysis/interpolation/interpolation)
#include INCL_KMATH(Math/LinearAlgebra/LinearSystem)
#include INCL_3DENGINE(Behavior/Datas/GenericKey)

namespace QDT
{
	namespace M3D
	{
		template <class T>
		class	GENERIC_TRACK
		{
		public:

			/** Constructor */
			GENERIC_TRACK()
				:
			_pH(NULL),
				_pF2(NULL),
				_pPredictedCoeff1(NULL),
				_pPredictedCoeff2(NULL),
				_rLength(0.0f)
			{
				_nLastIndiceUsed	= 0;
				_bInitBezier		= false;
			}

			/** Copy constructor */
			GENERIC_TRACK(const GENERIC_TRACK & Track)
			{
				Copy(Track);
			}

			/** Destructor */
			~GENERIC_TRACK()
			{/*
			 // Do nothing
			 if	( _pH )
			 {
			 delete[] _pH;
			 }

			 if	( _pF2 )
			 {
			 delete[] _pF2;
			 }

			 if	( _pPredictedCoeff1 )
			 {
			 delete[] _pPredictedCoeff1;
			 }

			 if	( _pPredictedCoeff2 )
			 {
			 delete[] _pPredictedCoeff2;
			 }*/
			}

			/** Operator = */
			GENERIC_TRACK &					operator = (const GENERIC_TRACK & Track)
			{
				if (this != &Track)
				{
					Copy(Track);
				}

				return (*this);
			}

			/** Push a new key */
			void							PushKey(const GENERIC_KEY<T> &Key)
			{
				if (GetNbrKeys() == 0)
				{
					_vKeys.PushTail(Key);
					_rLength = Key.GetTime();
					return;
				}

				QDT_ASSERT(_vKeys[GetNbrKeys() -1].GetTime() <= Key.GetTime());

				if (_vKeys[GetNbrKeys() -1].GetTime() <= Key.GetTime())
				{
					_vKeys.PushTail(Key);
					_rLength = Key.GetTime();
				}
			}

			/** Clear the keys */
			void							ClearKeys()						
			{ 
				_vKeys.Clear(); 
			}

			/** Destroy the vector */
			void							Destroy()
			{
				_vKeys.Destroy();
			}

			/** Get the number of keys */
			unsigned int					GetNbrKeys() const				{ return (_vKeys.GetSize()); }

			/** Get the Length of the animation */
			float							GetLength() const				{ return (_rLength); }

			/** Get key */
			GENERIC_KEY<T>					GetKey(unsigned int n) const	{ return (_vKeys[n]); }

			/** Get the value of the nearest key under rTime */
			T								GetPureValue(float rTime) const
			{
				QDT_ASSERT(GetNbrKeys() > 0);

				if (rTime <= _vKeys[0].GetTime())				return (_vKeys[0].GetValue());
				if (rTime >= _vKeys[GetNbrKeys() - 1].GetTime()) return (_vKeys[GetNbrKeys() - 1].GetValue());

				unsigned int nVal1, nVal2;
				GetInterval(rTime, nVal1, nVal2);

				return (_vKeys[nVal1].GetValue());
			}

			/** Get the value determined by linear interpolation with rTime */
			T								GetLinearValue(float rTime) const
			{
				QDT_ASSERT(GetNbrKeys() > 0);

				if (rTime <= _vKeys[0].GetTime())				return (_vKeys[0].GetValue());
				if (rTime >= _vKeys[GetNbrKeys() - 1].GetTime()) return (_vKeys[GetNbrKeys() - 1].GetValue());

				T tRes;
				unsigned int nVal1, nVal2;
				GetInterval(rTime, nVal1, nVal2);

				float	rInterCoeff = GetLinearInterpolationCoefficient(rTime, nVal1, nVal2);

				tRes = _vKeys[nVal1].GetValue() +  (_vKeys[nVal2].GetValue() - _vKeys[nVal1].GetValue()) * rInterCoeff;	

				return (tRes);
			}

			/** Get the value determined by bezier (order 2) interpolation with rTime */
			T								GetBezierValue(float rTime) const
			{
				// Need at least three keys
				QDT_ASSERT(GetNbrKeys() > 2);

				if (rTime <= _vKeys[0].GetTime())				return (_vKeys[0].GetValue());
				if (rTime >= _vKeys[GetNbrKeys() - 1].GetTime()) return (_vKeys[GetNbrKeys() - 1].GetValue());

				T tRes;
				unsigned int nVal1, nVal2, nVal3;
				GetInterval(rTime, nVal1, nVal2);
				nVal3 = nVal2 + 1;

				if (nVal3 >= GetNbrKeys())
				{
					--nVal1;
					--nVal2;
					--nVal3;
				}

				QDT_ASSERT(nVal1 >= 0);

				// Find control point P										
				T		tM;	// Point between A & B
				tM = _vKeys[nVal1].GetValue() + (_vKeys[nVal3].GetValue() - _vKeys[nVal1].GetValue()) / 2.0f;

				T		tP;
				tP = _vKeys[nVal2].GetValue() + (_vKeys[nVal2].GetValue() - tM);

				float rT = (rTime - _vKeys[nVal1].GetTime()) / (_vKeys[nVal3].GetTime()  - _vKeys[nVal1].GetTime());

				QDT_ASSERT(rT >= 0.0f && rT <= 1.0f);

				// Adjust time to match middle key with right time value
				float rMT = (_vKeys[nVal2].GetTime() - _vKeys[nVal1].GetTime()) / (_vKeys[nVal3].GetTime() - _vKeys[nVal1].GetTime());
				QDT_ASSERT(rMT > 0);

				if (rT <= 0.5f) 
				{
					rT = rT * rMT / 0.5f;
				}
				else	// rt > 0.5f
				{
					rT = (rT - 0.5f) * (1.0 - rMT) / 0.5 + rMT;
				}

				tRes = (_vKeys[nVal1].GetValue() * (1 - rT) * (1 - rT)) + 
					(tP * (2 * rT) * (1 - rT))  +
					(_vKeys[nVal3].GetValue() * rT * rT);

				return (tRes);
			}

			// Builds the elements ( _pH array, _pF2 array and
			// constant coefficients to optimize speed ) 
			// necessary to compute a value by a cubic spline
			// interpolation. At least 4 keys must be set
			void							InitBezierCubicInterpolation()
			{
				QDT_ASSERT( GetNbrKeys() > 3 );

				float			*b;
				int				nLin, nCol;
				int				nKeyIdx;
				LINEAR_SYSTEM	L(GetNbrKeys() - 2);
				float			**M;
				float			*S;

				if	( _pH )
				{
					delete[] _pH;
				}

				if	( _pF2 )
				{
					delete[] _pF2;
				}

				if	( _pPredictedCoeff1 )
				{
					delete[] _pPredictedCoeff1;
				}

				if	( _pPredictedCoeff2 )
				{
					delete[] _pPredictedCoeff2;
				}

				// Allocate arrays
				_pH					= new float[GetNbrKeys()];
				_pF2				= new float[GetNbrKeys()];
				b					= new float[GetNbrKeys()];
				_pPredictedCoeff1	= new float[GetNbrKeys()];
				_pPredictedCoeff2	= new float[GetNbrKeys()];

				// Allocate Linear System Matrix
				M	= new float*[GetNbrKeys()-2];

				for	( nLin = 0; nLin < GetNbrKeys()-2; ++nLin )
				{
					M[nLin] = new float[GetNbrKeys()-2];
				}

				// Initialize h(i) array with h(i) = t(i) - t(i-1)
				for	( nKeyIdx = 1; nKeyIdx < GetNbrKeys(); ++nKeyIdx )
				{
					_pH[nKeyIdx] = _vKeys[nKeyIdx].GetTime() + (- _vKeys[nKeyIdx-1].GetTime());
				}

				// Initialize b(i) array. It is the solution of the Linear System
				for	( nKeyIdx = 1; nKeyIdx < GetNbrKeys() - 1; ++nKeyIdx )
				{
					b[nKeyIdx]	= 
						( 6.0 / _pH[nKeyIdx+1] ) * ( _vKeys[nKeyIdx + 1].GetValue() - _vKeys[nKeyIdx].GetValue() ) 
						+ ( 6.0 / _pH[nKeyIdx] ) * ( _vKeys[nKeyIdx - 1].GetValue() - _vKeys[nKeyIdx].GetValue() );
				}


				// Zero Matrix
				for	( nLin = 0; nLin < GetNbrKeys()-2; ++nLin )
				{
					for	( nCol = 0; nCol < GetNbrKeys()-2; ++nCol )
					{
						M[nLin][nCol] = 0.0;
					}

				}

				// Build Matrix and solution Vector for Linear System
				for	( nLin = 1; nLin < GetNbrKeys()-1; ++nLin )
				{
					M[nLin-1][nLin-1] = 2.0 * ( _pH[nLin] + _pH[nLin+1] );
					L.SetVectorCoeff( nLin-1, b[nLin] );
				}

				// Build Matrix for Linear System
				for	( nLin = 1; nLin < GetNbrKeys()-2; ++nLin )
				{
					M[nLin-1][nLin] = _pH[nLin+1];
					M[nLin][nLin-1] = _pH[nLin+1];
				}

				// Assign Matrix to Linear System 
				// and get solution
				S = new float[GetNbrKeys()-2];

				L.SetMatrix( M );
				L.Solve( S, true ); // No Determinant

				// Build _pF2 array. It represents the second derivates values
				// of the spline on the interpolation point.
				// The two first conditions define a natural spline and
				// are necessary for the spline resolution
				_pF2[0]				= 0.0;
				_pF2[GetNbrKeys()-1]	= 0.0;
				for	( nLin = 1; nLin < GetNbrKeys()-1; ++nLin )
				{
					_pF2[nLin] = S[nLin-1];
				}

				for	( nLin = 1; nLin < GetNbrKeys(); ++nLin )
				{
					_pPredictedCoeff1[nLin] = ( _vKeys[nLin-1].GetValue() / _pH[nLin] - _pF2[nLin-1] * _pH[nLin] / 6.0 );
					_pPredictedCoeff2[nLin] = ( _vKeys[nLin].GetValue() / _pH[nLin] - _pF2[nLin] * _pH[nLin] / 6.0 );
				}

				for	( nLin = 0; nLin < GetNbrKeys()-2; ++nLin )
				{
					delete[] M[nLin];
				}

				delete[] M;
				delete[] b;
				delete[] S;

				_bInitBezier = true;
			}

			// Get the value by a cubic spline interpolation with rTime
			// InitBezierCubicInterpolation must be called before using
			// this method
			T								GetBezierCubicValue(float rTime) const
			{
				QDT_ASSERT( _bInitBezier == true );

				// Check interpolation limits
				if (rTime <= _vKeys[0].GetTime())				
				{
					return (_vKeys[0].GetValue());
				}

				if (rTime >= _vKeys[GetNbrKeys() - 1].GetTime()) 
				{
					return (_vKeys[GetNbrKeys() - 1].GetValue());
				}

				int nVal = 1;

				// Find the interpolation interval
				while	( rTime > _vKeys[nVal].GetTime() )
				{
					++nVal;
				}

				T f;

				// Compute the T value using precomputed arrays
				f = _pF2[nVal-1] * ( _vKeys[nVal].GetTime() - rTime ) * ( _vKeys[nVal].GetTime() - rTime ) * ( _vKeys[nVal].GetTime() - rTime ) / ( 6.0 * _pH[nVal] )
					+   _pF2[nVal] * ( rTime - _vKeys[nVal-1].GetTime() ) * ( rTime - _vKeys[nVal-1].GetTime() ) * ( rTime - _vKeys[nVal-1].GetTime() ) / ( 6.0 * _pH[nVal] )
					+   _pPredictedCoeff1[nVal] * ( _vKeys[nVal].GetTime() - rTime )
					+   _pPredictedCoeff2[nVal] * ( rTime  - _vKeys[nVal-1].GetTime() );

				return	f;
			}

			/** Get the value determined by Hermite Spline interpolation with rTime */
			T								GetHermiteValue(float rTime) const
			{
				QDT_ASSERT(GetNbrKeys() > 1);

				if (rTime <= _vKeys[0].GetTime())				return (_vKeys[0].GetValue());
				if (rTime >= _vKeys[GetNbrKeys() - 1].GetTime()) return (_vKeys[GetNbrKeys() - 1].GetValue());

				T tRes;
				unsigned int nVal1, nVal2;
				GetInterval(rTime, nVal1, nVal2);

				float	rHermiteCoeff	= ( rTime						- GetKey( nVal1 ).GetTime() ) 
					/ ( GetKey( nVal2 ).GetTime()	- GetKey( nVal1 ).GetTime() );

				tRes = INTERPOLATION<T>::HermiteInterpolation(	GetKey( nVal1 ).GetValue(), 
					GetKey( nVal2 ).GetValue(), 
					GetKey( nVal1 ).GetOutTangent(), 
					GetKey( nVal2 ).GetInTangent(), 
					rHermiteCoeff );

				return (tRes);
			}

			/** Get the value determined by SLERP interpolation (for quaternions) with rTime */
			T								GetSlerpValue(float rTime) const
			{
				QDT_ASSERT(GetNbrKeys() > 0);

				if (rTime <= _vKeys[0].GetTime())				return (_vKeys[0].GetValue());
				if (rTime >= _vKeys[GetNbrKeys() - 1].GetTime()) return (_vKeys[GetNbrKeys() - 1].GetValue());

				T tRes;
				unsigned int nVal1, nVal2;
				GetInterval(rTime, nVal1, nVal2);

				float rInterCoeff = GetLinearInterpolationCoefficient(rTime, nVal1, nVal2);

				tRes.InterSLERP(rInterCoeff, _vKeys[nVal1].GetValue(), _vKeys[nVal2].GetValue());

				return (tRes);
			}

			/** Get the value determined by random time between two only keys in vector*/
			T								GetRandomValue(float rTime) const
			{
				QDT_ASSERT(GetNbrKeys() == 2);

				T tRes;
				float rFirstTime = _vKeys[0].GetTime();
				float rSecondTime = _vKeys[1].GetTime();

				rTime = MATH::Rand(rSecondTime-rFirstTime) - rFirstTime;

				unsigned int nVal1, nVal2;
				GetInterval(rTime, nVal1, nVal2);

				float	rInterCoeff = GetLinearInterpolationCoefficient(rTime, nVal1, nVal2);

				tRes = _vKeys[nVal1].GetValue() +  (_vKeys[nVal2].GetValue() - _vKeys[nVal1].GetValue()) * rInterCoeff;	

				return (tRes);
			}

			/** Operator << */
			friend IO_STREAM&				operator << (IO_STREAM & Stream, const GENERIC_TRACK<T> & Track)
			{
				unsigned int nEachKey, nNbrKeys = Track.GetNbrKeys();
				Stream << nNbrKeys;
				for (nEachKey = 0 ; nEachKey < nNbrKeys ; ++nEachKey)
				{
					Stream << Track.GetKey(nEachKey);
				}

				return (Stream);
			}

			/** Operator >> */
			friend IO_STREAM&				operator >> (IO_STREAM & Stream, GENERIC_TRACK<T> & Track)
			{
				ClearKeys();

				GENERIC_KEY <T> K;
				unsigned int nEachKey, nNbrKeys;
				Stream >> nNbrKeys;
				for (nEachKey = 0 ; nEachKey < nNbrKeys ; ++nEachKey)
				{
					Stream >> K;
					Track.PushKey(K);
				}

				return (Stream);
			}

			//private:

			/** Copy */
			void							Copy (const GENERIC_TRACK & Track)
			{
				_vKeys = Track._vKeys;
			}

			/** Get interval */
			void							GetInterval(float		   rTime, 
				unsigned int & nValOut1, 
				unsigned int & nValOut2) const
			{
				nValOut1 = 0;
				nValOut2 = 0;

				QDT_ASSERT(GetNbrKeys() > 1);

				if (rTime < _vKeys[_nLastIndiceUsed].GetTime()) 
				{
					_nLastIndiceUsed = 0;
				}

				float rTime1, rTime2;

				unsigned int nIndice, nNbrIndices = GetNbrKeys() - 1;
				for (nIndice = _nLastIndiceUsed ; nIndice < nNbrIndices ; ++nIndice)
				{
					rTime1 = _vKeys[nIndice].GetTime();
					rTime2 = _vKeys[nIndice + 1].GetTime();

					if (rTime1 <= rTime && rTime <= rTime2)
					{
						nValOut1 = nIndice;
						nValOut2 = nIndice + 1;
						return;
					}
				}

				QDT_FAIL();
			}

			/** Compute the interpolation coeff from a time & an interval */
			float							GetLinearInterpolationCoefficient(float			rTime, 
				unsigned int	nVal1, 
				unsigned int	nVal2) const
			{
				return ((rTime - _vKeys[nVal1].GetTime())
					/
					(_vKeys[nVal2].GetTime() - _vKeys[nVal1].GetTime()));
			}

			QDT_VECTOR <GENERIC_KEY<T> >	_vKeys;

			// 
			float							*_pH;
			float							*_pF2;
			float							*_pPredictedCoeff1;
			float							*_pPredictedCoeff2;
			float							_rLength;	// In ms
			bool							_bInitBezier;

			// Used for optimization
			mutable unsigned int			_nLastIndiceUsed;
		};
	}
}

#endif // #define __M3D_GENERIC_TRACK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
