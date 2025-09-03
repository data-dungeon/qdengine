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
// CLASS: CAM_FREE_STRUCT
//
//
//00-12-11 RMA Created: 
//*****************************************************************************

//=============================================================================
// Name: CAM_FREE_STRUCT
//00-12-11 RMA Created: 
//=============================================================================

#ifndef __M3D_CAM_FREE_STRUCT_H__
#define __M3D_CAM_FREE_STRUCT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(CLOCK)

#include INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)
#include INCL_KSERV(Input/ObjectObserver)
#include INCL_KSERV(Input/DeviceObserver)

namespace QDT
{
	namespace M3D
	{
		class	CAM_FREE_STRUCT : public ANIM_NODE_STRUCT
		{
		public:

			enum KEYMOV_TYPE
			{
				UNKOWN_KEYMOV_TYPE = -1,
				KEYMOV_FAST_1 = 0,
				KEYMOV_FAST_2,

				KEYMOV_GO_FORWARD,
				KEYMOV_GO_BACKWARD,

				KEYMOV_GO_UP,
				KEYMOV_GO_DOWN,
				KEYMOV_GO_LEFT,
				KEYMOV_GO_RIGHT,

				KEYMOV_TURN_LEFT,
				KEYMOV_TURN_RIGHT,
				KEYMOV_TURN_UP,
				KEYMOV_TURN_DOWN,

				KEYMOV_ROLL_LEFT,
				KEYMOV_ROLL_RIGHT,

				KEYMOV_FOCAL_PLUS,
				KEYMOV_FOCAL_MINUS,

				KEYMOV_MORE,
				KEYMOV_LESS,

				KEYMOV_RESET,

				//dof patch
				KEYMOV_DOF_ENABLE,
				KEYMOV_DOF_DISABLE,
				KEYMOV_DOF_DEBUG_ENABLE,
				KEYMOV_DOF_DEBUG_DISABLE,
				KEYMOV_DOF_FAR_LESS,
				KEYMOV_DOF_FAR_MORE,
				KEYMOV_DOF_PLANE_LESS,
				KEYMOV_DOF_PLANE_MORE,
				KEYMOV_DOF_RANGE_LESS,
				KEYMOV_DOF_RANGE_MORE,
				KEYMOV_DOF_SAFERANGE_LESS,
				KEYMOV_DOF_SAFERANGE_MORE,
				KEYMOV_DOF_BLUR_LESS,
				KEYMOV_DOF_BLUR_MORE,

				KEYMOV_NBR_CTRLS
			};



			explicit	CAM_FREE_STRUCT();

			VECTOR				_LastPos;
			COM_INSTANCE		_VirtualNodeInst;


		protected:
			/* Copy constructor */
			explicit CAM_FREE_STRUCT(const CAM_FREE_STRUCT & BehCamFree) ;
			/** Destructor */
			virtual ~CAM_FREE_STRUCT() ;



		public :
			/** Operator = */
			CAM_FREE_STRUCT & operator=( const CAM_FREE_STRUCT & BehCamFree) ;
			
			void	SetEntityInst(const COM_INSTANCE & EntityInst);

			float	GetRoll() const;
			void	SetRoll(const float rRoll);
			void	ReinitRoll(const float rRoll);

			// Set keys to use 
			void	SetKey(KEYMOV_TYPE nKeyType , int nKeyCode );
			int		GetKey( KEYMOV_TYPE nKeyType ) const;

			////////////////////////////////////////////////////////////////
			virtual void	ReadRotation( float rTime, QUATERNION& Quat );
			virtual void	ReadTranslation( float rTime, VECTOR& Vect );
			virtual void	StoreRotTrans( float rTime );
			////////////////////////////////////////////////////////////////



		private:
			
			void		InitDefaultKeys();

			int			_nKeysDef[KEYMOV_NBR_CTRLS];	//	Key configuration

			int			_nHandle;

			static void OnKeyFast(IntPtr, int);
			static void OnKeyGoFoward(IntPtr, int);
			static void OnKeyGoBackward(IntPtr, int);
			static void OnKeyGoUp(IntPtr, int);
			static void OnKeyGoDown(IntPtr, int);
			static void OnKeyGoLeft(IntPtr, int);
			static void OnKeyGoRight(IntPtr, int);
			static void OnKeyTurnLeft(IntPtr, int);
			static void OnKeyTurnRight(IntPtr, int);
			static void OnKeyTurnUp(IntPtr, int);
			static void OnKeyTurnDown(IntPtr, int);
			static void OnKeyRollLeft(IntPtr, int);
			static void OnKeyRollRight(IntPtr, int);
			static void OnKeyFocalPlus(IntPtr, int);
			static void OnKeyFocalMinus(IntPtr, int);
			static void OnKeyMore(IntPtr, int);
			static void OnKeyLess(IntPtr, int);
			static void OnKeyReset(IntPtr, int);

			static void OnKeyDofEnable(IntPtr, int);
			static void OnKeyDofDisable(IntPtr, int);
			static void OnKeyDofDebugEnable(IntPtr, int);
			static void OnKeyDofDebugDisable(IntPtr, int);
			static void OnKeyDofFarLess(IntPtr, int);
			static void OnKeyDofFarMore(IntPtr, int);
			static void OnKeyDofPlaneLess(IntPtr, int);
			static void OnKeyDofPlaneMore(IntPtr, int);
			static void OnKeyDofRangeLess(IntPtr, int);
			static void OnKeyDofRangeMore(IntPtr, int);
			static void OnKeyDofSafeRangeLess(IntPtr, int);
			static void OnKeyDofSafeRangeMore(IntPtr, int);
			static void OnKeyDofBlurLess(IntPtr, int);
			static void OnKeyDofBlurMore(IntPtr, int);

			static void OnXAxis1(IntPtr, int);
			static void OnXAxis2(IntPtr, int);
			static void OnYAxis1(IntPtr, int);
			static void OnYAxis2(IntPtr, int);
			static void OnConnect(IntPtr);
			static void OnDisconnect(IntPtr);

			static class OBJECT_OBSERVER	_pKeyInst[KEYMOV_NBR_CTRLS];
			static int						_nObjectType[KEYMOV_NBR_CTRLS];
			static class DEVICE_OBSERVER	_ConnectInst;
			static class DEVICE_OBSERVER	_DisconnectInst;

			void (*_fnct[KEYMOV_NBR_CTRLS])(IntPtr, int);
			float _fMov[KEYMOV_NBR_CTRLS];	

			COM_INSTANCE		_EntityInst;

			//	Different const step
			static float		_rRotStep;
			static float		_rTransStep;
			static float		_rRollStep;
			static float		_rFocalStep;
			static float		_rMoreStep;

			//	Spped modifiers
			float				_rMulCoeff;
			bool				_bIsFast;

			//	Roll Save
			float				_rCurrentRoll;

			static CAM_FREE_STRUCT	   *_pActiveCamFree;
		};
	}
}

#endif // #define __M3D_CAM_FREE_STRUCT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
