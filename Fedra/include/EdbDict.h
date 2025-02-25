/********************************************************************
* EdbDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error EdbDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern G__DLLEXPORT int G__cpp_dllrevEdbDict();
extern G__DLLEXPORT void G__set_cpp_environmentEdbDict();
extern G__DLLEXPORT void G__cpp_setup_tagtableEdbDict();
extern G__DLLEXPORT void G__cpp_setup_inheritanceEdbDict();
extern G__DLLEXPORT void G__cpp_setup_typetableEdbDict();
extern G__DLLEXPORT void G__cpp_setup_memvarEdbDict();
extern G__DLLEXPORT void G__cpp_setup_globalEdbDict();
extern G__DLLEXPORT void G__cpp_setup_memfuncEdbDict();
extern G__DLLEXPORT void G__cpp_setup_funcEdbDict();
extern G__DLLEXPORT void G__cpp_setupEdbDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "EdbLog.h"
#include "EdbCluster.h"
#include "EdbView.h"
#include "EdbRunHeader.h"
#include "EdbRun.h"
#include "EdbImage.h"
#include "EdbFrame.h"
#include "EdbSegment.h"
#include "EdbStage.h"
#include "EdbAffine.h"
#include "EdbFiducial.h"
#include "EdbVirtual.h"
#include "EdbPrediction.h"
#include "EdbOnline.h"

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__EdbDictLN_TClass;
extern G__linked_taginfo G__EdbDictLN_TBuffer;
extern G__linked_taginfo G__EdbDictLN_TMemberInspector;
extern G__linked_taginfo G__EdbDictLN_TObject;
extern G__linked_taginfo G__EdbDictLN_TNamed;
extern G__linked_taginfo G__EdbDictLN_TString;
extern G__linked_taginfo G__EdbDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR;
extern G__linked_taginfo G__EdbDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__EdbDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__EdbDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR;
extern G__linked_taginfo G__EdbDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLconst_iteratorgR;
extern G__linked_taginfo G__EdbDictLN_va_list;
extern G__linked_taginfo G__EdbDictLN_TList;
extern G__linked_taginfo G__EdbDictLN_TObjArray;
extern G__linked_taginfo G__EdbDictLN_TClonesArray;
extern G__linked_taginfo G__EdbDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__EdbDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__EdbDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR;
extern G__linked_taginfo G__EdbDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLconst_iteratorgR;
extern G__linked_taginfo G__EdbDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__EdbDictLN_TTree;
extern G__linked_taginfo G__EdbDictLN_TFile;
extern G__linked_taginfo G__EdbDictLN_TArrayD;
extern G__linked_taginfo G__EdbDictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR;
extern G__linked_taginfo G__EdbDictLN_TNtuple;
extern G__linked_taginfo G__EdbDictLN_TH1F;
extern G__linked_taginfo G__EdbDictLN_TH2F;
extern G__linked_taginfo G__EdbDictLN_EdbAffine2D;
extern G__linked_taginfo G__EdbDictLN_EdbAffine3D;
extern G__linked_taginfo G__EdbDictLN_EdbPoint;
extern G__linked_taginfo G__EdbDictLN_EdbPoint2D;
extern G__linked_taginfo G__EdbDictLN_EdbPoint3D;
extern G__linked_taginfo G__EdbDictLN_EdbAngle2D;
extern G__linked_taginfo G__EdbDictLN_EdbTrack2D;
extern G__linked_taginfo G__EdbDictLN_EdbPointsBox2D;
extern G__linked_taginfo G__EdbDictLN_EdbPointsBox3D;
extern G__linked_taginfo G__EdbDictLN_EdbCluster;
extern G__linked_taginfo G__EdbDictLN_EdbClustersBox;
extern G__linked_taginfo G__EdbDictLN_TPolyMarker3D;
extern G__linked_taginfo G__EdbDictLN_TArrayF;
extern G__linked_taginfo G__EdbDictLN_EdbSeg3D;
extern G__linked_taginfo G__EdbDictLN_EdbSegment;
extern G__linked_taginfo G__EdbDictLN_EdbTrack;
extern G__linked_taginfo G__EdbDictLN_EdbFrame;
extern G__linked_taginfo G__EdbDictLN_EdbViewHeader;
extern G__linked_taginfo G__EdbDictLN_EdbView;
extern G__linked_taginfo G__EdbDictLN_TDatime;
extern G__linked_taginfo G__EdbDictLN_EdbCCD;
extern G__linked_taginfo G__EdbDictLN_EdbObjective;
extern G__linked_taginfo G__EdbDictLN_EdbStage;
extern G__linked_taginfo G__EdbDictLN_EdbMark;
extern G__linked_taginfo G__EdbDictLN_EdbMarksBox;
extern G__linked_taginfo G__EdbDictLN_EdbMarksSet;
extern G__linked_taginfo G__EdbDictLN_EdbArea;
extern G__linked_taginfo G__EdbDictLN_EdbPlate;
extern G__linked_taginfo G__EdbDictLN_EdbCamera;
extern G__linked_taginfo G__EdbDictLN_EdbRunHeader;
extern G__linked_taginfo G__EdbDictLN_TArrayC;
extern G__linked_taginfo G__EdbDictLN_EdbPredictionDC;
extern G__linked_taginfo G__EdbDictLN_EdbPredictionsBox;
extern G__linked_taginfo G__EdbDictLN_AlignmentParView;
extern G__linked_taginfo G__EdbDictLN_AlignmentParFrame;
extern G__linked_taginfo G__EdbDictLN_EdbRun;
extern G__linked_taginfo G__EdbDictLN_EdbImage;

/* STUB derived class for protected member access */
