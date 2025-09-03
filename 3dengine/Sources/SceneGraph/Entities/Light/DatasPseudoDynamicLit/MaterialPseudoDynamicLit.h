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
//	CLASS:	MATERIAL_PSEUDO_DYNAMIC_LIT
//	The MATERIAL_PSEUDO_DYNAMIC_LIT class implements ...
//
//	02-06-28:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	MATERIAL_PSEUDO_DYNAMIC_LIT
{
public:

	/** Constructor & destructor */
									MATERIAL_PSEUDO_DYNAMIC_LIT();
									MATERIAL_PSEUDO_DYNAMIC_LIT(const MATERIAL_PSEUDO_DYNAMIC_LIT & C);
								   ~MATERIAL_PSEUDO_DYNAMIC_LIT();

	/** Operator = */
	MATERIAL_PSEUDO_DYNAMIC_LIT &	operator=(const MATERIAL_PSEUDO_DYNAMIC_LIT & C);

	/** Update */
	void							Update(const COLOR &C);

	/** Accessors */
	_INLINE_ const COM_INSTANCE &	GetMaterialInst() const			{ return (_MaterialInst); }
	_INLINE_ unsigned int			GetNumLayer() const				{ return (_nNumLayer); }

	/** Modifiors */
	_INLINE_ void					SetMaterialInst(const COM_INSTANCE & Inst)	{ _MaterialInst = Inst; }
	_INLINE_ void					SetNumLayer(unsigned int n)					{ _nNumLayer = n; }

	/** Stream operators */
	friend IO_STREAM&				operator <<(IO_STREAM &Stream, const MATERIAL_PSEUDO_DYNAMIC_LIT &MaterialPDL)
									{
										MaterialPDL.Write(Stream);
										return (Stream);
									}	
	
	friend IO_STREAM&				operator >>(IO_STREAM &Stream,		 MATERIAL_PSEUDO_DYNAMIC_LIT &MaterialPDL)
									{
										MaterialPDL.Read(Stream);
										return (Stream);
									}

	void							Write(IO_STREAM &Stream) const;
	void							Read(IO_STREAM &Stream);


private:

	void							Copy(const MATERIAL_PSEUDO_DYNAMIC_LIT & C);

	COM_INSTANCE					_MaterialInst;
	unsigned int					_nNumLayer;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
