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
//	CLASS:	INPUT_OBJECT_ABC
//	The INPUT_OBJECT_ABC class implements ...
//
//	02-06-24:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_INPUTOBJECTABC_H__
#define __KINPUT_INPUT_INPUTOBJECTABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KINPUT(IOBJECT_PARAM)

#include INCL_KCORE(Template/Qdt_Vector)
#ifndef _MASTER
#include INCL_KCORE(Tools/QdtString)
#endif

namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_OBJECT_ABC
		{
		public:
			enum OBJ_TYPE
			{
				OBJ_UNKNOWN = -1,
				OBJ_X_AXIS_REL,
				OBJ_Y_AXIS_REL,
				OBJ_Z_AXIS_REL,
				OBJ_X_AXIS_ABS,
				OBJ_Y_AXIS_ABS,
				OBJ_Z_AXIS_ABS,
				OBJ_RX_AXIS,
				OBJ_RY_AXIS,
				OBJ_RZ_AXIS,
				OBJ_BUTTON,
				OBJ_KEY,
				OBJ_POV,
				OBJ_SLIDER_AXIS,
				MAX_OBJ_TYPE
			};

			enum OBJ_BUTTON_ID
			{
				OBJ_CROSS		= 0,
				OBJ_SQUARE,
				OBJ_CIRCLE,
				OBJ_TRIANGLE,
				OBJ_TRIGGER_L1,
				OBJ_TRIGGER_L2,
				OBJ_TRIGGER_R1,
				OBJ_TRIGGER_R2,
				OBJ_SELECT,
				OBJ_START,
				OBJ_L3,
				OBJ_R3,
				OBJ_DPAD_UP,
				OBJ_DPAD_RIGHT,
				OBJ_DPAD_DOWN,
				OBJ_DPAD_LEFT,
			};

					INPUT_OBJECT_ABC();
					INPUT_OBJECT_ABC(const INPUT_OBJECT_ABC & C);
			virtual ~INPUT_OBJECT_ABC();
			
			bool			AddObserver(IOBJECT_PARAM *pObjParam);
			bool			RemoveObserver(IOBJECT_PARAM *pObjParam);
			int				GetNbrObserver() const;
			IOBJECT_PARAM*	GetObserver(const int nIndex);
			void			ClearAllObserver();

			bool		IsAnAxis() const;

			void		SetType(const OBJ_TYPE eType);
			OBJ_TYPE	GetType() const;
		#ifndef _MASTER
			QDT_STRING	GetTypeString() const;
		#endif
			int			GetHandle() const;

			void				SetObjectName(const QDT_STRING& strObjectName);
			const QDT_STRING&	GetObjectName() const;

			INPUT_OBJECT_ABC &	operator=(const INPUT_OBJECT_ABC & C);
			void	Copy(const INPUT_OBJECT_ABC & C);
			
			virtual void		DispatchEvent(const float rVal);

			static int		GetDefaultMinAbsAxisValue();
			static int		GetDefaultMaxAbsAxisValue();

		private:
			
			QDT_VECTOR<IOBJECT_PARAM*>		_tVectObserver;
			
		protected:
			int			_nHandle;
			OBJ_TYPE	_eType;
			QDT_STRING	_strObjectName;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
