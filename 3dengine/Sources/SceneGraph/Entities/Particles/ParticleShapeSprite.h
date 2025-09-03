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
//	CLASS:	PARTICLE_SHAPE_SPRITE
//	The PARTICLE_SHAPE_SPRITE class implements ...
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESHAPESPRITE_H__
#define __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESHAPESPRITE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(COLOR)

#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleShape)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	PARTICLE_SHAPE_SPRITE : public PARTICLE_SHAPE
		{
		public:

			enum
			{ 
				CMD_PSSPRITE_SET_MATERIAL = CMD_PSHAPE_LAST,
				CMD_PSSPRITE_GET_MATERIAL,
				CMD_PSSPRITE_SET_UVS,
				CMD_PSSPRITE_SET_UVS2,
				CMD_PSSPRITE_SET_UVS_NB_KEYS,
				CMD_PSSPRITE_SET_UVS_TIME_KEY,
				CMD_PSSPRITE_SET_DIFFUSE_COLOR,
				CMD_PSSPRITE_SET_DIFFUSE_COLOR2,
				CMD_PSSPRITE_SET_DIFFUSE_COLOR_NB_KEYS,
				CMD_PSSPRITE_SET_DIFFUSE_COLOR_TIME_KEY,
				CMD_PSSPRITE_SET_DIFFUSE_COLOR_TRACK,
				CMD_PSSPRITE_ADD_DIFFUSE_COLOR_TRACK,
				CMD_PSSPRITE_REMOVE_DIFFUSE_COLOR_TRACK,
				CMD_PSSPRITE_SET_AXIS_ALIGNED,
				CMD_PSSPRITE_IS_AXIS_ALIGNED,
				CMD_PSSPRITE_SET_ALIGNMENT_AXIS,
				CMD_PSSPRITE_LAST,
			};

			PARTICLE_SHAPE_SPRITE();
			virtual ~PARTICLE_SHAPE_SPRITE();
			
			void								Update(float rDeltaTime);

			void								Copy(const PARTICLE_SHAPE_SPRITE & ParticleShapeSprite);
			PARTICLE_SHAPE_SPRITE &				operator = (const PARTICLE_SHAPE_SPRITE & ParticleShapeSprite);

			_INLINE_ void						SetMaterialInst(const COM_INSTANCE & MaterialInst);
			_INLINE_ const COM_INSTANCE &		GetMaterialInst() const;
		//			 MATERIAL *					GetMaterial() const;

		//	_INLINE_ PARTICLE_SHAPE_SPRITE_DATA *	GetShapeSpriteData() const;

			_INLINE_ int						GetUVsNbKeys() const;				
			_INLINE_ void						SetUVsNbKeys(int nNbrKeys);				
			_INLINE_ void						SetUVs(int nKey, const POINT2D pUVs[4]);
			_INLINE_ void						GetUVs(float rLife, POINT2D pUVs[4]) const;
			_INLINE_ float						GetUVsTimeKey(int nKey) const;
			_INLINE_ void						SetUVsTimeKey(int nKey, float rTimeKey);

			_INLINE_ void						AddColorTracks(int nNbrColorTracks = 1);
			_INLINE_ void						RemoveColorTrack(int nTrackIndex);
			_INLINE_ int						GetNbrColorTracks() const;
			_INLINE_ int						GetColorTrack() const;
			_INLINE_ void						SetColorTrack(int nColorTrack);

			_INLINE_ void						SetDiffuseColor(const COLOR &Color);
			_INLINE_ int						GetDiffuseColorNbKeys(int nTrackIndex) const;				
			_INLINE_ void						SetDiffuseColorNbKeys(int nTrackIndex, int nNbrKeys);				
			_INLINE_ void						SetDiffuseColor(int nTrackIndex, int nKey, const COLOR &Color);
			_INLINE_ void						GetDiffuseColor(int nTrackIndex, float rLife, COLOR &Color) const;
			_INLINE_ float						GetDiffuseColorTimeKey(int nTrackIndex, int nKey) const;
			_INLINE_ void						SetDiffuseColorTimeKey(int nTrackIndex, int nKey, float rTimeKey);
		//	_INLINE_ void						SetDiffuseColorTrack(QDT_VECTOR <DATA_TRACK<COLOR> *>	DiffuseColorTrack);

			_INLINE_ bool						IsAligned() const;
			_INLINE_ void						SetAligned(bool b);
			_INLINE_ void		 				GetAlignmentAxis(VECTOR & Axis1, VECTOR & Axis2) const;
			_INLINE_ const VECTOR &				GetAlignmentAxis1() const;
			_INLINE_ const VECTOR &				GetAlignmentAxis2() const;
			_INLINE_ void						SetAlignmentAxis(const VECTOR & Axis1, const VECTOR & Axis2);

			void								COM_PostLoad();
			void								UpdateShader();

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();

		private:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static  void						EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetMaterial(void * const pO, void * const pR, void * const pP);
			static	void						EM_GetMaterial(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetUVs(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetNbrUVsKeys(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetUVTimeKey(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetDiffuseColor(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetDiffuseColor2(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetNbrDiffuseColorKeys(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetDiffuseColorTimeKey(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetDiffuseColorTrack(void * const pO, void * const pR, void * const pP);
			static	void						EM_AddDiffuseColorTrack(void * const pO, void * const pR, void * const pP);
			static	void						EM_RemoveDiffuseColorTrack(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetAxisAligned(void * const pO, void * const pR, void * const pP);
			static	void						EM_IsAxisAligned(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetAlignmentAxis(void * const pO, void * const pR, void * const pP);

			// Chunk
		#ifndef _MASTER_ONLY_DATABANK
			static void							CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void							CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void							CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void							CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static void							CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

			static void*						_pVTable;
			
			COM_INSTANCE						_MaterialInst;

		//	PARTICLE_SHAPE_SPRITE_DATA			*_pShapeSpriteData;
		};
	}
}

#ifndef _DEBUG
	#include	"ParticleShapeSprite.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_PARTICLES_PARTICLESHAPESPRITE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
