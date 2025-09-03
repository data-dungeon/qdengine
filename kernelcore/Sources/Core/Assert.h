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
//	File	:	Assert.h
//	Category:	KernelCore/Sources/Core/
//
//	05-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_CORE_ASSERT_H__
#define __KCORE_CORE_ASSERT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

/*
	ASSERT HELP:

	QDT_ASSERT(condition) :					Rise an error if condition failed (only in QDT_DEBUG)
	QDT_ASSERT_MSG(condition, message) :	Same as QDT_ASSERT but the message can be configured
	QDT_FAIL() :							Raise an unconditional Error
	QDT_NOT_IMPLEMENTED() :					Same as QDT_FAIL but used to indicate that the case has to be managed later
	QDT_VERIFY(code) :						The code is executed in all configuration but the return code is only checked in QDT_DEBUG
	QDT_ENSURE() :							Same as QDT_ASSERT but the test is done and the error is Raised in both DEBUG and RELEASE mode
*/

namespace QDT
{
	namespace KCORE
	{
		/*
		//disabled because of ASSERT macro redefinition in Microsoft's includes :(
		#undef	ASSERT
		#undef	_ASSERT
		#define	ASSERT				NO_ASSERT_USE_QDT_ASSERT
		#define	_ASSERT				NO_ASSERT_USE_QDT_ASSERT
		*/
		
		#ifdef QDT_DEBUG
			#define QDT_USE_BREAK
		#endif

	    // /!\ use like this : QDT_ASSERT_TRACE(bCond, ("format %d %s...", 3, "toto"))
		#define QDT_ASSERT_TRACE(title, condition, parenthese_message)\
		do \
		{\
			static bool zzzbAssertDontCare= false;	\
			if(!(condition) && !zzzbAssertDontCare)\
			{\
				::QDT_TraceAssertFunc parenthese_message;\
				switch(::QDT_AssertFunc(title, #condition, __FILE__, __LINE__, &zzzbAssertDontCare))\
				{\
				case 1: QDT_BREAK;break;\
				case 2: QDT_FORCE_BREAK;break;\
				}\
			}\
		} while(0)

		#ifdef	QDT_DEBUG
		#	if (defined (PLATFORM_PS3) && !defined (SPU))
		#		define	QDT_ASSERT(condition)					QDT_ASSERT_TRACE("Assert", 			condition,	("ASSERTION FAILED !"));
		#		define	QDT_ASSERT_MSG(condition, message)		QDT_ASSERT_TRACE("Assert", 			condition,	message);
		#		define	QDT_VERIFY(condition)					QDT_ASSERT_TRACE("Verify", 			condition,	("VERIFY FAILED !"));
		#		define	QDT_FAIL()								QDT_ASSERT_TRACE("Fail", 			false,		("FAIL !"));
		#		define	QDT_NOT_IMPLEMENTED()					QDT_ASSERT_TRACE("Not Implemented", false,		("CODE IS NOT IMPLEMENTED !"));
		#		define	QDT_OBSOLETE(message)					QDT_ASSERT_TRACE("Obsolete",		false,		(message));
		#		define	QDT_ENSURE(condition)					QDT_ASSERT_TRACE("Ensure", 			condition,	("ENSURE FAILED !"));
		#		define	QDT_ENSURE_MSG(condition, message)		QDT_ASSERT_TRACE("Ensure", 			condition,	(message));
		#	elif (defined (PLATFORM_PS3) && defined (SPU))
		#		define	QDT_ASSERT(condition)					
		#		define	QDT_ASSERT_MSG(condition, message)		
		#		define	QDT_VERIFY(condition)					
		#		define	QDT_FAIL()								
		#		define	QDT_NOT_IMPLEMENTED()					
		#		define	QDT_OBSOLETE(message)					
		#		define	QDT_ENSURE(condition)					
		#		define	QDT_ENSURE_MSG(condition, message)		
		#	elif defined (PLATFORM_PC)
		#		define	QDT_ASSERT(condition)					QDT_ASSERT_TRACE("Assert", 			condition,	("ASSERTION FAILED !"));
		#		define	QDT_ASSERT_MSG(condition, message)		QDT_ASSERT_TRACE("Assert", 			condition,	message);
		#		define	QDT_VERIFY(condition)					QDT_ASSERT_TRACE("Verify", 			condition,	("VERIFY FAILED !"));
		#		define	QDT_FAIL()								QDT_ASSERT_TRACE("Fail", 			false,		("FAIL !"));
		#		define	QDT_NOT_IMPLEMENTED()					QDT_ASSERT_TRACE("Not Implemented", false,		("CODE IS NOT IMPLEMENTED !"));
		#		define	QDT_OBSOLETE(message)					QDT_ASSERT_TRACE("Obsolete",		false,		(message));
		#		define	QDT_ENSURE(condition)					QDT_ASSERT_TRACE("Ensure", 			condition,	("ENSURE FAILED !"));
		#		define	QDT_ENSURE_MSG(condition, message)		QDT_ASSERT_TRACE("Ensure", 			condition,	(message));
		#	elif defined (PLATFORM_X360)
		#		define	QDT_ASSERT(condition)					QDT_ASSERT_TRACE("Assert", 			condition,	("ASSERTION FAILED !"));
		#		define	QDT_ASSERT_MSG(condition, message)		QDT_ASSERT_TRACE("Assert", 			condition,	message);
		#		define	QDT_VERIFY(condition)					QDT_ASSERT_TRACE("Verify", 			condition,	("VERIFY FAILED !"));
		#		define	QDT_FAIL()								QDT_ASSERT_TRACE("Fail", 			false,		("FAIL !"));
		#		define	QDT_NOT_IMPLEMENTED()					QDT_ASSERT_TRACE("Not Implemented", false,		("CODE IS NOT IMPLEMENTED !"));
		#		define	QDT_OBSOLETE(message)					QDT_ASSERT_TRACE("Obsolete",		false,		(message));
		#		define	QDT_ENSURE(condition)					QDT_ASSERT_TRACE("Ensure", 			condition,	("ENSURE FAILED !"));
		#		define	QDT_ENSURE_MSG(condition, message)		QDT_ASSERT_TRACE("Ensure", 			condition,	(message));
		#	else
		#		error 	PLATFORM undefined!!
		#	endif
		#else
		#	define	QDT_ASSERT(condition)
		#	define	QDT_ASSERT_MSG(condition, message)
		#	define	QDT_VERIFY(condition)						(condition)
		#	define	QDT_FAIL()
		#	define	QDT_NOT_IMPLEMENTED()
		#	define	QDT_OBSOLETE(message)
		#	ifndef _MASTER
		#		define	QDT_ENSURE(condition)					QDT_ASSERT_TRACE("Ensure", condition, ("ENSURE FAILED !"));
		#		define	QDT_ENSURE_MSG(condition, message)		QDT_ASSERT_TRACE("Ensure", condition, (message));
		#	else
		#		define	QDT_ENSURE(condition)
		#		define	QDT_ENSURE_MSG(condition, message)
		#	endif
		#endif

		#define QDT_OFFSET_OF(type, member)						((int) &(((type *)NULL)->member))
		#define QDT_ASSERT_MEMBER_ALIGNMENT(v,c,condition)		{ if ( (QDT_OFFSET_OF(c,v) % condition) != 0) { QDT::KCORE::QDT_Error("\n%s(%ld): ASSERT_ALIGNMENT ! (need condition padding of %do at the beginning of the class " #c " for alignment of variable " #v " on " #condition "o) \n", __FILE__, __LINE__, ((int)condition) - ((int)(QDT_OFFSET_OF(c,v)%condition))); } }																			
		#define QDT_ASSERT_ADDRESS_ALIGNMENT(v,condition)		{ QDT_ASSERT( (((unsigned int)v) & (0xF)) == (condition)); }

		// ****** QDT_BREAK
		#ifdef PLATFORM_PC
		#	define QDT_FORCE_BREAK __asm { int 3 }
		#elif PLATFORM_PS3
		#	define QDT_FORCE_BREAK 	__asm__ volatile ( "tw 31,1,1" )
		#elif PLATFORM_X360
		#	define QDT_FORCE_BREAK  
		#else
		#	error QDT_FORCE_BREAK is not defined for this platform.
		#endif

		#ifdef QDT_USE_BREAK
		#	define QDT_BREAK  QDT_FORCE_BREAK
		#else
		#	define QDT_BREAK  ((void)0)
		#endif

	} // namespace QDT
} // namespace KCORE

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
