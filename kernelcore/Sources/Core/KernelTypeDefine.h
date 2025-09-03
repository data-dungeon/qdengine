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
// 
//
//
//00-11-13 JVA Created: 
//*****************************************************************************

#ifndef __KCORE_CORE_KERNELTYPEDEFINE_H__
#define __KCORE_CORE_KERNELTYPEDEFINE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{

		enum
		{
			MODULE_KERNEL		= 0x00010000,
			MODULE_MOTEUR3D		= 0x00020000,
			MODULE_IAMAPP		= 0x00030000,
			MODULE_VIEWER		= 0x00040000,
			MODULE_IASM			= 0x00050000,
			MODULE_IVM			= 0x00060000,
			MODULE_TOOLS		= 0x00070000,
			MODULE_VM			= 0x00080000,
		};

		//	seuls les BASIC_TYPEs sont utilisables comme types de parametres
		//	pour les prototypes de commandes et de messages dans le cadre de la communication inter-module
		enum
		{
			BASIC_TYPE			= 0x00001000,
		};

		enum
		{
			OTYPE_KERNEL_BOOLEAN			= MODULE_KERNEL + BASIC_TYPE + 0x00000001,
			OTYPE_KERNEL_INTEGER			= MODULE_KERNEL + BASIC_TYPE + 0x00000002,
			OTYPE_KERNEL_FLOAT				= MODULE_KERNEL + BASIC_TYPE + 0x00000003,
			OTYPE_KERNEL_CHAR				= MODULE_KERNEL + BASIC_TYPE + 0x00000004,
			OTYPE_KERNEL_OBJECT_TYPE		= MODULE_KERNEL + BASIC_TYPE + 0x00000005,
			OTYPE_KERNEL_OBJECT_ID			= MODULE_KERNEL + BASIC_TYPE + 0x00000006,
			OTYPE_KERNEL_POINT3D			= MODULE_KERNEL + BASIC_TYPE + 0x00000007,
			OTYPE_KERNEL_VECTOR				= MODULE_KERNEL + BASIC_TYPE + 0x00000008,
			OTYPE_KERNEL_POINT2D			= MODULE_KERNEL + BASIC_TYPE + 0x00000009,
			OTYPE_KERNEL_COLOR				= MODULE_KERNEL + BASIC_TYPE + 0x0000000a,
			OTYPE_KERNEL_COLOR_FLOAT		= MODULE_KERNEL + BASIC_TYPE + 0x0000000b,
			OTYPE_KERNEL_ANGLE3D			= MODULE_KERNEL + BASIC_TYPE + 0x0000000c,
			OTYPE_KERNEL_SCALE3D			= MODULE_KERNEL + BASIC_TYPE + 0x0000000d,
			OTYPE_KERNEL_QUATERNION			= MODULE_KERNEL + BASIC_TYPE + 0x0000000e,
			OTYPE_KERNEL_STRING				= MODULE_KERNEL + BASIC_TYPE + 0x0000000f,
			OTYPE_KERNEL_COM_INSTANCE		= MODULE_KERNEL + BASIC_TYPE + 0x00000010,
		};
	}
}

#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
