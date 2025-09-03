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
//	CLASS:	GLOW
//	The GLOW class implements ...
//
//	02-04-23:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_GLOW_H__
#define __M3D_SCENEGRAPH_ENTITIES_GLOW_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Color/Color)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	GLOW : public ENTITY_ABC
		{
		public:

			/** Commands */
			enum
			{	
				CMD_GLOW_SET_NORMAL = CMD_ENTITY_LAST,
				CMD_GLOW_SET_OUTER_ANGLE,
				CMD_GLOW_SET_INNER_ANGLE,
				CMD_GLOW_SET_COLORMAX,
				CMD_GLOW_SET_COLORMIN,
				CMD_GLOW_SET_SCALEMAX,
				CMD_GLOW_SET_SCALEMIN,
				CMD_GLOW_SET_UVS,
				CMD_GLOW_SET_SPIN_ANGLE,
				CMD_GLOW_SET_MATERIAL,
				CMD_GLOW_SET_DISTANCE,
				CMD_GLOW_ENABLE_ZTEST,
				CMD_GLOW_ENABLE_MITIGATION,
				CMD_GLOW_ENABLE_LENS_FLARE,
				CMD_GLOW_SET_LENS_FLARE_INNER_ANGLE,
				CMD_GLOW_SET_NBR_CIRCLES,
				CMD_GLOW_SET_CIRCLE_PARAMETERS,
				CMD_GLOW_ENABLE_CIRCLE_OWN_COLOR,
				CMD_GLOW_SET_CIRCLE_COLORMAX,
				CMD_GLOW_SET_CIRCLE_COLORMIN,
				CMD_GLOW_SET_CIRCLE_MATERIAL,
				CMD_GLOW_SET_CIRCLE_UVS,
				CMD_GLOW_SET_SPAWN_TIME,
				CMD_GLOW_ENABLE_AUTO_SPIN,
				CMD_GLOW_ENABLE_RAY_CAST,
				CMD_GLOW_ENABLE_FIXED_SIZE,
				CMD_GLOW_GET_COLORMAX,
				CMD_GLOW_GET_DISTANCE,
				CMD_GLOW_SET_ACTIVE,
				CMD_GLOW_IS_ACTIVE,
				CMD_GLOW_SET_SCALEMAX_X,
				CMD_GLOW_SET_SCALEMAX_Y,
				CMD_GLOW_GET_SCALEMAX_X,
				CMD_GLOW_GET_SCALEMAX_Y,
				CMD_GLOW_LAST,
			};

			/** Flags */
			enum	GLOW_FLAGS
			{	
				GLOW_RESERVED	= 1 << 0,
				GLOW_ZTEST		= 1 << 1,
				GLOW_MITIGATION	= 1 << 2,
				GLOW_LENS_FLARE	= 1 << 3,
				GLOW_AUTO_SPIN	= 1 << 4,
				GLOW_RAY_CAST	= 1 << 5,
				GLOW_FIXED_SIZE	= 1 << 6,
				GLOW_IS_ACTIVE	= 1 << 7,
				GLOW_ALL_AXIS	= 1 << 8,
				GLOW_UP_AXIS	= 1 << 9,
				GLOW_RIGHT_AXIS	= 1 << 10
			};

			GLOW();
			virtual ~GLOW();

			/** Accessors */
			_INLINE_ int						GetFlags(int nFlagsMask) const;
			_INLINE_ const VECTOR &				GetNormal() const;
			_INLINE_ float						GetOuterAngle() const;
			_INLINE_ float						GetInnerAngle() const;
			_INLINE_ const COLOR &				GetColorMax() const;
			_INLINE_ const COLOR &				GetColorMin() const;
			_INLINE_ const VECTOR &				GetScaleMax() const;
			_INLINE_ const VECTOR &				GetScaleMin() const;
			_INLINE_ const POINT2D *			GetUVs() const;
			_INLINE_ float						GetSpinAngle() const;
			_INLINE_ const COM_INSTANCE &		GetMaterialInst() const;
			_INLINE_ float						GetDistance() const;
			_INLINE_ bool						IsActive() const;

			/** Modifiors */
			_INLINE_ void						SetFlags(int nFlagsMask, int nFlagsSet);
			_INLINE_ void						SetNormal(const VECTOR & Normal);
			_INLINE_ void						SetOuterAngle(const float rAngle);
			_INLINE_ void						SetInnerAngle(const float rAngle);
			_INLINE_ void						SetColorMax(const COLOR & Color);
			_INLINE_ void						SetColorMin(const COLOR & Color);
			_INLINE_ void						SetScaleMax(const VECTOR & Scale);
			_INLINE_ void						SetScaleMin(const VECTOR & Scale);
			_INLINE_ void						SetUVs(const POINT2D * pUVs);
			_INLINE_ void						SetSpinAngle(const float rSpinAngle);
			_INLINE_ void						SetMaterialInst(const COM_INSTANCE & MaterialInst);
			_INLINE_ void						SetDistance(const float rDistance);
			_INLINE_ void						SetActive(bool b);

			_INLINE_ void						SetLensFlareInnerAngle(float rAngle);
					void						SetNbrCircles(int nNbrCircles);
			_INLINE_ void						SetCirclePosition(int nCircleIndex, float rPosition);
			_INLINE_ void						SetCircleIntensity(int nCircleIndex, float rIntensity);
			_INLINE_ void						SetCircleSize(int nCircleIndex, float rSize);
			_INLINE_ void						SetCircleSpinAngle(int nCircleIndex, float rAngle);
			_INLINE_ void						EnableCircleOwnColor(int nCircleIndex, bool bValue = true);
			_INLINE_ void						SetCircleColorMin(int nCircleIndex, const COLOR & Color);
			_INLINE_ void						SetCircleColorMax(int nCircleIndex, const COLOR & Color);
			_INLINE_ void						SetCircleMaterialInst(int nCircleIndex, const COM_INSTANCE & MaterialInst);
					void						SetCircleUVs(int nCircleIndex, POINT2D * pUVs);
			_INLINE_ void						SetSpawnTime(int nSpawnTime);

			/** Derivation from ENTITY_ABC */
			virtual void						Display(DISPLAY_PARAMETERS & DisplayParameters);
			virtual bool						TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetNormal(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetOuterAngle(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetInnerAngle(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetColorMax(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetColorMin(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleMax(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleMin(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetUVs(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetSpinAngle(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetMaterial(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetDistance(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableZTest(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableMitigation(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableLensFlare(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetLensFlareInnerAngle(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetNbrCircles(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetCircleParameters(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableCircleOwnColor(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetCircleColorMax(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetCircleColorMin(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetCircleMaterial(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetCircleUVs(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetSpawnTime(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableAutoSpin(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableRayCast(void * const pO, void * const pR, void * const pP);
			static  void						EM_EnableFixedSize(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetActive(void * const pO, void * const pR, void * const pP);
			static  void						EM_IsActive(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetColorMax(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetDistance(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleMax_X(void * const pO, void * const pR, void * const pP);
			static  void						EM_SetScaleMax_Y(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetScaleMax_X(void * const pO, void * const pR, void * const pP);
			static  void						EM_GetScaleMax_Y(void * const pO, void * const pR, void * const pP);
			

			// Chunk
		#ifndef _MASTER_ONLY_DATABANK
			static void							CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void							CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static void							CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

			bool								IsVisible(DISPLAY_PARAMETERS &	DisplayParameters,
														float &				rAngle,
														VECTOR &				PositionInViewCS,
														bool					bDoVisibility);

			bool								VisibilityRayCast(DISPLAY_PARAMETERS &	DisplayParameters);

			/*void								DrawLensFlare(DISPLAY_PARAMETERS &	DisplayParameters,
															const VECTOR &		GlowPosition,
															const VECTOR &		GlowScale,
															const COLOR &			GlowColor,
															const float			rGlowAngle,
															const float			rGlowIntensity,
															MATERIAL_ABC *		pMaterial);*/

			/** Glow parameters */
			VECTOR								_Normal;
			float								_rOuterAngle;
			float								_rInnerAngle;
			COLOR								_DiffuseColorMin;
			COLOR								_DiffuseColorMax;
			VECTOR								_ScaleMin;
			VECTOR								_ScaleMax;
			POINT2D								_pUVs[4];
			float								_rSpinAngle;
			COM_INSTANCE						_MaterialInst;

			/** Lens flare parameters */
			float								_rLensFlareInnerAngle;
			int									_nNbrCircles;
			float							   *_pCirclesPosition;
			float							   *_pCirclesIntensity;
			float							   *_pCirclesSize;
			float							   *_pCirclesSpinAngle;
			bool							   *_pCirclesOwnColor;
			COLOR							   *_pCirclesColorMin;
			COLOR							   *_pCirclesColorMax;
			QDT_VECTOR<COM_INSTANCE>			_vCirclesMaterialInst;
			POINT2D *						   *_pCirclesUVs;

			/** For smooth appearance / disappearance */
			bool								_bToDisplay;
			bool								_bSmooth;
			int									_nElapsedTime;
			int									_nSpawnTime;

			int									_nFlags;

			/** In case of ZTest activated */
			float								_rDistance;
		};
	}
}

#ifndef	_DEBUG
	#include	"Glow.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_GLOW_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

