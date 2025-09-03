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
//	CLASS:	FLOWGRID
//
//	05-11-09:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_MESH_FLOW_DATA_H__
#define __M3D_SCENEGRAPH_MESH_FLOW_DATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_MAYAVIEWER(CMD_CREATE_FLOW_DATA);

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		class FLOW_DATA
		{
			friend class CMD_CREATE_FLOW_DATA; // build from a factory within Maya

			public:
			FLOW_DATA();
			~FLOW_DATA();

			FLOW_DATA(IO_STREAM &Stream){Stream >> *this;};

		

			// Stream
			void		Read(IO_STREAM &Stream);
			void		Write(IO_STREAM &Stream) const;

			friend IO_STREAM&		operator << (IO_STREAM &Stream, const FLOW_DATA &V)
			{
				V.Write(Stream);
				return (Stream);
			}

			friend IO_STREAM&		operator >> (IO_STREAM &Stream, FLOW_DATA &V)
			{
				V.Read(Stream);
				return (Stream);
			}

			class FLOW_SPACE
			{
			public:
				Float32  ProjX[3];
				Float32  ProjY[3];
				Float32  ProjZ[3];
				bool		operator==(const FLOW_DATA::FLOW_SPACE& fs) const;

				friend IO_STREAM& operator << (IO_STREAM &Stream, const FLOW_SPACE &V)
				{
						for (UInt32 i = 0; i < 3; i++) Stream << V.ProjX[i];
						for (UInt32 j = 0; j < 3; j++) Stream << V.ProjY[j];
						for (UInt32 k = 0; k < 3; k++) Stream << V.ProjZ[k];
						return Stream;
				}

				friend IO_STREAM&	operator >> (IO_STREAM &Stream, FLOW_SPACE &V)
				{
						for (UInt32 i = 0; i < 3; i++) Stream >> V.ProjX[i];
						for (UInt32 j = 0; j < 3; j++) Stream >> V.ProjY[j];
						for (UInt32 k = 0; k < 3; k++) Stream >> V.ProjZ[k];
						return Stream;
				}

			};

			void GetSpawnPos(VECTOR_2& pos, UInt32 area);
			void ProjectVector(const VECTOR& worldVect,  const VECTOR_2& uv, VECTOR& res);


			private:

			class FLOW_SOURCE
			{
			public:
				VECTOR_2 a;
				VECTOR_2 b;
				VECTOR_2 c;
				UInt16     rValue;
				UInt16     gValue;
				UInt16     bValue;
				friend IO_STREAM& operator << (IO_STREAM &Stream, const FLOW_SOURCE &V)
				{
					Stream << V.a << V.b << V.c;
					Stream << V.rValue << V.gValue << V.bValue;
					return Stream;
				}

				friend IO_STREAM&		operator >> (IO_STREAM &Stream, FLOW_SOURCE &V)
				{
					Stream >> V.a;
					Stream >> V.b;
					Stream >> V.c;
					Stream >> V.rValue;
					Stream >> V.gValue;
					Stream >> V.bValue;
					return Stream;
				}
			};

			UInt32			uiNumProjectors;
			FLOW_SPACE*     pProjectors;
			UInt32			uiSize;
			UInt16*         pIndices;
			UInt32			uiNumSources;
			FLOW_SOURCE*    pSources;

		};
	}
}


#endif // __M3D_SCENEGRAPH_MESH_FLOW_DATA_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

