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
//	CLASS:	STREAM_ANIMATION
//	The STREAM_ANIMATION class implements ...
//
//	03-05-28:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_ANIMATION_H__
#define __M3D_STREAM_ANIMATION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_TRACK_HEADER)

#include INCL_KCOM(Streaming/Streamable)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_ANIMATION : public STREAMABLE
		{
		public:
			enum	COMMANDS
			{
				COMMAND_LAST,
			};

			///////////////////////////////////////////////////////////////
			STREAM_ANIMATION();
			STREAM_ANIMATION( CLASS_DESCRIPTOR* const pCD );
			~STREAM_ANIMATION();
			///////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////
			//	COMMUNICATOR
			///////////////////////////////////////////////////////////////
			static void			COMInit(CLASS_DESCRIPTOR * const pCD);
			static				COMMUNICATOR_PARTITION_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static				COMMUNICATOR_PARTITION_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			const QDT_STRING&	GetClassGUID() const;
			void				SetClassGUID(const QDT_STRING& sClassGUID);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			_INLINE_	int						GetNbrFrames()const;
			_INLINE_	int						GetNbrTracks()const;
						STREAM_TRACK_HEADER*	GetTrackHeader( int nNum )const;
			_INLINE_	float					GetBaseFrameRate()const;
			_INLINE_	float					GetTimeFromNumFrame( float rFrame )const;
			_INLINE_	float					GetFrameFromTime( float rTime )const;
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			_INLINE_	void					SetNbrFrames( int n );
			_INLINE_	void					SetNbrTracks( int n );
						void					SetTrackHeader( const STREAM_TRACK_HEADER& TH, int nNumTrack );
			_INLINE_	void					SetBaseFrameRate( float rVal );
			void								SetTrackHeaderArray( STREAM_TRACK_HEADER *pST );
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////	
#ifndef _MASTER_ONLY_DATABANK
			void				ReadSpecificStreamHeader(IO_STREAM & Stream);
#endif
#ifndef _MASTER
			void				WriteSpecificStreamHeader(IO_STREAM & Stream);
#endif
			///////////////////////////////////////////////////////////////	

			///////////////////////////////////////////////////////////////
			static _INLINE_ STREAM_ANIMATION*	GetStreamAnimFromInstance( const COM_INSTANCE& CI );
			STREAMABLE_PARTITION_WRITER*		CreatePartitionWriter();
			///////////////////////////////////////////////////////////////

		protected:

			///////////////////////////////////////////////////////////////
			STREAM_BLOCK * CreateBlock(int nID, float rStartTime, float rEndTime, const char * pBuffer, int nSize);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			void				ReadTrackHeader( IO_STREAM& Stream );
			void				WriteTrackHeader( IO_STREAM& Stream );
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			int						_nNbrFrames;
			int						_nNbrTracks;
			STREAM_TRACK_HEADER		*_pTracks;
			float					_rFPS;
			///////////////////////////////////////////////////////////////

		private:

			///////////////////////////////////////////////////////////////
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
#ifndef _MASTER_ONLY_DATABANK
			static void	CM_ReadHeaderChunkV1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
			static void	CM_WriteHeaderChunkV1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			QDT_STRING	_sClassGUID;
			///////////////////////////////////////////////////////////////

		};
	}
}

#ifndef _DEBUG
	#include "StreamAnimation.inl"
#endif

#endif // __M3D_STREAM_ANIMATION_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
