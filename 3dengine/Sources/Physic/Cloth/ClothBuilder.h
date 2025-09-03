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
//	CLASS:	CLOTH_BUILDER
//	The CLOTH_BUILDER class implements ...
//
//	02-02-27:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_PHYSIC_CLOTH_CLOTHBUILDER_H__
#define __M3D_PHYSIC_CLOTH_CLOTHBUILDER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace M3D
	{
		class CLOTH_BUILDER
		{
		private :
			enum 
			{
				STRETCH_NET = 0,
				BEND_NET,
				SHEAR_NET,

				NET_COUNT,
			};

		public:
			/** */
			CLOTH_BUILDER();
			/** */
			virtual ~CLOTH_BUILDER();

			/** */
			p_bool Build(LATTICE *pLattice, int nNbrLayers);

		private :
			/** */
			p_bool BuildStretch(LATTICE *pLattice);
			/** */
			p_bool BuildBend(LATTICE *pLattice);
			/** */
			p_bool BuildShear(LATTICE *pLattice);
		};
	}
}

#endif // __M3D_PHYSIC_CLOTH_CLOTHBUILDER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
