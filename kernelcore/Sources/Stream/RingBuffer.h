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
//	File	:	RingBuffer.h
//
//	01-08-07:	JLT - Created
//*****************************************************************************

/* **************************************************************************** */
/* *                                                                          * */
/* * YOU MUST ASK TO THE RING_BUFFER BEFORE READING IT						  * */
/* *                                                                          * */
/* **************************************************************************** */

#ifndef __KCORE_STREAM_RINGBUFFER_H__
#define __KCORE_STREAM_RINGBUFFER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Stream/MemStream)

namespace QDT
{
	namespace KCORE
	{
		class RING_BUFFER : public MEM_STREAM
		{
		public :
			enum { BASE_SIZE = 512384};
			
		protected :
			//=============================================================================
			// VARIABLES
			//=============================================================================
			UInt32 _nWriteOffset; // offset for writing
			
		public :
			//=============================================================================
			// Constructor & destructor
			//=============================================================================
			/** */
			RING_BUFFER(unsigned long Size = BASE_SIZE);
			/** */
			virtual ~RING_BUFFER();

			//=============================================================================
			// METHODS
			//=============================================================================
			/** */
			void GetWriteBuffer(int Byte2Write, char* *pBuffer1, int *pSize1, char* *pBuffer2, int *pSize2);
			/** */
			bool CanRead(int Byte2Read);
			/** */
			bool CanWrite(int Byte2Write);
			/** */
			void SwitchToReadMode() {  _nFlags &= (0xffff ^ WRITE); _nFlags |= READ; }
			/** */
			void SwitchToWriteMode() { _nFlags &= (0xffff ^ READ); _nFlags |= WRITE; }
			/** */
			void ReserveMemory(int ByteCount);
			/** */
			float GetFillinginPercent() const;
			/** */
			int GetReadOffset()const  { return _nOffset; }
			/** */
			void SetReadOffset(unsigned int offset);
			/** */
			int GetWriteOffset() const { return _nWriteOffset; }
			/** */
			void SetWriteOffset(unsigned int offset);
			/** */
			int GetAvailableSize() const;

			/** */
			IO_STREAM& operator +(const unsigned int);
			/** */
			IO_STREAM& operator -(const unsigned int);
			
		protected :
			//=============================================================================
			// METHODS from MEM_STREAM
			//=============================================================================
			/** */
			virtual void SetChars(const char *pC, int Size);
			/** */
			virtual void GetChars(char *,int);
		};
	}
}

#endif //__KCORE_STREAM_RINGBUFFER_H__
