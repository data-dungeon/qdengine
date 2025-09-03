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
//	CLASS:	VERTEX_SHADER_PSGL
//
//	05-09-20:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "Error.h"	
#define	QDT_INCLUDE_STDIO
#include INCL_KCORE(Includes\Includes)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#define QDT_INCLUDE_PC_CG
#include INCL_KCORE(Includes_PC/Includes_PC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace	QDT
{
	namespace KRENDERER
	{
		//-----------------------------------------------------------------------------
		//	Name:		QDT_TestGLError 
		//	Object:		
		//	05-09-29:	RMA - Created
		//-----------------------------------------------------------------------------
		void QDT_TestGLError(Char * sFileName, 
							 UInt32 nLineNumber)
		{
			GLenum Error = glGetError();

			if (Error != GL_NO_ERROR)
			{ 
				QDT_Message("GL ERROR in file %s at line %d : ", sFileName, nLineNumber);
			
				switch (Error)
				{
				default:
					QDT_Message("UNKNOWN GL ERROR!\n");
					break;

				case GL_INVALID_ENUM:
					QDT_Message("GL_INVALID_ENUM!\n");
					break;

				case GL_INVALID_VALUE:
					QDT_Message("GL_INVALID_VALUE!\n");
					break;

				case GL_INVALID_OPERATION:
					QDT_Message("GL_INVALID_OPERATION!\n");
					break;

				case GL_STACK_OVERFLOW:
					QDT_Message("GL_STACK_OVERFLOW!\n");
					break;

				case GL_STACK_UNDERFLOW:
					QDT_Message("GL_STACK_UNDERFLOW!\n");
					break;

				case GL_OUT_OF_MEMORY:
					QDT_Message("GL_OUT_OF_MEMORY!\n");
					break;
				}
			}
		}

		//-----------------------------------------------------------------------------
		//	Name:		QDT_TestCgError
		//	Object:		
		//	05-09-29:	RMA - Created
		//-----------------------------------------------------------------------------
		void QDT_TestCgError(Char * sFileName, 
							UInt32 nLineNumber)
		{
			CGerror Error = cgGetError();								
			
			if (Error != CG_NO_ERROR)									
			{ 
				QDT_Message("CG ERROR in file %s at line %d : \n", sFileName, nLineNumber);
			
				switch (Error)
				{
				default:
					QDT_Message("CG_UNKNOWN_ERROR!\n");
					break;

				case CG_COMPILER_ERROR: // 1
					QDT_Message("CG_COMPILER_ERROR : The compile returned an error.\n");
					break;

				case CG_INVALID_PARAMETER_ERROR: // 2
					QDT_Message("CG_INVALID_PARAMETER_ERROR : The parameter used is invalid.\n");
					break;

				case CG_INVALID_PROFILE_ERROR: // 3
					QDT_Message("CG_INVALID_PROFILE_ERROR : The profile is not supported.\n");
					break;

				case CG_PROGRAM_LOAD_ERROR: // 4
					QDT_Message("CG_PROGRAM_LOAD_ERROR : The program could not load.\n");
					break;

				case CG_PROGRAM_BIND_ERROR: // 5
					QDT_Message("CG_PROGRAM_BIND_ERROR : The program could not bind.\n");
					break;

				case CG_PROGRAM_NOT_LOADED_ERROR: // 6
					QDT_Message("CG_PROGRAM_NOT_LOADED_ERROR : The program must be loaded before this operation may be used.\n");
					break;

				case CG_UNSUPPORTED_GL_EXTENSION_ERROR: // 7
					QDT_Message("CG_UNSUPPORTED_GL_EXTENSION_ERROR : An unsupported GL extension was required to perform this operation.\n");
					break;

				case CG_INVALID_VALUE_TYPE_ERROR: // 8
					QDT_Message("CG_INVALID_VALUE_TYPE_ERROR : An unknown value type was assigned to a parameter.\n");
					break;

				case CG_NOT_MATRIX_PARAM_ERROR: // 9
					QDT_Message("CG_NOT_MATRIX_PARAM_ERROR : The parameter is not of matrix type.\n");
					break;

				case CG_INVALID_ENUMERANT_ERROR: // 10
					QDT_Message("CG_INVALID_ENUMERANT_ERROR : The enumerant parameter has an invalid value.\n");
					break;

				case CG_NOT_4x4_MATRIX_ERROR: // 11
					QDT_Message("CG_NOT_MATRIX_PARAM_ERROR : The parameter must be a 4x4 matrix type.\n");
					break;

				case CG_FILE_READ_ERROR: // 12
					QDT_Message("CG_FILE_READ_ERROR : The file could not be read.\n");
					break;

				case CG_FILE_WRITE_ERROR: // 13
					QDT_Message("CG_NOT_MATRIX_PARAM_ERROR : The file could not be written.\n");
					break;

				case CG_NVPARSE_ERROR: // 14
					QDT_Message("CG_NVPARSE_ERROR : nvparse could not successfully parse the output from the Cg compiler backend.\n");
					break;

				case CG_MEMORY_ALLOC_ERROR: // 15
					QDT_Message("CG_MEMORY_ALLOC_ERROR : Memory allocation failed.\n");
					break;

				case CG_INVALID_CONTEXT_HANDLE_ERROR: // 16
					QDT_Message("CG_INVALID_CONTEXT_HANDLE_ERROR : Invalid context handle.\n");
					break;

				case CG_INVALID_PROGRAM_HANDLE_ERROR: // 17
					QDT_Message("CG_INVALID_PROGRAM_HANDLE_ERROR : Invalid program handle.\n");
					break;

				case CG_INVALID_PARAM_HANDLE_ERROR: // 18
					QDT_Message("CG_INVALID_PARAM_HANDLE_ERROR : Invalid parameter handle.\n");
					break;

				case CG_UNKNOWN_PROFILE_ERROR: // 19
					QDT_Message("CG_UNKNOWN_PROFILE_ERROR : The specified profile is unknown.\n");
					break;

				case CG_VAR_ARG_ERROR: // 20
					QDT_Message("CG_VAR_ARG_ERROR : The variable arguments were specified incorrectly.\n");
					break;

				case CG_INVALID_DIMENSION_ERROR: // 21
					QDT_Message("CG_INVALID_DIMENSION_ERROR : The dimension value is invalid.\n");
					break;

				case CG_ARRAY_PARAM_ERROR: // 22
					QDT_Message("CG_ARRAY_PARAM_ERROR : The parameter must be an array.\n");
					break;

				case CG_OUT_OF_ARRAY_BOUNDS_ERROR: // 23
					QDT_Message("CG_OUT_OF_ARRAY_BOUNDS_ERROR : Index into the array is out of bounds.\n");
					break;

				case CG_CONFLICTING_TYPES_ERROR: // 24
					QDT_Message("CG_CONFLICTING_TYPES_ERROR : A type being added to the context conflicts with an existing type.\n");
					break;

				case CG_CONFLICTING_PARAMETER_TYPES_ERROR: // 25
					QDT_Message("CG_CONFLICTING_PARAMETER_TYPES_ERROR : The parameters being bound have conflicting types.\n");
					break;

				case CG_PARAMETER_IS_NOT_SHARED_ERROR: // 26
					QDT_Message("CG_PARAMETER_IS_NOT_SHARED_ERROR : The parameter must be global.\n");
					break;

				case CG_INVALID_PARAMETER_VARIABILITY_ERROR: // 27
					QDT_Message("CG_INVALID_PARAMETER_VARIABILITY_ERROR : The parameter could not be changed to the given variability.\n");
					break;

				case CG_CANNOT_DESTROY_PARAMETER_ERROR: // 28
					QDT_Message("CG_CANNOT_DESTROY_PARAMETER_ERROR : Cannot destroy the parameter. It is bound to other parameters or is not a root parameter.\n");
					break;

				case CG_NOT_ROOT_PARAMETER_ERROR: // 29
					QDT_Message("CG_NOT_ROOT_PARAMETER_ERROR : The parameter is not a root parameter.\n");
					break;

				case CG_PARAMETERS_DO_NOT_MATCH_ERROR: // 30
					QDT_Message("CG_PARAMETERS_DO_NOT_MATCH_ERROR : The two parameters being bound do not match.\n");
					break;

				case CG_IS_NOT_PROGRAM_PARAMETER_ERROR: // 31
					QDT_Message("CG_IS_NOT_PROGRAM_PARAMETER_ERROR : The parameter is not a program parameter.\n");
					break;

				case CG_INVALID_PARAMETER_TYPE_ERROR: // 32
					QDT_Message("CG_INVALID_PARAMETER_TYPE_ERROR : The type of the parameter is invalid.\n");
					break;

				case CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR: // 33
					QDT_Message("CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR : The parameter must be a resizable array.\n");
					break;

				case CG_INVALID_SIZE_ERROR: // 34
					QDT_Message("CG_INVALID_SIZE_ERROR : The size value is invalid.\n");
					break;

				case CG_BIND_CREATES_CYCLE_ERROR: // 35
					QDT_Message("CG_BIND_CREATES_CYCLE_ERROR : Cannot bind the given parameters.  Binding will form a cycle.\n");
					break;

				case CG_ARRAY_TYPES_DO_NOT_MATCH_ERROR: // 36
					QDT_Message("Cannot bind the given parameters.  Array types do not match.\n");
					break;

				case CG_ARRAY_DIMENSIONS_DO_NOT_MATCH_ERROR: // 37
					QDT_Message("CG_ARRAY_DIMENSIONS_DO_NOT_MATCH_ERROR : Cannot bind the given parameters. Array dimensions do not match.\n");
					break;

				case CG_ARRAY_HAS_WRONG_DIMENSION_ERROR: // 38
					QDT_Message("CG_ARRAY_HAS_WRONG_DIMENSION_ERROR : The array is has the wrong dimension.\n");
					break;

				case CG_TYPE_IS_NOT_DEFINED_IN_PROGRAM_ERROR: // 39
					QDT_Message("CG_TYPE_IS_NOT_DEFINED_IN_PROGRAM_ERROR : Connecting the parameters failed because The type of the source parameter is not defined within the given program or does not match the type with the same name in the program.\n");
					break;
				}
			}
		}
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
