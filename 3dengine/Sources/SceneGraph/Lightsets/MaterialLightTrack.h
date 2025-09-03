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
//	CLASS:	MATERIAL_LIGHT_TRACK
//	The MATERIAL_LIGHT_TRACK class implements ...
//
//	02-10-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	MATERIAL_LIGHT_TRACK
{
public:

	// Constructors & destructor
							MATERIAL_LIGHT_TRACK();
							MATERIAL_LIGHT_TRACK(const MATERIAL_LIGHT_TRACK & C);
						   ~MATERIAL_LIGHT_TRACK();

	// Operators
	MATERIAL_LIGHT_TRACK &	operator = (const MATERIAL_LIGHT_TRACK & C);


	// Modifiors
	void					SetMaterialInst(const COM_INSTANCE & MatInst)		{ _MaterialInst = MatInst; }
	void					SetTextureInst(const COM_INSTANCE & TextureInst)	{ _TextureInst = TextureInst; }
	void					SetLayer(int nLayer)								{ _nNumLayer = nLayer; }
	
	// Accessors
	const COM_INSTANCE &	GetMaterialInst()									{ return (_MaterialInst); }

	// Stream
	void					Preload(IO_STREAM & Stream);
	void					LoadAndApply(IO_STREAM & Stream);
	void					Write(IO_STREAM & Stream) const;
	void					Update(float rRatio, bool bEnd);
	void					UpdateTextureIndex();
	
private:

	void					Copy(const MATERIAL_LIGHT_TRACK & C);

	COM_INSTANCE			_MaterialInst;
	COM_INSTANCE			_TextureInst;
	COM_INSTANCE			_OldTextureInst;
	int						_nNumLayer;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
