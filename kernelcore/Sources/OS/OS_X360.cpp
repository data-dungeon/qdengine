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
// CLASS: OS
//
//
//00-11-21 ONA Created: 
//*****************************************************************************

#include "Root.h"
#include "OS.h"

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Tools/QdtString)

#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#define		QDT_INCLUDE_X360_XTL
#define		QDT_INCLUDE_X360_XBDM
#define		QDT_INCLUDE_WIN32_EXCEPTIONS
#include	INCL_KCORE(Includes_X360/Includes_X360)

static void* StackTrace[DM_MAX_STACK_DEPTH];

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetCallStack
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
void *	OS::GetCallStack(unsigned int	nLevel)
{
	if ( nLevel == 0 )
	{
		DmCaptureStackBackTrace( DM_MAX_STACK_DEPTH, StackTrace);
	}

	return (StackTrace[nLevel]);
}

//-----------------------------------------------------------------------------
// Name: OS::GetFunctionName
// Object: En fonction d'un a
//00-12-08 ONA Created: 
//-----------------------------------------------------------------------------
const void * const	OS::GetFunctionName( const void * const	pCallStackAddr,
										 char * const		pcBufferFuncName,
										 unsigned int		nSizeBuffer )
{
	QDT_FAIL();
	return	( NULL );
	
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectName
//	Object:		
//	01-07-06:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::GetProjectName(char * const	psBuffer,
						   const int	nSize)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectFileName
//	Object:		
//	01-07-06:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::GetProjectFileName(char * const	psBuffer,
							   const int	nSize)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		GetCursorPos
//	Object:		
//	01-11-12:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::GetCursorPos(int &	nXPos,
						 int &	nYPos)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		SetCursorPos
//	Object:		
//	01-11-12:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::SetCursorPos(const int	nXPos,
						 const int	nYPos)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		GetWinLastErrorMsg
//	Object:		
//	02-05-15:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::GetWinLastErrorMsg(QDT_STRING&	strMsgBuffer)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		GetPlatformString
//	Object:		
//	04-11-23:	DAC - Created
//-----------------------------------------------------------------------------
const char *	OS::GetPlatformString()
{	
	static const char * sPlatformString = "XBox360";	
	return	(sPlatformString);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
