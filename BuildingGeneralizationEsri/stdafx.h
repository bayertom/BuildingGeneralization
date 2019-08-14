// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Some CString constructors will be explicit

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

using std::string;
using namespace ATL;


#pragma warning(push)
#pragma warning(disable : 4192) //disables warning: automatically excluding 'name' while importing type library 'library'

//Add rename("XMLSerializer", "ESRI_XMLSerializer") to the end of the first line;
//Run VS2017 as administrator
//VS2017: Settings > Linker > General > Register Output and set the value to "No". 

//import esriSystem.olb
#import "libid:5E1F7BC3-67C5-4AEE-8EC6-C4B73AAC42ED" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "VARTYPE") rename("XMLSerializer", "ESRI_XMLSerializer")
	
//import esriSystemUI.olb
#import "libid:4ECCA6E2-B16B-4ACA-BD17-E74CAE4C150A" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_HANDLE", "OLE_COLOR")

//import esriFramework.olb
#import "libid:866AE5D3-530C-11D2-A2BD-0000F8774FB5" raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR")

//import esriGeometry.olb
#import "libid:C4B094C2-FF32-4FA1-ABCB-7820F8D6FB68" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_HANDLE", "OLE_COLOR") rename("ISegment", "esriISegment")

//import esriDisplay.olb
#import "libid:59FCCD31-434C-4017-BDEF-DB4B7EDC9CE0" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_HANDLE", "OLE_COLOR")

//import esriGeoDatabase.olb
#import "libid:0475BDB1-E5B2-4CA2-9127-B4B1683E70C2" raw_interfaces_only raw_native_types no_namespace named_guids

//import esriCarto.olb
#import "libid:45AC68FF-DEFF-4884-B3A9-7D882EDCAEF1" raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR")

//import esriControls.olb
#import "libid:033364CA-47F9-4251-98A5-C88CD8D3C808" raw_interfaces_only raw_native_types no_namespace named_guids

//import esriEditor.olb
#import "libid:4C896230-7F1A-11D2-8509-0000F875B9C6" raw_interfaces_only, raw_native_types, no_namespace, named_guids

//import esriEditorExt.olb
#import "libid:52AE85C8-0A79-490E-A5E0-EAE86D8ACA6B" raw_interfaces_only, raw_native_types, no_namespace, named_guids

// import GeodatabaseUI.olb
#import "libid:4A9C9ED7-F7DB-4614-B480-A5D265C961FC" raw_interfaces_only, raw_native_types, no_namespace, named_guids

// import ArcMapUI.olb

#import "libid:40499F24-596F-45D2-ACE1-A251E2990017" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "C:\Program Files (x86)\ArcGIS\Desktop10.6\com\esriSystemUI.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, auto_search

#pragma warning(pop)
