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
//	CLASS:	MOTEUR_3D_MODULE
//
//	02-07-31:	AMA - Created
//*****************************************************************************
#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(M3D/Moteur3D)
#include	INCL_KCOM(Streaming/CommunicatorPartitionDescriptor)
#include	INCL_KCOM(CommunicatorType/CommunicatorContainerDescriptor)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Behavior/Behaviors/Node Behaviors/CamFree)
#include	INCL_3DENGINE(Behavior/AnimNode/AnimNode)
#include	INCL_3DENGINE(Behavior/HipsAnim/HipsAnim)
#include	INCL_3DENGINE(Behavior/TargetAnim/TargetAnim)
#include	INCL_3DENGINE(Behavior/Datas/AnimPC)
#include	INCL_3DENGINE(Behavior/Datas/ColorAnimData)
#include	INCL_3DENGINE(Behavior/Datas/NumericAnimData)
#include	INCL_3DENGINE(Behavior/Datas/TextureAnimData)
#include	INCL_3DENGINE(Behavior/Datas/BlendShapeAnim/BlendShapePC)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamAnimation)
#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamAnimationOffset)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnim)
#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnimOffset)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_ElementsList)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Interface)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Page)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementPolyABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementQuad)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementTriangle)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementBitmap)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementRect)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementText)
#include	INCL_3DENGINE(Interface2D3D/DisplayFont/DisplayFont)
#include	INCL_3DENGINE(Interface2D3D/SpriteBank/SpriteBank)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/Viewport/ViewportList)
#include	INCL_3DENGINE(Display/Viewport/Filter/Filter)
#include	INCL_3DENGINE(Display/Viewport/Filter/FilterLayer)
#include	INCL_3DENGINE(Display/Viewport/Filter/FilterScratch)
//#include	INCL_3DENGINE(Collision/CollisionManager)
//#include	INCL_3DENGINE(Collision/CollisionNotifier)
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/ForceFieldABC)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/UniformField/UniformField)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/RadialField/RadialField)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/VortexField/VortexField)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/LifeField/LifeField)
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/PerturbationField/PerturbationField)
#include	INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include	INCL_3DENGINE(SceneGraph/Entities/Joint/Joint)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/PointLight)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/SpotLight)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/DirectionalLight)
#include	INCL_3DENGINE(SceneGraph/Entities/Glow/Glow)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/MeshData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/DynamicPhysicData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/StaticPhysicData)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollData)
#include	INCL_3DENGINE(SceneGraph/Entities/Constraint/Constraint)
#include	INCL_3DENGINE(SceneGraph/Entities/Constraint/ConstraintData)
#include	INCL_3DENGINE(SceneGraph/Entities/Trigger/Trigger)
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleShape)
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleShapeSprite)
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleEmitter)
#include	INCL_3DENGINE(SceneGraph/Entities/Interface3d/Interface3d)
#include	INCL_3DENGINE(SceneGraph/Entities/PrimitiveSet/PrimitiveSet)
#include	INCL_3DENGINE(SceneGraph/Entities/WayPoint/WayPoint)
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CollisionMesh)
#include	INCL_3DENGINE(SceneGraph/Entities/QdTransform/QdTransform)
#include	INCL_3DENGINE(SceneGraph/Entities/AmbientOctree/AmbientOctree)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollData)
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJoint)
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJointRevolute)
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJointSpherical)
#include	INCL_3DENGINE(SceneGraph/Raycaster/Raycaster)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderColorPerVertex)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderCustom)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderDefault)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderError)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderShadow)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFile)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFlow)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureBank)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(SceneGraph/Node/NodeMapper)

CLASS_DESCRIPTOR*	MOTEUR_3D_MODULE::_ModuleTypeLookUpTable[NB_MOTEUR3D_TYPES];
	
//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MODULE_SCRIPT_TYPES constructor
//	Object:		
//	02-07-31:	AMA - Created
//-----------------------------------------------------------------------------
MOTEUR_3D_MODULE::MOTEUR_3D_MODULE()
:
MODULE_ABC(MODULE_ENGINE3D, _ModuleTypeLookUpTable, NB_MOTEUR3D_TYPES)
{
	// Obsolete
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_TEXTURE_LAYER, "TEXTURE_LAYER"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FREE1, "FREE1"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FREE2, "FREE2"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FREE3, "FREE3"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FREE4, "FREE4"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FREE5, "FREE5"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_ANIM_INST, "ANIM_INST"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BLEND_SHAPE_INST, "BLEND_SHAPE_INST"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BEHAVIOR_CONSTRAINT_AIM, "BEHAVIOR_CONSTRAINT_AIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BEHAVIOR_MOVE_TO, "BEHAVIOR_MOVE_TO"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FLOW_DIALOG, "FLOW_DIALOG"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BEHAVIOR_INERT_ROT, "BEHAVIOR_INERT_ROT"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BEHAVIOR_CAM_TARGET, "BEHAVIOR_CAM_TARGET"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BEHAVIOR_BLENDER, "BEHAVIOR_BLENDER"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PATHFINDING, "PATHFINDING"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BLEND_SHAPE_BEHAVIOR, "BLEND_SHAPE_BEHAVIOR"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PATH, "PATH"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PATH_FINDER, "PATH_FINDER"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_TRACK_ANIM, "TRACK_ANIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_IHM_ELEMENT_MESH, "IHM_ELEMENT_MESH"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_QTE, "QTE"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BLENDER_ANIM, "BLENDER_ANIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_HIPS_BLENDER_ANIM, "HIPS_BLENDER_ANIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_HIPS_ANIM_COMB, "HIPS_ANIM_COMB"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_SOFT_BODY, "SOFT_BODY"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_GRAPHIC_OPTIONS, "GRAPHIC_OPTIONS"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PATH_UNIT, "PATH_UNIT"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_OBJECT, "COLLISION_OBJECT"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_MAP, "COLLISION_MAP"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_DISPLAY_DUMMIES, "DISPLAY_DUMMIES"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_PORTAL, "COLLISION_PORTAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_UV_ANIM_DATA, "UV_ANIM_DATA"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_TRACK_FLOAT, "TRACK_FLOAT"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_TRACK_POINT_2D, "TRACK_POINT_2D"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_UV_ANIM, "UV_ANIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_TRACK_COLOR, "TRACK_COLOR"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_LIGHT_ANIM_DATA, "LIGHT_ANIM_DATA"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_LIGHT_ANIM, "LIGHT_ANIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_OBJECT_TREE, "COLLISION_OBJECT_TREE"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_SEPARATE_LIGHTSET, "SEPARATE_LIGHTSET"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_LIGHT_TRACK, "LIGHT_TRACK"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PRIMITIVE_SET, "PRIMITIVE_SET"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_LIGHT_ABC, "LIGHT_ABC"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PORTAL_ABC, "PORTAL_ABC"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_MATERIAL_ANIM, "MATERIAL_ANIM"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_MATERIAL_ANIM_DATA, "MATERIAL_ANIM_DATA"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_NAVIGATION_MESH, "NAVIGATION_MESH"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_FREE6, "FREE6"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BLEND_SHAPE, "BLEND_SHAPE"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_MATERIAL, "MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_MATERIAL_ABC, "MATERIAL_ABC"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PRELIT_MATERIAL, "PRELIT_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_LAMBERT_MATERIAL, "LAMBERT_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_BLINN_MATERIAL, "BLINN_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_PHONG_MATERIAL, "PHONG_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_MICROSCOPIC_MATERIAL, "MICROSCOPIC_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_DYNAMIC_LIT_MATERIAL, "DYNAMIC_LIT_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_CARTOON_MATERIAL, "CARTOON_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_SKL_NODE, "SKL_NODE"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLOR_PER_VERTEX_MATERIAL, "COLOR_PER_VERTEX_MATERIAL"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_SCENE_GRAPH, "SCENE_GRAPH"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_QUADTREE, "QUADTREE"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_MESH_GROUP, "MESH_GROUP"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_MESH, "COLLISION_MESH"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_MANAGER, "COLLISION_MANAGER"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_COLLISION_NOTIFIER, "COLLISION_NOTIFIER"));
	AddClassDescriptor(COMMUNICATOR_DESCRIPTOR::CreateEmptyCommunicatorDescriptor(OTYPE_MOTEUR3D_SKIN, "SKIN"));

	// used types
	AddClassDescriptor(ENTITY_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(NODE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(NODE_ARRAY::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENTS_LIST::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_INTERFACE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_PAGE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_RECT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_BITMAP::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_TEXT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_POLY_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_QUAD::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(IHM_ELEMENT_TRIANGLE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(MOTEUR_3D::CreateSingletonDescriptor());
	AddClassDescriptor(DISPLAY_MANAGER::CreateSingletonDescriptor());
	AddClassDescriptor(PHYSIC_MANAGER::CreateSingletonDescriptor());
	AddClassDescriptor(INTERFACE_MANAGER::CreateSingletonDescriptor());
	AddClassDescriptor(BEHAVIOR_MANAGER::CreateSingletonDescriptor());
	AddClassDescriptor(RAY_CASTER::CreateClassDescriptor());
	AddClassDescriptor(VIEWPORT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(VIEWPORT_LIST::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(DISPLAY_FONT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(CAMERA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(FORCE_FIELD_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(UNIFORM_FIELD::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(RADIAL_FIELD::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(VORTEX_FIELD::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(LIFE_FIELD::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(JOINT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(LIGHT_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(DIRECTIONAL_LIGHT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(POINT_LIGHT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SPOT_LIGHT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(MESH::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(MESH_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(DYNAMIC_PHYSIC_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(STATIC_PHYSIC_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SKELETON::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(RAGDOLL_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(CONSTRAINT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(CONSTRAINT_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(TRIGGER::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PARTICLE_EMITTER::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PARTICLE_SHAPE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PARTICLE_SHAPE_SPRITE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(QD_TRANSFORM::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(GLOW::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(INTERFACE_3D::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PRIMITIVE_SET::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(WAY_POINT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(ANIM_PC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(BLEND_SHAPE_PC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(ANIM_NODE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(HIPS_ANIM::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(TARGET_ANIM::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(CAM_FREE::CreateGenericCommunicatorDescriptor());	
	AddClassDescriptor(STREAM_ANIMATION::CreateGenericCommunicatorDescriptor());	
	AddClassDescriptor(STREAM_ANIMATION_OFFSET::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(STREAMABLE_BLEND_SHAPE_ANIM::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SPRITE_BANK::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PERTURBATION_FIELD::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(COLOR_ANIM_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(NUMERIC_ANIM_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(TEXTURE_ANIM_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(FILTER::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(FILTER_LAYER::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(FILTER_SCRATCH::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(NODE_MAPPER::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PHYSIC_JOINT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PHYSIC_JOINT_REVOLUTE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(PHYSIC_JOINT_SPHERICAL::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SHADER_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SHADER_COLOR_PER_VERTEX::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SHADER_CUSTOM::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SHADER_DEFAULT::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SHADER_ERROR::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(ENGINE_TEXTURE_ABC::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(ENGINE_TEXTURE_FILE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(SHADER_SHADOW::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(AMBIENT_OCTREE::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(RAGDOLL_DATA::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(ENGINE_TEXTURE_FLOW::CreateGenericCommunicatorDescriptor());
	AddClassDescriptor(ENGINE_TEXTURE_BANK::CreateGenericCommunicatorDescriptor());

	InitModule();

#ifdef QDT_DEBUG
	VerifyModule();
#endif
}

//-----------------------------------------------------------------------------
//	Name:		MODULE_SCRIPT_TYPES destructor
//	Object:		
//	02-07-31:	AMA - Created
//-----------------------------------------------------------------------------
MOTEUR_3D_MODULE::~MOTEUR_3D_MODULE()
{
	// Do Nothing...
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

