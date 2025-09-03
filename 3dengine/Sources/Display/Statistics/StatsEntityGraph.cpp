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
//	CLASS:	STATS_ENTITY_GRAPH
//
//	06-03-24:	VHA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Statistics/StatsEntityGraph)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STATS_ENTITY_GRAPH constructor
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
STATS_ENTITY_GRAPH::STATS_ENTITY_GRAPH() : STATS_ENTITY_ABC(),
_fVerticalMin(0.f),
_fVerticalMax(0.f)
{
//		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		STATS_ENTITY_GRAPH destructor
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
STATS_ENTITY_GRAPH::~STATS_ENTITY_GRAPH()
{
		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-03-27:	VHA - Created
//-----------------------------------------------------------------------------
void	STATS_ENTITY_GRAPH::Display()
{
	RENDER_CONTEXT::EnableDepthTest(false);

	DISPLAY_MANAGER::Instance()->EnableSkinning(false);
	DISPLAY_MANAGER::Instance()->SetWorldMatrix(MATRIX::IDENTITY);
	DISPLAY_MANAGER::Instance()->SetViewMatrix(MATRIX::IDENTITY);
	DISPLAY_MANAGER::Instance()->SetProjMatrix(MATRIX::IDENTITY);

	SHADER_ABC* pMaterial = (SHADER_ABC *)DISPLAY_MANAGER::Instance()->GetLineShader().GetCommunicator();

	QDT_ASSERT(pMaterial->GetNbrPasses() == 1);

	pMaterial->SetPass(0, VECTOR(0,0,0));

	POINT2D points[4];
	COLOR colors[4];

	Float32 fTop;
	Float32 fBottom;
	Float32 fLeft;
	Float32 fRight;

	fTop = 1-GetDisplayRangeTop()/50.f;
	fBottom = 1-GetDisplayRangeBottom()/50.f;
	fLeft = -1+GetDisplayRangeLeft()/50.f;
	fRight = -1+GetDisplayRangeRight()/50.f;

	// compute Min/Max
	Float32 fMin = QDT_FLOAT32_MAX;
	Float32 fMax = QDT_FLOAT32_MIN;
	Float32 fHorizontalMax = 0;
	Float32 fHorizontalMin = 0;
	Float32 fHRange = 0.f;

	UInt32 j;
	for(j=0;j<_pTable.GetSize();j++)
	{
		UInt32 i;
		TIME_LOG* pTimeLog = _pTable[j]._pTimeLog;

		if(!IsTimeLogInRange(pTimeLog))
			continue;

		UInt32 numEntries = pTimeLog->GetLogSize();

		// update range
		for(i=0;i<numEntries;i++)
		{
			TIME_LOG::S_TIME_LOG_ELEMENT* pElement = pTimeLog->GetEntryIdx(i);

			if(pElement->_fValue < fMin)
				fMin = pElement->_fValue;

			if(pElement->_fValue > fMax)
				fMax = pElement->_fValue;

			if(pElement->_fTime > fHorizontalMax)
				fHorizontalMax = pElement->_fTime;
		}
		if(fMin < _fVerticalMin)
			_fVerticalMin = fMin;
		if(fMax > _fVerticalMax)
			_fVerticalMax = fMax;

		fHorizontalMin = fHorizontalMax - pTimeLog->GetRange();

		if(fHorizontalMin<0)
		{
			fHorizontalMin = 0;
		}

		if(fHRange<pTimeLog->GetRange())
		{
			QDT_ASSERT(fHRange == 0.f); // if this assert is ever raised, we are graphing timelogs with different ranges
			fHRange = pTimeLog->GetRange();
		}
	}

	// precompute some scalling values
	Float32 fVerticalScale = (GetDisplayRangeBottom() - GetDisplayRangeTop())/(_fVerticalMax-_fVerticalMin);
	Float32 fHorizontalScale = (GetDisplayRangeRight() - GetDisplayRangeLeft())/(fHRange);

	// Display the ranges
	for(j=0;j<_colorRange.GetSize();j++)
	{
		POINT2D dummyUVs[4];

		STATS_COLOR_RANGE* pRange = &_colorRange[j];
		Float32 fRangeTop;
		Float32 fRangeBottom;

		fRangeBottom = 1-(GetDisplayRangeBottom()-(pRange->_fStartValue-_fVerticalMin)*fVerticalScale)/50.f;
		fRangeTop = 1-(GetDisplayRangeBottom()-(pRange->_fStopValue-_fVerticalMin)*fVerticalScale)/50.f;

		//quick-eject out of range
		if(fRangeBottom > fTop)
			continue;
		if(fRangeTop < fBottom)
			continue;

		// clamping
		if(fRangeTop > fTop)
			fRangeTop = fTop;
		if(fRangeBottom < fBottom)
			fRangeBottom = fRangeBottom;

		points[0].SetXY(	fLeft,	fRangeTop);
		points[1].SetXY(	fLeft,	fRangeBottom);
		points[2].SetXY(	fRight,	fRangeTop);
		points[3].SetXY(	fRight,	fRangeBottom);

		COLOR transparentColor = pRange->_color;
		transparentColor.SetAlpha(40);

		colors[0] = transparentColor;
		colors[1] = transparentColor;
		colors[2] = transparentColor;
		colors[3] = transparentColor;

		DISPLAY_MANAGER::Instance()->PushQuad(points, dummyUVs, colors);
		DISPLAY_MANAGER::Instance()->FlushQuads();
	}

	// display borders
	points[0].SetXY(fLeft,fTop);
	colors[0] = COLOR::WHITE;
	DISPLAY_MANAGER::Instance()->PushLine(points, colors);
	points[0].SetXY(fLeft,fBottom);
	colors[0] = COLOR::WHITE;
	DISPLAY_MANAGER::Instance()->PushLine(points, colors);
	points[0].SetXY(fRight,fBottom);
	colors[0] = COLOR::WHITE;
	DISPLAY_MANAGER::Instance()->PushLine(points, colors);
	DISPLAY_MANAGER::Instance()->FlushLines();

	// draw max vertical scale
	QDT_STRING maxVerticalScale;
	maxVerticalScale = GetFormatedString("{S:50}Max", COLOR::WHITE, fMax, COLOR::WHITE);
	DISPLAY_MANAGER::Instance()->AddDebugText(maxVerticalScale,GetDisplayRangeLeft()-2.5f,GetDisplayRangeTop()-2.5f); // arbitrary

	// draw time range
	QDT_STRING timeRange;
	timeRange.Format("{S:50}-%0.2f sec", (fHorizontalMax-fHorizontalMin)/1000);
	DISPLAY_MANAGER::Instance()->AddDebugText(timeRange, GetDisplayRangeLeft()-2.5f, GetDisplayRangeBottom()+0.5f); // arbitrary

	for(j=0;j<_pTable.GetSize();j++)
	{
		UInt32 i;
		TIME_LOG* pTimeLog = _pTable[j]._pTimeLog;

		if(!IsTimeLogInRange(pTimeLog))
			continue;

		UInt32 numEntries = pTimeLog->GetLogSize();

		Float32 Y1;
		Float32 X1;

		// actual graph drawing
		for(i=0;i<numEntries;i++)
		{
			Float32 fTime1 = i*pTimeLog->GetLogStep();

			if(fHorizontalMax<fHorizontalMin+fTime1)
				break;

			Float32 fValue1 = pTimeLog->GetValueAtTime(fHorizontalMin+fTime1);

			// remap to bottom-top space
			Y1 = GetDisplayRangeBottom()-(fValue1-_fVerticalMin)*fVerticalScale;

			X1 = GetDisplayRangeLeft()+(fTime1)*fHorizontalScale;

			points[0].SetXY(	-1+X1/50.f,	1-Y1/50.f);
			colors[0] = _pTable[j]._color;
			DISPLAY_MANAGER::Instance()->PushLine(points, colors);
		}
		DISPLAY_MANAGER::Instance()->FlushLines();

		QDT_STRING curveName = QDT_STRING("{S:50}");
		Float32 fFinalValue = pTimeLog->GetLastElement()->_fValue;

		curveName += GetFormatedString(pTimeLog->GetName(), _pTable[j]._color, fFinalValue, GetColorForValue(fFinalValue));
		DISPLAY_MANAGER::Instance()->AddDebugText(curveName,X1+1,Y1-1.5f); // nota: Arbitrary values to match the debug font
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
