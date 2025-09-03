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
// CLASS: ANIM_PC
//
//
//00-11-24 GHP Created: 
//*****************************************************************************

#ifndef __M3D_ANIM_PC_H__
#define __M3D_ANIM_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(TRACK_DATA)

#include INCL_KMATH(Math/Color/ColorInt)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)
#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class ANIM_PC : public COMMUNICATOR
		{
		public :

			enum PACKETS_VERSION { DIALOG_VERSION = 1, CYBER_VERSION };

			/** Operator = */
			const ANIM_PC				operator=(const ANIM_PC &Anim) ;

			///////////////////////////////////////////////////////////////
			//	COMMUNICATOR
			///////////////////////////////////////////////////////////////
			static void					COMInit(CLASS_DESCRIPTOR * const pCD);
			static						COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static						COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			const QDT_STRING&			GetClassGUID() const;
			void						SetClassGUID(const QDT_STRING& sClassGUID);
			///////////////////////////////////////////////////////////////


			/** Accessors & modifiors */
			unsigned int				GetNbFrames() const;			//	
			unsigned int				GetNbTracks() const;			//	
			float						GetFPS() const;
			TRACK_DATA*					GetTrack(unsigned int index)  ;	
			const TRACK_DATA*			GetTrack(unsigned int index) const ;	
			void						SetNbTracks(unsigned int);	
			void						SetNbFrames(unsigned int);
			void						SetParams( unsigned int	nNbrFrames, unsigned int	nNbrTracks ) ;
			float						GetAnimHeading( bool bAbs, float rFrame = -1.0f)  ;
			float						GetAnimPitch( bool bAbs, float rFrame = -1.0f)  ;
			float						GetAnimRoll( bool bAbs, float rFrame = -1.0f)  ;
			VECTOR						GetAnimTrans( bool bAbs, float rFrame = -1.0f)  ;
			const QDT_STRING&			GetObjectName()const;
			void						SetObjectName( const QDT_STRING& ObjName );
			QUATERNION_VECTOR			GetAbsolute( float rFrame = -1.0f );
			VECTOR						GetAbsolutePosition()const;
			QUATERNION					GetAbsoluteRotation()const;
			void						AllocTracks();
			void						SetCompressed( bool b );
			bool						IsCompressed()const;

			// READ PART
#ifndef _MASTER_ONLY_DATABANK
			bool						ReadAnimationParam(IO_STREAM & Stream);
			bool						ReadObjectName(IO_STREAM & Stream);
			bool						ReadTrackData(IO_STREAM & Stream);
#endif

			// WRITE PART
#ifndef _MASTER
			bool						WriteAnimationParam(IO_STREAM & Stream) const;
			bool						WriteTrackData(IO_STREAM & Stream) const;
			bool						WriteObjectName(IO_STREAM & Stream) const;
#endif

		protected:

			/** Constructor & destructor */
			ANIM_PC();
			ANIM_PC(CLASS_DESCRIPTOR* const pCD);
			~ANIM_PC();

			QDT_STRING						_ObjectName;	
			TRACK_DATA						*_pTracks;
			mutable unsigned int			_nNbrFrames ;		
			unsigned int					_nNbrTracks ;	
			float							_rFPS;
			bool							_bPacked;

		private:

			enum	COMMANDS
			{
				COMMAND_GET_NB_FRAMES = CMD_COM_USER_START,
				COMMAND_GET_TRANS , 
				COMMAND_GET_HEADING ,
				COMMAND_GET_PITCH , 
				COMMAND_GET_ROLL ,
				COMMAND_LAST,
			};


			///////////////////////////////////////////////////////////////
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void	EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void	EM_GetNbrFrames(void * const pO, void * const pR, void * const pP);
			static void	EM_GetTrans(void * const pO, void * const pR, void * const pP);
			static void	EM_GetHead(void * const pO, void * const pR, void * const pP);
			static void	EM_GetPitch(void * const pO, void * const pR, void * const pP);
			static void	EM_GetRoll(void * const pO, void * const pR, void * const pP);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			static void	CM_ReadAnimationParam(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadTrackData(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteAnimationParam(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteTrackData(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadAnimationParam2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteAnimationParam2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			QDT_STRING						_sClassGUID;
			///////////////////////////////////////////////////////////////
		};
	}
}

#endif // __M3D_ANIM_PC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
