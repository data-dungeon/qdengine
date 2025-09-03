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
//	CLASS:	PARTICLE_SYSTEM
//	The PARTICLE_SYSTEM class implements ...
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESYSTEM_H__
#define __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESYSTEM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	PARTICLE_SYSTEM
		{
		public:

			PARTICLE_SYSTEM();
			~PARTICLE_SYSTEM();

					void								UpdateParticleSystem(float rDeltaTime);
					void								Display();
										
					int								GetNbrForceFields() const;
			_INLINE_ const QDT_VECTOR <COM_INSTANCE> &	GetForceFields() const;
					void								AddForceField(const COM_INSTANCE &ForceFieldNodeInst);
					void								ResetForceFields();
					void								RemoveForceField(const COM_INSTANCE &ForceFieldNodeInst);
		//			 void								ComputeForces(PARTICLE * pParticle, float rDeltaTime = 0.0f);

		//			 void								AddActiveParticle(PARTICLE_SPRITE * pParticle);
		//			 void								AddInactiveParticleSprite(PARTICLE_SPRITE * pParticle);

		/*			 void								AddGlow(const VECTOR &	Position,
																const VECTOR &	Size,
																POINT2D *		pUVs,
																float			rRotation,
																const COLOR &	Color,
																SHADER_ABC *	pShader,
																bool			bZTest = true);*/

		//			 void								AddSprite(VECTOR* Position, POINT2D* pUVs, const COLOR & Color, SHADER_ABC * pShader, bool bZTest);

					void								Clear();
					void								RemoveLinks();

		//			 PARTICLE_SPRITE *					GetParticleSprite();

		private:

			QDT_VECTOR <COM_INSTANCE>		_vForceFieldsNodes;

		//	QDT_VECTOR <PARTICLE_SPRITE *>	_vParticles;
			
		//	QDT_VECTOR <SPRITE_TABLE_ABC *>	_vSpriteTables;
		//	QDT_DLIST <SPRITE_TABLE_ABC *>	_dlActiveSpriteTables;
		//	QDT_DLIST <SPRITE_TABLE_ABC *>	_dlGlowSpriteTables;
		//	QDT_DLIST <SPRITE_TABLE_ABC *>	_dlInactiveSpriteTables;
			
		//	PARTICLE_SPRITE *				_lpInactiveParticlesSprite;
		};
	}
}

#ifndef _DEBUG
	#include	"ParticleSystem.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESYSTEM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
