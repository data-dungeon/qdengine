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

class	VERTICES_PSEUDO_DYNAMIC_LIT
{
public:

	/** Constructor & desctructor */
									VERTICES_PSEUDO_DYNAMIC_LIT();
									VERTICES_PSEUDO_DYNAMIC_LIT(const VERTICES_PSEUDO_DYNAMIC_LIT & C);
								   ~VERTICES_PSEUDO_DYNAMIC_LIT();

	/** Operator = */
	VERTICES_PSEUDO_DYNAMIC_LIT &	operator = (const VERTICES_PSEUDO_DYNAMIC_LIT & C);

	/** Update */
	void							Update(const COLOR &PreviousColor, const COLOR &NewColor);

	/** Accessors */
	unsigned int					GetGeometry() const	{ return (_nNumGeometry); }
	const COM_INSTANCE &			GetMeshInst() const	{ return (_MeshInst); }

	/** Modifiors */
	void							SetGeometry(unsigned int nGeometry);
	void							SetMesh(const COM_INSTANCE & _MeshInst);
	void							SetVerticesPseudoDynamicLitABC(VERTICES_PSEUDO_DYNAMIC_LIT_ABC* pVertices);
	
	void							Write(IO_STREAM &Stream) const;
	void							ReadV0(IO_STREAM &Stream);
	void							ReadV1(IO_STREAM &Stream);

private:

	void							Copy(const VERTICES_PSEUDO_DYNAMIC_LIT & C);

	COM_INSTANCE					_MeshInst;
	unsigned int					_nNumGeometry;

	VERTICES_PSEUDO_DYNAMIC_LIT_ABC*_pVertices;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
