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
//	CLASS:	GEOMETRY_LIGHT_TRACK
//	The GEOMETRY_LIGHT_TRACK class implements ...
//
//	02-10-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PORTAL_ABC;

class	GEOMETRY_LIGHT_TRACK
{
public:

	// Constructors & destructor
							GEOMETRY_LIGHT_TRACK();
							GEOMETRY_LIGHT_TRACK(const GEOMETRY_LIGHT_TRACK & C);
						   ~GEOMETRY_LIGHT_TRACK();
	
	// Operators
	GEOMETRY_LIGHT_TRACK &	operator = (const GEOMETRY_LIGHT_TRACK & C);

	// Update during interpolation
	void					Update(GEOMETRY_ABC *pGeometry, float rRatio);

	// Clear vectors
	void					Clear();
								
	// Stream
	void					ReadAndApplyColors(IO_STREAM &Stream, 
											   MESH		 *pMesh, 
											   int		  nNumGeometry);
	void					ReadAndApplyColors(IO_STREAM	&Stream, 
											   PORTAL_ABC	*pPortal, 
											   int			nNumGeometry);
	void					Read(IO_STREAM & Stream);
	void					Write(IO_STREAM & Stream) const;

	// Accessors
	int						GetNbrVertices() const		{ return (_nNbrVertices); }
	COLOR					GetColor(int i) const		{ return (_pColors[i]); }

	// Modifiors
	void					SetNbrVertices(int n);
	void					SetColor(int i, COLOR C)	{ _pColors[i] = C; }

	void					PushPseudoDynamicInfos(const COM_INSTANCE & LightInst, int nIndex, bool bPacked);
	
private:

	void					Copy(const GEOMETRY_LIGHT_TRACK & C);

	int						_nNbrVertices;
	COLOR				   *_pColors;

	// Pseudo dynamic lights
	QDT_VECTOR<COM_INSTANCE>	_vPseudoDynamicLightInst;
	QDT_VECTOR<int>				_vVerticesPseudoDynamicLitIndex;
//	QDT_VECTOR<bool>			_vbPacked;
	QDT_VECTOR<unsigned char>	_vbPacked;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
