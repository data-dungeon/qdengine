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
//	CLASS:	ENGINE_TEXTURE_ABC
//
//	05-11-10:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"FlowData.h"
#include	INCL_KCORE(Stream/iostream)
//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_ABC Constructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
FLOW_DATA::FLOW_DATA()
{
	pProjectors = NULL;
	pIndices = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_ABC Destructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
FLOW_DATA::~FLOW_DATA()
{
	if (pProjectors)
		delete [] pProjectors;
	if (pIndices)
		delete [] pIndices;
	if (pSources)
		delete [] pSources;
}



bool		FLOW_DATA::FLOW_SPACE::operator==(const FLOW_DATA::FLOW_SPACE& fs) const
{
	return (ProjX[0] == fs.ProjX[0] && ProjX[1] == fs.ProjX[1] && ProjX[2] == fs.ProjX[2] && 
			ProjY[0] == fs.ProjY[0] && ProjY[1] == fs.ProjY[1] && ProjY[2] == fs.ProjY[2] && 
			ProjZ[0] == fs.ProjZ[0] && ProjZ[1] == fs.ProjZ[1] && ProjZ[2] == fs.ProjZ[2]);
}


//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-03-27:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW_DATA::Read(IO_STREAM &	Stream)
{
	Stream >> uiNumProjectors;
	pProjectors = new FLOW_SPACE[uiNumProjectors];
	for (UInt32 nproj = 0; nproj < uiNumProjectors; nproj++)
	{
		Stream >> pProjectors[nproj];
	}
	
	Stream >> uiSize;
	pIndices = new UInt16[uiSize * uiSize];
	for (UInt32 i = 0; i < uiSize * uiSize; i++)
	{
		Stream >>  pIndices[i];
	}

	Stream >> uiNumSources;
	pSources = new FLOW_SOURCE[uiNumSources];
	for (UInt32 s = 0; s < uiNumSources; s++)
	{
		Stream >> pSources[s];
	}
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-03-27:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW_DATA::Write(IO_STREAM &	Stream) const
{
	Stream << uiNumProjectors;
	for (UInt32 nproj = 0; nproj < uiNumProjectors; nproj++)
	{
		Stream << pProjectors[nproj];
	}
	
	Stream << uiSize;
	for (UInt32 i = 0; i < uiSize * uiSize; i++)
	{
		Stream << pIndices[i];
	}

	Stream << uiNumSources;
	for (UInt32 s = 0; s < uiNumSources; s++)
	{
		Stream << pSources[s];
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSpawnPos
//	Object:		
//	06-03-28:	PDE - Created
//-----------------------------------------------------------------------------
void	FLOW_DATA::GetSpawnPos(VECTOR_2 &	pos,
							   UInt32		area)
{
	Bool    found = false;
	UInt32 maxTry = 40;

	while (!found && maxTry--)
	{
		UInt32	index = MATH::Rand((Int32)uiNumSources);
		FLOW_SOURCE* tri = &pSources[index];

		Int32 proba = 0;

		switch (area)
		{
		case 0:
			proba = tri->rValue; break;
		case 1:
			proba = tri->gValue; break;
		case 2:
			proba = tri->bValue; break;
		}

		if (MATH::Rand(255) < proba)
		{
			found = true;
			VECTOR_2 du(tri->b.GetX() - tri->a.GetX(), tri->b.GetY() - tri->a.GetY());
			VECTOR_2 dv(tri->c.GetX() - tri->a.GetX(), tri->c.GetY() - tri->a.GetY());
			
			// random homogenous distribution on a triangle
			Float32 uval, vval;
			do 
			{
				uval = MATH::Rand(1.0f);
				vval = MATH::Rand(1.0f);
			}
			while (uval + vval <= 1.0f);

			pos = VECTOR_2(tri->a.GetX() + uval * du.GetX() + vval * dv.GetX(),
						   tri->a.GetY() + uval * du.GetY() + vval * dv.GetY());

		}

		if (!found)
		{
			QDT_Message("WARNING : Maximum rejection happened in GetSpawnPos() (FlowData.cpp). This could cause performance issues");
		}
	}
}


void ProjectVector(const VECTOR& worldVect,  const VECTOR_2& uv, VECTOR& res)
{

}




//=============================================================================
//	CODE ENDS HERE
//=============================================================================
