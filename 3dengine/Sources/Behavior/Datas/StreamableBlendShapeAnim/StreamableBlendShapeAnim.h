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
//	CLASS:	STREAMABLE_BLEND_SHAPE_ANIM
//	The STREAMABLE_BLEND_SHAPE_ANIM class implements ...
//
//	03-06-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAMABLE_BLEND_SHAPE_ANIM_H__
#define __M3D_STREAMABLE_BLEND_SHAPE_ANIM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(Streaming/Streamable)

namespace QDT
{
	namespace M3D
	{
		class	STREAMABLE_BLEND_SHAPE_ANIM : public STREAMABLE
		{
		public:

			STREAMABLE_BLEND_SHAPE_ANIM();
			STREAMABLE_BLEND_SHAPE_ANIM(CLASS_DESCRIPTOR * const pCD);
			~STREAMABLE_BLEND_SHAPE_ANIM();

			// Communication
			static void							COMInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_PARTITION_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_PARTITION_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();
			const QDT_STRING &					GetClassGUID() const;
			void								SetClassGUID(const QDT_STRING & sClassGUID);

			virtual STREAMABLE_PARTITION_WRITER *	CreatePartitionWriter();

			virtual void			ReadSpecificStreamHeader(IO_STREAM & Stream);
#ifndef _MASTER
			virtual void			WriteSpecificStreamHeader(IO_STREAM & Stream);
#endif

			int						GetNbrFrames() const	{ return (_nFramesCount); }
			float					GetFPS() const			{ return (_rFPS); }
			int						GetNbrWeights() const	{ return (_nNbrWeights); }

			void					SetNbrFrames(int n)		{ _nFramesCount = n; }
			void					SetFPS( float rVal )	{ _rFPS = rVal; }
			void					SetNbrWeights( int n )	{ _nNbrWeights = n; }


		protected:

			STREAM_BLOCK * CreateBlock(int nID, float rStartTime, float rEndTime, const char * pBuffer, int nSize);

		private:

			// Communication
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);

#ifndef _MASTER_ONLY_DATABANK
			static void	CM_ReadHeaderChunkV1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER		
			static void	CM_WriteHeaderChunkV1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif

			QDT_STRING						_sClassGUID;

			int					_nFramesCount;
			int					_nNbrWeights;
			float				_rFPS;
		};
	}
}

#endif // __M3D_STREAMABLE_BLEND_SHAPE_ANIM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
