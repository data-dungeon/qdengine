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
//	File	:	TimeCode.h
//
//	01-08-02:	JLT - Created
//*****************************************************************************

#ifndef __KCORE_TIME_TIME_CODE_H__
#define __KCORE_TIME_TIME_CODE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KCORE
	{
		class TIME_CODE 
		{
		private :
			//=============================================================================
			// VARIABLES
			//=============================================================================
			unsigned char _nHours;
			unsigned char _nMinutes;
			unsigned char _nSeconds;
			unsigned char _nCentiseconds;
			int _nFramesCount;
			unsigned int _nNext;
			unsigned int _nPrev;
				
		public :
			//=============================================================================
			// METHODS
			//=============================================================================
			/** */
			TIME_CODE();
			/** */
			TIME_CODE(const TIME_CODE &TC);	
			/** */
			TIME_CODE(	unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi,
						int FramesCount, int DataLength,
						unsigned int nNext, unsigned int nPrev );
			/** NO VIRTUAL */
			~TIME_CODE();
			/** */
			TIME_CODE& operator=(const TIME_CODE& TC);
			/** */
			void SetInfos(	unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi,
							int FramesCount);
			/** */
			void GetInfos(	unsigned char &Hours, unsigned char &Minutes, unsigned char &Seconds, unsigned char &Centi,
							int &FramesCount);
			/** */
			void SetTime(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi);
			/** */
			void GetTime(unsigned char &Hours, unsigned char &Minutes, unsigned char &Seconds, unsigned char &Centi);
			/** in centiseconds */
			unsigned int GetTime() const;
			/** */
			void AddTime(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Centi);
			/** */
			void AddTimeInCentiseconds(int Centiseconds);
			/** */
			void SetNext(unsigned int nNext) { _nNext = nNext; }
			/** */
			unsigned int  GetNext() const { return _nNext; }
			/** */
			void SetPrev(unsigned int nPrev) { _nPrev = nPrev; }
			/** */
			unsigned int GetPrev() const { return _nPrev; }
			/** */
			void SetFramesCount(int FramesCount) { _nFramesCount = FramesCount; }
			/** */
			int GetFramesCount() const { return _nFramesCount; }
			/** */
			void Write(IO_STREAM &s) const;
			/** */
			void Read(IO_STREAM &s);
		};
	}
}

#endif //__KCORE_TIME_TIME_CODE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
