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
//	CLASS:	VERTICES_PSEUDO_DYNAMIC_LIT
//	The VERTICES_PSEUDO_DYNAMIC_LIT class implements ...
//
//	02-06-28:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	VERTICES_PSEUDO_DYNAMIC_LIT_PACKED
{
public:

	/** Constructor & desctructor */
											VERTICES_PSEUDO_DYNAMIC_LIT_PACKED();
											VERTICES_PSEUDO_DYNAMIC_LIT_PACKED(const VERTICES_PSEUDO_DYNAMIC_LIT_PACKED & C);
										   ~VERTICES_PSEUDO_DYNAMIC_LIT_PACKED();

	/** Operator = */
	VERTICES_PSEUDO_DYNAMIC_LIT_PACKED &	operator = (const VERTICES_PSEUDO_DYNAMIC_LIT_PACKED & C);

	/** Allocate memory for nNbrVertices */
	void									AllocateVertices(unsigned int nNbrVertices);

	/** Update */
	void									Update(const COLOR &PreviousColor, const COLOR &NewColor);

	/** Accessors */
	unsigned int							GetGeometry() const	{ return (_nNumGeometry); }
	const COM_INSTANCE &					GetMeshInst() const	{ return (_MeshInst); }

	/** Modifiors */
	void									SetValue(unsigned int nIndex, unsigned char nNumVertex, unsigned char Lighting);
	void									SetGeometry(unsigned int nGeometry);
	void									SetMesh(const COM_INSTANCE & MeshInst);
	
	/** Stream */
	void									Write(IO_STREAM &Stream) const;
	void									ReadV0(IO_STREAM &Stream);
	void									ReadV1(IO_STREAM &Stream);

private:

	void									Copy(const VERTICES_PSEUDO_DYNAMIC_LIT_PACKED & C);

	COM_INSTANCE							_MeshInst;
	unsigned int							_nNumGeometry;

	VERTICES_PSEUDO_DYNAMIC_LIT_ABC		   *_pVertices;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
