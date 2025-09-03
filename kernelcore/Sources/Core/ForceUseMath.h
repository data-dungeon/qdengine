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
//	CLASS:	MACROS
//	The MACROS class implements ...
//
//	05-09-21:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#define	cos(a)			NO_COS_LINKED_USE_MATH_Cos
#define	cosf(a)			NO_COSF_LINKED_USE_MATH_Cos
#define	sin(a)			NO_SIN_LINKED_USE_MATH_Sin
#define	sinf(a)			NO_SINF_LINKED_USE_MATH_Sin
#define	acos(a)			NO_ACOS_LINKED_USE_MATH_ACos
#define	acosf(a)		NO_ACOSF_LINKED_USE_MATH_ACos
#define	tan(a)			NO_TAN_LINKED_USE_MATH_Tan
#define	tanf(a)			NO_TANF_LINKED_USE_MATH_Tan
#define	atan(a)			NO_ATAN_LINKED_USE_MATH_ATan
#define	atanf(a)		NO_ATANF_LINKED_USE_MATH_ATan
#define	atan2(a,b)		NO_ATAN2_LINKED_USE_MATH_ATan
#define	atan2f(a,b)		NO_ATAN2F_LINKED_USE_MATH_ATan

#define	log(a)			NO_LOG_LINKED_USE_MATH_Ln
#define	logf(a)			NO_LOGF_LINKED_USE_MATH_Ln
#define	log10(a)		NO_LOG10_LINKED_USE_MATH_Log10
#define	log10f(a)		NO_LOG10F_LINKED_USE_MATH_Log10

#define	exp(a)			NO_EXP_LINKED_USE_MATH_Exp
#define	expf(a)			NO_EXPF_LINKED_USE_MATH_Exp
#define	sqrt(a)			NO_SQRT_LINKED_USE_MATH_Sqrt
#define	sqrtf(a)		NO_SQRTF_LINKED_USE_MATH_Sqrt
#define	abs(a)			NO_ABS_LINKED_USE_MATH_Abs
#define	fabs(a)			NO_FABS_LINKED_USE_MATH_Abs
#define	fabsf(a)		NO_FABSF_LINKED_USE_MATH_Abs

#define	floor(a)		NO_FLOOR_LINKED_USE_MATH_Floor
#define	floorf(a)		NO_FLOORF_LINKED_USE_MATH_Floor
#define	pow(a)			NO_POW_LINKED_USE_MATH_Pow	
#define	powf(a)			NO_POWF_LINKED_USE_MATH_Pow

#define	rand()			NO_RAND_LINKED_USE_MATH_Rand
#define	srand()			NO_SRAND_LINKED_USE_MATH_Rand	

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
