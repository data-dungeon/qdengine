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
//	CLASS:	STREAM_TRACK_HEADER
//	The STREAM_TRACK_HEADER class implements ...
//
//	03-05-28:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_TRACK_HEADER_H__
#define __M3D_STREAM_TRACK_HEADER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KMATH(Math/Geometry/Quaternions/Quaternion)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_TRACK_HEADER
		{
		public:

			//////////////////////////////////////////////
			STREAM_TRACK_HEADER();
			explicit STREAM_TRACK_HEADER(const STREAM_TRACK_HEADER & C);
			~STREAM_TRACK_HEADER();

			STREAM_TRACK_HEADER&	operator=(const STREAM_TRACK_HEADER & C);
			void					Copy(const STREAM_TRACK_HEADER & C);
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			_INLINE_	const	QUATERNION&		GetAbsRot()const;
			_INLINE_	const	VECTOR&			GetAbsTrans()const;
			_INLINE_	const	VECTOR&			GetAbsScale()const;
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			_INLINE_	void	SetAbsRot( const QUATERNION& Q );
			_INLINE_	void	SetAbsTrans( const VECTOR& V );
			_INLINE_	void	SetAbsScale( const VECTOR& S );
			//////////////////////////////////////////////

			//////////////////////////////////////////////
			/** Read-write */
			void						Read(IO_STREAM &s);
			void						Write(IO_STREAM &s);
			friend IO_STREAM&			operator << (IO_STREAM & Stream, STREAM_TRACK_HEADER& STH)
			{
				STH.Write(Stream);
				return (Stream);
			}

			friend IO_STREAM&			operator >> (IO_STREAM & Stream,  STREAM_TRACK_HEADER& STH)
			{
				STH.Read(Stream);
				return (Stream);
			}
			//////////////////////////////////////////////

		protected:

			//////////////////////////////////////////////
			QUATERNION			_AbsoluteRot;
			VECTOR				_AbsoluteTrans;
			VECTOR				_AbsoluteScale;
			//////////////////////////////////////////////


		private:

			//////////////////////////////////////////////
			//////////////////////////////////////////////
		};
	}
}

#ifndef _DEBUG
	#include "StreamTrackHeader.inl"
#endif

#endif // __M3D_STREAM_TRACK_HEADER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
