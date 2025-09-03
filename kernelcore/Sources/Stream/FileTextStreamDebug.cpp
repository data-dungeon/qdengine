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
//	CLASS:	FILE_TEXT_STREAM_DEBUG
//
//	02-02-13:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Stream/FileTextStreamDebug)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FILE_TEXT_STREAM_DEBUG constructor
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
FILE_TEXT_STREAM_DEBUG::FILE_TEXT_STREAM_DEBUG()
:
FILE_TEXT_STREAM()
{
}

//-----------------------------------------------------------------------------
//	Name:		FILE_TEXT_STREAM_DEBUG constructor
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
FILE_TEXT_STREAM_DEBUG::FILE_TEXT_STREAM_DEBUG(const char *	pFilename,
											   int			nAccessType)
:
FILE_TEXT_STREAM(pFilename, nAccessType)
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-01-19:	ELE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM_DEBUG::operator=(const unsigned int	s)
{
	QDT_ASSERT(_nFlags & READ);	
	if (_nFlags & WRITE)
	{
		FlushFile();
	}
	_nOffset = s;
	QDT_ASSERT(_nOffset <= _nSize);
	FillBuffer();
	return *this;
}

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM_DEBUG::DebugOutput(const char *	pTxt)
{
	while (*pTxt != 0)
	{
		SetChars(pTxt, 1);
		pTxt++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM_DEBUG::DebugOutput(unsigned char	Char)
{
	*this << Char;
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM_DEBUG::DebugOutput(int	Int)
{
	*this << Int;
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	03-05-21:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM_DEBUG::DebugOutput(float	Float)
{
	*this << Float;
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutputBinary
//	Object:		
//	02-02-14:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM_DEBUG::DebugOutputBinary(int	Int)
{
	char	Bin[35] = "0b";

	if	(Int & 0x80000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x40000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x20000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x10000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x08000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x04000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x02000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x01000000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00800000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00400000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00200000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00100000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00080000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00040000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00020000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00010000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00008000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00004000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00002000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00001000)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000800)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000400)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000200)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000100)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000080)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000040)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000020)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000010)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000008)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000004)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000002)	strcat(Bin, "1");	else	strcat(Bin, "0");
	if	(Int & 0x00000001)	strcat(Bin, "1");	else	strcat(Bin, "0");

	DebugOutput(Bin);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		IsFileTextStreamDebug
//	Object:		
//	02-02-14:	ELE - Created
//-----------------------------------------------------------------------------
bool	FILE_TEXT_STREAM_DEBUG::IsFileTextStreamDebug()
{
	return	(true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
