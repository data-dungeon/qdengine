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
//	CLASS:	MESH_GROUP_HDR_REF
//	The MESH_GROUP_HDR_REF class implements ...
//
//	03-10-07:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	MESH_GROUP_HDR_REF
{
public:

								MESH_GROUP_HDR_REF();
							   ~MESH_GROUP_HDR_REF();

	void							Clear();
	void							PushIndex(unsigned int nIndex);
	unsigned int					GetIndex(unsigned int i) const;
	unsigned int					GetSize() const;

	// Modifiors
	void							SetNode(NODE * pN);
	void							SetMeshGroup(MESH_GROUP *pG);

	// Accessors
	NODE *							GetNode() const;
	MESH_GROUP *					GetMeshGroup() const;

	// Display
	void							Display();

private:

	NODE *						_pNode;
	MESH_GROUP *				_pMeshGroup;
	QDT_VECTOR<unsigned int>	_vIndexes;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
