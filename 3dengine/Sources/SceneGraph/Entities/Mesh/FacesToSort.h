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
//	CLASS:	FACES_TO_SORT
//	The FACES_TO_SORT class implements ...
//
//	06-02-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_FACES_TO_SORT_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_FACES_TO_SORT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_INDEX_BUFFER)
DECL_KCORE(IO_STREAM)
DECL_KMATH(PLANE)

#define MAX_INDEX_BUFFER_COUNT 4

namespace QDT
{
	namespace M3D
	{
		class	FACES_TO_SORT
		{
		public:

			FACES_TO_SORT();
			~FACES_TO_SORT();

			void Allocate(UInt32 uiSize, bool bSkinning, bool bCreateIndexBuffer);
			void Copy(const FACES_TO_SORT & Faces);

			I_INDEX_BUFFER * Sort(const PLANE & vEyePlane);

			void SetNbrTriangles(UInt32 uiNbrTriangles);
			void SetPositions(Float32 * prX, Float32 * prY, Float32 * prZ);
			void SetWeightsIndices(UInt8 * p0, UInt8 * p1, UInt8 * p2, UInt8 * p3);
			void SetWeights(Float32 * p0, Float32 * p1, Float32 * p2, Float32 * p3);
			void SetIndices(UInt16 * pi);
			void SetSkinning(Bool b);

			// Streams
			void				Read(IO_STREAM &Stream);
			void				Write(IO_STREAM &Stream) const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const FACES_TO_SORT &Faces)
			{
				Faces.Write(s);
				return s;
			}

			friend IO_STREAM&	operator >>(IO_STREAM &s, FACES_TO_SORT &Faces)
			{
				Faces.Read(s);
				return s;
			}

		private:

			void ComputeDistance(const PLANE & vEyePlane);
			
			_INLINE_ UInt32 GetSize() const { return (_uiSize); }

			UInt32				_uiSize;
			Bool				_bSkinning;

			Float32	*			_pDistance;

			// Position
			Float32 *			_prX;
			Float32 *			_prY;
			Float32 *			_prZ;

			// Blending indices
			UInt8 *				_puiBI0;
			UInt8 *				_puiBI1;
			UInt8 *				_puiBI2;
			UInt8 *				_puiBI3;

			// Blending weights
			Float32 *			_puiBW0;
			Float32 *			_puiBW1;
			Float32 *			_puiBW2;
			Float32 *			_puiBW3;

			// Triangles indices
			UInt16 *			_puiTriangleIndices;

			// Index buffer
			UInt32				_uiCurrentIndexBuffer;
			I_INDEX_BUFFER *	_apIndexBuffer[MAX_INDEX_BUFFER_COUNT];
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_FACES_TO_SORT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
