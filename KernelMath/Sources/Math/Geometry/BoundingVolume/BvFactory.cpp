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
//	CLASS:	BV_FACTORY
//
//	02-07-02:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include INCL_KMATH(Math/Geometry/BoundingVolume/BvFactory)
#include INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)
#include INCL_KMATH(Math/Geometry/BoundingVolume/BoundingBox/BoundingBox)
#include INCL_KCORE(stream/iostream)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-07-02:	JLT - Created
//-----------------------------------------------------------------------------
BV_FACTORY* BV_FACTORY::Instance()
{
	static BV_FACTORY Inst;

	return &Inst;
}

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		
//	02-07-02:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME* BV_FACTORY::Create(const BOUNDING_VOLUME::TYPE	type)
{
	switch (type)
	{
	case BOUNDING_VOLUME::TYPE_AABB :
		return (new AABB);
		
	case BOUNDING_VOLUME::TYPE_OBB :
		return (new OBB);
		
	case BOUNDING_VOLUME::TYPE_SPHERE :
		return (new SPHERE);
		
	case BOUNDING_VOLUME::TYPE_CYLINDER :
		return (new CYLINDER);
			
/*	case BOUNDING_VOLUME::TYPE_BOX :
 		return (new BOUNDING_BOX);*/
	}
	
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME *	BV_FACTORY::Create(const BOUNDING_VOLUME *	pBV)
{
	switch (pBV->GetType())
	{
	case BOUNDING_VOLUME::TYPE_AABB :
		return (new AABB(*static_cast<const AABB*>(pBV)));
		
	case BOUNDING_VOLUME::TYPE_OBB :
		return (new OBB(*static_cast<const OBB*>(pBV)));
		
	case BOUNDING_VOLUME::TYPE_SPHERE :
		return (new SPHERE(*static_cast<const SPHERE*>(pBV)));
		
	case BOUNDING_VOLUME::TYPE_CYLINDER :
		return (new CYLINDER(*static_cast<const CYLINDER*>(pBV)));
			
/*	case BOUNDING_VOLUME::TYPE_BOX :
 		return (new BOUNDING_BOX(*static_cast<const BOUNDING_BOX*>(pBV)));*/
	}
	
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
BOUNDING_VOLUME *	BV_FACTORY::Read(IO_STREAM &	Stream)
{
	int nType;
	BOUNDING_VOLUME	*pBV;

	Stream >> nType;

	pBV = Create(static_cast<BOUNDING_VOLUME::TYPE>(nType));
	pBV->Read(Stream);

	return (pBV);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	BV_FACTORY::Write(IO_STREAM &		Stream,
						  BOUNDING_VOLUME *	pBV)
{
	Stream << pBV->GetType();
	pBV->Write(Stream);
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
