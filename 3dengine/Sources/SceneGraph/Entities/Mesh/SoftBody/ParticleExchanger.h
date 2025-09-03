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
//	CLASS:	PARTICLE_EXCHANGER
//	The PARTICLE_EXCHANGER class implements ...
//
//	02-03-13:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class PARTICLE_EXCHANGER
{
private :
	unsigned int _NbrPoints;
	POINT3D *_pPoints;
	QDT_DLIST<int> *_pGeometryIndex;
	QDT_DLIST<int> *_pVertexIndex;
	float *_pTargetCoeff;
	
public :
	/** */
	PARTICLE_EXCHANGER();
	/** */
	virtual ~PARTICLE_EXCHANGER();

	/** */
	void SetIndices(const unsigned int NbrPoints, const POINT3D *pPts, const QDT_DLIST<int> *pGI, const QDT_DLIST<int> *pVI);
	/** */
	bool Read(IO_STREAM &s);
	/** */
	bool Write(IO_STREAM &s) const;
	/** */
	POINT3D* GetPoint(unsigned int i);
	/** */
	unsigned int GetNbrPoints() const;
	/** */
	POINT3D* GetPoints();
	/** */
	QDT_DLIST<int>* GetGeometryIndex();
	/** */
	QDT_DLIST<int> * GetVertexIndex();
	/** */
	void SetTargetCoeff(const unsigned int i, const float V);
	/** */
	float GetTargetCoeff(const unsigned int i) const;

private :
	/** */
	void Clean();
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
