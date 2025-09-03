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
//	CLASS:	MESH_LIGHT_TRACK
//	The MESH_LIGHT_TRACK class implements ...
//
//	02-10-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	MESH_LIGHT_TRACK
{
public:

	// Constructors & destructor
										MESH_LIGHT_TRACK();
										MESH_LIGHT_TRACK(const MESH_LIGHT_TRACK & C);
									   ~MESH_LIGHT_TRACK();

	// Operators
	MESH_LIGHT_TRACK &					operator = (const MESH_LIGHT_TRACK & C);

	void								Clear();
	void								Push(const GEOMETRY_LIGHT_TRACK &GLT);
	void								Update(float rRatio);

	// Accessors
	const COM_INSTANCE &				GetMeshInst() const				{ return (_MeshInst); }
	unsigned int						GetNbrGeometryTracks() const	{ return (_vGeometryTracks.GetSize()); }
	const GEOMETRY_LIGHT_TRACK &		GetGeometryTrack(int i) const	{ return (_vGeometryTracks[i]); }

	// Modifiors
	void								SetMeshInst(const COM_INSTANCE & Inst)		{ _MeshInst = Inst; }

	// Stream
	void								LoadAndApply(IO_STREAM & Stream);
	void								Preload(IO_STREAM & Stream);
	void								Write(IO_STREAM & Stream) const;
	
private:

	void								Copy(const MESH_LIGHT_TRACK & C);
	MESH*								GetMesh() const;
	PORTAL_ABC*							GetPortal() const;

	int									_nMeshType;	// Mesh, mirror or portal
	COM_INSTANCE						_MeshInst;
	QDT_VECTOR<GEOMETRY_LIGHT_TRACK>	_vGeometryTracks;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
