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
//	CLASS:	DATAS_PSEUDO_DYNAMIC_LIT
//	The DATAS_PSEUDO_DYNAMIC_LIT class implements ...
//
//	02-06-28:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	DATAS_PSEUDO_DYNAMIC_LIT
{
public:

	/** Constructor & destructor */
								DATAS_PSEUDO_DYNAMIC_LIT();
								DATAS_PSEUDO_DYNAMIC_LIT(const DATAS_PSEUDO_DYNAMIC_LIT & C);
							   ~DATAS_PSEUDO_DYNAMIC_LIT();

	/** Operator = */
	DATAS_PSEUDO_DYNAMIC_LIT &	operator = (const DATAS_PSEUDO_DYNAMIC_LIT & C);

	/** Accessors */
	unsigned int								GetNbrMaterials() const							{ return (_vMaterials.GetSize()); }
	unsigned int								GetNbrVertices() const							{ return (_vVertices.GetSize()); }
	unsigned int								GetNbrVerticesPacked() const					{ return (_vVerticesPacked.GetSize()); }
	const MATERIAL_PSEUDO_DYNAMIC_LIT &			GetMaterial(unsigned int i) const				{ return (_vMaterials[i]); }
	const VERTICES_PSEUDO_DYNAMIC_LIT &			GetVertices(unsigned int i) const				{ return (_vVertices[i]); }
	const VERTICES_PSEUDO_DYNAMIC_LIT_PACKED &	GetVerticesPacked(unsigned int i) const			{ return (_vVerticesPacked[i]); }
	MATERIAL_PSEUDO_DYNAMIC_LIT &				GetMaterial(unsigned int i) 					{ return (_vMaterials[i]); }
	VERTICES_PSEUDO_DYNAMIC_LIT &				GetVertices(unsigned int i) 					{ return (_vVertices[i]); }
	VERTICES_PSEUDO_DYNAMIC_LIT_PACKED &		GetVerticesPacked(unsigned int i) 				{ return (_vVerticesPacked[i]); }
	const COLOR &								GetPreviousColor() const						{ return (_PreviousColor); }

	/** Update */
	void						Update(const COLOR &NewColor);
	void						SetToZero(unsigned int nIndex, bool bPacked);
	void						Restore(unsigned int nIndex, bool bPacked);

	/** Push */
	void						PushMaterial(const MATERIAL_PSEUDO_DYNAMIC_LIT	&MaterialPDL);
	void						PushVertices(const VERTICES_PSEUDO_DYNAMIC_LIT &VerticesPDL);
	void						PushVerticesPacked(const VERTICES_PSEUDO_DYNAMIC_LIT_PACKED &VerticesPDL);

	/** Stream */
	void						Write(IO_STREAM &Stream) const;
	void						ReadV0(IO_STREAM &Stream);
	void						ReadV1(IO_STREAM &Stream);

	void						WriteBinary(IO_STREAM &Stream);
	void						InitBinary();
	void						CloseBinary();

private:

	void						Copy(const DATAS_PSEUDO_DYNAMIC_LIT & C);

	COLOR						_PreviousColor;

	QDT_VECTOR<MATERIAL_PSEUDO_DYNAMIC_LIT>			_vMaterials;
	QDT_VECTOR<VERTICES_PSEUDO_DYNAMIC_LIT>			_vVertices;
	QDT_VECTOR<VERTICES_PSEUDO_DYNAMIC_LIT_PACKED>	_vVerticesPacked;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
