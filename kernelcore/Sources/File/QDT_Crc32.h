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
//	CLASS:	QDT_CRC_32
//	The QDT_CRC_32 class implements ...
//
//	02-12-09:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_FILE_QDT_CRC32_H__
#define __KCORE_FILE_QDT_CRC32_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class	QDT_CRC_32
		{
		public:
			enum
			{
				// Read 4K of data at a time
				MAX_BUFFER_SIZE = 4096
			};

			enum CRC_ERRORS
			{
				CE_UNKNOWN_ERROR = 0,
				CE_NO_ERROR,
				CE_ERROR_CRC,
				CE_ERROR_OPEN_FILE,
				CE_ERROR_TABLE_NOT_INIT,
				CE_LAST_CRC_ERROR
			};

			QDT_CRC_32();
			~QDT_CRC_32();

			CRC_ERRORS	StringCrc32(const char * pString, unsigned int &nCrc32);
			CRC_ERRORS	FileCrc32(const char * pFileName, unsigned int &nCrc32);
			CRC_ERRORS	BufferCrc32(const void * pBuffer, unsigned int nSize, unsigned int &nCrc32);
			
			void		Init(void);
			void		Free(void);

		protected:

			//	HIDDEN STUFF
			explicit QDT_CRC_32(const QDT_CRC_32 & C);
			QDT_CRC_32 &	operator=(const QDT_CRC_32 & C);
			void	Copy(const QDT_CRC_32 & C);

			
			void	CalcCrc32(const unsigned char cByte, unsigned int &nCrc32) const;

		private:
			unsigned int *	_pCrc32Table;
		};
	}
}

#endif //__KCORE_FILE_QDT_CRC32_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
