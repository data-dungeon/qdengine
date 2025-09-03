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
//	CLASS:	MESH_GROUP_GEOMETRY
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
/*
//-----------------------------------------------------------------------------
//	Name:		GetGeometry
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	MESH_GROUP_GEOMETRY::GetGeometry() const
{
	return (_pGeometry);
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	MESH_GROUP_GEOMETRY::GetMaterial() const
{
	return (_ciMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		GetBBox
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
const AABB &	MESH_GROUP_GEOMETRY::GetBoundingBox() const
{
	return (_BBox);
}

//-----------------------------------------------------------------------------
//	Name:		GetBSphere
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
const SPHERE	MESH_GROUP_GEOMETRY::GetBoundingSphere() const
{
	return (_BSphere);
}
/*
//-----------------------------------------------------------------------------
//	Name:		SetGeometry
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetGeometry(GEOMETRY_ABC *	pGeometry)
{
	_pGeometry = pGeometry;
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetBoundingBox(const AABB &	BBox)
{
	_BBox = BBox;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingSphere
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetBoundingSphere(const SPHERE &	BSphere)
{
	_BSphere = BSphere;
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleSided
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetDoubleSided(bool	b)
{
	_Flags.Set(MGF_DOUBLE_SIDED, b);
}

//-----------------------------------------------------------------------------
//	Name:		NeedLightsOnPrelit
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::NeedLightsOnPrelit() const
{
	return (_Flags.IsSet(MGF_NEED_LIGHTS_ON_PRELIT));
}

//-----------------------------------------------------------------------------
//	Name:		NeedLightsOnDynamicLit
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::NeedLightsOnDynamicLit() const
{
	return (_Flags.IsSet(MGF_NEED_LIGHTS_ON_DYNAMICLIT));
}

//-----------------------------------------------------------------------------
//	Name:		SetNeedLightsOnPrelit
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetNeedLightsOnPrelit(bool	b)
{
	_Flags.Set(MGF_NEED_LIGHTS_ON_PRELIT, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetNeedLightsOnDynamicLit
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetNeedLightsOnDynamicLit(bool	b)
{
	_Flags.Set(MGF_NEED_LIGHTS_ON_DYNAMICLIT, b);
}

//-----------------------------------------------------------------------------
//	Name:		IsDoubleSided
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::IsDoubleSided() const
{
	return (_Flags.IsSet(MGF_DOUBLE_SIDED));
}

//-----------------------------------------------------------------------------
//	Name:		GetEntityFog
//	Object:		
//	04-08-12:	ELE - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::GetEntityFog() const
{
	return (_Flags.IsSet(MGF_ENTITY_FOG));
}

//-----------------------------------------------------------------------------
//	Name:		SetCullingOnShadowing
//	Object:		
//	03-10-27:	FPR - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetCullingOnShadowing(bool	b)
{
	_Flags.Set(MGF_CULLING_ON_SHADOWING, b);
}

//-----------------------------------------------------------------------------
//	Name:		DoClipping
//	Object:		
//	04-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::DoClipping() const
{
	return (_Flags.IsSet(MGF_DO_CLIPPING));
}

//-----------------------------------------------------------------------------
//	Name:		DoShadowCulling
//	Object:		
//	03-10-27:	FPR - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::DoShadowCulling() const
{
	return (_Flags.IsSet(MGF_CULLING_ON_SHADOWING));
}

//-----------------------------------------------------------------------------
//	Name:		DoShadowClipping
//	Object:		
//	04-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP_GEOMETRY::DoShadowClipping() const
{
	return (_Flags.IsSet(MGF_DO_SHADOW_CLIPPING));
}

//-----------------------------------------------------------------------------
//	Name:		SetSharedVBIndex
//	Object:		
//	04-06-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetSharedVBIndex(unsigned int	n)
{
	_nSharedVertexBufferIndex = n;
}

//-----------------------------------------------------------------------------
//	Name:		GetSharedVBIndex
//	Object:		
//	04-06-09:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	MESH_GROUP_GEOMETRY::GetSharedVBIndex() const
{
	return _nSharedVertexBufferIndex;
}
/*
//-----------------------------------------------------------------------------
//	Name:		SetSharedVB
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetSharedVB(GEOMETRY_ABC *	pSharedVB)
{
	_pGeometry->SetSharedVertexBuffer(pSharedVB);
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetEntityFog
//	Object:		
//	04-08-12:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP_GEOMETRY::SetEntityFog(bool	b)
{
	_Flags.Set(MGF_ENTITY_FOG, b);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
