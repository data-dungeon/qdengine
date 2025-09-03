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
//	CLASS:	TRACK_INDEXES
//	The TRACK_INDEXES class implements ...
//
//	02-01-14:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TRACK_INDEXES_H__
#define __M3D_TRACK_INDEXES_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace M3D
	{
		class TRACK_INDEXES
		{
		public:

			/** Constructor & modifiors */
			explicit			TRACK_INDEXES();
			TRACK_INDEXES(const TRACK_INDEXES & TrackIndexes);
			~TRACK_INDEXES();

			TRACK_INDEXES &		operator = (const TRACK_INDEXES & TrackIndexes);
			void				Reset();

			/** Accessors */
			_INLINE_ UInt32		GetOldRotIndex() const			{ return (_nOldRotIndex);		} 
			_INLINE_ UInt32		GetRotIndex() const				{ return (_nRotIndex);			}
			_INLINE_ UInt32		GetOldTransIndex() const		{ return (_nOldTransIndex);		}	
			_INLINE_ UInt32		GetTransIndex() const			{ return (_nTransIndex);		}
			_INLINE_ UInt32		GetOldScaleIndex() const		{ return (_nOldScaleIndex);		}
			_INLINE_ UInt32		GetScaleIndex() const			{ return (_nScaleIndex);		}
			_INLINE_ UInt32		GetOldAnimTopIndex() const		{ return (_nOldAnimTopIndex);	}
			_INLINE_ UInt32		GetAnimTopIndex() const			{ return (_nAnimTopIndex);		}

			/** Modifiors */
			_INLINE_ void				SetOldRotIndex(UInt32 nIndex)			{ _nOldRotIndex = nIndex;		}
			_INLINE_ void				SetRotIndex(UInt32 nIndex)			{ _nRotIndex = nIndex;			}		
			_INLINE_ void				SetOldTransIndex(UInt32 nIndex)		{ _nOldTransIndex = nIndex;		}
			_INLINE_ void				SetTransIndex(UInt32 nIndex)			{ _nTransIndex = nIndex;		}
			_INLINE_ void				SetOldScaleIndex(UInt32 nIndex)		{ _nOldScaleIndex = nIndex;		}
			_INLINE_ void				SetScaleIndex(UInt32 nIndex)			{ _nScaleIndex = nIndex;		}
			_INLINE_ void				SetOldAnimTopIndex(UInt32 nIndex)		{ _nOldAnimTopIndex = nIndex;	}
			_INLINE_ void				SetAnimTopIndex(UInt32 nIndex)		{ _nAnimTopIndex = nIndex;		}

		private:

			void				Copy(const TRACK_INDEXES & ItHolder);

			UInt32		_nRotIndex;
			UInt32		_nOldRotIndex;	
			UInt32		_nTransIndex; 
			UInt32		_nOldTransIndex; 	
			UInt32		_nScaleIndex; 
			UInt32		_nOldScaleIndex;
			UInt32		_nAnimTopIndex;
			UInt32		_nOldAnimTopIndex;
		};
	}
}

#endif // __M3D_TRACK_INDEXES_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
