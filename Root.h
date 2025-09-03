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
//	File	:	Include.h
//	Category:	Generic include for ALL sources in QD
//				Be extremely cautionous with this file, as any modification
//				will result in all project / all sources recompilation.
//
//	05-11-09:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __QUANTIC_ROOT_INCLUDE_H__
#define __QUANTIC_ROOT_INCLUDE_H__

// Macro used to hint about compiler specific options
#if ( defined(PLATFORM_PC) )
#define USE_PRAGMA_ONCE
#endif

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

// Namespaces definition, place here all possible namespaces you want to use
namespace QDT
{
	namespace M3D			{ }
	namespace EXPCOMMON		{ }
	namespace COMTOOLS		{ }
	namespace EXPTOOLS		{ }
	namespace IAM4			{ }
	namespace IADMIN		{ }
	namespace IAM			{ namespace APP { } }
	namespace IAMLITE		{ }
	namespace IAMENGINE		{ }
	namespace IAMDBB		{ }
	namespace IAMMISCED		{ }
	namespace IAMVIEWERS	{ }
	namespace IAMSC			{ }
	namespace IAMGRAPHTOOL	{ }
	namespace KCOM			{ }
	namespace KCORE			{ }
	namespace KINPUT		{ }
	namespace KMATH			{ }
	namespace KNETWORK		{ }
	namespace KPHYSICS		{ }
	namespace KRENDERER 		{ }
	namespace KSERV			{ }
	namespace KSOUND		{ }
	namespace LUAENGINE		{ }
	namespace MAYACORE			{ }
	namespace MAYAUI			{ }
	namespace MAYAPLUI			{ }
	namespace MAYAVIEWER		{ }
	namespace MAYAEXCOMMON		{ }
	namespace MAYAMODELEXP		{ }	
	namespace MAYAANIMINT		{ }
	namespace MAYAANIMEXP		{ }
	namespace WMFC			{ }
	namespace NRG			{ }
	namespace OLDRENDERER		{ }
	namespace QDDS			{ }
	namespace SINT			{ }
	namespace SQLDB			{ }
	namespace STOOLS		{ }
	namespace TEST_RENDERER { }
	namespace TEST_CORE		{ }
	namespace VM			{ }
	namespace XMLTOOLS		{ }
	namespace RUN			{ }
	namespace ANIMATION		{ }
}

// Using declarations of above namespaces
using namespace QDT::M3D;
using namespace QDT::EXPCOMMON;
using namespace QDT::COMTOOLS;
using namespace QDT::EXPTOOLS;
using namespace QDT::IAM4;
using namespace QDT::IADMIN;
using namespace QDT::IAM::APP;
using namespace QDT::IAMLITE;
using namespace QDT::IAMENGINE;
using namespace QDT::IAMDBB;
using namespace QDT::IAMMISCED;
using namespace QDT::IAMVIEWERS;
using namespace QDT::IAMGRAPHTOOL;
using namespace QDT::IAMSC;
using namespace QDT::KCOM;
using namespace QDT::KCORE;
using namespace QDT::KINPUT;
using namespace QDT::KMATH;
using namespace QDT::KNETWORK;
using namespace QDT::KPHYSICS;
using namespace QDT::KRENDERER;
using namespace QDT::KSERV;
using namespace QDT::KSOUND;
using namespace QDT::LUAENGINE;
using namespace QDT::MAYACORE;
using namespace QDT::MAYAUI;
using namespace QDT::MAYAPLUI;
using namespace QDT::MAYAVIEWER;
using namespace QDT::MAYAEXCOMMON;
using namespace QDT::MAYAMODELEXP;
using namespace QDT::MAYAANIMINT;
using namespace QDT::MAYAANIMEXP;
using namespace QDT::WMFC;
using namespace QDT::NRG;
using namespace QDT::OLDRENDERER;
using namespace QDT::QDDS;
using namespace QDT::SINT;
using namespace QDT::SQLDB;
using namespace QDT::STOOLS;
using namespace QDT::TEST_RENDERER;
using namespace QDT::TEST_CORE;
using namespace QDT::VM;
using namespace QDT::XMLTOOLS;
using namespace QDT::RUN;
using namespace QDT::ANIMATION;

// defines for class declaration within a given namespace
#define	DECL_STD(exp)			class exp;
#define DECL_3DENGINE(exp)		namespace QDT { namespace M3D { class exp; } }
#define DECL_EXPCOMMON(exp)		namespace QDT { namespace EXPCOMMON { class exp; } }
#define DECL_COMTOOLS(exp)		namespace QDT { namespace COMTOOLS { class exp; } }
#define DECL_EXPTOOLS(exp)		namespace QDT { namespace EXPTOOLS { class exp; } }
#define DECL_IAM4(exp)			namespace QDT { namespace IAM4 { class exp; } }
#define DECL_IADMIN(exp)		namespace QDT { namespace IADMIN { class exp; } }
#define DECL_IAMAPP(exp)		namespace QDT { namespace IAM { namespace APP { class exp; } } }
#define	DECL_IAMLITE(exp)		namespace QDT { namespace IAMLITE { class exp; } }
#define DECL_IAMENGINE(exp)		namespace QDT { namespace IAMENGINE { class exp; } }
#define DECL_IAMDBB(exp)		namespace QDT { namespace IAMDBB { class exp; } }
#define DECL_IAMMISCED(exp)		namespace QDT { namespace IAMMISCED { class exp; } }
#define DECL_IAMVIEWERS(exp)	namespace QDT { namespace IAMVIEWERS { class exp; } }
#define DECL_IAMGRAPHTOOL(exp)	namespace QDT { namespace IAMGRAPHTOOL { class exp; } }
#define DECL_IAMSC(exp)			namespace QDT { namespace IAMSC { class exp; } }
#define DECL_KCOM(exp)			namespace QDT { namespace KCOM { class exp; } }
#define DECL_KCORE(exp)			namespace QDT { namespace KCORE { class exp; } }
#define DECL_KINPUT(exp)		namespace QDT { namespace KINPUT { class exp; } }
#define DECL_KMATH(exp)			namespace QDT { namespace KMATH { class exp; } }
#define DECL_KNETWORK(exp)		namespace QDT { namespace KNETWORK { class exp; } }
#define DECL_KPHYSICS(exp)		namespace QDT { namespace KPHYSICS { class exp; } }
#define DECL_KRENDERER(exp)		namespace QDT { namespace KRENDERER { class exp; } }
#define DECL_KSERV(exp)			namespace QDT { namespace KSERV { class exp; } }
#define DECL_KSOUND(exp)		namespace QDT { namespace KSOUND { class exp; } }
#define DECL_LUAENGINE(exp)		namespace QDT { namespace LUAENGINE { class exp; } }
#define DECL_MAYACORE(exp)		namespace QDT { namespace MAYACORE { class exp; } }
#define DECL_MAYAUI(exp)		namespace QDT { namespace MAYAUI { class exp; } }
#define DECL_MAYAPLUI(exp)		namespace QDT { namespace MAYAPLUI { class exp; } }
#define DECL_MAYAVIEWER(exp)	namespace QDT { namespace MAYAVIEWER { class exp; } }
#define DECL_MAYAEXCOMMON(exp)	namespace QDT { namespace MAYAEXCOMMON { class exp; } }
#define DECL_MAYAMODELEXP(exp)	namespace QDT { namespace MAYAMODELEXP { class exp; } }
#define DECL_MAYAANIMINT(exp)	namespace QDT { namespace MAYAANIMINT { class exp; } }
#define DECL_MAYAANIMEXP(exp)	namespace QDT { namespace MAYAANIMEXP { class exp; } }
#define DECL_WMFC(exp)			namespace QDT { namespace WMFC { class exp; } }
#define DECL_NRG(exp)			namespace QDT { namespace NRG { class exp; } }
#define DECL_OLDRENDERER(exp)	namespace QDT { namespace OLDRENDERER { class exp; } }
#define DECL_QDDS(exp)			namespace QDT { namespace QDDS { class exp; } }
#define DECL_SINT(exp)			namespace QDT { namespace SINT { class exp; } }
#define DECL_SQLDB(exp)			namespace QDT { namespace SQLDB { class exp; } }
#define DECL_STOOLS(exp)		namespace QDT { namespace STOOLS { class exp; } }
#define DECL_TEST_RENDERER(exp)	namespace QDT { namespace TEST_RENDERER { class exp; } }
#define DECL_TEST_CORE(exp)		namespace QDT { namespace TEST_CORE { class exp; } }
#define DECL_VM(exp)			namespace QDT { namespace VM { class exp; } }
#define DECL_XMLTOOLS(exp)		namespace QDT { namespace XMLTOOLS { class exp; } }
#define DECL_RUN(exp)			namespace QDT { namespace RUN { class exp; } }
#define DECL_ANIMATION(exp)		namespace QDT { namespace ANIMATION { class exp; } }

// New defines for inclusion
#if defined (PLATFORM_PS3)
	#define	INCL_3DENGINE(a)		<3dengine/sources/a.h>
	#define	INCL_ANIMATION(a)		<3dengine/sources/Animation/a.h>
	#define	INCL_EXPCOMMON(a)		DONT_USE_ON_PS3
	#define	INCL_COMTOOLS(a)		DONT_USE_ON_PS3
	#define	INCL_EXPTOOLS(a)		DONT_USE_ON_PS3
	#define INCL_IAM4(a)			DONT_USE_ON_PS3
	#define	INCL_IAMADMIN(a)		DONT_USE_ON_PS3
	#define	INCL_IAMAPP(a)			DONT_USE_ON_PS3
	#define	INCL_IAMENGINE(a)		DONT_USE_ON_PS3
	#define	INCL_IAMDBB(a)			DONT_USE_ON_PS3
	#define	INCL_IAMMISCED(a)		DONT_USE_ON_PS3
	#define	INCL_IAMVIEWERS(a)		DONT_USE_ON_PS3
	#define	INCL_IAMGRAPHTOOL(a)	DONT_USE_ON_PS3
	#define	INCL_IAMSC(a)			DONT_USE_ON_PS3
	#define	INCL_IAMLITE(a)			DONT_USE_ON_PS3
	#define	INCL_KCOM(a)			<KernelCom/Sources/a.h>
	#define	INCL_KCORE(a)			<KernelCore/Sources/a.h>
	#define	INCL_KINPUT(a)			<KernelInput/sources/a.h>
	#define	INCL_KMATH(a)			<KernelMath/Sources/a.h>
	#define	INCL_KNETWORK(a)		<KernelNetwork/sources/a.h>
	#define	INCL_KPHYSICS(a)		<KernelPhysics/sources/a.h>
	#define INCL_KRENDERER(a)		<KernelRenderer/Sources/a.h>
	#define	INCL_KSERV(a)			<KernelServ/Sources/a.h>
	#define	INCL_KSOUND(a)			<KernelSound/sources/a.h>
	#define	INCL_LUAENGINE(a)		DONT_USE_ON_PS3
	#define	INCL_MAYACORE(a)		DONT_USE_ON_PS3
	#define	INCL_MAYAUI(a)			DONT_USE_ON_PS3
	#define	INCL_MAYAPLUI(a)			DONT_USE_ON_PS3
	#define	INCL_MAYAVIEWER(a)		DONT_USE_ON_PS3
	#define INCL_MAYAEXCOMMON(a)	DONT_USE_ON_PS3
	#define	INCL_MAYAMODELEXP(a)	DONT_USE_ON_PS3
	#define	INCL_MAYAANIMINT(a)		DONT_USE_ON_PS3
	#define	INCL_MAYAANIMEXP(a)		DONT_USE_ON_PS3
	#define	INCL_MFC_WRAPPER(a)		DONT_USE_ON_PS3
	#define INCL_NRG(a)				<nonregression/Sources/a.h>
	#define INCL_NRGLIB(a)			<nonregressionLib/Sources/a.h>
	#define	INCL_OLDRENDERER(a)		<OldRenderer/sources/a.h>
	#define	INCL_QDDS(a)			DONT_USE_ON_PS3
	#define	INCL_SINT(a)			<ScriptInterface/sources/a.h>
	#define	INCL_SQLDB(a)			<SqlDb/sources/a.h>
	#define	INCL_STOOLS(a)			DONT_USE_ON_PS3
	#define	INCL_TEST_RENDERER(a)	<TestRenderer/sources/a.h>
	#define	INCL_TEST_CORE(a)		<TestCore/sources/a.h>
	#define	INCL_VM(a)				<Vm/Sources/a.h>
	#define	INCL_XMLTOOLS(a)		DONT_USE_ON_PS3
	#define	INCL_RUN(a)			<runtime/sources/a.h>
#else
	#define	INCL_3DENGINE(a)		<3dengine/sources/a.h>
	#define	INCL_ANIMATION(a)		<3dengine/sources/Animation/a.h>
	#define	INCL_EXPCOMMON(a)		<Plug-ins/Common plugin/Sources/a.h>
	#define	INCL_COMTOOLS(a)		<COMTools/Sources/a.h>
	#define	INCL_EXPTOOLS(a)		<ExportTools/Sources/a.h>
	#define INCL_IAM4(a)			<iam4_1/Sources/a.h>
	#define	INCL_IAMADMIN(a)		<IAMAdmin/Sources/a.h>
	#define	INCL_IAMAPP(a)			<iamapp/sources/a.h>
	#define	INCL_IAMLITE(a)			<iamlite/sources/a.h>
	#define	INCL_IAMENGINE(a)		<IAMEngine/sources/a.h>
	#define	INCL_IAMDBB(a)			<IAMDatabaseBox/sources/a.h>
	#define	INCL_IAMMISCED(a)		<IamMiscEditors/Sources/a.h>
	#define	INCL_IAMVIEWERS(a)		<IamViewers/Sources/a.h>
	#define	INCL_IAMGRAPHTOOL(a)	<IamGraphTool/Sources/a.h>
	#define	INCL_IAMSC(a)			<IAMSourceControl/sources/a.h>
	#define	INCL_KCOM(a)			<KernelCom/Sources/a.h>
	#define	INCL_KCORE(a)			<KernelCore/Sources/a.h>
	#define	INCL_KINPUT(a)			<KernelInput/sources/a.h>
	#define	INCL_KMATH(a)			<KernelMath/Sources/a.h>
	#define	INCL_KNETWORK(a)		<KernelNetwork/sources/a.h>
	#define	INCL_KPHYSICS(a)		<KernelPhysics/sources/a.h>
	#define INCL_KRENDERER(a)		<KernelRenderer/Sources/a.h>
	#define	INCL_KSERV(a)			<KernelServ/Sources/a.h>
	#define	INCL_KSOUND(a)			<KernelSound/sources/a.h>
	#define	INCL_LUAENGINE(a)		<LUAEngine/Sources/a.h>
	#define	INCL_MAYACORE(a)		<Plug-ins/MayaCore/Sources/a.h>
	#define	INCL_MAYAUI(a)			<Plug-ins/MayaUI/Sources/a.h>
	#define	INCL_MAYAPLUI(a)		<Plug-ins/MayaPLUI/Sources/a.h>
	#define	INCL_MAYAVIEWER(a)		<Plug-ins/MayaViewer/Sources/a.h>
	#define INCL_MAYAEXCOMMON(a)	<Plug-ins/Common plugin/Sources/a.h>
	#define	INCL_MAYAMODELEXP(a)	<Plug-ins/MayaModelingExport/Sources/a.h>
	#define	INCL_MAYAANIMINT(a)		<Plug-ins/MayaAnimInterface/Sources/a.h>
	#define	INCL_MAYAANIMEXP(a)		<Plug-ins/MayaAnimExport/Sources/a.h>			
	#define	INCL_MFC_WRAPPER(a)		<mfc_wrapper/sources/a.h>
	#define INCL_NRG(a)				<NonRegression/Sources/a.h>
	#define INCL_NRGLIB(a)			<nonregressionLib/Sources/a.h>
	#define	INCL_OLDRENDERER(a)		<OldRenderer/sources/a.h>
	#define	INCL_QDDS(a)			<QDDataSafe/QDDataSafe Lib/Sources/a.h>
	#define	INCL_SINT(a)			<ScriptInterface/sources/a.h>
	#define	INCL_SQLDB(a)			<SqlDb/sources/a.h>
	#define	INCL_STOOLS(a)			<StatTools/sources/a.h>
	#define	INCL_TEST_RENDERER(a)	<TestRenderer/sources/a.h>
	#define	INCL_TEST_CORE(a)		<TestCore/sources/a.h>
	#define	INCL_VM(a)				<Vm/Sources/a.h>
	#define	INCL_XMLTOOLS(a)		<XmlTools/Sources/a.h>
	#define	INCL_RUN(a)				<runtime/sources/a.h>
#endif


// Old defines for inclusion
#define	INC_KRNMATH(a)			DONT_USE_OLD_INCLUDES
#define INL_KRNMATH(a)			DONT_USE_OLD_INCLUDES
#define	DEC_KRNMATH(a)			DONT_USE_OLD_INCLUDES
#define	INC_SCPTINT(a)			DONT_USE_OLD_INCLUDES
#define	DEC_SCPTINT(a)			DONT_USE_OLD_INCLUDES
#define	INL_SCPTINT(a)			DONT_USE_OLD_INCLUDES
#define	INC_3DENGINE(a)			DONT_USE_OLD_INCLUDES_FOR_3DENGINE
#define	DEC_3DENGINE(a)			<3dengine/sources/a/Declaration.h>
#define	INL_3DENGINE(a)			DONT_USE_OLD_INCLUDES_FOR_3DENGINE
#define	INC_EXPCOMMON(a)		<Plug-ins/Common plugin/Sources/a/facade.h>
#define	DEC_EXPCOMMON(a)		<Plug-ins/Common plugin/Sources/a/Declaration.h>
#define	INL_EXPCOMMON(a)		<Plug-ins/Common plugin/Sources/a/Inline.h>
#define	INC_COMTOOLS(a)			<COMTools/Sources/a/Facade.h>
#define INL_COMTOOLS(a)			<COMTools/Sources/a/Inline.h>
#define	DEC_COMTOOLS(a)			<COMTools/Sources/a/Declaration.h>
#define	INC_EXPTOOLS(a)			<ExportTools/Sources/a/Facade.h>
#define INL_EXPTOOLS(a)			<ExportTools/Sources/a/Inline.h>
#define	DEC_EXPTOOLS(a)			<ExportTools/Sources/a/Declaration.h>
#define INC_IAM4(a)				<iam4_1/Sources/a/Facade.h>
#define DEC_IAM4(a)				<iam4_1/Sources/a/Declaration.h>
#define	INC_IAMADMIN(a)			<IAMAdmin/Sources/a/Facade.h>
#define INL_IAMADMIN(a)			<IAMAdmin/Sources/a/Inline.h>
#define	DEC_IAMADMIN(a)			<IAMAdmin/Sources/a/Declaration.h>
#define	INC_IAMAPP(a)			<iamapp/sources/a/facade.h>
#define	DEC_IAMAPP(a)			<iamapp/sources/a/Declaration.h>
#define	INC_IAMDBB(a)			<IAMDatabaseBox/sources/a/facade.h>
#define	DEC_IAMDBB(a)			<IAMDatabaseBox/sources/a/Declaration.h>
#define	INL_IAMDBB(a)			<IAMDatabaseBox/sources/a/Inline.h>
#define	INC_IAMENGINE(a)		<IAMEngine/sources/a/facade.h>
#define	DEC_IAMENGINE(a)		<IAMEngine/sources/a/Declaration.h>
#define	INL_IAMENGINE(a)		<IAMEngine/sources/a/Inline.h>
#define	INC_IAMMISCED(a)		<IamMiscEditors/Sources/a/Facade.h>
#define INL_IAMMISCED(a)		<IamMiscEditors/Sources/a/Inline.h>
#define	DEC_IAMMISCED(a)		<IamMiscEditors/Sources/a/Declaration.h>
#define	INC_IAMSC(a)			<IAMSourceControl/sources/a/facade.h>
#define	DEC_IAMSC(a)			<IAMSourceControl/sources/a/Declaration.h>
#define	INL_IAMSC(a)			<IAMSourceControl/sources/a/Inline.h>
#define	INC_KRNCOM(a)			<KernelCom/Sources/a/Facade.h>
#define INL_KRNCOM(a)			<KernelCom/Sources/a/Inline.h>
#define	DEC_KRNCOM(a)			<KernelCom/Sources/a/Declaration.h>
#define	INC_KRNCORE(a)			<KernelCore/Sources/a/Facade.h>
#define INL_KRNCORE(a)			<KernelCore/Sources/a/Inline.h>
#define	DEC_KRNCORE(a)			<KernelCore/Sources/a/Declaration.h>
#define	DEC_KRNINPUT(a)			<KernelInput/sources/a/Declaration.h>
#define	INC_KRNINPUT(a)			<KernelInput/sources/a/facade.h>
#define	INL_KRNINPUT(a)			<KernelInput/sources/a/Inline.h>
#define	DEC_KRNNETWORK(a)		<KernelNetwork/sources/a/Declaration.h>
#define	INC_KRNNETWORK(a)		<KernelNetwork/sources/a/facade.h>
#define	INL_KRNNETWORK(a)		<KernelNetwork/sources/a/Inline.h>
#define	DEC_KRNPHYSICS(a)		<KernelPhysics/sources/a/Declaration.h>
#define	INC_KRNPHYSICS(a)		<KernelPhysics/sources/a/facade.h>
#define	INL_KRNPHYSICS(a)		<KernelPhysics/sources/a/Inline.h>
#define	INC_KRNRENDERER(a)		<KernelRenderer/Sources/a/Facade.h>
#define INL_KRNRENDERER(a)		<KernelRenderer/Sources/a/Inline.h>
#define	DEC_KRNRENDERER(a)		<KernelRenderer/Sources/a/Declaration.h>
#define	INC_KRNSERV(a)			<KernelServ/Sources/a/Facade.h>
#define INL_KRNSERV(a)			<KernelServ/Sources/a/Inline.h>
#define	DEC_KRNSERV(a)			<KernelServ/Sources/a/Declaration.h>
#define	DEC_KRNSOUND(a)			<KernelSound/sources/a/Declaration.h>
#define	INC_KRNSOUND(a)			<KernelSound/sources/a/facade.h>
#define	INL_KRNSOUND(a)			<KernelSound/sources/a/Inline.h>
#define	INC_LUAENGINE(a)		<LUAEngine/Sources/a/Facade.h>
#define	DEC_LUAENGINE(a)		<LUAEngine/Sources/a/Declaration.h>
#define	INL_LUAENGINE(a)		<LUAEngine/Sources/a/Inline.inl>
#define	INC_MAYAANIMEXP(a)		<Plug-ins/MayaAnimExport/Sources/a/facade.h>
#define	DEC_MAYAANIMEXP(a)		<Plug-ins/MayaAnimExport/Sources/a/Declaration.h>
#define	INL_MAYAANIMEXP(a)		<Plug-ins/MayaAnimExport/Sources/a/Inline.h>
#define	INC_MAYAANIMINT(a)		<Plug-ins/MayaAnimInterface/Sources/a/facade.h>
#define	DEC_MAYAANIMINT(a)		<Plug-ins/MayaAnimInterface/Sources/a/Declaration.h>
#define	INL_MAYAANIMINT(a)		<Plug-ins/MayaAnimInterface/Sources/a/Inline.h>
#define	INC_MAYAMODELEXP(a)		<Plug-ins/MayaModelingExport/Sources/a/facade.h>
#define	DEC_MAYAMODELEXP(a)		<Plug-ins/MayaModelingExport/Sources/a/Declaration.h>
#define	INL_MAYAMODELEXP(a)		<Plug-ins/MayaModelingExport/Sources/a/Inline.h>
#define	INC_MFC_WRAPPER(a)		<mfc_wrapper/sources/a/facade.h>
#define	DEC_MFC_WRAPPER(a)		<mfc_wrapper/sources/a/Declaration.h>
#define	INL_MFC_WRAPPER(a)		<mfc_wrapper/sources/a/Inline.h>
#define INC_NRG(a)				<NonRegression/Sources/a/Facade.h>
#define DEC_NRG(a)				<NonRegression/Sources/a/Declaration.h>
#define	DEC_OLDRENDERER(a)		<OldRenderer/sources/a/Declaration.h>
#define	INC_OLDRENDERER(a)		<OldRenderer/sources/a/facade.h>
#define	INL_OLDRENDERER(a)		<OldRenderer/sources/a/Inline.h>
#define	INC_QDDS(a)				<QDDataSafe/QDDataSafe Lib/Sources/a/Facade.h>
#define INL_QDDS(a)				<QDDataSafe/QDDataSafe Lib/Sources/a/Inline.h>
#define	DEC_QDDS(a)				<QDDataSafe/QDDataSafe Lib/Sources/a/Declaration.h>
#define	DEC_SQLDB(a)			<SqlDb/sources/a/Declaration.h>
#define	INC_SQLDB(a)			<SqlDb/sources/a/facade.h>
#define	INL_SQLDB(a)			<SqlDb/sources/a/Inline.h>
#define	INC_STOOLS(a)			<StatTools/sources/a/facade.h>
#define	DEC_STOOLS(a)			<StatTools/sources/a/Declaration.h>
#define	INL_STOOLS(a)			<StatTools/sources/a/Inline.h>
#define	INC_TEST_RENDERER(a)	<TestRenderer/sources/a/facade.h>
#define	DEC_TEST_RENDERER(a)	<TestRenderer/sources/a/Declaration.h>
#define	INC_TEST_CORE(a)		<TestCore/sources/a/facade.h>
#define	DEC_TEST_CORE(a)		<TestCore/sources/a/Declaration.h>
#define INC_TEST_NETWORK(a) <TestNetwork/sources/a.h>
#define	INC_VM(a)				<Vm/Sources/a/Facade.h>
#define	DEC_VM(a)				<Vm/Sources/a/Declaration.h>
#define	INL_VM(a)				<Vm/Sources/a.inl>
#define	INC_XMLTOOLS(a)			<XmlTools/Sources/a/Facade.h>
#define	DEC_XMLTOOLS(a)			<XmlTools/Sources/a/Declaration.h>
#define	INL_XMLTOOLS(a)			<XmlTools/Sources/a.inl>
#define	INC_RUNTIME(a)			<runtime/sources/a/facade.h>
#define	DEC_RUNTIME(a)			<runtime/sources/a/Declaration.h>

// Place here all ABSOLUTELY GENERIC includes ONLY if it's needed in every single file

// all asserting code for example is defined on QDT_DEBUG, QDT_DEBUG is defined in fastdebug build, 
// we need to define it while in _DEBUG too
#ifdef _DEBUG
	#ifndef QDT_DEBUG
		#define QDT_DEBUG
	#endif
#endif

#include INCL_KCORE(Mem/MemConfig)
#include INCL_KCORE(Core/NewDelete)
#include INCL_KCORE(Core/Types)
#include INCL_KCORE(Core/Macros)
#include INCL_KCORE(Core/Assert)
#include INCL_KCORE(Core/Constants)
#include INCL_KCORE(Core/Message)
//#include INCL_KCORE(Core/DisableWarnings)
#include INCL_KCORE(Mem/MemInterface)

// default LIB id config
#ifndef QDT_LIB_ID
#define QDT_LIB_ID MEM_LIBID_DEFAULT
#endif

#endif


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
