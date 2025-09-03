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
// FILE: Zone.inl
//
//
//	01-02-08:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ZONE::GetDynamicAmbientColor
//	Object:		Get the ambient color
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
const COLOR_FLOAT & ZONE::GetDynamicAmbientColor() const
{
	return (_DynamicAmbientColor);
}

//-----------------------------------------------------------------------------
//	Name:		ZONE::GetStaticAmbientColor
//	Object:		Get the ambient color
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
const COLOR_FLOAT & ZONE::GetStaticAmbientColor() const
{
	return (_StaticAmbientColor);
}

//-----------------------------------------------------------------------------
//	Name:		ZONE::SetDynamicAmbientColor
//	Object:		Set the ambient color
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
void ZONE::SetDynamicAmbientColor(const COLOR_FLOAT & Color)
{
	_DynamicAmbientColor = Color;
}

//-----------------------------------------------------------------------------
//	Name:		ZONE::SetDynamicAmbientColor
//	Object:		Set the ambient color
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
void ZONE::SetStaticAmbientColor(const COLOR_FLOAT & Color)
{
	_StaticAmbientColor = Color;
}

//-----------------------------------------------------------------------------
//	Name:		ZONE::SetBoundingSphere
//	Object:		Set the bounding sphere
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void ZONE::SetBoundingSphere(const SPHERE & Sphere)
{
	_BSphere = Sphere;
}

//-----------------------------------------------------------------------------
//	Name:		ZONE::GetBoundingSphere
//	Object: Get the bounding sphere
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
const SPHERE & ZONE::GetBoundingSphere()const
{
	return (_BSphere) ;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogEnable
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	ZONE::SetFogEnable(bool	b)
{
	_bFogEnable = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogNear
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	ZONE::SetFogNear(float	rval)
{
	_rFogNear = rval;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogFar
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	ZONE::SetFogFar(float	rval)
{
	_rFogFar = rval;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogColor
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	ZONE::SetFogColor(const COLOR &	C)
{
	_FogColor = C;
}

//-----------------------------------------------------------------------------
//	Name:		IsFogEnable
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
bool	ZONE::IsFogEnable()const
{
	return ( _bFogEnable);
}

//-----------------------------------------------------------------------------
//	Name:		GetFogNear
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
float	ZONE::GetFogNear()const
{
	return ( _rFogNear );
}

//-----------------------------------------------------------------------------
//	Name:		GetFogFar
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
float	ZONE::GetFogFar()const
{
	return ( _rFogFar );
}

//-----------------------------------------------------------------------------
//	Name:		GetFogColor
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
const COLOR &	ZONE::GetFogColor()const
{
	return ( _FogColor );
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingBox
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
const AABB &	ZONE::GetBoundingBox()const
{
	return ( _BBox);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	ZONE::SetBoundingBox(const AABB &	Box)
{
	_BBox = AABB(Box);
}

//-----------------------------------------------------------------------------
//	Name:		GetParticleSystem
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM *	ZONE::GetParticleSystem()
{
	return	(_pParticleSystem);
}

//-----------------------------------------------------------------------------
//	Name:		GetLightsOnDynamicLit
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_VECTOR < NODE * > &	ZONE::GetLightsOnDynamicLit() const
{
	return	(_vLightsOnDynamicLit);
}

//-----------------------------------------------------------------------------
//	Name:		GetLightsOnPrelit
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_VECTOR < NODE * > &	ZONE::GetLightsOnPrelit() const
{
	return	(_vLightsOnPrelit);
}

//-----------------------------------------------------------------------------
//	Name:		ResetLights
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::ResetLights()
{
	_vLightsOnDynamicLit.Clear();
	_vLightsOnPrelit.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ResetParticleEmitters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::ResetParticleEmitters()
{
	_vParticleEmitters.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ResetShadowEmitters
//	Object:		
//	04-06-01:	ELE - Created
//-----------------------------------------------------------------------------
void	ZONE::ResetShadowEmitters()
{
	_vShadowEmitters.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrShadowEmitters
//	Object:		
//	04-06-01:	ELE - Created
//-----------------------------------------------------------------------------
int	ZONE::GetNbrShadowEmitters() const
{
	return	(_vShadowEmitters.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetMeshGroupNodes
//	Object:		
//	04-06-01:	FPR - Created
//-----------------------------------------------------------------------------
const QDT_DLIST<NODE*>&	ZONE::GetMeshGroupNodes() const
{
	return (_lMeshGroupNodes);
}

//-----------------------------------------------------------------------------
//	Name:		GetMeshNodes
//	Object:		
//	04-06-01:	FPR - Created
//-----------------------------------------------------------------------------
const QDT_DLIST<NODE*>&	ZONE::GetMeshNodes() const
{
	return(_lMeshNodes);
}

//-----------------------------------------------------------------------------
//	Name:		GetShadowEmitter
//	Object:		
//	04-06-01:	ELE - Created
//-----------------------------------------------------------------------------
NODE *	ZONE::GetShadowEmitter(unsigned int	i) const
{
	return	(_vShadowEmitters[i]);
}

//-----------------------------------------------------------------------------
//	Name:		GetForceFields
//	Object:		
//	04-10-25:	RMA - Created
//-----------------------------------------------------------------------------
const QDT_DLIST <NODE *> &	ZONE::GetForceFields()
{
	return (_lForceFieldNodes);
}

//-----------------------------------------------------------------------------
//	Name:		GetPhysicScene
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
I_PHYSIC_SCENE * SCENE_GRAPH::GetPhysicScene() const
{
	return (_pPhysicScene);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================