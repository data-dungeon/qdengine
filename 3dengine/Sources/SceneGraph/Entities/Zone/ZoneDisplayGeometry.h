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
//	CLASS:	ZONE_DISPLAY_GEOMETRY
//	The ZONE_DISPLAY_GEOMETRY class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	ZONE_DISPLAY_GEOMETRY
{
public:

									ZONE_DISPLAY_GEOMETRY();
								   ~ZONE_DISPLAY_GEOMETRY();

	_INLINE_ GEOMETRY_ABC *			GetGeometry() const;
	_INLINE_ const COM_INSTANCE&	GetMaterial() const;
	_INLINE_ const AABB&			GetBBox() const	;
	_INLINE_ const SPHERE			GetBSphere() const;

	_INLINE_ void					SetGeometry(GEOMETRY_ABC * pGeomtry);
	_INLINE_ void					SetMaterial(const COM_INSTANCE & ciMaterial);
	_INLINE_ void					SetBoundingBox(const AABB& BBox);
	_INLINE_ void					SetBoundingSphere(const SPHERE & BSphere);

private:

	GEOMETRY_ABC				   *_pGeometry;
	COM_INSTANCE					_ciMaterial;
	AABB							_BBox;
	SPHERE							_BSphere;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
